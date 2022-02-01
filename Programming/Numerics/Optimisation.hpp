// Oliver Kullmann, 10.12.2021 (Swansea)
/* Copyright 2021, 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Tools for mathematical optimisation

   Basic functions:

    - eval(function_t, vec_t, y_t) -> y_t
    - eval(function_t, fpoint_t) -> y_t

    - min_value_points(list_points_t) -> y_t
    - val_argument_points(list_points_t, y_t) -> point_t
    - min_argument_points(list_points_t) -> point_t


    Algorithm bbopt_rounds_scan (minimising coordinates independently in
    rounds, with shrinking of intervals, and possible scanning of starting
    points):

      bbopt_rounds_scan(
        vector<F80ai>, // vector of float80 or integers
        list_intervals_t I,
        function_t f,
        Parameters P)
      -> fpoint_t

    where

      Parameters:
       - data-members M, R, S, T
       - constructors Parameters(M, [R], [S], [T]) and
         Parameters(string,string,string,string)
       - equality-comparison
       - valid(Parameters).

     bbopt_rounds_scan is packaged into

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

-2. For the round parameter we should also use F80ai:
 - Only integers here.
 - But the modes "+" (boxed) and "-" (non-random) make good sense.
   "Boxed" requires randomisation.
 - In bbopt_index and bbopt_index_parallel the call of
   SP::sampling_points needs to be generalised.
 - Class Parameters should obtain a member of type SP::RSmode, which is
   then passed to bbopt_index(_parallel).
 - For the constructor of Parameters from numbers this makes another
   argument (appended at the end).
 - While for the string-constructor this is read from argument Ms.
   If a non-integer is presented, an exception is thrown.
 - SP::RSmode should then move to OptTypes.hpp.

-1. Create application-tests (BBOpt.cpp).

0. In case of capping, inf should be returned (so that one can see that
   capping took place).

1. The input should be output (in completed form).
    - Also showing the intervals etc.

2. Logging is needed.
 - Showing clearly the treatment of a single index, the whole
   round, and the shrinking.
 - Perhaps not for the scanning mode.

3. The final result should include the final intervals.

4. There should be an option to turn off the capping:
 - In order to see the full behaviour.
 - Also a sensitivity-analysis is useful.

5. Output in advance the number FC of function-calls.
 - Then in non-scanning mode for the running output one can introduce an
   easy progress bar, the two numbers "i/FC", where 1 <= i <= FC is the
   consecutive number of the current instance.

6. Optionally use database of function-graph:
 - The input is a file with sorted pairs (x,y) in f (i.e., f(x) = y);
   sorted lexicographically by x.
 - The case of capped values is reported by (x,>y).
 - The database is read, used to avoid recalculations, and a new database
   is output at the end.
 - Using std::map or std::unordered_map.

7. Once a round runs into unchanged parameter-values, it
   should be abandoned.
 - We have with the scanning-mode for the scanning of the first dimension:
   all scans are the same except of the starting-call.
 - For now just always using "0" for the middle-element of the first dimension
   in scanning.

8. Scanning via Latin hypercube sampling (LHS)
 - See https://en.wikipedia.org/wiki/Latin_hypercube_sampling .
 - Given n dimensions, with intervals [a_i, b_i], and the target is to
   create p >= 1 sample points, for the scanning phase.
 - Each dimension is partitioned into p subintervals:
  - The typical partitioning would be left-closed-right-open.
  - So then the b_i would be left out; so well, seems to be the case.
 - n random permutations P_j, 1 <= j <= n, in S_p are created.
 - The i-th sampling point has for coordinate j a random point of the
   subintervals given by P_j(i).
 - It seems sensible for us to standardise the first permutation S_1 to
   the identity.
 - LHS makes also sense for partial non-random scanning:
  - The random permutations are still needed.
  - But the partition can just be the equidistant sampling.
  - It seems some decision-parameter is needed.
  - But best would be to make the decision per dimension.
  - As we have "asserted integrality" we could have "asserted
    positivity", that is, using "+" for numbers >= 0: then
    an asserted positive integer would mean partiticipation in
    an LHS (as with the "-").
 - We can indicate participation in LHS for a dimension by choosing the
   middle point as -p (a negative integer).
 - Then we can create the product with the all-combination-dimensions.
 - And each different value of p is bundled together to another component
   of the overal product.


OUTLOOK:

After having experience with the simple approach, perhaps we look into the
more advanced approaches:

0. An implementation of
     Mesh Adaptive Direct Search algorithm (MADS)
   is given at https://www.gerad.ca/en/software/nomad .
   See docus/Nomad.

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
#include <stdexcept>
#include <map>

#include <cmath>
#include <cassert>

#include <Transformers/Generators/Random/Numbers.hpp>
#include <Transformers/Generators/Random/FPDistributions.hpp>

#include "NumTypes.hpp"
#include "NumBasicFunctions.hpp"
#include "NumInOut.hpp"

// Guaranteed to be included:
#include "OptTypes.hpp"
#include "Sampling.hpp"

namespace Optimisation {

  namespace SP = Sampling;

  /* Test cases */
  // Beale function https://www.sfu.ca/~ssurjano/beale.html ,
  // global minimum at (3,0.5) -> 0 :
  inline y_t bealef(const vec_t& v) noexcept {
    assert(v.size() == 2);
    using namespace FloatingPoint;
    const float80 x = v[0], y = v[1];
    return sq(1.5L-x+x*y) + sq(2.25L-x+x*sq(y)) + sq(2.625L-x+x*cb(y));
  }
  // Goldstein-Price function https://www.sfu.ca/~ssurjano/goldpr.html ,
  // global minimum at (0,-1) -> 3 :
  inline y_t goldsteinpricef(const vec_t& v) noexcept {
    assert(v.size() == 2);
    using namespace FloatingPoint;
    const float80 x = v[0], y = v[1];
    return
      (1 + sq(x+y+1) * (19-14*x+3*sq(x)-14*y+6*x*y+3*sq(y))) *
      (30 + sq(2*x-3*y) * (18-32*x+12*sq(x)+48*y-36*x*y+27*sq(y)));
  }

  inline function_t expand(const f0_t f) noexcept {
    return [f](const vec_t& x, y_t){return f(x);};
  }
  // Evaluation, taking the known upper bound into account:
  inline y_t eval(const function_t f, const vec_t& x, const y_t b) noexcept {
    return FP::min(f(x,b), b);
  }

  inline y_t eval(const function_t f, const fpoint_t& p) noexcept {
    return eval(f,p.x,p.y);
  }


  inline y_t min_value_points(const list_points_t& v) noexcept {
    assert(not v.empty());
    return std::min_element(v.begin(), v.end(),
      [](const point_t a, const point_t b) noexcept {return a.y < b.y;}) ->y;
  }

  // Finding the midle argument of the points with given value:
  inline point_t val_argument_points(const list_points_t& v, const y_t val) {
    assert(not v.empty());
    std::vector<x_t> valargs;
    for (const auto& p : v) if (p.y == val) valargs.push_back(p.x);
    assert(not valargs.empty());
    return {valargs[(valargs.size()-1)/2], val};
  }
  inline point_t min_argument_points(const list_points_t& v) {
    assert(not v.empty());
    return val_argument_points(v, min_value_points(v));
  }



  /*
     Algorithm bbopt_rounds
  */

  inline constexpr bool valid_partitionsize(const index_t M) noexcept {
    return M < FP::P264m1-1;
  }

  /*
    Optimising one index i, within interval I and with initial full
    point (x, y0) (that is, f(x) = y0).

    More precisely:

    The interval I = [l,r] is equidistantly subdivided into
    a_0, ..., a_M, with a_0 = l and a_M = r.
    Let x_i be x with x[i] replaced by a_i.
    Let opt := min(y0, min_{i=0}^M f(x_i)).
    The "central argument" (left-sided if not unique) xopt of the
    arguments yielding opt (including x, if applicable) is determined,
    and (xopt[i], opt) is returned.
  */
  point_t bbopt_index(vec_t x, const y_t y0, const index_t i, const Interval I, const function_t f, const index_t M, RandGen::RandGen_t* const rg = nullptr) {
    assert(valid(x));
    assert(f(x,FP::pinfinity) == y0);
    assert(i < x.size());
    assert(valid(I));
    assert(element(x[i], I));
    assert(valid_partitionsize(M));

    const x_t x0 = x[i];
    assert(element(x0, I));
    if (I.l == I.r) return {x0,y0};
    bool inserted = false;
    list_points_t results; results.reserve(M+2);
    y_t opt = y0;
    const vec_t samples = SP::sampling_points(I.l, I.r, M, rg);
    assert(samples.size() == M+1);
    for (const x_t x1 : samples) {
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
    if (not inserted) {
      assert(M == 0 or rg);
      assert(samples.back() < x0);
      results.push_back({x0,y0});
      inserted = true;
    }
    assert(results.size()==M+1 or results.size()==M+2);
    const point_t res = val_argument_points(results, opt);
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

  point_t bbopt_index_parallel(vec_t x, const y_t y0, const index_t i,
    const Interval I, const function_t f, const index_t M, const index_t T,
    RandGen::RandGen_t* const rg = nullptr) noexcept {
    assert(valid(x));
    assert(f(x,FP::pinfinity) == y0);
    assert(i < x.size());
    assert(valid(I));
    assert(element(x[i], I));
    assert(valid_partitionsize(M));
    assert(T >= 2);

    const x_t x0 = x[i];
    if (I.l == I.r) return {x0,y0};
    bool inserted = false;
    list_points_t results; results.reserve(M+2);
    std::vector<Computation> computations; computations.reserve(M+1);
    const vec_t samples = SP::sampling_points(I.l, I.r, M, rg);
    assert(samples.size() == M+1);
    for (const x_t x1 : samples) {
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
        results.push_back({x1,FP::NaN});
        const auto last = &results.back();
        computations.emplace_back(x, y0, f, last);
      }
    }
    if (not inserted) {
      assert(M == 0 or rg);
      assert(samples.back() < x0);
      results.push_back({x0,y0});
      inserted = true;
    }
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


  /*
    Shrink the box given by the intervals Iv around the point x by a
    the factor "factor":
     - for each interval I[i] in Iv, let the radius ndelta be the old
       radius (half the length) divided by factor;
     - the new I has radius ndelta around x[i], within the bounds given
       by the hard bounds of I.
  */
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

    constexpr Parameters(const index_t M, const index_t R=1, const index_t S=1, const index_t T=1) noexcept : M(M), R(R), S(S), T(T) {
      assert(valid());
    }

    Parameters(const std::string& Ms, const std::string& Rs, const std::string& Ss, const std::string& Ts) :
      M(FP::toUInt(Ms)), R(FP::toUInt(Rs)), S(FP::toUInt(Ss)),
      T(FP::touint(Ts)) {
      if (not valid_M())
        throw std::out_of_range("Optimisation::Parameters : M=" + Ms);
      if (not valid_S())
        throw std::out_of_range("Optimisation::Parameters : S=" + Ss);
      if (not valid_T())
        throw std::out_of_range("Optimisation::Parameters : T=" + Ts);
    }

    constexpr bool valid_M() const noexcept { return valid_partitionsize(M); }
    constexpr bool valid_S() const noexcept { return S >= 1; }
    constexpr bool valid_T() const noexcept { return T >= 1; }
    constexpr bool valid() const noexcept {
      return valid_M() and valid_S() and valid_T();
    }

    friend constexpr bool operator ==(const Parameters&, const Parameters&) noexcept;
  };
  inline constexpr bool operator ==(const Parameters&, const Parameters&) noexcept = default;
  inline constexpr bool valid(const Parameters& P) noexcept {
    return P.valid();
  }


  fpoint_t bbopt_rounds(fpoint_t p, list_intervals_t I, const function_t f,
    const Parameters& P, RandGen::RandGen_t* const rg = nullptr) noexcept {
    assert(valid(p));
    assert(f(p.x,FP::pinfinity) == p.y);
    assert(valid(I));
    assert(element(p.x, I));
    assert(valid(P));

    const index_t size = p.x.size();
    for (index_t s = 0; s < P.S; ++s) {
      for (index_t r = 0; r < P.R; ++r)
        for (index_t i = 0; i < size; ++i) {
          const point_t opt = P.T == 1 ?
            bbopt_index         (p.x, p.y, i, I[i], f, P.M, rg) :
            bbopt_index_parallel(p.x, p.y, i, I[i], f, P.M, P.T, rg);
          p.x[i] = opt.x; p.y = opt.y;
        }
      shrink_intervals(p.x, I);
    }
    return p;
  }


  struct Computation2 {
    const vec_t x;
    const function_t f;
    const list_intervals_t* const I;
    const Parameters P;

    fpoint_t* const target;
    RandGen::RandGen_t* rg;
    const Computation2* next;

    Computation2(const vec_t x, const function_t f,
                 const list_intervals_t* const I, const Parameters P,
                 fpoint_t* const t)
      noexcept :
    x(x), f(f), I(I), P(P), target(t), rg(nullptr), next(nullptr) {}
    Computation2(const Computation2&) = default;
    Computation2(Computation2&&) = delete;

    void operator()() const noexcept {
      const y_t y = f(x,FP::pinfinity);
      *target = bbopt_rounds({x,y}, *I, f, P, rg);
      if (next) next->operator()();
    }
  };


  fpoint_t bbopt_rounds_scan(
      const evec_t& x, const list_intervals_t& I, const function_t f, const Parameters& P,
      RandGen::vec_eseed_t seeds, const bool randomised) {
    const auto N = x.size();
    assert(I.size() == N);
    assert(valid(I));
    assert(valid(P));
    assert(randomised or seeds.empty());

    const bool has_ai = std::any_of(x.begin(), x.end(),
                                    [](const FP::F80ai x){return x.isint;});
    if (not has_ai) { // non-scanning
      fpoint_t p; p.x.reserve(N);
      for (const FP::F80ai xi : x) p.x.push_back(xi.x);
      p.y = f(p.x, FP::pinfinity);
      if (randomised) {
        RandGen::RandGen_t rg(seeds);
        return bbopt_rounds(p, I, f, P, &rg);
      }
      else
        return bbopt_rounds(p, I, f, P, nullptr);
    }
    else { // scanning
      const bool has_e0 = std::any_of(x.begin(), x.end(),
        [](const FP::F80ai x){return x.isint and x.hase0;});
      assert(not has_e0 or randomised);
      const bool has_plus = std::any_of(x.begin(), x.end(),
        [](const FP::F80ai x){return x.isint and x.hasplus;});
      assert(not has_plus or randomised);
      const std::vector<vec_t> init_poss = randomised ?
        [&x,&I, &seeds]{RandGen::RandGen_t g(seeds);
                        return SP::fill_possibilities(x, I, &g);}()
        : SP::fill_possibilities(x, I, nullptr);
      assert(init_poss.size() == N);

      auto ipc = init_poss;
       typedef SP::Lockstep<vec_t, FP::float80> LS_t;
       typedef LS_t::vcon_t vcon_t;
       typedef LS_t::vpelem_t vpelem_t;
       std::vector<LS_t> equiv_classes; equiv_classes.reserve(N);
       vec_t currv(N);
       for (index_t i = 0; i < N; ++i)
         if (not x[i].isint or not x[i].hase0)
           equiv_classes.emplace_back(
             vcon_t{std::move(ipc[i])}, vpelem_t{&currv[i]});
       assert(equiv_classes.size() <= N);
       assert(has_e0 or equiv_classes.size() == N);
       if (has_e0) {
         std::map<FP::UInt_t, std::vector<index_t>> groups;
         for (index_t i = 0; i < N; ++i) {
           if (not x[i].isint or not x[i].hase0) continue;
           assert(FP::isUInt(FP::abs(x[i].x)));
           const FP::UInt_t m = FP::abs(x[i].x);
           groups[m].push_back(i);
         }
         for (const auto& pair : groups) {
           const std::vector<index_t>& group = pair.second;
           vcon_t C; C.reserve(group.size());
           vpelem_t D; D.reserve(group.size());
           for (const index_t j : group) {
             C.push_back(std::move(ipc[j]));
             D.push_back(&currv[j]);
           }
           equiv_classes.emplace_back(std::move(C), std::move(D));
         }
       }
       assert(N == [&equiv_classes]{
                index_t s = 0;
                for (const auto& ec : equiv_classes) s += ec.content.size();
                return s;}());
       std::vector<LS_t::It> equicl_it;
       equicl_it.reserve(equiv_classes.size());
       for (const auto& ec : equiv_classes)
         equicl_it.push_back(ec.begin());
       const auto b = equicl_it;
       const std::vector<LS_t::It> e = [&equiv_classes]{
         std::vector<LS_t::It> res;
         for (const auto& ec : equiv_classes)
           res.push_back(ec.end());
         return res;}();
       /*
       auto copy = equicl_it;
       do {
         for (index_t i = 0; i < equiv_classes.size(); ++i)
           equiv_classes[i].update(copy[i]);
         for (const auto x : currv) std::cerr << x << " ";
         std::cerr << "\n";
       } while (SP::next_combination(copy, b, e));
       */

      fpoint_t optimum; optimum.y = FP::pinfinity;
      if (P.T == 1) { // sequential
        if (randomised) {
          seeds.push_back(0);
          RandGen::RandGen_t g(seeds);
          do {
            fpoint_t init; init.x.reserve(N);
            for (index_t i = 0; i < equiv_classes.size(); ++i)
              equiv_classes[i].update(equicl_it[i]);
            init.x = currv;
            init.y = f(init.x, FP::pinfinity);
            const fpoint_t res = bbopt_rounds(init, I, f, P, &g);
            if (res.y < optimum.y) optimum = res;
          } while (SP::next_combination(equicl_it, b, e));
          return optimum;
        }
        else {
          do {
            fpoint_t init; init.x.reserve(N);
            for (index_t i = 0; i < equiv_classes.size(); ++i)
              equiv_classes[i].update(equicl_it[i]);
            init.x = currv;
            init.y = f(init.x, FP::pinfinity);
            const fpoint_t res = bbopt_rounds(init, I, f, P, nullptr);
            if (res.y < optimum.y) optimum = res;
          } while (SP::next_combination(equicl_it, b, e));
          return optimum;
        }
      }
      else { // using P.T >= 2 threads
        const index_t size = [&equiv_classes]{
          index_t prod = 1;
          for (const auto& e : equiv_classes) prod *= e.content.front().size();
          return prod;
        }();
std::cerr << "size=" << size << "\n";
        std::vector<fpoint_t> results(size);
        std::vector<Computation2> computations; computations.reserve(size);
        {index_t i = 0;
         const Parameters Pnew(P.M, P.R, P.S, 1);
         do {
           for (index_t j = 0; j < equiv_classes.size(); ++j)
             equiv_classes[j].update(equicl_it[j]);
           computations.emplace_back(currv, f, &I, Pnew, &results[i++]);
         } while (SP::next_combination(equicl_it, b, e));
         assert(i == size);
        }
        assert(computations.size() == size);
        for (index_t i = 0; i+P.T < size; ++i)
          computations[i].next = &computations[i+P.T];
        const index_t num_threads = std::min(P.T,size);
        std::vector<RandGen::RandGen_t> generators;
        if (randomised) {
          generators.reserve(num_threads);
          seeds.push_back(0);
          for (index_t i = 0; i < num_threads; ++i) {
            seeds.back() = i+1;
            generators.emplace_back(seeds);
            computations[i].rg = &generators.back();
          }
          for (index_t i = num_threads; i < size; ++i)
            computations[i].rg = computations[i-num_threads].rg;
        }

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


  /*
    Helper function to call bbopt_rounds_scan:

     - reading the command-line arguments: consuming the first 6 arguments
       for the four parameters and the filename of the interval-data
     - using class FUNC, which shall be copyable, and provide member-functions
       F::init(argc,argv) and F.func(x).

    See BBOpt.cpp for an example-application.
  */
  template <class FUNC>
  fpoint_t bbopt_rounds_app(const int argc, const char* const argv[], FUNC F) {
    constexpr int num_args = 1+4+1+1;
    assert(argc >= num_args);
    const int newargc = argc - num_args;
    const char* const* const newargv = argv + num_args;

    F.init(newargc, newargv);
    const function_t f = [&F](const vec_t& x, const y_t y)
      {return F.func(x,y);};

    const Parameters P(argv[1], argv[2], argv[3], argv[4]);

    const std::string filename(argv[5]);
    const auto table = FP::read_table_ai(filename, 2);
    const index_t N = table.size();
    list_intervals_t I; I.reserve(N);
    evec_t x; x.reserve(N);
    for (const auto& line : table) {
      x.push_back(line.second);
      const vec_t ivs = line.first;
      assert(ivs.size() >= 4);
      I.emplace_back(ivs[1],ivs[2], ivs[0],ivs[3]);
    }
    if (not valid(I))
      throw std::domain_error("Optimisation::bbopt_rounds_app : "
        "Invalid intervals in file \"" + filename + "\"");
    if (not element(x, I))
      throw std::domain_error("Optimisation::bbopt_rounds_app : "
        "Point not included in intervals of file \"" + filename + "\"");

    const std::string seeds_string(argv[6]);
    const bool randomised = not seeds_string.empty();
    RandGen::vec_eseed_t seeds;
    [[maybe_unused]] const auto num_seeds =
      randomised ? RandGen::add_seeds(seeds_string, seeds) : 0;
    assert(num_seeds == seeds.size());
    assert((randomised and num_seeds >= 1)
           or (not randomised and num_seeds == 0));

    return bbopt_rounds_scan(x, I, f, P, seeds, randomised);
  }

}

#endif
