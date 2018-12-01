// Oliver Kullmann, 6.7.2018 (Swansea)
/* Copyright 2018 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* TODOS

1. Add parallelisation

   1. Simply using powers of 2 as the number of parallel computations, and
      parallelising by depth accordingly, that is the basic idea.

      Basic code:

        #include <future>

        ACLS G(F); G.set(bv, false);
        auto handle0 = std::async(std::launch::async, [&G, this](){ return this->operator()(std::move(G)); });
        F.set(bv, true);
        const Statistics stats1 = operator()(std::move(F));
        const Statistics stats0 = handle0.get();

        stats.solutions += stats0.solutions + stats1.solutions;

      In Programming/Teaching/PhilosophicalCpp/Part02_ThreadParallelism
      examples for implementing parallelisms are given.

   2. Currently the two branches actually have to be performed in order,
      due to
              const auto right_index = T.index()+1;
      The main problem here is the access to the global variable T, which
      needed synchronisation.
   3. So first the output of the tree-information needs to be improved/changed,
      so that parallel construction becomes possible.
   4. Likely it is best to introduce a policy-argument (a template parameter)
      about parallelisation. If turned on, then we assume that only relatively
      small trees are handled, and then a different tree-handling might be
      used, which uses a traditional node-based approach, which is inherently
      parallelisable.

*/

#include <utility>
#include <ostream>
#include <algorithm>
#include <type_traits>

#include <cmath>
#include <cassert>

#include "ChessBoard.hpp"
#include "Trees.hpp"

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
  static_assert(std::is_pod_v<Statistics>);
  inline constexpr Statistics operator +(const Statistics& s1, const Statistics& s2) noexcept {
    Statistics s{};
    s.solutions = s1.solutions + s2.solutions;
    s.nodes = 1 + s1.nodes + s2.nodes;
    s.height = std::max(s1.height, s2.height) + 1;
    if (s1.solutions == 0 and s2.solutions == 0)
      s.maxusat_nodes = 1 + s1.maxusat_nodes + s2.maxusat_nodes;
    else
      s.maxusat_nodes = std::max(s1.maxusat_nodes, s2.maxusat_nodes);
    if (s1.maxusat_nodes == 0 and s2.maxusat_nodes == 0)
      s.maxsat_nodes = 1 + s1.maxsat_nodes + s2.maxsat_nodes;
    else
      s.maxsat_nodes = std::max(s1.maxsat_nodes, s2.maxsat_nodes);
    if (s1.hs == s2.hs) s.hs = s1.hs + 1;
    else s.hs = std::max(s1.hs, s2.hs);
    return s;
  }
  inline constexpr Statistics satstats(const Statistics::Var_uint n, const Statistics::Var_uint nset) noexcept {
    return {Statistics::Count_t(std::pow(2, n - nset)), 1, 0, 0, 1, 0};
  }
  constexpr Statistics unsatstats{0, 1, 0, 1, 0, 0};
  std::ostream& operator <<(std::ostream& out, const Statistics& stats) {
    return out <<
         "c solutions                             " << stats.solutions << "\n"
         "c nodes                                 " << stats.nodes << "\n"
         "c height                                " << stats.height << "\n"
         "c max_unodes                            " << stats.maxusat_nodes << "\n"
         "c max_snodes                            " << stats.maxsat_nodes << "\n"
         "c HortonStrahler                        " << stats.hs << "\n";
  }


  template <class ActiveClauseSet, class Branching, class Tree = Trees::NoOpTree>
  struct CountSat {
    Tree T;
    using ACLS = ActiveClauseSet;
    using coord_t = typename ACLS::coord_t;
    using Var = typename ACLS::Var;

    CountSat() = default;

    CountSat(const coord_t N) { Branching::init(N); }
    template <class Binit>
    CountSat(Binit bi) {
      Branching::init(bi);
    }
    template <class Binit>
    CountSat(const coord_t N, const Binit& bi) {
      Branching::init(N,bi);
    }

    Statistics operator()(ACLS F) {
      const auto root_info = T.root_info();
      using NT = Trees::NodeType;
      if (F.satisfied()) {
        T.add(root_info, NT::sl);
        return satstats(F.n(), F.nset());
      }
      if (F.falsified()) {
        T.add(root_info, NT::ul);
        return unsatstats;
      }
      const Var bv = Branching(F)();
      assert(not ChessBoard::singular(bv));

      ACLS G(F); G.set(bv, false);
      const Statistics stats0 = operator()(std::move(G));
      F.set(bv, true);
      const auto after_left_info = T.after_left_info(root_info);
      const Statistics stats1 = operator()(std::move(F));

      T.add(root_info, after_left_info,
        (stats0.solutions == 0 and stats1.solutions == 0) ? NT::ui : NT::si);
      return stats0 + stats1;
    }

  };

}
