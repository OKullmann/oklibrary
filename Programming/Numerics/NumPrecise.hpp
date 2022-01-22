// Oliver Kullmann, 22.1.2022 (Swansea)
/* Copyright 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Components for precise handling of floating-point types (float80)

  Wrappers:

   - copysign, signbit
   - nextafter

  Accuracy measurement:

    - scoped enum PrecZ
    - accuracy(float80, float80, PrecZ)
    - (plus accuracy_64(float64, float64, PrecZ)
    - accuracyg<FlOAT, FLOAT, PrecZ>
    - accuracyv<VEC1, VEC2, PrecZ>
    - accuracymax<VEC1, VEC2, PrecZ>.

  measures the distance in steps from the "precise" value.


*/

#ifndef NUMPRECISE_L12T2zb1LI
#define NUMPRECISE_L12T2zb1LI

#include <type_traits>
#include <limits>
#include <algorithm>

#include <cmath>

#include "NumTypes.hpp"
#include "NumBasicFunctions.hpp"


namespace FloatingPoint {

  inline CONSTEXPR float80 copysign(const float80 x, const float80 y) noexcept {
    return std::copysign(x,y); // ERROR with gcc 10.1: std::copysignl not available
  }
  STATIC_ASSERT(copysign(3,-2) == -3);
  STATIC_ASSERT(copysign(1,-0.0) == -1);
  STATIC_ASSERT(copysign(-3,2) == 3);

  inline CONSTEXPR float80 signbit(const float80 x) noexcept {
    return std::signbit(x);
  }
  STATIC_ASSERT(not signbit(1));
  STATIC_ASSERT(not signbit(0));
  STATIC_ASSERT(signbit(-0.0));
  STATIC_ASSERT(signbit(-1));


  /* Accuracy testing */

  inline CONSTEXPR float80 nextafter(const float80 from, const float80 to) noexcept {
    return std::nextafterl(from, to);
  }

  // The relative error of x compared with the exact value, in multiples of
  // the smallest possible difference (0 iff we have equality, 1 iff the
  // difference is minimal nonzero).
  enum class PrecZ { denorm=0, min=1, eps=2 };
  inline CONSTEXPR float80 accuracy(const float80 exact, const float80 x, const PrecZ pz = PrecZ::denorm) noexcept {
    if (isnan(exact)) return pinfinity;
    if (exact == pinfinity)
      if (x == pinfinity) return 0;
      else return pinfinity;
    if (exact == minfinity)
      if (x == minfinity) return 0;
      else return pinfinity;
    if (exact == x) return 0;
    if (exact == 0) {
      const float80 ax = FloatingPoint::abs(x);
      switch (pz) {
      case PrecZ::denorm : return ax / denorm_min_value;
      case PrecZ::min : return ax / min_value;
      default : return ax / epsilon; }
    }
    else {
      const float80 pres = exact < x ?
        (x - exact) / (nextafter(exact,x) - exact) :
        (exact - x) / (exact - nextafter(exact,x));
      if (pres > 10) return pres;
      unsigned count = 1;
      for (float80 xp=nextafter(exact,x); xp != x;
           xp=nextafter(xp,x), ++count);
      return count;
    }
  }
  STATIC_ASSERT(accuracy(NaN,NaN) == pinfinity);
  STATIC_ASSERT(accuracy(pinfinity,pinfinity) == 0);
  STATIC_ASSERT(accuracy(minfinity,minfinity) == 0);
  STATIC_ASSERT(accuracy(0,0) == 0);


