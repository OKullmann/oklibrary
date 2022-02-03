// Oliver Kullmann, 19.12.2021 (Swansea)
/* Copyright 2021, 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Basic numeric types

  For float80, the basic type, see
  https://en.wikipedia.org/wiki/Extended_precision#x86_extended_precision_format

  Delivers the fundamental floating-type float80 and the underlying
  64-bit unsigned UInt_t and the 32-bit unsigned uint_t; the signed versions
  are Int_t and int_t.

  The type limitfloat resp. limitfloat64 abbreviates the
  corresponding limits-type. Additionally the constants P264m1 = 2^64-1
  of type UInt_t and P232m1 = 2^32-1 of type uint_t are defined.


  Listing of all provided components:

  Macros:
    - CONSTEXPR (disappears for non-gcc-compilation)
    - STATIC_ASSERT(X) (the same)
    - is_pod(X).

  Related to float80:
   - typedef limitfloat
   - constant bool fp_fast_fmal
   - struct Wrap
   - constants pinfinity, minfinity, NaN
   - function isinf, isnan
   - constants epsilon, min_value, denorm_min_value, max_value
   - constants P264, P232
   - functions max, min.

  Related integral types:
   - typedefs UInt_t, uint_t, Int_t, int_t
   - constants P264m1, P32m1
   - functions for checking integrality:
    - isUInt(float80), isUInt(list of float80),
    - isuint(float80)
    - is_int(float80)
    - is_integral(float80)

   - struct F80ai (float80 with possibly asserted integrality and asserted
     "positivity" (i.e., "+")) and asserted "e0"
   - equality-comparison and valid for F80ai

  Related to float64:
   - constants pinfinity64, minfinity64, NaN64
   - function isinf64, isnan64
   - constants epsilon64, min_value64, denorm_min_value64, max_value64
   - struct Wrap64

  Related to float32:
   - struct Wrap32

  Generic wrapper:
   - template class WrapE<FLOAT>.

*/

#ifndef NUMTYPES_87bJ65ZkQQ
#define NUMTYPES_87bJ65ZkQQ

#include <limits>
#include <type_traits>

#include <cstdint>
#include <cmath>

namespace FloatingPoint {

// Uncomment for testing non-gcc-compilers:
//#undef __GNUC__

#if defined __GNUC__ && ! defined __clang__
#  define CONSTEXPR constexpr
#  define STATIC_ASSERT(X) static_assert(X)
#else
#  define CONSTEXPR
#  define STATIC_ASSERT(X) static_assert(0 == 0)
#endif

#define is_pod(X) std::is_standard_layout_v<X> and std::is_trivial_v<X>


  /* Basic concepts */

  typedef long double float80;
  typedef double float64;
  typedef float float32;

  using limitfloat = std::numeric_limits<float80>;
  using limitfloat64 = std::numeric_limits<float64>;
  using limitfloat32 = std::numeric_limits<float32>;


  // float80 fully includes 64-bit integer arithmetic:
  typedef std::uint64_t UInt_t;
  typedef std::uint32_t uint_t;
  typedef std::int64_t Int_t;
  typedef std::int32_t int_t;

  static_assert(limitfloat::is_iec559);
  static_assert(limitfloat::round_style == std::round_to_nearest);
  static_assert(limitfloat::digits == 64);
  static_assert(limitfloat::radix == 2);
  static_assert(limitfloat::digits10 == 18);
  constexpr bool fp_fast_fmal =
#ifdef FP_FAST_FMAL
  true
#else
  false
#endif
;
  // static_assert(fp_fast_fmal); // not given in gcc 10.2, at least not for OK's laptop; http://www.cplusplus.com/reference/cmath/fma/

  struct Wrap {
    float80 x;
    Wrap() = default;
    Wrap(const float80 x) noexcept : x(x) {}
  };
  static_assert(is_pod(Wrap));

