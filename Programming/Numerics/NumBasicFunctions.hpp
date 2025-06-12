// Oliver Kullmann, 22.1.2022 (Swansea)
/* Copyright 2022, 2023, 2025 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Very basic functions, imported from the standard library

  The functions

    - abs

    - fma

    - log, log1p, log10, log2, ilogb
    - exp, expm1, pow, exp2, ldexp
    - sin, cos, tan, asin, acos, atan, atan2

    - erf, erfc

    - recip (1/x), sq (x^2), cb (x^3), qa (x^4) (own functions)
    - sqrt, cbrt, qart (own function)
    - hypot

    - midpoint, lerp

  are provided as wrappers, to make sure they work with float80.

  The following constants of type float80 are defined (only defined for
  float64 if explicitly mentioned):

    - Log2 (= log(2))
    - euler, eulerm1

    - pi

    - Sqr2 = sqrt(2), Cbr2 = cbrt(2), Qar2 = qart(2)

    - golden_ratio, log_golden_ratio, log_golden_ratio64
      UInt_t golden_ratio_u64 (rounded P264 / golden_ration to nearest)
      uint_t golden_ratio_u32 (rounded P232 / golden_ration to nearest).

  Hashing:

    - hash(UInt_t) -> UInt_t
      hash_UInt
    - hash(uint_t) -> uint_t (different from UInt_t)
      hash_uint
    - hash(Int_t) -> UInt_t (reduces to UInt_t)
      hash_Int
    - hash(int) -> UInt_t (special case of Int_t)
      hash_int

    - hash(float80) -> UInt_t
    - hash(float64) -> UInt_t (special case of float80)
    - hash(F80ai) -> UInt_t

    For user-defined types X a function
      hash(X) -> UInt_t
    can be provided in the namespace of X.

    Tools for combining hash-values:

    - hash_combine(UInt_t& seed, UInt_t other)
      hash_combine_UInt
    - hash_combine(uint_t& seed, uint_t other)
      hash_combine_UInt

    - functor hash_UInt_range with member-operator (), taking
      a range of values x (assuming hash(x) is defined) as argument,
      and producing a UInt_t hash-value (also a brace-enclosed
      initialiser-list is possible);
      the apply-member-template takes the hash-function explicitly
      as argument.
      The default-constructor uses size(x) as starting seed, otherwise
      the constructor provides it.

    - convenience-wrappers of hash_UInt_range, for ranges of ranges:
     - hash_sizes(RAN r) -> UInt_t
     - hash_ranges(RAN r, UInt_t seed) -> UInt_t

    See ProgramOptions/Strings.hpp for a hash-function for strings,
    and Random/VarLit.hpp for a hash-function for Lit.

*/

#ifndef NUMBASICFUNCTIONS_Ok1Ktgi6Lz
#define NUMBASICFUNCTIONS_Ok1Ktgi6Lz

#include <numbers>
#include <numeric>
#include <array>

#include <cmath>

// Guaranteed to be included:
#include "NumTypes.hpp"

#include "Conversions.hpp"

namespace FloatingPoint {

  inline CONSTEXPR float80 abs(const float80 x) noexcept {
    return std::fabsl(x);
  }
  STATIC_ASSERT(abs(0) == 0);
  STATIC_ASSERT(abs(1) == 1);
  STATIC_ASSERT(abs(-1) == 1);


  // x* y + z:
  inline CONSTEXPR float80 fma(const float80 x, const float80 y, const float80 z) noexcept {
    return std::fmal(x,y,z);
  }
  STATIC_ASSERT(fma(2,3,4) == 10);


  inline CONSTEXPR float80 log(const float80 x) noexcept {
    return std::logl(x);
  }
  STATIC_ASSERT(log(1) == 0);
  STATIC_ASSERT(log(4) == 2*log(2));
  STATIC_ASSERT(log(0.5) == -log(2));
  constexpr float80 Log2 = 0.693147180559945309417232121458L;
  STATIC_ASSERT(Log2 == log(2));
  STATIC_ASSERT(float64(Log2) == std::numbers::ln2);
  // STATIC_ASSERT(log(pinfinity) == pinfinity); // bug with gcc 10.2
  // STATIC_ASSERT(log(0) == -pinfinity); // bug with gcc 10.2

