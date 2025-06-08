// Oliver Kullmann, 26.3.2023 (Swansea)
/* Copyright 2023, 2025 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  General algorithms (tools)

   Iterators and ranges:

   - uint_iterator_t<UINT> is a random access iterator wrapping an
     element of UINT
   - make_uint_iterator_range<UINT>(UINT a, UINT b) creates a range of
     elements from a to b (which for b < a "wrappes around" !).

     Could also be used for signed integral types, but here we would need to
     have a <= b to make sense and avoid overflow.

     Remark: For signed integral types, std::views::iota(a,b) works well,
     but not for unsigned integral types, and thus the iterator and range
     are needed (since the difference-type of the range produced by
     std::views::iota for unsigned a, b is, as for C++23, unsigned, and thus
     fails the test even for an input-iterator).

   - pointed_view(RAN r, size_t i) produces a view, which is the range r minus
     the element at index i (if in the range -- otherwise nothing is removed).

   - sum_sizes(RAN) -> FloatingPoint::UInt_t : summation of member.size()
   - sum_proj(RAN, PROJ) -> FloatingPoint::UInt_t (sum the projections)
   - prod_sizes(RAN) -> FloatingPoint::UInt_t : multiplication of member.size()
   - prod_proj(RAN, PROJ) -> FloatingPoint::UInt_t (multiply the projections)

   - is_strictly_ascending(RAN) -> bool

   - extract_beginend(RAN r) : produces a pair of vectors, containing
     x.begin() resp. x.end() for x in r.


   Set operations:

     for sorted ranges r1, r2:
   - empty_intersection(RAN1 r1, RAN2 r2) -> bool (uses < and ==)
   - intersection(RAN1 r1, RAN2 r2) -> std::vector<RAN1::value_type>
     (uses only <; convenience-wrapper for std::intersection)
   - split(RAN1 r0, RAN2 r1) -> std::array<std::vector<RAN1::value_type>, 3>
     splits the sorted sequence r1, r2 into res[0,1,2], with res[2] the
     intersection (uses < and ==)

   - forward_strictsubsumption_by_erase(vec& r) removes elements r[j] such that
     there is i < j with r[i] a strict subset of r[j] (all r[k] are sorted ranges); uses
   - strict_subsumption_test_sized(RAN r1, RAN r2) -> bool for checking whether
     r1 is a (strict) subset of r2 (assuming RAN has standard size-function)

   - complement_uint(RAN r, UINT N) -> vector<UINT> :
     returns the sorted vector of elements of {0, ..., N-1} not in r
     (assumes r to be sorted)
   - complement_subsequence(RAN1 a, RAN2 b) -> vector<RAN2::value_type>
     assumes a is a subsequence of b, and returns "b - a"


   Sequence operations:

   - allnotequal(VEC v1, VEC v2) -> bool
     (false iff there is i with v1[i] == v2[i])
   - allsamesize(VEC v1, VEC v2) -> bool
     (false iff there is i with v1[i].size() != v2[i].size())

   - generate_vector(RAN r, FUN f) -> vector of f(x) for x : r

   - rmerge(RAN r) -> vector of elements of the elements of r; for the elements of
     r are sorted, so is the merge (unstable, that is, the order of equivalent elements
     is unspecified)

   - append_ranges(RAN1 r1, RAN2 r2) -> RAN1 (copies r1, and appends r2
     elementwise to it, using conversions if applicable)
   - append_ranges(RAN1 r1, RAN2 r2, RAN3 r3) -> RAN1
   - append2d_ranges(RAN1 r1, RAN2 r2) -> vector<vector<value_type>>
     (considered r1, r2 as ragged matrices, and appends their rows; as above,
     value_type is from RAN1)

   - erase_if_unstable(vec&, pred) : faster than std::erase_if due to
     not keeping the order of vec (just moving elements from the end
     to the erased slots)

   - erase_indices(vec&, range-of-indices) (removing the given indices)

   - erase_krandom(vec&, k, RandGen_t&) (removing k elements at random)
   - erase_prandom(vec&, Prob64 p, RandGen_t&) (removing with probability p)
   - keep_krandom(vec&, k, RandGen_t&) (now keeping k elements at random)
   - keep_prandom(vec&, Prob64 p, RandGen_t&) (keeping with probability p)


   Finding equal elements:

    - nt_eqel_bysort(VEC v) -> std::vector<std::vector<FloatingPoint::UInt_t>>
      computes the nontrivial equivalence-classes regarding == on v (via
      indices), by stably sorting the indices of v w.r.t. == on the
      corresponding elements of v (w.r.t. <), and then extracting subsequent
      equal intervals;
      thus the result-elements are sorted vectors of indices, and this
      collection of vectors is lexicographically sorted w.r.t. v.
    - nt_eqel_bydef(VEC v) compares the elements directly (thus using
      only ==), and so the collection of vectors (the equivalence-classes)
      is sorted lexicographically w.r.t. the indices themselves.

      Both functions use only the indices of VEC v.

    - nt_eqel_byhash(VEC v, HASH h, COMP c) in a sense combines the above
      two approaches; for a good hash-function h we only compare
      the equal elements (and only with the first of them).
      So if a good h can be provided, this should be the most efficient
      approach (the default for h is the identity, which means that just
      the inverse of the map v (from indices to values) is computed, via
      a std::map). COMP c is == by default.
      The resulting order is the same as of nt_eqel_bydef.

      Above, for VEC just size and [] is required, where the arguments of
      [] (the indices) are FloatingPoint::UInt_t.

   Graph algorithms:

   - finding greedily a maximal independent set in a graph, given by
     a predicate (using the order of v):
     - is_independent(VEC v, PRED p)
     - orderly_max_independent_unstable(VEC v, PRED p)
     - orderly_max_independent(VEC v, PRED p)


TODOS:

-1. empty_intersection is slow for larger ranges
    - The ladder-approach should be basically similar to the function
      split.
    - However there we need to run through all elements, since all elements
      are treated.
    - But for the intersection we can (should) use std::lower_bound to jump
      from ladder spoke to ladder spoke.
    - One should see whether std::set_intersection uses std::lower_bound
      (or something similar to binary search).
    - Same for std::set_difference.
      Indeed the std-algorithms assume only LegacyInputIterator,
      and thus binary search is not performed (since no random access)!
    - So we should provide refined versions, assuming random access
      and using binary search.

0. Try to find out if erase_if_unstable is indeed faster.

1. Once we switch to C++23, use std::vector::append_range
   in function append_ranges.

2. Consolidate with
    - General/Algorithms.hpp
    - Structures/Sets/SetAlgorithms/BasicSetOperations.hpp
    - Random/Algorithms.hpp

   Especially forward_strictsubsumption_by_erase is a variation on the trivial
   algorithm as in Structures/Sets/SetAlgorithms/Subsumption.hpp.

*/


