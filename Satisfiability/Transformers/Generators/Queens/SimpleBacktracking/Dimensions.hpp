// Oliver Kullmann, 1.7.2020 (Swansea)
/* Copyright 2020 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* The most fundamental basics for the 2-sweep algorithm

*/

#ifndef DIMENSIONS_CFUh4eoEIW
#define DIMENSIONS_CFUh4eoEIW

#include <ostream>
#include <type_traits>

#include <cstdint>

namespace Dimensions {

  typedef unsigned sizet;
  static_assert(std::is_integral_v<sizet> and std::is_unsigned_v<sizet>);

  typedef std::uint64_t row_t; // using the first N bits
  static_assert(std::is_integral_v<row_t> and std::is_unsigned_v<row_t>);


#ifndef NUMQUEENS
  constexpr sizet N=16;
#else
  constexpr sizet N=NUMQUEENS;
#endif
  static_assert(N >= 1);

  enum class Btypes {recursive=0, nonrecursive=1 };
  static_assert(std::is_same_v<int, std::underlying_type_t<Btypes>>);
  constexpr bool valid(const Btypes bt) noexcept {
    const int i = int(bt);
    return i >= 0 and i <= 1;
  }
  static_assert(valid(Btypes::recursive) and valid(Btypes::nonrecursive));
  std::ostream& operator <<(std::ostream& out, const Btypes rt) {
    switch (rt) {
    case Btypes::recursive : return out << "recursive";
    case Btypes::nonrecursive : return out << "nonrecursive";
    default : return out << "Btypes::undefined";
    }
  }

  // The implementation choices:
#ifndef BTYPES
  constexpr Btypes bt = Btypes::recursive;
#else
  constexpr Btypes bt = Btypes(BTYPES);
#endif

}

#endif
