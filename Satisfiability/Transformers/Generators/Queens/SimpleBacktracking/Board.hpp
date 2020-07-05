// Oliver Kullmann, 1.7.2020 (Swansea)
/* Copyright 2020 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* The board, including ucp, for the 2-sweep algorithm

TODOS:

1. Can the two sweeps of ucp be unified (nicely)?

*/

#ifndef BOARD_IJj1sO4ZFB
#define BOARD_IJj1sO4ZFB

#include <array>
#include <ostream>
#include <type_traits>

#include "Dimensions.hpp"
#include "Statistics.hpp"

namespace Board {

  namespace D = Dimensions;


  template <class R>
  struct DoubleSweep {
  private :
    using size_t = Dimensions::size_t;
    typedef std::array<R,D::N> board_t;
    board_t b;
    size_t i; // current bottom-row, i <= N
    size_t cbi() const noexcept { return i; }
    void inc() noexcept { ++i; }
    R closed_columns;
    bool falsified_;
    // If not falsified, then the board is amo+alo-consistent, assuming that
    // all-0-rows mean rows with placed queen.
    void set_falsified() noexcept { falsified_ = true; }

  public :

    DoubleSweep() = default;
    DoubleSweep(const size_t i) noexcept : i(0), closed_columns(R(i,false)), falsified_(false) {
      b.fill(closed_columns);
    }

    const R& cbr() const noexcept { return b[i]; }
    void set_cbr(R r) noexcept { b[i] = r; closed_columns |= r; }

    bool falsified() const noexcept { return falsified_; }
    bool satisfied() const noexcept { return not falsified_ and i >= D::N-1; }

    friend std::ostream& operator <<(std::ostream& out, const DoubleSweep& B) {
      for (size_t i = D::N; i != 0; --i) out << B.b[i-1] << "\n";
      out << "i=" << B.i << ", falsified=" << B.falsified() << "\n";
      return out << "closed_columns=" << B.closed_columns << "\n";
    }

    // Propagate the single queen which is set in the current bottom-row:
    template <template <class> class ExtR>
    void ucp(Statistics::NodeCounts& s) noexcept {
      if (D::N == 1) {s.found_r2s(); return;}
      typedef ExtR<R> ER;
      static_assert(std::is_trivially_copyable_v<ER>);
      assert(not falsified());
      assert(not satisfied());
      assert(closed_columns.count() >= cbi());
      R units = cbr(), old_units;
      inc();
      ER diag(units), antidiag = diag;
      R open_columns;
      do {
        // Up-sweep:
        old_units = units;
        open_columns.set();
        for (size_t j = cbi(); j != D::N; ++j) {
          diag.left(); antidiag.right();
          R& curr(b[j]);
          if (curr.none()) continue;
          using Rows::RS;
          assert(curr.rs() != RS::empty);
          curr |= units | diag | antidiag;
          switch (curr.rs()) {
          case RS::empty : s.found_r2u(); falsified_ = true; return;
          case RS::unit : { s.found_uc();
            const R new_unit = ~curr; curr.reset();
            units |= new_unit; diag.add(new_unit); antidiag.add(new_unit);
            break; }
          default : open_columns &= curr; }
        }
        closed_columns |= units;
        if ((~closed_columns & open_columns).any()) {
          s.found_cu(); falsified_ = true; return;
        }
        if (units == old_units) break;

        // Down-sweep:
        old_units = units;
        if (b[D::N-1].none()) open_columns.set();
        else open_columns = b[D::N-1];
        for (size_t j = D::N-2; j != cbi()-1; --j) {
          diag.right(); antidiag.left();
          R& curr(b[j]);
          if (curr.none()) continue;
          using Rows::RS;
          assert(curr.rs() != RS::empty);
          curr |= units | diag | antidiag;
          switch (curr.rs()) {
          case RS::empty : s.found_r2u(); falsified_ = true; return;
          case RS::unit : { s.found_uc();
            const R new_unit = ~curr; curr.reset();
            units |= new_unit; diag.add(new_unit); antidiag.add(new_unit);
            break; }
          default : open_columns &= curr; }
        }
        closed_columns |= units;
        if ((~closed_columns & open_columns).any()) {
          s.found_cu(); falsified_ = true; return;
        }
        diag.right(); antidiag.left();
      } while (units != old_units);

      while (cbi() < D::N and cbr().none()) inc();
      if (cbi() == D::N) {s.found_r2s(); return;}
      assert(cbi() < D::N-1);
    }

  };

}

#endif