#ifndef ALGORITHMS_iWjSrlmVWS
#define ALGORITHMS_iWjSrlmVWS

#include <numeric>
#include <vector>
#include <algorithm>
#include <iterator>
#include <array>
#include <map>
#include <set>
#include <functional>
#include <type_traits>
#include <ranges>
#include <queue>

#include <cassert>
#include <cstddef>

#include <Numerics/NumTypes.hpp>
#include <Transformers/Generators/Random/Algorithms.hpp>

namespace Algorithms {

  template <typename UINT>
  struct uint_iterator_t {
    UINT current;

    using iterator_category = std::random_access_iterator_tag;
    using value_type = UINT;
    using difference_type = std::ptrdiff_t;
    using pointer = const UINT*;
    using reference = UINT;

    constexpr uint_iterator_t() noexcept : current(0) {}
    explicit constexpr uint_iterator_t(const UINT x) noexcept : current(x) {}
    constexpr uint_iterator_t(const uint_iterator_t& other) noexcept = default;

    uint_iterator_t& operator ++() noexcept { ++current; return *this; }
    uint_iterator_t& operator --() noexcept { --current; return *this; }
    uint_iterator_t operator ++(int) noexcept {
      const auto t = *this; ++current; return t;
    }
    uint_iterator_t operator --(int) noexcept {
      const auto t = *this; --current; return t;
    }
    uint_iterator_t& operator +=(const difference_type d) noexcept {
      current += d; return *this;
    }
    uint_iterator_t& operator -=(const difference_type d) noexcept {
      current -= d; return *this;
    }
    reference operator [](const difference_type d) const noexcept {
      return current + d;
    }
    reference operator *() const noexcept { return current; }
    bool operator ==(const uint_iterator_t&) const noexcept = default;
    auto operator <=>(const uint_iterator_t&) const noexcept = default;

