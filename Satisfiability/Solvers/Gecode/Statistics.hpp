// Oleg Zaikin, 28.10.2021 (Irkutsk)
/* Copyright 2021 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#ifndef STATISTICS_ouJtrmvkbM
#define STATISTICS_ouJtrmvkbM

#include <gecode/search.hh>

#include <Numerics/Statistics.hpp>

namespace Statistics {

  typedef std::uint64_t count_t;

  // XXX Specification XXX
  struct SearchStat {
    count_t nodes;
    count_t inner_nodes;
    count_t inner_nodes_2chld;
    count_t inner_nodes_3chld;
    count_t unsat_leaves;
    count_t solutions;
    count_t single_child_brnch;
    GenStats::StatsStore<float_t, float_t> choice_time;
    GenStats::StatsStore<float_t, float_t> tau_time;
    GenStats::StatsStore<float_t, float_t> subproblem_time;
    Gecode::Search::Statistics gecode_stat;

    SearchStat() : nodes(0), inner_nodes(0), inner_nodes_2chld(0),
                   inner_nodes_3chld(0), unsat_leaves(0),
                   solutions(0), single_child_brnch(0), choice_time(),
                   tau_time(), subproblem_time(), gecode_stat() {}

    bool valid() const noexcept {
      return (unsat_leaves + solutions + inner_nodes == nodes)
             and (inner_nodes + single_child_brnch == choice_time.N());
    }

    // XXX ??? remove XXX
    void reset() noexcept {
      assert(valid());
      nodes = inner_nodes = unsat_leaves = solutions = 0;
      inner_nodes_2chld = inner_nodes_3chld = single_child_brnch = 0;
      choice_time = GenStats::StatsStore<float_t, float_t>();
      tau_time = GenStats::StatsStore<float_t, float_t>();
      subproblem_time = GenStats::StatsStore<float_t, float_t>();
    }

    // XXX Use a proper class, make all data members private, and make all
    // updating-etc automatic (so this should become private) XXX
    void update_nodes() noexcept {
      unsat_leaves = std::max(unsat_leaves, gecode_stat.fail);
      nodes = inner_nodes + unsat_leaves + solutions;
      assert(valid());
    }

    void update_choice_stat(const double t) noexcept {
      choice_time += t;
    }
    void update_tau_stat(const double t) noexcept {
      tau_time += t;
    }
    void update_subproblem_stat(const double t) noexcept {
      subproblem_time += t;
    }

    void print() const noexcept {
      assert(valid());
      using std::setw;
      const auto w = setw(10);
      std::cout << "nds" << w << "inds" << w << "ulvs" << w << "sol" << "\n";
      std::cout << nodes << w << inner_nodes << w << unsat_leaves << w
        << solutions << "\n";
    }
  };
}

#endif