  // log(1+x):
  inline CONSTEXPR float80 log1p(const float80 x) noexcept {
    return std::log1pl(x);
  }
  STATIC_ASSERT(log1p(0) == 0);
  STATIC_ASSERT(log1p(1e-1000L) == 1e-1000L);

  inline CONSTEXPR float80 log10(const float80 x) noexcept {
    return std::log10l(x);
  }
  STATIC_ASSERT(log10(10) == 1);

  inline CONSTEXPR float80 log2(const float80 x) noexcept {
    return std::log2l(x);
  }
  STATIC_ASSERT(log2(64) == 6);
  STATIC_ASSERT(log2(0.125) == -3);

  inline CONSTEXPR int ilogb(const float80 x) noexcept {
    return std::ilogbl(x);
  }
  STATIC_ASSERT(ilogb(8) == 3);
  STATIC_ASSERT(ilogb(9) == 3);
  STATIC_ASSERT(ilogb(16) == 4);
  STATIC_ASSERT(ilogb(1) == 0);
  STATIC_ASSERT(ilogb(0.9) == -1);
  STATIC_ASSERT(ilogb(0.5) == -1);
  STATIC_ASSERT(ilogb(0.4) == -2);


  inline CONSTEXPR float80 exp(const float80 x) noexcept {
    return std::expl(x);
  }
  STATIC_ASSERT(exp(0) == 1);
  STATIC_ASSERT(exp(2) == exp(1)*exp(1));
  STATIC_ASSERT(log(exp(1)) == 1);
  constexpr float80 euler = 2.718281828459045235360287471352662497757L;
  STATIC_ASSERT(euler == exp(1));
  STATIC_ASSERT(log(euler) == 1);
  constexpr float80 eulerm1 = 1.718281828459045235360287471352662497757L;

  // exp(x) - 1:
  inline CONSTEXPR float80 expm1(const float80 x) noexcept {
    return std::expm1l(x);
  }
  STATIC_ASSERT(expm1(0) == 0);
  STATIC_ASSERT(expm1(1e-1000L) == 1e-1000L);
  STATIC_ASSERT(expm1(1) == eulerm1);

  inline CONSTEXPR float80 pow(const float80 x, const float80 y) noexcept {
    return std::powl(x,y);
  }
  STATIC_ASSERT(pow(0,0) == 1);
  STATIC_ASSERT(pow(2,-1) == 0.5);
  STATIC_ASSERT(pow(2,16) == 65536);
  STATIC_ASSERT(P264m1 == pow(2,64) - 1);
  STATIC_ASSERT(P232m1 == pow(2,32) - 1);
  STATIC_ASSERT(UInt_t(P232m1)*P232m1 == pow(2,64) - pow(2,33) + 1);
  STATIC_ASSERT(P264 == pow(2,64));
  STATIC_ASSERT(P232 == pow(2,32));

  // 2^x:
  inline CONSTEXPR float80 exp2(const float80 x) noexcept {
    return std::exp2l(x);
  }
  STATIC_ASSERT(exp2(64) == pow(2,64));
  STATIC_ASSERT(exp2(-1) == 0.5);

  // x * 2^x:
  inline CONSTEXPR float80 ldexp(const float80 x, const int exp) noexcept {
    return std::ldexpl(x, exp);
  }
  STATIC_ASSERT(ldexp(1,-1000) == pow(2,-1000));
  STATIC_ASSERT(ldexp(ldexp(P264m1,10000),-10000) == P264m1);
  STATIC_ASSERT(ldexp(ldexp(P264m1,-10000),10000) == P264m1);


  inline CONSTEXPR float80 sin(const float80 x) noexcept {
    return std::sinl(x);
  }
  STATIC_ASSERT(sin(0) == 0);
  inline CONSTEXPR float80 cos(const float80 x) noexcept {
    return std::cosl(x);
  }
  STATIC_ASSERT(cos(0) == 1);
  inline CONSTEXPR float80 tan(const float80 x) noexcept {
    return std::tanl(x);
  }
  STATIC_ASSERT(tan(0) == 0);
  inline CONSTEXPR float80 asin(const float80 x) noexcept {
    return std::asinl(x);
  }
  STATIC_ASSERT(asin(0) == 0);
  inline CONSTEXPR float80 acos(const float80 x) noexcept {
    return std::acosl(x);
  }
  STATIC_ASSERT(acos(1) == 0);
  inline CONSTEXPR float80 atan(const float80 x) noexcept {
    return std::atanl(x);
  }
  STATIC_ASSERT(atan(0) == 0);
  inline CONSTEXPR float80 atan2(const float80 x, const float80 y) noexcept {
    return std::atan2l(x,y);
  }
  STATIC_ASSERT(atan2(0,0) == 0);
  STATIC_ASSERT(atan2(-1,0) == -acos(0));
  STATIC_ASSERT(atan2(1,0) == acos(0));

