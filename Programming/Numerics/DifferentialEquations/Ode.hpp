// Oliver Kullmann, 6.2.2021 (Swansea)
/* Copyright 2021 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

*/

#ifndef ODE_4HkvmZBVgf
#define ODE_4HkvmZBVgf

#include <functional>

// Guaranteed to be included:
#include <Numerics/FloatingPoint.hpp>

namespace Ode {

  namespace FP = FloatingPoint;
  using FP::float80;

  struct Euler1d {
    typedef FP::UInt_t count_t;
    static constexpr count_t default_N = 100'000;

    typedef std::function<float80(float80,float80)> function_t;

    const function_t fp;

  private :
    float80 x0, y0;
  public :

    Euler1d(const float80 x0, const float80 y0, const function_t fp) noexcept :
    fp(fp), x0(x0), y0(y0) {}

    float80 x() const noexcept { return x0; }
    float80 y() const noexcept { return y0; }

    void step(const float80 delta) {
      y0 = FP::fma(delta, fp(x0,y0), y0);
      x0 += delta;
    }
    void steps(const float80 delta, const count_t N = default_N) {
      if (N == 0) return;
      if (N == 1) {step(delta); return;}
      const float80 old_x0 = x0;
      const float80 small_d = delta / N;
      for (count_t i = 0; i < N;) {
        y0 = FP::fma(small_d, fp(x0,y0), y0);
        x0 = FP::fma(++i, small_d, old_x0);
      }
      x0 = old_x0 + delta;
    }
  };

}

#endif
