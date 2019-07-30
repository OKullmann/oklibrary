// Oliver Kullmann, 14.10.2018 (Swansea)
/* Copyright 2018, 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*  Heuristics for branching variables

     - BasicBranching

     - TawHeuristics
     - AntiTaw
     - FirstOpen
     - FirstOpenRandom

     - rc_branching_t
     - BasicBranchingRC
     - MinLengthRC

*/

/*

TODOS:

1. The documentation of this file needs improvement.

2. Simple heuristics

   (a) Maximum o-degree.

3. Optimising heuristics

   - A heuristics generalising TawHeuristics, with free weights to be
     optimised.
   - That is, generalising TawHeuristics::heuristics, while keeping operator().
   - Some simple algorithms for optimising the weights need to be written:
     - Using given ranges for the weights, sweep through them in some order,
       and optimise each single weight, by running through the range with
       some given number of steps.
     - Starting this process at random points (within the ranges), and repeat
       often.
   - These optimisations need to be performed for different N.

4. Distance-based heuristics like the TawHeuristics need the option
   to use ltau instead of the product.
    - ctawSolver versus cttawSolver (using tau): The former uses
      fewer nodes (even exponentially fewer), this should be reproduced and
      examined.
    - That is, we use ltau (or ltau_d, after checking that there isn't
      a difference), and see whether we can reproduce the results.
    - The customisation-point is operator(), the setting of variable
      prod, which is the primary projection, plus variable sum, which
      is the secondary projection.
    - Seems best to have another template-argument Proj, providing functions
      p1(a,b) and p2(a,b) for the two projections, with default-value
      a class providing product and sum.
    - a, b would be here of type Weight_t; this might be a parameter of
      class Proj.
    - One could have Proj with primary ltau_d, secondary product.
    - Also primary product, secondary ltau_d.
    - Perhaps p1 and p2 take one argument, a "branching pair"? Seems
      reasonable.
    - Since first and second projection are independent, better to
      have two template-arguments Proj1<distance_t>, Proj2<distance_t>.

5. We need a better organisation of how to name and choose heuristics.

   (a) The command-line input is one aspect: here we need a syntax for
       heuristics and their parameters.

6. Implement a simple static heuristics, which orders the fields first by
   rows, then by columns, where the rows are chosen as central as possible,
   while the columns are simple chosen from left to right:
   - So the idea is to choose a most central row, which is unique for an
     odd number of rows, while for an even number of rows alternatingly the
     lower/upper middle row is chosen.
   - Within each row, the fields are simply chosen left to right (first open).
   - Examples for enumerating the rows 1, ..., N:
    - N = 1 : 1
    - N = 2 : 1 2
    - N = 3 : 2 1 3
    - N = 4 : 2 3 4 1
    - N = 5 : 3 2 4 5 1
    - N = 6 : 3 4 5 2 1 6.
   - The resulting order of all fields of the board e.g. for N=3:
     (2,1),(2,2),(2,3),(1,1),(1,2),(1,3),(3,1),(3,2),(3,3).
   - This heuristics uses FirstOpenRandom, with initialising the ordering
     of all fields directly.
   - A generalisation of FirstOpenRandom is needed, so that the various
     instantiations, like via the order ChessBoard::enum_square(N),
     become more explicit.
   - Likely best to have each heuristics as a separate type, providing the
     initialisation of the order via a policy-argument as template-parameter
     to the basic class-template "FixedFieldOrder" (which corresponds to
     the restriction of the class of branching trees to be "DP-like", that is,
     each path from the root to a leaf follows the same global order of
     variables).
   - Perhaps also the basic "geometric aspects" should be implemented
     in this file (Heuristics.hpp; moving then enum_square here).

*/

#ifndef HEURISTICS_qinU4hPuf3
#define HEURISTICS_qinU4hPuf3

#include <utility>
#include <array>
#include <random>
#include <limits>
#include <ostream>

#include <cassert>
#include <cmath>

#include <Random/Algorithms.hpp>

#include "ChessBoard.hpp"
#include "NQueens.hpp"

namespace Heuristics {

  // The prototype:
  class BasicBranching {
    using Var = ChessBoard::Var;
  public :
    const NQueens::BasicACLS& F;

    explicit BasicBranching(const NQueens::BasicACLS& F) noexcept : F(F) {}

    Var operator()() const noexcept { return {0,0}; }

  };


  /* Exactly the heuristics from tawSolver
     https://github.com/OKullmann/oklibrary/commits/master/Satisfiability/Solvers/TawSolver
     ID a227f64a6c66a817e4b53fa4c1a1244d530a25c5
  */

