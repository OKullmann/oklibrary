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

    explicit AmoAlo_board(const coord_t N) :
      N(N), b{N+1, std::vector<State>(N+1)},
      r_ranks{N+1, {N,0,0}}, c_ranks(r_ranks),
      d_ranks(dad_init()), ad_ranks(d_ranks), trank{N*N,0,0} {
        assert(N <= ChessBoard::max_coord);
        assert(b.size() == N+1);
        assert(r_ranks.size() == N+1);
        assert(c_ranks.size() == N+1);
        assert(d_ranks.size() == 2*N-1);
        assert(ad_ranks.size() == 2*N-1);
        r_ranks[0].o = 0; c_ranks[0].o = 0;
        if (N == 1) set_true({1,1});
    }

    bool satisfied() const noexcept { return trank.p == N; }
    bool falsified() const noexcept { return falsified_; }
    Var_uint n() const noexcept { return N*N; }
    Var_uint nset() const noexcept { return trank.p+trank.f; }

    void set(const Var v, const bool val) noexcept {
      assert(v.first >= 1 and v.second >= 1);
      assert(v.first <= N and v.second <= N);
      assert(board(v) == State::open);
      assert(r_ranks[v.first].o >= 2);
      assert(c_ranks[v.second].o >= 2);
      if (val) set_true(v); else set_false(v);
      while (not falsified_ and not stack.empty()) {
        const Var w = stack.top(); stack.pop();
        switch (board(w)) {
        case State::forbidden : falsified_ = true; return;
        case State::open : set_true(w);
        default:;
        }
      }
    }

    typedef std::vector<Rank> Ranks;
    typedef std::vector<std::vector<State>> Board;
    const Ranks& r_rank() const noexcept { return r_ranks; }
    const Rank& r_rank(const coord_t i) const noexcept { return r_ranks[i]; }
    const Ranks& c_rank() const noexcept { return c_ranks; }
    const Rank& c_rank(const coord_t j) const noexcept { return c_ranks[j]; }
    const Ranks& d_rank() const noexcept { return d_ranks; }
    const Rank& d_rank(const coord_t i) const noexcept { return d_ranks[i]; }
    const Ranks& ad_rank() const noexcept { return ad_ranks; }
    const Rank& ad_rank(const coord_t i) const noexcept { return ad_ranks[i]; }
    const TotalRank& t_rank() const noexcept { return trank; }
    const Board& board() const noexcept { return b; }
    State board(const Var v) const noexcept {
      assert(v.first >= 1 and v.second >= 1);
      assert(v.first <= N and v.second <= N);
      return b[v.first][v.second];
    }
    bool open(const Var v) const noexcept {
      assert(v.first >= 1 and v.second >= 1);
      assert(v.first <= N and v.second <= N);
      return (board(v) == State::open);
    }

    Diagonal diagonal(const Var v) const noexcept {
      return ChessBoard::diagonal(v, N);
    }
    AntiDiagonal anti_diagonal(const Var v) const noexcept {
      return ChessBoard::anti_diagonal(v, N);
    }

    // The number of open fields on the four lines of v, excluding v;
    // o-ranks must be correct, except of possibly v having changed before
    // from open to placed, which then must *not* have been updated:
    Var_uint odegree(const Var v) const noexcept {
      assert(v.first >= 1 and v.second >= 1);
      assert(v.first <= N and v.second <= N);
      const Diagonal d = diagonal(v);
      const AntiDiagonal ad = anti_diagonal(v);
      assert(d.i < d_ranks.size());
      assert(ad.i < ad_ranks.size());
      return r_ranks[v.first].o + c_ranks[v.second].o + d_ranks[d.i].o + ad_ranks[ad.i].o - 4;
    }

  private:

    enum class Line {r,c,d,ad,none};
    // f/o-ranks of forbidden v are updated for non-occupied lines (given by
    // parameter exclude), with alo-falsification and alo-propagation detected:
    void forbidden_forank_update(const Var v, const Line exclude) noexcept {
      assert(v.first >= 1 and v.second >= 1);
      assert(v.first <= N and v.second <= N);
      assert(board(v) == State::forbidden);
      if (exclude != Line::r) {
        auto& rank = r_ranks[v.first];
	assert(rank.p == 0);
        --rank.o; ++rank.f;
        if (exclude != Line::none and rank.o == 0) {
          falsified_ = true; return;
        }
        else if (rank.o == 1) {
          const auto& R = b[v.first];
          for (coord_t j = 1; j <= N ; ++j)
            if (R[j] == State::open) {stack.push({v.first, j}); break;}
        }
      }
      if (exclude != Line::c) {
        auto& rank = c_ranks[v.second];
        assert(rank.p == 0);
        --rank.o; ++rank.f;
        if (exclude != Line::none and rank.o == 0) {
          falsified_ = true; return;
        }
        else if (rank.o == 1)
          for (coord_t i = 1; i <= N ; ++i)
            if (open({i,v.second})) {stack.push({i,v.second}); break;}
      }
      if (exclude != Line::d) {
        const Diagonal d = diagonal(v);
        assert(d.i < d_ranks.size());
        auto& rank = d_ranks[d.i];
        --rank.o; ++rank.f;
      }
      if (exclude != Line::ad) {
        const AntiDiagonal ad = anti_diagonal(v);
        assert(ad.i < ad_ranks.size());
        auto& rank = ad_ranks[ad.i];
        --rank.o; ++rank.f;
      }
    }

    // The following four propagation-functions assume that cur_v is placed,
    // and propagate amo to its row, column, diagonal and antidiagonal.
    void r_propagate(const Var cur_v) noexcept {
      assert(cur_v.first >= 1 and cur_v.second >= 1);
      assert(cur_v.first <= N and cur_v.second <= N);
      assert(board(cur_v) == State::placed);
      auto& R = b[cur_v.first];
      assert(r_ranks[cur_v.first].p == 1);
      auto& ro = r_ranks[cur_v.first].o;
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
      assert(board(cur_v) == State::placed);
      assert(c_ranks[cur_v.second].p == 1);
      auto& ro = c_ranks[cur_v.second].o;
      for (coord_t i = 1 ; ro != 0 and i <= N ; ++i) {
        const Var v = {i,cur_v.second};
        if (open(v)) {
          board(v) = State::forbidden; --ro;
          forbidden_forank_update(v, Line::c);
          if (falsified_) return;
        }
      }
    }
    void d_propagate(const Var cur_v) noexcept {
      assert(cur_v.first >= 1 and cur_v.second >= 1);
      assert(cur_v.first <= N and cur_v.second <= N);
      assert(board(cur_v) == State::placed);
      const Diagonal d = diagonal(cur_v);
      const Var d_v = d.s;
      assert(d.l <= N);
      assert(d_ranks[d.i].p == 1);
      auto& ro = d_ranks[d.i].o;
      for (coord_t i = 0; ro != 0 and i < d.l; ++i) {
        const Var v = {d_v.first + i, d_v.second + i};
        if (open(v)) {
          board(v) = State::forbidden; --ro;
          forbidden_forank_update(v, Line::d);
          if (falsified_) return;
        }
      }
    }
    void ad_propagate(const Var cur_v) noexcept {
      assert(cur_v.first >= 1 and cur_v.second >= 1);
      assert(cur_v.first <= N and cur_v.second <= N);
      assert(board(cur_v) == State::placed);
      const AntiDiagonal ad = anti_diagonal(cur_v);
      const Var ad_v = ad.s;
      assert(ad.l <= N);
      assert(ad_ranks[ad.i].p == 1);
      auto& ro = ad_ranks[ad.i].o;
      for (coord_t i = 0; ro != 0 and i < ad.l; ++i) {
        const Var v = {ad_v.first + i, ad_v.second - i};
        if (open(v)) {
          board(v) = State::forbidden; --ro;
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
      assert(board(v) == State::open);
      board(v) = State::placed;
      // Update tranks:
      assert(trank.o+trank.p+trank.f == n());
      ++trank.p; --trank.o;
      // Using the "old" o-degree:
      {const auto deg = odegree(v); trank.o -= deg; trank.f += deg;}
      // Update o/p-ranks (to current state of board), while updating f-rank
      // in anticipation of amo-propagation:
      {auto& r = r_ranks[v.first]; --r.o; r.p = 1; r.f = N-1;}
      {auto& c = c_ranks[v.second]; --c.o; c.p = 1; c.f = N-1;}
      {const auto d = diagonal(v); auto& dr = d_ranks[d.i];
       --dr.o; dr.p = 1; dr.f = d.l-1;}
      {const auto a = anti_diagonal(v); auto& ar = ad_ranks[a.i];
       --ar.o; ar.p = 1; ar.f = a.l-1;}
      r_propagate(v); if (falsified_) return;
      c_propagate(v); if (falsified_) return;
      d_propagate(v); if (falsified_) return;
      ad_propagate(v);
    }
    void set_false(const Var v) noexcept {
      assert(v.first >= 1 and v.second >= 1);
      assert(v.first <= N and v.second <= N);
      assert(board(v) == State::open);
      assert(trank.o+trank.p+trank.f == n());
      ++trank.f; --trank.o;
      board(v) = State::forbidden;
      forbidden_forank_update(v,Line::none);
    }

    Board b;
    Ranks r_ranks;
    Ranks c_ranks;
    Ranks d_ranks;
    Ranks ad_ranks;
    TotalRank trank;
    typedef std::stack<Var> Stack;
    Stack stack;
    bool falsified_ = false;

    Ranks dad_init() const {
      Ranks ranks(2*N-1);
      Var_uint i = 0;
      for (Var_uint r = 1; r < N ; ++r) ranks[i++].o = r;
      for (Var_uint r = N; r > 0 ; --r) ranks[i++].o = r;
      return ranks;
    }

    State& board(const Var v) noexcept {
      assert(v.first >= 1 and v.second >= 1);
      assert(v.first <= N and v.second <= N);
      return b[v.first][v.second];
    }
  };


  // Phased AmoALo propagation:
// BUG: We clearly said that the invariants are the basic --- but there are NONE.
  class PhasedAmoAlo_board {
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

    explicit PhasedAmoAlo_board(const coord_t N) :
      N(N), b{N+1, std::vector<State>(N+1)},
      r_ranks{N+1, {N,0,0}}, c_ranks(r_ranks),
      d_ranks(dad_init()), ad_ranks(d_ranks), trank{N*N,0,0} {
        assert(N <= ChessBoard::max_coord);
        assert(b.size() == N+1);
        assert(r_ranks.size() == N+1);
        assert(c_ranks.size() == N+1);
        assert(d_ranks.size() == 2*N-1);
        assert(ad_ranks.size() == 2*N-1);
        r_ranks[0].o = 0; c_ranks[0].o = 0;
        if (N == 1) set_true({1,1});
    }

    bool satisfied() const noexcept { return trank.p == N; }
    bool falsified() const noexcept { return falsified_; }
    Var_uint n() const noexcept { return N*N; }
    Var_uint nset() const noexcept { return trank.p+trank.f; }

    void set(const Var v, const bool val) noexcept {
      assert(v.first >= 1 and v.second >= 1);
      assert(v.first <= N and v.second <= N);
      assert(board(v) == State::open);
      assert(r_ranks[v.first].o >= 2);
      assert(c_ranks[v.second].o >= 2);
      if (val) { set_true(v); if(not falsified_) alo(); }
      else     { set_false(v); alo(v); }
      while (not falsified_ and not place.empty()) {
        // Using single-field set_true function:
        if (place.size() == 1) set_true(place.front());
        else set_true(place);
        if (falsified_) break;
        place.clear();
        alo();
      }
    }

    typedef std::vector<Rank> Ranks;
    typedef std::vector<std::vector<State>> Board;
    typedef std::vector<Var> Place;
    const Ranks& r_rank() const noexcept { return r_ranks; }
    const Rank& r_rank(const coord_t i) const noexcept { return r_ranks[i]; }
    const Ranks& c_rank() const noexcept { return c_ranks; }
    const Rank& c_rank(const coord_t j) const noexcept { return c_ranks[j]; }
    const Ranks& d_rank() const noexcept { return d_ranks; }
    const Rank& d_rank(const coord_t i) const noexcept { return d_ranks[i]; }
    const Ranks& ad_rank() const noexcept { return ad_ranks; }
    const Rank& ad_rank(const coord_t i) const noexcept { return ad_ranks[i]; }
    const TotalRank& t_rank() const noexcept { return trank; }
    const Board& board() const noexcept { return b; }
    State board(const Var v) const noexcept {
      assert(v.first >= 1 and v.second >= 1);
      assert(v.first <= N and v.second <= N);
      return b[v.first][v.second];
    }
    bool open(const Var v) const noexcept {
      assert(v.first >= 1 and v.second >= 1);
      assert(v.first <= N and v.second <= N);
      return (board(v) == State::open);
    }

    Diagonal diagonal(const Var v) const noexcept {
      return ChessBoard::diagonal(v, N);
    }
    AntiDiagonal anti_diagonal(const Var v) const noexcept {
      return ChessBoard::anti_diagonal(v, N);
    }

    // Returns true if at least one field is set to placed in corresponding r, c, d and ad:
    bool placed(const Var v) const noexcept {
      assert(v.first >= 1 and v.second >= 1);
      assert(v.first <= N and v.second <= N);
      const Diagonal d = diagonal(v);
      const AntiDiagonal ad = anti_diagonal(v);
      assert(d.i < d_ranks.size());
      assert(ad.i < ad_ranks.size());
      return (r_ranks[v.first].p == 1 or c_ranks[v.second].p == 1 or d_ranks[d.i].p == 1 or ad_ranks[ad.i].p == 1);
    }
    // The number of open fields on the four lines of v, excluding v;
    // o-ranks must be correct, except of possibly v having changed before
    // from open to placed, which then must *not* have been updated:
    Var_uint odegree(const Var v) const noexcept {
      assert(v.first >= 1 and v.second >= 1);
      assert(v.first <= N and v.second <= N);
      const Diagonal d = diagonal(v);
      const AntiDiagonal ad = anti_diagonal(v);
      assert(d.i < d_ranks.size());
      assert(ad.i < ad_ranks.size());
      return r_ranks[v.first].o + c_ranks[v.second].o + d_ranks[d.i].o + ad_ranks[ad.i].o - 4;
    }

  private:

    enum class Line {r,c,d,ad,none};
    // f/o-ranks of forbidden v are updated for non-occupied lines (given by
    // parameter exclude), with alo-falsification detected:
    void forbidden_forank_update(const Var v, const Line exclude) noexcept {
      assert(v.first >= 1 and v.second >= 1);
      assert(v.first <= N and v.second <= N);
      assert(board(v) == State::forbidden);
      if (exclude != Line::r) {
        auto& rank = r_ranks[v.first];
        --rank.o; ++rank.f;
// BUG: We clearly said *phased*, thus no such interleaved tests (also at any other similar place)
        if (exclude != Line::none and rank.o == 0) {
          falsified_ = true; return;
        }
      }
      if (exclude != Line::c) {
        auto& rank = c_ranks[v.second];
        --rank.o; ++rank.f;
        if (exclude != Line::none and rank.o == 0) {
          falsified_ = true; return;
        }
      }
      if (exclude != Line::d) {
        const Diagonal d = diagonal(v);
        assert(d.i < d_ranks.size());
        auto& rank = d_ranks[d.i];
        --rank.o; ++rank.f;
      }
      if (exclude != Line::ad) {
        const AntiDiagonal ad = anti_diagonal(v);
        assert(ad.i < ad_ranks.size());
        auto& rank = ad_ranks[ad.i];
        --rank.o; ++rank.f;
      }
    }

    // All the fields propagated by alo-constraints pushed into place-vector.
    // Only one field is pushed in place (even if different alo propagation trigger the same field):
// BUG: The above explanations make no sense; one does not "push into a vector".
    void alo() noexcept {
      for (coord_t i = 1 ; i <= N ; ++i) {
        auto& rank = r_ranks[i];
        if (rank.p == 0 and rank.o == 1) {
          const auto& R = b[i];
          for (coord_t j = 1; j <= N ; ++j)
            if (R[j] == State::open) {place.push_back({i,j}); break;}
        }
      }
      for (coord_t j = 1 ; j <= N ; ++j) {
        auto& rank = c_ranks[j];
        if (rank.p == 0 and rank.o == 1)
          for (coord_t i = 1; i <= N ; ++i)
            if (open({i,j})) {
// BUG: this is very inefficient, and a todo needs to be opened with the various alternative possibilities
              if (std::find(place.begin(), place.end(), Var{i,j}) == place.end()) place.push_back({i,j});
              break;
            }
      }
    }

    // All the fields propagated by alo constraints (Var v) pushed into placed:
// BUG: this explains nothing (which INVARIANT is established is the point)
    void alo(const Var v) noexcept {
      assert(v.first >= 1 and v.second >= 1);
      assert(v.first <= N and v.second <= N);
      assert(board(v) == State::forbidden);
      auto& r_rank = r_ranks[v.first];
      assert(r_rank.p == 0);
      if (r_rank.o == 1) {
        const auto& R = b[v.first];
        for (coord_t j = 1; j <= N ; ++j)
          if (R[j] == State::open) {place.push_back({v.first, j}); break;}
      }
      auto& c_rank = c_ranks[v.second];
      assert(c_rank.p == 0);
      if (c_rank.o == 1) {
          for (coord_t i = 1; i <= N ; ++i)
// BUG: same as before
            if (open({i,v.second})) {place.push_back({i, v.second}); break;}
      }
    }
    // The following four propagation-functions assume that cur_v is placed,
    // and propagate amo to its row, column, diagonal and antidiagonal.
// BUG: this should have been replaced / eliminated --- it is inappropropriate for the phased approach
    void r_propagate(const Var cur_v) noexcept {
      assert(cur_v.first >= 1 and cur_v.second >= 1);
      assert(cur_v.first <= N and cur_v.second <= N);
      assert(board(cur_v) == State::placed);
      auto& R = b[cur_v.first];
      assert(r_ranks[cur_v.first].p == 1);
      auto& ro = r_ranks[cur_v.first].o;
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
      assert(board(cur_v) == State::placed);
      assert(c_ranks[cur_v.second].p == 1);
      auto& ro = c_ranks[cur_v.second].o;
      for (coord_t i = 1 ; ro != 0 and i <= N ; ++i) {
        const Var v = {i,cur_v.second};
        if (open(v)) {
          board(v) = State::forbidden; --ro;
          forbidden_forank_update(v, Line::c);
          if (falsified_) return;
        }
      }
    }
    void d_propagate(const Var cur_v) noexcept {
      assert(cur_v.first >= 1 and cur_v.second >= 1);
      assert(cur_v.first <= N and cur_v.second <= N);
      assert(board(cur_v) == State::placed);
      const Diagonal d = diagonal(cur_v);
      const Var d_v = d.s;
      assert(d.l <= N);
      assert(d_ranks[d.i].p == 1);
      auto& ro = d_ranks[d.i].o;
      for (coord_t i = 0; ro != 0 and i < d.l; ++i) {
        const Var v = {d_v.first + i, d_v.second + i};
        if (open(v)) {
          board(v) = State::forbidden; --ro;
          forbidden_forank_update(v, Line::d);
          if (falsified_) return;
        }
      }
    }
    void ad_propagate(const Var cur_v) noexcept {
      assert(cur_v.first >= 1 and cur_v.second >= 1);
      assert(cur_v.first <= N and cur_v.second <= N);
      assert(board(cur_v) == State::placed);
      const AntiDiagonal ad = anti_diagonal(cur_v);
      const Var ad_v = ad.s;
      assert(ad.l <= N);
      assert(ad_ranks[ad.i].p == 1);
      auto& ro = ad_ranks[ad.i].o;
      for (coord_t i = 0; ro != 0 and i < ad.l; ++i) {
        const Var v = {ad_v.first + i, ad_v.second - i};
        if (open(v)) {
          board(v) = State::forbidden; --ro;
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
      assert(board(v) == State::open);
      board(v) = State::placed;
      // Update tranks:
      assert(trank.o+trank.p+trank.f == n());
      ++trank.p; --trank.o;
      // Using the "old" o-degree:
      {const auto deg = odegree(v); trank.o -= deg; trank.f += deg;}
      // Update o/p-ranks (to current state of board), while updating f-rank
      // in anticipation of amo-propagation:
      {auto& r = r_ranks[v.first]; --r.o; r.p = 1; r.f = N-1;}
      {auto& c = c_ranks[v.second]; --c.o; c.p = 1; c.f = N-1;}
      {const auto d = diagonal(v); auto& dr = d_ranks[d.i];
       --dr.o; dr.p = 1; dr.f = d.l-1;}
      {const auto a = anti_diagonal(v); auto& ar = ad_ranks[a.i];
       --ar.o; ar.p = 1; ar.f = a.l-1;}
      r_propagate(v); if (falsified_) return;
      c_propagate(v); if (falsified_) return;
      d_propagate(v); if (falsified_) return;
      ad_propagate(v);
    }
    // Occupy fields in place, update falsified_ if found (if already a field is placed in r, c, d or ad) and return.
    // Propagate amo, update falsified_ if found and return:
// BUG: where are the invariants?? no design visible
    void set_true(const Place place) noexcept {
      for (const Var v : place) {
        if(open(v)) {
          if (placed(v)) { falsified_ = true; return; }
          else {
            assert(v.first >= 1 and v.second >= 1);
            assert(v.first <= N and v.second <= N);
            assert(board(v) == State::open);
            board(v) = State::placed;
            // Update tranks:
            assert(trank.o+trank.p+trank.f == n());
            ++trank.p; --trank.o;
            // Update p-rank for falsification detection (while setting multiple fields at once):
            {auto& r = r_ranks[v.first]; r.p = 1;}
            {auto& c = c_ranks[v.second]; c.p = 1;}
            {const auto d = diagonal(v); auto& dr = d_ranks[d.i]; dr.p = 1;}
            {const auto a = anti_diagonal(v); auto& ar = ad_ranks[a.i]; ar.p = 1;}
          }
        }
      }
      for (const Var v : place) {
        {const auto deg = odegree(v); trank.o -= deg; trank.f += deg;}
        // Update o-rank (to current state of board), while updating f-rank
        // in anticipation of amo-propagation:
        {auto& r = r_ranks[v.first]; --r.o; r.f = N-1;}
        {auto& c = c_ranks[v.second]; --c.o; c.f = N-1;}
        {const auto d = diagonal(v); auto& dr = d_ranks[d.i];
         --dr.o; dr.f = d.l-1;}
        {const auto a = anti_diagonal(v); auto& ar = ad_ranks[a.i];
         --ar.o; ar.f = a.l-1;}
        r_propagate(v); if (falsified_) return;
        c_propagate(v); if (falsified_) return;
        d_propagate(v); if (falsified_) return;
        ad_propagate(v);
      }
    }

    void set_false(const Var v) noexcept {
      assert(v.first >= 1 and v.second >= 1);
      assert(v.first <= N and v.second <= N);
      assert(board(v) == State::open);
      assert(trank.o+trank.p+trank.f == n());
      ++trank.f; --trank.o;
      board(v) = State::forbidden;
      forbidden_forank_update(v,Line::none);
    }

    Board b;
    Ranks r_ranks;
    Ranks c_ranks;
    Ranks d_ranks;
    Ranks ad_ranks;
    TotalRank trank;
    Place place;
    bool falsified_ = false;

    Ranks dad_init() const {
      Ranks ranks(2*N-1);
      Var_uint i = 0;
      for (Var_uint r = 1; r < N ; ++r) ranks[i++].o = r;
      for (Var_uint r = N; r > 0 ; --r) ranks[i++].o = r;
      return ranks;
    }

    State& board(const Var v) noexcept {
      assert(v.first >= 1 and v.second >= 1);
      assert(v.first <= N and v.second <= N);
      return b[v.first][v.second];
    }
  };

}

#endif
