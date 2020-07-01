// Oliver Kullmann, 29.6.2020 (Swansea)
/* Copyright 2020 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* The most fundamental basics for the 2-sweep algorithm

*/

#ifndef DIMENSIONS_CFUh4eoEIW
#define DIMENSIONS_CFUh4eoEIW

#include <cstdlib>

namespace Dimensions {

  typedef std::size_t size_t;

#ifndef NUMQUEENS
  constexpr size_t N=16;
#else
  constexpr size_t N=NUMQUEENS;
#endif
  static_assert(N >= 1);

}

#endif
