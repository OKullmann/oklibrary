// Oliver Kullmann, 18.12.2020 (Swansea)
/* Copyright 2020, 2021 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

TODOS:

1. Establish relation to LatinSquares/Mols.cpp.

2. Move general functions later to more appropriate place.

*/

#ifndef LATINSQUARES_WPlhfGMfBs
#define LATINSQUARES_WPlhfGMfBs

#include <type_traits>
#include <vector>
#include <array>
#include <ostream>
#include <algorithm>
#include <numeric>
#include <initializer_list>
#include <utility>
#include <optional>

#include <cassert>
#include <cstdint>

#include "Numbers.hpp"
#include "Algorithms.hpp"
#include "Distributions.hpp"


namespace LatinSquares {

  namespace RG = RandGen;

  typedef std::uint32_t ls_dim_t;
  static_assert(std::is_same_v<ls_dim_t, std::uint32_t>);
  constexpr ls_dim_t max_dim = ls_dim_t(-1);
  static_assert(max_dim == RG::iexp2(32)-1);

  // Counts:
  constexpr ls_dim_t max64_N_all_ls = 7;
  constexpr std::array<std::uint64_t, max64_N_all_ls+1> c_all_ls
    {1, 1, 2, 12, 576, 161280, 812851200, 61479419904000};

  constexpr ls_dim_t max64_N_all_reduced_ls = 9;
  constexpr std::array<std::uint64_t, max64_N_all_reduced_ls+1>
  c_all_reduced_ls
    {1, 1, 1, 1, 4, 56, 9408, 16942080, 535281401856, 377597570964258816};
  constexpr ls_dim_t max64_N_all_hreduced_ls = 8;
  constexpr std::array<std::uint64_t, max64_N_all_hreduced_ls+1>
  c_all_hreduced_ls
    {1, 1, 1, 2, 24, 1344, 1128960, 12198297600, 2697818265354240};


  typedef std::vector<ls_dim_t> ls_row_t;
  typedef std::vector<ls_row_t> ls_t;

  constexpr ls_dim_t max_N_list_reduced_ls = 4;
  // Listing all reduced latin squares of very small order:
  std::vector<ls_t> all_reduced_ls(const ls_dim_t N) noexcept {
    switch(N) {
    case 0 : return {{}};
    case 1 : return {{{0}}};
    case 2 : return {{{0,1},{1,0}}};
    case 3 : return {{{0,1,2},{1,2,0},{2,0,1}}};
    case 4 : return {{{0,1,2,3},{1,2,3,0},{2,3,0,1},{3,0,1,2}},
                     {{0,1,2,3},{1,0,3,2},{2,3,1,0},{3,2,0,1}},
                     {{0,1,2,3},{1,3,0,2},{2,0,3,1},{3,2,1,0}},
                     {{0,1,2,3,},{1,0,3,2},{2,3,0,1},{3,2,1,0}}};
    default : return {};
    }
  }


  /* Valid latin squares: */

  // Valid dimension:
  inline constexpr bool valid(const ls_dim_t N) noexcept {
    return N != 0 and N < max_dim;
  }
  static_assert(not valid(0));
  static_assert(valid(1));
  static_assert(valid(max_dim-1));
  static_assert(not valid(max_dim));


  // Valid value:
  inline constexpr bool valid(const ls_dim_t v, const ls_dim_t N) noexcept {
    return v < N;
  }
  static_assert(not valid(0,0));
  static_assert(valid(0,1));
  inline constexpr bool singular(const ls_dim_t v, const ls_dim_t N) noexcept {
    return v == N;
  }
  static_assert(singular(0,0));
  static_assert(singular(1,1));
  static_assert(not singular(0,1));
  inline constexpr bool valid_partial(const ls_dim_t v, const ls_dim_t N) noexcept {
    return v <= N;
  }
  static_assert(valid_partial(0,0));
  static_assert(valid_partial(0,1));
  static_assert(valid_partial(1,1));
  static_assert(not valid_partial(2,1));


