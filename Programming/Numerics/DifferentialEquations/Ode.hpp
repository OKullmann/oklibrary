// Oliver Kullmann, 6.2.2021 (Swansea)
/* Copyright 2021 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  Tools for numerical solution of differential equations

   - Euler1d<FLOAT>

   - RK41d<FLOAT>
   - RK41d_auto<FLOAT>

   - RK4<FLOAT>

   - vec80_2d, vec80_3d

TODOS:

1. The function RK41d::interval should likely become a free-standing
   facility, operating on "any" solver like RK41d.

2. The functions available as statistics associated to RK41d::interval
   should become free-standing facilities for evaluating any vector of
   points.
    - Such statistics-tool should likely be placed directly to module Numerics.
    - DONE As the current module Statistics in Random.
    - Standards:
     - Value and corresponding x-value: pair (v, x)
     - Overview on range; simple just min mean max for x; for y:
         min mean max; median standard-deviation
       where min and max are pairs.
     - Perhaps one first give a quik overview, with precision say 5, and
       at the bottom then all numbers in full precision.

*/

#ifndef ODE_4HkvmZBVgf
#define ODE_4HkvmZBVgf

#include <functional>
#include <array>
#include <vector>
#include <algorithm>
#include <limits>
#include <ostream>
#include <numeric>
#include <iomanip>

// Guaranteed to be included:
#include <Numerics/FloatingPoint.hpp>

#include <Numerics/Statistics.hpp>

#include "Stepper.hpp"

namespace Ode {

  namespace FP = FloatingPoint;
  using FP::float80;

  template <typename FLOAT>
  struct Euler1d {
    typedef FLOAT float_t;
    typedef std::function<float_t(float_t)> f_t;
    typedef std::function<float_t(float_t,float_t)> F_t;

    typedef FP::UInt_t count_t;
    static constexpr count_t default_N = 100'000;

    const F_t F;
    const f_t sol;

  private :
    float_t x0, y0;
  public :

    Euler1d(const float_t x0, const float_t y0, const F_t F, const f_t sol = f_t()) noexcept :
    F(F), sol(sol), x0(x0), y0(y0) { assert(F); }

    float_t x() const noexcept { return x0; }
    float_t y() const noexcept { return y0; }
    float_t accuracy() const {
      return FP::accuracyg<float_t>(sol(x0), y0, FP::PrecZ::eps);
    }
    void reset(const float_t x1, const float_t y1) noexcept {
      x0 = x1; y0 = y1;
    }
    // For providing a more accurate value:
    void precise_x0(const float_t x1) noexcept { x0 = x1; }

    void step(const float_t delta) noexcept {
      y0 = std::fma(delta, F(x0,y0), y0);
      x0 += delta;
    }
    void steps(const float_t delta, const count_t N = default_N) noexcept {
      if (N == 0) return;
      if (N == 1) {step(delta); return;}
      const float_t old_x0 = x0;
      const float_t small_d = delta / N;
      for (count_t i = 0; i < N;) {
        y0 = std::fma(small_d, F(x0,y0), y0);
        x0 = std::fma(++i, small_d, old_x0);
      }
      x0 = old_x0 + delta;
    }
  };

  template <typename FLOAT>
  struct RK41d {
    typedef FLOAT float_t;
    typedef std::function<float_t(float_t)> f_t;
    typedef std::function<float_t(float_t,float_t)> F_t;

    typedef FP::UInt_t count_t;
    static constexpr count_t default_N = 25'000;

    const F_t F;
    const f_t sol;

  private :
    float_t x0, y0;
  public :

    RK41d(const float_t x0, const float_t y0, const F_t F, const f_t sol = f_t()) noexcept :
    F(F), sol(sol), x0(x0), y0(y0) { assert(F); }

    float_t x() const noexcept { return x0; }
    float_t y() const noexcept { return y0; }
    float_t accuracy() const noexcept {
      return FP::accuracyg<float_t>(sol(x0), y0, FP::PrecZ::eps);
    }
    void reset(const float_t x1, const float_t y1) noexcept {
      x0 = x1; y0 = y1;
    }
    // For providing a more accurate value:
    void precise_x0(const float_t x1) noexcept { x0 = x1; }

    void step(const float_t delta) noexcept {
      const float_t k1 = F(x0,y0);
      const float_t d2 = delta / 2;
      const float_t xm = x0 + d2;
      const float_t k2 = F(xm, std::fma(d2, k1, y0));
      const float_t k3 = F(xm, std::fma(d2, k2, y0));
      x0 += delta;
      const float_t k4 = F(x0, std::fma(delta, k3, y0));
      y0 = std::fma(delta/6, k1 + 2*k2 + 2*k3 + k4, y0);
    }
    void steps(const float_t delta, const count_t N = default_N) noexcept {
      if (N == 0) return;
      if (N == 1) {step(delta); return;}
      const float_t old_x0 = x0;
      const float_t small_d = delta / N;
      for (count_t i = 0; i < N;) {
        const float_t k1 = F(x0,y0);
        const float_t d2 = small_d / 2;
        const float_t xm = x0 + d2;
        const float_t k2 = F(xm, std::fma(d2, k1, y0));
        const float_t k3 = F(xm, std::fma(d2, k2, y0));
        x0 = std::fma(++i, small_d, old_x0);
        const float_t k4 = F(x0, std::fma(small_d, k3, y0));
        y0 = std::fma(small_d/6, k1 + 2*k2 + 2*k3 + k4, y0);
      }
      x0 = old_x0 + delta;
    }

  };