    friend uint_iterator_t operator +(uint_iterator_t it,
                                      const difference_type d) noexcept {
      return it += d;
    }
    friend uint_iterator_t operator +(const difference_type d,
                                      uint_iterator_t it) noexcept {
      return it += d;
    }
    friend uint_iterator_t operator -(uint_iterator_t it,
                                      const difference_type d) noexcept {
      return it -= d;
    }
    friend difference_type operator -(const uint_iterator_t& lhs,
                                      const uint_iterator_t& rhs) noexcept {
      return lhs.current - rhs.current;
    }
  };
  template <typename UINT>
  using uint_range_t = std::ranges::subrange<uint_iterator_t<UINT>>;
  template <typename UINT>
  uint_range_t<UINT> make_uint_iterator_range(const UINT a,
                                              const UINT b) noexcept {
    // a <= b is not necessary -- unsigned types can wrap around
    return std::ranges::subrange(uint_iterator_t(a), uint_iterator_t(b));
  }


  // Creates a view of range r without the element at indiex ;
  template <std::ranges::random_access_range RAN>
    requires std::ranges::sized_range<RAN>
  constexpr auto pointed_view(const RAN& r,
                              const typename RAN::size_type i) noexcept {
    using size_t = typename RAN::size_type;
    return std::views::iota(size_t{0}, r.size()) |
      std::ranges::views::filter([i, size = r.size()](const size_t j) noexcept {
                 return j >= size or j != i; }) |
      std::ranges::views::transform([&r](const size_t j) noexcept { return r[j]; });
  }


  template <class RAN>
  std::pair<std::vector<typename RAN::value_type::const_iterator>,
            std::vector<typename RAN::value_type::const_iterator>>
  extract_beginend(const RAN& r) {
    typedef typename RAN::value_type::const_iterator iterator;
    typedef std::vector<iterator> vec_t;
    std::pair<vec_t, vec_t> res;
    const auto size = r.size();
    res.first.reserve(size); res.second.reserve(size);
    for (const auto& x : r) {
      res.first.push_back(x.begin());
      res.second.push_back(x.end());
    }
    return res;
  }


  template <class RANGE>
  constexpr FloatingPoint::UInt_t sum_sizes(const RANGE& r) noexcept {
    using ui = FloatingPoint::UInt_t;
    using std::size;
    constexpr auto op = [](const ui acc, const auto& x) noexcept -> ui {
      return acc + size(x);
    };
    using std::begin; using std::end;
    return std::accumulate(begin(r), end(r), ui(0), op);
  }
  // More generally, applying projection proj:
  template <class RANGE, class PROJ>
  constexpr FloatingPoint::UInt_t sum_proj(const RANGE& r, const PROJ& proj) noexcept {
    using ui = FloatingPoint::UInt_t;
    const auto op = [&proj](const ui acc, const auto& x) noexcept -> ui {
      return acc + proj(x);
    };
    using std::begin; using std::end;
    return std::accumulate(begin(r), end(r), ui(0), op);
  }
  template <class RANGE>
  constexpr FloatingPoint::UInt_t prod_sizes(const RANGE& r) noexcept {
    using ui = FloatingPoint::UInt_t;
    using std::size;
    constexpr auto op = [](const ui acc, const auto& x) noexcept -> ui {
      return acc * size(x);
    };
    using std::begin; using std::end;
    return std::accumulate(begin(r), end(r), ui(1), op);
  }
  template <class RANGE, class PROJ>
  constexpr FloatingPoint::UInt_t prod_proj(const RANGE& r, const PROJ& proj) noexcept {
    using ui = FloatingPoint::UInt_t;
    const auto op = [&proj](const ui acc, const auto& x) noexcept -> ui {
      return acc * proj(x);
    };
    using std::begin; using std::end;
    return std::accumulate(begin(r), end(r), ui(1), op);
  }


  template <class RAN>
  bool is_strictly_ascending(const RAN& r) noexcept {
    const auto end = r.end();
    auto prev = r.begin();
    if (prev == end) return true;
    for (auto next = prev; ++next != end; prev=next)
      if (not (*prev < *next)) return false;
    return true;
  }


