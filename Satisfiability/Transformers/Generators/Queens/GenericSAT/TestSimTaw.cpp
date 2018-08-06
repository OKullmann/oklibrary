// Oliver Kullmann, 6.8.2018 (Swansea)

#include <cassert>

#include "Backtracking.hpp"
#include "NQueens.hpp"

int main() {
  using namespace NQueens;
  {
    AmoAlo_board F(1);
    assert(F.N == 1);
    assert(not F.satisfied());
    assert(not F.falsified());
    assert(F.n() == 1);
    assert(F.nset() == 0);
    assert(F.open({1,1}));
    const AmoAlo_board::Board b {{State::open, State::open}, {State::open, State::open}};
    assert(F.board() == b);
    {
      const AmoAlo_board FC(F);
      assert(FC.board({1,1}) == State::open);
    }
  }
  {
    NQueens::AmoAlo_board F(2);
    assert(F.N == 2);
  }
}
