// Oliver Kullmann, 26.3.2023 (Swansea)
/* Copyright 2023 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  General algorithms (tools)

   Set operations:

   - empty_intersection(RAN1 r1, RAN2 r2) (r1, r2 must be sorted)

   - complement_uint(RAN r, UINT N) -> vector<UINT> :
     returns the sorted vector of elements of {0, ..., N-1} not in r
     (assumes r to be sorted)
   - complement_subsequence(RAN1 a, RAN2 b) -> vector<RAN2::value_type>
     assumes a is a subsequence of b, and returns "b - a"

   Sequence operations:

   - append_ranges(RAN1 r1, RAN2 r2) -> RAN1 (copies r1, and appends to it)
   - append_ranges(RAN1 r1, RAN2 r2, RAN3 r3) -> RAN1

   - erase_if_unstable(vec, pred) : possibly faster than std::erase_if due to
     not keeping the order of vec

   - sum_sizes(RAN) -> FloatingPoint::UInt_t : summation of member.size()

   Graph algorithms:

   - finding greedily a maximal independent set in a graph, given by
     a predicate:
     - is_independent(VEC v, PRED p)
     - greedy_max_independent_unstable(VEC v, PRED p)
     - greedy_max_independent(VEC v, PRED p)


TODOS:

0. Try to find out if erase_if_unstable is indeed faster.

1. Once we switch to C++23, use std::vector::append_range
   in function append_ranges.

2. Consolidate with
    - General/Algorithms.hpp
    - Structures/Sets/SetAlgorithms/BasicSetOperations.hpp
    - Random/Algorithms.hpp

*/


#ifndef ALGORITHMS_iWjSrlmVWS
#define ALGORITHMS_iWjSrlmVWS

#include <numeric>
#include <vector>

#include <cassert>

#include <Numerics/NumTypes.hpp>

namespace Algorithms {

  // For sorted ranges decide whether their intersection is empty:
  template <class RAN1, class RAN2>
  inline bool empty_intersection(const RAN1& r1, const RAN2& r2) noexcept {
    if (r1.empty() or r2.empty()) return true;
    const auto e1 = r1.end(); const auto e2 = r2.end();
    auto i1 = r1.begin(); auto i2 = r2.begin();
    if (r1.size() <= r2.size()) {
      for (;;) {
        while (*i1 < *i2) { ++i1; if (i1 == e1) return true; }
        if (*i1 == *i2) return false;
        do { ++i2; if (i2 == e2) return true; } while  (*i2 < *i1);
        if (*i2 == *i1) return false;
      }
    }
    else {
      for (;;) {
        while (*i2 < *i1) { ++i2; if (i2 == e2) return true; }
        if (*i2 == *i1) return false;
        do { ++i1; if (i1 == e1) return true; } while (*i1 < *i2);
        if (*i1 == *i2) return false;
      }
    }
    return true;
  }


  // The base-set is {0, ..., N-1}; r must be sorted (but otherwise is
  // arbitrary, except for that its elements must be convertible to UINT):
  template <class RANGE, typename UINT>
  std::vector<UINT> complement_uint(const RANGE& r, const UINT N) {
    if (N == 0) return {};
    assert(std::ranges::is_sorted(r));
    std::vector<UINT> res;
    UINT x = 0;
    const auto end = r.end();
    for (auto it = r.begin(); it != end; ++it, ++x) {
      const UINT y = UINT(*it);
      if (y >= N) break;
      assert(x <= y);
      if (x < y) {
        res.reserve(res.size() + (y-x));
        do res.push_back(x++); while (x != y);
      }
    }
    if (x < N) {
      res.reserve(res.size() + (N-x));
      do res.push_back(x++); while (x != N);
    }
    return res;
  }

