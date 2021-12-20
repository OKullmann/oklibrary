// Oliver Kullmann, 10.12.2021 (Swansea)
/* Copyright 2021 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Tools for mathematical optimisation

  Basic types:

   - x_t, y_t
   - vec_t (vector of x_t), valid(vec_t)
   - index_t
   - function_t (vec_t -> y_t)

   - point_t:
     - data-members x (x_t), y
     - valid(point_t)
     - operators ==, <<
   - list_points_t (vector of point_t), valid(list_points_t)
   - fpoint_t:
     - data-members x (vec_t), y
     - valid(fpoint_t)
     - operators ==

   - Interval:
     - data-members l, r, hl, hr (all x_t)
     - constructors:
      - Interval()
      - Interval(l,r)
      - Interval(l,r.hl,hr)
     - valid(Interval)
   - list_intervals_t (vector of Interval), valid(list_intervals_t)

   Basic functions:

    - eval(function_t, vec_t, i) -> point_t

    - member(x_t, Interval)
    - member(point_t, Interval)
    - member(vec_t, list_intervals_t)
    - member(list_points_t, list_intervals_t)

    - min_value_points(list_points_t) -> y_t
    - min_argument_points(list_points_t) -> point_t


    Algorithm bbopt_rounds (minimising coordinates independently in rounds,
    with shrinking of intervals):

      bbopt_rounds(fpoint_t p, list_intervals_t I, function_t f,
                   Parameters P)
      -> fpoint_t

    where

      Parameters:
       - data-members M, R, S, T
       - constructor Parameters(M, [R], [S], [T])
       - valid(Parameters).


TODOS:

1. valid(fpoint_t) requires y >= 0 --- should we do this?

2. Every function should have a unit-test.

OUTLOOK:

After having experience with the simple approach, perhaps we look into the
more advanced approaches:

1. An overview on black-box optimisation one finds at
     https://en.wikipedia.org/wiki/Derivative-free_optimization
     Derivative-free optimization

2. Randomised search is overviewed at
     https://en.wikipedia.org/wiki/Random_search
     Random search

3. Also relevant should be "Multilevel Coordinate Search (MCS)"
     https://en.wikipedia.org/wiki/MCS_algorithm
     MCS algorithm

4. Concerning shrinking of intervals,
     https://en.wikipedia.org/wiki/Golden-section_search
     Golden-section search

   should be relevant.
   The assumption of having only one minimum seems reasonable (see
   https://en.wikipedia.org/wiki/Unimodality#Unimodal_function ),
   but noise needs to be added.
   Alternatively there is
     https://en.wikipedia.org/wiki/Ternary_search
     Ternary search.

*/

#ifndef OPTIMISATION_2FjdZTuPIn
#define OPTIMISATION_2FjdZTuPIn

