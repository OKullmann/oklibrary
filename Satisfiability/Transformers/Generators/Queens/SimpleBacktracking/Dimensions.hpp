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

#include <cstdlib>

namespace Dimensions {

  typedef std::size_t size_t;

#ifndef NUMQUEENS
  constexpr size_t N=16;
#else
  constexpr size_t N=NUMQUEENS;
#endif
  static_assert(N >= 1);


  enum class Rtypes {bitset=0, uint=1 };
  static_assert(std::is_same_v<int, std::underlying_type_t<Rtypes>>);
  constexpr bool valid(const Rtypes rt) noexcept {
    const int i = int(rt);
    return i >= 0 and i <= 1;
  }
  static_assert(valid(Rtypes::bitset) and valid(Rtypes::uint));
  std::ostream& operator <<(std::ostream& out, const Rtypes rt) {
    switch (rt) {
    case Rtypes::bitset : return out << "bitset";
    case Rtypes::uint   : return out << "uint";
    default : return out << "Rtypes::undefined";
    }
  }

  enum class ERtypes {bitset=0, uint=1 };
  static_assert(std::is_same_v<int, std::underlying_type_t<ERtypes>>);
  constexpr bool valid(const ERtypes ert) noexcept {
    const int i = int(ert);
    return i >= 0 and i <= 1;
  }
  static_assert(valid(ERtypes::bitset) and valid(ERtypes::uint));
  std::ostream& operator <<(std::ostream& out, const ERtypes rt) {
    switch (rt) {
    case ERtypes::bitset : return out << "bitset";
    case ERtypes::uint   : return out << "uint";
    default : return out << "ERtypes::undefined";
    }
  }

  // The implementation choices:
#ifndef RTYPES
  constexpr Rtypes rt = Rtypes::uint;
#else
  constexpr Rtypes rt = Rtypes(RTYPES);
#endif
#ifndef ERTYPES
  constexpr ERtypes ert = ERtypes::uint;
#else
  constexpr ERtypes ert = ERtypes(ERTYPES);
#endif

}

#endif
