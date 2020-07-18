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
    static_assert(Row::valid);
    using sizet = Dimensions::sizet;
    typedef std::bitset<D::N> board_t;
    board_t b; // only indices > curri relevant
    sizet curri; // current bottom-row <= N
    sizet cbi() const noexcept { return curri; }
    void inc() noexcept { ++curri; }
    Row closed_columns;
    typedef ExtRows::DADlines ER;
    ER dad;
    // If not falsified, then the board is amo+alo-consistent.

  public :

    DoubleSweep() noexcept = default;
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
    // returns true if the propagation lead to a decision:
    bool ucp(Statistics::NodeCounts& s) noexcept {
      if (D::N == 1) {s.found_r2s(); return true;}
      assert(closed_columns.count() >= cbi());
      Row old_closed_columns, open_columns, curr;
      inc();
      do {
        // Up-sweep:
        old_closed_columns = closed_columns;
        open_columns.set();
        for (sizet j = cbi(); j != D::N; ++j) {
          if (b[j]) continue;
          using Rows::RS;
          curr = closed_columns | dad.extract(j);
          switch (curr.rs()) {
          case RS::empty : s.found_r2u(); return true;
          case RS::unit : { s.found_uc(); b[j] = true;
            const Row new_unit = ~curr;
            closed_columns |= new_unit; dad.add(new_unit,j);
            break; }
          default : open_columns &= curr; }
        }
        if ((~closed_columns & open_columns).any()) {
          s.found_cu(); return true;
        }
        if (closed_columns == old_closed_columns) break;

        // Down-sweep:
        old_closed_columns = closed_columns;
        if (b[D::N-1]) open_columns.set();
        else open_columns = curr;
        for (sizet j = D::N-2; j != cbi()-1; --j) {
          if (b[j]) continue;
          using Rows::RS;
          curr = closed_columns | dad.extract(j);
          switch (curr.rs()) {
          case RS::empty : s.found_r2u(); return true;
          case RS::unit : { s.found_uc(); b[j] = true;
            const Row new_unit = ~curr;
            closed_columns |= new_unit; dad.add(new_unit,j);
            break; }
          default : open_columns &= curr; }
        }
        if ((~closed_columns & open_columns).any()) {
          s.found_cu(); return true;
        }
      } while (closed_columns != old_closed_columns);

      while (cbi() < D::N and b[curri]) inc();
      if (cbi() == D::N) {s.found_r2s(); return true;}
      assert(cbi() < D::N-1);
      return false;
    }

  };

}

#endif
