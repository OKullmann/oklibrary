// Oliver Kullmann, 6.7.2018 (Swansea)

#include <utility>
#include <math.h>

namespace Backtracking {

  template <class ActiveClauseSet, class Branching, class Count_t>
  struct CountSat {
    typedef ActiveClauseSet ACLS;
    typedef typename ACLS::Var Var;
    typedef Count_t count_t;

    Count_t num_nodes = 0;

    count_t operator()(ACLS F) {
      ++num_nodes;
      if (F.satisfied()) return pow(2,F.n() - F.nset());
      if (F.falsified()) return 0;
      const Var bv = Branching(F)();
      ACLS G(F); G.set(bv, false);
      const count_t count0 = operator()(std::move(G));
      F.set(bv, true);
      return count0 + operator()(std::move(F));
    }

  };

}