  constexpr float80 pinfinity = limitfloat::infinity();
  static_assert(pinfinity > 0);
  static_assert(pinfinity > limitfloat::max());
  static_assert(-pinfinity < limitfloat::lowest());

  constexpr float80 minfinity = -pinfinity;
  static_assert(minfinity == -pinfinity);
  static_assert(-minfinity == pinfinity);
  static_assert(minfinity < pinfinity);
  static_assert(minfinity != pinfinity);
  static_assert(minfinity < limitfloat::lowest());

  inline CONSTEXPR bool isinf(const float80 x) noexcept {
    return std::isinf(x);
  }
  STATIC_ASSERT(isinf(pinfinity));
  STATIC_ASSERT(not isinf(limitfloat::max()));
  STATIC_ASSERT(isinf(-pinfinity));
  STATIC_ASSERT(isinf(minfinity));
  STATIC_ASSERT(not isinf(limitfloat::lowest()));

  constexpr float80 NaN = limitfloat::quiet_NaN();
  inline CONSTEXPR bool isnan(const float80 x) noexcept {
    return std::isnan(x);
  }
  STATIC_ASSERT(isnan(limitfloat::quiet_NaN()));
  STATIC_ASSERT(isnan(NaN));

  constexpr float80 epsilon = limitfloat::epsilon();
  static_assert(1 - epsilon < 1);
  static_assert(1 + epsilon > 1);
  static_assert(1 + epsilon/2 == 1);
  static_assert(epsilon < 1.1e-19L);

  constexpr float80 min_value = limitfloat::min();
  constexpr float80 denorm_min_value = limitfloat::denorm_min();
  static_assert(min_value > 0);
  static_assert(min_value < 3.4e-4932L);
  static_assert(denorm_min_value < 1e-4950L);
  static_assert(denorm_min_value > 0);
  static_assert(denorm_min_value / 2 == 0, "Higher precision than usual for denorm_min.");
  constexpr float80 max_value = limitfloat::max();
  static_assert(max_value < pinfinity);
  static_assert(1/max_value > 0);
  static_assert(1/min_value < max_value);
  static_assert(1/max_value < min_value);
  static_assert(max_value > 1.1e4932L);
  static_assert(limitfloat::lowest() == -max_value);


  inline CONSTEXPR float80 max(const float80 x, const float80 y) noexcept {
    return std::fmax(x,y);
  }
  inline CONSTEXPR float80 min(const float80 x, const float80 y) noexcept {
    return std::fmin(x,y);
  }
  STATIC_ASSERT(max(1.23L, -1.09L) == 1.23L);
  STATIC_ASSERT(min(44.123L, 55.88L) == 44.123L);


  /* Connection with integral types */

  static_assert(0.3e1L == 3L);

  constexpr UInt_t P264m1 = std::numeric_limits<UInt_t>::max();
  static_assert(P264m1 == UInt_t(-1));
  static_assert(P264m1 == UInt_t(1.8446744073709551615e19L));
  static_assert(P264m1 + 1 == 0);
  static_assert(UInt_t(float80(P264m1)) == P264m1);

  constexpr uint_t P232m1 = std::numeric_limits<uint_t>::max();
  static_assert(P232m1 + 1 == 0);
  static_assert(UInt_t(P232m1)*P232m1 == P264m1 - 2*(UInt_t(P232m1)+1) + 2);

  constexpr float80 P264 = 18446744073709551616.0L;
  constexpr float80 P232 = 4294967296.0L;
  static_assert(P264 == 1.8446744073709551616e19L);
  static_assert(P232 == 4.294967296e9L);
  static_assert(-(-P264) == P264);

  // Exactly the integers in the interval [-P264, +P264] are exactly
  // represented by float80:
  static_assert(P264-1 == P264m1);
  static_assert(-P264 == -float80(P264m1) - 1);
  static_assert(P264+1 == P264);
  static_assert(-P264-1 == -P264);
  static_assert(P232-1 == P232m1);
  static_assert(-P232 == -float80(P232m1) - 1);

