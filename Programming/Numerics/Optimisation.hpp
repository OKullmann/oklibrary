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
   - function_t ((vec_t, y_t) -> y_t)

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

    - eval(function_t, vec_t, y_t) -> y_t
    - eval(function_t, fpoint_t) -> y_t

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
       - constructors Parameters(M, [R], [S], [T]) and
         Parameters(string,string,string,string)
       - equality-comparison
       - valid(Parameters).

     bbopt_rounds is packaged into

       bbopt_rounds_app(const int argc, const char* const argv[], FUNC F)
       -> fpoint_t

     which constructs the arguments for bbopt_rounds from the command-line
     arguments.
     If one wants to improve given values, then the x-values shall be given
     as the point inside the intervals (while the y-value will be computed).

     The optimisation for one index-value happens in
       bbopt_index
     resp. the parallel version bb_opt_index_parallel (which uses the class
     Computation to encapsulate one computation-unit).

     Shrinking the intervals (parameter S) is done via function
       shrink_intervals.


TODOS:

-1. The input-file needs to be checked.

0. The input should be output (in completed form).
    - Perhaps also outputting the intervals etc.

1. Should valid(fpoint_t) require y >= 0 ?
    - Or can we supply a more general test?

2. Every function should have a unit-test.

3. Another meta-level is needed, which doesn't provide a starting-point x,
   but just another natural number m, which yields m+1 equidistant points
   (from left to right) for each subinterval, and which runs the optimisation
   for each of these starting-points.
    - The data-file then only contains 4 numbers per line.
    - Perhaps we allow every line to either have 4 or 5 numbers: in the latter
      case this is the unique starting point, in the former case m is used.
    - Perhaps m=0 is the case with a unique starting point (it is an error
      then if a line only contains 4 numbers).
    - Perhaps m is just always provided, and integrated into Parameters.
    - For dimension d of the problem this yields (m+1)^d runs; would be
      good if they could be parallelised.

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
                         [](const x_t x){return not FP::isnan(x);});
  }


  // f(v, opt) : if the evaluation of v yields a value > opt, then
  // any value > opt can be returned:
  typedef std::function<y_t(const vec_t&, x_t)> function_t;
  inline y_t eval(const function_t f, const vec_t& x, const y_t b) noexcept {
    return FP::min(f(x,b), b);
  }


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

  inline y_t eval(const function_t f, const fpoint_t& p) noexcept {
    return eval(f,p.x,p.y);
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
      [](const point_t a, const point_t b) noexcept {return a.y < b.y;}) ->y;
  }

  inline point_t min_argument_points(const list_points_t& v, const y_t min) {
    assert(not v.empty());
    std::vector<x_t> minargs;
    for (const auto& p : v)
      if (p.y == min) minargs.push_back(p.x);
    assert(not minargs.empty());
    return {minargs[(minargs.size()-1)/2], min};
  }
  inline point_t min_argument_points(const list_points_t& v) {
    assert(not v.empty());
    return min_argument_points(v, min_value_points(v));
  }


  /*
     Algorithm bbopt_rounds
  */

  inline constexpr bool valid_partitionsize(const index_t M) noexcept {
    return M >= 1 and M < FP::P264m1-1;
  }

  point_t bbopt_index(vec_t x, const y_t y0, const index_t i, const Interval I, const function_t f, const index_t M) {
    assert(valid(x));
    assert(eval(f,x,y0) == y0);
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
    y_t opt = y0;
    for (index_t j = 0; j <= M; ++j) {
      const x_t x1 = FP::fma(j, delta, I.l);
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
        const y_t y1 = f(x,opt);
        opt = FP::min(opt, y1);
        results.push_back({x1,y1});
      }
    }
    assert(inserted);
    assert(results.size()==M+1 or results.size()==M+2);
    const point_t res = min_argument_points(results);
    assert(res.y == opt);
    return res;
  }

  // Node for computing f(x) and storing i at target->y :
  struct Computation {
    const vec_t x;
    y_t opt;
    const function_t f;
    point_t* const target;
    Computation* next;

    Computation(const vec_t x, const y_t opt, const function_t f, point_t* const t) noexcept :
      x(x), opt(opt), f(f), target(t), next(nullptr) {}
    Computation(const Computation&) = default;
    Computation(Computation&&) = delete;

    void operator()() const noexcept {
      const y_t y = f(x,opt);
      assert(target);
      target->y = y;
      if (next) {
        next->opt = FP::min(opt, y);
        next->operator()();
      }
    }
  };

  point_t bbopt_index_parallel(vec_t x, const y_t y0, const index_t i, const Interval I, const function_t f, const index_t M, const index_t T) noexcept {
    assert(valid(x));
    assert(eval(f,x,y0) == y0);
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
        const auto last = &results.back();
        computations.emplace_back(x, y0, f, last);
      }
    }
    assert(inserted);
    assert(results.size()==M+1 or results.size()==M+2);
    const auto csize = computations.size();
    assert(csize==M or csize==M+1);

    for (index_t i = 0; i+T < csize; ++i)
      computations[i].next = &computations[i+T];
    const index_t num_threads = std::min(T,csize);
    std::vector<std::thread> threads; threads.reserve(num_threads);
    for (index_t i = 0; i < num_threads; ++i)
      threads.emplace_back(computations[i]);
    assert(threads.size() == num_threads);
    for (std::thread& t : threads) {
      assert(t.joinable());
      t.join();
    }
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
      I.l = FP::max(I.hl, x0 - ndelta);
      I.r = FP::min(I.hr, x0 + ndelta);
    }
    assert(valid(Iv));
    assert(element(x,Iv));
  }

  struct Parameters {
    index_t
      M, // number of subintervals
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

  fpoint_t bbopt_rounds(fpoint_t p, list_intervals_t I, const function_t f, const Parameters& P) noexcept {
    assert(valid(p));
    assert(eval(f,p) == p.y);
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


  std::vector<vec_t> fill_possibilities(const std::vector<FP::F80ai>& x, const list_intervals_t& I) {
    const auto N = x.size();
    assert(I.size() == N);
    std::vector<vec_t> res(N);
    for (index_t i = 0; i < N; ++i) {
      const x_t xi = x[i].x, li = I[i].l, ri = I[i].r;
      auto& poss = res[i];
      if (not x[i].isint)
        poss.push_back({xi});
      else if (li == ri)
        poss.push_back({li});
      else {
        assert(FP::isUInt(xi));
        const FP::UInt_t M = xi;
        if (M == 0)
          poss.push_back(FP::midpoint(li, ri));
        else {
          poss.push_back(li);
          const x_t delta = (ri - li) / M;
          assert(delta > 0);
          for (index_t j = 1; j < M; ++j)
            poss.push_back(FP::fma(j, delta, li));
          poss.push_back(ri);
        }
        assert(res[i].size() == M + 1);
      }
    }
    assert(res.size() == N);
    return res;
  }

  template <class ITER>
  bool next_combination(
    std::vector<ITER>& current,
    const std::vector<ITER>& begin, const std::vector<ITER>& end) noexcept {
    const auto N = current.size();
    assert(begin.size() == N);
    assert(end.size() == N);
    for (index_t i = 0; i < N; ++i) {
      assert(current[i] != end[i]);
      ++current[i];
      if (current[i] != end[i]) {
        for (index_t j = 0; j < i; ++j) current[j] = begin[j];
        return true;
      }
    }
    return false;
  }

  struct Computation2 {
    const vec_t x;
    const function_t f;
    const list_intervals_t* const I;
    const Parameters* const P;
    fpoint_t* const target;
    const Computation2* next;

    Computation2(const vec_t x, const function_t f,
                 const list_intervals_t* const I, const Parameters* const P,
                 fpoint_t* const t) noexcept :
    x(x), f(f), I(I), P(P), target(t), next(nullptr) {}
    Computation2(const Computation2&) = default;
    Computation2(Computation2&&) = delete;

    void operator()() const noexcept {
      const y_t y = f(x,FP::pinfinity);
      *target = bbopt_rounds({x,y}, *I, f, *P);
      if (next) next->operator()();
    }
  };


  fpoint_t bbopt_rounds_scan(const std::vector<FP::F80ai>& x, const list_intervals_t& I, const function_t f, const Parameters& P) {
    const auto N = x.size();
    assert(I.size() == N);
    assert(valid(I));
    assert(valid(P));

    const bool has_ai = std::any_of(x.begin(), x.end(),
                                    [](const FP::F80ai x){return x.isint;});
    if (not has_ai) {
      fpoint_t p; p.x.reserve(N);
      for (const FP::F80ai xi : x) p.x.push_back(xi.x);
      p.y = f(p.x, FP::pinfinity);
      return bbopt_rounds(p, I, f, P);
    }
    else {
      const std::vector<vec_t> init_poss = fill_possibilities(x, I);
      typedef vec_t::const_iterator iterator_t;
      std::vector<iterator_t> curr_init; curr_init.reserve(N);
      for (const vec_t& v : init_poss) curr_init.push_back(v.begin());
      typedef std::vector<iterator_t> itv_t;
      const itv_t begin = [&init_poss](){
        itv_t v; v.reserve(init_poss.size());
        for (const auto& e : init_poss) v.push_back(e.begin());
        return v;}();
      const itv_t end = [&init_poss](){
        itv_t v; v.reserve(init_poss.size());
        for (const auto& e : init_poss) v.push_back(e.end());
        return v;}();
      fpoint_t optimum; optimum.y = FP::pinfinity;
      if (P.T == 1) {
        do {
          fpoint_t init; init.x.reserve(N);
          for (const iterator_t it : curr_init) init.x.push_back(*it);
          assert(init.x.size() == N);
          init.y = f(init.x, FP::pinfinity);
          const fpoint_t res = bbopt_rounds(init, I, f, P);
          if (res.y < optimum.y) optimum = res;
        } while (next_combination(curr_init, begin, end));
        return optimum;
      }
      else {
        const index_t size = [&init_poss]{
          index_t prod = 1;
          for (const auto& e : init_poss) prod *= e.size();
          return prod;
        }();
        std::vector<fpoint_t> results(size);
        std::vector<Computation2> computations; computations.reserve(size);
        {index_t i = 0;
         do {
           vec_t x; x.reserve(N);
           for (const iterator_t it : curr_init) x.push_back(*it);
           assert(x.size() == N);
           computations.emplace_back(x, f, &I, &P, &results[i++]);
         } while (next_combination(curr_init, begin, end));
         assert(i == size);
        }
        assert(computations.size() == size);
        for (index_t i = 0; i+P.T < size; ++i)
          computations[i].next = &computations[i+P.T];
        const index_t num_threads = std::min(P.T,size);
        std::vector<std::thread> threads; threads.reserve(num_threads);
        for (index_t i = 0; i < num_threads; ++i)
          threads.emplace_back(computations[i]);
        assert(threads.size() == num_threads);
        for (std::thread& t : threads) {
          assert(t.joinable());
          t.join();
        }
        fpoint_t opt; opt.y = FP::pinfinity;
        for (const fpoint_t& p : results)
          if (p.y < opt.y) opt = p;
        return opt;
      }
    }
  }

  template <class FUNC>
  fpoint_t bbopt_rounds_app(const int argc, const char* const argv[], FUNC F) {
    constexpr int num_args = 1+4+1;
    assert(argc >= num_args);
    const int newargc = argc - num_args;
    const char* const* const newargv = argv + num_args;
    F.init(newargc, newargv);
    const function_t f = [&F](const vec_t& x, const y_t y)
      {return F.func(x,y);};

    const Parameters P(argv[1], argv[2], argv[3], argv[4]);
    const auto table = FP::read_table_ai(argv[5], 2);
    const index_t N = table.size();
    list_intervals_t I; I.reserve(N);
    std::vector<FP::F80ai> x; x.reserve(N);
    for (const auto& line : table) {
      x.push_back(line.second);
      const vec_t ivs = line.first;
      assert(ivs.size() >= 4);
      I.emplace_back(ivs[1],ivs[2], ivs[0],ivs[3]);
    }
    return bbopt_rounds_scan(x, I, f, P);
  }

}

#endif
