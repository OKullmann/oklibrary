// Oliver Kullmann, 6.8.2018 (Swansea)
/* Copyright 2018, 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* TODOS:

1. At place XXX provide some basic explanation

One should be able to see without pen-and-paper what the test tests.
So in this case an overview on what the tree is should be given (precise
and concise).

2. Design:

    - Split this file into TestXXX.cpp:
     - TestBacktracking.cpp
     - TestChessBoard.cpp
     - TestColour.cpp
     - TestHeuristics.cpp
     - TestNQueens.cpp
     - TestTrees.cpp

    - List of dependencies in Makefile:
     - TestBacktracking.cpp: Backtracking.hpp NQueens.hpp
     - TestChessBoard.cpp: ChessBoard.hpp
     - TestColour.cpp: Colour.hpp
     - TestHeuristics.cpp: Heuristics.hpp NQueens.hpp
     - TestNQueens.cpp: NQueens.hpp
     - TestTrees.cpp: Trees.hpp

     If we do not automatically extract the dependencies, then having
     specialised dependencies per TestXXX.cpp is in the way of treating
     them automatically (see below) -- but possibly this can be handled
     by associative arrays or be dedicated dependency-files (see below).

    - How to expand the Makefile for the many TestXXX.cpp?
     - We should not list them all separately, since this would be
       too many.
     - TestSimTaw is the first role model.
     - One seens that it has special options (here to disable a warning):
       how to handle this? One needed the possibility to create additional
       options for each header-file separately. A kind of map is needed.
       One might try https://gmsl.sourceforge.io/ , which seems to provide
       associative arrays to be used in makefiles.
     - However this creates another dependency: better, as already used
       in the OKlibrary, seems to use special files, like TestXXX.d and
       variations, best in a subdirectory "deps", where then per file
       variables are set.
     - Likely we have a new makefile-variable "headerfiles", where then
       automatically for each element the corresponding TestXXX.cpp is
       created.
     - Currently there are targets "optimised" and "debug", where TestSimTaw
       belongs to "debug": this needs to be redesigned.

*/

#include <sstream>
#include <stdexcept>
#include <numeric>
#include <string>

#include <cassert>

