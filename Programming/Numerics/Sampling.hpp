// Oliver Kullmann, 28.1.2022 (Swansea)
/* Copyright 2022, 2023 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Tools for creating sampling sequences

    - scoped enum Smode (eq_un, boxed, eq:
       - "equi-distant or uniform"
       - for uniformly random in "boxes"
       - for equi-distant only ("forced", whether randomised or not)

    - sampling_points(x_t l, x_t r, index_t M, RandGen_t* rg, Smode rsm)
        -> vec_t
      creates M+1 sorted values in the interval [l, r]:
       - in principle there are 3 * 2 modes: 3 values of Smode, and
         rg can be nullptr or not (in which case rg is active);
       - however if rsm==boxed, then rg must be active
       - so if rg is inactive, then we either have eq_un or eq, which here
         become the same (that is, here eq_un -> eq); this is also the
         default, when not giving rg and rsm;
         "equi-distant" means inclusion of l and r except for M=0, where
         the mid-point of [l,r] is chosen
       - while if rg is active :
        - rsm=eq forces equi-distant values (without randomisation)
        - rsm=eq_un means uniform randomness (so here eq_un -> un)
        - rsm=boxed means first M+2 equidistant sampling points are
          created, which yield M+1 right-open intervals, from which
          the M+1 points are chosen
        - M=0 means:
         - uniform-random in [l,r] for eq_un
         - uniform-random in [l,r) for boxed (right-open).
      So altogether we have actually exactly three modes (covered by 5 modes):
        - equi-distant (for M=0 this means midpoint), created by
         - eq and rg arbitrary
         - eq_un and rg inactive
        - uniform-random, created by eq_un and rg active
        - boxed-random (for M=0 this means right-open), created by
          boxed and rg active.

    - fill_possibilities(evec_t x, list_intervals_t I, RandGen_t* rg) ->
        vector<vec_t>
      creates the mesh (grid) for scanning:
       - handles each coordinate 0 <= i < x.size() individually, within the
         given soft interval-bounds l_i, r_i
       - the value x[i] determines the 4 cases (single value, or 3 cases
         of sampling points) as follows;
       - single values: either fixed given value (due to decimal point), or
          - a random element of [l,r] resp. [l,r)
          - or the middle point
         for x[i]=0 resp. +0 resp. -0 ("asserted integral", i.e., no decimal
         point)
       - we get single-values also if l=r
       - sequences of length L+1: either equidistant, or uniform random or
         random from the half-open subintervals; here x[i]=L is
         "asserted integral", (for strings: no decimal point), and we get the
         three cases from -L resp. L resp. +L
       - this generalises the single-value-case:
        - 0 resp. L for random value(s) from closed interval
        - -0 resp. -L for middle-point resp. equidistant values
        - +0 resp. +L for random-value from right-open interval resp.
          half-open subintervals
       - here "+" requires rg (!= nullptr)
       - while without "+" and with rg == nullptr we always get equi-distant
       - furthermore asserted int can have e0 or not: if yes, then rg is
         required, and groups are created according to having the same number
         of sampling-points >= 2 (i.e., L >= 1 above), where then all
         sequences in a group except of the first are randomly permuted
         (the first's thus stay in sorted order).

    - next_combination allows to run through all combinations, via considering
      a range given by a vector of iterators, which are iterated through
      anti-lexicographically
    - get_vit_range(std::vector<ITER> begin, const vector<ITER> end) ->
        std::vector<std::vector<ITER>>
      shows how to use next_combination (by creating explicitly the
      range of iterator-vectors)
    here "iterator" just means == and ++ (so for example numbers can be
    ITER).

    - struct Lockstep<CON, ELEM> provides a wrapper Lockstep::It, which
      creates an iterator, which increments a vector of iterators (e.g., those
      combined in a latin-hypercube-sampling); at construction a "delivery"-
      vector of pointers is given, where via the update-function the
      vector of iterators is dereferenced (all elements);
      the iterators are constructed from a vector<CON>, where each "container"
      contains the sampling points of the respective dimension;
      thus we have a "zip"-operation
    - get_vcon_range(std::vector<CON> content) ->
        std::vector<std::vector<CON::value_type>>
      shows how to use Lockstep (by creating explicitly the range of the
      zip-results).

    - typedefs StdLockstep, vit_t = vector<StdLockstep::It>
    - typedef scanning_t = tuple<StdLockstep, vit_t, vit_t>
    - prepare_scanning(evec_t x, list_intervals_t I, vec_eseed_t seeds,
                       bool randomised, vec_t& currv) -> scanning_t
      creates the scanning-range (as the underlying Lockstep-vector, plus
      begin and end):
       - the initial mesh is created by fill_possibilities (using the above
         possibilities for F80ai, the elements of x)
       - all coordinates i with asserted int and without e0 are combined
         combinatorially and independently
       - while those with e0 are combined into groups for latin-hypercube
         sampling, grouped together for equal number of points (different
         groups are combined combinatorially and independently);
         echo coordinate may be equidistant or uniform random or boxed random
    - randomised (affects only coordinates with asserted int):
       - false : there must be no + or e0, and then all coordinates are
         equidistant;
       - true  : the coordinates with - are still equidistant, while the
         coordinates without + or - are now uniform random
    - size_scanning(vector<StdLockstep>) -> index_t
      computes the size of the range (as the product of the sizes of the
      Lockstep-vectors)
    - get_scanning_points(evec_t x, list_intervals_t I, vec_eseed_t seeds,
        bool randomised) -> vector<vec_t>
      shows how to get the list of points, and sorts them.

    - perform_scanning<VAL>(evec_t x, list_intervals_t I, vec_eseed_t seeds,
        bool randomised, FUN f, index_t threads)
          -> pair<vector<vec_t>, vector<VAL>>
      applies f to all scanning-points x : vec_t (in sorted order),
      and stores the pairs x, f(x) : VAL in the two (separate) output-vectors.

    - perform_scanning_script(istream& in, vec_eseed_t& seeds,
        bool randomised, string script, TRANS T, index_t threads)
      applies perform_scanning with x,I obtained from in, and f obtained
      by applying T to the result of the script (fed with the generated
      vectors);
       - seeds is extended with three 64-bit keys:
        - first with the hash for the sequence of intervals;
        - then the hash for the sequence of "midpoints";
        - finally the hash for the "script"-string
    - perform_scanning_script(istream& in, vec_eseed_t& seeds,
        bool randomised, string script, index_t threads)
      is the special case, where the results of script are translated
      into vectors of float80
    - calling a script and applying a transformation is wrapped into
      the struct call_script<TRANS>
    - the translation of strings into vectors of float80 is wrapped into
      the struct string2vec_t

    - scanning_output(string grid, string script) -> string
      computes the standard output-filename for the scanning-results
      (applying script to grid).


TODOS:

0. Proper ranges for lexicographical order:
    - next_combination likely should evolve into a proper
      range-generator.
    - So that the users (get_vit_range for a start) just employ the usual
      range-operations.
    - Same for Lockstep (this performs bundling of iterators).
    - So that the users (get_vcon_range for a start) again just employ the
      usual range-operations.

1. Accuracy of sampling_points(l,r,M)

   For example

const auto res = sampling_points(0,10,9);
for (const auto x : res) std::cerr << " " << Wrap(x);
std::cerr << "\n";

   yields

 0 1.1111111111111111111 2.2222222222222222222 3.3333333333333333333 4.4444444444444444445 5.5555555555555555555 6.6666666666666666665 7.777777777777777778 8.888888888888888889 10

which actually seems correct:

assert(res[3] == *--F80it(3.3333333333333333335L));
assert(res[4] == 4.444444444444444444444444444444444L);
assert(res[5] == 5.555555555555555555555555555555555L);
assert(res[6] == 6.666666666666666666666666666666666L);

works out (where the above value "...5" for res[3] is obtained without using
the gcd, and thus indeed we need the gcd).

Is the current computation the best we can do?

*/

