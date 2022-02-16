// Oleg Zaikin, 28.10.2021 (Irkutsk)
/* Copyright 2021, 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* TODOS

0) DONE (Specification is added)
   Complete specification of SearchStat
    - As precisely as possible.

1) DONE (Global variables and "reset" were removed. States of all main
         variables are updated automatically when other values are
         set or incremented)
Update design, according to Numerics/Statistics.hpp.
Never any use of global variable.
Thus no "reset" (this is handled at contruction) or "update" (this is
handled by members for setting or adding to values).

2) DONE (operator "+" combines all statistics.)
Overload operator "+" for combining the statistics from
different laprops.

3) DONE (simple_output(std::ostream& out) is provided like in
         Numerics/Statistics.hpp)
There need to be proper output-facilities (currently it's just a hack).

4) DONE (count_t is defined like in Numerics/Statistics.hpp, double
         was replaced by float_t)
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

  typedef GenStats::BasicStats<float_t, float_t> stats_t;

  // Function for combining two BasicStats.
  stats_t unite_stats(const stats_t& lhs, const stats_t& rhs) noexcept {
    stats_t result;
    result.N = lhs.N + rhs.N;
    result.sum = lhs.sum + rhs.sum;
    result.sum_sq = lhs.sum_sq + rhs.sum_sq;
    result.min_ = lhs.min_ + rhs.min_;
    result.max_ = lhs.max_ + rhs.max_;
    return result;
  }

  // Structure for maintaining all backtracking-tree statistics in Gecode.
  struct SearchStat {
    typedef std::uint64_t count_t;
  private :
    count_t nodes_ = 0; // nodes in the backtracking, either at least two children or no children
    count_t inner_nodes_ = 0; // nodes with at least two children
    count_t inner_nodes_1chld_ = 0; // inner nodes with exactly 1 child
    count_t inner_nodes_2chld_ = 0; // inner nodes with exactly 2 children
    count_t inner_nodes_3chld_ = 0; // inner nodes with exactly 3 children
    count_t unsat_leaves_ = 0; // those leaves which are unsatisfiable
    count_t solutions_ = 0; // those leaves which are satisfiable
    count_t rdc_1chld_ = 0; // nodes with exectly 1 child in lookahead reduction
    stats_t choice_time;
    // total time for reduction and branching-determination; N is number of
    // reduction-applications. N >= number of inner nodes because unsat
    // or sat leaves may be formed as a result of reduction.
    stats_t tau_time;
    // total time for tau-computation; N is the number of considered branchings
    stats_t la_prop_time;
    // total time for look-ahead propagation in reduction and branching;
    // N is the number of calls to the look-ahead propagation-function
    Gecode::Search::Statistics gecode_stat;

    // Check if statistics is valid:
    bool valid() const noexcept {
      assert(choice_time.N >= inner_nodes_);
      return (unsat_leaves_ + solutions_ + inner_nodes_ == nodes_);
    }

  public:
    // Incremening unsigned integer members with updating nodes:
    void increment_inner_nodes() noexcept { ++inner_nodes_; update_nodes(); }
    void increment_unsat_leaves() noexcept { ++unsat_leaves_; update_nodes(); }
    void increment_solutions() noexcept { ++solutions_; update_nodes(); }
    // Incremening unsigned integer members without updating nodes:
    void increment_inner_nodes_1chld() noexcept { ++inner_nodes_1chld_; }
    void increment_inner_nodes_2chld() noexcept { ++inner_nodes_2chld_; }
    void increment_inner_nodes_3chld() noexcept { ++inner_nodes_3chld_; }
    void increment_rdc_1chld() noexcept { ++rdc_1chld_; }
    // Incrementing BasicStats members used for measuring elapsed time:
    void increment_choice(const float_t t) noexcept { choice_time += t; }
    void increment_tau(const float_t t) noexcept { tau_time += t; }
    void increment_la_prop(const float_t t) noexcept { la_prop_time += t; }

    // Updating the number of nodes; it is done when any of three
    // components (inner_nodes_, unsat_leaves_, solutions_) is incremented:
    void update_nodes() noexcept {
      nodes_ = inner_nodes_ + unsat_leaves_ + solutions_;
      assert(valid());
    }

    // Setting Gecode statistics using a given object:
    void set_gecode_stat(const Gecode::Search::Statistics &g_s) {
      gecode_stat = g_s;
      // Some UNSAT leaves are detected directly in customised branchers,
      // but some of them are detected only by Gecode.
      // On the other hand, Gecode does not count UNSAT leaves correctly
      // in some trivial cases, e.g. when a problem is solved via
      // the initial propagation.
      // That is why maximal of two values is taken here.
      unsat_leaves_ = std::max(unsat_leaves_, count_t(gecode_stat.fail));
      update_nodes();
      assert(valid());
    }

    // Main statistics:
    count_t nodes() const noexcept { return nodes_; }
    count_t inner_nodes() const noexcept { return inner_nodes_; }
    count_t unsat_leaves() const noexcept { return unsat_leaves_; }
    count_t solutions() const noexcept { return solutions_; }
    count_t inner_nodes_1chld() const noexcept { return inner_nodes_1chld_; }
    count_t inner_nodes_2chld() const noexcept { return inner_nodes_2chld_; }
    count_t inner_nodes_3chld() const noexcept { return inner_nodes_3chld_; }
    count_t rdc_1chld() const noexcept { return rdc_1chld_; }
    count_t la_props() const noexcept { return la_prop_time.N; }
    float_t la_props_time() const noexcept { return la_prop_time.sum; }

    // Auxilary statistics that depends on the main one:
    count_t leaves() const noexcept { return unsat_leaves_ + solutions_; }
    count_t sat() const noexcept { return solutions()==0 ? 0 : 1; };

    // Gecode statistics:
    count_t gecode_nodes() const noexcept { return gecode_stat.node; }
    count_t gecode_unsat_leaves() const noexcept { return gecode_stat.fail; }

    // Output the statistics:
    void simple_output(std::ostream& out) noexcept {
      assert(valid());
      out << sat() << " " << nodes() << " " << inner_nodes() << " "
      << inner_nodes_1chld() << " " << inner_nodes_2chld() << " "
      << inner_nodes_3chld() << " " << leaves() << " " << unsat_leaves() << " "
      << solutions() << " " << rdc_1chld() << " " << choice_time.N << " "
      << tau_time.N << " " << la_props() << " " << choice_time.sum
      << " " << tau_time.sum << " " << la_props_time();
    }

    // Compare two SearchStat:
    friend bool operator ==(const SearchStat& lhs, const SearchStat& rhs)
      noexcept {
      return lhs.nodes() == rhs.nodes() and
        lhs.inner_nodes() == rhs.inner_nodes() and
        lhs.unsat_leaves() == rhs.unsat_leaves() and
        lhs.solutions() == rhs.solutions();
    }

    // Combining two SearchStat:
    friend SearchStat operator +(const SearchStat& lhs,
                                 const SearchStat& rhs) {
      SearchStat result;
      result.nodes_ = lhs.nodes_ + rhs.nodes_;
      result.inner_nodes_ = lhs.inner_nodes_ + rhs.inner_nodes_;
      result.inner_nodes_1chld_ = lhs.inner_nodes_1chld_ +
        rhs.inner_nodes_1chld_;
      result.inner_nodes_2chld_ = lhs.inner_nodes_2chld_ +
        rhs.inner_nodes_2chld_;
      result.inner_nodes_3chld_ = lhs.inner_nodes_3chld_ +
        rhs.inner_nodes_3chld_;
      result.unsat_leaves_ = lhs.unsat_leaves_ + rhs.unsat_leaves_;
      result.solutions_ = lhs.solutions_ + rhs.solutions_;
      result.rdc_1chld_ = lhs.rdc_1chld_ + rhs.rdc_1chld_;
      result.choice_time = unite_stats(lhs.choice_time, rhs.choice_time);
      result.tau_time = unite_stats(lhs.tau_time, rhs.tau_time);
      result.la_prop_time = unite_stats(lhs.la_prop_time, rhs.la_prop_time);
      return result;
    }

  };

}

#endif