  template <class AmoAloInference = NQueens::AmoAlo_board>
  class TawHeuristics {
    typedef AmoAloInference AmoAlo_board;
    using Var = ChessBoard::Var;
    using Var_uint = ChessBoard::Var_uint;
    using State = ChessBoard::State;
  public :
    typedef double Weight_t;
    typedef std::pair<Weight_t, Weight_t> Bp;

    const ChessBoard::Board& B;

    explicit TawHeuristics(const AmoAlo_board& F) noexcept : B(F.board()) {}

    constexpr static Weight_t weight(const Var_uint cl) noexcept {
      return (cl < size) ? weights[cl] :
        std::pow(basis, -ChessBoard::Var_int((cl-size)+1)) * weights.back();
    }

    Bp heuristics(const Var v) const noexcept {
      return {B.odegree(v) * weight(2),
              weight(B.r_rank(v.first).o) + weight(B.c_rank(v.second).o)};
    }

    Var operator()() const noexcept {
      Weight_t max1 = 0, max2 = 0;
      Var bv{0,0};
      for (ChessBoard::coord_t i = 1; i <= B.N; ++i) {
        if (B.r_rank(i).p != 0) continue;
        const auto& R = B()[i];
        Var v; v.first = i;
        for (ChessBoard::coord_t j = 1; j <= B.N ; ++j) {
          if (R[j] != State::open) continue;
          v.second = j;
          const Bp h = heuristics(v);
          const Weight_t prod = h.first * h.second;
          if (prod < max1) continue;
          const Weight_t sum = h.first + h.second;
          if (prod > max1) max1 = prod;
          else if (sum <= max2) continue;
          max2 = sum;
          bv = v;
        }
      }
      return bv;
    }

  private :

    static constexpr Var_uint size{7};
    typedef std::array<Weight_t,size> Weights;
    static constexpr Weights weights{{0, 0, 4.85, 1, 0.354, 0.11, 0.0694}};
    static constexpr Weight_t basis = 1.46;

  };
  static_assert(TawHeuristics<>::weight(0) == 0);
  static_assert(TawHeuristics<>::weight(1) == 0);
  static_assert(TawHeuristics<>::weight(2) == 4.85);
  static_assert(TawHeuristics<>::weight(3) == 1);
  static_assert(TawHeuristics<>::weight(4) == 0.354);
  static_assert(TawHeuristics<>::weight(5) == 0.11);
  static_assert(TawHeuristics<>::weight(6) == 0.0694);
  static_assert(TawHeuristics<>::weight(7) == 0.0694 * std::pow(1.46,-1));
  static_assert(TawHeuristics<>::weight(8) == 0.0694 * std::pow(1.46,-2));


  template <class AmoAloInference = NQueens::AmoAlo_board>
  class AntiTaw : public TawHeuristics<AmoAloInference> {
    using Base = TawHeuristics<AmoAloInference>;
    typedef AmoAloInference AmoAlo_board;
    using Var = ChessBoard::Var;
    using State = ChessBoard::State;
  public :
    using Weight_t = typename Base::Weight_t;
    using Bp = typename Base::Bp;
    explicit AntiTaw(const AmoAlo_board& F) noexcept : Base(F) {}

    Var operator()() const noexcept {
      Weight_t min1 = std::numeric_limits<Weight_t>::infinity(), min2 = min1;
      Var bv{0,0};
      for (ChessBoard::coord_t i = 1; i <= Base::B.N; ++i) {
        if (Base::B.r_rank(i).p != 0) continue;
        const auto& R = Base::B()[i];
        Var v; v.first = i;
        for (ChessBoard::coord_t j = 1; j <= Base::B.N ; ++j) {
          if (R[j] != State::open) continue;
          v.second = j;
          const Bp h = Base::heuristics(v);
          const Weight_t prod = h.first * h.second;
          if (prod > min1) continue;
          const Weight_t sum = h.first + h.second;
          if (prod < min1) min1 = prod;
          else if (sum >= min2) continue;
          min2 = sum;
          bv = v;
        }
      }
      return bv;
    }
  };


  // Choosing the first open variable:
  class FirstOpen {
    using Var = ChessBoard::Var;
    using State = ChessBoard::State;
  public :
    const ChessBoard::Board& B;
    explicit FirstOpen(const NQueens::AmoAlo_board& F) noexcept : B(F.board()) {}
    Var operator()() const noexcept {
      for (ChessBoard::coord_t i = 1; i <= B.N; ++i) {
        if (B.r_rank(i).p != 0) continue;
        const auto& R = B()[i];
        for (ChessBoard::coord_t j = 1; j <= B.N ; ++j) {
          if (R[j] != State::open) continue;
          return {i,j};
        }
      }
      assert(false);
      return {};
    }
  };

