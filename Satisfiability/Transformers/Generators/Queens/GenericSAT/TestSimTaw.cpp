// Oliver Kullmann, 6.8.2018 (Swansea)

#include <iostream>

#include <cassert>

#include "Backtracking.hpp"
#include "NQueens.hpp"

int main() {
  using namespace NQueens;
  using namespace ChessBoard;
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
    NQueens::AmoAlo_board F(2);
    assert(F.N == 2);
    const Var v11{1,1};
    assert(F.odegree(v11) == 3);
    TawHeuristics h(F);
    assert(h.heuristics(v11) == TawHeuristics::Bp(3*4.85, 2*4.85));
  }
}
