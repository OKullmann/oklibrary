// Oliver Kullmann, 11.5.2025 (Swansea)
/* Copyright 2025 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  General combinatorial components (like enumeration/ranking/unranking)

  Related:
   - General/Combinatorics.hpp contains components for enumerating k-subsets
   - Combinatorics/Enumeration/plans/Subsets.hpp contains plans.

  Components:

   - typedef UInt_t = FloatingPoint::UInt_t;
   - typedef pair_t = Graphs::AdjVecUInt::edge_t

   - enum class PaTy (pair-types): full, fullneq, sorted, sortedneq.

   - Functor-class AntiDiagonal:
    - AntiDiagonal{PaTy}(UInt_t i) computes the pair corresponding to
      index i in the "anti-diagonal" enumeration of NN_0^2 (unranking)
    - AntiDiagonal{PaTy}(pair_t)
      computes the inverse (ranking).

   - Functor-class CoLexicographic:
    - CoLexicographic(PaTy pt)(UInt_t i) computes the pair corresponding to
      index i in the colexicographical enumeration of NN_0^2, for
      pt in {sorted, sortedneq}.


TODOS:

1. Is AntiDiagonal::sum exact for all inputs? Etc.

*/

#ifndef COMBINATORICS_h9dRdvvDx1
#define COMBINATORICS_h9dRdvvDx1

#include <utility>

#include <cassert>

#include <Numerics/NumTypes.hpp>
#include <Numerics/NumBasicFunctions.hpp>

namespace Combinatorics {

  using UInt_t = FloatingPoint::UInt_t;

  // Types of pairs {u,v} :
  enum class PaTy { full, // just u, v >= 0
                    fullneq, // u # v
                    sorted, // u <= v
                    sortedneq // u < v
                  };

  // Equal to Graphs::AdjVecUInt::edge_t :
  typedef std::pair<UInt_t, UInt_t> pair_t;


  /* *********************************************************
    Anti-diagonal enumeration (unranking):
    Sorting the pairs (u,v), u,v in NN_0
     - first by sum s=u+v (the antidiagonal)
     - second by ascending u.
  */

  struct AntiDiagonal {
    const PaTy pt;
    using float80 = FloatingPoint::float80;

    // The number of (consecutive) indices with sum s:
    constexpr UInt_t length(const UInt_t s) const noexcept {
      using enum PaTy;
      switch (pt) {
      case full : return s + 1;
      case fullneq : return s + 1 - (s % 2 == 0);
      case sorted : return s/2 + 1;
      case sortedneq : return s%2==0 ? s/2 : (s+1)/2; }
      assert(false); return 0;
    }

    // The antidiagonal of i (that is, u+v for the pair corresponding to i):
    constexpr UInt_t antid(const UInt_t i) const noexcept {
      using enum PaTy;
      switch (pt) {
      case full : return (FloatingPoint::sqrt(float80(i)*8 + 1) - 1) / 2;
        // https://oeis.org/A003056
      case fullneq : return FloatingPoint::sqrt(float80(i)*2 + 1);
        // https://oeis.org/A103128, but there starting with i=1
      case sorted : return FloatingPoint::sqrt(float80(i)*4 + 1) - 1;
        // https://oeis.org/A055086
      case sortedneq : return FloatingPoint::sqrt(float80(i)*4 + 1);
        // https://oeis.org/A000267
      }
      assert(false); return 0;
    }

    // The first i such that antid(i) >= n, first trivially computed: */
    constexpr UInt_t mintriv(const UInt_t n) const noexcept {
      UInt_t sum = 0;
      for (UInt_t s = 0; s < antid(n); ++s) sum += length(s);
      return sum;
    }
    /* Underlying summation-formulas for sum_{s=0}^m length(s) :
        - full      : (m+1) * (m+2) / 2
        - fullneq   : floor((m+1)^2/2)
        - sorted    : floor((m+2)^2/4)
        - sortedneq : ceil(m * (m+2) / 4).
       These formulas are applied with m = antid(n) - 1.
       The sums for indices < m:
    */
    constexpr UInt_t sum_m1(const UInt_t m) const noexcept {
      using enum PaTy;
      switch (pt) {
      case full : return m%2==0 ? (m/2) * (m+1) : ((m+1)/2) * m;
      case fullneq : return m%2==0 ? (m/2) * m : ((m-1)/2) * (m+1);
      case sorted : { const float80 h = float80(m) + 1;
          return (h/4) * h; }
      case sortedneq : { const float80 h = m;
          return FloatingPoint::ceil((h/4) * h - float80(1)/4); } }
      assert(false); return 0;
    }
    constexpr UInt_t min(const UInt_t n) const noexcept {
      return sum_m1(antid(n));
    }