  // Fixing a random order of fields, and choosing the first open variable:
  class FirstOpenRandom {
    using Var = ChessBoard::Var;
    using coord_t = ChessBoard::coord_t;
    using Var_uint = ChessBoard::Var_uint;
  public :
    const ChessBoard::Board& B;
    explicit FirstOpenRandom(const NQueens::AmoAlo_board& F) noexcept : B(F.board()) {}
    Var operator()() const noexcept {
      assert(Var_uint(B.N) * Var_uint(B.N) == random_permutation.size());
      for (const Var v : random_permutation) if (B.open(v)) return v;
      assert(false);
      return {};
    }
    using seed_t = RandGen::seed_t;
    using vec_seed_t = RandGen::vec_seed_t;
    static void init(const coord_t N, const vec_seed_t s = vec_seed_t{}) {
      random_permutation.resize(Var_uint(N) * Var_uint(N));
      {varvec_t::size_type index = 0;
       for (coord_t i = 1; i <= N; ++i)
         for (coord_t j = 1; j <= N; ++j)
           random_permutation[index++] = {i,j};
      }
      std::seed_seq seq(s.begin(), s.end());
      RandGen::shuffle(random_permutation.begin(), random_permutation.end(), RandGen::randgen_t(seq));
    }
    using varvec_t = ChessBoard::varvec_t;
    static const varvec_t& permutation() noexcept { return random_permutation; }
    static void init(varvec_t P) noexcept {
      random_permutation = P;
    }
  private :
    static varvec_t random_permutation;
  };
  FirstOpenRandom::varvec_t FirstOpenRandom::random_permutation;


  // The prototype for row-column-branching:
  typedef std::pair<ChessBoard::coord_t, bool> rc_branching_t;
  class BasicBranchingRC {
  public :
    const NQueens::BasicACLS& F;

    explicit BasicBranchingRC(const NQueens::BasicACLS& F) noexcept : F(F) {}

    rc_branching_t operator()() const noexcept { return {}; }

  };

  // Possible heuristics concerning lengths:
  enum class LRC { min=0, max=1, minrows=2 };
  constexpr int maxLRC = int(LRC::minrows);
  std::ostream& operator <<(std::ostream& out, const LRC h) {
    switch (h) {
      case LRC::min : return out << "minlength_rc";
      case LRC::max : return out << "maxlength_rc";
      case LRC::minrows : return out << "minlength_r";
      default : return out << "LRC_uncovered:" << int(h);
    }
  }

  inline constexpr ChessBoard::coord_t init_opt(const LRC o, const ChessBoard::coord_t N) noexcept {
    switch (o) {
    case LRC::minrows:
    case LRC::min : return N+1;
    case LRC::max : return 0;
    default : return 0;
    }
  }

  template <LRC option, class AmoAloInference = NQueens::AmoAlo_board>
  class ByLengthRC {
    typedef AmoAloInference AmoAlo_board;
  public :
    const ChessBoard::Board& B;
    using coord_t = ChessBoard::coord_t;

    explicit ByLengthRC(const AmoAlo_board& F) noexcept : B(F.board()) {}

    rc_branching_t operator()() const noexcept {
      bool row = true;
      coord_t index = 0;
      coord_t opt = init_opt(option, B.N);
      assert(option!=LRC::min or opt != 0);
      for (coord_t i = 1; i <= B.N; ++i) {
        const auto open = B.r_rank(i).o;
        if (open == 0) {
          assert(B.r_rank(i).p == 1);
          continue;
	}
        switch (option) {
        case LRC::minrows : ;
        case LRC::min :
          if (open < opt) {index = i; opt = open;}
          break;
        case LRC::max :
          if (open > opt) {index = i; opt = open;}
        }
      }
      if constexpr (option == LRC::minrows) {
        assert(row);
        assert(index != 0);
        return {index, row};
      }
      for (coord_t i = 1; i <= B.N; ++i) {
        const auto open = B.c_rank(i).o;
        if (open == 0) continue;
        switch (option) {
        case LRC::min :
          if (open < opt) {index = i; opt = open; row=false;}
          break;
        default :
          if (open > opt) {index = i; opt = open; row=false;}
        }
      }
      assert(index != 0);
      return {index, row};
    }

  };

}

#endif