  inline CONSTEXPR float64 accuracy_64(const float64 exact, const float64 x, const PrecZ pz = PrecZ::denorm) noexcept {
    if (std::isnan(exact)) return pinfinity64;
    if (exact == pinfinity64)
      if (x == pinfinity64) return 0;
      else return pinfinity64;
    if (exact == minfinity64)
      if (x == minfinity64) return 0;
      else return pinfinity64;
    if (exact == x) return 0;
    if (exact == 0) {
      const float64 ax = std::abs(x);
      switch (pz) {
      case PrecZ::denorm : return ax / denorm_min_value64;
      case PrecZ::min : return ax / min_value64;
      default : return ax / epsilon64; }
    }
    else {
      const float64 pres = exact < x ?
        (x - exact) / (std::nextafter(exact,x) - exact) :
        (exact - x) / (exact - std::nextafter(exact,x));
      if (pres > 10) return pres;
      unsigned count = 1;
      for (float64 xp=std::nextafter(exact,x); xp != x;
           xp=std::nextafter(xp,x), ++count);
      return count;
    }
  }
  STATIC_ASSERT(accuracy_64(NaN64,NaN64) == pinfinity64);
  STATIC_ASSERT(accuracy_64(pinfinity64,pinfinity64) == 0);
  STATIC_ASSERT(accuracy_64(minfinity64,minfinity64) == 0);
  STATIC_ASSERT(accuracy_64(0,0) == 0);

  template <typename FL>
  inline CONSTEXPR FL accuracyg(const FL exact, const FL x, const PrecZ pz = PrecZ::denorm) noexcept {
    if constexpr (std::is_same_v<FL,float80>) return accuracy(exact, x, pz);
    if constexpr (std::is_same_v<FL,float64>) return accuracy_64(exact, x, pz);
    using limitfloatg = std::numeric_limits<FL>;
    constexpr FL pinfinityg = limitfloatg::infinity();
    constexpr FL minfinityg = -pinfinityg;
    if (std::isnan(exact)) return pinfinityg;
    if (exact == pinfinityg)
      if (x == pinfinityg) return 0;
      else return pinfinityg;
    if (exact == minfinityg)
      if (x == minfinityg) return 0;
      else return pinfinityg;
    if (exact == x) return 0;
    if (exact == 0) {
      const FL ax = std::abs(x);
      switch (pz) {
      case PrecZ::denorm : return ax / limitfloatg::denorm_min();
      case PrecZ::min : return ax / limitfloatg::min();
      default : return ax / limitfloatg::epsilon(); }
    }
    else {
      const FL pres = exact < x ?
        (x - exact) / (std::nextafter(exact,x) - exact) :
        (exact - x) / (exact - std::nextafter(exact,x));
      if (pres > 10) return pres;
      unsigned count = 1;
      for (FL xp=std::nextafter(exact,x); xp != x;
           xp=std::nextafter(xp,x), ++count);
      return count;
    }
  }
  STATIC_ASSERT(accuracyg(NaN,NaN) == pinfinity);
  STATIC_ASSERT(accuracyg(pinfinity,pinfinity) == 0);
  STATIC_ASSERT(accuracyg(minfinity,minfinity) == 0);
  STATIC_ASSERT(accuracyg(0.0L,0.0L) == 0);

  template <typename VEC1, typename VEC2 = VEC1, typename VEC3 = VEC1>
  inline VEC3 accuracyv(const VEC1& vex, const VEC2& v, const PrecZ pz = PrecZ::denorm) {
    typedef typename VEC1::value_type float_t;
    typedef typename VEC1::size_type size_t;
    const size_t size = std::min(vex.size(), v.size());
    VEC3 res(size);
    for (size_t i = 0; i < size; ++i)
      res[i] = accuracyg<float_t>(vex[i], v[i], pz);
    return res;
  }

  template <typename VEC1, typename VEC2 = VEC1>
  inline auto accuracymax(const VEC1& vex, const VEC2& v, const PrecZ pz = PrecZ::denorm) noexcept {
    typedef typename VEC1::value_type float_t;
    float_t res = -1;
    typedef typename VEC1::size_type size_t;
    const size_t size = std::min(vex.size(), v.size());
    for (size_t i = 0; i < size; ++i)
      res = std::max(res, accuracyg<float_t>(vex[i], v[i], pz));
    return res;
  }

}

#endif