#ifndef SAMPLING_Msl8gWybWD
#define SAMPLING_Msl8gWybWD

#include <algorithm>
#include <vector>
#include <set>
#include <numeric>
#include <map>
#include <tuple>
#include <utility>
#include <thread>
#include <string>
#include <istream>
#include <type_traits>
#include <filesystem>

#include <cassert>

#include <Transformers/Generators/Random/Numbers.hpp>
#include <Transformers/Generators/Random/FPDistributions.hpp>
#include <Transformers/Generators/Random/Algorithms.hpp>
#include <SystemSpecifics/SystemCalls.hpp>
#include <Transformers/Generators/Bicliques/Algorithms.hpp>
#include <ProgramOptions/Strings.hpp>

#include "NumTypes.hpp"
#include "NumBasicFunctions.hpp"
#include "NumPrecise.hpp"

#include "OptTypes.hpp"

namespace Sampling {

  namespace FP = FloatingPoint;
  namespace OS = Optimisation;

  // SamplingMode
  enum class Smode {
    // equi-distant or uniform random (depending on whether rg is given):
    eq_un = 0,
    // kind of equi-distant uniform random (requires rg):
    boxed = 1,
    // equi-distant (independent of rg):
    eq = 2
  };

  /* M+1 Sampling points in the interval [l,r] uniformly:
      - if rg is the null-pointer, then using equi-distant sampling
      - otherwise use random uniform sampling or random boxed sampling,
        (or again equi-distant sampling, when forced).
     The resulting vector is sorted.
  */
  inline OS::vec_t sampling_points(const OS::x_t l, const OS::x_t r,
                                   const OS::index_t M,
                                   RandGen::RandGen_t* const rg = nullptr,
                                   Smode rsm = Smode::eq_un) {
    assert(l < r);
    assert(M < FP::P264m1);
    assert(rsm != Smode::boxed or rg);
    OS::vec_t res; res.reserve(M+1);
    if (not rg or rsm == Smode::eq) {
      if (M == 0) res.push_back(FP::midpoint(l,r));
      else {
        res.push_back(l);
        if (M >= 2) {
          for (OS::index_t i = 1; i < M; ++i) {
            const OS::index_t g = std::gcd(i,M);
            const OS::index_t ip = i/g, Mp = M/g;
            const OS::x_t delta = (r - l) / Mp;
            const OS::x_t x = FP::fma(ip, delta, l);
            /* Remark: this computation of x seems more accurate than e.g.
                 const OS::x_t x = FP::fma(ip * (r - l), 1.0L / Mp, l);
                 const OS::x_t x = FP::lerp(l, r, OS::x_t(ip) / Mp);
               which seem to yield worse results.
            */
            assert(x <= r);
            res.push_back(x);
          }
        }
        res.push_back(r);
      }
    }
    else {
      if (rsm == Smode::eq_un) {
        const RandGen::Uniform80RangeI U(*rg, l, r);
        for (OS::index_t i = 0; i <= M; ++i) res.push_back(U());
        std::sort(res.begin(), res.end());
      }
      else {
        assert(rsm == Smode::boxed);
        assert(M < FP::P264m1-1);
        const auto boxes = sampling_points(l,r,M+1);
        assert(boxes.size() == M+2);
        for (OS::index_t i = 0; i <= M; ++i)
          res.push_back(RandGen::Uniform80Range(*rg, boxes[i], boxes[i+1])());
      }
    }
    assert(res.size() == M+1);
    assert(std::is_sorted(res.begin(), res.end()));
    return res;
  }



