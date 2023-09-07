// Oliver Kullmann, 28.1.2022 (Swansea)
/* Copyright 2022, 2023 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Tools for creating sampling sequences

      helper functions:
    - allnotequal(VEC v1, VEC v2) -> bool
    - allsamesize(VEC v1, VEC v2) -> bool

    - scoped enum Smode (eq_un, boxed, eq)
    - sampling_points(x_t l, x_t r, index_t M, RandGen_t*, Smode) -> vec_t

    - fill_possibilities(evec_t x, list_intervals_t I, RandGen_t*) ->
        vector<vec_t>
      creates the mesh (grid) for scanning
;
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

TODOS:

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

#include <cassert>

#include <Transformers/Generators/Random/Numbers.hpp>
#include <Transformers/Generators/Random/FPDistributions.hpp>
#include <Transformers/Generators/Random/Algorithms.hpp>

#include "NumTypes.hpp"
#include "NumBasicFunctions.hpp"
#include "NumPrecise.hpp"

#include "OptTypes.hpp"

namespace Sampling {

  namespace FP = FloatingPoint;
  namespace OS = Optimisation;

  // SamplingMode
  enum class Smode {
    eq_un = 0, // equi-distant or uniform random
    boxed = 1, // kind of equi-distant uniform random
    eq = 2     // equi-distant (only)
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
            /* Remark: this computation of x seems most accurate than e.g.
                 const OS::x_t x = FP::fma(ip * (r - l), 1.0L / Mp, l);
                 const OS::x_t x = FP::lerp(l, r, OS::x_t(ip) / Mp);
               seem to yield worse results.
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
            if (groups.contains(M))
              RandGen::shuffle(res.back().begin(), res.back().end(), *rg);
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
            if (groups.contains(M))
              RandGen::shuffle(res.back().begin(), res.back().end(), *rg);
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
  template <class VEC>
  bool allnotequal(const VEC& v1, const VEC& v2) noexcept {
    const auto size = std::min(v1.size(), v2.size());
    for (typename VEC::size_type i = 0; i < size; ++i)
      if (v1[i] == v2[i]) return false;
    return true;
  }
  template <class ITER>
  std::vector<std::vector<ITER>>
  get_vit_range(const std::vector<ITER>& begin,
               const std::vector<ITER>& end) {
    const auto size = begin.size();
    assert(size == end.size());
    std::vector<std::vector<ITER>> res;
    if (begin == end) return res;
    assert(allnotequal(begin, end));
    std::vector<ITER> current = begin;
    do res.push_back(current);
    while (next_combination(current, begin, end));
    assert(current == end);
    return res;
  }


  template<class VEC>
  bool allsamesize(const VEC& v) noexcept {
    const auto size = v.size();
    if (size <= 1) return true;
    const auto size0 = v[0].size();
    for (typename VEC::size_type i = 1; i < size; ++i)
      if (v[i].size() != size0) return false;
    return true;
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
      assert(allsamesize(content));
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
  std::vector<StdLockstep> prepare_scanning(
      const OS::evec_t& x, const OS::list_intervals_t& I,
      RandGen::vec_eseed_t seeds, const bool randomised,
      OS::vec_t& currv) {
    const auto N = x.size();
    assert(I.size() == N);
    assert(valid(I));
    assert(randomised or seeds.empty());
    [[maybe_unused]] const bool has_ai =
      std::ranges::any_of(x, [](const FP::F80ai x){return x.isint;});
    assert(has_ai);
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
    std::vector<StdLockstep> res; res.reserve(N);
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
    return res;
  }

}

#endif
