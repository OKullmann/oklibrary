// Oliver Kullmann, 1.7.2020 (Swansea)
/* Copyright 2020 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* The board, including ucp, for the 2-sweep algorithm

TODOS:

2. Dimensions for optimising the code:
   - DONE It is enough to
       add a new data-member
mutable sizet curri; // current branching-row
       (after open), add the line
curri = nearest_centre(lower, upper);
       to the constructor, change the first line of cbr() to that line,
       and delete the first row of set_cbr().
   - On ltok2 this impairs runtimes for N=16, and improves for N=17.
   - Our main machine is the server, so times need to be determined there.
     And here runtimes are worse for both N=16,17.
   - Thus there seems no point in persuing this option (so OZ might try):
     the direct computation of nearest_centre is "too fast" to be improved.
   - The storing of current branching-row (see mutable sizet curri above)
     is implemented in 0.18.19. In fact, 0.18.19 is 0.18.14 with this additional feature.
     However, it impares server compared with 0.18.14.
   - On amd1, 0.18.19 is slightly better than 0.18.14 on N=16, while on N=17 the situation
     is vice versa.
 (c) DONE (it seems we should consider this as the basis)
     Unrolling the loop in ucp into two loops (which might help the compiler
     to better unroll the loops).
 (d) Updating lower/upper after each loop, or only once.
 (e) DONE Introducing variables bottom and top, so that the two loops run from
     bottom to lower, and upper to top.
      - It seems that bottom and top can be updated in two ways.
        In the first one they are updated only in the constructor of
        DoubleSweep. It is cheap and it will lead to improvement if on the
        symmetry breaking step either the first or the last row is assigned
        (our case).
      - In this way only one or two checks are saved (though permanently),
        at the expense of a more complicated loop-structure, so performance
        might even impair; but one needs to try it out.
      - According to the second variant, both bottom and top are updated not
        only in the constructor, but also in set_cbr() and ucp().
      - It seems that this update is quite expensive, so as a result the
        peformance might decrease (as always).
        If fact, we move from the middle row to the
        first and the last rows, so updating of bottom and top in set_cbr()
        and ucp() might happen quite rarely.
      - Both version need to be explored (as always -- only experience tells!).
      - 0.18.15 realises the cheap version, and degrades performance.
      - 0.18.16 tries the expensive form (since we are now on that path).
        If not succesful, 0.18.17 likely should remove bottom,top (if there
        are no other ideas).
      - 0.18.16 seems to impair most machines (especially ltok, but that's the
        least important machine), but, surprisingly, improves server noticeable
        (and that's the most important machine). Can one explain this?
      - In 0.18.16 bottom is updated in set_cbr() and ucp() as follows:
while (bottom < lower and b[bottom]) ++bottom;
        When lower becomes sizet(-1), then bottom continues to increment because
        in this case (bottom < lower) is always true.
        According to experiments, e.g. on N=16, in some cases bottom's final value is 14.
        Since this updating is redundand, maybe it would be better to stop updating
        bottom as soon as lower becomes sizet(-1).
      - Since bottom,top impare most machines, in 0.18.19 they are removed.
      - bottom,top was beneficial on the server (especially 0.18.16 on N=17), herefore 
        in 0.18.20 it is again retained.
      - 0.18.20 is in fact 0.18.18 without wrapping lower to sizet(-1)).
 (f) Do not call ucp() when it can not return true.
      - Placing a queen on a board's square can close from 1 up to 3 squares of any other
        row/column.
      - ucp() returns true in 3 cases: (i) N-1 squares of some unchecked row are closed;
        (ii) N squares of some unchecked row are closed; (iii) N squares of any column
        are closed.
      - Therefore, ucp() always returns false if (N-open)*3 < N-1.
      - E.g., for N=16 it make sense to run ucp() for the first time after placing the
        5-th queen.
      - The corresponding check can be added to ucp(). It also can be implemented in
        Backtracking.hpp, in count() and countnr(). In countnr: calculate the number
        of rows that can be safely set without ucp(), run a loop for setting these rows,
        then for the remaining rows run the exsisting loop with ucp().
 (g) Reduce conditional checks in set_cbr()
      - If it is known whether curri is lower or upper in set_cbr(), then
        it might reduce conditional checks. if lower == curri, then it is not
        required to check whether (lower != D::N) and b[lower], because it
        is already true. The same for upper.

3. Investigating the difference between even and odd N
  - It seemed rather clear, that updating lower/upper after each loop performed
    better for even N than for odd N (or at least for N=16 vs N=17).
  - Could be an artifact, but should be investigated.

4. Handling main counters via lookup tables.
  - Every main counter (bottom, lower, upper, and top) can be
    constructed from the bitset b, which in turn reflects the current
    opened/closed rows.
  - Using the standard bitset function to_ulong() one can get from b
    an unsigned long, then all 4 counters can be assigned using bit operations.
  - One can construct a lookup table, where for each 2^N possible
    values of b four corresponding counters will be assigned. An
    unsigned long representaions of b works as an index.
  - The lookup table should be constructed during compilation; C++17
    allows one to easily do it by constructing constexp arrays.
  - All four counters can be removed from the class DoubleSweep -
    instead of them the corresponding values from the lookup table
    are used. As a result, it is not required to update the counters
    via the while loop, that is quite expensive. Additionally,
    four 4-byte variables are removed from the DoubleSweep class.
  - For large N the lookup table is quite large. E.g., for N=25 we
    have 2^25 million values of b, 4 4-byte counters for each value,
    that gives us 512 Mb. 
  - It is possible to maintain two lookup tables instead of one:
    Table 1 correpsonds to the first half of the board and gives the
    values of bottom and lower; Table 2 gives upper and top. In this
    case even for large N the lookup tables are quite small.
    For N=25 Table 1 has size 2^13 (64 Kb), while Table 2 has size 2^12
    (32 Kb). However, this is not for free - one need to construct two indexes
    from b, and this costs several additional bit operations each time when main
    counters are required.
  - A preliminary implementation of the 2-tables tecnique gives about 11.5 %
    speedup on the server (N=16,17,19) compared with the current version
    (0.18.20). On amd1 the speedup is more modest: from 7 % to 9 %.


*/

