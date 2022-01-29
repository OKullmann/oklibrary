// Oliver Kullmann, 29.1.2022 (Swansea)
/* Copyright 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Types for optimisation components

   - x_t, y_t
   - vec_t (vector of x_t), valid(vec_t)
   - evec_t (vector of x_t or integers)
   - index_t
   - function_t ((vec_t, y_t) -> y_t)

   - point_t:
     - data-members x (x_t), y
     - valid(point_t)
     - operators ==, <<
   - list_points_t (vector of point_t), valid(list_points_t)
   - fpoint_t:
     - data-members x (vec_t), y
     - valid(fpoint_t)
     - operators ==, <<

   - Interval:
     - data-members l, r, hl, hr (all x_t)
     - constructors:
      - Interval()
      - Interval(l,r)
      - Interval(l,r.hl,hr)
     - valid(Interval)
     - operators ==
   - list_intervals_t (vector of Interval), valid(list_intervals_t)

    - element(x_t, Interval)
    - element(point_t, Interval)
    - element(vec_t, list_intervals_t)
    - element(evec_t, list_intervals_t) (ignoring the integral-elements)
    - element(list_points_t, list_intervals_t)


*/

#ifndef OPTTYPES_YHJUpn7ZfD
#define OPTTYPES_YHJUpn7ZfD

#include <vector>
#include <algorithm>
#include <ostream>
#include <functional>

#include <cassert>

#include "NumTypes.hpp"

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
                         [](const x_t x){return not FP::isnan(x);});
  }


  /*
    The "underlying function" f : x_t -> y_t (this is f0_t)
    is expanded by an additional argument b, a "known upper bound",
    yielding function_t:

    f(x, b) = f(x) if f(x) <= b, while otherwise f(x,b) is any
    value > b.

    Thus f(x) = f(x, FP::pinfinity).
  */

  typedef std::function<y_t(const vec_t&)> f0_t;
  typedef std::function<y_t(const vec_t&, x_t)> function_t;


  struct point_t {
    x_t x; y_t y;
  };
  inline bool valid(const point_t p) noexcept {
    return not FP::isnan(p.x) and not FP::isnan(p.y);
  }
  inline constexpr bool operator ==(const point_t& lhs, const point_t& rhs) noexcept {
    return lhs.x == rhs.x and lhs.y == rhs.y;
  }
  std::ostream& operator <<(std::ostream& out, const point_t p) {
    assert(valid(p));
    return out << p.x << "," << p.y;
  }

  typedef std::vector<point_t> list_points_t;
  inline bool valid(const list_points_t& v) noexcept {
    return std::all_of(v.begin(), v.end(),
                       [](const point_t p){return valid(p);});
  }


  struct fpoint_t {
    vec_t x; y_t y;
  };
  inline bool valid(const fpoint_t& p) noexcept {
    return valid(p.x) and not FP::isnan(p.y);
  }
  inline bool operator ==(const fpoint_t& lhs, const fpoint_t& rhs) noexcept {
    return lhs.x == rhs.x and lhs.y == rhs.y;
  }
  std::ostream& operator <<(std::ostream& out, const fpoint_t& p) {
    assert(valid(p));
    out << "(" << p.x[0];
    for (index_t i = 1; i < p.x.size(); ++i) out << "," << p.x[i];
    return out << ")," << p.y;
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
  inline constexpr bool operator ==(const Interval& lhs, const Interval& rhs) noexcept {
    return lhs.l==rhs.l and lhs.r==rhs.r and lhs.hl==rhs.hl and rhs.hr==rhs.hr;
  }
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


  // Is vector v element in the cube given by the intervals in I:
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


  typedef std::vector<FP::F80ai> evec_t ;

  inline bool element(const evec_t& v, const list_intervals_t& I) noexcept {
    const index_t N = v.size();
    assert(I.size() >= N);
    for (index_t i = 0; i < N; ++i)
      if (not v[i].isint and not element(v[i].x, I[i])) return false;
    return true;
  }


}

#endif
