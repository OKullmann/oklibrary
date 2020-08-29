// Oliver Kullmann, 1.7.2020 (Swansea)
/* Copyright 2020 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* The board, including ucp, for the 2-sweep algorithm

TODOS:

BASIC

EXTENSIONS

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

namespace Board {

  namespace D = Dimensions;


  struct Square { D::sizet i,j; };
  constexpr bool valid(const Square& s) noexcept {
    return s.i < D::N and s.j < D::N;
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

  struct DoubleSweep {
    typedef Rows::Row Row;
  private :
    using sizet = Dimensions::sizet;
    typedef std::bitset<D::N> board_t; // true means queen placed in that row
    typedef ExtRows::DADlines ER;

    board_t b; // only indices >= curri relevant
    sizet curri; // current bottom-row <= N
    Row closed_columns;
    ER dad;

  public :

    DoubleSweep() noexcept = default;
    // Placing queen in row 0, column j:
    explicit DoubleSweep(const sizet j) noexcept : DoubleSweep({{0,j}}) {}

    explicit DoubleSweep(const square_v& v) noexcept : b{}, curri(0), closed_columns{}, dad{} {
      assert(valid(v));
      for (const Square& s : v) {
        b[s.i] = true;
        const Row r(s.j, false);
        closed_columns |= r;
        dad.add(r, s.i);
      }
      while (curri < D::N and b[curri]) ++curri;
    }
    bool completed() const noexcept {
      assert(curri <= D::N);
      return curri == D::N;
    }

    Row cbr() const noexcept {
      assert(curri < D::N and not b[curri]);
      return closed_columns | dad.extract(curri);
    }
    void set_cbr(Row r) noexcept {
      assert(curri < D::N and not b[curri]);
      closed_columns |= r;
      dad.add(r, curri);
      ++curri;
      while (curri < D::N and b[curri]) ++curri;
    }

    friend std::ostream& operator <<(std::ostream& out, const DoubleSweep& B) {
      for (sizet i = D::N; i != 0; --i) out << B.b[i-1] << "\n";
      out << "curri=" << B.curri << "\n";
      return out << "closed_columns=" << B.closed_columns << "\n";
    }

    // Perform complete row-ucp and empty-column-detection;
    // returns true if the propagation leads to a decision:
    bool ucp(Statistics::NodeCounts& s) noexcept {
      if (D::N == 1) {s.found_r2s(); return true;}
      assert(closed_columns.count() >= curri);
      assert(curri < D::N);
      assert(not b[curri]); // possibly curri is empty or unit (after setting a new bottom row)

      for (bool changed = false;;changed = false) {
        using Rows::RS;
        Row open_columns(-1);
        assert(curri < D::N);
        assert(not b[curri]);
        for (sizet j = curri; j != D::N; ++j) {
          if (b[j]) continue;
          const Row cur_row = closed_columns | dad.extract(j);
          switch (cur_row.rs()) {
          case RS::empty : s.found_r2u(); return true;
          case RS::unit : { s.found_uc(); b[j] = true; changed = true;
            closed_columns |= ~cur_row; dad.add(~cur_row, j);
            break; }
          default : open_columns &= cur_row; }
        }
        if ((~closed_columns & open_columns).any()) {
          s.found_cu(); return true;
        }
        if (not changed) {
          assert(curri < D::N-1);
          return false;
        }
        while (curri < D::N and b[curri]) ++curri;
        if (curri == D::N) {s.found_r2s(); return true;}
      }
    }


  };

  static_assert(std::is_trivially_copyable_v<DoubleSweep>);

}

#endif
