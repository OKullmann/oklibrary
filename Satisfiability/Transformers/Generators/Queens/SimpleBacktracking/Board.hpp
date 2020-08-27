// Oliver Kullmann, 1.7.2020 (Swansea)
/* Copyright 2020 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* The board, including ucp, for the 2-sweep algorithm

TODOS:

BASIC

1. Can the two sweeps of ucp be unified (nicely)?

EXTENSIONS

*/

#ifndef BOARD_IJj1sO4ZFB
#define BOARD_IJj1sO4ZFB

#include <bitset>
#include <ostream>
#include <type_traits>

#include "Dimensions.hpp"
#include "Statistics.hpp"
#include "Rows.hpp"

namespace Board {

  namespace D = Dimensions;


  struct DoubleSweep {
    typedef Rows::Row Row;
  private :
    using sizet = Dimensions::sizet;
    typedef std::bitset<D::N> board_t;
    typedef ExtRows::DADlines ER;

    board_t b; // only indices > curri relevant
    sizet curri; // current bottom-row <= N
    Row closed_columns;
    ER dad;

  public :

    DoubleSweep() noexcept = default;
    // Placing queen in row 0, column i:
    DoubleSweep(const sizet i) noexcept : b{}, curri(0), closed_columns(Row(i,false)), dad(closed_columns, curri) {}

    Row cbr() const noexcept {
      assert(curri < D::N and not b[curri]);
      return closed_columns | dad.extract(curri);
    }
    void set_cbr(Row r) noexcept {
      assert(curri < D::N and not b[curri]);
      closed_columns |= r;
      dad.add(r, curri);
    }

    friend std::ostream& operator <<(std::ostream& out, const DoubleSweep& B) {
      for (sizet i = D::N; i != 0; --i) out << B.b[i-1] << "\n";
      out << "curri=" << B.curri << "\n";
      return out << "closed_columns=" << B.closed_columns << "\n";
    }

    // Propagate the single queen which is set in the current bottom-row;
    // returns true if the propagation leads to a decision:
    bool ucp(Statistics::NodeCounts& s) noexcept {
      if (D::N == 1) {s.found_r2s(); return true;}
      assert(closed_columns.count() >= curri);
      ++curri;
      assert(curri < D::N);

      for (sizet last;;) {
        // Up-sweep:
        using Rows::RS;
        last = 0;
        {Row open_columns(-1);
         for (sizet j = curri; j != D::N; ++j) {
           if (b[j]) continue;
           const Row cur_row = closed_columns | dad.extract(j);
           switch (cur_row.rs()) {
           case RS::empty : s.found_r2u(); return true;
           case RS::unit : { s.found_uc(); b[j] = true; last = j;
             closed_columns |= ~cur_row; dad.add(~cur_row, j);
             break; }
           default : open_columns &= cur_row; }
         }
         if ((~closed_columns & open_columns).any()) {
           s.found_cu(); return true;
         }
         if (last == 0) {
           while (curri < D::N and b[curri]) ++curri;
           if (curri == D::N) {s.found_r2s(); return true;}
           break;
         }
        }

        // Down-sweep:
        Row open_columns(-1);
        for (sizet j = D::N-1; j != last; --j)
          if (not b[j]) open_columns &= closed_columns | dad.extract(j);
        const Row old_closed_columns = closed_columns;
        for (sizet j = last-1; j != curri-1; --j) {
          if (b[j]) continue;
          const Row cur_row = closed_columns | dad.extract(j);
          switch (cur_row.rs()) {
          case RS::empty : s.found_r2u(); return true;
          case RS::unit : { s.found_uc(); b[j] = true;
            closed_columns |= ~cur_row; dad.add(~cur_row, j);
            break; }
          default : open_columns &= cur_row; }
        }
        if ((~closed_columns & open_columns).any()) {
          s.found_cu(); return true;
        }
        while (curri < D::N and b[curri]) ++curri;
        if (curri == D::N) {s.found_r2s(); return true;}
        if (old_closed_columns == closed_columns) break;
      };

      assert(curri < D::N-1);
      return false;
    }

  };

  static_assert(std::is_trivially_copyable_v<DoubleSweep>);

}

#endif
