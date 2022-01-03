// Oleg Zaikin, 28.10.2021 (Irkutsk)
/* Copyright 2021, 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* TODOS

0) Complete specification of SearchStat
    - As precisely as possible.

1)

Update design, according to Numerics/Statistics.hpp.

Never any use of global variable.

Thus no "reset" (this is handled at contruction) or "update" (this is
handled by members for setting or adding to values).
Plus overloading operator "+" for combining the statistics from
different subproblems.

2)

There need to be proper output-facilities (currently it's just a hack).

3)

There needs to be a proper handling of fundamental types.
 - count_t shouldn't be defined from scratch here.
 - Why is here a use of double ?

*/

#ifndef STATISTICS_ouJtrmvkbM
#define STATISTICS_ouJtrmvkbM

#include <ostream>
#include <iomanip>

#include <cstdint>

#include <gecode/search.hh>

#include <Numerics/Statistics.hpp>

namespace Statistics {

  typedef std::uint64_t count_t;

  struct SearchStat {
    count_t nodes; // nodes in the backtracking, either at least two children or no children
    count_t inner_nodes; // nodes with at least two children
    count_t inner_nodes_2chld; // inner nodes with exactly 2 children
    count_t inner_nodes_3chld; // inner nodes with exactly 3 children
    count_t unsat_leaves; // those leaves which are unsatisfiable
    count_t solutions; // those leaves which are satisfiable
    count_t single_child_brnch; // inner nodes with exactly 1 child
    GenStats::BasicStats<float_t, float_t> choice_time;
    // total time for reduction and branching-determination; N is number of
    // reduction-applications = number of inner nodes
    GenStats::BasicStats<float_t, float_t> tau_time;
    // total time for tau-computation; N is the number of considered branchings
    GenStats::BasicStats<float_t, float_t> subproblem_time;
    // total time for look-ahead propagation in reduction and branching;
    // N is the number of calls to the look-ahead propagation-function
    Gecode::Search::Statistics gecode_stat;

    SearchStat() : nodes(0), inner_nodes(0), inner_nodes_2chld(0),
                   inner_nodes_3chld(0), unsat_leaves(0),
                   solutions(0), single_child_brnch(0), choice_time(),
                   tau_time(), subproblem_time(), gecode_stat() {}

    bool valid() const noexcept {
      return (unsat_leaves + solutions + inner_nodes == nodes);
    }

    // XXX ??? remove XXX
    void reset() noexcept {
      assert(valid());
      nodes = inner_nodes = unsat_leaves = solutions = 0;
      inner_nodes_2chld = inner_nodes_3chld = single_child_brnch = 0;
      choice_time = GenStats::BasicStats<float_t, float_t>();
      tau_time = GenStats::BasicStats<float_t, float_t>();
      subproblem_time = GenStats::BasicStats<float_t, float_t>();
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

  };

  std::ostream& operator <<(std::ostream& out, const SearchStat& s) {
    assert(s.valid());
    using std::setw;
    const auto w = setw(10);
    //out << "nds" << w << "inds" << w << "ulvs" << w << "sol" << "\n";
    out << w << s.nodes << w << s.inner_nodes << w << s.unsat_leaves << w
        << s.solutions;
    return out;
  }

}

#endif