#ifndef BOARD_IJj1sO4ZFB
#define BOARD_IJj1sO4ZFB

#include <bitset>
#include <ostream>
#include <type_traits>
#include <vector>
#include <algorithm>

#include "Dimensions.hpp"
#include "Statistics.hpp"
#include "Rows.hpp"
#include "ExtRows.hpp"
#include "Tables.hpp"

namespace Board {

  namespace D = Dimensions;


  struct Square { D::sizet i,j; };
  constexpr bool valid(const Square& s) noexcept {
    return s.i < D::N and s.j < D::N;
  }
  std::ostream& operator <<(std::ostream& out, const Square& s) {
    return out << "(" << s.i << "," << s.j << ")";
  }
  typedef std::vector<Square> square_v;
  // Whether the squares of v all valid and non-attacking:
  bool valid(const square_v& v) noexcept {
    if (v.empty()) return true;
    if (v.size() >= D::N) return false;
    for (const Square& s : v) if (not valid(s)) return false;

    std::bitset<D::N> r, c;
    std::bitset<2*D::N-1> da, ad;
    for (const Square& s : v) {
      if (r[s.i]) return false; else r[s.i]=true;
      if (c[s.j]) return false; else c[s.j]=true;
      const D::sizet d = (s.i + (D::N-1)) - s.j;
      if (da[d]) return false; else da[d] = true;
      const D::sizet a = s.i + s.j;
      if (ad[a]) return false; else ad[a] = true;
    }

    return true;
  }
  std::ostream& operator <<(std::ostream& out, const square_v& v) {
    for (const Square& s : v) out << s;
    return out;
  }


  constexpr D::sizet nearest_centre(const D::sizet l, const D::sizet u) noexcept {
    assert(l <= D::N and u <= D::N);
    assert(l != D::N or u != D::N);
    assert(l == D::N or u == D::N or
           (D::N%2==1 and l <= u) or (D::N%2==0 and l < u));
    if (l == D::N) return u;
    else if (u == D::N) return l;
    return l >=  D::N-1 - u ? l : u;
  }
  static_assert(nearest_centre(D::N, D::N-1) == D::N-1);
  static_assert(nearest_centre(0, D::N) == 0);
  static_assert(nearest_centre(0, D::N-1) == 0);