  inline CONSTEXPR bool isUInt(const float80 x) noexcept {
    if (isnan(x)) return false;
    if (x < 0) return false;
    if (x > P264m1) return false;
    if (UInt_t(x) != x) return false;
    return true;
  }
  STATIC_ASSERT(isUInt(0)); STATIC_ASSERT(not isUInt(-1));
  STATIC_ASSERT(isUInt(P264m1)); STATIC_ASSERT(not isUInt(NaN));
  STATIC_ASSERT(not isUInt(pinfinity)); STATIC_ASSERT(not isUInt(minfinity));
  STATIC_ASSERT(not isUInt(0.5)); STATIC_ASSERT(not isUInt(P264m1 + 0.5L));

  inline CONSTEXPR bool isUInt(const std::initializer_list<float80> X) noexcept {
    for (const float80 x : X) if (not isUInt(x)) return false;
    return true;
  }
  STATIC_ASSERT(isUInt({0,1,2,P264m1}));
  STATIC_ASSERT(not isUInt({0,1,2,P264m1,1.1}));

  inline CONSTEXPR bool is_integral(const float80 x) noexcept {
    return std::truncl(x) == x;
  }
  STATIC_ASSERT(is_integral(P264)); STATIC_ASSERT(is_integral(-P264));
  STATIC_ASSERT(is_integral(pow(2,1000)));
  STATIC_ASSERT(is_integral(-pow(2,1000)));
  STATIC_ASSERT(not is_integral(0.5)); STATIC_ASSERT(not is_integral(-0.5));
  STATIC_ASSERT(is_integral(1.1e1000L));
  STATIC_ASSERT(not is_integral(1e-1000L));

  inline CONSTEXPR bool isuint(const float80 x) noexcept {
    if (isnan(x)) return false;
    if (x < 0) return false;
    if (x > P232m1) return false;
    if (uint_t(x) != x) return false;
    return true;
  }
  STATIC_ASSERT(isuint(0)); STATIC_ASSERT(not isuint(-1));
  STATIC_ASSERT(isUInt(P232m1)); STATIC_ASSERT(not isuint(NaN));
  STATIC_ASSERT(not isuint(pinfinity)); STATIC_ASSERT(not isuint(minfinity));
  STATIC_ASSERT(not isuint(0.5)); STATIC_ASSERT(not isuint(P232m1 + 0.5L));

  inline CONSTEXPR bool is_int(const float80 x) noexcept {
    if (isnan(x)) return false;
    using limitint = std::numeric_limits<int>;
    if (x < limitint::min()) return false;
    if (x > limitint::max()) return false;
    if (int(x) != x) return false;
    return true;
  }
  STATIC_ASSERT(is_int(0)); STATIC_ASSERT(is_int(-1));
  STATIC_ASSERT(is_int(std::numeric_limits<int>::max()));
  STATIC_ASSERT(not is_int(float80(std::numeric_limits<int>::max()) + 0.5L));
  STATIC_ASSERT(is_int(std::numeric_limits<int>::min()));
  STATIC_ASSERT(not is_int(float80(std::numeric_limits<int>::min()) - 0.5L));
  STATIC_ASSERT(not is_int(NaN));
  STATIC_ASSERT(not is_int(pinfinity)); STATIC_ASSERT(not is_int(minfinity));
  STATIC_ASSERT(not is_int(0.5));


  /* Basic definitions for float64 = double */

  static_assert(limitfloat64::is_iec559);
  static_assert(limitfloat64::round_style == std::round_to_nearest);
  static_assert(limitfloat64::digits == 53);
  static_assert(limitfloat64::radix == 2);
  static_assert(limitfloat64::digits10 == 15);

