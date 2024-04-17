// Oliver Kullmann, 28.3.2024 (Swansea)
/* Copyright 2024 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  Algorithms (tools) related to "queens cubings"

   - typedef UInt_t : imported from FloatingPoint

   - typedef vector_t : vector of UInt_t
   - typedef cubing_t : vector of vector_t

   - struct qplaces : wrapper for co, cu : UInt_t
     ("co" for "column" is the queens-value, "cu" like "cube" is the
     index of the queens-cube)
   - typedef qp_selection_t : vector of qplaces (a form of partial square)

   - struct Cubing_t :
      - N, m : UIint_t
      - A : cubing_t
      - valid(qplaces) -> bool
      - valid() -> bool
      - queen(qplaces, i) -> UInt_t
      - queens(qplaces) -> vector_t
      - disjoint(qplaces, qplaces) -> bool
      - valid_selection(qp_selection_t) -> bool
      - disjoint(qp_selection_t) -> bool
      - valid_initial_selection(vector_t) -> bool
      - initial_selection(vector_t) -> qp_selection_t
      - solution(vector_t) -> true

    - read_queens_cubing(istream) -> Cubing_t

    - global variable: expand_total_count : UInt_t
    - all_solutions(Cubing_t, UInt_t depth, vector_t init, ostream,
        EQOptions::OT) : if EQOptions::OT::count_only, then
        set expand_total_count. otherwise output solutions of the given
        depth to ostream
    - all_solutions(Cubing_t, ostream, EQOptions::OT) : calling above
      form of all solutions with full depth and empty init.

*/

#ifndef ALGORITHMS_SuuIcB9aND
#define ALGORITHMS_SuuIcB9aND

#include <vector>
#include <type_traits>
#include <istream>
#include <ostream>

#include <cassert>

#include <Numerics/NumTypes.hpp>
#include <Transformers/Generators/Bicliques/Algorithms.hpp>
#include <Solvers/Gecode/MOLS/OrthogonalArrays.hpp> // subsets(N, k)

#include "EQOptions.hpp"
#include "PQEncoding.hpp"

namespace Algorithms {

  typedef FloatingPoint::UInt_t UInt_t;

  typedef std::vector<UInt_t> vector_t;
  static_assert(std::is_same_v<vector_t, OrthogonalArrays::ls_row_t>);
  static_assert(std::is_same_v<vector_t, OrthogonalArrays::oa_row_t>);
  typedef std::vector<vector_t> cubing_t;

  struct qplaces {
    UInt_t co; // column (or "digit")
    UInt_t cu; // index of cube (Queens-solutions)
    constexpr auto operator <=>(const qplaces& rhs) const noexcept = default;
  };
  typedef std::vector<qplaces> qp_selection_t;

  struct Cubing_t {
    UInt_t N; // order of the square
    UInt_t m; // size of A
    cubing_t A;

    bool valid(const qplaces& p) const noexcept {
      return p.co < N and p.cu < m;
    }
    bool valid() const noexcept {
      if (m != A.size()) return false;
      if (m == 0) return N == 0;
      if (N != A[0].size()) return false;
      for (const vector_t& row : A) {
        if (row.size() != N) return false;
        for (const UInt_t& x : row)
          if (x >= N) return false;
      }
      return true;
    }

    UInt_t queen(const qplaces& p, const UInt_t i) const noexcept {
      return (A[p.cu][i] + p.co) % N;
    }
    vector_t queens(const qplaces& p) const noexcept {
      vector_t res(A[p.cu]);
      for (UInt_t i = 0; i < N; ++i) res[i] = (res[i] + p.co) % N;
      return res;
    }

    bool disjoint(const qplaces& p1, const qplaces& p2) const noexcept {
      assert(valid(p1) and valid(p2));
      for (UInt_t i = 0; i < N; ++i)
        if (queen(p1, i) == queen(p2, i)) return false;
      return true;
    }

