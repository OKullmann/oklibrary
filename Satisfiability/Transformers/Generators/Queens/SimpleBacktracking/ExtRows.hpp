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

#include <bitset>
#include <ostream>

#include <cstdint>

#include "Dimensions.hpp"

namespace ExtRows {

  namespace D = Dimensions;


  // Diagonal/antidiagonal-type, based on sliding window:
  template <class R> class DADlines {
    typedef std::uint64_t extrow_t;
    static_assert(std::is_integral_v<extrow_t>&&std::is_unsigned_v<extrow_t>);
    typedef D::size_t size_t;
    static constexpr size_t digits = std::numeric_limits<extrow_t>::digits;
    static constexpr size_t digits_ull = std::numeric_limits<unsigned long long>::digits;

    extrow_t d, ad;
    static constexpr extrow_t mask_d = (extrow_t(1) << D::N) - 1;

  public :
    static constexpr bool valid = (2*D::N-1<=digits) and (digits<=digits_ull);

    constexpr DADlines() noexcept = default;
    constexpr DADlines(const R& r, const size_t i) noexcept :
       d(r.to_ullong() << i), ad(r.to_ullong() << (digits-D::N-i)) {
       assert(i < D::N);
     }

    constexpr R extract(const size_t i) const noexcept {
      assert(i < D::N);
      return ((d >> i) | (ad >> (digits-D::N-i))) & mask_d;
    }
    void add(const R& r, const size_t i) noexcept {
      assert(i < D::N);
      d |= r.to_ullong() << i;
      ad |= r.to_ullong() << (digits-D::N - i);
    }
  };

}

#endif