  constexpr float64 pinfinity64 = limitfloat64::infinity();
  static_assert(pinfinity64 > 0);
  static_assert(pinfinity64 > limitfloat64::max());
  static_assert(-pinfinity64 < limitfloat64::lowest());
  constexpr float64 minfinity64 = -pinfinity64;
  static_assert(minfinity64 == -pinfinity64);
  static_assert(-minfinity64 == pinfinity64);
  static_assert(minfinity64 < pinfinity64);
  static_assert(minfinity64 != pinfinity64);
  static_assert(minfinity64 < limitfloat64::lowest());

  inline CONSTEXPR bool isinf64(const float64 x) noexcept {
    return std::isinf(x);
  }
  STATIC_ASSERT(isinf64(pinfinity64));
  STATIC_ASSERT(not isinf64(limitfloat64::max()));
  STATIC_ASSERT(isinf64(-pinfinity64));
  STATIC_ASSERT(isinf64(minfinity64));
  STATIC_ASSERT(not isinf64(limitfloat64::lowest()));
  STATIC_ASSERT(isinf64(pinfinity));
  STATIC_ASSERT(isinf(pinfinity64));

  constexpr float64 NaN64 = limitfloat64::quiet_NaN();
  inline CONSTEXPR bool isnan64(const float64 x) noexcept {
    return std::isnan(x);
  }
  STATIC_ASSERT(isnan64(limitfloat64::quiet_NaN()));
  STATIC_ASSERT(isnan64(NaN64));
  STATIC_ASSERT(isnan64(NaN));
  STATIC_ASSERT(isnan(NaN64));

  constexpr float64 epsilon64 = limitfloat64::epsilon();
  static_assert(1 - epsilon64 < 1);
  static_assert(1 + epsilon64 > 1);
  static_assert(1 + epsilon64/2 == 1);
  static_assert(epsilon64 < 3e-16L);
  constexpr float64 min_value64 = limitfloat64::min();
  constexpr float64 denorm_min_value64 = limitfloat64::denorm_min();
  static_assert(min_value64 > 0);
  static_assert(min_value64 < 2.3e-308);
  static_assert(denorm_min_value64 < 1e-323);
  static_assert(denorm_min_value64 > 0);
  static_assert(denorm_min_value64 / 2 == 0, "Higher precision than usual for denorm_min.");
  constexpr float64 max_value64 = limitfloat64::max();
  static_assert(max_value64 < pinfinity64);
  static_assert(1/max_value64 > 0);
  static_assert(1/min_value64 < max_value64);
  static_assert(1/max_value64 < min_value64);
  static_assert(max_value64 > 1.7e308);
  static_assert(limitfloat64::lowest() == -max_value64);


  struct Wrap64 {
    float64 x;
    Wrap64() = default;
    Wrap64(const float64 x) noexcept : x(x) {}
  };
  static_assert(is_pod(Wrap64));

  struct Wrap32 {
    float32 x;
    Wrap32() = default;
    Wrap32(const float32 x) noexcept : x(x) {}
  };
  static_assert(is_pod(Wrap32));

  // Scientific notation, with current precision, but without trailing zeros:
  template <typename FLOAT>
  struct WrapE {
    inline static bool deactivated = false;
    typedef FLOAT float_t;
    float_t x;
    WrapE() = default;
    WrapE(const float_t x) noexcept : x(x) {}
  };


  // float80, possibly asserted as integral or non-negative or "with e0":
  struct F80ai {
    float80 x;
    bool isint = false;
    bool hasplus = false;
    bool hase0 = false;
    friend constexpr bool operator ==(F80ai, F80ai) noexcept;
  };
  inline constexpr bool operator ==(const F80ai lhs, const F80ai rhs) noexcept = default;
  static_assert(F80ai{0} == F80ai{0,false,false,false});
  inline CONSTEXPR bool valid(const F80ai x) noexcept {
    return (not x.isint or is_integral(x.x)) and
      (not x.hasplus or not std::signbit(x.x));
  }

}

#endif