  /*
    For a vector x of float80 with assertions, of size N,
    and a vector I of intervals (of size N),
    create a vector res of size N of vec_t:

     - if x[i] is not asserted int, then res[i] = {x[i]};
       in the sequel x[i] is asserted int (correctly);
     - if x[i] is negative, then (-x[i]+1) equidistant points
       with first element I[i].l and last element I[i].r are
       put into x[i];
       in the sequel x[i] >= 0;
     - if x[i] has not "+", then in case rg = 0 we also create
       x[i]+1 equidistant points (as above), while otherwise
       x[i]+1 many random points in the closed interval given by
       I[i] are created (uniform distribution);
     - if x[i] has "+", then rg must not be 0, and x[i]+1 "boxed"
       random points are created (in the half-open subintervals).

    The special case x[i]=0 also has the same cases as above (in
    all cases res[i] consists of a single point {p}):
     - If isint=false, then p = 0;
       in the sequel isint=true.
     - If "x[i]==+0", that is, hasplus=true, then p is a random element
       of the half-open interval [l,r).
     - If "x[i]==-0", that is, x[i] has signbit, then p = (l+r)/2.
     - If "x[i]==0", that is, hasplus is false and signbit is false,
       then if rg==0, then p = (l+r)/2, otherwise p is a random element
       of the closed interval [l,r].
  */
  std::vector<OS::vec_t> fill_possibilities(const OS::evec_t& x,
      const OS::list_intervals_t& I, RandGen::RandGen_t* const rg = nullptr) {
    assert(OS::element(x, I));
    const auto N = x.size();
    assert(I.size() == N);
    std::vector<OS::vec_t> res; res.reserve(N);
    std::set<OS::index_t> groups;
    for (OS::index_t i = 0; i < N; ++i) {
      const OS::x_t xi = x[i].x, li = I[i].l, ri = I[i].r;
      if (not x[i].isint) res.push_back({xi});
      else if (li == ri) res.push_back({li});
      else {
        if (xi >= 0) {
          assert(FP::isUInt(xi));
          const FP::UInt_t M = xi;
          if (x[i].hasplus)
            res.push_back(sampling_points(li, ri, M, rg, Smode::boxed));
          else if (FP::signbit(xi))
            res.push_back(sampling_points(li, ri, M, nullptr));
          else
            res.push_back(sampling_points(li, ri, M, rg));
          if (x[i].hase0) {
            if (groups.contains(M)) {
              assert(rg);
              RandGen::shuffle(res.back().begin(), res.back().end(), *rg);
            }
            else
              groups.insert(M);
          }
        }
        else {
          const OS::x_t nxi = -xi;
          assert(FP::isUInt(nxi));
          const FP::UInt_t M = nxi;
          res.push_back(sampling_points(li, ri, M));
          if (x[i].hase0) {
            if (groups.contains(M)) {
              assert(rg);
              RandGen::shuffle(res.back().begin(), res.back().end(), *rg);
            }
            else
              groups.insert(M);
          }
        }
      }
    }
    assert(res.size() == N);
    return res;
  }