  constexpr float80 pi = 3.141592653589793238462643383279502884L;
  STATIC_ASSERT(pi == std::acos(float80(-1)));
  STATIC_ASSERT(pi == FloatingPoint::acos(-1));
  STATIC_ASSERT(pi/2 == FloatingPoint::acos(0));
  STATIC_ASSERT(std::cos(pi) == -1);
  STATIC_ASSERT(abs(std::sin(pi)) < epsilon);


  inline constexpr float80 recip(const float80 x) noexcept { return 1/x; }
  static_assert(recip(1) == 1);
  static_assert(recip(0.5) == 2);
  static_assert(recip(2) == 0.5);
  static_assert(isnan(recip(NaN)));
  static_assert(recip(pinfinity) == 0);
  static_assert(recip(minfinity) == 0);

  inline constexpr float80 sq(const float80 x) noexcept {
    return x*x;
  }
  static_assert(sq(0) == 0);
  static_assert(sq(1) == 1);
  static_assert(sq(2) == 4);
  static_assert(sq(-1) == 1);

  inline CONSTEXPR float80 sqrt(const float80 x) noexcept {
    return std::sqrtl(x);
  }
  STATIC_ASSERT(sqrt(0) == 0);
  STATIC_ASSERT(sqrt(1) == 1);
  STATIC_ASSERT(sqrt(4) == 2);
  STATIC_ASSERT(sqrt(3*3+4*4) == 5);
  constexpr float80 Sqr2 = 1.4142135623730950488016887242L;
  STATIC_ASSERT(Sqr2 == sqrt(2));
  constexpr float80 Sqr3 = 1.7320508075688772935274463415L;
  STATIC_ASSERT(Sqr3 == sqrt(3));
  // STATIC_ASSERT(isnan(sqrt(-1))); // bug with gcc 10.2
  STATIC_ASSERT(sqrt(P264) == pow(2,32));
  STATIC_ASSERT(sqrt(sqrt(P264)) == pow(2,16));


  constexpr float80 golden_ratio = 1.6180339887498948482045868L;
  STATIC_ASSERT(golden_ratio == (1+sqrt(5))/2);
  STATIC_ASSERT(fma(golden_ratio,golden_ratio,-golden_ratio) == 1);
  STATIC_ASSERT(float64(golden_ratio) == std::numbers::phi);
  constexpr float80 log_golden_ratio = 0.4812118250596034474977589L;
  STATIC_ASSERT(log_golden_ratio == log(golden_ratio));
  STATIC_ASSERT(exp(log_golden_ratio) == golden_ratio);
  constexpr float64 log_golden_ratio64 = log_golden_ratio;
  STATIC_ASSERT(log_golden_ratio64 == std::log(std::numbers::phi));
  constexpr UInt_t golden_ratio_u64 = UInt_t(round(P264 * (golden_ratio-1)));
  static_assert(golden_ratio_u64 == 0x9e3779b97f4a7c16LLU);
  constexpr uint_t golden_ratio_u32 = uint_t(round(P232 * (golden_ratio-1)));
  static_assert(golden_ratio_u32 == 0x9e3779b9U);


  inline constexpr float80 cb(const float80 x) noexcept {
    return x*x*x;
  }
  static_assert(cb(0) == 0);
  static_assert(cb(1) == 1);
  static_assert(cb(2) == 8);
  static_assert(cb(-1) == -1);

  inline CONSTEXPR float80 cbrt(const float80 x) noexcept {
    return std::cbrtl(x);
  }
  STATIC_ASSERT(cbrt(27) == 3);
  STATIC_ASSERT(cbrt(1e3) == 1e1L);
  STATIC_ASSERT(cb(cbrt(8)) == 8);

  inline constexpr float80 qa(const float80 x) noexcept {
    return sq(sq(x));
  }
  static_assert(qa(0) == 0);
  static_assert(qa(1) == 1);
  static_assert(qa(2) == 16);
  static_assert(qa(-1) == 1);

