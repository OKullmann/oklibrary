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
  std::ostream& operator <<(std::ostream& out, const Rtypes rt) {
    switch (rt) {
    case Rtypes::bitset : return out << "bitset";
    case Rtypes::uint   : return out << "uint";
    default : return out << "Rtypes::undefined";
    }
  }

  enum class ERtypes {bitset=0, uint=1 };
  std::ostream& operator <<(std::ostream& out, const ERtypes rt) {
    switch (rt) {
    case ERtypes::bitset : return out << "bitset";
    case ERtypes::uint   : return out << "uint";
    default : return out << "ERtypes::undefined";
    }
  }

  // The implementation choices:
  constexpr Rtypes rt = Rtypes::uint;
  constexpr ERtypes ert = ERtypes::uint;

}

#endif