    // Unranking:
    constexpr pair_t operator()(const UInt_t i) const noexcept {
      const UInt_t s = antid(i), m = min(i);
      if (pt != PaTy::fullneq or s%2==1 or i - m < (s+m) - i)
        return {i - m, (s+m) - i};
      else return {i - m + 1, (s+m) - i - 1};
    }
    // Ranking:
    constexpr UInt_t operator()(const pair_t& p) const noexcept {
      const UInt_t s = p.first + p.second;
      if (pt != PaTy::fullneq or s%2==1 or p.first < p.second)
        return sum_m1(s) + p.first;
      else
        return sum_m1(s) + p.first - 1;
    }
  };

  static_assert(AntiDiagonal{PaTy::full}.antid(0) == 0);
  static_assert(AntiDiagonal{PaTy::full}.antid(65535) == 361);
  static_assert(AntiDiagonal{PaTy::full}.antid(-1) == 6074000999UL);
  static_assert(AntiDiagonal{PaTy::fullneq}.antid(0) == 1);
  static_assert(AntiDiagonal{PaTy::fullneq}.antid(65535) == 362);
  static_assert(AntiDiagonal{PaTy::fullneq}.antid(-1) == 6074000999UL);
  static_assert(AntiDiagonal{PaTy::sorted}.antid(0) == 0);
  static_assert(AntiDiagonal{PaTy::sorted}.antid(65535) == 510);
  static_assert(AntiDiagonal{PaTy::sorted}.antid(-1) == 8589934590UL);
  static_assert(AntiDiagonal{PaTy::sortedneq}.antid(0) == 1);
  static_assert(AntiDiagonal{PaTy::sortedneq}.antid(65535) == 511);
  static_assert(AntiDiagonal{PaTy::sortedneq}.antid(-1) == 8589934591UL);

  static_assert(AntiDiagonal{PaTy::full}.min(-1) == 18446744070963499500UL);
  static_assert(AntiDiagonal{PaTy::fullneq}.min(-1) == 18446744067926499000UL);
  static_assert(AntiDiagonal{PaTy::sorted}.min(-1) == 18446744069414584320UL);
  static_assert(AntiDiagonal{PaTy::sortedneq}.min(-1) == 18446744069414584320UL);

  static_assert(AntiDiagonal{PaTy::full}(0) == pair_t{0,0});
  static_assert(AntiDiagonal{PaTy::full}(pair_t{0,0}) == 0);
  static_assert(AntiDiagonal{PaTy::full}(65535) == pair_t{194,167});
  static_assert(AntiDiagonal{PaTy::full}(pair_t{194,167}) == 65535);
  static_assert(AntiDiagonal{PaTy::full}(-1) == pair_t{2746052115UL,3327948884UL});
  static_assert(AntiDiagonal{PaTy::full}(pair_t{2746052115UL,3327948884UL}) == UInt_t(-1));
  static_assert(AntiDiagonal{PaTy::fullneq}(0) == pair_t{0,1});
  static_assert(AntiDiagonal{PaTy::fullneq}(pair_t{0,1}) == 0);
  static_assert(AntiDiagonal{PaTy::fullneq}(65535) == pair_t{13,349});
  static_assert(AntiDiagonal{PaTy::fullneq}(pair_t{13,349}) == 65535);
  static_assert(AntiDiagonal{PaTy::fullneq}(-1) == pair_t{5783052615UL,290948384UL});
  static_assert(AntiDiagonal{PaTy::fullneq}(pair_t{5783052615UL,290948384UL}) == UInt_t(-1));
  static_assert(AntiDiagonal{PaTy::sorted}(0) == pair_t{0,0});
  static_assert(AntiDiagonal{PaTy::sorted}(pair_t{0,0}) == 0);
  static_assert(AntiDiagonal{PaTy::sorted}(65535) == pair_t{255,255});
  static_assert(AntiDiagonal{PaTy::sorted}(pair_t{255,255}) == 65535);
  static_assert(AntiDiagonal{PaTy::sorted}(-1) == pair_t{4294967295UL,4294967295UL});
  static_assert(AntiDiagonal{PaTy::sorted}(pair_t{4294967295UL,4294967295UL}) == UInt_t(-1));
  static_assert(AntiDiagonal{PaTy::sortedneq}(0) == pair_t{0,1});
  static_assert(AntiDiagonal{PaTy::sortedneq}(pair_t{0,1}) == 0);
  static_assert(AntiDiagonal{PaTy::sortedneq}(65535) == pair_t{255,256});
  static_assert(AntiDiagonal{PaTy::sortedneq}(pair_t{255,256}) == 65535);
  static_assert(AntiDiagonal{PaTy::sortedneq}(-1) == pair_t{4294967295UL,4294967296UL});
  static_assert(AntiDiagonal{PaTy::sortedneq}(pair_t{4294967295UL,4294967296UL}) == UInt_t(-1));