  // Valid complete row:
  inline bool valid_basic(const ls_row_t& r, const ls_dim_t N) noexcept {
    if (not valid(N) or r.size() != N) return false;
    for (const ls_dim_t x : r) if (not valid(x,N)) return false;
    return true;
  }
  // Testing whether all entries are different; for computing
  // invalidating indices see find_first_duplication(r):
  inline bool all_different(const ls_row_t& r) {
    const auto N = r.size();
    assert(valid_basic(r, N));
    std::vector<bool> found(N);
    for (ls_dim_t x : r)
      if (found[x]) return false;
      else found[x] = true;
    return true;
  }
  inline bool valid(const ls_row_t& r, const ls_dim_t N) noexcept {
    return valid_basic(r, N) and all_different(r);
  }

  // Valid partial row:
  inline bool valid_basic_partial(const ls_row_t& r, const ls_dim_t N) noexcept {
    if (not valid(N) or r.size() != N) return false;
    for (const ls_dim_t x : r)
      if (not valid_partial(x,N)) return false;
    return true;
  }
  inline bool all_different_partial(const ls_row_t& r) {
    const auto N = r.size();
    assert(valid_basic_partial(r, N));
    std::vector<bool> found(N);
    for (ls_dim_t x : r)
      if (x != N)
        if (found[x]) return false;
        else found[x] = true;
    return true;
  }
  inline bool valid_partial(const ls_row_t& r, const ls_dim_t N) noexcept {
    return valid_basic_partial(r, N) and all_different_partial(r);
  }

  // Valid complete latin square:
  inline bool valid_basic(const ls_t& L) noexcept {
    const auto N = L.size();
    if (not valid(N)) return false;
    for (const ls_row_t& r : L) if (not valid_basic(r,N)) return false;
    return true;
  }
  inline bool all_different_rows(const ls_t& L) {
    assert(valid_basic(L));
    for (const ls_row_t& r : L) if (not all_different(r)) return false;
    return true;
  }
  inline bool all_different_columns(const ls_t& L) {
    assert(valid_basic(L));
    const auto N = L.size();
    for (ls_dim_t j = 0; j < N; ++j) {
      ls_row_t r((N));
      for (ls_dim_t i = 0; i < N; ++i) r[i] = L[i][j];
      if (not all_different(r)) return false;
    }
    return true;
  }
  inline bool valid(const ls_t& L) {
    return valid_basic(L) and all_different_rows(L) and
      all_different_columns(L);
  }

  // Valid partial latin square:
  inline bool valid_basic_partial(const ls_t& L) noexcept {
    const auto N = L.size();
    if (not valid(N)) return false;
    for (const ls_row_t& r : L) if (not valid_basic_partial(r,N)) return false;
    return true;
  }
  inline bool all_different_rows_partial(const ls_t& L) {
    assert(valid_basic_partial(L));
    for (const ls_row_t& r : L) if (not all_different_partial(r)) return false;
    return true;
  }
  inline bool all_different_columns_partial(const ls_t& L) {
    assert(valid_basic_partial(L));
    const auto N = L.size();
    for (ls_dim_t j = 0; j < N; ++j) {
      ls_row_t r((N));
      for (ls_dim_t i = 0; i < N; ++i) r[i] = L[i][j];
      if (not all_different_partial(r)) return false;
    }
    return true;
  }
  inline bool valid_partial(const ls_t& L) {
    return valid_basic_partial(L) and all_different_rows_partial(L) and
      all_different_columns_partial(L);
  }

  inline bool is_square(const ls_t& L) noexcept {
    const ls_dim_t N = L.size();
    if (N == 0) return true;
    assert(N == L.size() and valid(N));
    for (ls_dim_t i = 0; i < N; ++i)
      if (L[i].size() != N) return false;
    return true;
  }


  // Enumeration of basic matrices (with valid_basic(L)) in lexicographical
  // order:
  inline ls_t first_basic(const ls_dim_t N) {
    return ls_t(N, ls_row_t(N));
  }
  inline bool next_basic(ls_t& L) noexcept {
    assert(valid_basic(L));
    const ls_dim_t N = L.size();
    ls_dim_t i = 0, j = 0;
    while (i < N) {
      if (L[i][j] < N-1) break;
      if (++j == N) {j = 0; ++i; }
    }
    if (i == N) return false;
    ++L[i][j];
    for (ls_dim_t ip = 0; ip < i; ++ip)
      for (ls_dim_t j = 0; j < N; ++j)
        L[ip][j] = 0;
    for (ls_dim_t jp = 0; jp < j; ++jp)
      L[i][jp] = 0;
    return true;
  }


