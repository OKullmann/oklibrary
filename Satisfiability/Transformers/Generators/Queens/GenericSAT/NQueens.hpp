// Oliver Kullmann, 6.7.2018 (Swansea)
/* Copyright 2018 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */


/* TODOS

1. Good statistics on the inference process

2. Simple heuristics

   (a) First open field (this should still have a better q(N) than
       Somers algorithm).
   (b) Random choice (from all the remaining open fields).
   (c) Maximum o-degree.

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

4. Strengthened inference

   (a) Using that a solution has an even number of white fields.
   (b) Using that a solution occupies exactly N diagonals and exactly N
       antidiagonals.

*/

#ifndef NQUEENS_4WuKbxqFgV
#define NQUEENS_4WuKbxqFgV

#include <stack>
#include <vector>
#include <algorithm>

#include <cassert>

#include "ChessBoard.hpp"

namespace NQueens {

  // The prototype:
  struct BasicACLS {

    using coord_t = ChessBoard::coord_t;
    using Var = ChessBoard::Var;

    const coord_t N;
    const ChessBoard::Board B{N};

    explicit BasicACLS(const coord_t N) : N(N) {}

    // We declare the problem to be satisfied in order to run it without backtracking:
    bool satisfied() const noexcept { return true; }
    bool falsified() const noexcept { return false; }

    // The total number of variables:
    ChessBoard::Var_uint n() const noexcept { return N*N; }
    // Number of variables set to true or false:
    ChessBoard::Var_uint nset() const noexcept { return 0; }

    // Occupy or forbid field v:
    void set(const Var, const bool) noexcept {}

    const ChessBoard::Board& board() const noexcept { return B; }

  };

  // A concrete instance of BasicACLS:
  class AmoAlo_board {
    using Var_uint = ChessBoard::Var_uint;
    using Var_int = ChessBoard::Var_int;
    using Diagonal = ChessBoard::Diagonal;
    using AntiDiagonal = ChessBoard::AntiDiagonal;
    using Rank = ChessBoard::Rank;
    using TotalRank = ChessBoard::TotalRank;
    using State = ChessBoard::State;
  public :
    using coord_t = ChessBoard::coord_t;
    using Var = ChessBoard::Var;
    const coord_t N;

    explicit AmoAlo_board(const coord_t N) : N(N), B(N) {
      assert(N <= ChessBoard::max_coord);
      if (N == 1) set_true({1,1});
    }

    bool satisfied() const noexcept { return B.t_rank().p == N; }
    bool falsified() const noexcept { return falsified_; }

    ChessBoard::Var_uint n() const noexcept { return N*N; }
    ChessBoard::Var_uint nset() const noexcept {
      return B.t_rank().p + B.t_rank().f;
    }

    void set(const Var v, const bool val) noexcept {
      assert(v.first >= 1 and v.second >= 1);
      assert(v.first <= N and v.second <= N);
      assert(B(v) == State::open);
      assert(B.r_rank(v.first).o >= 2);
      assert(B.c_rank(v.second).o >= 2);
      if (val) set_true(v); else set_false(v);
      while (not falsified_ and not stack.empty()) {
        const Var w = stack.top(); stack.pop();
        switch (B(w)) {
        case State::forbidden : falsified_ = true; return;
        case State::open : set_true(w);
        default:;
        }
      }
    }

    const ChessBoard::Board& board() const noexcept { return B; }

  private:

    ChessBoard::Board B;

    enum class Line {r,c,d,ad,none};
    // f/o-ranks of forbidden v are updated for non-occupied lines (given by
    // parameter exclude), with alo-falsification and alo-propagation detected:
    void forbidden_forank_update(const Var v, const Line exclude) noexcept {
      assert(v.first >= 1 and v.second >= 1);
      assert(v.first <= N and v.second <= N);
      assert(B(v) == State::forbidden);
      if (exclude != Line::r) {
        auto& rank = B.r_rank(v.first);
        assert(rank.p == 0);
        --rank.o; ++rank.f;
        if (exclude != Line::none and rank.o == 0) {
          falsified_ = true; return;
        }
        else if (rank.o == 1) {
          const auto& R = B()[v.first];
          for (coord_t j = 1; j <= N ; ++j)
            if (R[j] == State::open) {stack.push({v.first, j}); break;}
        }
      }
      if (exclude != Line::c) {
        auto& rank = B.c_rank(v.second);
        assert(rank.p == 0);
        --rank.o; ++rank.f;
        if (exclude != Line::none and rank.o == 0) {
          falsified_ = true; return;
        }
        else if (rank.o == 1)
          for (coord_t i = 1; i <= N ; ++i)
            if (B.open({i,v.second})) {stack.push({i,v.second}); break;}
      }
      if (exclude != Line::d) {
        const Diagonal d = B.diagonal(v);
        assert(d.i < B.d_rank().size());
        auto& rank = B.d_rank(d.i);
        --rank.o; ++rank.f;
      }
      if (exclude != Line::ad) {
        const AntiDiagonal ad = B.anti_diagonal(v);
        assert(ad.i < B.ad_rank().size());
        auto& rank = B.ad_rank(ad.i);
        --rank.o; ++rank.f;
      }
    }