  inline CONSTEXPR float80 qart(const float80 x) noexcept {
    return sqrt(sqrt(x));
  }
  STATIC_ASSERT(qart(81) == 3);
  STATIC_ASSERT(qart(1e4) == 1e1L);
  STATIC_ASSERT(qart(qa(8)) == 8);

  constexpr float80 Cbr2 = 1.259921049894873164767210607L;
  STATIC_ASSERT(Cbr2 == cbrt(2));
  constexpr float80 Qar2 = 1.18920711500272106671749997056L;
  STATIC_ASSERT(Qar2 == qart(2));

  inline CONSTEXPR float80 hypot(const float80 x, const float80 y) noexcept {
    return std::hypot(x,y);
  }
  STATIC_ASSERT(hypot(3,4) == 5);


  inline constexpr float80 midpoint(const float80 a, const float80 b) noexcept {
    return std::midpoint(a,b);
  }
  static_assert(midpoint(-1, 1) == 0);

  inline constexpr float80 lerp(const float80 a, const float80 b, const float80 t) noexcept {
    return std::lerp(a,b,t);
  }
  static_assert(lerp(0, 4, 0.25L) == 1);


  inline CONSTEXPR float80 erf(const float80 x) noexcept {
    return std::erfl(x);
  }
  STATIC_ASSERT(erf(0) == 0);
  STATIC_ASSERT(erf(1) == 0.8427007929497148693412L);
  STATIC_ASSERT(erf(-1) == -0.8427007929497148693412L);

  inline CONSTEXPR float80 erfc(const float80 x) noexcept {
    return std::erfcl(x);
  }
  STATIC_ASSERT(erfc(0) == 1);
  STATIC_ASSERT(erfc(1) == 0.15729920705028513066L);
  STATIC_ASSERT(erfc(-1) == 1.8427007929497148693L);


  /*
    Hashing
  */

  inline constexpr uint_t hash_uint(uint_t x) noexcept {
    x = ((x >> 16) ^ x) * 0x45d9f3bU;
    x = ((x >> 16) ^ x) * 0x45d9f3bU;
    x = (x >> 16) ^ x;
    return x;
  }
  constexpr uint_t hash(const uint_t x) noexcept {
    return hash_uint(x);
  }
  static_assert(hash(uint_t(0)) == 0);
  static_assert(hash(uint_t(1)) == 824515495);

  inline constexpr UInt_t hash_UInt(UInt_t x) noexcept {
    x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
    x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
    x = x ^ (x >> 31);
    return x;
  }
  constexpr UInt_t hash(const UInt_t x) noexcept {
    return hash_UInt(x);
  }
  static_assert(hash(UInt_t(0)) == 0);
  static_assert(hash(UInt_t(1)) == 6238072747940578789ULL);
  static_assert(hash(UInt_t(-1)) == 13029008266876403067ULL);


  inline constexpr UInt_t hash_Int(const Int_t x) noexcept {
    return hash(UInt_t(x));
  }
  constexpr UInt_t hash(const Int_t x) noexcept {
    return hash_Int(x);
  }
  static_assert(hash(Int_t(0)) == 0);
  static_assert(hash(Int_t(1)) == 6238072747940578789ULL);
  static_assert(hash(Int_t(-1)) == 13029008266876403067ULL);

  inline constexpr UInt_t hash_int(const int x) noexcept {
    static_assert(std::numeric_limits<int>::digits <= 63);
    return hash(UInt_t(x));
  }
  constexpr UInt_t hash(const int x) noexcept {
    return hash_int(x);
  }
  static_assert(hash(0) == 0);
  static_assert(hash(1) == 6238072747940578789ULL);
  static_assert(hash(-1) == 13029008266876403067ULL);


  constexpr void hash_combine_uint(uint_t& seed, uint_t other) noexcept {
    seed ^= other + golden_ratio_u32 + (seed<<6) + (seed>>2);
  }
  constexpr void hash_combine(uint_t& seed, const uint_t other) noexcept {
    return hash_combine_uint(seed, other);
  }
  constexpr void hash_combine_UInt(UInt_t& seed, UInt_t other) noexcept {
    seed ^= other + golden_ratio_u64 + (seed<<12) + (seed>>4);
  }
  constexpr void hash_combine(UInt_t& seed, const UInt_t other) noexcept {
    return hash_combine_UInt(seed, other);
  }


