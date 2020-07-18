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

namespace Board {

  namespace D = Dimensions;


  template <class R>
  struct DoubleSweep {
  private :
    static_assert(R::valid);
    using size_t = Dimensions::size_t;
    typedef std::bitset<D::N> board_t;
    board_t b; // only indices > curri relevant
    size_t curri; // current bottom-row <= N
    size_t cbi() const noexcept { return curri; }
    void inc() noexcept { ++curri; }
    R closed_columns;
    typedef ExtRows::DADlines<R> ER;
    static_assert(ER::valid);
    static_assert(std::is_trivially_copyable_v<ER>);
    ER dad;
    bool decided_;
    // If not falsified, then the board is amo+alo-consistent.

  public :

    DoubleSweep() noexcept = default;
    DoubleSweep(const size_t i) noexcept : b{}, curri(0), closed_columns(R(i,false)), dad(closed_columns, curri), decided_(false) {}

    R cbr() const noexcept {
      assert(curri < D::N and not b[curri]);
      return closed_columns | dad.extract(curri);
    }
    void set_cbr(R r) noexcept {
      assert(curri < D::N and not b[curri]);
      closed_columns |= r;
      dad.add(r, curri);
    }

    bool decided() const noexcept { return decided_; }

    friend std::ostream& operator <<(std::ostream& out, const DoubleSweep& B) {
      for (size_t i = D::N; i != 0; --i) out << B.b[i-1] << "\n";
      out << "curri=" << B.curri << ", decided=" << B.decided() << "\n";
      return out << "closed_columns=" << B.closed_columns << "\n";
    }

    // Propagate the single queen which is set in the current bottom-row:
    void ucp(Statistics::NodeCounts& s) noexcept {
      if (D::N == 1) {s.found_r2s(); decided_ = true; return;}
      assert(not decided());
      assert(closed_columns.count() >= cbi());
      R old_closed_columns, open_columns, curr;
      inc();
      do {
        // Up-sweep:
        old_closed_columns = closed_columns;
        open_columns.set();
        for (size_t j = cbi(); j != D::N; ++j) {
          if (b[j]) continue;
          using Rows::RS;
          curr = closed_columns | dad.extract(j);
          switch (curr.rs()) {
          case RS::empty : s.found_r2u(); decided_ = true; return;
          case RS::unit : { s.found_uc(); b[j] = true;
            const R new_unit = ~curr;
            closed_columns |= new_unit; dad.add(new_unit,j);
            break; }
          default : open_columns &= curr; }
        }
        if ((~closed_columns & open_columns).any()) {
          s.found_cu(); decided_ = true; return;
        }
        if (closed_columns == old_closed_columns) break;

        // Down-sweep:
        old_closed_columns = closed_columns;
        if (b[D::N-1]) open_columns.set();
        else open_columns = curr;
        for (size_t j = D::N-2; j != cbi()-1; --j) {
          if (b[j]) continue;
          using Rows::RS;
          curr = closed_columns | dad.extract(j);
          switch (curr.rs()) {
          case RS::empty : s.found_r2u(); decided_ = true; return;
          case RS::unit : { s.found_uc(); b[j] = true;
            const R new_unit = ~curr; curr.reset();
            closed_columns |= new_unit; dad.add(new_unit,j);
            break; }
          default : open_columns &= curr; }
        }
        if ((~closed_columns & open_columns).any()) {
          s.found_cu(); decided_ = true; return;
        }
      } while (closed_columns != old_closed_columns);

      while (cbi() < D::N and b[curri]) inc();
      if (cbi() == D::N) {s.found_r2s(); decided_ = true; return;}
      assert(cbi() < D::N-1);
    }

  };

}

#endif
