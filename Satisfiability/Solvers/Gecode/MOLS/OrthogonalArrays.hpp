// Oliver Kullmann, 27.4.2022 (Swansea)
/* Copyright 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  Tools for handling orthogonal arrays

  Namespace OrthogonalArrays, abbreviated "OA".

  For basic definitions see
  https://en.wikipedia.org/wiki/Orthogonal_array

  We consider here only simple orthogonal arrays (no repeated rows).

*/

#ifndef ORTHOGONALARRAYS_TgDVWG9CBQ
#define ORTHOGONALARRAYS_TgDVWG9CBQ

#include <vector>
#include <set>
#include <algorithm>
#include <sstream>
#include <utility>
#include <exception>
#include <type_traits>

#include <cassert>
#include <cmath>

#include <ProgramOptions/Strings.hpp>
#include <Numerics/FloatingPoint.hpp>

#include "Conditions.hpp"
#include "BasicLatinSquares.hpp"

namespace OrthogonalArrays {

  namespace BS = BasicLatinSquares;

  typedef Conditions::size_t size_t;
  typedef BS::ls_row_t ls_row_t;
  typedef BS::ls_t ls_t;

  using oa_row_t = ls_row_t; // the rows of the orthogonal array
  typedef std::set<oa_row_t> oa_t; // underlying concrete alias type of oa's


  void out(std::ostream& out, const ls_row_t& r, const std::string& sep=" ") {
    Environment::out_line(out, r, sep);
  }
  void out(std::ostream& out, const ls_t& S, const std::string& sep = " ") {
    Environment::out_lines(out, S, "\n", sep);
  }
  void out(std::ostream& out, const oa_t& A, const std::string& sep = " ") {
    Environment::out_lines(out, A, "\n", sep);
  }


  oa_t ls2oa(const ls_t& S) { return oa_t(S.begin(), S.end()); }
  ls_t oa2ls(const oa_t& A) { return ls_t(A.begin(), A.end()); }


  size_t maxval(const oa_row_t& r) noexcept {
    if (r.empty()) return 0;
    else return *std::ranges::max_element(r);
  }
  size_t maxval(const oa_t& oa) noexcept {
    size_t res = 0;
    for (const oa_row_t& r : oa) res = std::max(res, maxval(r));
    return res;
  }
  size_t maxsize(const oa_t& oa) noexcept {
    size_t res = 0;
    for (const oa_row_t& r : oa) res = std::max(res, r.size());
    return res;
  }


  ls_t projection(const oa_t& oa, const ls_row_t& indices) {
    if (indices.empty()) {
      if (oa.empty()) return ls_t(0);
      else return ls_t(1);
    }
    const size_t k = indices.size();
    const size_t m = maxval(indices);
    ls_t res;
    for (const oa_row_t& r : oa) {
      if (m >= r.size()) continue;
      ls_row_t p; p.reserve(k);
      for (const size_t i : indices) p.push_back(r[i]);
      res.push_back(p);
    }
    return res;
  }


  std::vector<ls_row_t> subsets(const size_t N, const size_t k) {
    if (k > N) return {};
    if (k == 0) return {1,ls_row_t{}};
    if (k == N) {
      std::vector<ls_row_t> res(1); res[0].reserve(N);
      for (size_t i = 0; i < N; ++i) res[0].push_back(i);
      return res;
    }
    if (k == 1) {
      std::vector<ls_row_t> res(N);
      for (size_t i = 0; i < N; ++i) res[i].push_back(i);
      return res;
    }
    const auto size0 = FloatingPoint::fbinomial_coeff(N, k);
    if (size0 >= FloatingPoint::P264) {
      std::ostringstream ss;
      using W = FloatingPoint::Wrap;
      ss << "OrthogonalArrays::subsets: N=" << N << ", k=" << k <<
        " yields size=" << W(size0) << " >= 2^64=" << W(FloatingPoint::P264);
      throw std::runtime_error(ss.str());
    }
    const size_t size = size0;
    std::vector<ls_row_t> res; res.reserve(size);
    res = subsets(N-1, k);
    std::vector<ls_row_t> res2 = subsets(N-1, k-1);
    for (ls_row_t& r : res2) {
      res.push_back(std::move(r));
      res.back().push_back(N-1);
    }
    assert(res.size() == size);
    return res;
  }