  // The range is given by begin, end (assumed to be fixed), as a vector
  // of iterators (just need ++ and end()), and this range is iterated through
  // in antilexicographical order, with current within this range; returns
  // false iff at the end, and always advances current:
  template <class ITER>
  bool next_combination(
    std::vector<ITER>& current,
    const std::vector<ITER>& begin, const std::vector<ITER>& end) noexcept {
    const auto N = current.size();
    assert(begin.size() == N);
    assert(end.size() == N);
    for (OS::index_t i = 0; i < N; ++i) {
      assert(current[i] != end[i]);
      ++current[i];
      if (current[i] != end[i]) {
        for (OS::index_t j = 0; j < i; ++j) current[j] = begin[j];
        return true;
      }
    }
    return false;
  }
  // Demonstration of how to run through such a "range":
  template <class ITER>
  std::vector<std::vector<ITER>>
  get_vit_range(const std::vector<ITER>& begin,
               const std::vector<ITER>& end) {
    const auto size = begin.size();
    assert(size == end.size());
    std::vector<std::vector<ITER>> res;
    if (begin == end) return res;
    assert(Algorithms::allnotequal(begin, end));
    std::vector<ITER> current = begin;
    do res.push_back(current);
    while (next_combination(current, begin, end));
    assert(current == end);
    return res;
  }


