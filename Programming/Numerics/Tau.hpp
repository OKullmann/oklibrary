// Oliver Kullmann, 7.11.2020 (Swansea)
/* Copyright 2020 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Computing the tau-function and variations
*/

#include <cassert>

#include "FloatingPoint.hpp"

#ifndef TAU_RKly55jp2p
#define TAU_RKly55jp2p

namespace Tau {

  namespace FP = FloatingPoint;

  inline constexpr FP::float80 wtau_ge1(const FP::float80 a) noexcept {
    assert(a >= 1);
    if (FP::isinf(a)) return FP::pinfinity;
    if (a == 1) return FP::Log2;
    FP::float80 x0 = 0;

    return x0;
  }
  static_assert(wtau_ge1(1) == FP::Log2);
  static_assert(wtau_ge1(FP::pinfinity) == FP::pinfinity);
  //static_assert(wtau_ge1(2) == 2 * FP::log_golden_ratio);
  

  inline constexpr FP::float80 wtau_le1(const FP::float80 a) noexcept {
    assert(a <= 1);
    if (a == 0) return 0;
    if (a == 1) return FP::Log2;
    FP::float80 x0 = 0;

    return x0;
  }
  static_assert(wtau_le1(1) == FP::Log2);
  static_assert(wtau_le1(0) == 0);
  //static_assert(wtau_le1(0.5L) == FP::log_golden_ratio);

}
#endif