  struct DoubleSweep {
    typedef Rows::Row Row;
  private :
    using sizet = D::sizet;

    std::bitset<D::N> b; // true means queen placed in that row
    sizet open; // number of open rows, <= N
    mutable sizet curri; // current branching-row
    Row closed_columns;
    ExtRows::DADlines dad;


  public :

    DoubleSweep() noexcept = default;

    explicit DoubleSweep(const square_v& v) noexcept :
      b{}, open(D::N), curri(0), closed_columns{}, dad{} {
      assert(valid(v));
      open -= v.size();
      if (open == 0) return;
      for (const Square& s : v) {
        b[s.i] = true;
        const Row r(s.j, false);
        closed_columns |= r;
        dad.add(r, s.i);
      }
      sizet b_mask = b.to_ulong();
      const sizet& lower = Tables::lower(b_mask);
      const sizet& upper = Tables::upper(b_mask);
      curri = nearest_centre(lower, upper);
    }
    bool completed() const noexcept {
      return open == 0;
    }

    Row cbr() const noexcept {
      sizet b_mask = b.to_ulong();
      const sizet& lower = Tables::lower(b_mask);
      const sizet& upper = Tables::upper(b_mask);
      curri = nearest_centre(lower, upper);
      assert(curri < D::N and not b[curri] and open != 0);
      return closed_columns | dad.extract(curri);
    }
    void set_cbr(Row r) noexcept {
      assert(curri < D::N and not b[curri]);
      assert(open >= 2);
      closed_columns |= r;
      dad.add(r, curri);
      b[curri] = true;
      --open;
    }

    friend std::ostream& operator <<(std::ostream& out, const DoubleSweep& B) {
      for (sizet i = D::N; i != 0; --i) out << B.b[i-1] << "\n";
      return out << "closed_columns=" << B.closed_columns << "\n";
    }

    // Perform complete row-ucp and empty-column-detection;
    // returns true if the propagation leads to a decision:
    bool ucp(Statistics::NodeCounts& s) noexcept {
      if (D::N == 1) {s.found_r2s(); return true;}

      for (bool changed = false;;changed = false) {
        using Rows::RS;
        Row open_columns(-1);
        sizet b_mask = (sizet)b.to_ulong();
        const sizet& bottom = Tables::bottom(b_mask);
        const sizet& lower = Tables::lower(b_mask);
        const sizet& upper = Tables::upper(b_mask);
        const sizet& top = Tables::top(b_mask);
        assert(open != 0);
        assert(lower==D::N or lower<upper);
        assert(lower==D::N or bottom<=lower);
        assert(upper==D::N or top>=upper);
        assert(open != 0);
        for (sizet j = bottom; j <= lower; ++j) {
          if (b[j]) continue;
          const Row cur_row = closed_columns | dad.extract(j);
          switch (cur_row.rs()) {
          case RS::empty : s.found_r2u(); return true;
          case RS::unit : { s.found_uc();
            if (--open == 0) {s.found_r2s(); return true;}
            b[j] = true; changed = true;
            closed_columns |= ~cur_row; dad.add(~cur_row, j);
            break; }
          default : open_columns &= cur_row; }
        }
        for (sizet j = upper; j <= top; ++j) {
          if (b[j]) continue;
          const Row cur_row = closed_columns | dad.extract(j);
          switch (cur_row.rs()) {
          case RS::empty : s.found_r2u(); return true;
          case RS::unit : { s.found_uc();
            if (--open == 0) {s.found_r2s(); return true;}
            b[j] = true; changed = true;
            closed_columns |= ~cur_row; dad.add(~cur_row, j);
            break; }
          default : open_columns &= cur_row; }
        }
        if ((~closed_columns & open_columns).any()) {
          s.found_cu(); return true;
        }
        if (not changed) return false;
      }
    }

  };

  static_assert(std::is_trivially_copyable_v<DoubleSweep>);

}

#endif
