// Oliver Kullmann, 14.10.2018 (Swansea)
/* Copyright 2018 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */



#ifndef HEURISTICS_qinU4hPuf3
#define HEURISTICS_qinU4hPuf3

#include <utility>
#include <array>
#include <random>
#include <limits>

#include <cassert>
#include <cmath>

#include "ChessBoard.hpp"
#include "NQueens.hpp"
#include "RandGen.hpp"

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
    inline static constexpr Weights weights{{0, 0, 4.85, 1, 0.354, 0.11, 0.0694}};
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

}

#endif
