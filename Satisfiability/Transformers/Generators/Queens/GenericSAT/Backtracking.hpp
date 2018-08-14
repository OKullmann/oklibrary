// Oliver Kullmann, 6.7.2018 (Swansea)

#include <utility>
#include <ostream>

#include <cmath>
#include <cassert>

#include "ChessBoard.hpp"

namespace Backtracking {

  struct Statistics {
    ChessBoard::Count_t nodes;
    ChessBoard::Var_uint height;
  };
  static_assert(std::is_pod<Statistics>::value, "Statistics is not POD.");
  std::ostream& operator <<(std::ostream& out, const Statistics& stats) {
    return out << "c nodes      " << stats.nodes << "\n"
               << "c height     " << stats.height << "\n";
  }

  template <class ActiveClauseSet, class Branching>
  struct CountSat {
    using ACLS = ActiveClauseSet;

    Statistics stats{};

    ChessBoard::Count_t operator()(ACLS F) {
      ++stats.nodes;
      if (F.satisfied()) return std::pow(2, F.n() - F.nset());
      if (F.falsified()) return 0;
      const ChessBoard::Var bv = Branching(F)();
      assert(not ChessBoard::singular(bv));
      ACLS G(F); G.set(bv, false);
      const ChessBoard::Count_t count0 = operator()(std::move(G));
      F.set(bv, true);
      return count0 + operator()(std::move(F));
    }

  };

}

