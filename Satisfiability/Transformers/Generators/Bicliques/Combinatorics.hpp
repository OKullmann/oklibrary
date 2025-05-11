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


TODOS:

1. Is AntiDiagonal::sum exact for all inputs?

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

  /*
    Anti-diagonal enumeration (unranking):
    Sorting the pairs (u,v), u,v in NN_0
     - first by sum s=u+v (the antidiagonal)
     - second by ascending u.
  */

  struct AntiDiagonal {
    const PaTy pt;
    using float80 = FloatingPoint::float80;

    // The antidiagonal of i (u+v for the corresponding pair):
    constexpr UInt_t sum(const UInt_t i) const noexcept {
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
      default : assert(false); return 0;
      }
    }
  };

  static_assert(AntiDiagonal{PaTy::full}.sum(0) == 0);
  static_assert(AntiDiagonal{PaTy::full}.sum(65535) == 361);
  static_assert(AntiDiagonal{PaTy::full}.sum(-1) == 6074000999UL);
  static_assert(AntiDiagonal{PaTy::fullneq}.sum(0) == 1);
  static_assert(AntiDiagonal{PaTy::fullneq}.sum(65535) == 362);
  static_assert(AntiDiagonal{PaTy::fullneq}.sum(-1) == 6074000999UL);
  static_assert(AntiDiagonal{PaTy::sorted}.sum(0) == 0);
  static_assert(AntiDiagonal{PaTy::sorted}.sum(65535) == 510);
  static_assert(AntiDiagonal{PaTy::sorted}.sum(-1) == 8589934590UL);
  static_assert(AntiDiagonal{PaTy::sortedneq}.sum(0) == 1);
  static_assert(AntiDiagonal{PaTy::sortedneq}.sum(65535) == 511);
  static_assert(AntiDiagonal{PaTy::sortedneq}.sum(-1) == 8589934591UL);
}

#endif