#include "Backtracking.hpp"
#include "NQueens.hpp"
#include "Heuristics.hpp"
#include "Trees.hpp"
#include "Colour.hpp"

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
    assert((B() == Board::Board_t{{State::open, State::open}, {State::open, State::placed}}));
    {
      const AmoAlo_board FC(F);
      assert(FC.board()({1,1}) == State::placed);
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
    assert(BC({1,1}) == State::placed);
    assert(BC({1,2}) == State::forbidden);
    assert(BC({1,3}) == State::forbidden);
    assert(BC({1,4}) == State::forbidden);
    assert(BC({2,1}) == State::forbidden);
    assert(BC({2,2}) == State::forbidden);
    assert(BC({2,3}) == State::open);
    assert(BC({2,4}) == State::open);
    assert(BC({3,1}) == State::forbidden);
    assert(BC({3,2}) == State::open);
    assert(BC({3,3}) == State::forbidden);
    assert(BC({3,4}) == State::open);
    assert(BC({4,1}) == State::forbidden);
    assert(BC({4,2}) == State::open);
    assert(BC({4,3}) == State::open);
    assert(BC({4,4}) == State::forbidden);
  }
  {
    // Falsified due to amo propagation:
    AmoAlo_board F(3);
    const Var v{2,2};
    F.set(v,true);
    assert(F.falsified());
    const auto& B{F.board()};
    assert(B(v) == State::placed);
    assert((B.t_rank() == TotalRank{0,1,8}));
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
    const auto& B = F.board();
    ChessBoard::Diagonal d = B.diagonal({1,1});
    assert(d.s.first == 1);
    assert(d.s.second == 1);
    assert(d.l == 5);
    d = B.diagonal({1,3});
    assert(d.s.first == 1);
    assert(d.s.second == 3);
    assert(d.l == 3);
    d = B.diagonal({4,2});
    assert(d.s.first == 3);
    assert(d.s.second == 1);
    assert(d.l == 3);
    d = B.diagonal({2,5});
    assert(d.s.first == 1);
    assert(d.s.second == 4);
    assert(d.l == 2);
    d = B.diagonal({5,5});
    assert(d.s.first == 1);
    assert(d.s.second == 1);
    assert(d.l == 5);
  }
  {
    NQueens::AmoAlo_board F(5);
    const auto& B = F.board();
    ChessBoard::AntiDiagonal ad = B.anti_diagonal({1,1});
    assert(ad.s.first == 1);
    assert(ad.s.second == 1);
    assert(ad.l == 1);
    ad = B.anti_diagonal({1,3});
    assert(ad.s.first == 1);
    assert(ad.s.second == 3);
    assert(ad.l == 3);
    ad = B.anti_diagonal({4,2});
    assert(ad.s.first == 1);
    assert(ad.s.second == 5);
    assert(ad.l == 5);
    ad = B.anti_diagonal({2,5});
    assert(ad.s.first == 2);
    assert(ad.s.second == 5);
    assert(ad.l == 4);
    ad = B.anti_diagonal({5,5});
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
    /*
    assert((FirstOpenRandom::permutation() == FirstOpenRandom::varvec_t{{1,1},{6,3},{1,6},{2,2},{1,5},{1,4},{3,1},{2,3},{6,6},{4,4},{6,4},{3,2},{6,1},{2,5},{5,6},{5,2},{6,2},{4,2},{4,1},{6,5},{4,6},{5,4},{1,2},{3,3},{3,6},{4,3},{4,5},{3,4},{5,3},{3,5},{5,5},{5,1},{2,6},{2,4},{2,1},{1,3}}));
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
    assert(stats3.nodes == 47);*/
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
    assert((enum_square(3) == varvec_t{{1,1},{2,1},{2,2},{1,2},{3,1},{3,2},{3,3},{2,3},{1,3}}));
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
  {// XXX Explanations missing
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
    assert((T.tree() == Tree{{0,0},{2,3},{0,0},{0,0}}));
    assert((T.nodetypes() == NodeType_v{NodeType::undef, NodeType::si, NodeType::ul, NodeType::sl}));
    std::stringstream out;
    output(out, T, "OK", "Test");
    std::string s1, s2, s3, s4;
    out >> s1 >> s2;
    assert(s1 == "(tlp" and s2 == "\"2.3\"");
    out >> s1 >> s2;
    assert(s1 == "(date");
    out >> s1 >> s2;
    assert(s1 == "(author" and s2 == "\"OK\")");
    out >> s1 >> s2;
    assert(s1 == "(comments" and s2 == "\"Test\")");
    out >> s1 >> s2;
    assert(s1 == "(nb_nodes" and s2 == "3)");
    out >> s1 >> s2;
    assert(s1 == "(nodes" and s2 == "0..2)");
    out >> s1 >> s2 >> s3 >> s4;
    assert(s1 == "(edge" and s2 == "0" and s3 == "0" and s4 == "1)");
    out >> s1 >> s2 >> s3 >> s4;
    assert(s1 == "(edge" and s2 == "1" and s3 == "0" and s4 == "2)");
    out >> s1 >> s2 >> s3 >> s4;
    assert(s1 == "(property" and s2 == "0" and s3 == "int" and s4 == "\"type\"");
    out >> s1 >> s2 >> s3;
    assert(s1 == "(default" and s2 == "\"0\"" and s3 == "\"0\")");
    out >> s1 >> s2 >> s3;
    assert(s1 == "(node" and s2 == "0" and s3 == "\"3\")");
    out >> s1 >> s2 >> s3;
    assert(s1 == "(node" and s2 == "1" and s3 == "\"2\")");
    out >> s1 >> s2 >> s3 >> s4;
    assert(s1 == "(node" and s2 == "2" and s3 == "\"1\")" and s4 == ")");
    out >> s1 >> s2 >> s3 >> s4;
    assert(s1 == "(property" and s2 == "0" and s3 == "color" and s4 == "\"viewColor\"");
    out >> s1 >> s2 >> s3;
    assert(s1 == "(default" and s2 == "\"(0,0,0,0)\"" and s3 == "\"(0,0,0,0)\")");
    out >> s1 >> s2 >> s3;
    assert(s1 == "(node" and s2 == "0" and s3 == "\"(255,255,0,255)\")");
    out >> s1 >> s2 >> s3;
    assert(s1 == "(node" and s2 == "1" and s3 == "\"(0,0,0,255)\")");
    out >> s1 >> s2 >> s3;
    assert(s1 == "(node" and s2 == "2" and s3 == "\"(255,255,0,255)\")");
    out >> s1 >> s2 >> s3;
    assert(s1 == "(edge" and s2 == "0" and s3 == "\"(0,0,0,255)\")");
    out >> s1 >> s2 >> s3;
    assert(s1 == "(edge" and s2 == "1" and s3 == "\"(255,255,0,255)\")");
    out >> s1 >> s2;
    assert(s1 == ")" and s2 == ")");
    assert(out.eof() == false);
    out >> s1;
    assert(out.eof() == true);
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
    assert(B({1,1}) == State::forbidden);
    assert(B({1,2}) == State::forbidden);
    assert(B({1,3}) == State::forbidden);
    assert(B({2,1}) == State::forbidden);
    assert(B({2,2}) == State::forbidden);
    assert(B({2,3}) == State::placed);
    assert(B({3,1}) == State::open);
    assert(B({3,2}) == State::open);
    assert(B({3,3}) == State::forbidden);
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
    assert(B({1,1}) == State::placed);
    assert(B({1,2}) == State::forbidden);
    assert(B({1,3}) == State::forbidden);
    assert(B({1,4}) == State::forbidden);
    assert(B({2,1}) == State::forbidden);
    assert(B({2,2}) == State::forbidden);
    assert(B({2,3}) == State::forbidden);
    assert(B({2,4}) == State::placed);
    assert(B({3,1}) == State::forbidden);
    assert(B({3,2}) == State::placed);
    assert(B({3,3}) == State::forbidden);
    assert(B({3,4}) == State::forbidden);
    assert(B({4,1}) == State::forbidden);
    assert(B({4,2}) == State::forbidden);
    assert(B({4,3}) == State::forbidden);
    assert(B({4,4}) == State::forbidden);
  }
  {
    using namespace Colour;
    assert(*(Colour3{}) == rgb3_t{});
    assert(*(Colour3()) == rgb3_t{});
    constexpr Colour3 all0c{};
    static_assert((*all0c).size() == 3);
    static_assert(all0c -> size() == 3);
    static_assert(all0c[0] == 0);
    static_assert(all0c.g() == 0);
    constexpr rgb3_t all0 = *all0c;
    assert(all0 == rgb3_t{});
    Colour3 x{};
    (*x)[0] = 1;
    x[1] = 2;
    assert(*x != rgb3_t{});
    assert(x.r() == 1);
    assert(x.g() == 2);
    assert(x.b() == 0);
    assert(x[0] == 1);
    assert(x[1] == 2);
    assert(x[2] == 0);
    assert(std::accumulate(white3->begin(),white3->end(),0) == 765);
    unsigned int sum = 0;
    for (const auto c : *x) sum += c;
    assert(sum == 3);
    x = Colour3(2,3,4);
    for (auto i = x -> crbegin(); i != x -> crend(); ++i) sum += *i;
    assert(sum == 3 + (2+3+4));
    x[1] = 77;
    assert(x.g() == 77);
    x = Colour3(rgb3_t{11,22,33});
    assert(x[2] == 33);
    Colour3 y; // y undefined
    y = {3,4,5};
    assert(x != y);
    const auto [y0,y1,y2] = *y;
    assert(y0==3 and y1==4 and y2==5);
    assert(y->at(0) == 3);
    bool was_thrown = false;
    try { y->at(3); }
    catch(const std::out_of_range&) { was_thrown = true; }
    assert(was_thrown);
  }
  {
    using namespace Colour;
    assert(*(Colour4{}) == rgb4_t{});
    assert(*(Colour4()) == rgb4_t{});
    constexpr Colour4 all0c{};
    static_assert((*all0c).size() == 4);
    static_assert(all0c -> size() == 4);
    static_assert(all0c[0] == 0);
    static_assert(all0c.g() == 0);
    constexpr rgb4_t all0 = *all0c;
    assert(all0 == rgb4_t{});
    Colour4 x{};
    (*x)[0] = 1;
    x[1] = 2;
    x[3] = 3;
    assert(*x != rgb4_t{});
    assert(x.r() == 1);
    assert(x.g() == 2);
    assert(x.b() == 0);
    assert(x.a() == 3);
    assert(x[0] == 1);
    assert(x[1] == 2);
    assert(x[2] == 0);
    assert(x[3] == 3);
    unsigned int sum = 0;
    for (const auto c : *x) sum += c;
    assert(sum == 6);
    x = Colour4(2,3,4,5);
    for (auto i = x -> crbegin(); i != x -> crend(); ++i) sum += *i;
    assert(sum == 6 + (2+3+4+5));
    x[1] = 77;
    assert(x.g() == 77);
    x = Colour4(rgb3_t{11,22,33});
    assert(x[2] == 33);
    assert(x[3] == 255);
    Colour4 y; // y undefined
    y = {3,4,5,0};
    assert(x != y);
    const auto [y0,y1,y2,y3] = *y;
    assert(y0==3 and y1==4 and y2==5 and y3 ==0);
    assert(y->at(0) == 3);
    bool was_thrown = false;
    try { y->at(4); }
    catch(const std::out_of_range&) { was_thrown = true; }
    assert(was_thrown);
    x = Colour3(10,20,30);
    assert(x == Colour4(10,20,30,255));
    assert(Colour3(x) == Colour3(10,20,30));
    assert(yellow(Colour4(yellow3)));
    const Colour3_v v1{{1,2,3},{4,5,6}};
    assert(v1.size() == 2);
    assert(v1[0] == Colour3(1,2,3));
    const Colour4_v v2{{3,4,5,22},{2,3,4,0},Colour3{34,45,56}};
    assert(v2.size() == 3);
    assert(v2[1] == Colour4(2,3,4,0));
    assert(v2[2] == Colour4(34,45,56,-1));
  }
}
