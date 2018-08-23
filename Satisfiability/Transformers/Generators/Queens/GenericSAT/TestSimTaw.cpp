// Oliver Kullmann, 6.8.2018 (Swansea)
/* Copyright 2018 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#include <iostream>

#include <cassert>

#include "Backtracking.hpp"
#include "NQueens.hpp"

int main() {
  using namespace NQueens;
  using namespace ChessBoard;
  using namespace Backtracking;
  {
    AmoAlo_board F(1);
    assert(F.N == 1);
    assert(F.satisfied());
    assert(not F.falsified());
    assert(F.n() == 1);
    assert(F.nset() == 1);
    assert(F.t_rank().o == 0);
    assert(F.t_rank().p == 1);
    assert(F.t_rank().f == 0);
    assert(not F.open({1,1}));
    const AmoAlo_board::Board b {{State::open, State::open}, {State::open, State::placed}};
    assert(F.board() == b);
    {
      const AmoAlo_board FC(F);
      assert(FC.board({1,1}) == State::placed);
    }
    {
      TawHeuristics h(F);
      assert(h.weight(0) == 0.0);
      assert(h.weight(1) == 0.0);
      assert(h.weight(2) == 4.85);
      assert(h.weight(3) == 1.0);
      assert(h.weight(4) == 0.354);
      assert(h.weight(5) == 0.11);
      assert(h.weight(6) == 0.0694);
      assert(h.weight(7) == 0.0694 * std::pow(1.46,-1));
    }
  }
  {
    const AmoAlo_board F(2);
    assert(F.N == 2);
    assert(F.t_rank().o == 4);
    assert(F.t_rank().p == 0);
    assert(F.t_rank().f == 0);
    const Var v11{1,1};
    assert(F.odegree(v11) == 3);
    TawHeuristics h(F);
    assert(h.heuristics(v11) == TawHeuristics::Bp(3*4.85, 2*4.85));
  }
  {
    AmoAlo_board F(4);
    const Var v{1,1};
    F.set(v,true);
    assert(F.t_rank().o == 16 - 4 - 3 - 3);
    assert(F.t_rank().p == 1);
    assert(F.t_rank().f == 3 + 3 + 3);
    const AmoAlo_board FC(F);
    assert(FC.board({1,1}) == State::placed);
    assert(FC.board({1,2}) == State::forbidden);
    assert(FC.board({1,3}) == State::forbidden);
    assert(FC.board({1,4}) == State::forbidden);
    assert(FC.board({2,1}) == State::forbidden);
    assert(FC.board({2,2}) == State::forbidden);
    assert(FC.board({2,3}) == State::open);
    assert(FC.board({2,4}) == State::open);
    assert(FC.board({3,1}) == State::forbidden);
    assert(FC.board({3,2}) == State::open);
    assert(FC.board({3,3}) == State::forbidden);
    assert(FC.board({3,4}) == State::open);
    assert(FC.board({4,1}) == State::forbidden);
    assert(FC.board({4,2}) == State::open);
    assert(FC.board({4,3}) == State::open);
    assert(FC.board({4,4}) == State::forbidden);
  }
  {
    // Falsified due to amo propagation:
    AmoAlo_board F(3);
    const Var v{2,2};
    F.set(v,true);
    assert(F.falsified());
    assert(((const AmoAlo_board) F).board(v) == State::placed);
    {const TotalRank t{0,1,8}; assert(F.t_rank() == t);}
    assert(F.r_rank(2).p == 1);
    assert(F.r_rank(2).f == 2);
    assert(F.c_rank(2).p == 1);
    assert(F.c_rank(2).f == 2);
    assert(F.d_rank(2).p == 1);
    assert(F.d_rank(2).f == 2);
    assert(F.ad_rank(2).p == 1);
    assert(F.ad_rank(2).f == 2);
  }
  {
    // Satisfied due to alo propagation:
    AmoAlo_board F(4);
    const Var v{1,2};
    F.set(v,true);
    assert(F.satisfied());
  }
  {
    AmoAlo_board F(5);
    ChessBoard::Diagonal d = F.diagonal({1,1});
    assert(d.s.first == 1);
    assert(d.s.second == 1);
    assert(d.l == 5);
    d = F.diagonal({1,3});
    assert(d.s.first == 1);
    assert(d.s.second == 3);
    assert(d.l == 3);
    d = F.diagonal({4,2});
    assert(d.s.first == 3);
    assert(d.s.second == 1);
    assert(d.l == 3);
    d = F.diagonal({2,5});
    assert(d.s.first == 1);
    assert(d.s.second == 4);
    assert(d.l == 2);
    d = F.diagonal({5,5});
    assert(d.s.first == 1);
    assert(d.s.second == 1);
    assert(d.l == 5);
  }
  {
    NQueens::AmoAlo_board F(5);
    ChessBoard::AntiDiagonal ad = F.anti_diagonal({1,1});
    assert(ad.s.first == 1);
    assert(ad.s.second == 1);
    assert(ad.l == 1);
    ad = F.anti_diagonal({1,3});
    assert(ad.s.first == 1);
    assert(ad.s.second == 3);
    assert(ad.l == 3);
    ad = F.anti_diagonal({4,2});
    assert(ad.s.first == 1);
    assert(ad.s.second == 5);
    assert(ad.l == 5);
    ad = F.anti_diagonal({2,5});
    assert(ad.s.first == 2);
    assert(ad.s.second == 5);
    assert(ad.l == 4);
    ad = F.anti_diagonal({5,5});
    assert(ad.s.first == 5);
    assert(ad.s.second == 5);
    assert(ad.l == 1);
  }
  {
    const AmoAlo_board F(6);
    assert(F.N == 6);
    const Var v11{1,1};
    assert(F.odegree(v11) == 15);
    CountSat<AmoAlo_board, FirstOpenRandom> B(F.N);
    const auto stats = B(F);
    assert(stats.solutions == 4);
    {const FirstOpenRandom::varvec_t P{{5,3},{2,6},{3,6},{1,2},{4,6},{1,5},{3,1},{2,1},{6,3},{3,5},{4,4},{3,2},{5,4},{3,3},{5,2},{1,6},{4,5},{2,2},{2,4},{5,5},{6,2},{6,1},{4,3},{5,6},{6,4},{5,1},{4,2},{2,3},{1,3},{1,1},{2,5},{6,6},{1,4},{6,5},{3,4},{4,1}};
     assert(FirstOpenRandom::permutation() == P);}
    assert(stats.nodes == 141);
    {CountSat<AmoAlo_board, FirstOpenRandom> B2(F.N,FirstOpenRandom::vec_seed_t{{0}});
     const auto stats2 = B(F); // the unique (static) random_permutation has been set
     assert(stats2.solutions == 4);
     assert(stats2.nodes == 119);
    }
    CountSat<AmoAlo_board, FirstOpenRandom> B2(F.N,FirstOpenRandom::vec_seed_t(1,FirstOpenRandom::seed_t(4294967296)));
    const auto stats2 = B2(F);
    assert(stats2.solutions == 4);
    assert(stats2.nodes == 119);
    const AmoAlo_board F2(5);
    CountSat<AmoAlo_board, FirstOpenRandom>(F2.N,FirstOpenRandom::vec_seed_t(1,FirstOpenRandom::seed_t(5)));
    const auto stats3 = B(F2);
    assert(stats3.solutions == 10);
    assert(stats3.nodes == 49);
  }
  {
    AmoAlo_board F(9);
    F.set({5,5}, true);
    assert(F.c_rank(5).o == 0);
  }
}
