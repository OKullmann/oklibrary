// Oliver Kullmann, 1.7.2020 (Swansea)
/* Copyright 2020 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* Data structures representing extended rows for the 2-sweep algorithm


TODOS :

*/

#ifndef EXTROWS_Q9FdphioR5
#define EXTROWS_Q9FdphioR5

#include <ostream>
#include <type_traits>

#include <cstdint>

#include "Dimensions.hpp"
#include "Rows.hpp"

namespace ExtRows {

  namespace D = Dimensions;


  // Diagonal/antidiagonal-type, based on sliding window:
  class DADlines {
    typedef Rows::Row Row;
    typedef Dimensions::row_t extrow_t;
    typedef D::sizet sizet;
    static constexpr sizet digits = std::numeric_limits<extrow_t>::digits;

    extrow_t d, ad;

  public :
    static constexpr bool valid = 2*D::N-1<=digits;
    static_assert(valid);

    DADlines() noexcept = default;
    constexpr DADlines(const Row& r, const sizet i) noexcept :
       d(r.extract() << i), ad(r.extract() << (digits-D::N-i)) {
       assert(i < D::N);
     }

    constexpr Row extract(const sizet i) const noexcept {
      assert(i < D::N);
      return (d >> i) | (ad >> (digits-D::N-i));
    }
    void add(const Row& r, const sizet i) noexcept {
      assert(i < D::N);
      d |= r.extract() << i;
      ad |= r.extract() << (digits-D::N - i);
    }
  };

  static_assert(std::is_trivially_copyable_v<DADlines>);


}

#endif