  // Merge a range of ranges into a vector; if the element-ranges are ascendingly
  // sorted, so is the resulting range (order of equivalent element is not stable):
  template <class RAN>
  std::vector<typename RAN::value_type::value_type> rmerge(const RAN& r) {
    typedef typename RAN::value_type::value_type value_type;
    const auto size = sum_sizes(r);
    std::vector<value_type> res; res.reserve(size);
    typedef typename RAN::const_iterator iterator1;
    typedef typename RAN::value_type::const_iterator iterator2;
    typedef std::pair<iterator1, iterator2> queue_element_t;
    struct comp {
      bool operator()(const queue_element_t& p1,
                      queue_element_t& p2) const noexcept {
        return *p2.second < *p1.second; }
    };
    std::priority_queue<queue_element_t, std::vector<queue_element_t>,comp> P;
    for (iterator1 it = r.begin(); it != r.end(); ++it)
      if (not it->empty()) P.push({it, it->begin()});
    while (not P.empty()) {
      auto [it1, it2] = P.top(); P.pop();
      res.push_back(*it2); ++it2;
      if (it2 != it1->end()) P.push({it1, it2});
    }
    assert(res.size() == size);
    return res;
  }


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
  // Compute the intersection as a std::vector, using value_type from first
  // range; uses std::set_intersection, and thus for multiple elements
  // occurring in r1 or r2, uses the minimum-count:
  template <class RAN1, class RAN2>
  std::vector<typename RAN1::value_type>
  intersection(const RAN1& r1, const RAN2& r2) {
    assert(std::ranges::is_sorted(r1) and std::ranges::is_sorted(r2));
    std::vector<typename RAN1::value_type> res;
    std::ranges::set_intersection(r1, r2, std::back_inserter(res));
    return res;
  }
  // Splits the sorted sequences into their unique parts and their
  // intersection; repeated elements are treated individually (the mininum
  // of the occurrence-number is used for the intersection, the overhang
  // goes to the respective unique part; so
  // r0.size()+r1.size() == res[0].size()+res[1].size()+2*res[2].size();
  // and so res[2] == intersection(r0,r1)):
  template <class RAN0, class RAN1>
  std::array<std::vector<typename RAN0::value_type>, 3>
  split(const RAN0& r0, const RAN1& r1) {
    assert(std::ranges::is_sorted(r0) and std::ranges::is_sorted(r1));
    typedef typename RAN0::value_type val_t;
    std::array<std::vector<val_t>, 3> res;
    auto i0 = r0.begin(); auto i1 = r1.begin();
    const auto e0 = r0.end(); const auto e1 = r1.end();
    for (;;) {
      if (i1 == e1) {
        while (i0 != e0) res[0].push_back(*i0++);
        return res;
      }
      skipped :
      if (i0 == e0) {
        while (i1 != e1) res[1].push_back(*i1++);
        return res;
      }
      const auto v1 = *i1;
      while (*i0 < v1) {
        res[0].push_back(*i0++);
        if (i0 == e0) {
          while (i1 != e1) res[1].push_back(*i1++);
          return res;
        }
      }
      const auto v0 = *i0;
      if (v0 == v1) {res[2].push_back(v1); ++i0; ++i1;}
      else {
        res[1].push_back(v1); ++i1;
        if (i1 == e1) {
          while (i0 != e0) res[0].push_back(*i0++);
          return res;
        }
        while (*i1 < v0) {
          res[1].push_back(*i1++);
          if (i1 == e1) {
            while (i0 != e0) res[0].push_back(*i0++);
            return res;
          }
        }
        if (*i1 == v0) {res[2].push_back(v0); ++i0; ++i1;}
        else {res[0].push_back(v0); ++i0; goto skipped;}
      }
    }
  }