#include <vector>
#include <algorithm>
#include <ostream>
#include <thread>
#include <string>

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
  inline constexpr bool valid(const point_t& p) noexcept {
    return p.x >= 0 and p.y >= 0;
  }
  inline bool operator ==(const point_t& lhs, const point_t& rhs) noexcept {
    return lhs.x == rhs.x and lhs.y == rhs.y;
  }
  std::ostream& operator <<(std::ostream& out, const point_t& p) {
    assert(valid(p));
    return out << p.x << "," << p.y;
  }


  typedef std::vector<point_t> list_points_t;
  inline bool valid(const list_points_t& v) noexcept {
    return std::all_of(v.begin(), v.end(),
                       [](const point_t& p){return valid(p);});
  }


  inline point_t eval(const function_t f, const vec_t& x, const index_t i) noexcept {
    assert(i < x.size());
    return {x[i], f(x)};
  }


  struct fpoint_t {
    vec_t x; y_t y;
  };
  inline bool valid(const fpoint_t& p) noexcept {
    return valid(p.x) and p.y >= 0;
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


  /*
     Algorithm bbopt_rounds
  */

  inline constexpr bool valid_partitionsize(const index_t M) noexcept {
    return M >= 1 and M < FP::P264m1-1;
  }

  point_t bbopt_index(vec_t x, const y_t y0, const index_t i, const Interval I, const function_t f, const index_t M) noexcept {
    assert(valid(x));
    assert(f(x) == y0);
    assert(i < x.size());
    assert(valid(I));
    assert(element(x[i], I));
    assert(valid_partitionsize(M));

    const x_t x0 = x[i];
    if (I.l == I.r) return {x0,y0};
    const x_t delta = (I.r - I.l) / M;
    assert(delta > 0);
    bool inserted = false;
    list_points_t results; results.reserve(M+2);
    for (index_t j = 0; j <= M; ++j) {
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
    assert(results.size()==M+1 or results.size()==M+2);
    return min_argument_points(results);
  }

  struct Computation {
    vec_t x;
    function_t f;
    point_t* target;
    const Computation* next = nullptr;
    void operator()() const noexcept {
      const y_t y = f(x);
      target->y = y;
      if (next == nullptr) return;
      else next->operator()();
    }
  };

  point_t bbopt_index_parallel(vec_t x, const y_t y0, const index_t i, const Interval I, const function_t f, const index_t M, const index_t T) noexcept {
    assert(valid(x));
    assert(f(x) == y0);
    assert(i < x.size());
    assert(valid(I));
    assert(element(x[i], I));
    assert(valid_partitionsize(M));
    assert(T >= 2);

    const x_t x0 = x[i];
    if (I.l == I.r) return {x0,y0};
    const x_t delta = (I.r - I.l) / M;
    assert(delta > 0);
    bool inserted = false;
    list_points_t results; results.reserve(M+2);
    std::vector<Computation> computations; computations.reserve(M+1);
    for (index_t j = 0; j <= M; ++j) {
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
        results.push_back({x1,FP::pinfinity});
        computations.push_back({x, f, &results.back()});
      }
    }
    assert(inserted);
    assert(results.size()==M+1 or results.size()==M+2);
    const auto csize = computations.size();
    assert(csize==M or csize==M+1);

    for (index_t i = 0; i+T < csize; ++i)
      computations[i].next = &computations[i+T];
    std::vector<std::thread> threads; threads.reserve(csize);
    for (const Computation c : computations)
      threads.push_back(std::thread(c));
    for (auto& t : threads) t.join();
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
    index_t M,
      R, // rounds
      S, // shrinking-rounds (S=1 means no shrinking)
      T; // threads (T=1 means sequential computing)
    constexpr Parameters(const index_t M, const index_t R=1, const index_t S=1, const index_t T=1) noexcept : M(M), R(R), S(S), T(T) {}
    Parameters(const std::string& M, const std::string& R, const std::string& S, const std::string& T) :
      M(FP::toUInt(M)), R(FP::toUInt(R)), S(FP::toUInt(S)),
      T(FP::touint(T)) {}
  };
  inline constexpr bool operator ==(const Parameters& lhs, const Parameters& rhs) noexcept {
    return lhs.M==rhs.M and lhs.R==rhs.R and lhs.S==rhs.S and lhs.T==rhs.T;
  }
  inline constexpr bool valid(const Parameters& P) noexcept {
    return valid_partitionsize(P.M) and P.S >= 1 and P.T >= 1;
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
          const point_t opt = P.T == 1 ?
            bbopt_index(p.x, p.y, i, I[i], f, P.M) :
            bbopt_index_parallel(p.x, p.y, i, I[i], f, P.M, P.T);
          p.x[i] = opt.x; p.y = opt.y;
        }
      shrink_intervals(p.x, I);
    }
    return p;
  }

  template <class FUNC>
  fpoint_t bbopt_rounds_app(const int argc, const char* const argv[], FUNC F) {
    constexpr int num_args = 1+4+1;
    assert(argc >= num_args);
    const int newargc = argc - num_args;
    const char* const* const newargv = argv + num_args;
    F.init(newargc, newargv);
    const function_t f = &F.func;

    const Parameters P(argv[1], argv[2], argv[3], argv[4]);
    const auto table = FP::read_table(argv[5]);
    const index_t N = table.size();
    list_intervals_t I; I.reserve(N);
    fpoint_t p; p.x.reserve(N);
    for (const auto& line : table) {
      assert(line.size() >= 5);
      p.x.push_back(line[2]);
      I.emplace_back(line[1],line[3], line[0],line[4]);
    }
    p.y = f(p.x);
    return bbopt_rounds(p, I, f, P);
  }

}

#endif
