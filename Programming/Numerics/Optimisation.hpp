// Oliver Kullmann, 10.12.2021 (Swansea)
/* Copyright 2021 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Tools for mathematical optimisation

*/

#ifndef OPTIMISATION_2FjdZTuPIn
#define OPTIMISATION_2FjdZTuPIn

#include <vector>
#include <algorithm>
#include <ostream>

#include <cmath>
#include <cassert>

#include "FloatingPoint.hpp"

namespace Optimisation {

  namespace FP = FloatingPoint;

  typedef FP::float80 x_t;
  typedef FP::float80 y_t;
  typedef FP::UInt_t index_t;


  typedef std::vector<x_t> vec_t;
  inline bool valid(const vec_t& v) noexcept {
    if (v.empty()) return false;
    else
      return std::all_of(v.begin(), v.end(),
                         [](const x_t x){return x>=0;});
  }


  typedef std::function<y_t(const vec_t&)> function_t;


  struct point_t {
    x_t x; y_t y;
  };
  inline bool operator ==(const point_t& lhs, const point_t& rhs) noexcept {
    return lhs.x == rhs.x and lhs.y == rhs.y;
  }
  std::ostream& operator <<(std::ostream& out, const point_t& p) {
    return out << p.x << "," << p.y;
  }
  inline constexpr bool valid(const point_t& p) noexcept {
    return p.x >= 0 and p.y >= 0;
  }


  typedef std::vector<point_t> list_points_t;
  inline bool valid(const list_points_t& v) noexcept {
    return std::all_of(v.begin(), v.end(),
                       [](const point_t& p){return valid(p);});
  }


  inline point_t eval(const function_t f, const vec_t& x, const index_t i ) noexcept {
    assert(i < x.size());
    return {x[i], f(x)};
  }


  struct fpoint_t {
    vec_t x; y_t y;
  };
  inline bool operator ==(const fpoint_t& lhs, const fpoint_t& rhs) noexcept {
    return lhs.x == rhs.x and lhs.y == rhs.y;
  }
  inline bool valid(const fpoint_t& p) noexcept {
    return valid(p.x) and p.y >= 0;
  }


  struct Interval {
    x_t l, r, hl, hr; // left-/right soft and hard bounds

    constexpr Interval
      (const x_t l, const x_t r, const x_t hl, const x_t hr) noexcept :
      l(l), r(r), hl(hl), hr(hr) {}
    constexpr Interval(const x_t l, const x_t r) noexcept :
      Interval(l,r,0,FP::pinfinity) {}
    constexpr Interval() noexcept : Interval(0,0) {}
  };
  inline constexpr bool valid(const Interval& I) noexcept {
    return I.l >= I.hl and I.r >= I.l and I.hr >= I.r;
  }
  inline constexpr bool element(const x_t x, const Interval& I) noexcept {
    return x >= I.l and x <= I.r;
  }
  inline constexpr bool element(const point_t& p, const Interval& I) noexcept {
    return element(p.x, I);
  }


  typedef std::vector<Interval> list_intervals_t;
  inline bool valid(const list_intervals_t& v) noexcept {
    return std::all_of(v.begin(), v.end(),
                       [](const Interval& I){return valid(I);});
  }


  // Is list v element in the cube given by the intervals in I:
  inline bool element(const vec_t& v, const list_intervals_t& I) noexcept {
    const index_t N = v.size();
    assert(I.size() >= N);
    for (index_t i = 0; i < N; ++i)
      if (not element(v[i], I[i])) return false;
    return true;
  }
  inline bool element(const list_points_t& v, const list_intervals_t& I) noexcept {
    const index_t N = v.size();
    assert(I.size() >= N);
    for (index_t i = 0; i < N; ++i)
      if (not element(v[i], I[i])) return false;
    return true;
  }


  inline y_t min_value_points(const list_points_t& v) noexcept {
    assert(not v.empty());
    return std::min_element(v.begin(), v.end(),
      [](const point_t& a, const point_t& b) noexcept {return a.y < b.y;}) ->y;
  }

  inline point_t min_argument_points(const list_points_t& v) noexcept {
    assert(not v.empty());
    const y_t minval = min_value_points(v);
    std::vector<x_t> minargs;
    for (const auto& p : v)
      if (p.y == minval) minargs.push_back(p.x);
    assert(not minargs.empty());
    return {minargs[(minargs.size()-1)/2], minval};
  }



  inline constexpr bool valid_partitionsize(const index_t N) noexcept {
    return N >= 1 and N < FP::P264m1-1;
  }

  point_t bbopt_index(vec_t x, const y_t y0, const index_t i, const Interval I, const function_t f, const index_t N) noexcept {
    assert(valid(x));
    assert(f(x) == y0);
    assert(i < x.size());
    assert(valid(I));
    assert(element(x[i], I));
    assert(valid_partitionsize(N));

    const x_t x0 = x[i];
    if (I.l == I.r) return {x0,y0};
    const x_t delta = (I.r - I.l) / N;
    assert(delta > 0);
    bool inserted = false;
    list_points_t results; results.reserve(N+2);
    for (index_t j = 0; j <= N; ++j) {
      const x_t x1 = std::fma(j, delta, I.l);
      if (x1 == x0) {
        assert(not inserted);
        results.push_back({x0,y0});
        inserted = true;
      }
      else {
        if (not inserted and x1 > x0) {
          results.push_back({x0,y0});
          inserted = true;
        }
        x[i] = x1;
        const y_t y1 = f(x);
        results.push_back({x1,y1});
      }
    }
    assert(inserted);
    assert(results.size()==N+1 or results.size()==N+2);
    return min_argument_points(results);
  }

  void shrink_intervals(const vec_t& x, list_intervals_t& Iv, const x_t factor = 2) noexcept {
    assert(valid(x));
    assert(valid(Iv));
    assert(element(x,Iv));
    assert(factor > 0);
    const auto size = x.size();
    for (index_t i = 0; i < size; ++i) {
      Interval& I = Iv[i];
      const x_t delta = (I.r - I.l) / 2;
      const x_t ndelta = delta / factor;
      const x_t x0 = x[i];
      I.l = std::max(I.hl, x0 - ndelta);
      I.r = std::min(I.hr, x0 + ndelta);
    }
    assert(valid(Iv));
    assert(element(x,Iv));
  }

  struct Parameters {
    index_t N,
      R=1, // rounds
      S=1, // shrinkings
      T=1; // threads
    constexpr Parameters(const index_t N, const index_t R=1, const index_t S=1, const index_t T=1) noexcept : N(N), R(R), S(S), T(T) {}
  };
  inline constexpr bool valid(const Parameters& P) noexcept {
    return valid_partitionsize(P.N) and P.S >= 1 and P.T >= 1;
  }

  fpoint_t bbopt_rounds(fpoint_t p, list_intervals_t I, const function_t f, const Parameters P) noexcept {
    assert(valid(p));
    assert(f(p.x) == p.y);
    assert(valid(I));
    assert(element(p.x,I));
    assert(valid(P));

    const index_t size = p.x.size();
    for (index_t s = 0; s < P.S; ++s) {
      for (index_t r = 0; r < P.R; ++r)
        for (index_t i = 0; i < size; ++i) {
          const point_t opt = bbopt_index(p.x, p.y, i, I[i], f, P.N);
          p.x[i] = opt.x; p.y = opt.y;
        }
      shrink_intervals(p.x, I);
    }
    return p;
  }

}

#endif
