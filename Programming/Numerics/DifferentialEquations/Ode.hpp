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
    - As the current module Statistics in Random.

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
    float_t accuracy() const {
      return FP::accuracyg<float_t>(sol(x0), y0, FP::PrecZ::eps);
    }
    void reset(const float_t x1, const float_t y1) noexcept {
      x0 = x1; y0 = y1;
    }

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

    typedef std::array<float_t, 2> point_t;
    typedef std::vector<point_t> points_vt;
  private :
    count_t N; // N >= 1 is the number of sub-intervals
    count_t ssi; // steps for sub-intervals
    count_t iN; // steps for initial move
    float_t a0, b0; // left and right border of interval
    bool left, right; // whether the border are included

    float_t xmin0, xmax0;
    float_t ymin0, yminx0, ymax0, ymaxx0, ymean0, ysd0;
    float_t accmin0, accmax0, accmaxx0, accmean0, accsd0, accmed0;

    points_vt pv; // vector of points (x,f(x))
    points_vt acc; // (x, accuracy)

  public :
    float_t a() const noexcept { return a0; }
    float_t b() const noexcept { return b0; }
    bool left_included() const noexcept { return left; }
    bool right_included() const noexcept { return right; }

    float_t xmin() const noexcept { return xmin0; }
    float_t xmax() const noexcept { return xmax0; }
    float_t ymin() const noexcept { return ymin0; }
    float_t yminx() const noexcept { return yminx0; }
    float_t ymax() const noexcept { return ymax0; }
    float_t ymaxx() const noexcept { return ymaxx0; }
    float_t ymean() const noexcept { return ymean0; }
    float_t ysd() const noexcept { return ysd0; }
    const points_vt& points() const noexcept { return pv; }

    float_t accmin() const noexcept { return accmin0; }
    float_t accmax() const noexcept { return accmax0; }
    float_t accmaxx() const noexcept { return accmaxx0; }
    float_t accmean() const noexcept { return accmean0; }
    float_t accsd() const noexcept { return accsd0; }
    float_t accmed() const noexcept { return accmed0; }
    const points_vt& accuracies() const noexcept { return acc; }

    inline static std::pair<count_t, float_t> best(const float_t a0, const float_t b0, const float_t delta, const float_t x0, const bool left, const bool right, const count_t N) noexcept {
      assert(N >= 2);
      const float_t i = (x0-a0) / delta;
      const count_t fi = std::floor(i), ci = std::ceil(i);
      const float_t xf = a0 + fi * delta, xc = a0 + ci * delta;
      if (std::abs(xf - x0) < std::abs(xc - x0)) {
        if (left or fi != 0) return {fi, xf};
        else return {1, a0+delta};
      }
      else {
        if (ci == 0) if (left) return {0, a0}; else return {1, a0+delta};
        if (right or ci != N) return {ci, xc};
        else return {N-1, b0-delta};
      }
    }

    void interval(const float_t b, const bool bi, const float_t e, const bool ei, const count_t bs, const count_t ss, const count_t is = default_N) {
      N = bs; ssi = ss; iN = is;
      a0 = b; b0 = e; left = bi; right = ei;
      assert(N >= 1 and ssi >= 1 and iN >= 1);
      assert(a0 < b0);
      pv.clear();
      const count_t size = N + 1 - not left - not right;
      if (size == 0) return;
      if (x0 < a0) { steps(a0 - x0, iN); x0 = a0; }
      else if (x0 > b0) { steps(b0 - x0, iN); x0 = b0; }
      assert(x0 <= x0 and x0 <= b0);

      if (size == 1) {
        assert(not left or not right);
        if (left) {
          if (x0 != a0) { steps(a0 - x0, ssi); x0 = a0; }
        }
        else {
          if (x0 != b0) { steps(b0 - x0, ssi); x0 = a0; }
        }
        pv.push_back({x0,y0});
        assert(pv.size() == size); return;
      }
      assert(size >= 2);
      if (x0 == a0) {
        const float_t delta = (b0 - a0) / N;
        if (left) pv.push_back({x0,y0});
        for (count_t i = 1; i < N; ++i) {
          steps(delta, ssi);
          x0 = a0 + i * delta;
          pv.push_back({x0,y0});
        }
        if (right) {
          steps(delta, ssi);
          x0 = b0;
          pv.push_back({x0,y0});
        }
        assert(pv.size() == size); return;
      }
      else if (x0 == b0) {
        const float_t delta = (a0 - b0) / N;
        if (right) pv.push_back({x0,y0});
        for (count_t i = 1; i < N; ++i) {
          steps(delta, ssi);
          x0 = b0 + i * delta;
          pv.push_back({x0,y0});
        }
        if (left) {
          steps(delta, ssi);
          x0 = a0;
          pv.push_back({x0,y0});
        }
        std::reverse(pv.begin(), pv.end());
        assert(pv.size() == size); return;
      }
      else {
        assert(a0 < x0 and x0 < b0);
        const float_t orig_x0 = x0, orig_y0 = y0;
        const float_t diffl = a0 - x0, diffr = b0 - x0;
        assert(diffl < 0 and diffr > 0);
        if (N == 1) {
          assert(left and right);
          steps(diffl, ssi);
          x0 = a0; pv.push_back({x0,y0});
          x0 = orig_x0; y0 = orig_y0;
          steps(diffr, ssi);
          x0 = b0; pv.push_back({x0,y0});
          assert(pv.size() == size); return;
        }

        assert(N >= 2);
        const float_t delta = (b0 - a0) / N;
        const auto [i_middle, x0_middle] = best(a0,b0, delta,x0, left,right,N);
        assert(i_middle <= N and x0_middle == a0 + i_middle*delta);
        steps(x0_middle - x0, ssi);
        const float_t y0_middle = y0;
        x0 = x0_middle;
        if (i_middle == 0) {
          if (left) pv.push_back({x0,y0});
          for (count_t i = 1; i < N-1; ++i) {
            steps(delta, ssi);
            x0 = a0 + i * delta;
            pv.push_back({x0,y0});
          }
          if (right) {
            steps(delta, ssi);
            x0 = b0;
            pv.push_back({x0,y0});
          }
          assert(pv.size() == size); return;
        }
        else if (i_middle == N) {
           const float_t deltan = -delta;
           if (right) pv.push_back({x0,y0});
           for (count_t i = 1; i < N-1; ++i) {
             steps(deltan, ssi);
             x0 = b0 + i * deltan;
             pv.push_back({x0,y0});
           }
           if (left) {
             steps(deltan, ssi);
             x0 = a0;
             pv.push_back({x0,y0});
           }
           std::reverse(pv.begin(), pv.end());
           assert(pv.size() == size); return;
        }
        else {
          pv.push_back({x0,y0});
          {const float_t deltan = -delta;
           for (count_t i = i_middle; i != 1; --i) {
             steps(deltan, ssi);
             x0 = a0 + (i-1) * delta;
             pv.push_back({x0,y0});
           }
           if (left) {
             steps(deltan, ssi);
             x0 = a0;
             pv.push_back({x0,y0});
           }
           std::reverse(pv.begin(), pv.end());
          }
          x0 = x0_middle;
          y0 = y0_middle;
          for (count_t i = i_middle+1; i != N; ++i) {
            steps(delta, ssi);
            x0 = a0 + i * delta;
            pv.push_back({x0,y0});
          }
          if (right) {
            steps(delta, ssi);
            x0 = b0;
            pv.push_back({x0,y0});
          }
          assert(pv.size() == size); return;
        }
      }
    }

    void update_stats() {
      if (pv.empty()) {
        xmin0 = std::numeric_limits<float_t>::infinity();
        xmax0 = -std::numeric_limits<float_t>::infinity();
        ymin0 = std::numeric_limits<float_t>::infinity();
        ymax0 = -std::numeric_limits<float_t>::infinity();
        ymean0 = 0; ysd0 = 0;
      }
      else {
        assert(is_sorted(pv.begin(), pv.end()));
        xmin0 = pv.front()[0]; xmax0 = pv.back()[0];
        ymin0 = pv.front()[1]; ymax0 = ymin0;
        yminx0 = xmin0; ymaxx0 = yminx0;
        float_t sum = ymin0;
        const size_t size = pv.size();
        for (size_t i = 1; i < size; ++i) {
          const float_t y = pv[i][1];
          sum += y;
          if (y < ymin0) { ymin0 = y; yminx0 = pv[i][0]; }
          if (y > ymax0) { ymax0 = y; ymaxx0 = pv[i][0]; }
        }
        ymean0 = sum / size;
        sum = 0;
        for (const auto& p : pv) {
          const float_t diff = p[1] - ymean0;
          sum += diff*diff;
        }
        ysd0 = std::sqrt(sum / size);
      }
    }

    void update_accuracies() {
      acc.clear();
      accmin0 = std::numeric_limits<float_t>::infinity();
      accmax0 = -std::numeric_limits<float_t>::infinity();
      if (pv.empty()) { accmean0 = 0; accsd0 = 0; accmed0 = 0; }
      else {
        acc.reserve(pv.size());
        float_t sum = 0;
        for (const auto [x,y] : pv) {
          const auto a = FP::accuracyg<float_t>(sol(x), y, FP::PrecZ::eps);
          acc.push_back({x,a});
          sum += a;
          accmin0 = std::min(accmin0, a);
          if (a > accmax0) { accmax0 = a; accmaxx0 = x; }
        }
        const size_t size = pv.size();
        accmean0 = sum / size;
        sum = 0;
        std::vector<float_t> a; a.reserve(acc.size());
        for (const auto& p : acc) {
          const float_t diff = p[1] - accmean0;
          sum += diff*diff;
          a.push_back(p[1]);
        }
        accsd0 = std::sqrt(sum / size);
        std::sort(a.begin(), a.end());
        accmed0 = GenStats::median<float_t>(a);
      }
    }

    friend std::ostream& operator <<(std::ostream& out, const RK41d& rk) {
      using W = FP::WrapE<float_t>;
      out << rk.N << " " << rk.ssi << " " << rk.iN << "\n"
        "x  : " << rk.xmin() << " " << std::midpoint(rk.xmin(), rk.xmax())
          << " " << rk.xmax() << "\n"
        "y  :\n  (" << rk.ymin() << ", " << rk.yminx() << ")\n  "
          << std::midpoint(rk.ymin(), rk.ymax()) << "\n  "
        "("  << rk.ymax() << ", " << rk.ymaxx() << ")\n"
        "  ms : " << rk.ymean() << "  " << rk.ysd() << "\n"
        "span_y / span_x = " <<
        (rk.ymax() - rk.ymin()) / (rk.xmax() - rk.xmin()) << "\n"
        "acc: " << W(rk.accmin()) << "  "
        "(" << W(rk.accmax()) << ", " << rk.accmaxx() << ")\n"
        "  msm: " <<
        W(rk.accmean()) << "  " << W(rk.accsd()) << "  " << W(rk.accmed())
          << "\n";
      return out;
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
