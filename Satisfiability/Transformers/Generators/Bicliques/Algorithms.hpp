// Oliver Kullmann, 26.3.2023 (Swansea)
/* Copyright 2023 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  General algorithms (tools)

   - empty_intersection(RAN r1, RAN r2) (r1, r2 must be sorted)

   - complement_uint(RAN r, UINT N) -> vector<UINT> :
     returns the sorted vector of elements of {0, ..., N-1} not in r
     (assumes r to be sorted)

   - append_ranges(RAN1 r1, RAN2 r2) -> RAN1 (copies r1, and appends to it)
   - append_ranges(RAN1 r1, RAN2 r2, RAN3 r3) -> RAN1

   - erase_if_byswap(vec, pred) : possibly faster than std::erase_if due to
     not keeping the order of vec.


TODOS:

0. Try to find out if erase_if_byswap is indeed faster.
    - Perhaps remain to "erase_if_unstable".

1. Once we switch to C++23, use std::vector::append_range
   in function append_ranges.

2. Consolidate with
    - General/Algorithms.hpp
    - Structures/Sets/SetAlgorithms/BasicSetOperations.hpp
    - Random/Algorithms.hpp

*/


#ifndef ALGORITHMS_iWjSrlmVWS
#define ALGORITHMS_iWjSrlmVWS

#include <algorithm>

namespace Algorithms {

  // For sorted ranges decide whether their intersection is empty:
  template <class RAN>
  inline bool empty_intersection(const RAN& r1, const RAN& r2) noexcept {
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


  // As std::erase_if, but not preserving the order:
  template <class T, class Alloc, class Pred>
  constexpr typename std::vector<T,Alloc>::size_type erase_if_byswap
      (std::vector<T,Alloc>& v, const Pred pred) noexcept {
    const auto end = v.end();
    const auto it = std::partition(v.begin(), end, std::not_fn(pred));
    const auto r = std::distance(it, end);
    v.erase(it, end);
    return r;
  }

}

#endif