    // The following four propagation-functions assume that cur_v is placed,
    // and propagate amo to its row, column, diagonal and antidiagonal.
    void r_propagate(const Var cur_v) noexcept {
      assert(cur_v.first >= 1 and cur_v.second >= 1);
      assert(cur_v.first <= N and cur_v.second <= N);
      assert(B(cur_v) == State::placed);
      auto& R = B()[cur_v.first];
      assert(B.r_rank(cur_v.first).p == 1);
      auto& ro = B.r_rank(cur_v.first).o;
      for (coord_t j = 1 ; ro != 0 and j <= N ; ++j) {
        if (R[j] == State::open) {
          R[j] = State::forbidden; --ro;
          forbidden_forank_update({cur_v.first, j}, Line::r);
          if (falsified_) return;
        }
      }
    }
    void c_propagate(const Var cur_v) noexcept {
      assert(cur_v.first >= 1 and cur_v.second >= 1);
      assert(cur_v.first <= N and cur_v.second <= N);
      assert(B(cur_v) == State::placed);
      assert(B.c_rank(cur_v.second).p == 1);
      auto& ro = B.c_rank(cur_v.second).o;
      for (coord_t i = 1 ; ro != 0 and i <= N ; ++i) {
        const Var v = {i,cur_v.second};
        if (B.open(v)) {
          B(v) = State::forbidden; --ro;
          forbidden_forank_update(v, Line::c);
          if (falsified_) return;
        }
      }
    }
    void d_propagate(const Var cur_v) noexcept {
      assert(cur_v.first >= 1 and cur_v.second >= 1);
      assert(cur_v.first <= N and cur_v.second <= N);
      assert(B(cur_v) == State::placed);
      const Diagonal d = B.diagonal(cur_v);
      const Var d_v = d.s;
      assert(d.l <= N);
      assert(B.d_rank(d.i).p == 1);
      auto& ro = B.d_rank(d.i).o;
      for (coord_t i = 0; ro != 0 and i < d.l; ++i) {
        const Var v = {d_v.first + i, d_v.second + i};
        if (B.open(v)) {
          B(v) = State::forbidden; --ro;
          forbidden_forank_update(v, Line::d);
          if (falsified_) return;
        }
      }
    }
    void ad_propagate(const Var cur_v) noexcept {
      assert(cur_v.first >= 1 and cur_v.second >= 1);
      assert(cur_v.first <= N and cur_v.second <= N);
      assert(B(cur_v) == State::placed);
      const AntiDiagonal ad = B.anti_diagonal(cur_v);
      const Var ad_v = ad.s;
      assert(ad.l <= N);
      assert(B.ad_rank(ad.i).p == 1);
      auto& ro = B.ad_rank(ad.i).o;
      for (coord_t i = 0; ro != 0 and i < ad.l; ++i) {
        const Var v = {ad_v.first + i, ad_v.second - i};
        if (B.open(v)) {
          B(v) = State::forbidden; --ro;
          forbidden_forank_update(v, Line::ad);
          if (falsified_) return;
        }
      }
    }

    // Occupy field v and propagate amo,
    // update falsified_ if found and return:
    void set_true(const Var v) noexcept {
      assert(v.first >= 1 and v.second >= 1);
      assert(v.first <= N and v.second <= N);
      assert(B(v) == State::open);
      B(v) = State::placed;
      // Update tranks:
      assert(B.t_rank().o+B.t_rank().p+B.t_rank().f == n());
      ++B.t_rank().p; --B.t_rank().o;
      // Using the "old" o-degree:
      {const auto deg = B.odegree(v); B.t_rank().o -= deg; B.t_rank().f += deg;}
      // Update o/p-ranks (to current state of board), while updating f-rank
      // in anticipation of amo-propagation:
      {auto& r = B.r_rank(v.first); --r.o; r.p = 1; r.f = N-1;}
      {auto& c = B.c_rank(v.second); --c.o; c.p = 1; c.f = N-1;}
      {const auto d = B.diagonal(v); auto& dr = B.d_rank(d.i);
       --dr.o; dr.p = 1; dr.f = d.l-1;}
      {const auto a = B.anti_diagonal(v); auto& ar = B.ad_rank(a.i);
       --ar.o; ar.p = 1; ar.f = a.l-1;}
      r_propagate(v); if (falsified_) return;
      c_propagate(v); if (falsified_) return;
      d_propagate(v); if (falsified_) return;
      ad_propagate(v);
    }
    void set_false(const Var v) noexcept {
      assert(v.first >= 1 and v.second >= 1);
      assert(v.first <= N and v.second <= N);
      assert(B(v) == State::open);
      assert(B.t_rank().o+B.t_rank().p+B.t_rank().f == n());
      ++B.t_rank().f; --B.t_rank().o;
      B(v) = State::forbidden;
      forbidden_forank_update(v,Line::none);
    }


    typedef std::stack<Var> Stack;
    Stack stack;
    bool falsified_ = false;

  };

}

#endif
