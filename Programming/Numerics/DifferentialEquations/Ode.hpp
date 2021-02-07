// Oliver Kullmann, 6.2.2021 (Swansea)
/* Copyright 2021 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  Tools for numerical solution of differential equations

*/

#ifndef ODE_4HkvmZBVgf
#define ODE_4HkvmZBVgf

#include <functional>

// Guaranteed to be included:
#include <Numerics/FloatingPoint.hpp>

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

    const F_t fp;
    const f_t sol;

  private :
    float_t x0, y0;
  public :

    Euler1d(const float_t x0, const float_t y0, const F_t fp, const f_t sol = f_t()) noexcept :
    fp(fp), sol(sol), x0(x0), y0(y0) {}

    float_t x() const noexcept { return x0; }
    float_t y() const noexcept { return y0; }
    float_t accuracy() const { return FP::accuracyg<float_t>(sol(x0), y0); }

    void step(const float_t delta) noexcept {
      y0 = FP::fma(delta, fp(x0,y0), y0);
      x0 += delta;
    }
    void steps(const float_t delta, const count_t N = default_N) noexcept {
      if (N == 0) return;
      if (N == 1) {step(delta); return;}
      const float_t old_x0 = x0;
      const float_t small_d = delta / N;
      for (count_t i = 0; i < N;) {
        y0 = FP::fma(small_d, fp(x0,y0), y0);
        x0 = FP::fma(++i, small_d, old_x0);
      }
      x0 = old_x0 + delta;
    }
  };

}

#endif