  // Most trivial counting of all latin squares:
  std::uint64_t trivial_count_all_ls(const ls_dim_t N) {
    assert(valid(N));
    ls_t L = first_basic(N);
    std::uint64_t count = 0;
    do count += valid(L);
    while (next_basic(L));
    return count;
  }


  /* Generation of standard objects */

  // Standardised rows:
  ls_row_t standard(const ls_dim_t N) {
    ls_row_t res(N);
    std::iota(res.begin(), res.end(), 0);
    return res;
  }

  // Empty latin squares:
  ls_t empty_ls(const ls_dim_t N) {
    return ls_t(N, ls_row_t(N,N));
  }

  // The cyclic latin square of order N (the multiplication table of the
  // cyclic group):
  ls_t cyclic_ls(const ls_dim_t N) {
    assert(valid(N));
    ls_t L(N, ls_row_t(N));
    for (ls_dim_t i = 0; i < N; ++i)
      for (ls_dim_t j = 0; j < N; ++j)
        L[i][j] = (i + j) % N;
    assert(valid(L));
    return L;
  }


  /* Helper functions */

  // Find first duplication in r, returning their indices (sorted):
  typedef std::array<ls_dim_t,2> index_pair_t;
  index_pair_t find_first_duplication(const ls_row_t& r) {
    assert(valid_basic(r, r.size()));
    const ls_dim_t N = r.size();
    std::vector<ls_dim_t> found(N,N);
    for (ls_dim_t i = 0; i < N; ++i)
      if (found[r[i]] != N) return {found[r[i]], i};
      else found[r[i]] = i;
    return {N,N};
  }


  /* Basic operations */

  ls_t transpose(const ls_t& L) {
    const ls_dim_t N = L.size();
    ls_t res(N,ls_row_t(N));
    for (ls_dim_t i = 0; i < N; ++i) {
      assert(L[i].size() == N);
      for (ls_dim_t j = 0; j < N; ++j)
        res[j][i] = L[i][j];
    }
    return res;
  }

  bool has_standardised_first_column(const ls_t& L) noexcept {
    for (ls_dim_t i = 0; i < L.size(); ++i) {
      assert(not L[i].empty());
      if (L[i][0] != i) return false;
    }
    return true;
  }
  ls_t standardise_first_column(const ls_t& L) {
    const ls_dim_t N = L.size();
    ls_t res(N);
    for (ls_dim_t i = 0; i < N; ++i) {
      assert(not L[i].empty() and L[i][0] < N);
      res[L[i][0]] = L[i];
    }
    return res;
  }

  bool has_standardised_first_row(const ls_t& L) noexcept {
    if (L.empty()) return true;
    const ls_dim_t N = L[0].size();
    for (ls_dim_t i = 0; i < N; ++i) if (L[0][i] != i) return false;
    return true;
  }
  ls_t standardise_first_row(const ls_t& L) {
    return transpose(standardise_first_column(transpose(L)));
  }

  bool is_standardised(const ls_t& L) noexcept {
    return has_standardised_first_row(L) and
      has_standardised_first_column(L);
  }
  ls_t standardise(const ls_t& L) {
    return standardise_first_column(standardise_first_row(L));
  }

  enum class StRLS {none=0, row=1, column=2, both=3};
  ls_t standardise(const ls_t& L, const StRLS ost) {
    switch (ost) {
    case StRLS::none   : return L;
    case StRLS::row    : return standardise_first_row(L);
    case StRLS::column : return standardise_first_column(L);
    default            : return standardise(L);
    }
  }
  constexpr bool available_counts(const ls_dim_t N, const StRLS o) noexcept {
    switch (o) {
    case StRLS::none : return N <= max64_N_all_ls;
    case StRLS::both : return N <= max64_N_all_reduced_ls;
    default : return N <= max64_N_all_hreduced_ls;
    }
  }
  constexpr std::uint64_t count_ls(const ls_dim_t N, const StRLS o) noexcept {
    assert(available_counts(N,o));
    switch (o) {
    case StRLS::none : return c_all_ls[N];
    case StRLS::both : return c_all_reduced_ls[N];
    default : return c_all_hreduced_ls[N];
    }
  }


  /* Basic output */