  /* *********************************************************
    Co-lexicographical enumeration (unranking):
    Sorting the pairs (u,v), u,v in NN_0
     - first by v
     - second by u.

    Only for PaTy::sorted/sortedneq, since otherwise we haver order-type
    omega^2.
  */

  struct CoLexicographic {
    const PaTy pt;
    using float80 = FloatingPoint::float80;

    // The number of (consecutive) indices with second component v:
    constexpr UInt_t length(const UInt_t v) const noexcept {
      using enum PaTy;
      switch (pt) {
      case sorted : return v+1;
      case sortedneq : return v;
      case full : [[fallthrough]];
      case fullneq : ; }
      assert(false); return 0;
    }

    // The second component of i (that is, v for the pair corresponding to i):
    constexpr UInt_t secondc(const UInt_t i) const noexcept {
      using enum PaTy;
      switch (pt) {
      case sorted : return (FloatingPoint::sqrt(float80(i)*8 + 1) - 1) / 2;
        // https://oeis.org/A003056
      case sortedneq : return FloatingPoint::sqrt(float80(i)*2 + 2) + 0.5;
        // https://oeis.org/A002024, but there starting with i=1
      case full : [[fallthrough]];
      case fullneq : ; }
      assert(false); return 0;
    }

    // The first i such that secondc(i) >= n, first trivially computed: */
    constexpr UInt_t mintriv(const UInt_t n) const noexcept {
      UInt_t sum = 0;
      for (UInt_t s = 0; s < secondc(n); ++s) sum += length(s);
      return sum;
    }
    /* Underlying summation-formulas for sum_{s=0}^m length(s) :
        - sorted    : (m+1) * (m+2) / 2
        - sortedneq : m * (m+1) /2.
       These formulas are applied with m = secondc(n) - 1.
       The sums for indices < m:
    */
    constexpr UInt_t sum_m1(const UInt_t m) const noexcept {
      using enum PaTy;
      switch (pt) {
      case sorted : return m%2==0 ? (m/2) * (m+1) : ((m+1)/2) * m;
      case sortedneq : return m%2==0 ? (m/2) * (m-1) : ((m-1)/2) * m;
      case full : [[fallthrough]];
      case fullneq : ; }
      assert(false); return 0;
    }
    constexpr UInt_t min(const UInt_t n) const noexcept {
      return sum_m1(secondc(n));
    }

    // Unranking:
    constexpr pair_t operator()(const UInt_t i) const noexcept {
      const UInt_t v = secondc(i), m = min(i);
      return {i - m, v};
    }
    // Ranking:
    constexpr UInt_t operator()(const pair_t& p) const noexcept {
      const UInt_t s = p.second;
      return sum_m1(s) + p.first;
    }
  };

  static_assert(CoLexicographic{PaTy::sorted}.secondc(0) == 0);
  static_assert(CoLexicographic{PaTy::sorted}.secondc(65535) == 361);
  static_assert(CoLexicographic{PaTy::sorted}.secondc(-1) == 6074000999UL);
  static_assert(CoLexicographic{PaTy::sortedneq}.secondc(0) == 1);
  static_assert(CoLexicographic{PaTy::sortedneq}.secondc(65535) == 362);
  static_assert(CoLexicographic{PaTy::sortedneq}.secondc(-1) == 6074001000UL);

  static_assert(CoLexicographic{PaTy::sorted}.min(-1) == 18446744070963499500UL);
  static_assert(CoLexicographic{PaTy::sortedneq}.min(-1) == 18446744070963499500UL);

  static_assert(CoLexicographic{PaTy::sorted}(0) == pair_t{0,0});
  static_assert(CoLexicographic{PaTy::sorted}(pair_t{0,0}) == 0);
  static_assert(CoLexicographic{PaTy::sorted}(65535) == pair_t{194,361});
  static_assert(CoLexicographic{PaTy::sorted}(pair_t{194,361}) == 65535);
  static_assert(CoLexicographic{PaTy::sorted}(-1) == pair_t{2746052115UL,6074000999UL});
  static_assert(CoLexicographic{PaTy::sorted}(pair_t{2746052115UL,6074000999UL}) == UInt_t(-1));
  static_assert(CoLexicographic{PaTy::sortedneq}(0) == pair_t{0,1});
  static_assert(CoLexicographic{PaTy::sortedneq}(pair_t{0,1}) == 0);
  static_assert(CoLexicographic{PaTy::sortedneq}(65535) == pair_t{194,362});
  static_assert(CoLexicographic{PaTy::sortedneq}(pair_t{194,362}) == 65535);
  static_assert(CoLexicographic{PaTy::sortedneq}(-1) == pair_t{2746052115UL,6074001000UL});
  static_assert(CoLexicographic{PaTy::sortedneq}(pair_t{2746052115UL,6074001000UL}) == UInt_t(-1));
}

#endif