  typedef struct RK41d<FP::float80> RK41d_80;
  typedef struct RK41d<FP::float64> RK41d_64;

  template <typename FLOAT>
  struct RK41d_auto {
    typedef FLOAT float_t;
    typedef std::function<float_t(float_t)> f_t;
    typedef std::function<float_t(float_t)> F_t;

    typedef FP::UInt_t count_t;
    static constexpr count_t default_N = 25'000;

    const F_t F;
    const f_t sol;

  private :
    float_t x0, y0;
  public :

    RK41d_auto(const float_t x0, const float_t y0, const F_t F, const f_t sol = f_t()) noexcept :
    F(F), sol(sol), x0(x0), y0(y0) { assert(F); }

    float_t x() const noexcept { return x0; }
    float_t y() const noexcept { return y0; }
    float_t accuracy() const {
      return FP::accuracyg<float_t>(sol(x0), y0, FP::PrecZ::eps);
    }

    void step(const float_t delta) noexcept {
      const float_t k1 = F(y0);
      const float_t d2 = delta / 2;
      const float_t k2 = F(std::fma(d2, k1, y0));
      const float_t k3 = F(std::fma(d2, k2, y0));
      const float_t k4 = F(std::fma(delta, k3, y0));
      y0 = std::fma(delta/6, k1 + 2*k2 + 2*k3 + k4, y0);
      x0 += delta;
    }
    void steps(const float_t delta, const count_t N = default_N) noexcept {
      if (N == 0) return;
      if (N == 1) {step(delta); return;}
      const float_t small_d = delta / N;
      for (count_t i = 0; i < N; ++i) {
        const float_t k1 = F(y0);
        const float_t d2 = small_d / 2;
        const float_t k2 = F(std::fma(d2, k1, y0));
        const float_t k3 = F(std::fma(d2, k2, y0));
        const float_t k4 = F(std::fma(small_d, k3, y0));
        y0 = std::fma(small_d/6, k1 + 2*k2 + 2*k3 + k4, y0);
      }
      x0 += delta;
    }
  };

  template <typename FLOAT, class VEC>
  struct RK4 {
    typedef FLOAT float_t;
    typedef VEC vec_t;

    typedef std::function<vec_t(float_t,vec_t)> F_t;
    typedef std::function<vec_t(float_t)> f_t;

    typedef FP::UInt_t count_t;
    static constexpr count_t default_N = 25'000;

    const F_t F;
    const f_t sol;

  private :
    float_t x0;
    vec_t y0;
  public :
    const count_t size;

    RK4(const float_t x0, const vec_t y0, const F_t F, const f_t sol = f_t()) noexcept :
    F(F), sol(sol), x0(x0), y0(y0), size(y0.size()) { assert(F); }

    float_t x() const noexcept { return x0; }
    const vec_t& y() const noexcept { return y0; }
    float_t accuracy() const {
      return FP::accuracyv<vec_t>(sol(x0), y0, FP::PrecZ::eps);
    }

  private :
    vec_t fma(const float_t d, const vec_t& k) {
      vec_t res(y0);
      for (count_t i = 0; i < size; ++i)
        res[i] = std::fma(d, k[i], res[i]);
      return res;
    }
    vec_t wadd(const vec_t& k1, const vec_t& k2, const vec_t& k3, const vec_t& k4) {
      vec_t res(k1);
      for (count_t i = 0; i < size; ++i)
        res[i] += 2*k2[i] + 2*k3[i] + k4[i];
      return res;
    }
  public :

    void step(const float_t delta) noexcept {
      const vec_t k1 = F(x0,y0);
      const float_t d2 = delta / 2;
      const float_t xm = x0 + d2;
      const vec_t k2 = F(xm, fma(d2, k1));
      const vec_t k3 = F(xm, fma(d2, k2));
      x0 += delta;
      const vec_t k4 = F(x0, fma(delta, k3));
      y0 = fma(delta/6, wadd(k1, k2, k3, k4));
    }
    void steps(const float_t delta, const count_t N = default_N) noexcept {
      if (N == 0) return;
      if (N == 1) {step(delta); return;}
      const float_t old_x0 = x0;
      const float_t small_d = delta / N;
      for (count_t i = 0; i < N;) {
        const vec_t k1 = F(x0,y0);
        const float_t d2 = small_d / 2;
        const float_t xm = x0 + d2;
        const vec_t k2 = F(xm, fma(d2, k1));
        const vec_t k3 = F(xm, fma(d2, k2));
        x0 = std::fma(++i, small_d, old_x0);
        const vec_t k4 = F(x0, fma(small_d, k3));
        y0 = fma(small_d/6, wadd(k1, k2, k3, k4));
      }
      x0 = old_x0 + delta;
    }
  };


  typedef std::array<FP::float80, 2> vec80_2d;
  typedef std::array<FP::float80, 3> vec80_3d;

}

#endif