  std::ostream& operator <<(std::ostream& out, const ls_row_t& r) {
    const auto N = r.size();
    if (N == 0) return out;
    if (r[0] != N) out << r[0]; else out << "*";
    for (ls_dim_t i = 1; i < N; ++i)
      if (r[i] != N) out << " " << r[i]; else out << " *";
    return out;
  }
  struct LS_t {
    const ls_t& L;
  };
  std::ostream& operator <<(std::ostream& out, const ls_t& L) {
    for (const auto& r : L) out << r << "\n";
    return out;
  }
  std::ostream& operator <<(std::ostream& out, const LS_t& L) {
    for (const auto& r : L.L) out << r << "\n";
    return out;
  }
  std::ostream& operator <<(std::ostream& out, const index_pair_t& p) {
    return out << p[0] << "," << p[1];
  }


  /* Computing SDRs (systems of distinct representatives) */

  /* The set-systems used here use the integers 0, ..., N-1
     as indices as well as for values, where N is the number of
     indices, given by the size of the vector of values.
     Thus here only cases are handled where the number of values is
     at most the number of indices, and where the values are directly
     used as indices.
  */

  typedef std::vector<ls_dim_t> set_t;
  struct Set {
    set_t s;
    Set(std::initializer_list<ls_dim_t> l) : s(l) {}
    Set(const set_t& s) : s(s) {}

    ls_dim_t size() const noexcept { return s.size(); }
    bool contains(const ls_dim_t x) const noexcept {
      return std::binary_search(s.begin(), s.end(), x);
    }
    void remove(const ls_dim_t x) noexcept {
      const auto it = std::lower_bound(s.begin(),s.end(),x);
      assert(it != s.end());
      s.erase(it);
    }

    void sort() noexcept { std::sort(s.begin(),s.end()); }

    bool operator ==(const Set& rhs) const noexcept = default;

    auto begin() noexcept {return s.begin();}
    auto begin() const noexcept {return s.cbegin();}
    auto end() noexcept {return s.end();}
    auto end() const noexcept {return s.cend();}

    friend std::ostream& operator <<(std::ostream& out, const Set& s) {
      for (const ls_dim_t x : s) out << x << ",";
      return out;
    }
  };
  bool valid_basic(const Set& s, const ls_dim_t N) noexcept {
    if ( N >= max_dim) return false;
    for (const ls_dim_t x : s.s) if (not valid(x,N)) return false;
    return true;
  }
  bool valid(const Set& s, const ls_dim_t N) noexcept {
    return valid_basic(s,N) and std::is_sorted(s.s.begin(), s.s.end())
      and std::adjacent_find(s.s.begin(),s.s.end()) == s.s.end();
  }

  typedef std::vector<Set> setsystem_t;
  struct SetSystem {
    setsystem_t S;
    SetSystem(std::initializer_list<Set> l) : S(l) {}
    SetSystem(const ls_dim_t N) {
      S.reserve(N);
      for (ls_dim_t i = 0; i < N; ++i) S.emplace_back(standard(N));
    }
    auto size() const noexcept { return S.size(); }
    bool empty() const noexcept { return S.empty(); }

    void sort() noexcept { for (Set& s : S) s.sort(); }

    auto begin() noexcept {return S.begin();}
    auto begin() const noexcept {return S.cbegin();}
    auto end() noexcept {return S.end();}
    auto end() const noexcept {return S.cend();}

    friend std::ostream& operator <<(std::ostream& out, const SetSystem& S) {
      for (const Set s : S) out << s << ";";
      return out;
    }
  };
  bool valid_basic(const SetSystem& S) noexcept {
    const auto N = S.size();
    if (N >= max_dim) return false;
    for (const Set& s : S.S) if (not valid_basic(s,N)) return false;
    return true;
  }
  bool valid(const SetSystem& S) noexcept {
    const auto N = S.size();
    if (N >= max_dim) return false;
    for (const Set& s : S.S) if (not valid(s,N)) return false;
    return true;
  }