  // Assuming T is a container-form with normal size-function, and the elements
  // themselves are sorted:
  template <class RAN>
  bool strict_subsumption_test_sized(const RAN& r1, const RAN& r2) noexcept {
    if (r1.size() >= r2.size()) return false;
    assert(std::ranges::is_sorted(r1)); assert(std::ranges::is_sorted(r2));
    return std::ranges::includes(r2, r1);
  }
  // Eliminates all strict-forward-subsumptions (i.e., strict-subset-relations):
  template <class T, class Alloc>
  void forward_strictsubsumption_by_erase(std::vector<T,Alloc>& v) noexcept {
    typedef typename std::vector<T,Alloc>::size_type size_t;
    size_t size = v.size();
    if (size <= 1) return;
    typedef typename std::vector<T,Alloc>::value_type seq_t;
    for (size_t i = 0; i < size-1; ++i) {
      const seq_t& e1 = v[i];
      const auto pred = [&e1](const seq_t& e2)noexcept{
        return strict_subsumption_test_sized(e1, e2);};
      const auto end = v.end();
      const auto nend = std::remove_if(v.begin()+(i+1), end, pred);
      v.erase(nend, end);
      if ((size = v.size()) <= 1) return;
    }
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
  // We have
  // complement_uint(r, N) == erase_indices(make_uint_iterator_range(UINT(0),N), r)
  // where here we must assume that for all elements x of r holds x < N, and
  // that there is no repetition in r.

  // Copying the elements of b to the output (in the given order),
  // skipping the elements of a (in the given order); for efficiency,
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


  template <class VEC>
  bool allnotequal(const VEC& v1, const VEC& v2) noexcept {
    const auto size = std::min(v1.size(), v2.size());
    for (typename VEC::size_type i = 0; i < size; ++i)
      if (v1[i] == v2[i]) return false;
    return true;
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


  template <class RAN, class FUN>
  std::vector<typename std::remove_reference_t<
    std::invoke_result_t<FUN, typename RAN::value_type>>>
  generate_vector(const RAN& r, const FUN& f) {
    std::vector<
      typename std::remove_reference_t<
      std::invoke_result_t<FUN, typename RAN::value_type>>>
      res;
    res.reserve(r.size());
    std::ranges::transform(r, std::back_inserter(res), f);
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

  template <class RANGE1, class RANGE2>
  std::vector<std::vector<typename RANGE1::value_type::value_type>>
  append2d_ranges(const RANGE1& r1, const RANGE2& r2) {
    using size_type = typename RANGE1::size_type;
    const size_type size = std::min(r1.size(), r2.size());
    using value_type = typename RANGE1::value_type::value_type;
    std::vector<std::vector<value_type>> res(size);
    for (size_type i = 0; i < size; ++i) {
      const auto& row1 = r1[i]; const auto& row2 = r2[i];
      std::vector<value_type> row(row1.begin(), row1.end());
      row.reserve(row1.size() + row2.size());
      for (const auto& x : row2) row.push_back(x);
      res[i] = std::move(row);
    }
    return res;
  }


  // As std::erase_if, but not preserving the order (not shifting, but just swapping
  // elements from the back into the open positions);
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

  // Now keeping the order of the elements of v, thus shifting
  template <class T, class Alloc, class RAN>
  void erase_indices(std::vector<T,Alloc>& v, const RAN& I) noexcept {
    assert(is_strictly_ascending(I));
    typedef typename std::vector<T,Alloc>::size_type size_t;
    const size_t size = v.size();
    if (size == 0) return;
    typedef typename RAN::const_iterator iterator;
    const iterator begin = I.begin(), end = I.end();
    if (begin == end) return;
    /* Assuming I = i_0, ..., i_m, adding i_{-1} := -1, i_{m+1} := size :
       Move element v[i] with i_{j-1} < i < i_j exactly j steps to the left. */
    size_t current = *begin + 1, dist = 1; assert(current <= size);
    for (iterator to_delete = ++iterator(begin); to_delete != end; ++to_delete, ++dist) {
      const size_t next = *to_delete; assert(next < size);
      for (; current != next; ++current)
        v[current - dist] = std::move(v[current]);
      current = next + 1;
    }
    for (; current != size; ++current)
      v[current - dist] = std::move(v[current]);
    v.erase(v.end() - dist, v.end());
  }

  // Erases k random elements:
  template <class T, class Alloc>
  void erase_krandom(std::vector<T,Alloc>& v,
                     const typename std::vector<T,Alloc>::size_type k,
                     RandGen::RandGen_t& g) {
    if (const auto size = v.size(); k >= size)
      v.clear();
    else
      erase_indices(v, RandGen::choose_kn(k,size,g,true));
  }
  // Keeps k random elements:
  template <class T, class Alloc>
  void keep_krandom(std::vector<T,Alloc>& v,
                    const typename std::vector<T,Alloc>::size_type k,
                    RandGen::RandGen_t& g) {
    if (const auto size = v.size(); k >= size)
      return;
    else if (k == 0)
      v.clear();
    else
      erase_krandom(v, size - k, g);
  }
  // Erases elements with probability p:
  template <class T, class Alloc>
  void erase_prandom(std::vector<T,Alloc>& v,
                     const RandGen::Prob64 p,
                     RandGen::RandGen_t& g) {
    RandGen::Bernoulli B(g,p);
    const auto pred = [&B](const auto&){return B();};
    std::erase_if(v, pred);
  }
  // Keeps elements with probability p:
  template <class T, class Alloc>
  void keep_prandom(std::vector<T,Alloc>& v,
                     const RandGen::Prob64 p,
                     RandGen::RandGen_t& g) {
    erase_prandom(v, comp(p), g);
  }

  // Using only ==:
  typedef std::vector<std::vector<FloatingPoint::UInt_t>> vec_equivs_t;
  template <class VEC>
  vec_equivs_t nt_eqel_bydef(const VEC& v) {
    using UInt_t = FloatingPoint::UInt_t;
    const UInt_t size = v.size();
    if (size <= 1) return {};
    vec_equivs_t res;
    std::vector<bool> eliminated(size);
    for (UInt_t i = 0; i < size-1; ++i) {
      if (eliminated[i]) continue;
      const auto& x = v[i];
      UInt_t j = i+1;
      while (not (v[j] == x)) if (++j == size) break;
      if (j != size) {
        std::vector<UInt_t> eqcl;
        eqcl.push_back(i);
        eqcl.push_back(j); eliminated[j] = true;
        while (++j < size)
          if (not eliminated[j] and v[j] == x) {
            eqcl.push_back(j); eliminated[j] = true;
          }
        res.push_back(std::move(eqcl));
      }
    }
    return res;
  }
  template <class RAN>
  struct trivial_hash {
    typedef typename RAN::value_type return_type;
    return_type operator()(const return_type& x) const {return x;}
  };
  // More generally, using comparison by COMP, and relying on a
  // "good" hash-function (using the default just means the simple
  // algorithm using a map from the elements to their indices, which needs
  // < on the value_type of VEC):
  template <class VEC,
            class HASH = trivial_hash<VEC>,
            class COMP= std::equal_to<typename VEC::value_type>>
  vec_equivs_t nt_eqel_byhash(const VEC& v,
                              const HASH& hash = HASH(),
                              const COMP& comp = COMP()) {
    using UInt_t = FloatingPoint::UInt_t;
    const UInt_t size = v.size();
    if (size <= 1) return {};
    typedef std::map<typename HASH::return_type, std::set<UInt_t>> map_t;
    map_t M;
    for (UInt_t i = 0; i < size; ++i) M[hash(v[i])].insert(i);
    vec_equivs_t res;
    for (auto [h,S] : M) {
       if (S.size() == 1) continue;
       const auto end = S.end();
       do {
         assert(not S.empty());
         auto it = S.begin();
         const UInt_t i = *it;
         const auto& x = v[i];
         it = S.erase(it);
         assert(it != end);
         while (not comp(v[*it], x)) if (++it == end) break;
         if (it != end) {
           std::vector<UInt_t> eqcl;
           eqcl.push_back(i); eqcl.push_back(*it);
           it = S.erase(it);
           while (it != end) {
             if (comp(v[*it], x)) {
               eqcl.push_back(*it);
               it = S.erase(it);
             }
             else ++it;
           }
           res.push_back(std::move(eqcl));
         }
       } while (not S.empty());
    }
    std::ranges::sort(res);
    return res;
  }
  // Using == and <:
  template <class VEC>
  vec_equivs_t nt_eqel_bysort(const VEC& v) {
    using UInt_t = FloatingPoint::UInt_t;
    const UInt_t size = v.size();
    if (size <= 1) return {};
    std::vector<UInt_t> indices; indices.reserve(size);
    for (UInt_t i = 0; i < size; ++i) indices.push_back(i);
    std::ranges::stable_sort(indices, [&v](UInt_t i, UInt_t j)noexcept{
                               return v[i] < v[j];});
    vec_equivs_t res;
    for (UInt_t i = 0; i < size-1;) {
      const UInt_t indi = indices[i];
      const auto& x = v[indi];
      UInt_t j = i+1;
      const UInt_t indj = indices[j];
      if (v[indj] == x) {
        std::vector<UInt_t> eqcl;
        eqcl.push_back(indi); eqcl.push_back(indj);
        while (++j < size) {
          const UInt_t indj = indices[j];
          if (v[indj] == x) eqcl.push_back(indj);
          else break;
        }
        res.push_back(std::move(eqcl));
      }
      i = j;
    }
    return res;
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
  VEC orderly_max_independent_unstable(VEC v, const PRED p) noexcept {
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
  VEC orderly_max_independent(VEC v, const PRED p) noexcept {
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
