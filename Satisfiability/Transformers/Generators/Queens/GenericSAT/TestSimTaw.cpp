// Oliver Kullmann, 6.8.2018 (Swansea)
/* Copyright 2018 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#include <iostream>
#include <sstream>

#include <cassert>

#include "Backtracking.hpp"
#include "NQueens.hpp"
#include "Heuristics.hpp"
#include "Trees.hpp"

int main() {
  using namespace NQueens;
  using namespace Heuristics;
  using namespace ChessBoard;
  using namespace Backtracking;
  using namespace Trees;
  {
    AmoAlo_board F(1);
    assert(F.N == 1);
    assert(F.satisfied());
    assert(not F.falsified());
    assert(F.n() == 1);
    assert(F.nset() == 1);
    const auto& B = F.board();
    assert(B.t_rank().o == 0);
    assert(B.t_rank().p == 1);
    assert(B.t_rank().f == 0);
    assert(not B.open({1,1}));
    const Board::Board_t b {{State::open, State::open}, {State::open, State::placed}};
    assert(B.board() == b);
    {
      const AmoAlo_board FC(F);
      assert(FC.board().board({1,1}) == State::placed);
    }
    {
      TawHeuristics<> h(F);
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
    const auto& B = F.board();
    assert(B.t_rank().o == 4);
    assert(B.t_rank().p == 0);
    assert(B.t_rank().f == 0);
    const Var v11{1,1};
    assert(B.odegree(v11) == 3);
    TawHeuristics<> h(F);
    assert(h.heuristics(v11) == TawHeuristics<>::Bp(3*4.85, 2*4.85));
  }
  {
    AmoAlo_board F(4);
    const Var v{1,1};
    F.set(v,true);
    const auto& B = F.board();
    assert(B.t_rank().o == 16 - 4 - 3 - 3);
    assert(B.t_rank().p == 1);
    assert(B.t_rank().f == 3 + 3 + 3);
    const AmoAlo_board FC(F);
    const auto& BC = FC.board();
    assert(BC.board({1,1}) == State::placed);
    assert(BC.board({1,2}) == State::forbidden);
    assert(BC.board({1,3}) == State::forbidden);
    assert(BC.board({1,4}) == State::forbidden);
    assert(BC.board({2,1}) == State::forbidden);
    assert(BC.board({2,2}) == State::forbidden);
    assert(BC.board({2,3}) == State::open);
    assert(BC.board({2,4}) == State::open);
    assert(BC.board({3,1}) == State::forbidden);
    assert(BC.board({3,2}) == State::open);
    assert(BC.board({3,3}) == State::forbidden);
    assert(BC.board({3,4}) == State::open);
    assert(BC.board({4,1}) == State::forbidden);
    assert(BC.board({4,2}) == State::open);
    assert(BC.board({4,3}) == State::open);
    assert(BC.board({4,4}) == State::forbidden);
  }
  {
    // Falsified due to amo propagation:
    AmoAlo_board F(3);
    const Var v{2,2};
    F.set(v,true);
    assert(F.falsified());
    const auto& B{F.board()};
    assert(B.board(v) == State::placed);
    {const TotalRank t{0,1,8}; assert(B.t_rank() == t);}
    assert(B.r_rank(2).p == 1);
    assert(B.r_rank(2).f == 2);
    assert(B.c_rank(2).p == 1);
    assert(B.c_rank(2).f == 2);
    assert(B.d_rank(2).p == 1);
    assert(B.d_rank(2).f == 2);
    assert(B.ad_rank(2).p == 1);
    assert(B.ad_rank(2).f == 2);
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
    assert(F.board().odegree(v11) == 15);
    CountSat<AmoAlo_board, FirstOpenRandom> B(F.N);
    const auto stats = B(F);
    assert(stats.solutions == 4);
    {const FirstOpenRandom::varvec_t P{{1,1},{6,3},{1,6},{2,2},{1,5},{1,4},{3,1},{2,3},{6,6},{4,4},{6,4},{3,2},{6,1},{2,5},{5,6},{5,2},{6,2},{4,2},{4,1},{6,5},{4,6},{5,4},{1,2},{3,3},{3,6},{4,3},{4,5},{3,4},{5,3},{3,5},{5,5},{5,1},{2,6},{2,4},{2,1},{1,3}};
     assert(FirstOpenRandom::permutation() == P);}
    assert(stats.nodes == 129);
    {CountSat<AmoAlo_board, FirstOpenRandom> B2(F.N,FirstOpenRandom::vec_seed_t{{0}});
     const auto stats2 = B(F); // the unique (static) random_permutation has been set
     assert(stats2.solutions == 4);
     assert(stats2.nodes == 133);
    }
    CountSat<AmoAlo_board, FirstOpenRandom> B2(F.N,FirstOpenRandom::vec_seed_t(1,FirstOpenRandom::seed_t(4294967296)));
    const auto stats2 = B2(F);
    assert(stats2.solutions == 4);
    assert(stats2.nodes == 133);
    const AmoAlo_board F2(5);
    CountSat<AmoAlo_board, FirstOpenRandom>(F2.N,FirstOpenRandom::vec_seed_t(1,FirstOpenRandom::seed_t(5)));
    const auto stats3 = B(F2);
    assert(stats3.solutions == 10);
    assert(stats3.nodes == 47);
  }
  {
    AmoAlo_board F(9);
    F.set({5,5}, true);
    assert(F.board().c_rank(5).o == 0);
  }
  {
    const Var v0{1,1};
    assert(not singular(v0));
    assert(enum_squarenumbering(1) == v0);
    const Var_uint two64m1 = std::numeric_limits<Var_uint>::max();
    const Var_uint two32m1 = std::numeric_limits<coord_t>::max();
    const Var v{1,two32m1};
    assert(not singular(v));
    assert(enum_squarenumbering(two64m1 - 2*two32m1) == v);
    const Var vmax{two32m1,two32m1};
    assert(not singular(vmax));
    assert(enum_squarenumbering(18446744060824649731ull) == vmax);
    const varvec_t V{{1,1},{2,1},{2,2},{1,2},{3,1},{3,2},{3,3},{2,3},{1,3}};
    assert(enum_square(3) == V);
  }
  {
    PhasedAmoAlo_board F(1);
    assert(F.N == 1);
    assert(F.satisfied());
    assert(not F.falsified());
    assert(F.n() == 1);
    assert(F.nset() == 1);
    const auto& B = F.board();
    assert(B.t_rank().o == 0);
    assert(B.t_rank().p == 1);
    assert(B.t_rank().f == 0);
    assert(not B.open({1,1}));
    const Board::Board_t b {{State::open, State::open}, {State::open, State::placed}};
    assert(B.board() == b);
    {
      const PhasedAmoAlo_board FC(F);
      assert(FC.board().board({1,1}) == State::placed);
    }
  }
  {
    // Falsified due to amo propagation:
    PhasedAmoAlo_board F(3);
    const Var v{2,2};
    F.set(v,true);
    assert(F.falsified());
    const auto& B = F.board();
    assert(B.board(v) == State::placed);
    {const TotalRank t{0,1,8}; assert(B.t_rank() == t);}
    assert(B.r_rank(2).p == 1);
    assert(B.r_rank(2).f == 2);
    assert(B.c_rank(2).p == 1);
    assert(B.c_rank(2).f == 2);
    assert(B.d_rank(2).p == 1);
    assert(B.d_rank(2).f == 2);
    assert(B.ad_rank(2).p == 1);
    assert(B.ad_rank(2).f == 2);
  }
  {
    // Satisfied due to alo propagation:
    PhasedAmoAlo_board F(4);
    const Var v{1,2};
    F.set(v,true);
    assert(F.satisfied());
  }
  {
    PhasedAmoAlo_board F(4);
    assert(F.board().placed({1,1}) == false);
    F.set({2,2},true);
    assert(F.board().placed({3,3}) == true);
  }

  {
    assert(max_index >= 0xFFFFFFFFFFFFFFFF - 1);
    assert(validindex(0));
    assert(validindex(0xFFFFFFFFFFFFFFFF - 1));
    assert(not validindex(std::numeric_limits<index_t>::max()));
    assert(null(0));
    assert(not null(1));
    assert(not validnode(0));
    assert(validnode(1));
    assert(not validedge(0));
    assert(not validedge(1));
    assert(validedge(2));
    TreeNode tn{1,1};
    assert(valid(tn));
    assert(not leaf(tn));
    constexpr TreeNode tnz{};
    assert(valid(tnz));
    assert(tnz.l == 0);
    assert(tnz.r == 0);
    assert(tnz == tnz);
    assert(not (tnz != tnz));
    assert(tn != tnz);
    assert(leaf(tnz));
    assert(Tree(1)[0] == tnz);
    assert(NodeType_v(1)[0] == NodeType::undef);
    for (const NodeType t : {NodeType::undef, NodeType::ul, NodeType::sl, NodeType::ui, NodeType::si})
     switch (t) {
     case NodeType::undef :
       assert(not leaf(t));
       assert(not innernode(t));
       assert(not satisfiable(t));
       assert(not unsatisfiable(t));
       break;
     case NodeType::ul :
       assert(leaf(t));
       assert(not innernode(t));
       assert(not satisfiable(t));
       assert(unsatisfiable(t));
       break;
     case NodeType::sl :
       assert(leaf(t));
       assert(not innernode(t));
       assert(satisfiable(t));
       assert(not unsatisfiable(t));
       break;
     case NodeType::ui :
       assert(not leaf(t));
       assert(innernode(t));
       assert(not satisfiable(t));
       assert(unsatisfiable(t));
       break;
     case NodeType::si :
       assert(not leaf(t));
       assert(innernode(t));
       assert(satisfiable(t));
       assert(not unsatisfiable(t));
       break;
     }
  }
  {
    BasicTree T;
    assert(T.numver() == 0);
    assert(T.index() == 0);
    const auto i0 = T.next_index();
    assert(i0 == 1);
    const auto i1 = T.next_index();
    assert(i1 == 2);
    T.add(i1, NodeType::ul);
    const auto i20 = T.index();
    assert(i20 == 2);
    T.add(i0, {i0+1,i20+1}, NodeType::si);
    const auto i2 = T.next_index();
    assert(i2 == 3);
    T.add(i2, NodeType::sl);
    const Tree Tstruct{{0,0},{2,3},{0,0},{0,0}};
    assert(T.tree() == Tstruct);
    const NodeType_v Ttypes{NodeType::undef, NodeType::si, NodeType::ul, NodeType::sl};
    assert(T.nodetypes() == Ttypes);
    std::stringstream out;
    output(out, T, "OK", "Test");
  }
  {
    const AmoAlo_board F(3);
    assert(F.N == 3);
    const auto B = F.board();
    assert(B.t_rank().o == 9);
    assert(B.t_rank().p == 0);
    assert(B.t_rank().f == 0);
    const Var v11{1,1};
    assert(B.odegree(v11) == 6);
    AntiTaw<> h(F);
    assert(h.heuristics(v11) == AntiTaw<>::Bp(6*4.85, 2*1));
    const ChessBoard::Var bv = Heuristics::AntiTaw(F)();
    assert(bv == v11);
  }
  {
    AmoAlo_board F(3);
    const Var v11{1,1};
    F.set(v11,false);
    assert(not F.falsified());
    AntiTaw<> h(F);
    {const ChessBoard::Var bv = Heuristics::AntiTaw(F)();
     assert(bv.first == 3 and bv.second == 3);
     assert(h.heuristics(bv) == AntiTaw<>::Bp(5*4.85, 2*1));
     F.set(bv,false);}
    assert(not F.falsified());
    {const ChessBoard::Var bv = Heuristics::AntiTaw(F)();
     assert(bv.first == 2 and bv.second == 2);
     assert(h.heuristics(bv) == AntiTaw<>::Bp(6*4.85, 2*1));
     F.set(bv,false);}
    assert(not F.falsified());
    {const ChessBoard::Var bv = Heuristics::AntiTaw(F)();
     assert(bv.first == 1 and bv.second == 3);
     assert(h.heuristics(bv) == AntiTaw<>::Bp(3*4.85, 2*4.85));
     F.set(bv,false);}
    assert(F.falsified());
    const AmoAlo_board FC(F);
    const auto& B = FC.board();
    assert(B.board({1,1}) == State::forbidden);
    assert(B.board({1,2}) == State::forbidden);
    assert(B.board({1,3}) == State::forbidden);
    assert(B.board({2,1}) == State::forbidden);
    assert(B.board({2,2}) == State::forbidden);
    assert(B.board({2,3}) == State::placed);
    assert(B.board({3,1}) == State::open);
    assert(B.board({3,2}) == State::open);
    assert(B.board({3,3}) == State::forbidden);
  }
  {
    AmoAlo_board F(4);
    AntiTaw<> h(F);
    {const ChessBoard::Var bv = Heuristics::AntiTaw(F)();
     assert(bv.first == 1 and bv.second == 1);
     assert(h.heuristics(bv) == AntiTaw<>::Bp(9*4.85, 2*0.354));
     F.set(bv,true);}
     assert(not F.falsified());
    {const ChessBoard::Var bv = Heuristics::AntiTaw(F)();
     assert(bv.first == 2 and bv.second == 4);
     assert(h.heuristics(bv) == AntiTaw<>::Bp(3*4.85, 2*4.85));
     F.set(bv,true);}
    assert(F.falsified());
    const AmoAlo_board FC(F);
    const auto& B(F.board());
    assert(B.board({1,1}) == State::placed);
    assert(B.board({1,2}) == State::forbidden);
    assert(B.board({1,3}) == State::forbidden);
    assert(B.board({1,4}) == State::forbidden);
    assert(B.board({2,1}) == State::forbidden);
    assert(B.board({2,2}) == State::forbidden);
    assert(B.board({2,3}) == State::forbidden);
    assert(B.board({2,4}) == State::placed);
    assert(B.board({3,1}) == State::forbidden);
    assert(B.board({3,2}) == State::placed);
    assert(B.board({3,3}) == State::forbidden);
    assert(B.board({3,4}) == State::forbidden);
    assert(B.board({4,1}) == State::forbidden);
    assert(B.board({4,2}) == State::forbidden);
    assert(B.board({4,3}) == State::forbidden);
    assert(B.board({4,4}) == State::forbidden);
  }
}