  /* Due to the restricted nature of set-systems here, a system of
     distinct representatives is exactly a valid row of a latin square
     of size N = number of sets, such that each element of the row is
     in the corresponding set:
  */
  bool is_sdr(const ls_row_t f, const SetSystem& S) noexcept {
    assert(f.size() == S.size());
    const ls_dim_t N = S.size();
    if (N == 0) return true;
    assert(valid(f,N));
    assert(valid(S));
    for (ls_dim_t i = 0; i < N; ++i)
      if (not S.S[i].contains(f[i])) return false;
    return true;
  }
  bool is_psdr(const ls_row_t f, SetSystem S) noexcept {
    assert(f.size() == S.size());
    const ls_dim_t N = S.size();
    if (N == 0) return true;
    assert(valid_partial(f,N));
    S.sort();
    assert(valid(S));
    for (ls_dim_t i = 0; i < N; ++i)
      if (f[i] != N and not S.S[i].contains(f[i])) return false;
    return true;
  }


  typedef std::vector<std::pair<ls_dim_t,ls_dim_t>> ls_map_t;
  std::ostream& operator <<(std::ostream& out, const ls_map_t m) {
    for (const auto [x,y] : m) out << "(" << x << "," << y << ")";
    return out;
  }

  // Partial bijection between indices and values:
  class PBij {
    ls_row_t f, b; // forward, backward
    ls_dim_t s = 0;
    ls_dim_t N;
  public :
    PBij(const ls_dim_t N) : f(N,N), b(N,N), N(N) {
      assert(N < max_dim);
    }

    ls_dim_t insert(const ls_map_t& m) noexcept {
      ls_dim_t count = 0;
      for (const auto [x,y] : m) count += set(x,y);
      return count;
    }

    PBij(const ls_dim_t N, const ls_map_t& m) : PBij(N) {
      [[maybe_unused]] const ls_dim_t inserted = insert(m);
      assert(inserted == m.size());
    }

    ls_dim_t size() const noexcept { return s; }
    ls_dim_t total_size() const noexcept { return N; }
    bool empty() const noexcept { return s == 0; }
    bool total() const noexcept { return s == N; }

    ls_dim_t operator()(const ls_dim_t i) const noexcept {
      assert(i < N);
      return f[i];
    }
    ls_dim_t operator[](const ls_dim_t v) const noexcept {
      assert(v < N);
      return b[v];
    }

    bool set(const ls_dim_t i, const ls_dim_t v) noexcept {
      assert(i < N);
      assert(v < N);
      if (f[i] != N or b[v] != N) return false;
      f[i] = v; b[v] = i; ++s;
      return true;
    }
    void unset(const ls_dim_t i) noexcept {
      assert(i < N);
      if (f[i] != N) {b[f[i]] = N; f[i] = N; --s;}
    }

    const ls_row_t& r() const noexcept { return f; }

    bool operator ==(const PBij& rhs) const noexcept = default;

    friend std::ostream& operator <<(std::ostream& out, const PBij& pb) {
      return out << pb.f << ";" << pb.b;
    }

  };


  // Randomly choose assignments, computing a maximal psdr; no unit-clause
  // propagation, since used here only on regular bipartite graphs:
  PBij random_psdr(const SetSystem& S, RG::RandGen_t& g) {
    assert(valid(S));
    const ls_dim_t N = S.size();
    PBij res(N);
    if (S.empty()) return res;
    ls_row_t order = standard(N);
    RG::shuffle(order.begin(), order.end(), g);
    for (const ls_dim_t x : order) {
      auto values = S.S[x].s;
      RG::shuffle(values.begin(), values.end(), g);
      for (const ls_dim_t y : values) if (res.set(x,y)) break;
    }
    return res;
  }

  void remove_psdr(const PBij& p, SetSystem& S) noexcept {
    assert(is_psdr(p.r(),S));
    const ls_dim_t N = S.size();
    for (ls_dim_t i = 0; i < N; ++i) {
      ls_dim_t v = p(i);
      if (v != N) S.S[i].remove(v);
    }
  }

  struct PartiallyFilled {
    ls_t L;
    ls_dim_t rows_completed;
    SetSystem A; // available values
    PBij next; // next row
  };

  std::ostream& operator <<(std::ostream& out, const PartiallyFilled& P) {
    return out << P.L << P.rows_completed << ":\n" << P.next.r() << "\n";
  }

  PartiallyFilled random_pls(const ls_dim_t N, RG::RandGen_t& g) {
    assert(valid(N));
    PartiallyFilled res{empty_ls(N), 0, N, N};
    do {
      res.next = random_psdr(res.A, g);
      if (res.next.total()) {
        for (ls_dim_t i = 0; i < N; ++i)
          res.L[res.rows_completed][i] = res.next(i);
        remove_psdr(res.next, res.A);
        ++res.rows_completed;
      }
      else return res;
    } while (res.rows_completed < N);
    return res;
  }


