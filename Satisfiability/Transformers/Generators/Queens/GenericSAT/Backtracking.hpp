// Oliver Kullmann, 6.7.2018 (Swansea)
/* Copyright 2018, 2019, 2020 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  - class template Statistics<T>
  - binary operator + and functions satstats, unsatstats for creation
    of Statistics-objects

  - prototype EmptyUSAT for unsat-tests
  - class NotEnoughDiags

  - CountSat<ACLS, Branching, Tree, USAT, Statistics> for branching
    on fields (boolean variables -- placing a queen versus forbidding
    a field)

  - class StatisticsRC (only branching on rows and columns)
  - typedef StatisticsRC_v for a vector of StatisticsRC's
  - functions, sum(Statistics_v) satstatsrc, unsatstatsrc, cachestatsrc
    for creation of StatisticsRC-objects
  - class GlobalStatsRC for additional global statistics (computed after
    the tree has been completed)
  - class FullStatsRC comprising both types of statistics

  - prototype EmptyCACHING for caching-functionality
  - class CountSatRC<ACLS, Branching, CACHING, USAT>

*/

/* TODOS

1. Add parallelisation

   REVISE the following in the light of
   CubeAndConquer.cpp.

   1. Simply using powers of 2 as the number of parallel computations, and
      parallelising by depth accordingly, that is the basic idea.

      Basic code:

        #include <future>

        ACLS G(F); G.set(bv, false);
        auto handle0 = std::async(std::launch::async, [&G, this]{ return this->operator()(std::move(G)); });
        F.set(bv, true);
        const Statistics stats1 = operator()(std::move(F));
        const Statistics stats0 = handle0.get();

        stats.solutions += stats0.solutions + stats1.solutions;

      In Programming/Teaching/PhilosophicalCpp/Part02_ThreadParallelism
      examples for implementing parallelisms are given.

      Here, in "C&C" fashion, the vector of all computations is computed
      completely in the beginning (the Cube-phase), just as instances of
      (similar to) CountSat, with the residual ACLS.

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
   5. For CountSatRC see the todo in Caching.hpp.

2. Collecting data

   1. We have summary-statistics, handled by class Statistics.
   2. We should also output the command-line parameters, which likely should
      be the responsibility of ExpQueens.cpp. For the different heuristics
      and active clause-sets, we also need identifying abbreviations.
   3. We need also some summary on inferences.
   4. In general, the active clause-set should provide the "measure"-object,
      the branching-function should provide the "distance"-object.
   5. The summary-statistics considers these numbers, and perhaps always
      shows a standard statistical evaluation:
       min, max, arithmetical mean, standard deviation.

*/

#ifndef BACKTRACKING_Abfe1fM3Q7
#define BACKTRACKING_Abfe1fM3Q7

#include <utility>
#include <ostream>
#include <algorithm>
#include <type_traits>

#include <cmath>
#include <cassert>

#include <ProgramOptions/Environment.hpp>
#include <Numerics/FloatingPoint.hpp>

#include "ChessBoard.hpp"
#include "Trees.hpp"

namespace Backtracking {

  template <class T>
  struct Statistics : T {
    using Count_t = ChessBoard::Count_t;
    using Var_uint = ChessBoard::Var_uint;
    Count_t solutions;
    Count_t nodes;
    Var_uint height;
    Count_t maxusat_nodes; // maximum size of subtree with unsatisfiable root
    Count_t maxsat_nodes; // maximum size of subtree with no unsatisfiable node
    Var_uint hs;
  };
  template <class T>
  inline constexpr Statistics<T> operator +(const Statistics<T>& s1, const Statistics<T>& s2) noexcept {
    Statistics<T> s{};
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
    if constexpr (not std::is_empty_v<T>) s.combine(s1, s2);
    return s;
  }
  template <class T>
  inline constexpr Statistics<T> satstats(const typename Statistics<T>::Var_uint n, const typename Statistics<T>::Var_uint nset) noexcept {
    return {{}, typename Statistics<T>::Count_t(std::pow(2, n - nset)), 1, 0, 0, 1, 0};
  }
  template <class T>
  inline constexpr Statistics<T> unsatstats(T v = T()) noexcept {
    return {v, 0, 1, 0, 1, 0, 0};
  }
  template <class T>
  std::ostream& operator <<(std::ostream& out, const Statistics<T>& stats) {
    using Environment::DHW;
    using Environment::DWW;
    out << DHW{"Results"}
        << DWW{"solutions"} << stats.solutions << "\n"
        << DWW{"nodes"} << stats.nodes << "\n"
        << DWW{"height"} << stats.height << "\n"
        << DWW{"max_unodes"} << stats.maxusat_nodes << "\n"
        << DWW{"max_snodes"} << stats.maxsat_nodes << "\n"
        << DWW{"HortonStrahler"} << stats.hs << "\n"
        << DWW{"q=leaves/sols"} << std::defaultfloat << double(stats.nodes+1) / 2 / (stats.solutions) << "\n";
         if constexpr (not std::is_empty_v<T>) stats.output(out);
    return out;
  }