  template <size_t k>
  using index_t = std::array<size_t, k>;
  template <size_t k>
  constexpr index_t<k-1> rest(const index_t<k>& v) noexcept {
    static_assert(k >= 1);
    index_t<k-1> res;
    std::copy(v.begin()+1, v.end(), res.begin());
    return res;
  }


  template <size_t D>
  struct GenLS {
    typedef std::vector<typename GenLS<D-1>::type> type;
    static constexpr type create(const size_t N) {
      return type(N, GenLS<D-1>::create(N));
    }
    template <size_t k>
    static constexpr typename GenLS<D-k>::type
    apply(const typename GenLS::type& A,
          const index_t<k>& v) noexcept {
      static_assert(k <= D);
      if constexpr (k == 0) return A;
      else if constexpr (k == 1) {
        assert(v[0] < A.size());
        return A[v[0]];
      }
      else {
        assert(v[0] < A.size());
        const index_t<k-1> nv = rest(v);
        return GenLS<D-1>::template apply<k-1>(A[v[0]], nv);
      }
    }
  };
  template <>
  struct GenLS<0> {
    typedef size_t type;
    static constexpr type create(const size_t N) noexcept { return N; }
    static constexpr size_t apply(const size_t A,
                                  const index_t<0>&) noexcept {
      return A;
    }
  };
  template <size_t D>
  using GLS = typename GenLS<D>::type;
  static_assert(std::is_same_v<GLS<0>, size_t>);
  static_assert(std::is_same_v<GLS<1>, ls_row_t>);
  static_assert(std::is_same_v<GLS<2>, ls_t>);
  static_assert(std::is_same_v<GLS<3>, std::vector<ls_t>>);


  // A "possible str-(N,k,rep) orthogonal array" ("str" means "strength"):
  template <size_t str0>
  struct OrthArr {
    static constexpr size_t str = str0;
    oa_t oa;

    const size_t N; // the number of "levels" or "values": 0, ..., N-1
    const size_t nblocks = std::pow(N, str);
    const size_t k; // number of "factors" (columns)
    const size_t rep = 1; // repetitions ("index")
    const size_t trows = rep * nblocks;
    // total number of rows ("number of experimental runs")

    OrthArr(const oa_t oa) :
      oa(oa), N(detN()), k(maxsize(oa)), rep(detrep()) {}
    OrthArr(const size_t N, const size_t k) noexcept : N(N), k(k) {}
    OrthArr(const size_t N, const size_t k, const size_t r)
      noexcept : N(N), k(k), rep(r) {}

    size_t detN() const noexcept {
      if (noval()) return 0;
      else return maxval(oa) + 1;
    }
    size_t detrep() const noexcept {
      if (nblocks == 0) return 0;
      const size_t s = oa.size();
      if (s % nblocks == 0) return s / nblocks;
      else return s / nblocks + 1;
    }

    bool noval() const noexcept {
      if (oa.empty()) return true;
      if (oa.size() == 1 and oa.contains({})) return true;
      return false;
    }
    bool propval() const noexcept {
      if (noval()) return true;
      else return maxval(oa) < N;
    }

    bool propfactors() const noexcept {
      return std::ranges::all_of(oa,
               [this](const oa_row_t& r){return r.size() == k;});
    }

    bool valid() const {
      if (not propval() or not propfactors() or oa.size() != trows)
        return false;
      if (N <= 1) return true;
      if (rep == 0) return true;
      if (str >= k) return true;
      for (const ls_row_t& s : subsets(N,k)) {
        const ls_t p = projection(oa, s);
        assert(std::ranges::is_sorted(p));
        assert(p.size() == trows);
        size_t curr = 0;
        for (size_t i = 0; i < nblocks; ++i) {
          for (size_t j = 0; j < rep; ++j, ++curr)
            if (j+1 < rep and p[curr] != p[curr+1]) return false;
          if (i+1 < nblocks and p[curr] <= p[curr-1]) return false;
        }
      }
      return true;
    }

  };

  typedef OrthArr<2> OrthArr2;

}

#endif
