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


  struct interval_t {
    x_t l, r;
  };
  inline constexpr bool valid(const interval_t& I) noexcept {
    return I.l >= 0 and I.r >= I.l;
  }
  inline constexpr bool element(const x_t x, const interval_t& I) noexcept {
    return x >= I.l and x <= I.r;
  }
  inline constexpr bool element(const point_t& p, const interval_t& I) noexcept {
    return element(p.x, I);
  }


  typedef std::vector<interval_t> list_intervals_t;
  inline bool valid(const list_intervals_t& v) noexcept {
    return std::all_of(v.begin(), v.end(),
                       [](const interval_t& I){return valid(I);});
  }


  // Is list v element in the cube given by the intervals in I:
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

  inline x_t min_argument_points(const list_points_t& v) noexcept {
    assert(not v.empty());
    const y_t minval = min_value_points(v);
    std::vector<index_t> minargs;
    for (const auto& p : v)
      if (p.y == minval) minargs.push_back(p.x);
    assert(not minargs.empty());
    return minargs[(minargs.size()-1)/2];
  }


  x_t bbopt_index(vec_t x, const y_t y0, const index_t i, const interval_t I, const function_t f, const index_t N) noexcept {
    assert(valid(x));
    assert(f(x) == y0);
    assert(i < x.size());
    assert(valid(I));
    assert(element(x[i], I));
    assert(N >= 1);
    assert(N < FP::P264m1-1);

    const x_t x0 = x[i];
    if (I.l == I.r) return x0;
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

}

#endif
