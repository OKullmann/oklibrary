// Oliver Kullmann, 1.5.2004 (Swansea)
/* Copyright 2004 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#ifndef NUMERICALLIMITSWAECHTER_hdhdt535ae
#define NUMERICALLIMITSWAECHTER_hdhdt535ae

#include <limits>

namespace NumericalLimits {

  // Int x can be safely negated iff -signed_max() <= x <= signed_max()  
  template <typename Int>
  Int signed_max() {
    const Int max = std::numeric_limits<Int>::max();
    const Int min = std::numeric_limits<Int>::min();
    return (min + max <= 0) ? max : - min;
  }
 
}

#endif
