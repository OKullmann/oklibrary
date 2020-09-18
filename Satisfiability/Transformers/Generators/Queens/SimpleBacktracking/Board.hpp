// Oliver Kullmann, 1.7.2020 (Swansea)
/* Copyright 2020 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* The board, including ucp, for the 2-sweep algorithm

TODOS:

1. Proper "double sweep"
 - DONE For the choice of the branching-row two indices "lower", "upper" are
   maintained, with "lower" the first open row from the middle downwards,
   and "upper" the first open row from the middle upwards.
 - DONE One of lower, upper is chosen for branching, namely the one closer to
   the middle (in case of a tie, choose lower).
 - DONE
   The case that the lower part or the the upper part has been filled could
   be handled by the special value N.
 - DONE (see below)
   ucp would now consist of two similar loops, for the lower and the upper
   part; possibly always to 0 resp. N, or one could maintain additional
   indices "bottom", "top" (bottom like curri).
 - DONE
   The assumption is that this heuristics results in smaller trees, and that
   this is worth the additional effort.
 - DONE The changes can (first) be kept internally, still using cbr() for returning
   the branching row (no longer "current bottom row", but "current branching
   row").

2. Dimensions for optimising the code:
 (a) Wrapping lower to sizet(-1) (not N):
   - Seems to have slightly worse runtime on the server.
   - One should check whether the implementation is optimal.
 (b) Storing the current branching-row (avoiding recomputation):
   - Given (lower,upper), the branching row is a static function, and thus
     could be pre-computed.
   - One possibility is to use an unordered_map in the constructor of
     DoubleSweep. However calling this map is likely very expensive.
   - Much better should be an array computed at compile-time.
   - Still, experience is that looking up values is expensive (in this very
     time-sensitive context).
   - DONE: made the static computation explicit (to help the compiler).
   - We need to distinguish here between the recursive and the non-recursive
     backtracking: for the recursive form, it is enough to
       add a new data-member
mutable sizet curri; // current branching-row
       (after open), change the first line of cbr() to
curri = nearest_centre(lower, upper);
        and delete the first row of set_cbr().
     However this does not work for the non-recursive form.
     The problem is the initialisation of Stack S, due to undefined
     behaviour: *first* B.cbr() needed to be called, and *then* B needed to be
     copied into the board b.
     Now S is only initialised once, so that should be easy to fix.

 (c) DONE (it seems we should consider this as the basis)
     Unrolling the loop in ucp into two loops (which might help the compiler
     to better unroll the loops).
 (d) Updating lower/upper after each loop, or only once.
 (e) Introducing variables bottom and top, so that the two loops run from
     bottom to lower, and upper to top.
      - It seems that bottom and top can be updated in two ways.
        In the first one they are updated only in the constructor of
        DoubleSweep. It is cheap and it will lead to improvement if on the
        symmetry breaking step either the first or the last row is assigned
        (our case).
      - Otherwise the performance will not change at all.
      - According to the second variant, both bottom and top are updated not
        only in the constructor, but also in set_cbr() and ucp().
        It seems that this update is quite expensive, so as a result the
        peformance can decrease. If fact, we move from the middle row to the
        first and the last rows, so updating of bottom and top in set_cbr()
        and ucp() can happen quite rarely. Is it worth trying both variants,
        or only the first one should be implemented?

3. Investigating the difference between even and odd N
  - It seemed rather clear, that updating lower/upper after each loop performed
    better for even N than for odd N (or at least for N=16 vs N=17).
  - Could be an artifact, but should be investigated.


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
    using sizet = D::sizet;
    assert(l == sizet(-1) or l < D::N);
    assert(u <= D::N);
    assert(l != sizet(-1) or u != D::N);
    assert(l == sizet(-1) or u == D::N or
           (D::N%2==1 and l <= u) or (D::N%2==0 and l < u));
    if (l == sizet(-1)) return u;
    else if (u == D::N) return l;
    return l >=  D::N-1 - u ? l : u;
  }
  static_assert(nearest_centre(D::sizet(-1), D::N-1) == D::N-1);
  static_assert(nearest_centre(0, D::N) == 0);
  static_assert(nearest_centre(0, D::N-1) == 0);


  struct DoubleSweep {
    typedef Rows::Row Row;
  private :
    using sizet = D::sizet;

    std::bitset<D::N> b; // true means queen placed in that row
    sizet lower; // the first open row from the middle downwards <= N
    sizet upper; // the first open row from the middle upwards <= N
    sizet open; // number of open rows, <= N
    Row closed_columns;
    ExtRows::DADlines dad;


  public :

    DoubleSweep() noexcept = default;

    explicit DoubleSweep(const square_v& v) noexcept :
      b{}, lower(D::N%2 == 1 ? D::N/2 : D::N/2-1), upper(lower+1), open(D::N), closed_columns{}, dad{} {
      assert(valid(v));
      open -= v.size();
      if (open == 0) return;
      for (const Square& s : v) {
        b[s.i] = true;
        const Row r(s.j, false);
        closed_columns |= r;
        dad.add(r, s.i);
      }
      while (lower != sizet(-1) and b[lower]) --lower;
      while (upper != D::N and b[upper]) ++upper;
    }
    bool completed() const noexcept {
      return open == 0;
    }

    Row cbr() const noexcept {
      const sizet curri = nearest_centre(lower, upper);
      assert(curri < D::N and not b[curri] and open != 0);
      return closed_columns | dad.extract(curri);
    }
    void set_cbr(Row r) noexcept {
      const sizet curri = nearest_centre(lower, upper);
      assert(curri < D::N and not b[curri]);
      assert(open >= 2);
      closed_columns |= r;
      dad.add(r, curri);
      b[curri] = true;
      --open;
      while (lower != sizet(-1) and b[lower]) --lower;
      while (upper != D::N and b[upper]) ++upper;
    }

    friend std::ostream& operator <<(std::ostream& out, const DoubleSweep& B) {
      for (sizet i = D::N; i != 0; --i) out << B.b[i-1] << "\n";
      out << "lower=" << B.lower << "\n";
      out << "upper=" << B.upper << "\n";
      return out << "closed_columns=" << B.closed_columns << "\n";
    }

    // Perform complete row-ucp and empty-column-detection;
    // returns true if the propagation leads to a decision:
    bool ucp(Statistics::NodeCounts& s) noexcept {
      if (D::N == 1) {s.found_r2s(); return true;}

      for (bool changed = false;;changed = false) {
        using Rows::RS;
        Row open_columns(-1);
        assert(open != 0);
        assert(lower==sizet(-1) or lower<upper);
        if (lower != sizet(-1))
          for (sizet j = 0; j <= lower; ++j) {
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
        for (sizet j = upper; j != D::N; ++j) {
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
        while (lower != sizet(-1) and b[lower]) --lower;
        while (upper != D::N and b[upper]) ++upper;
      }
    }

  };

  static_assert(std::is_trivially_copyable_v<DoubleSweep>);

}

#endif