  constexpr UInt_t hash(const float80 x) noexcept {
    const RepFloat80 r(x);
    UInt_t seed = 5;
    hash_combine_UInt(seed, hash(r.m));
    hash_combine_UInt(seed, hash(r.be));
    hash_combine_UInt(seed, hash(1*r.neg + 2*r.nan + 4*r.inf));
    return seed;
  }
  static_assert(hash(float80(0)) == 4962778281351967142ULL);
  static_assert(hash(float80(-0.0)) == 11128757977616285571ULL);
  static_assert(hash(NaN) == 2355714540944813352ULL);
  static_assert(hash(pinfinity) == 18195357615279250098ULL);
  static_assert(hash(minfinity) == 18275595512282103802ULL);
  constexpr UInt_t hash(const float64 x) noexcept {
    return hash(float80(x));
  }
  static_assert(hash(0.0) == 4962778281351967142ULL);
  static_assert(hash(-0.0) == 11128757977616285571ULL);

  constexpr UInt_t hash(const F80ai x) noexcept {
    UInt_t seed = hash(x.x);
    hash_combine_UInt(seed, hash(1*x.isint + 2*x.hasplus + 4*x.hase0));
    return seed;
  }
  static_assert(hash(F80ai(0)) == 15208958856954972822ULL);


  /*
    The empty range has always hash 0 in automatic mode.
    Equal ranges (according to std::ranges::equal) have equal hash-values,
    given that the underlying ==-relation respects hash-values.
  */
  struct hash_UInt_range {
    typedef UInt_t return_type;
    bool automatic;
    UInt_t seed;

    constexpr hash_UInt_range() noexcept : automatic(true) {};
    explicit constexpr hash_UInt_range(const UInt_t s)
      : automatic(false), seed(s) {}

    template <class RAN>
    constexpr UInt_t operator ()(const RAN& r) noexcept {
      if (automatic) seed = r.size();
      for (const auto& x : r) hash_combine_UInt(seed, hash(x));
      return seed;
    }
    template <class RAN, class FUN>
    constexpr UInt_t apply(const RAN& r, const FUN& hash) noexcept {
      if (automatic) seed = r.size();
      for (const auto& x : r) hash_combine_UInt(seed, hash(x));
      return seed;
    }
    template <typename T>
    constexpr UInt_t operator ()(const std::initializer_list<T>& r)
      noexcept {
      if (automatic) seed = r.size();
      for (const auto& x : r) hash_combine_UInt(seed, hash(x));
      return seed;
    }
    template <typename T, class FUN>
    constexpr UInt_t apply(const std::initializer_list<T>& r,
                 const FUN& hash) noexcept {
      if (automatic) seed = r.size();
      for (const auto& x : r) hash_combine_UInt(seed, hash(x));
      return seed;
    }
  };
  static_assert(hash_UInt_range()(std::initializer_list<int>{}) == 0);
  static_assert(hash_UInt_range()({0,1,2}) == 7962449136380669187ULL);
  static_assert(hash_UInt_range()({UInt_t(2),UInt_t(2),UInt_t(2)}) ==
                9938619543513371241ULL);
  static_assert(hash_UInt_range()({0.0,1.0,2.0}) == 17664979641484437695ULL);
  static_assert(hash_UInt_range()({UInt_t(5),UInt_t(2),
                      UInt_t(17115513403910344103ULL)})
                == 5758276966762890889ULL);

  // For a range of ranges, hashing the sizes:
  template <class RAN>
  constexpr UInt_t hash_sizes(const RAN& r) noexcept {
    return hash_UInt_range().apply(r,
        [](const auto& x)noexcept{return hash_UInt(x.size());});
  }
  static_assert(hash_sizes(std::array<std::array<int,2>,3>{}) ==
                9938619543513371241ULL);

  // For a range of ranges, start with seed, and apply hash_UInt_range:
  template <class RAN>
  constexpr UInt_t hash_ranges(const RAN& r, const UInt_t seed) noexcept {
    return hash_UInt_range(seed).apply(r, [](const auto& x){
                                         return hash_UInt_range()(x);});
  }
  static_assert(hash_ranges(
      std::initializer_list<std::initializer_list<int>>{{1,-2},{3}},
      5758276966762890889ULL)
                == 11180425951301671036ULL);

}

#endif
