// Oliver Kullmann, 6.7.2018 (Swansea)

#include <utility>
#include <math.h>

#include "ChessBoard.hpp"

namespace Backtracking {

  template <class ActiveClauseSet, class Branching>
  struct CountSat {
    using ACLS = ActiveClauseSet;

    ChessBoard::Count_t num_nodes = 0;

    ChessBoard::Count_t operator()(ACLS F) {
      ++num_nodes;
      if (F.satisfied()) return pow(2,F.n() - F.nset());
      if (F.falsified()) return 0;
      const ChessBoard::Var bv = Branching(F)();
      ACLS G(F); G.set(bv, false);
      const ChessBoard::Count_t count0 = operator()(std::move(G));
      F.set(bv, true);
      return count0 + operator()(std::move(F));
    }

  };

}