  template <class CON, typename ELEM>
  struct Lockstep {
    typedef CON container;
    typedef ELEM element;

    typedef std::vector<container> vcon_t;
    typedef std::vector<element*> vpelem_t;
    vcon_t content;
    vpelem_t delivery;

    Lockstep(const vcon_t V, const vpelem_t D) noexcept :
        content(V), delivery(D) {
      assert(not content.empty());
      assert(content.size() == delivery.size());
      assert(Algorithms::allsamesize(content));
    }

    typedef typename container::const_iterator iterator;
    typedef std::vector<iterator> vit_t;

    struct It {
      vit_t vi;
      void operator ++() noexcept { for (auto& i : vi) ++i; }
      bool operator ==(const It& rhs) const noexcept {
        assert(not vi.empty() and not rhs.vi.empty());
        return vi.front() == rhs.vi.front();
      }
    };

    It begin() const {
      It vi; vi.vi.reserve(content.size());
      for (const container& c : content) vi.vi.push_back(c.begin());
      return vi;
    }
    It end() const noexcept {
      return {{content.front().end()}};
    }

    void update(const It& vi) const noexcept {
      assert(vi.vi.size() == delivery.size());
      for (OS::index_t i = 0; i < delivery.size(); ++i) {
        *delivery[i] = *vi.vi[i];
      }
    }

  };

  template <class CON>
  std::vector<std::vector<typename CON::value_type>>
  get_vcon_range(std::vector<CON> content) {
    typedef typename CON::size_type size_type;
    const size_type size = content.size();
    if (size == 0) return {};
    typedef typename CON::value_type value_type;
    typedef std::vector<std::vector<value_type>> res_t;
    const auto N = content[0].size();
    if (N == 0) return res_t{{}};
    typedef Lockstep<CON, value_type> LS_t;
    typedef typename LS_t::vpelem_t vpelem_t;
    std::vector<value_type> delivery0(size);
    vpelem_t delivery; delivery.reserve(size);
    for (size_type i = 0; i < size; ++i) delivery.push_back(&delivery0[i]);
    LS_t LS(std::move(content), std::move(delivery));
    res_t res; res.reserve(N);
    const auto end = LS.end();
    for (auto it = LS.begin(); it != end; ++it) {
      LS.update(it); res.push_back(delivery0);
    }
    return res;
  }


  typedef Lockstep<OS::vec_t, FP::float80> StdLockstep;
  typedef std::vector<StdLockstep::It> vit_t;
  // The basic lockstep-iterator, plus begin and end:
  typedef std::tuple<std::vector<StdLockstep>, vit_t, vit_t> scanning_t;