    // A valid selection has different (valid) columns:
    bool valid_selection(const qp_selection_t& S) const noexcept {
      for (const qplaces& p : S) if (not valid(p)) return false;
      const auto size = S.size();
      if (size <= 1) return true;
      // Trivial pairwise comparison, assuming N is small:
      for (UInt_t i = 0; i < size-1; ++i) {
        const UInt_t co1 = S[i].co;
        for (UInt_t j = i+1; j < size; ++j)
          if (co1 == S[j].co) return false;
      }
      return true;
    }
    bool disjoint(const qp_selection_t& S) const noexcept {
      const auto size = S.size();
      if (size <= 1) return true;
      for (UInt_t i = 0; i < size-1; ++i) {
        const qplaces& p1 = S[i];
        for (UInt_t j = i+1; j < size; ++j)
          if (not disjoint(p1, S[j])) return false;
      }
      return true;
    }

    // "Initial selections" select for columns 0, 1, ... :
    bool valid_initial_selection(const vector_t& S) const noexcept {
      for (const auto& x : S) if (x >= m) return false;
      return true;
    }
    qp_selection_t initial_selection(const vector_t& S) const noexcept {
      assert(valid_initial_selection(S));
      const auto size = S.size();
      qp_selection_t res; res.reserve(size);
      for (UInt_t i = 0; i < size; ++i)
        res.push_back({i,S[i]});
      return res;
    }
    bool solution(const vector_t& S) const noexcept {
      const qp_selection_t S2 = initial_selection(S);
      return disjoint(S2) and S.size() == N;
    }
  };

  Cubing_t read_queens_cubing(std::istream& in) {
    Cubing_t res{};
    res.A = FloatingPoint::read_gentable<UInt_t>(in);
    res.m = res.A.size();
    if (not res.A.empty()) res.N = res.A[0].size();
    assert(res.valid());
    return res;
  }


  // Functors for obtaining the partial solutions (of type vector_t)
  // as a function-call:
  struct CountOnly {
    UInt_t& count;
    void operator()(const vector_t&) const noexcept {++count;}
  };
  struct CubeIndices {
    std::ostream& out;
    void operator()(const vector_t& v) const {
      Environment::out_line(out, v);
      out << "\n"; out.flush();
    }
  };
  struct DimacsPandiagonal {
    std::ostream& out;
    const Cubing_t& C;
    using dim_t = PQEncoding::dim_t;

    void operator()(const vector_t& v) const {
      assert(C.N <= 65535U);
      const UInt_t co = v.size();
      out << "v";
      for (dim_t q = 0; q < co; ++q) {
        const vector_t Q = C.queens({q,v[q]});
        for (dim_t i = 0; i < C.N; ++i) {
          const dim_t j = Q[i]; // at (i,j) place Queen q
          const UInt_t v = PQEncoding::PEncoding::index({i,j}, q, C.N);
          out << " " << v;
        }
      }
      out << " 0\n"; out.flush();
    }
  };


  UInt_t expand_total_count = 0;
  // Trivial algorithm for finding all solutions:
  template <class OUT>
  void all_solutions(const Cubing_t& C, const UInt_t d, const vector_t init,
                     OUT& out) {
    const UInt_t co = init.size();
    if (d == 0 or co >= C.N) { out(init); return; }
    for (UInt_t cu = 0; cu < C.m; ++cu) {
      const qplaces p{co,cu};
      for (UInt_t co0 = 0; co0 < co; ++co0)
        if (not C.disjoint(p, {co0, init[co0]})) goto END;
      {vector_t ext(init); ext.push_back(cu);
       all_solutions(C, d-1, std::move(ext), out);
      }
      END:;
    }
  }
  void all_solutions(const Cubing_t& C, const UInt_t k) {
    CountOnly co{expand_total_count};
    all_solutions(C, k, {}, co);
  }

}

#endif