  /* The concept of class X as UNSAT-test is:
   - static function test(const Board&) returns the decision-statistics,
     which is convertible to bool, being true if unsat was detected.
   - It the class contains no data, then indeed it is considered as no-op.
   - The decision-statistics-object can be output via member-function
     output.
   - combine(X,X) takes the statistics-objects from the left and right branch,
     and stores the combination-result in the object.
  */
  // Empty prototype for class providing additional unsat-tests:
  struct EmptyUSAT {
    //static bool test(const ChessBoard::Board&) noexcept { return false; }
    //void combine(EmptyUSAT, EmptyUSAT) noexcept {};
    //std::ostream& output(std::ostream&) const;
  };
  static_assert(std::is_empty_v<EmptyUSAT>);
  static_assert(is_pod(Statistics<EmptyUSAT>));

  // Simplest case of additional unsat-test: not enough (anti)diagonals:
  struct NotEnoughDiags {
    using Count_t = ChessBoard::Count_t;
    using Var_uint = ChessBoard::Var_uint;
    Count_t diag_unsat_count;
    NotEnoughDiags() : diag_unsat_count(0) {}
    NotEnoughDiags(const bool b) : diag_unsat_count(b) {}
    static bool test(const ChessBoard::Board& B) noexcept {
      const auto N = B.N - B.t_rank().p;
      {Var_uint open_d = 0;
       for (const auto r : B.d_rank())
         if ((open_d += bool(r.o)) >= N) goto antidiag;
       return true;
      }
      antidiag :
      Var_uint open_ad = 0;
      for (const auto r : B.ad_rank())
        if ((open_ad += bool(r.o)) >= N) return false;
      return true;
    }
    void combine(const NotEnoughDiags n1, const NotEnoughDiags n2) noexcept {
      diag_unsat_count = n1.diag_unsat_count + n2.diag_unsat_count;
    }
    void output(std::ostream& out) const {
      out << Environment::DWW{"not_enough_diags"} << diag_unsat_count << "\n";
    }
  };
  static_assert(not std::is_empty_v<NotEnoughDiags>);


  // Prototype for LeafActions:
  template <class ActiveClauseSet>
  struct NoOpLeaves {
    void sat(const ActiveClauseSet&) noexcept {}
    void unsat(const ActiveClauseSet&) noexcept {}
  };
  static_assert(std::is_empty_v<NoOpLeaves<void*>>);


  template <class ActiveClauseSet,
            class Branching_t,
            class Tree_t = Trees::NoOpTree,
            class USAT_test = EmptyUSAT,
            class Statistics_t = Backtracking::Statistics<USAT_test>,
            class LeafActions_t = NoOpLeaves<ActiveClauseSet>>
  struct CountSat {
    Tree_t T;
    LeafActions_t L;

