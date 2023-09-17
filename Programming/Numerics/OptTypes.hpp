// Oliver Kullmann, 29.1.2022 (Swansea)
/* Copyright 2022, 2023 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Types for optimisation components

   Typedefs:
   - x_t, y_t (both just float80)
   - vec_t (vector of x_t)
     valid(vec_t) (not empty and no NaN)
   - evec_t (vector of F80ai)
   - index_t (UInt_t)
   - function_t ((vec_t, y_t) -> y_t)
     (the "underlying function" f0_t just is vec_t -> y_t)

   - struct point_t:
     - data-members x : x_t, y : y_t
     - valid(point_t) (none NaN)
     - operators == (default), << (comma-separated)
   - typedef list_points_t (vector of point_t)
     valid(list_points_t) (all valid)

   - struct fpoint_t:
     - data-members x : vec_t, y : y_t
     - valid(fpoint_t) (none NaN)
     - operators ==, << ( "(x_1,...,x_n),y" )

   - Interval:
     - data-members l, r, hl, hr (all x_t; l,r "soft" bounds, hl, hr "hard")
     - constructors:
      - Interval(l,r, hl,hr)
      - Interval(l,r) = Interval(l,r, hl = 0, hr = +inf))
      - Interval() = Interval(0,0)
     - valid(Interval)
     - operators ==, <<
   - typedef list_intervals_t (vector of Interval)
     valid(list_intervals_t)

    - element(x_t, Interval)
    - element(point_t, Interval)
    - element(FP80ai, Interval)
    - element(vec_t, list_intervals_t)
    - element(evec_t, list_intervals_t) (ignoring the integral-elements)
    - element(list_points_t, list_intervals_t)

   - Scanning of lines like "-10 -1 77 1 10", which mean the hard-interval
     [-10, 10], the soft-interval [-1,1], and 77 sub-intervals:
     - typedef scanning_info_t (pair of list_intervals_t and evec_t)
       (in the above example, evec_t would have the component 77 with
       asserted int)
     - read_scanning_info(FP::table_wai_t lines)
       read_scanning_info(istream in)
       read_scanning_info(filesystem::path p)
         all -> scanning_info_t
     - hash2(scanning_info_t) -> array<FP::UInt_t, 2>
         computes a pair of hashes, based on
         hash(Intervals) -> FP::UInt_t

   - scoped enum Error


*/

#ifndef OPTTYPES_YHJUpn7ZfD
#define OPTTYPES_YHJUpn7ZfD

#include <vector>
#include <algorithm>
#include <ostream>
#include <functional>
#include <stdexcept>
#include <filesystem>
#include <utility>
#include <array>

#include <cassert>

#include "NumTypes.hpp"
#include "NumInOut.hpp"
#include "NumBasicFunctions.hpp"

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
    friend constexpr bool operator ==(const point_t&, const point_t&) noexcept;
  };
  inline constexpr bool operator ==(const point_t&, const point_t&)
    noexcept = default;
  inline bool valid(const point_t p) noexcept {
    return not FP::isnan(p.x) and not FP::isnan(p.y);
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
    friend bool operator ==(const fpoint_t& lhs, const fpoint_t& rhs) noexcept;
  };
  inline bool operator ==(const fpoint_t& lhs, const fpoint_t& rhs) noexcept = default;
  inline bool valid(const fpoint_t& p) noexcept {
    return valid(p.x) and not FP::isnan(p.y);
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

    friend constexpr bool operator ==(const Interval&, const Interval&)
      noexcept;
  };
  inline constexpr bool operator ==(const Interval&, const Interval&)
    noexcept = default;
  std::ostream& operator <<(std::ostream& out, const Interval& I) {
    return out << "[" << I.hl << " [" << I.l << "," << I.r << "] " << I.hr
               << "]";
  }
  inline FP::UInt_t hash(const Interval& I) noexcept {
    FP::UInt_t seed = 4;
    FP::hash_combine(seed, FP::hash(I.l));
    FP::hash_combine(seed, FP::hash(I.r));
    FP::hash_combine(seed, FP::hash(I.hl));
    FP::hash_combine(seed, FP::hash(I.hr));
    return seed;
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
  inline bool element(const list_points_t& v, const list_intervals_t& I)
    noexcept {
    const index_t N = v.size();
    assert(I.size() >= N);
    for (index_t i = 0; i < N; ++i)
      if (not element(v[i], I[i])) return false;
    return true;
  }


  typedef std::vector<FP::F80ai> evec_t ;

  inline bool element(const FP::F80ai x, const Interval& I) noexcept {
    return x.isint or element(x.x, I);
  }
  inline bool element(const evec_t& v, const list_intervals_t& I) noexcept {
    const index_t N = v.size();
    assert(I.size() >= N);
    for (index_t i = 0; i < N; ++i)
      if (not element(v[i], I[i])) return false;
    return true;
  }


  typedef std::pair<list_intervals_t, evec_t> scanning_info_t;

  scanning_info_t read_scanning_info(const FP::table_wai_t& lines) {
    const auto N = lines.size();
    scanning_info_t res;
    res.first.reserve(N); res.second.reserve(N);
    for (const auto& line : lines) {
      const vec_t& v = line.first;
      if (v.size() != 4) {
        std::ostringstream ss;
        ss << "ERROR[Optimisation::read_scanning_info(lines)]: "
          "Vector with " << v.size() << " != 4 elements found;"
          " the vector is\n  ";
        Environment::out_line(ss, v);
        throw std::domain_error(ss.str());
      }
      Interval I(v[1],v[2], v[0],v[3]);
      if (not valid(I)) {
        std::ostringstream ss;
        ss << "ERROR[Optimisation::read_scanning_info(lines)]: "
          "Invalid interval (w.r.t. order) found; the interval is " << I;
        throw std::domain_error(ss.str());
      }
      const auto& x = line.second;
      if (not element(x, I)) {
        std::ostringstream ss;
        ss << "ERROR[Optimisation::read_scanning_info(lines)]: "
          "The value " << x.x << " is not element of the interval " << I;
        throw std::domain_error(ss.str());
      }
      res.first.push_back(std::move(I));
      res.second.push_back(x);
    }
    return res;
  }
  scanning_info_t read_scanning_info(const std::istream& in) {
    FP::table_wai_t lines;
    try { lines = FP::read_table_ai(in, 2); }
    catch (const std::exception& e) {
      std::ostringstream ss;
      ss << "ERROR[Optimisation::read_scanning_info(in)]: failed due to\n  "
         << typeid(e).name() << "\n  " << e.what();
      throw std::runtime_error(ss.str());
    }
    return read_scanning_info(std::move(lines));
  }
  scanning_info_t read_scanning_info(const std::filesystem::path& p) {
    FP::table_wai_t lines;
    try { lines = FP::read_table_ai(p, 2); }
    catch (const std::exception& e) {
      std::ostringstream ss;
      ss << "ERROR[Optimisation::read_scanning_info(p)]: failed due to\n  "
         << typeid(e).name() << "\n  " << e.what();
      throw std::runtime_error(ss.str());
    }
    return read_scanning_info(std::move(lines));
  }

  std::array<FP::UInt_t, 2> hash2(const scanning_info_t& S) noexcept {
    return {FP::hash_UInt_range().apply(S.first, hash),
            FP::hash_UInt_range()(S.second)};
  }


  enum class Error {
    missing_parameters = 1,
    faulty_parameters = 2,
    write_error = 3,
  };


}

#endif