  inline PBij improve(const PBij& init, const SetSystem& A, const ls_row_t& back_arcs, const ls_map_t& final) {
    assert(is_psdr(init.r(), A));
    const ls_dim_t N = A.size();
    PBij res(N);
    ls_dim_t failures = 0;
    for (auto [x,y] : final) {
      assert(init[y] == N);
      [[maybe_unused]] const bool success = res.set(x,y);
      assert(success);
      while (init(x) != N) {
        y = init(x); x = back_arcs[y];
        if (not res.set(x,y)) {
          ++failures;
          do {
            x = init[y]; y = res(x);
            res.unset(x);
          } while (init[y] != N);
          goto Loop_end;
        }
      }
    Loop_end: ;
    }
    assert(failures < final.size());
    for (ls_dim_t i = 0; i < N; ++i)
      if (init(i) != N and res(i) == N) {
        [[maybe_unused]] const bool success = res.set(i, init(i));
        assert(success);
      }
    assert(res.size() == init.size() + final.size() - failures);
    return res;
  }

  /* Starting with a given partial sdr init for A, compute a maximum
     partial sdr for A, randomised:
  */
  std::optional<PBij> maximise_once(const PBij& init, SetSystem A, RG::RandGen_t& g) {
    assert(is_psdr(init.r(), A));
    const ls_dim_t N = A.size();
    assert(init.total_size() == N);
    const ls_dim_t M = init.size();
    if (M == N) return std::nullopt;
    for (ls_dim_t i = 0; i < N; ++i) {
      auto& set = A.S[i].s;
      RG::shuffle(set.begin(),set.end(),g);
    }

    ls_row_t back_arcs(N,N);
    ls_row_t next;
    for (ls_dim_t i = 0; i < N; ++i) if (init(i)==N) next.push_back(i);
    assert(next.size() <= N-M);
    RG::shuffle(next.begin(), next.end(), g);

    while (true) {
      ls_t alt_values(next.size());
      for (ls_dim_t i = 0; i < next.size(); ++i) {
        const ls_dim_t x = next[i];
        for (const ls_dim_t y : A.S[x].s)
          if (init[y] == N) { // found augmenting path
            ls_map_t final;
            std::vector<bool> used(N);
            final.push_back({x,y}); used[y] = true;
            for (++i; i < next.size(); ++i) {
              const ls_dim_t x = next[i];
              for (const ls_dim_t y : A.S[x].s)
                if (init[y] == N and not used[y]) {
                  final.push_back({x,y}); used[y] = true;
                  break;
                }
            }
            RG::shuffle(final.begin(), final.end(), g);
            return improve(init, A, back_arcs, final);
          }
          else if (back_arcs[y] == N) alt_values[i].push_back(y);
        RG::shuffle(alt_values[i].begin(), alt_values[i].end(), g);
      }
      ls_row_t new_next;
      bool values_left;
      do {
        values_left = false;
        for (ls_dim_t i = 0; i < alt_values.size(); ++i) {
          if (alt_values[i].empty()) continue;
          const ls_dim_t x = next[i];
          const ls_dim_t y = alt_values[i].back();
          if (back_arcs[y] == N) {
            back_arcs[y] = x;
            new_next.push_back(init[y]);
          }
          alt_values[i].pop_back();
          if (not alt_values[i].empty()) values_left = true;
        }
      } while (values_left);
      if (new_next.empty()) return std::nullopt;
      next = new_next;
    }
  }

  PBij maximise(PBij init, const SetSystem& A, RG::RandGen_t& g) {
    for (std::optional<PBij> res = maximise_once(init, A, g);
         res;
         res = maximise_once(init, A, g))
      init = *res;
    return init;
  }


  enum class CrRLS {with_initial_phase = 0, without_initial_phase = 1};