    using ACLS = ActiveClauseSet;
    using Branching = Branching_t;
    using Tree = Tree_t;
    using USAT = USAT_test;
    using Statistics = Statistics_t;
    using LeafActions = LeafActions_t;

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
        L.sat(F);
        return satstats<USAT_test>(F.n(), F.nset());
      }
      if (F.falsified()) {
        T.add(root_info, NT::ul);
        L.unsat(F);
        return unsatstats<USAT_test>();
      }
      if constexpr (not std::is_empty_v<USAT>) {
        const auto usat_test{USAT::test(F.board())};
        if (usat_test) return unsatstats<USAT_test>(usat_test);
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


  struct StatisticsRC {
    using Count_t = ChessBoard::Count_t;
    using Var_uint = ChessBoard::Var_uint;
    Count_t solutions;
    Count_t nodes; // these are internal nodes (not leaves)
    Count_t leaves; // leaves = rs2 + r2u + cache_hits
    Var_uint height;
    Count_t maxusat_nodes;
    Var_uint hs;
    Count_t r2s;
    Count_t r2u;
    Count_t cache_hits;
    static Environment::OP op;
  };
  Environment::OP StatisticsRC::op;

  typedef std::vector<StatisticsRC> StatisticsRC_v;
  inline StatisticsRC sum(const StatisticsRC_v& v) noexcept {
    assert(not v.empty());
    StatisticsRC s{};
    bool some_sat = false;
    for (const auto& x : v) {
      if (x.solutions > 0) {
        s.solutions += x.solutions;
        some_sat = true;
      }
      s.nodes += x.nodes;
      s.leaves += x.leaves;
      s.height = std::max(s.height, x.height);
      s.hs = std::max(s.hs, x.hs);
      s.r2s += x.r2s;
      s.r2u += x.r2u;
      s.cache_hits += x.cache_hits;
    }
    ++s.height; ++s.nodes;
    StatisticsRC::Var_uint hs_width = 0;
    for (const auto& x : v) {
      if (x.hs == s.hs) ++hs_width;
      if (some_sat)
        s.maxusat_nodes = std::max(s.maxusat_nodes, x.maxusat_nodes);
      else s.maxusat_nodes += x.maxusat_nodes;
    }
    if (hs_width >= 2) ++s.hs;
    if (not some_sat) ++s.maxusat_nodes;
    return s;
  }
  inline constexpr StatisticsRC satstatsrc(const StatisticsRC::Var_uint n, const StatisticsRC::Var_uint nset) noexcept {
    StatisticsRC s{};
    s.solutions = std::pow(2, n-nset);
    s.r2s = 1;
    s.leaves = 1;
    return s;
  }
  inline constexpr StatisticsRC unsatstatsrc() noexcept {
    StatisticsRC s{};
    s.r2u = 1;
    s.leaves = 1;
    s.maxusat_nodes = 1;
    return s;
  }
  inline constexpr StatisticsRC cachestatsrc(const StatisticsRC::Count_t c) noexcept {
    StatisticsRC s{};
    s.leaves = 1;
    if (c != 0) s.solutions = c;
    else s.maxusat_nodes = 1;
    s.cache_hits = 1;
    return s;
  }
  std::ostream& operator <<(std::ostream& out, const StatisticsRC& s) {
    using Environment::OP;
    using FloatingPoint::float80;
    using FloatingPoint::Wrap;
    if (StatisticsRC::op == OP::dimacs) {
      using Environment::DHW;
      using Environment::DWW;
      out << DHW{"Results"}
          << DWW{"solutions"} << s.solutions << "\n"
          << DWW{"nodes"} << s.nodes << "\n"
          << DWW{"leaves"} << s.leaves << "\n"
          << DWW{"height"} << s.height << "\n"
          << DWW{"max_unodes"} << s.maxusat_nodes << "\n"
          << DWW{"HortonStrahler"} << s.hs << "\n"
          << DWW{"r2_sat"} << s.r2s << "\n"
          << DWW{"r2_unsat"} << s.r2u << "\n"
          << DWW{"cache_hits"} << s.cache_hits << "\n"
          << DWW{"q=leaves/sols"} << Wrap(float80(s.leaves) / (s.solutions)) << "\n";
    }
    else if (StatisticsRC::op == OP::explained) {
      out << "  ** Results **\n\n"
          << "  Number solutions: " << s.solutions << "\n"
          << "  Number nodes: " << s.nodes << "\n"
          << "  Number leaves: " << s.leaves << "\n"
          << "  Height of backtracking-tree: " << s.height << "\n"
          << "  Maximal number of nodes in unsatisfiable subtree: " << s.maxusat_nodes << "\n"
          << "  HortonStrahler number of backtracking-tree: " << s.hs << "\n"
          << "  Number of satisfying r2-reduction: " << s.r2s << "\n"
          << "  Number of falsifying r2_reductions: " << s.r2u << "\n"
          << "  Number of cache_hits: " << s.cache_hits << "\n"
          << "  Quotient leaves/sols: " << Wrap(float80(s.leaves) / (s.solutions)) << "\n";
    }
    else if (Environment::isR(StatisticsRC::op) or StatisticsRC::op == OP::simple) {
      out << " " << s.solutions << " " << s.nodes << " " << s.leaves <<
          " " << s.height << " " << s.maxusat_nodes << " " << s.hs <<
          " " << s.r2s << " " << s.r2u << " " << s.cache_hits << " " <<
          std::defaultfloat << Wrap(float80(s.leaves) / (s.solutions));
    }
    return out;
  }

  struct GlobalStatsRC {
    using Count_t = ChessBoard::Count_t;
    Count_t cache_size;
    double load_factor;
  };
  std::ostream& operator <<(std::ostream& out, const GlobalStatsRC& s) {
    using Environment::OP;
    if (StatisticsRC::op == OP::dimacs) {
      using Environment::DWW;
      out << DWW{"max_cache_size"} << s.cache_size << "\n"
          << DWW{"final_load_factor"} << s.load_factor << "\n";
    }
    else if (StatisticsRC::op == OP::explained) {
      out << "  Final number of elements in cache: " << s.cache_size << "\n"
          << "  Final load-factor: " << s.load_factor << "\n";
    }
    else if (Environment::isR(StatisticsRC::op) or StatisticsRC::op == OP::simple) {
      out << " " << s.cache_size << " " << s.load_factor;
    }
    return out;
  }

  struct FullStatsRC {
    StatisticsRC s;
    GlobalStatsRC gs;
  };
  std::ostream& operator <<(std::ostream& out, const FullStatsRC& fs) {
    return out << fs.s << fs.gs;
  }


  // Empty prototype of class providing caching-functionality:
  struct EmptyCACHING {
    typedef ChessBoard::Count_t size_t;
    static size_t size() {return 0;}
    static double load_factor() {return 0;}
    static void init(ChessBoard::coord_t) {}
    // types cache_t, return_t = std::optional<Count_t>
    // static functions:
    //  - void init(ChessBoard::coord_t N)
    //  - cache_t hash(const ChessBoard::Board&)
    //  - return_t find(const cache_t&, const ChessBoard::Board&);
    //  - bool insert(const chache_t&, ChessBoard::Count_t)
  };
  static_assert(std::is_empty_v<EmptyCACHING>);
  static_assert(is_pod(Statistics<EmptyCACHING>));

  template <class ActiveClauseSet, class Branching_t, class CACHING_function = EmptyCACHING, class USAT_test = EmptyUSAT>
  struct CountSatRC {
    using ACLS = ActiveClauseSet;
    using Branching = Branching_t;
    using USAT = USAT_test;
    using CACHING = CACHING_function;
    using Count_t = ChessBoard::Count_t;

    using coord_t = typename ACLS::coord_t;
    using Var = typename ACLS::Var;

    static constexpr bool use_caching = not std::is_same_v<CACHING,EmptyCACHING>;

    const coord_t N;
    const ACLS F;
    CountSatRC(const coord_t N) : N(N), F(N) {};

    FullStatsRC operator()() const {
      CACHING::init(N);
      const auto res = operator()(F);
      const GlobalStatsRC gs{CACHING::size(), CACHING::load_factor()};
      return {res, gs};
    }
    // Invariant: F.satisfied() = F.falsified() = false, and also
    // USAT::test yields false, if applicable.
    StatisticsRC operator()(const ACLS& F) const {
      const auto [index, row] = Branching(F)();
      assert(1 <= index and index <= N);
      StatisticsRC_v stats;
      if (row) {
        assert(F.board().r_rank(index).o >= 1);
        const auto& R = F.board()()[index];
        for (coord_t j = 1; j <= N; ++j) {
          if (R[j] != ChessBoard::State::open) continue;
          ACLS G(F);
          G.set({index, j}, true);
          branch(G, stats);
        }
      }
      else {
        assert(F.board().c_rank(index).o >= 1);
        for (coord_t i = 1; i <= N; ++i) {
          const Var bv{i, index};
          if (not F.board().open(bv)) continue;
          ACLS G(F);
          G.set(bv, true);
          branch(G, stats);
        }
      }
      return sum(stats);
    }

  private :

    void branch(const ACLS& G, StatisticsRC_v& stats) const {
      if (G.satisfied()) {
        stats.push_back(satstatsrc(G.n(), G.nset()));
        return;
      }
      if (G.falsified()) {
        stats.push_back(unsatstatsrc());
        return;
      }
      if constexpr (not std::is_empty_v<USAT>) {
        if (USAT::test(G.board())) {
          stats.push_back(unsatstatsrc());
          return;
        }
      }
      if constexpr (use_caching) {
        const auto hash = CACHING::hash(G.board());
        if (const auto found = CACHING::find(hash, G.board()))
          stats.push_back(cachestatsrc(*found));
        else {
          const auto s = operator()(G);
          [[maybe_unused]] const bool inserted =
            CACHING::insert(hash, s.solutions);
          assert(inserted);
          stats.push_back(s);
        }
      }
      else stats.push_back(operator()(G));
    }

  };


}

#endif
