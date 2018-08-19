// Oliver Kullmann, 6.8.2018 (Swansea)

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
    const Var v11{1,1};
    assert(F.odegree(v11) == 3);
    TawHeuristics h(F);
    assert(h.heuristics(v11) == TawHeuristics::Bp(3*4.85, 2*4.85));
  }
  {
    const AmoAlo_board F(6);
    assert(F.N == 6);
    const Var v11{1,1};
    assert(F.odegree(v11) == 15);
    CountSat<AmoAlo_board, FirstOpenRandom> B(F.N);
    const auto stats = B(F);
    assert(stats.solutions == 4);
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
}
