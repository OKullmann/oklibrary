// Oliver Kullmann, 6.7.2018 (Swansea)

/* TODOS

1. Add parallelisation

   Simply using powers of 2 as the number of parallel computations, and
   parallelising by depth accordingly.

*/

#include <utility>
#include <ostream>
#include <algorithm>

#include <cmath>
#include <cassert>

#include "ChessBoard.hpp"

namespace Backtracking {

  struct Statistics {
    using Count_t = ChessBoard::Count_t;
    using Var_uint = ChessBoard::Var_uint;
    Count_t solutions;
    Count_t nodes;
    Var_uint height;
    Count_t maxusat_nodes; // maximum size of subtree with unsatisfiable root
    Count_t maxsat_nodes; // maximum size of subtree with no unsatisfiable node
    Var_uint hs;
  };
  static_assert(std::is_pod<Statistics>::value, "Statistics is not POD.");
  std::ostream& operator <<(std::ostream& out, const Statistics& stats) {
    return out <<
         "c solutions                             " << stats.solutions << "\n"
         "c nodes                                 " << stats.nodes << "\n"
         "c height                                " << stats.height << "\n"
         "c max_unodes                            " << stats.maxusat_nodes << "\n"
         "c max_snodes                            " << stats.maxsat_nodes << "\n"
         "c HortonStrahler                        " << stats.hs << "\n";
  }

  template <class ActiveClauseSet, class Branching>
  struct CountSat {
    using ACLS = ActiveClauseSet;

    Statistics operator()(ACLS F) const {
      Statistics stats{0,1,0,0,0,0};
      if (F.satisfied()) {
        stats.solutions = std::pow(2, F.n() - F.nset());
        stats.maxsat_nodes = 1;
        return stats;
      }
      if (F.falsified()) {
        stats.maxusat_nodes = 1;
        return stats;
      }
      const ChessBoard::Var bv = Branching(F)();
      assert(not ChessBoard::singular(bv));
      ACLS G(F); G.set(bv, false);
      const Statistics stats0 = operator()(std::move(G));
      stats.solutions += stats0.solutions;
      stats.nodes +=stats0.nodes;
      F.set(bv, true);
      const Statistics stats1 = operator()(std::move(F));
      stats.solutions += stats1.solutions;
      stats.nodes +=stats1.nodes;
      stats.height = std::max(stats0.height, stats1.height) + 1;
      if (stats0.solutions == 0 and stats1.solutions == 0)
        stats.maxusat_nodes = 1 + stats0.maxusat_nodes + stats1.maxusat_nodes;
      else
        stats.maxusat_nodes = std::max(stats0.maxusat_nodes, stats1.maxusat_nodes);
      if (stats0.maxusat_nodes == 0 and stats1.maxusat_nodes == 0)
        stats.maxsat_nodes = 1 + stats0.maxsat_nodes + stats1.maxsat_nodes;
      else
        stats.maxsat_nodes = std::max(stats0.maxsat_nodes, stats1.maxsat_nodes);
      if (stats0.hs == stats1.hs) stats.hs = stats0.hs + 1;
      else stats.hs = std::max(stats0.hs, stats1.hs);
      return stats;
    }

  };

}

