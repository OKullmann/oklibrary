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
#include <map>

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
  using oa_t = ls_t; // orthogonal arrays as vectors of vectors
  typedef std::set<oa_row_t> soa_t; // simple oa's


  void out(std::ostream& out, const oa_row_t& r, const std::string& sep=" ") {
    Environment::out_line(out, r, sep);
  }
  void out(std::ostream& out, const oa_t& S, const std::string& sep = " ") {
    Environment::out_lines(out, S, "\n", sep);
  }
  void out(std::ostream& out, const soa_t& A, const std::string& sep = " ") {
    Environment::out_lines(out, A, "\n", sep);
  }


  soa_t oa2soa(const oa_t& S) { return soa_t(S.begin(), S.end()); }
  oa_t soa2oa(const soa_t& A) { return ls_t(A.begin(), A.end()); }


  size_t maxval(const oa_row_t& r) noexcept {
    if (r.empty()) return 0;
    else return *std::ranges::max_element(r);
  }
  template <class OAT>
  size_t maxval(const OAT& oa) noexcept {
    size_t res = 0;
    for (const oa_row_t& r : oa) res = std::max(res, maxval(r));
    return res;
  }
  template <class OAT>
  size_t maxsize(const OAT& oa) noexcept {
    size_t res = 0;
    for (const oa_row_t& r : oa) res = std::max(res, r.size());
    return res;
  }


  template <class OAT>
  oa_t projection(const OAT& oa, const oa_row_t& indices,
                  const bool sorting = false) {
    if (indices.empty()) return ls_t(oa.size());
    const size_t k = indices.size();
    const size_t m = maxval(indices);
    oa_t res;
    for (const oa_row_t& r : oa) {
      if (m >= r.size()) continue;
      ls_row_t p; p.reserve(k);
      for (const size_t i : indices) p.push_back(r[i]);
      res.push_back(p);
    }
    if (sorting) std::ranges::sort(res);
    return res;
  }


  std::vector<oa_row_t> subsets(const size_t N, const size_t k) {
    if (k > N) return {};
    if (k == 0) return {1,ls_row_t{}};
    if (k == N) {
      std::vector<oa_row_t> res(1); res[0].reserve(N);
      for (size_t i = 0; i < N; ++i) res[0].push_back(i);
      return res;
    }
    if (k == 1) {
      std::vector<oa_row_t> res(N);
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
    std::vector<oa_row_t> res; res.reserve(size);
    res = subsets(N-1, k);
    std::vector<oa_row_t> res2 = subsets(N-1, k-1);
    for (oa_row_t& r : res2) {
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


  // Transformations especially for strength = 2:
  typedef std::map<index_t<2>, oa_row_t> ragged_array_t;
  soa_t rarr2soa(const ragged_array_t& ra) {
    soa_t res;
    for (const auto& [x,y] : ra) {
      oa_row_t r; r.reserve(y.size() + 2);
      r.push_back(x[0]); r.push_back(x[1]);
      for (const size_t yi : y) r.push_back(yi);
      res.insert(std::move(r));
    }
    return res;
  }
  oa_t rarr2oa(const ragged_array_t& ra) {
    oa_t res;
    for (const auto& [x,y] : ra) {
      oa_row_t r; r.reserve(y.size() + 2);
      r.push_back(x[0]); r.push_back(x[1]);
      for (const size_t yi : y) r.push_back(yi);
      res.push_back(std::move(r));
    }
    return res;
  }
  template <class OAT>
  ragged_array_t oa2rarr(const OAT& oa) {
    ragged_array_t res;
    for (const oa_row_t& r : oa) {
      const size_t k = r.size();
      if (k < 2) continue;
      const index_t<2> x{r[0], r[1]};
      const oa_row_t y(r.begin()+2, r.end());
      res[x] = y;
    }
    return res;
  }
  ragged_array_t lls2rarr(const std::vector<ls_t>& L) {
    ragged_array_t res;
    for (const ls_t& S : L) {
      const size_t N = S.size();
      for (size_t i = 0; i < N; ++i) {
        const ls_row_t& r = S[i];
        for (size_t j = 0; j < r.size(); ++j)
          res[{i,j}].push_back(r[j]);
      }
    }
    return res;
  }
  std::vector<ls_t> rarr2lls(const ragged_array_t& ra) {
    if (ra.empty()) return {};
    size_t N = 0, k = 0, max = 0;
    for (const auto& [x,y] : ra) {
      N = std::max({N, x[0], x[1]});
      max = std::max({max, N, maxval(y)});
      k = std::max(k, y.size());
    }
    ++N, ++max;
    std::vector<ls_t> res(k, ls_t(N, ls_row_t(N,max)));
    for (const auto& [x,y] : ra)
      for (size_t i = 0; i < y.size(); ++i)
        res[i][x[0]][x[1]] = y[i];
    return res;
  }
  soa_t lls2soa(const std::vector<ls_t>& L) {
    return rarr2soa(lls2rarr(L));
  }
  oa_t lls2oa(const std::vector<ls_t>& L) {
    return rarr2oa(lls2rarr(L));
  }
  template <class OAT>
  std::vector<ls_t> oa2lls(const OAT& oa) {
    return rarr2lls(oa2rarr(oa));
  }

  std::vector<ls_t> project_lls(const std::vector<ls_t>& L,
                                const ls_row_t& indices) {
    return oa2lls(projection(lls2oa(L), indices));
  }

  template <class ROW>
  bool is_permutation(const ROW& r, const bool one_based = 0) noexcept {
    if (r.empty()) return true;
    const size_t N = r.size();
    std::vector<bool> occ(N);
    for (const size_t x : r)
      if (not one_based) { if (x >= N or occ[x]) return false; occ[x] = true; }
      else { if (x == 0 or x > N or occ[x-1]) return false; occ[x-1] = true; }
    return true;
  }
  ls_t conjugate_ls(const ls_t& L, ls_row_t p, const bool one_based = 0) {
    assert(p.size() == 3);
    assert(is_permutation(p, one_based));
    if (one_based) for (size_t& x : p) --x;
    const auto res = project_lls({L}, p);
    assert(res.size() == 1);
    return res[0];
  }


  // For the creation of the trivial orthogonal arrays:
  oa_t allcombinations(const size_t N, const size_t k) {
    if (k == 0) return {};
    assert(N >= 1);
    if (k == 1) {
      oa_t res(N, ls_row_t(1));
      for (size_t i = 1; i < N; ++i) res[i][0] = i;
      return res;
    }
    else {
      const oa_t res0 = allcombinations(N,k-1);
      oa_t res(N*res0.size(), ls_row_t(k));
      size_t i = 0;
      for (size_t b = 0; b < N; ++b) {
        for (const oa_row_t& r0 : res0) {
          oa_row_t& r = res[i];
          r[0] = b;
          for (size_t j = 0; j < r0.size(); ++j) r[j+1] = r0[j];
          ++i;
        }
      }
      assert(i == res.size());
      return res;
    }
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
  struct SOrthArr {
    static constexpr size_t str = str0;
    soa_t oa;

    const size_t N; // the number of "levels" or "values": 0, ..., N-1
    const size_t nblocks = std::pow(N, str);
    const size_t k; // number of "factors" (columns)
    const size_t rep = 1; // repetitions ("index")
    const size_t trows = rep * nblocks;
    // total number of rows ("number of experimental runs")

    SOrthArr(const soa_t oa) :
      oa(oa), N(detN()), k(maxsize(oa)), rep(detrep()), trows(oa.size()) {}
    SOrthArr(const size_t N, const size_t k) noexcept : N(N), k(k) {}
    SOrthArr(const size_t N, const size_t k, const size_t r)
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
      if (trows != rep * nblocks) return false;
      if (N <= 1) return true;
      if (rep == 0) return true;
      if (str >= k) return true;
      for (const oa_row_t& s : subsets(k,str)) {
        const oa_t p = projection(oa, s, true);
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

  typedef SOrthArr<0> SOrthArr0;
  typedef SOrthArr<1> SOrthArr1;
  typedef SOrthArr<2> SOrthArr2;
  typedef SOrthArr<3> SOrthArr3;

}

#endif