  // Copying the elements of b to the output (in the given order),
  // skipping the elemnts of a (in the given order); for efficiency,
  // assuming that the elements of a occur in b (in that order).
  template <class RAN1, class RAN2>
  std::vector<typename RAN2::value_type>
    complement_subsequence(const RAN1& a, const RAN2& b) {
    const auto bsize = b.size();
    if (bsize == 0) return {};
    const auto asize = a.size();
    assert(asize <= bsize);
    const auto size = bsize - asize;
    std::vector<typename RAN2::value_type> res;
    res.reserve(size);
    auto itb = b.begin();
    {const auto aend = a.end();
     for (auto ita = a.begin(); ita != aend; ++ita, ++itb) {
       const auto& x = *ita;
       for (; x != *itb; ++itb) res.push_back(*itb);
     }
    }
    const auto bend = b.end();
    for (; itb != bend; ++itb) res.push_back(*itb);
    assert(res.size() == size);
    return res;
  }


  // Using implicit conversions:
  template <class RANGE1, class RANGE2>
  RANGE1 append_ranges(RANGE1 r1, const RANGE2& r2) {
    const auto size = r1.size() + r2.size();
    r1.reserve(size);
    for (const auto& x : r2) r1.push_back(x);
    assert(r1.size() == size);
    return r1;
  }
  template <class RANGE1, class RANGE2, class RANGE3>
  RANGE1 append_ranges(RANGE1 r1, const RANGE2& r2, const RANGE3& r3) {
    const auto size = r1.size() + r2.size() + r3.size();
    r1.reserve(size);
    for (const auto& x : r2) r1.push_back(x);
    for (const auto& x : r3) r1.push_back(x);
    assert(r1.size() == size);
    return r1;
  }


  // As std::erase_if, but not preserving the order;
  // if no element was erased, then the vector is unchanged:
  template <class T, class Alloc, class Pred>
  constexpr typename std::vector<T,Alloc>::size_type
  erase_if_unstable(std::vector<T,Alloc>& v, const Pred pred) noexcept {
    const auto end = v.end(); auto itb = end;
    for (auto ita = v.begin(); ita != itb; ++ita) {
      while (itb != ita and pred(*(itb-1))) --itb;
      if (itb == ita) break;
      while (ita != itb and not pred(*ita)) ++ita;
      if (ita == itb) break;
      *ita = std::move(*--itb);
      assert(ita != itb);
    }
    const auto r = std::distance(itb, end);
    v.erase(itb, end);
    return r;
  }


  template <class RANGE>
  FloatingPoint::UInt_t sum_sizes(const RANGE& r) noexcept {
    using ui = FloatingPoint::UInt_t;
    const auto op = [](const ui acc, const auto& x) noexcept -> ui {
      return acc + std::size(x);
    };
    return std::accumulate(r.begin(), r.end(), ui(0), op);
  }


  // p(i,j) stands for an edge-relation; see Graphs::is_independent for the
  // graph-version; here we ignore loops:
  template <class VEC, class PRED>
  bool is_independent(const VEC& v, const PRED p) noexcept {
    const auto size = v.size();
    if (size <= 1) return true;
    const auto end = v.end(), endm1 = end-1;
    for (auto i = v.begin(); i != endm1; ++i) {
      const auto& x = *i;
      for (auto j = i+1; j != end; ++j)
        if (p(x, *j)) return false;
    }
    return true;
  }

  // Taking the elements from v in reverse order, using unstable removal:
  template <class VEC, class PRED>
  VEC greedy_max_independent_unstable(VEC v, const PRED p) noexcept {
    VEC res;
    while (not v.empty()) {
      const auto e = v.back(); v.pop_back();
      res.push_back(e);
      erase_if_unstable(v, [&e,p](const auto& x){return p(e,x);});
    }
    assert(is_independent(res,p));
    return res;
  }
  // Now also taking in reverse order, but using stable removal:
  template <class VEC, class PRED>
  VEC greedy_max_independent(VEC v, const PRED p) noexcept {
    VEC res;
    while (not v.empty()) {
      const auto e = v.back(); v.pop_back();
      res.push_back(e);
      std::erase_if(v, [&e,p](const auto& x){return p(e,x);});
    }
    assert(is_independent(res,p));
    return res;
  }

}

#endif