  // Creates the scanning-vectors, as a kind of range:
  scanning_t prepare_scanning(
      const OS::evec_t& x, const OS::list_intervals_t& I,
      const RandGen::vec_eseed_t seeds, const bool randomised,
      OS::vec_t& currv) {
    const auto N = x.size();
    assert(N != 0);
    assert(I.size() == N); assert(currv.size() == N);
    assert(valid(I));
    assert(randomised or seeds.empty());
    [[maybe_unused]] const bool has_ai =
      std::ranges::any_of(x, [](const FP::F80ai x){return x.isint;});
    assert(has_ai); // XXX ???
    const bool has_e0 = std::ranges::any_of(x,
      [](const FP::F80ai x){return x.isint and x.hase0;});
    assert(not has_e0 or randomised);
    [[maybe_unused]] const bool has_plus = std::ranges::any_of(x,
      [](const FP::F80ai x){return x.isint and x.hasplus;});
    assert(not has_plus or randomised);

    using vec_t = OS::vec_t;
    const std::vector<vec_t> init_poss = randomised ?
      [&x,&I, &seeds]{RandGen::RandGen_t g(seeds);
                      return fill_possibilities(x, I, &g);}()
      : fill_possibilities(x, I, nullptr);
    assert(init_poss.size() == N);

    auto ipc = init_poss;
    std::vector<StdLockstep> res;
    using vcon_t = StdLockstep::vcon_t;
    using vpelem_t = StdLockstep::vpelem_t;
    using index_t = OS::index_t;
    for (index_t i = 0; i < N; ++i)
      if (not x[i].isint or not x[i].hase0)
        res.emplace_back(vcon_t{std::move(ipc[i])}, vpelem_t{&currv[i]});
    assert(res.size() <= N);
    assert(has_e0 or res.size() == N);
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
        res.emplace_back(std::move(C), std::move(D));
      }
    }
    assert(N == [&res]{
             index_t s = 0;
             for (const auto& ec : res) s += ec.content.size();
             return s;}());
    vit_t b, e; b.reserve(res.size()); e.reserve(res.size());
    for (const auto& ec : res) {
      b.push_back(ec.begin());
      e.push_back(ec.end());
    }
    return {std::move(res), std::move(b), std::move(e)};
  }

  OS::index_t size_scanning(const std::vector<StdLockstep>& S) noexcept {
    OS::index_t res = 1;
    for (const auto& e : S) res *= e.content.front().size();
    return res;
  }

  // Creating the "material" scanning-vectors:
  std::vector<OS::vec_t>
  get_scanning_points(
      const OS::evec_t& x, const OS::list_intervals_t& I,
      const RandGen::vec_eseed_t seeds, const bool randomised) {
    OS::vec_t currv(x.size());
    const auto [equiv_classes, b, e] =
      prepare_scanning(x,I,seeds,randomised, currv);
    auto equicl_it = b;
    std::vector<OS::vec_t> res; res.reserve(size_scanning(equiv_classes));
    do {
      for (OS::index_t i = 0; i < equiv_classes.size(); ++i)
        equiv_classes[i].update(equicl_it[i]);
      res.push_back(currv);
    } while (next_combination(equicl_it, b, e));
    assert(res.size() == size_scanning(equiv_classes));
    std::ranges::sort(res);
    return res;
  }


  /*
    The usual helper-class for parallel computation of the application
    of a function FUN to a scanning-range
     - requirements on FUN: just operator()(vec_t)
  */
  template <class FUN, typename VAL>
  struct Computation_scanning {
    typedef FUN f_t;
    typedef VAL val_t;

    const f_t& f;
    const OS::vec_t* const input;
    val_t* const output;
    const Computation_scanning* next = nullptr;

    Computation_scanning(const f_t& f,
                         const OS::vec_t* const i,
                         val_t* const o) noexcept :
    f(f), input(i), output(o) { assert(input); assert(output); }
    Computation_scanning(const Computation_scanning&) = default;
    Computation_scanning(Computation_scanning&&) = delete;

    void operator()() const noexcept {
      *output = f(*input);
      if (next) next->operator()();
    }
  };

  // Apply f to the x from the scanning-range, and return the vectors
  // of x and f(x) :
  template <typename VAL, class FUN>
  std::pair<std::vector<OS::vec_t>, std::vector<VAL>>
  perform_scanning(const OS::evec_t& x, const OS::list_intervals_t& I,
                   const RandGen::vec_eseed_t seeds, const bool randomised,
                   const FUN& f,
                   const OS::index_t threads) {
    if (threads == 0) return {};
    std::vector<OS::vec_t> inputs = get_scanning_points(x,I,seeds,randomised);
    using index_t = OS::index_t;
    const index_t size = inputs.size();
    if (size == 0) return {};
    std::vector<VAL> outputs; outputs.reserve(size);
    if (threads == 1) {
      for (const auto& x : inputs) outputs.push_back(f(x));
    }
    else {
      outputs.resize(size);
      std::vector<Computation_scanning<FUN, VAL>> computations;
      computations.reserve(size);
      for (index_t i = 0; i < size; ++i)
        computations.emplace_back(f, &inputs[i], &outputs[i]);
      assert(computations.size() == size);
      for (index_t i = 0; i+threads < size; ++i)
        computations[i].next = &computations[i+threads];
      const index_t num_threads = std::min(threads, size);
      std::vector<std::thread> threads; threads.reserve(num_threads);
      for (index_t i = 0; i < num_threads; ++i)
        threads.emplace_back(computations[i]);
      assert(threads.size() == num_threads);
      for (std::thread& t : threads) {
        assert(t.joinable());
        t.join();
      }
    }
    assert(inputs.size() == size and outputs.size() == size);
    return {std::move(inputs), std::move(outputs)};
  }

  /* Helper classes to call an external program ("script"),
     with T(string) being the result, for TRANS T.
      - the type of T(string) is used as result_type, after
        having been stripped of possible references;
      - thus the results can be directly stored in a vector, and
        for example the perfectly-forwarding std::identity can
        be used for TRANS.
  */
  template <class TRANS>
  struct call_script {
    typedef OS::vec_t vec_t;
    typedef TRANS trans_t;
    typedef typename
      std::remove_reference_t<std::invoke_result_t<TRANS, std::string>>
      result_type;

    const std::string& command;
    const trans_t& T;

    call_script(const std::string& command, const trans_t& T) noexcept :
    command(command), T(T) {}

    result_type operator()(const vec_t& x) const {
      std::ostringstream ss;
      FP::fullprec_float80(ss);
      Environment::out_line(ss, x);
      SystemCalls::Popen po(command, std::to_string(FP::hash_UInt_range()(x)));
      const auto res = po.etransfer(SystemCalls::stringref_put(ss.str()));
      if (res.rv.s != SystemCalls::ExitStatus::normal or
          not res.err.empty()) {
        std::ostringstream ss;
        ss << "Sampling::call_script::operator(vec_t): error with calling"
          " command \"" << command << "\", exit-code is " << res.rv.s <<
          ", and error-output is\n  \"" << res.err << "\"";
        throw std::runtime_error(ss.str());
      }
      return T(res.out);
    }
  };

  // Special case of perform_scanning, where f is realised by a "script",
  // invoked by a system-command, which returns a string, and which is
  // transformed via TRANS:
  template <class TRANS>
  std::pair<std::vector<OS::vec_t>,
            std::vector<typename call_script<TRANS>::result_type >>
  perform_scanning_script(const std::istream& in,
                          RandGen::vec_eseed_t& seeds, const bool rand,
                          const std::string& script0, const TRANS& T,
                          const OS::index_t threads) {
    const auto si = OS::read_scanning_info(in);
    if (rand) {
      const auto [seed1, seed2] = hash2(si);
      seeds.push_back(seed1); seeds.push_back(seed2);
      seeds.push_back(Environment::hash(script0));
    }
    typedef call_script<TRANS> script_t;
    const script_t script(script0, T);
    typedef typename script_t::result_type result_type;
    const auto& [I,x] = si;
    return perform_scanning<result_type>(x,I,seeds,rand,script,threads);
  }

  struct string2vec_t {
    typedef OS::vec_t vec_t;
    vec_t operator()(const std::string& s) const {
      return FP::to_vec_float80(s, ' ');
    }
  };

  // The special case of perform_scanning_script, where the result is
  // transformed into vectors:
  std::pair<std::vector<OS::vec_t>, std::vector<OS::vec_t>>
  perform_scanning_script(const std::istream& in,
                          RandGen::vec_eseed_t& seeds, const bool rand,
                          const std::string& script0,
                          const OS::index_t threads) {
    return
      perform_scanning_script(in, seeds, rand, script0,
                              string2vec_t{}, threads);
  }


  const std::string scanning_prefix = "BBs";
  std::string scanning_output(const std::string& grid,
                              const std::string& script) {
    const std::string gridn =
      Environment::str2scorename(
        std::filesystem::path(grid).filename().string());
    const std::string scriptn = Environment::str2scorename(script);
    return scanning_prefix + "_" + gridn + "_" + scriptn + "_" +
      Environment::CurrentTime::timestamp_str() + ".R";
  }

}

#endif