  ls_t random_ma_ls(const ls_dim_t N, const CrRLS ocr, RG::RandGen_t& g) {
    assert(valid(N));
    ls_t L;
    SetSystem A(N);
    ls_dim_t row = 0;

    if (ocr == CrRLS::with_initial_phase) {
      PartiallyFilled pls = random_pls(N, g);
      L = std::move(pls.L);
      row = pls.rows_completed;
      if (row == N) return L;
      A = std::move(pls.A);
      PBij next = maximise(pls.next, A, g);
      assert(next.size() == N);
      L[row++] = std::move(next.r());
      remove_psdr(next, A);
    } else L = ls_t(N);

    for (; row < N; ++row) {
      PBij next = maximise(PBij(N), A, g);
      assert(next.size() == N);
      L[row] = std::move(next.r());
      remove_psdr(next, A);
    }
    return L;
  }


  /* The Jacobson-Matthews generator */

  // The special cell is used if a current square is improper;
  // x and y are indices of the cell, while triple (i,j,k) is cell's value:
  struct SpecialCell {
    ls_dim_t x, y;
    ls_dim_t i, j, k;
    bool active;
  };
  constexpr bool valid(const SpecialCell& s, const ls_dim_t N) noexcept {
    if (not(s.x<N and s.y<N and s.i<N and s.j<N and s.k<N)) return false;
    if (not s.active) return true;
    return s.i!=s.j and s.i!=s.k and s.j!=s.k;
  }

  struct StatsJM {
    typedef RG::gen_uint_t count_t;
    const count_t initial_rounds;
    count_t additional_rounds=0, proper_ls_encountered=0;
    constexpr StatsJM(const count_t T) noexcept : initial_rounds(T) {}
  };

  class JacobsMatthews {
    const ls_dim_t N;
    ls_t L;
    SpecialCell scell;
    StatsJM stats;
    RG::RandGen_t& g;

  public:

    constexpr static ls_dim_t max_N = 2642245;
    STATIC_ASSERT(max_N == RG::gen_uint_t(std::cbrt(RG::randgen_max)));
    constexpr static bool valid(const ls_dim_t N) noexcept {
      return  N <= max_N;
    }
    constexpr static RG::gen_uint_t cb(const ls_dim_t N) noexcept {
      assert(valid(N));
      const RG::gen_uint_t n{N};
      return n*n*n;
    }

    JacobsMatthews(const ls_dim_t N, RG::RandGen_t& g) noexcept :
        N(N),
        L(cyclic_ls(N)),
        scell{0,0,0,0,0,false},
        stats(cb(N)),
        g(g) {
      assert(valid(N));
      if (N>1) iterate();
    }
    JacobsMatthews(ls_t L, RG::RandGen_t& g) noexcept :
        N(L.size()),
        L(L),
        scell{0,0,0,0,0,false},
        stats(cb(N)),
        g(g) {
      assert(valid(N));
      assert(LatinSquares::valid(L));
      if (N>1) iterate();
    }

    const ls_t& ls() const noexcept { return L; }
    const StatsJM& statistics() const noexcept { return stats; }

  private:

    void iterate() noexcept {
      for (RG::gen_uint_t i = 0; i < stats.initial_rounds; ++i)
        perturbate_square();
      while (not LatinSquares::valid(L)) {
        perturbate_square(); ++stats.additional_rounds;
      }
    }

    // Perturbate current square:
    void perturbate_square() noexcept {
      ls_dim_t modrow, modcol, modcellnewv, modcelloldv,
        opposrow, opposcol, opposcellv;

      if (not scell.active) {
        // Randomly choose cell and its new value:
        {RG::UniformRange U(g, N);
         modrow = U(); modcol = U();
         modcelloldv = L[modrow][modcol];
         modcellnewv = RG::UniformRange(g,N-1)();
         if (modcellnewv >= modcelloldv) ++modcellnewv;
        }
        // Determine the 2*2 subsquare for modification:
        const ls_row_t& row = L[modrow];
        {const auto it = std::find(row.begin(), row.end(), modcellnewv);
         assert(it != row.end());
         opposcol = std::distance(row.begin(), it);
        }
        ls_row_t col(N);
        for (unsigned i = 0; i < N; ++i) col[i] = L[i][modcol];
        {const auto it = std::find(col.begin(), col.end(), modcellnewv);
         assert(it != col.end());
         opposrow = std::distance(col.begin(), it);
        }
        opposcellv = L[opposrow][opposcol];
        // Update the found 2*2 subsquare:
        L[modrow][modcol] = modcellnewv;
        L[modrow][opposcol] = modcelloldv;
        L[opposrow][modcol] = modcelloldv;
        L[opposrow][opposcol] = modcellnewv;
      }
      else {
        // Randomly choose a positive value (one of two) in the special cell:
        const ls_dim_t firstimpposv = RG::bernoulli(g) ? scell.i : scell.j;
        modcelloldv = (firstimpposv == scell.i) ? scell.j : scell.i;
        // Randomly choose one of two duplicate indices in the improper row:
        const ls_row_t& row = L[scell.x];
        {const std::array<ls_dim_t,2> duplvinds = find_first_duplication(row);
         opposcol = duplvinds[RG::bernoulli(g)];
        }
        // Randomly choose one of two duplicate indices in the improper column:
        {ls_row_t col(N);
         for (unsigned i = 0; i < N; ++i) col[i] = L[i][scell.y];
         {const std::array<ls_dim_t,2> duplvinds = find_first_duplication(col);
          opposrow = duplvinds[RG::bernoulli(g)];
         }
        }
        // Modify values of the formed subsquare:
        assert(L[scell.x][opposcol] == L[opposrow][scell.y]);
        modcellnewv = L[scell.x][opposcol];
        opposcellv = L[opposrow][opposcol];
        L[scell.x][scell.y] = firstimpposv;
        L[scell.x][opposcol] = modcelloldv;
        L[opposrow][scell.y] = modcelloldv;
        L[opposrow][opposcol] = modcellnewv;
      }
      assert(valid_basic(L));

      if (LatinSquares::valid(L))
        {scell.active = false; ++stats.proper_ls_encountered;}
      else
        scell = {opposrow,opposcol, opposcellv,modcellnewv,modcelloldv, true};
    }

  };


  // Construct selection-object by giving the number of selected rows, columns,
  // and additional single cells, stores for rows and columns the
  // number of deletions:
  struct Selection {
    const ls_dim_t N, r, c;
    const ls_dim_t remaining, s;
    const bool additional_cells;

    constexpr Selection(const ls_dim_t N, const ls_dim_t r_, const ls_dim_t c_, const ls_dim_t s) noexcept : N(N), r(N-r_), c(N-c_), remaining(N*N-r_*c_), s(s), additional_cells(s != 0) {
      assert(valid(N));
      assert(r_ <= N and c_ <= N and s <=  remaining);
    }
    constexpr Selection(const ls_dim_t N) noexcept : Selection(N,N,N,0) {}
  };


  ls_t select(ls_t L, const Selection& sel, RG::RandGen_t& g) noexcept {
    const ls_dim_t N = sel.N;
    assert(sel.N == L.size());
    assert(is_square(L));
    const auto sel_row = RG::choose_kn(sel.r, N, g, true);
    const auto sel_col = RG::choose_kn(sel.c, N, g, true);

    if (not sel.additional_cells) {
      for (ls_dim_t i = 0, i_sel = 0; i < N; ++i) {
        if (i_sel != sel.r and i == sel_row[i_sel]) {
          std::fill(L[i].begin(), L[i].end(), N);
          ++i_sel; continue;
        }
        for (ls_dim_t j=0, j_sel=0; j < N; ++j)
          if (j_sel != sel.c and j == sel_col[j_sel]) {
            L[i][j] = N; ++j_sel;
          }
      }
    }
    else {
      std::vector<std::pair<index_pair_t, ls_dim_t>> rem_cells;
      rem_cells.reserve(sel.remaining);
      for (ls_dim_t i = 0, i_sel = 0; i < N; ++i) {
        if (i_sel != sel.r and i == sel_row[i_sel]) {
          for (ls_dim_t j = 0; j < N; ++j) {
            rem_cells.push_back({{i,j}, L[i][j]});
            L[i][j] = N;
          }
          ++i_sel; continue;
        }
        for (ls_dim_t j=0, j_sel=0; j < N; ++j)
          if (j_sel != sel.c and j == sel_col[j_sel]) {
            rem_cells.push_back({{i,j}, L[i][j]});
            L[i][j] = N; ++j_sel;
          }
      }
      assert(rem_cells.size() == sel.remaining);
      const auto selection = RG::choose_kn(sel.s, sel.remaining, g);
      for (const auto i : selection) {
        const auto [p,v] = rem_cells[i];
        assert( L[p[0]][p[1]] == N);
        L[p[0]][p[1]] = v;
      }
    }
    return L;
  }

}

#endif
