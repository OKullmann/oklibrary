// Oleg Zaikin, 5.4.2021 (Irkutsk)
/* Copyright 2021, 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

An implementation of look-ahead for the Gecode library.

BUGS:

 TODOS:

-8. DONE (The BaseBrancher class is implemented.)
    Create a base class derived from GC::Brancher
    - DONE It should contain all common functionality, including array of
      Gecode variables.
    - DONE All customised branchers must be inherited from it.
    - DONE (The increment_depth-action is now executed the commit()
      function of the base class. This function is called in
      the commit() functions of all inherited classes.)
      The increment_depth-action should be also located in the base class.

-7. Create a base class derived from GC::Space
 - DONE (The class Node was created)
   This class contains all common functionality.
 - DONE (Testing was held, all app tests show the same results)
   First it is empty (of own functionality), and "TWO_MOLS" (which needs to
   be renamed --- all-capitals is for macros) is to be derived from it
   (testing whether it works).
 - Then a structure for logging and the unsigned integer for depth
   are placed in there, and appropriate member functions for using them are
   provided.

-6. Provide logging
 - DONE The basic class contains a pointer to std::ostream (non-owning) and
   an enumerated value for the level of logging.
 - A null-pointer means no logging.

-5. DONE (Now TwoMOLS is inherited from the class Node, which has member dpth.
    dpth is initialised with 0, and in all customised brancher it is
    incremented in the commit() function. Now in the choice() function
    when computing the distance, depth can be taken by m->depth())
    The depth in the backtracking tree is needed:
 - A brancher needs to have access to this information.
 - 0 at the root, increasing by one by each proper branching.
 - As usual, standard statistics for this value are created.
 Comment by OZ:
 - It can be done by adding an unsigned integer variable 'depth'
   to the class TWO_MOLS (see above --- not directly).
 - In the default constructor of TWO_MOLS, depth is initialised
   by 0 (that correspond to the root of the backtracking tree),
 - depth should be copied to new objects of the class TWO_MOLS.
 - After call of the choice() function of a customised brancher,
   for each branch a clone of the current space is created,
   and on each of them the commit() function is called (see
   documentation in /docus/Gecode.txt)
 - In the commit() function, depth is incremented.

-4. Code review: OZ,OK
  - No function in a header-files uses std::cout or the like.

-3. DONE (see comments for sections A and B)
   Handling the memory leak

A) DONE (Gecode::Brancher does not have a virtual destructor. All objects
   of Gecode type, which belong to the current home space, e.g. IntViewArray x,
   are deleted automatically when the home space is deleted. It is possible
   to use the Gecode::dispose() function to provide a destructor for
   any other user-defined objects in a derived class, which need destruction.
   See Section V in the documentation Gecode/docus/Gecode.txt)
   Since GC::Brancher apparently does not have a virtual destructor, one can
   not put objects into derived classes, which need destruction.
   This needs to be explained in our documentation. OZ

B) DONE (In all cases std::shared_ptr was replaced by std::unique_ptr
   since no shared ownership is needed)
   The use of std::unique_ptr should be reviewed: OZ,OK
 - It was just a first solution, and with all such "first solutions",
   they neeed to be constantly monitored and reflected upon.
 - Likely at many (all?) place std::unique_ptr is more adequate, since
   no shared ownership is anticipated.

-2. Independent reduction before choosing branchings:
    - The reduction should be either eager or lazy.
    - Additional parameter: restart processing the main loop or not.
    - OZ needs to report on that, and write some documentation.

-1. Provide overview on functionality provided:
    - Also each function/class needs at least a short specification.

0. Remove code-duplication OZ,OK
    - A general design must emerge.

1. Divide Lookahead.hpp into several files OZ:
    - DONE (SearchStat was moved to Statistics.hpp)
      At least the SearchStat struct should mode into new Statistics.hpp,
    - LAInOut.hpp
    - LADistances.hpp

2. Statistics are urgently needed:
    - Documentation is needed, with precise specifications. OZ
    - DONE Basic statistics (number of nodes, inner nodes,
      unsatisfiable leaves, and solutions)
    - DONE Number of inner nodes with 2 children and 3 children.
    - DONE (single-child branchings are counted and not treated as nodes)
      Distinguish between branchings and nodes.
      Now each branching is treated as an inner node. Rather, single-child
      branchings should not be treated like any node. However, such branchings
      must be also counted.

3. Four levels of LA-reduction:
    - OZ: report on how the following is related to actual code
    - Level 0 :
     - no explicit reduction;
     - for every branching unsatisfiable branches are just removed;
     - if a branching of width 0 is created, the problem is (immediately)
       recognised as unsatisfiable;
     - if a branching of width 1 is created, then this branching, as a single-
       child-branching, is immediately excecuted.
    - Level 1 :
     - still no explicit reduction;
     - additionally to level 1, if in a considered branching a branch is found
       unsatisfiable, then the corresponding restriction is applied to the
       current instance (globally).
    - Level 2 :
     - now there is an explicit reduction, which applies the reductions found
       by falsified branches until a fixed-point is reached;
     - then in the separate branching function no checks for unsatisfiability
       etc. are done (since no such case can occur).
    - Level 3 :
     - additionally to level 2, now in a considered branching also the
       intersection of the branches is considered for a common reduction.

4. Generate examples for which tree sizes when using look-ahead are known:
  - It will allow checking correctness of the look-ahead implementation.
  - By now correctness is checked by hand on several small examples:
      Trivial::Sum; Send-more-money; Send-most-money.

5. Branchers for finding all solutions:
    - Now the same branchers are used to find one or all solutions.
    - The behaviour should be a bit different.

6. Later: we don't want to handle variables, but branchers.
  - We can't restrict to just branching on values.
  - We want access to the given possibilities for branching.

7. Later: general concept of a distance.
  - A general concept of a distance should be properly supported.
  - DONE A distance can be handled as a function of two arguments dist(F,F').

8. Later: we don't want to handle variables, but branchers.
  - We can't restrict to just branching on values.
  - We want access to the given possibilities for branching.

*/

#ifndef LOOKAHEAD_lNFKYYpHQ8
#define LOOKAHEAD_lNFKYYpHQ8

#include <iostream> // DEPRECATED
#include <ostream>
#include <limits>
#include <vector>
#include <memory>
#include <array>
#include <tuple>
#include <numeric>
#include <algorithm>

#include <cassert>

#include <gecode/int.hh>
#include <gecode/search.hh>

#include <Numerics/FloatingPoint.hpp>
#include <Numerics/Tau.hpp>
#include <SystemSpecifics/Timing.hpp>
#include <ProgramOptions/Environment.hpp>

#include "Statistics.hpp"

namespace Lookahead {

  namespace FP = FloatingPoint;
  namespace GC = Gecode;

  /*
    Helper functions for sorting
  */
  // Select indices from x:
  template <class VEC_VAL, class VEC_IND>
  VEC_VAL select(const VEC_VAL& x, const VEC_IND& ind) noexcept {
    assert(ind.size() == x.size());
    VEC_VAL res; res.reserve(x.size());
    for (const auto i : ind) {
      assert(i < x.size());
      res.push_back(x[i]);
    }
    return res;
  }
  // Sort in ascending/descending order according to reference-vector:
  template <class VEC_REF, class VEC_MIRROR>
  void sort_two(VEC_REF& x, VEC_MIRROR& y, const bool ascending) {
    assert(x.size() == y.size());
    std::vector<unsigned> indices(x.size());
    std::iota(indices.begin(), indices.end(), 0);
    std::stable_sort(indices.begin(), indices.end(),
              [&x, &ascending](const int a, const int b) noexcept {
                return ascending ? x[a] < x[b] : x[a] > x[b];
              });
    x = select(x, indices);
    assert((ascending and std::is_sorted(x.begin(), x.end())) or
           std::is_sorted(x.rbegin(), x.rend()));
    y = select(y, indices);
  }


  // size_t is used for sizes of Gecode arrays.
  // For a Gecode array, size() returns int, so the function
  // size_t tr(int size) was introduced to convert int to size_t.
  typedef unsigned size_t;
  inline constexpr size_t tr(const int size, [[maybe_unused]] const size_t bound = 0) noexcept {
    assert(bound <= std::numeric_limits<int>::max());
    assert(size >= int(bound));
    return size;
  }

  typedef std::uint64_t count_t;
  typedef FP::float80 float_t;
  typedef std::vector<float_t> vec_t;
  using weights_t = const vec_t*;
  using statistics_t = Statistics::SearchStat*;
  using log_t = std::ostream*;

  // Array of values of an integer variable:
  typedef GC::Int::IntView IntView;
  // Array of array of values of integer variables:
  typedef GC::ViewArray<IntView> IntViewArray;
  // Value iterator for an integer variable:
  typedef GC::IntVarValues IntVarValues;

  typedef Statistics::BrStatus BrStatus;
  typedef std::vector<int> values_t;
  typedef std::vector<bool> eq_values_t;

  // bt_t is a branching tuple, i.e. a tuple of distances.
  // Distance is how much simpler the problem became.
  typedef std::vector<float_t> bt_t;

  // A matrix of variables and their values:
  typedef std::vector< std::vector<bool> > var_values_matrix_t;

  // Branching type, i.e. how branching is formed and executed.
  // la: choose a variable via look-ahead.
  // mind: choose a variable with minimal domain size.
  // mindr: like mind, but with reduction from la.
  enum class BrTypeO {la=0, mind=1, mindr=2};

  // How branches are formed for a branching.
  // eq: for a variable var and its values val1, ..., valk,
  //  for each value vali the branching is formed by two branches
  //  var=vali, var!=vali.
  // val: for a variable var and its values val1, ..., valk,
  //  one branching is formed by branches var=val1, ..., var=valk.
  enum class BrSourceO {eqval=0, eq=1, val=2};

  // The number of solutions to find.
  // one: find one solution or prove that no solution exists.
  // all: find all solutions or prove that no solution exists.
  enum class BrSolutionO {one=0, all=1};

  // Mode of applying found single-child branchings.
  // eager - apply a found single-child branching immediately.
  // lazy  - collect all single-child branchings and apply them
  //        as a partial assignment.
  enum class BrEagernessO {eager=0, lazy=1};

  // Enable/disable pruning in the reduction.
  // pruning - enable pruning.
  // nopruning - disable pruning.
  enum class BrPruneO {pruning=0, nopruning=1};

  // Enable/disable the upper bound for weights-optimisation.
  enum class UpperBoundO {upperbound=0, noupperbound=1};

  // Ordering of branchings.
  // given               - default order given by Gecode
  // reverse given       - reverse given order
  // descending distance - descending distance from the original formula
  // ascending distance  - ascending distance from the original formula
  enum class BrOrderO {given=0, revgiven=1, descdist=2, ascendist=3};

  // Level of logging information about nodes.
  // none    - no logging.
  // reduced - id, depth, branching variable, values of child branches,
  // full    - as reduced, but with states of variables before and after
  //           lookahead reduction.
  enum class LogLvlO {none=0, reduced=1, full=2};
}
namespace Environment {
  template <>
  struct RegistrationPolicies<Lookahead::BrTypeO> {
    static constexpr int size = int(Lookahead::BrTypeO::mindr)+1;
    static constexpr std::array<const char*, size> string
    {"la", "mind", "mindr"};
  };
  template <>
  struct RegistrationPolicies<Lookahead::BrSourceO> {
    static constexpr int size = int(Lookahead::BrSourceO::val)+1;
    static constexpr std::array<const char*, size> string
    {"eqval", "eq", "val"};
  };
  template <>
  struct RegistrationPolicies<Lookahead::BrSolutionO> {
    static constexpr int size = int(Lookahead::BrSolutionO::all)+1;
    static constexpr std::array<const char*, size> string
    {"one", "all"};
  };
  template <>
  struct RegistrationPolicies<Lookahead::BrEagernessO> {
    static constexpr int size = int(Lookahead::BrEagernessO::lazy)+1;
    static constexpr std::array<const char*, size> string
    {"eager", "lazy"};
  };
  template <>
  struct RegistrationPolicies<Lookahead::BrPruneO> {
    static constexpr int size = int(Lookahead::BrPruneO::nopruning)+1;
    static constexpr std::array<const char*, size> string
    {"prun", "noprun"};
  };
  template <>
  struct RegistrationPolicies<Lookahead::UpperBoundO> {
    static constexpr int size = int(Lookahead::UpperBoundO::noupperbound)+1;
    static constexpr std::array<const char*, size> string
    {"upbnd", "noupbnd"};
  };
  template <>
  struct RegistrationPolicies<Lookahead::BrOrderO> {
    static constexpr int size = int(Lookahead::BrOrderO::ascendist)+1;
    static constexpr std::array<const char*, size> string
    {"given", "revgiven", "descdist", "ascdist"};
  };
  template <>
  struct RegistrationPolicies<Lookahead::LogLvlO> {
    static constexpr int size = int(Lookahead::LogLvlO::full)+1;
    static constexpr std::array<const char*, size> string
    {"nolog", "rdcdlog", "fulllog"};
  };
}
namespace Lookahead {
  constexpr char sep = ',';
  typedef std::tuple<BrTypeO, BrSourceO, BrSolutionO, BrEagernessO, BrPruneO,
    UpperBoundO, BrOrderO, LogLvlO> option_t;

  std::ostream& operator <<(std::ostream& out, const BrTypeO brt) {
    switch (brt) {
    case BrTypeO::mind : return out << "min-domain-size";
    case BrTypeO::mindr : return out << "mind-with-reduction";
    default : return out << "lookahead";}
  }
  std::ostream& operator <<(std::ostream& out, const BrSourceO brsrs) {
    switch (brsrs) {
    case BrSourceO::eq : return out << "equals-only";
    case BrSourceO::val : return out << "values-only";
    default : return out << "equals+values";}
  }
  std::ostream& operator <<(std::ostream& out, const BrSolutionO brsln) {
    switch (brsln) {
    case BrSolutionO::all : return out << "all-sol";
    default : return out << "one-sol";}
  }
  std::ostream& operator <<(std::ostream& out, const BrEagernessO bregr) {
    switch (bregr) {
    case BrEagernessO::lazy : return out << "lazy";
    default : return out << "eager";}
  }
  std::ostream& operator <<(std::ostream& out, const BrPruneO brpr) {
    switch (brpr) {
    case BrPruneO::nopruning : return out << "nopruning";
    default : return out << "pruning";}
  }
  std::ostream& operator <<(std::ostream& out, const UpperBoundO ub) {
    switch (ub) {
    case UpperBoundO::noupperbound : return out << "no-unsat-leaves-upperbound";
    default : return out << "unsat-leaves-upper-bound";}
  }
  std::ostream& operator <<(std::ostream& out, const BrOrderO bo) {
    switch (bo) {
    case BrOrderO::revgiven : return out << "reverse-given";
    case BrOrderO::descdist : return out << "descending-distance";
    case BrOrderO::ascendist : return out << "ascending-distance";
    default : return out << "given";}
  }
  std::ostream& operator <<(std::ostream& out, const LogLvlO llo) {
    switch (llo) {
    case LogLvlO::reduced : return out << "reduced-logging";
    case LogLvlO::full : return out << "full-logging";
    default : return out << "no-logging";}
  }

  inline float_t mu0(const GC::IntVarArray& V,
    [[maybe_unused]] const weights_t wghts = nullptr) noexcept {
    assert(wghts == nullptr);
    float_t s = 0;
    for (const auto& v : V) {
      const auto is = tr(v.size(), 1);
      s += is - 1;
    }
    return s;
  }

  inline float_t mu1(const GC::IntVarArray& V,
    [[maybe_unused]] const weights_t wghts = nullptr) noexcept {
    assert(wghts == nullptr);
    float_t s = 0;
    for (const auto& v : V) {
      const auto is = tr(v.size(), 1);
      s += FP::log2(is);
    }
    return s;
  }

  // A domain of size 1 has weight 0.
  // A domain of size 2 has weight 1.
  // The remaining domains have weights specified in a given vector wghts.
  inline float_t muw(const GC::IntVarArray& V,
                     const weights_t wghts) noexcept {
    assert(wghts);
    float_t s = 0;
    for (const auto& v : V) {
      const auto is = tr(v.size(), 1);
      assert(is >= 1);
      if (is == 1) continue;
      else if (is == 2) ++s;
      else {
        assert(is-3 < wghts->size());
        s += (*wghts)[is-3];
      }
    }
    return s;
  }

  inline float_t distance(const GC::IntVarArray& V, const GC::IntVarArray& Vn,
                          const weights_t wghts) noexcept {
    assert(wghts);
    float_t s = 0;
    const int N = V.size();
    for (int i = 0; i < N; ++i) {
      const auto ds = tr(V[i].size(), 1);
      const auto dsn = tr(Vn[i].size(), 1);
      if (dsn == ds) continue;
      assert(dsn < ds);
      if (dsn == 1) ++s;
      else {
        assert(dsn-2 < wghts->size());
        s += (*wghts)[dsn-2];
      }
    }
    return s;
  }


  template<class ModSpace>
  std::unique_ptr<ModSpace> subproblem(ModSpace* const m, const int v,
                                       const int val, const bool eq = true,
                                       statistics_t stat = nullptr) noexcept {
    assert(stat);
    assert(m->valid());
    assert(m->valid(v));
    assert(m->status() == GC::SS_BRANCH);
    const Timing::UserTime timing;
    const Timing::Time_point t0 = timing();
    // Clone space:
    std::unique_ptr<ModSpace> c(static_cast<ModSpace*>(m->clone()));
    assert(c->valid());
    assert(c->valid(v));
    assert(c->status() == GC::SS_BRANCH);
    // Add an equality constraint for the given variable and its value:
    if (eq) GC::rel(*(c.get()), (c.get())->at(v), GC::IRT_EQ, val, GC::IPL_DOM);
    else GC::rel(*(c.get()), (c.get())->at(v), GC::IRT_NQ, val, GC::IPL_DOM);
    const Timing::Time_point t1 = timing();
    stat->increment_la_prop(t1-t0);
    return c;
  }

  // Equality branching: at most two branches of the kind var==val and var!=val.
  //  - brvalues : is a Boolean array of branches: true means var==val,
  //    false means var!=val. Possible eq_values are: {}, {false}, {true}, {false, true}.
  //  - tuple : branching tuple, where each value corresponds to a branch.
  //  - ltau : value of the ltau function for the branching tuple.
  struct EqBranching {
    int var;
    int value;
    eq_values_t brvalues;
    bt_t tuple;
    float_t ltau;
    BrStatus brstatus;

    EqBranching(const int v=0, const int val=0, const eq_values_t brvls={}, const bt_t tpl={})
      : var(v), value(val), brvalues(brvls), tuple(tpl), ltau(FP::pinfinity) {
        // If branching of width 0, the problem is unsat:
        if (tuple.empty() and brvalues.empty()) brstatus = BrStatus::unsat;
        // If at least one subproblem is satisfiable:
        else if (tuple.size() != brvalues.size()) brstatus = BrStatus::sat;
        // If branching of width 1:
        else if (tuple.size() == 1 and brvalues.size() == 1) brstatus = BrStatus::single;
        // Two branches, neither unsat or sat:
        else brstatus = BrStatus::branching;
        valid();
    }

   bool valid() const noexcept {
      return var >= 0 and brvalues.size() <= 2 and ltau >= 0 and
      (brvalues.empty() or brvalues.size() == 1 or brvalues[0] != brvalues[1]) and
      tuple.size() <= brvalues.size();
    }

    bool operator <(const EqBranching& a) const noexcept { return ltau < a.ltau; }

    void print() const noexcept { // DEPRECATED
      std::cout << static_cast<int>(brstatus) << " " << var << " " << value << "{";
      for (auto x : brvalues) std::cout << int(x) << ",";
      std::cout << "} {";
      for (auto& x : tuple) std::cout << (int)x << ",";
      std::cout << "} " << ltau << std::endl;
    }

    BrStatus status() const noexcept { return brstatus; }

    void calc_ltau(statistics_t stat = nullptr) noexcept {
      assert(stat);
      assert(not tuple.empty());
      const Timing::UserTime timing;
      const Timing::Time_point t0 = timing();
      ltau = Tau::ltau(tuple);
      const Timing::Time_point t1 = timing();
      stat->increment_tau(t1-t0);
    }

    size_t branches_num() const noexcept {
      assert(valid());
      if (brstatus == BrStatus::unsat) return 1;
      else return brvalues.size();
    }

    // Sort branches and values in the branching tuple
    // as reversed given order:
    void sort_revgiven() noexcept {
      std::reverse(brvalues.begin(), brvalues.end());
      std::reverse(tuple.begin(), tuple.end());
    }
    // By distance in ascending order:
    void sort_ascendist() noexcept {
      sort_two(tuple, brvalues, true);
    }
    // By distance in descending order:
    void sort_descdist() noexcept {
      sort_two(tuple, brvalues, false);
    }

  };

  // Value branching of the kind var=val1, ... , var=valk.
  //  - values : is a integer array of branches.
  //  - tuple : branching tuple, where each value corresponds to a branch.
  //  - ltau : value of the ltau function for the branching tuple.
  struct ValBranching {
    int var;
    values_t values;
    bt_t tuple;
    float_t ltau;
    BrStatus brstatus;

    ValBranching(const int v=0, const values_t vls={}, const bt_t tpl={})
      : var(v), values(vls), tuple(tpl), ltau(FP::pinfinity) {
        // If branching of width 0, the problem is unsat:
        if (tuple.empty() and values.empty()) brstatus = BrStatus::unsat;
        // If at least one subproblem is satisfiable:
        else if (tuple.size() != values.size()) brstatus = BrStatus::sat;
        // If branching of width 1:
        else if (tuple.size() == 1 and values.size() == 1) brstatus = BrStatus::single;
        // Two branches, neither unsat or sat:
        else brstatus = BrStatus::branching;
        valid();
    }

   bool valid() const noexcept {
      return var >= 0 and ltau >= 0 and tuple.size() <= values.size();
    }

    bool operator <(const ValBranching& a) const noexcept { return ltau < a.ltau; }

    void print() const noexcept { // DEPRECATED
      std::cout << static_cast<int>(brstatus) << " " << var << " {";
      for (auto x : values) std::cout << int(x) << ",";
      std::cout << "} {";
      for (auto& x : tuple) std::cout << (int)x << ",";
      std::cout << "} " << ltau << std::endl;
    }

    BrStatus status() const noexcept { return brstatus; }

    void calc_ltau(statistics_t stat = nullptr) noexcept {
      assert(stat);
      assert(not tuple.empty());
      const Timing::UserTime timing;
      const Timing::Time_point t0 = timing();
      ltau = Tau::ltau(tuple);
      const Timing::Time_point t1 = timing();
      stat->increment_tau(t1-t0);
    }

    size_t branches_num() const noexcept {
      assert(valid());
      return values.empty() ? 1 : values.size();
    }

    // Sort branches and values in the branching tuple
    // as reversed given order:
    void sort_revgiven() noexcept {
      std::reverse(values.begin(), values.end());
      std::reverse(tuple.begin(), tuple.end());
    }
    // By distance in ascending order:
    void sort_ascendist() noexcept {
      sort_two(tuple, values, true);
    }
    // By distance in descending order:
    void sort_descdist() noexcept {
      sort_two(tuple, values, false);
    }

  };

  struct Branching {
    BrStatus status;
    int var;
    values_t values;
    eq_values_t eq_values;
    bt_t v_tuple;
    bt_t eq_tuple;
    float_t ltau;

    Branching(BrStatus status=BrStatus::unsat, const int v=0,
              const values_t vls={},
              const eq_values_t eq_vls={}, const bt_t v_tpl={},
              const bt_t eq_tpl={})
      : status(status), var(v), values(vls), eq_values(eq_vls), v_tuple(v_tpl),
      eq_tuple(eq_tpl), ltau(FP::pinfinity) { valid(); }

   bool valid() const noexcept {
      return var >= 0 and eq_values.size() <= 2 and ltau >= 0 and
      (eq_values.empty() or eq_values.size() == 1 or eq_values[0] != eq_values[1]) and
      v_tuple.size() <= values.size() and eq_tuple.size() <= eq_values.size() and
      ((status == BrStatus::unsat and values.empty() and eq_values.empty()) or
       (status != BrStatus::unsat and values.size() == 1 and not eq_values.empty()) or
       (status != BrStatus::unsat and not values.empty() and eq_values.empty()));
    }

    bool operator <(const Branching& a) const noexcept {
      return ltau < a.ltau;
    }

    void print() const noexcept { // DEPRECATED
      std::cout << static_cast<int>(status) << " " << var << " {";
      for (auto& x : values) std::cout << x << ",";
      std::cout << "} {";
      for (auto x : eq_values) std::cout << int(x) << ",";
      std::cout << "} {";
      for (auto& x : v_tuple) std::cout << (int)x << ",";
      std::cout << "} {";
      for (auto& x : eq_tuple) std::cout << (int)x << ",";
      std::cout << "} " << ltau << std::endl;
    }

    BrStatus status_eq() noexcept {
      status = BrStatus::branching;
      // If branching of width 0, the problem is unsat:
      if (eq_tuple.empty() and eq_values.empty()) {
        status = BrStatus::unsat;
      }
      // If at least one subproblem is satisfiable:
      else if (eq_tuple.size() != eq_values.size()) {
        status = BrStatus::sat;
      }
      // If branching of width 1:
      else if (eq_tuple.size() == 1 and eq_values.size() == 1) {
        status = BrStatus::single;
      }
      return status;
    }

    BrStatus status_val() noexcept {
      status = BrStatus::branching;
      if (v_tuple.empty() and values.empty()) {
        status = BrStatus::unsat;
      }
      else if (v_tuple.size() != values.size()) {
        status = BrStatus::sat;
      }
      else if (v_tuple.size() == 1 and values.size() == 1) {
        status = BrStatus::single;
      }
      return status;
    }

    void calc_ltau(statistics_t stat = nullptr) noexcept {
      assert(stat);
      assert(not eq_tuple.empty() or not v_tuple.empty());
      const Timing::UserTime timing;
      const Timing::Time_point t0 = timing();
      if (not eq_tuple.empty()) ltau = Tau::ltau(eq_tuple);
      else ltau = Tau::ltau(v_tuple);
      const Timing::Time_point t1 = timing();
      stat->increment_tau(t1-t0);
    }

    size_t branches_num() const noexcept {
      assert(valid());
      if (status == BrStatus::unsat) return 1;
      if (eq_values.empty()) return values.empty() ? 1 : values.size();
      else return eq_values.size();
    }

  };

  template<class CustomBranching>
  CustomBranching best_branching(std::vector<CustomBranching>& branchings,
                                 statistics_t stat = nullptr,
                                 const BrOrderO bro = BrOrderO::given) {
    assert(not branchings.empty());
    CustomBranching best_br;
    for (auto& br : branchings) {
      assert(br.status() == BrStatus::branching);
      br.calc_ltau(stat);
      best_br = std::min(best_br, br);
    }
    // Change the order of branches if required:
    if (bro == BrOrderO::revgiven) best_br.sort_revgiven();
    else if (bro == BrOrderO::ascendist) best_br.sort_ascendist();
    else if (bro == BrOrderO::descdist) best_br.sort_descdist();
    return best_br;
  }

  Branching best_branching(std::vector<Branching>& branchings, statistics_t stat = nullptr) {
    assert(stat);
    assert(not branchings.empty());
    Branching best_br;
    for (auto& br : branchings) {
      assert(br.status == BrStatus::branching);
      br.calc_ltau(stat);
      best_br = std::min(best_br, br);
    }
    return best_br;
  }

  template<class CustomBranching>
  std::vector<CustomBranching> best_branchings(
    std::vector<CustomBranching>& tau_brs,
    statistics_t stat = nullptr) noexcept {
    assert(stat);
    std::vector<CustomBranching> branchings;
    assert(not tau_brs.empty());
    CustomBranching br = best_branching<CustomBranching>(tau_brs);
    branchings = {br};
    return branchings;
  }

  std::vector<Branching> best_branchings(std::vector<Branching>& tau_brs,
                                         statistics_t stat = nullptr) noexcept {
    assert(stat);
    std::vector<Branching> branchings;
    assert(not tau_brs.empty());
    Branching br = best_branching(tau_brs, stat);
    branchings = {br};
    return branchings;
  }

  struct SingleChildBranching {
    int var;
    int val;
    bool eq;
    SingleChildBranching(const int var, const int val, const bool eq) :
      var(var), val(val), eq(eq) {}
  };
  struct ReduceRes {
    int var;
    values_t values;
    BrStatus status;
    ReduceRes() : var(0), values{}, status(BrStatus::branching) {}
    ReduceRes(const int var, const values_t values, const BrStatus status) :
      var(var), values(values), status(status) {}
  };
  // A lookup table for all variables and their values.
  // The element [i][j] == True iff the value x[i]==j is pruned, False otherwise.
  // The table size is n x m where n is the number of variables, m is maximal
  // value among all variables.
  var_values_matrix_t get_initial_pruned_values(const IntViewArray x, const int start) {
    const auto xsize = x.size();
    var_values_matrix_t res(xsize);
    for (int var = start; var < xsize; ++var) {
      const IntView view = x[var];
      if (view.assigned()) continue;
      assert(view.size() >= 2);
      // Find the maximal value among unassigned variables:
      int maxval = 0;
      for (IntVarValues j(view); j(); ++j) maxval = std::max(j.val(), maxval);
      std::vector<bool> vec(maxval + 1, false);
      //initial_pruned_values[var] = vec;
      res[var] = vec;
    }
    return res;
  }
  template<class ModSpace>
  ReduceRes reduceEager(GC::Space& home, const IntViewArray x, const int start,
                        const BrPruneO brpr, const bool eqbr=false,
                        statistics_t stat = nullptr) {
    assert(start < x.size());
    assert(stat);
    ReduceRes res;
    ModSpace* m = &(static_cast<ModSpace&>(home));
    assert(m->status() == GC::SS_BRANCH);
    var_values_matrix_t initial_pruned_values, pruned_values;
    if (brpr == BrPruneO::pruning) {
      initial_pruned_values = get_initial_pruned_values(x, start);
      pruned_values = initial_pruned_values;
    }
    bool reduction = false;
    do {
      reduction = false;
      for (int var = start; var < x.size(); ++var) {
        const IntView view = x[var];
        if (view.assigned()) continue;
        if (brpr == BrPruneO::pruning) {
          // Count pruned values for the variable:
          size_t pruned_var_values = 0;
          for (auto v : pruned_values[var]) if (v) ++pruned_var_values;
          // If all values are pruned, skip the variable:
          if (pruned_var_values == view.size()) continue;
        }
        assert(view.size() >= 2);
        values_t brvalues;
        std::vector<SingleChildBranching> var_single_child_brs;
        for (IntVarValues j(view); j(); ++j) {
          const int val = j.val();
          if (brpr == BrPruneO::pruning and pruned_values[var][val]) {
            brvalues.push_back(val);
            continue;
          }
          assert(m->status() == GC::SS_BRANCH);
          const auto subm = subproblem<ModSpace>(m, var, val, true, stat);
          const auto subm_st = subm->status();
          // The assignment var==val is inconsistent:
          if (subm_st == GC::SS_FAILED) {
            SingleChildBranching sch(var, val, false);
            var_single_child_brs.push_back(sch);
          }
          // The assignment var==val is relatively inconsistent,
          // i.e. it is not clear whether it is inconsistent or not:
          else {
            brvalues.push_back(val);
            if (subm_st == GC::SS_SOLVED) {
              if (eqbr) return ReduceRes(var, {val}, BrStatus::sat);
              else res.status = BrStatus::sat;
            }
            if (brpr == BrPruneO::pruning) {
              // Update the LUT with pruned values:
              const auto subm_x = subm->at();
              for (int var2 = start; var2 < subm_x.size(); ++var2) {
                if (var2 == var) continue;
                const IntView view = x[var2];
                const IntView subm_view = subm_x[var2];
                if (not view.assigned() and subm_view.assigned()) {
                  assert((unsigned)var2 < pruned_values.size());
                  assert((unsigned)subm_view.val() < pruned_values[var2].size());
                  pruned_values[var2][subm_view.val()] = true;
                }
              }
            }
          }
        }

        if (res.status == BrStatus::sat) {
          assert(not brvalues.empty());
          return ReduceRes(var, brvalues, BrStatus::sat);
        }
        // No branches, so the problem is unsatisfiable:
        else if (brvalues.size() == 0) {
          return ReduceRes(0, {}, BrStatus::unsat);
        }
        // If single-child branching:
        else if (brvalues.size() == 1) {
          stat->increment_rdc_1chld();
          reduction = true;
          if (brpr == BrPruneO::pruning) {
            // Reset LUT with pruned values since the reduction has happened:
            pruned_values = initial_pruned_values;
          }
          GC::rel(home, x[var], GC::IRT_EQ, brvalues[0], GC::IPL_DOM);
          const auto st = home.status();
          if (st == GC::SS_FAILED) return ReduceRes(0, {}, BrStatus::unsat);
          else if (st == GC::SS_SOLVED) return ReduceRes(var, brvalues, BrStatus::sat);
        }
        // None from above - non-sat, non-unsat, at least 2 branches:
        else {
          if (not var_single_child_brs.empty()) {
            reduction = true;
            if (brpr == BrPruneO::pruning) {
              // Reset LUT with pruned values since the reduction has happened:
              pruned_values = initial_pruned_values;
            }
          }
          for (auto& sch : var_single_child_brs) {
            assert(not sch.eq);
            GC::rel(home, x[sch.var], GC::IRT_NQ, sch.val, GC::IPL_DOM);
            const auto st = home.status();
            if (st == GC::SS_FAILED) return ReduceRes(0, {}, BrStatus::unsat);
            else if (st == GC::SS_SOLVED) return ReduceRes(sch.var, {sch.val}, BrStatus::sat);
          }
        }
      } // for (int var = start; var < x.size(); ++var) {
    } while (reduction);

    return res;
  }

  template<class ModSpace>
  ReduceRes reduceLazy(GC::Space& home, const IntViewArray x, const int start,
                       const BrPruneO brpr, const bool eqbr=false,
                       statistics_t stat = nullptr) {
    assert(start < x.size());
    assert(stat);
    ReduceRes res;
    ModSpace* m = &(static_cast<ModSpace&>(home));
    assert(m->status() == GC::SS_BRANCH);
    const int xsize = x.size();
    var_values_matrix_t initial_pruned_values;
    if (brpr == BrPruneO::pruning) {
      initial_pruned_values = get_initial_pruned_values(x, start);
    }
    bool reduction = false;
    do {
      reduction = false;
      std::vector<SingleChildBranching> single_child_brs;
      var_values_matrix_t pruned_values = initial_pruned_values;
      for (int var = start; var < xsize; ++var) {
        const IntView view = x[var];
        if (view.assigned()) continue;
        if (brpr == BrPruneO::pruning) {
          // Count pruned values for the variable:
          size_t pruned_var_values = 0;
          for (auto v : pruned_values[var]) if (v) ++pruned_var_values;
          // If all values are pruned, skip the variable:
          if (pruned_var_values == view.size()) continue;
        }
        assert(view.size() >= 2);
        values_t brvalues;
        std::vector<SingleChildBranching> var_single_child_brs;
        for (IntVarValues j(view); j(); ++j) {
          const int val = j.val();
          if (brpr == BrPruneO::pruning and pruned_values[var][val]) {
            brvalues.push_back(val);
            continue;
          }
          assert(m->status() == GC::SS_BRANCH);
          const auto subm = subproblem<ModSpace>(m, var, val, true, stat);
          const auto subm_st = subm->status();
          if (subm_st == GC::SS_FAILED) {
            SingleChildBranching sch(var, val, false);
            var_single_child_brs.push_back(sch);
          }
          else {
            brvalues.push_back(val);
            if (subm_st == GC::SS_SOLVED) {
              if (eqbr) return ReduceRes(var, {val}, BrStatus::sat);
              else res.status = BrStatus::sat;
            }
            if (brpr == BrPruneO::pruning) {
              // Update the LUT with pruned values:
              const auto subm_x = subm->at();
              for (int var2 = start; var2 < subm_x.size(); ++var2) {
                if (var2 == var) continue;
                const IntView view = x[var2];
                const IntView subm_view = subm_x[var2];
                if (not view.assigned() and subm_view.assigned()) {
                  assert((unsigned)var2 < pruned_values.size());
                  assert((unsigned)subm_view.val() < pruned_values[var2].size());
                  pruned_values[var2][subm_view.val()] = true;
                }
              }
            }
          }
        }

        if (res.status == BrStatus::sat) {
          assert(not brvalues.empty());
          return ReduceRes(var, brvalues, BrStatus::sat);
        }
        // No branches, so the problem is unsatisfiable:
        else if (brvalues.size() == 0) {
          return ReduceRes(0, {}, BrStatus::unsat);
        }
        // If single-child branching:
        else if (brvalues.size() == 1) {
          stat->increment_rdc_1chld();
          reduction = true;
          // var==val:
          SingleChildBranching sch(var, brvalues[0], true);
          single_child_brs.push_back(sch);
        }
        // None from above - non-sat, non-unsat, at least 2 branches:
        else {
          if (not var_single_child_brs.empty()) reduction = true;
          for (auto& sch : var_single_child_brs) {
            assert(not sch.eq);
            single_child_brs.push_back(sch);
          }
        }
      } // for (int var = start; var < x.size(); ++var) {
      if (not single_child_brs.empty()) {
        for (auto& sch : single_child_brs) {
          if (sch.eq) GC::rel(home, x[sch.var], GC::IRT_EQ, sch.val, GC::IPL_DOM);
          else GC::rel(home, x[sch.var], GC::IRT_NQ, sch.val, GC::IPL_DOM);
          const auto st = home.status();
          if (st == GC::SS_FAILED) return ReduceRes(0, {}, BrStatus::unsat);
          else if (st == GC::SS_SOLVED) return ReduceRes(sch.var, {sch.val}, BrStatus::sat);
        }
      }
    } while (reduction);

    return res;
  }

  template <class CustomisedEqBrancher>
  struct EqBranchingChoice : public GC::Choice {
    EqBranching br;
    statistics_t stat;
    bool valid() const noexcept { return br.valid(); }
    EqBranchingChoice(const CustomisedEqBrancher& b,
                      const EqBranching& br = EqBranching(),
                      const statistics_t stat = nullptr)
      : GC::Choice(b, br.branches_num()), br(br) {
      assert(stat);
      stat->new_node(br.status(), br.branches_num());
    }
  };

  template <class CustomisedValBrancher>
  struct ValBranchingChoice : public GC::Choice {
    ValBranching br;
    statistics_t stat;
    bool valid() const noexcept { return br.valid(); }
    ValBranchingChoice(const CustomisedValBrancher& b,
                       const ValBranching& br = ValBranching(),
                       const statistics_t stat = nullptr)
      : GC::Choice(b, br.branches_num()), br(br), stat(stat) {
      assert(stat);
      stat->new_node(br.status(), br.branches_num());
    }
  };

  template <class CustomisedBrancher>
  struct BranchingChoice : public GC::Choice {
    Branching br;
    statistics_t stat;
    bool valid() const noexcept { return br.valid(); }
    BranchingChoice(const CustomisedBrancher& b,
                    const Branching& br = Branching(),
                    statistics_t stat = nullptr)
      : GC::Choice(b, br.branches_num()), br(br), stat(stat) {
      assert(stat);
      stat->new_node(br.status, br.branches_num());
    }
  };

  // Class for logging tree-data.
  class Logging {
    log_t log;
    LogLvlO loglvl;
  public:
    Logging(log_t log = nullptr, const LogLvlO loglvl = LogLvlO::none) :
              log(log), loglvl(loglvl) {}

    // Add data to a log:
    void update_log(const count_t dpth, const count_t id, const int branchvar,
                   const values_t values) noexcept {
      assert(not values.empty());
      if (log == nullptr or loglvl == LogLvlO::none) return;
      // First write basic data:
      *log << id << " " << dpth << " " << branchvar << " ";
      for (auto& val : values) *log << val << " ";
      *log << std::endl;
      // Write states of variables if given:
      if (loglvl == LogLvlO::full) {
        // XXX
      } 
    }
  };

  // A node in the backtracking tree. All classes that describe problems
  // (like TwoMOLS) should be derived from this class.
  class Node : public GC::Space {
    // Node's depth in the backtracking tree:
    count_t dpth;
    Logging lgging;

  public:
    Node(const log_t log = nullptr, const LogLvlO loglvl = LogLvlO::none) :
      dpth(0), lgging(log, loglvl) {}

    count_t depth() const noexcept { return dpth; }
    void increment_depth() noexcept { ++dpth; }
    void update_log(const count_t id, const int branchvar,
      const values_t values) noexcept {
      lgging.update_log(dpth, id, branchvar, values);
    }
  };


  // A base customised brancher inherited from Gecode::Brancher.
  // It contains common functionality to reduce code duplications.
  // All customised branchers should be inherited from it.
  class BaseBrancher : public GC::Brancher {
  protected:
    IntViewArray x;
    mutable int start;

    static bool valid(const IntViewArray x) noexcept { return x.size() > 0; }
    static bool valid(const int s, const IntViewArray x) noexcept {
      return s >= 0 and valid(x) and s < x.size();
    }
  public:
    bool valid() const noexcept { return valid(start, x); }

    BaseBrancher(const GC::Home home, const IntViewArray& x)
      : GC::Brancher(home), x(x), start(0) { assert(valid(start, x)); }
    BaseBrancher(GC::Space& home, BaseBrancher& b)
      : GC::Brancher(home,b), start(b.start) {
      assert(valid(b.x));
      x.update(home, b.x);
      assert(valid(start, x));
    }

    virtual bool status(const GC::Space&) const noexcept {
      assert(valid(start, x));
      for (auto i = start; i < x.size(); ++i)
        if (not x[i].assigned()) { start = i; return true; }
      return false;
    }

    virtual GC::Choice* choice(const GC::Space&, GC::Archive&) noexcept {
      return new BranchingChoice<BaseBrancher>(*this);
    }

    template <class ModSpace>
    GC::ExecStatus commit(GC::Space& home, const GC::Choice&,
                          const unsigned) noexcept {
      ModSpace* m = &(static_cast<ModSpace&>(home));
      m->increment_depth();
      return GC::ES_OK;
    }

  };

  // A customised brancher. Branchings are formed by assigning all possible
  // values to all unassigned variables. A branching with minimal domain
  // size is chosen as the best branching.
  template <class ModSpace>
  class MinDomValue : public BaseBrancher {
  public:
    using BaseBrancher::BaseBrancher;

    static void post(GC::Home home, const IntViewArray& x) noexcept {
      new (home) MinDomValue(home, x);
    }
    virtual GC::Brancher* copy(GC::Space& home) noexcept {
      return new (home) MinDomValue(home, *this);
    }

    virtual GC::Choice* choice(GC::Space& home) noexcept {
      ModSpace* m = &(static_cast<ModSpace&>(home));
      assert(m->status() == GC::SS_BRANCH);
      statistics_t stat = m->statistics();
      assert(stat);
      const Timing::UserTime timing;
      const Timing::Time_point t0 = timing();
      assert(valid(start, x));
      int var = start;
      size_t width = tr(x[var].size());
      assert(width > 0);
      for (int i = start + 1; i < x.size(); ++i)
        if (not x[i].assigned() and x[i].size() < width) {
          var = i; width = tr(x[var].size());
          assert(width > 0);
        }
      assert(var >= start and var >= 0 and not x[var].assigned());
      values_t values;
      for (GC::Int::ViewValues i(x[var]); i(); ++i)
        values.push_back(i.val());
      assert(not values.empty());
      Branching br(BrStatus::branching, var, values);
      const Timing::Time_point t1 = timing();
      stat->increment_choice(t1-t0);
      // Log node data. Here the number of nodes is not yet updated, so it
      // corresponds to the current node id:
      const count_t nodeid = stat->nodes();
      m->update_log(nodeid, var, values);
      return new BranchingChoice<MinDomValue>(*this, br, stat);
    }

    virtual GC::ExecStatus commit(GC::Space& home, const GC::Choice& c,
                                  const unsigned branch) noexcept {
      BaseBrancher::commit<ModSpace>(home, c, branch);
      [[maybe_unused]] ModSpace* m = &(static_cast<ModSpace&>(home));
      assert(m->status() == GC::SS_BRANCH);
      typedef BranchingChoice<MinDomValue> BrChoice;
      const BrChoice& brc = static_cast<const BrChoice&>(c);
      const Branching& br = brc.br;
      assert(brc.valid() and br.valid());
      const auto& values = br.values;
      const auto var = br.var;
      assert(br.status == BrStatus::branching);
      assert(var >= 0 and not values.empty());
      assert(branch < values.size());
      // Unsatisfiable leaf:
      if (GC::me_failed(x[var].eq(home, values[branch]))) {
        return GC::ES_FAILED;
      }
      // Execute branching:
      return GC::ES_OK;
    }

  };

  // A customised brancher. Branchings are formed by assigning all possible
  // values to all unassigned variables. A branching with minimal domain
  // size is chosen as the best branching. In addition, reduction is used as
  // in lookahead.
  template <class ModSpace>
  class MinDomValueReduction : public BaseBrancher {
  public:

    using BaseBrancher::BaseBrancher;

    static void post(GC::Home home, const IntViewArray& x) noexcept {
      new (home) MinDomValueReduction(home, x);
    }
    virtual GC::Brancher* copy(GC::Space& home) noexcept {
      return new (home) MinDomValueReduction(home, *this);
    }

    virtual GC::Choice* choice(GC::Space& home) noexcept {
      const Timing::UserTime timing;
      const Timing::Time_point t0 = timing();
      assert(valid(start, x));
      ModSpace* m = &(static_cast<ModSpace&>(home));
      assert(m->status() == GC::SS_BRANCH);
      statistics_t stat = m->statistics();
      assert(stat);
      const option_t& options = m->branching_options();
      const BrEagernessO bregr = std::get<BrEagernessO>(options);
      const BrPruneO brpr = std::get<BrPruneO>(options);
      ReduceRes res = (bregr == BrEagernessO::eager) ?
        reduceEager<ModSpace>(home, x, start, brpr, false, stat):
        reduceLazy<ModSpace>(home, x, start, brpr, false, stat);
      // Update the start variable:
      for (auto i = start; i < x.size(); ++i)
        if (not x[i].assigned()) { start = i; break;}
      assert(valid(start, x));
      assert(start < x.size());
      Branching best_br;
      Branching unsat_br(BrStatus::unsat);
      assert(unsat_br.status_val() == BrStatus::unsat);
      if (res.status == BrStatus::unsat) {
        best_br = unsat_br;
        assert(best_br.status_val() == BrStatus::unsat);
      }
      else if (res.status == BrStatus::sat) {
        best_br = Branching(BrStatus::sat, res.var, res.values);
        assert(best_br.status_val() == BrStatus::sat);
      }
      else {
        int var = start;
        size_t width = tr(x[var].size());
        assert(width > 0);
        for (int i = start + 1; i < x.size(); ++i)
          if (not x[i].assigned() and x[i].size() < width) {
            var = i; width = tr(x[var].size());
            assert(width > 0);
          }
        assert(var >= start and var >= 0 and not x[var].assigned());
        values_t values;
        for (GC::Int::ViewValues i(x[var]); i(); ++i)
          values.push_back(i.val());
        assert(not values.empty());
        best_br = Branching(BrStatus::branching, var, values);
      }
      const Timing::Time_point t1 = timing();
      stat->increment_choice(t1-t0);
      return new BranchingChoice<MinDomValueReduction>(*this, best_br, stat);
    }

    virtual GC::ExecStatus commit(GC::Space& home, const GC::Choice& c,
                                  const unsigned branch) noexcept {
      BaseBrancher::commit<ModSpace>(home, c, branch);
      [[maybe_unused]] ModSpace* m = &(static_cast<ModSpace&>(home));
      assert(m->status() == GC::SS_BRANCH);
      typedef BranchingChoice<MinDomValueReduction> BrChoice;
      const BrChoice& brc = static_cast<const BrChoice&>(c);
      Branching br = brc.br;
      assert(brc.valid() and br.valid());
      const auto status = br.status_val();
      const auto var = br.var;
      const auto& values = br.values;
      assert(status == BrStatus::unsat or branch < values.size());
      // Unsatisfiable leaf:
      if (status == BrStatus::unsat or
          GC::me_failed(x[var].eq(home, values[branch]))) {
        return GC::ES_FAILED;
      }
      // Execute branching:
      return GC::ES_OK;
    }

  };

  // A customised brancher. For a variable var and its value val, branching is
  // formed by two branches: var==val and var!=val. The best branching
  // corresponds to a variable with minimal domain size and the minimal
  // variable value.
  template <class ModSpace>
  class MinDomMinValEq : public BaseBrancher {
  public:

    using BaseBrancher::BaseBrancher;

    static void post(GC::Home home, const IntViewArray& x) noexcept {
      new (home) MinDomMinValEq(home, x);
    }
    virtual GC::Brancher* copy(GC::Space& home) noexcept {
      return new (home) MinDomMinValEq(home, *this);
    }

    virtual GC::Choice* choice(GC::Space& home) noexcept {
      ModSpace* m = &(static_cast<ModSpace&>(home));
      assert(m->status() == GC::SS_BRANCH);
      statistics_t stat = m->statistics();
      assert(stat);
      const Timing::UserTime timing;
      const Timing::Time_point t0 = timing();
      assert(valid(start, x));
      int var = start;
      size_t width = tr(x[var].size());
      assert(width > 0);
      for (int i = start + 1; i < x.size(); ++i)
        if (not x[i].assigned() and x[i].size() < width) {
          var = i; width = tr(x[var].size());
          assert(width > 0);
        }
      assert(var >= start and var >= 0 and not x[var].assigned());
      values_t values{x[var].min()};
      assert(values.size() == 1);
      eq_values_t eq_values = {true, false};
      Branching br(BrStatus::branching, var, values, eq_values);
      const Timing::Time_point t1 = timing();
      stat->increment_choice(t1-t0);
      return new BranchingChoice<MinDomMinValEq>(*this, br, stat);
    }

    virtual GC::ExecStatus commit(GC::Space& home, const GC::Choice& c,
                                  const unsigned branch) noexcept {
      BaseBrancher::commit<ModSpace>(home, c, branch);
      [[maybe_unused]] ModSpace* m = &(static_cast<ModSpace&>(home));
      assert(m->status() == GC::SS_BRANCH);
      typedef BranchingChoice<MinDomMinValEq> BrChoice;
      const BrChoice& brc = static_cast<const BrChoice&>(c);
      const Branching& br = brc.br;
      assert(brc.valid() and br.valid());
      assert(br.status == BrStatus::branching);
      assert(br.values.size() == 1);
      const auto var = br.var;
      const auto val = br.values[0];
      const auto& eq_values = br.eq_values;
      assert(var >= 0);
      assert(eq_values.size() == 2);
      assert(branch == 0 or branch == 1);
      if ( (eq_values[branch] == true and GC::me_failed(x[var].eq(home, val))) or
           (eq_values[branch] == false and GC::me_failed(x[var].nq(home, val))) ) {
        return GC::ES_FAILED;
      }
      return GC::ES_OK;
    }

  };

  template <class ModSpace>
  class MinDomMinValEqReduction : public BaseBrancher {
  public:

    using BaseBrancher::BaseBrancher;

    static void post(GC::Home home, const IntViewArray& x) noexcept {
      new (home) MinDomMinValEqReduction(home, x);
    }
    virtual GC::Brancher* copy(GC::Space& home) noexcept {
      return new (home) MinDomMinValEqReduction(home, *this);
    }

    virtual GC::Choice* choice(GC::Space& home) noexcept {
      const Timing::UserTime timing;
      const Timing::Time_point t0 = timing();
      assert(valid(start, x));
      ModSpace* m = &(static_cast<ModSpace&>(home));
      assert(m->status() == GC::SS_BRANCH);
      statistics_t stat = m->statistics();
      assert(stat);
      const option_t& options = m->branching_options();
      const BrEagernessO bregr = std::get<BrEagernessO>(options);
      const BrPruneO brpr = std::get<BrPruneO>(options);
      ReduceRes res = (bregr == BrEagernessO::eager) ?
        reduceEager<ModSpace>(home, x, start, brpr, true, stat):
        reduceLazy<ModSpace>(home, x, start, brpr, true, stat);
      // Update the start variable:
      for (auto i = start; i < x.size(); ++i)
        if (not x[i].assigned()) { start = i; break;}
      assert(valid(start, x));
      assert(start < x.size());
      Branching best_br;
      Branching unsat_br(BrStatus::unsat);
      assert(unsat_br.status_eq() == BrStatus::unsat);
      if (res.status == BrStatus::unsat) {
        best_br = unsat_br;
        assert(best_br.status_eq() == BrStatus::unsat);
      }
      else if (res.status == BrStatus::sat) {
        best_br = Branching(BrStatus::sat, res.var, res.values, {true,false});
        assert(best_br.status_eq() == BrStatus::sat);
      }
      else {
        int var = start;
        size_t width = tr(x[var].size());
        assert(width > 0);
        for (int i = start + 1; i < x.size(); ++i)
          if (not x[i].assigned() and x[i].size() < width) {
            var = i; width = tr(x[var].size());
            assert(width > 0);
          }
        assert(var >= start and var >= 0 and not x[var].assigned());
        values_t values{x[var].min()};
        assert(values.size() == 1);
        best_br = Branching(BrStatus::branching, var, values, {true, false});
      }
      const Timing::Time_point t1 = timing();
      stat->increment_choice(t1-t0);
      return new BranchingChoice<MinDomMinValEqReduction>(*this, best_br, stat);
    }

    virtual GC::ExecStatus commit(GC::Space& home, const GC::Choice& c,
                                  const unsigned branch) noexcept {
      BaseBrancher::commit<ModSpace>(home, c, branch);
      [[maybe_unused]] ModSpace* m = &(static_cast<ModSpace&>(home));
      assert(m->status() == GC::SS_BRANCH);
      typedef BranchingChoice<MinDomMinValEqReduction> BrChoice;
      const BrChoice& brc = static_cast<const BrChoice&>(c);
      Branching br = brc.br;
      const auto status = br.status_eq();
      if (status == BrStatus::unsat) return GC::ES_FAILED;
      assert(branch <= 1);
      const auto var = br.var;
      assert(br.values.size() == 1);
      const auto val = br.values[0];
      const auto& eq_values = br.eq_values;
      assert(var >= 0);
      assert(eq_values.size() == 2);
      assert(branch == 0 or branch == 1);
      if ( (eq_values[branch] == true and GC::me_failed(x[var].eq(home, val))) or
           (eq_values[branch] == false and GC::me_failed(x[var].nq(home, val))) ) {
        return GC::ES_FAILED;
      }
      return GC::ES_OK;
    }

  };


  // A customised LA-based brancher. Branchings are formed by assigning
  // all possible values to all unassigned variables. The best branching
  // is chosen via the tau-function.
  template <class ModSpace>
  class LookaheadValue : public BaseBrancher {
  public:

    using BaseBrancher::BaseBrancher;

    static void post(GC::Home home, const IntViewArray& x) noexcept {
      new (home) LookaheadValue(home, x);
    }
    virtual GC::Brancher* copy(GC::Space& home) noexcept {
      return new (home) LookaheadValue(home, *this);
    }

    virtual GC::Choice* choice(GC::Space& home) noexcept {
      const Timing::UserTime timing;
      const Timing::Time_point t0 = timing();
      ModSpace* m = &(static_cast<ModSpace&>(home));
      assert(m->status() == GC::SS_BRANCH);
      statistics_t stat = m->statistics();
      assert(stat);
      const option_t& options = m->branching_options();
      const BrEagernessO bregr = std::get<BrEagernessO>(options);
      const BrPruneO brpr = std::get<BrPruneO>(options);
      const BrOrderO bro = std::get<BrOrderO>(options);
      ReduceRes res = (bregr == BrEagernessO::eager) ?
        reduceEager<ModSpace>(home, x, start, brpr, false, stat):
        reduceLazy<ModSpace>(home, x, start, brpr, false, stat);
      // Update the start variable:
      for (auto i = start; i < x.size(); ++i)
        if (not x[i].assigned()) { start = i; break;}
      assert(valid(start, x));
      assert(start < x.size());
      ValBranching best_br;
      ValBranching unsat_br(start);
      assert(unsat_br.status() == BrStatus::unsat);
      if (res.status == BrStatus::unsat) {
        best_br = unsat_br;
        assert(best_br.status() == BrStatus::unsat);
      }
      else if (res.status == BrStatus::sat) {
        best_br = ValBranching(res.var, res.values, {});
        assert(best_br.status() == BrStatus::sat);
      }
      else {
        assert(res.status == BrStatus::branching);
        const weights_t wghts = m->weights();
        const statistics_t stat = m->statistics();
        assert(wghts);
        assert(stat);
        std::vector<ValBranching> tau_brs;
        // For remaining variables (all before 'start' are assigned):
        for (int var = start; var < x.size(); ++var) {
          // v is a variable, view is the values in Gecode format:
          const IntView view = x[var];
          // Skip assigned variables:
          if (view.assigned()) continue;
          assert(view.size() >= 2);
          bt_t v_tuple; values_t vls;
          // For all values of the current variable:
          for (IntVarValues j(view); j(); ++j) {
            // Assign value, propagate, and measure:
            const int val = j.val();
            const auto subm = subproblem<ModSpace>(m, var, val, true, stat);
            [[maybe_unused]] const auto subm_st = subm->status();
            assert(subm_st == GC::SS_BRANCH);
            // Calculate distance:
            const float_t dist = distance(m->at(), subm->at(), wghts);
            assert(dist > 0);
            vls.push_back(val);
            v_tuple.push_back(dist);
          }
          ValBranching br(var, vls, v_tuple);
          assert(br.status() == BrStatus::branching);
          tau_brs.push_back(br);
        } // for (int v = start; v < x.size(); ++v) {
        best_br = best_branching<ValBranching>(tau_brs, stat, bro);
      }

      [[maybe_unused]] const auto var = best_br.var;
      assert(var >= 0 and var >= start);
      assert(not x[var].assigned() or best_br.status() == BrStatus::unsat);
      const Timing::Time_point t1 = timing();
      stat->increment_choice(t1-t0);
      return new ValBranchingChoice<LookaheadValue>(*this, best_br, stat);
    }

    virtual GC::ExecStatus commit(GC::Space& home, const GC::Choice& c,
                                  const unsigned branch) noexcept {
      BaseBrancher::commit<ModSpace>(home, c, branch);
      typedef ValBranchingChoice<LookaheadValue> BrChoice;
      const BrChoice& brc = static_cast<const BrChoice&>(c);
      const ValBranching& br = brc.br;
      assert(brc.valid());
      const auto status = br.status();
      const auto var = br.var;
      const auto& values = br.values;
      assert(status == BrStatus::unsat or branch < values.size());
      // If unsatisfiable branching, stop executing:
      if (status == BrStatus::unsat or
          GC::me_failed(x[var].eq(home, values[branch]))) {
        return GC::ES_FAILED;
      }
      // Execute branching:
      return GC::ES_OK;
    }
  };

  // A customised LA-based brancher. For a variable var and its value val,
  // branching is formed by two branches: var==val and
  // var!=val. The best branching is chosen via the tau-function.
  template <class ModSpace>
  class LookaheadEq : public BaseBrancher {
  public:

    using BaseBrancher::BaseBrancher;

    static void post(GC::Home home, const IntViewArray& x) noexcept {
      new (home) LookaheadEq(home, x);
    }
    virtual GC::Brancher* copy(GC::Space& home) noexcept {
      return new (home) LookaheadEq(home, *this);
    }

    virtual GC::Choice* choice(GC::Space& home) noexcept {
      const Timing::UserTime timing;
      const Timing::Time_point t0 = timing();
      ModSpace* m = &(static_cast<ModSpace&>(home));
      assert(m->status() == GC::SS_BRANCH);
      statistics_t stat = m->statistics();
      assert(stat);
      const option_t& options = m->branching_options();
      const BrEagernessO bregr = std::get<BrEagernessO>(options);
      const BrPruneO brpr = std::get<BrPruneO>(options);
      ReduceRes res = (bregr == BrEagernessO::eager) ?
        reduceEager<ModSpace>(home, x, start, brpr, true, stat):
        reduceLazy<ModSpace>(home, x, start, brpr, true, stat);
      // Update the start variable:
      for (auto i = start; i < x.size(); ++i)
        if (not x[i].assigned()) { start = i; break;}
      assert(valid(start, x));
      assert(start < x.size());
      EqBranching best_br;
      EqBranching unsat_br(start);
      assert(unsat_br.status() == BrStatus::unsat);
      if (res.status == BrStatus::unsat) {
        best_br = unsat_br;
        assert(best_br.status() == BrStatus::unsat);
      }
      else if (res.status == BrStatus::sat) {
        assert(res.values.size() == 1);
        best_br = EqBranching(res.var, res.values[0], {true,false});
        assert(best_br.status() == BrStatus::sat);
      }
      else {
        assert(res.status == BrStatus::branching);
        assert(m->status() == GC::SS_BRANCH);
        std::vector<EqBranching> tau_brs;
        const weights_t wghts = m->weights();
        assert(wghts);

        for (int var = start; var < x.size(); ++var) {
          const IntView view = x[var];
          if (view.assigned()) continue;
          assert(view.size() >= 2);
          for (IntVarValues j(view); j(); ++j) {
            const int val = j.val();
            const auto subm_eq = subproblem<ModSpace>(m, var, val, true, stat);
            [[maybe_unused]] const auto subm_eq_st = subm_eq->status();
            assert(subm_eq_st == GC::SS_BRANCH);
            const float_t dist1 = distance(m->at(), subm_eq->at(), wghts);
            assert(dist1 > 0);
            const auto subm_neq = subproblem<ModSpace>(m, var, val, false, stat);
            [[maybe_unused]] const auto subm_neq_st = subm_neq->status();
            assert(subm_neq_st == GC::SS_BRANCH);
            const float_t dist2 = distance(m->at(), subm_neq->at(), wghts);
            assert(dist2 > 0);
            EqBranching br(var, val, {true,false}, {dist1,dist2});
            assert(br.status() == BrStatus::branching);
            tau_brs.push_back(br);
          }
        }
        assert(not tau_brs.empty());
        best_br = best_branching<EqBranching>(tau_brs, stat);
      }

      [[maybe_unused]] const auto var = best_br.var;
      assert(var >= 0 and var >= start);
      assert(not x[var].assigned() or best_br.status() == BrStatus::unsat);
      const Timing::Time_point t1 = timing();
      stat->increment_choice(t1-t0);
      return new EqBranchingChoice<LookaheadEq>(*this, best_br, stat);
    }

    virtual GC::ExecStatus commit(GC::Space& home, const GC::Choice& c,
                                  const unsigned branch) noexcept {
      BaseBrancher::commit<ModSpace>(home, c, branch);
      [[maybe_unused]] ModSpace* m = &(static_cast<ModSpace&>(home));
      assert(m->status() == GC::SS_BRANCH);
      typedef EqBranchingChoice<LookaheadEq> BrChoice;
      const BrChoice& brc = static_cast<const BrChoice&>(c);
      const EqBranching& br = brc.br;
      if (br.status() == BrStatus::unsat) return GC::ES_FAILED;
      const auto var = br.var;
      const auto& val = br.value;
      const auto& brvalues = br.brvalues;
      assert(var >= 0);
      assert(brvalues.size() == 1 or brvalues.size() == 2);
      assert(branch == 0 or branch == 1);
      assert(branch < brvalues.size());
      if ( (brvalues[branch] == true and GC::me_failed(x[var].eq(home, val))) or
           (brvalues[branch] == false and GC::me_failed(x[var].nq(home, val))) ) {
        return GC::ES_FAILED;
      }
      return GC::ES_OK;
    }

  };

  // A customised LA-based brancher for finding one solution. For a variable var,
  // branching is formed by eq-branches and val-branches.
  template <class ModSpace>
  class LookaheadEqVal : public BaseBrancher {
    public:

    using BaseBrancher::BaseBrancher;

    static void post(GC::Home home, const IntViewArray& x) noexcept {
      new (home) LookaheadEqVal(home, x);
    }
    virtual GC::Brancher* copy(GC::Space& home) noexcept {
      return new (home) LookaheadEqVal(home, *this);
    }

    virtual GC::Choice* choice(GC::Space& home) noexcept {
      const Timing::UserTime timing;
      const Timing::Time_point t0 = timing();
      ModSpace* m = &(static_cast<ModSpace&>(home));
      assert(m->status() == GC::SS_BRANCH);
       statistics_t stat = m->statistics();
      assert(stat);
      const option_t& options = m->branching_options();
      const BrEagernessO bregr = std::get<BrEagernessO>(options);
      const BrPruneO brpr = std::get<BrPruneO>(options);
      ReduceRes res = (bregr == BrEagernessO::eager) ?
        reduceEager<ModSpace>(home, x, start, brpr, false, stat):
        reduceLazy<ModSpace>(home, x, start, brpr, false, stat);
      for (auto i = start; i < x.size(); ++i)
        if (not x[i].assigned()) { start = i; break;}
      assert(valid(start, x));
      assert(start < x.size());
      Branching best_br;
      Branching unsat_br(BrStatus::unsat, start);
      assert(unsat_br.status_val() == BrStatus::unsat);
      if (res.status == BrStatus::unsat) {
        best_br = unsat_br;
        assert(best_br.status_val() == BrStatus::unsat);
      }
      else if (res.status == BrStatus::sat) {
        best_br = Branching(BrStatus::sat, res.var, res.values);
        assert(best_br.status_val() == BrStatus::sat);
      }
      else {
        assert(res.status == BrStatus::branching);
        const weights_t wghts = m->weights();
        assert(wghts);
        std::vector<Branching> tau_brs;

        for (int var = start; var < x.size(); ++var) {
          const IntView view = x[var];
          if (view.assigned()) continue;
          assert(view.size() >= 2);
          bt_t v_tuple;
          values_t vls;
          for (IntVarValues j(view); j(); ++j) {
            const int val = j.val();
            const auto subm_eq = subproblem<ModSpace>(m, var, val, true, stat);
            [[maybe_unused]] const auto subm_eq_st = subm_eq->status();
            assert(subm_eq_st == GC::SS_BRANCH);
            const float_t dist1 = distance(m->at(), subm_eq->at(), wghts);
            assert(dist1 > 0);
            vls.push_back(val); v_tuple.push_back(dist1);
            const auto subm_neq = subproblem<ModSpace>(m, var, val, false, stat);
            [[maybe_unused]] const auto subm_neq_st = subm_neq->status();
            assert(subm_neq_st == GC::SS_BRANCH);
            const float_t dist2 = distance(m->at(), subm_neq->at(), wghts);
            assert(dist2 > 0);
            Branching br(BrStatus::branching, var, {val}, {true,false}, {}, {dist1,dist2});
            assert(br.status_eq() == BrStatus::branching);
            tau_brs.push_back(br);
          }
          Branching br(BrStatus::branching, var, vls, {}, v_tuple);
          assert(br.status_val() == BrStatus::branching);
          tau_brs.push_back(br);
        }
        best_br = best_branching(tau_brs, stat);
      }

      [[maybe_unused]] const auto var = best_br.var;
      assert(var >= 0 and var >= start);
      assert(not x[var].assigned() or best_br.status_eq() == BrStatus::unsat
             or best_br.status_val() == BrStatus::unsat);
      const Timing::Time_point t1 = timing();
      stat->increment_choice(t1-t0);
      return new BranchingChoice<LookaheadEqVal>(*this, best_br, stat);
    }

    virtual GC::ExecStatus commit(GC::Space& home, const GC::Choice& c,
                                  const unsigned branch) noexcept {
      BaseBrancher::commit<ModSpace>(home, c, branch);
      [[maybe_unused]] ModSpace* m = &(static_cast<ModSpace&>(home));
      assert(m->status() == GC::SS_BRANCH);
      typedef BranchingChoice<LookaheadEqVal> BrChoice;
      const BrChoice& brc = static_cast<const BrChoice&>(c);
      const Branching& br = brc.br;
      assert(brc.valid() and br.valid());
      if (br.status == BrStatus::unsat) return GC::ES_FAILED;
      const auto var = br.var;
      const auto& values = br.values;
      const auto& eq_values = br.eq_values;
      assert(var >= 0);
      assert(not values.empty() or not eq_values.empty());
      // Equality-branching:
      if (not eq_values.empty()) {
        assert(values.size() == 1);
        assert(branch == 0 or branch == 1);
        assert(eq_values.size() == 1 or eq_values.size() == 2);
        const auto val = values[0];
        if ( (eq_values[branch] == true and GC::me_failed(x[var].eq(home, val))) or
             (eq_values[branch] == false and GC::me_failed(x[var].nq(home, val))) ) {
          return GC::ES_FAILED;
        }
      }
      // Value-branching:
      else {
        assert(not values.empty());
        assert(branch < values.size());
        if ( GC::me_failed(x[var].eq(home, values[branch])) ) {
          return GC::ES_FAILED;
        }
      }

      return GC::ES_OK;
    }

  };

  template <class ModSpace>
  inline void post_branching(GC::Home home, const GC::IntVarArgs& V,
                             const option_t& options) noexcept {
    assert(not home.failed());
    const BrTypeO brt = std::get<BrTypeO>(options);
    const BrSourceO brsrc = std::get<BrSourceO>(options);
    //const BrSolutionO brsln = std::get<BrSolutionO>(options);
    const IntViewArray y(home, V);
    if (brt == BrTypeO::mind and brsrc == BrSourceO::eq) {
      MinDomMinValEq<ModSpace>::post(home, y); }
    else if (brt == BrTypeO::mind and brsrc == BrSourceO::val) {
      MinDomValue<ModSpace>::post(home, y);
    }
    else if (brt == BrTypeO::mind and brsrc == BrSourceO::eqval) {
      MinDomMinValEq<ModSpace>::post(home, y);
    }
    else if (brt == BrTypeO::mindr and brsrc == BrSourceO::eq) {
       MinDomMinValEqReduction<ModSpace>::post(home, y);
    }
    else if (brt == BrTypeO::mindr and brsrc == BrSourceO::val) {
       MinDomValueReduction<ModSpace>::post(home, y);
    }
    else if (brt == BrTypeO::mindr and brsrc == BrSourceO::eqval) {
       MinDomMinValEqReduction<ModSpace>::post(home, y);
    }
    else if (brt == BrTypeO::la) {
      if (brsrc == BrSourceO::eq) {
        LookaheadEq<ModSpace>::post(home, y);
      }
      else if (brsrc == BrSourceO::val) {
        LookaheadValue<ModSpace>::post(home, y);
      }
      else if (brsrc == BrSourceO::eqval) {
        LookaheadEqVal<ModSpace>::post(home, y);
      }
    }
  }

  template <class ModSpace>
  void find_all_solutions(const std::unique_ptr<ModSpace>& m,
                          const bool print = false,
                          const unsigned long maxunsatleaves = 0,
                          statistics_t stat = nullptr) noexcept {
    assert(m->valid());
    assert(stat);
    // Set limit on maximal unsat leaves if given:
    GC::Search::Options opt;
    if (maxunsatleaves > 0) {
      GC::Search::Stop* stp = GC::Search::Stop::fail(maxunsatleaves);
      opt.stop = stp;
    }
    // c_d stands for commit distance. A clone is created after every c_d
    // commits. By default c_d=8. If c_d=1, clone is created for each commit,
    // i.e. for each child node of the backtracking tree.
    opt.c_d = 1;
    GC::DFS<ModSpace> e(m.get(), opt);
    typedef std::unique_ptr<ModSpace> node_ptr;
    while (const node_ptr s{e.next()}) {
      if (print) s->print();
      stat->increment_solutions();
    }
    stat->set_gecode_stat(e.statistics());
  }
  template <class ModSpace>
  void find_one_solution(const std::unique_ptr<ModSpace>& m,
                         const bool print = false,
                         const unsigned long maxunsatleaves = 0,
                         statistics_t stat = nullptr) noexcept {
    assert(m->valid());
    assert(stat);
    // Set limit on maximal unsat leaves if given:
    GC::Search::Options opt;
    if (maxunsatleaves > 0) {
      GC::Search::Stop* stp = GC::Search::Stop::fail(maxunsatleaves);
      opt.stop = stp;
    }
    opt.c_d = 1;
    GC::DFS<ModSpace> e(m.get(), opt);
    typedef std::unique_ptr<ModSpace> node_ptr;
    if (const node_ptr s{e.next()}) {
      if (print) s->print();
      stat->increment_solutions();
    }
    stat->set_gecode_stat(e.statistics());
  }
  template <class ModSpace>
  void solve(const std::unique_ptr<ModSpace>& m,
             const bool printsol = false,
             const unsigned long maxunsatleaves = 0,
             statistics_t stat = nullptr) noexcept {
    assert(m->valid());
    assert(stat);
    auto const st = m->status();
    if (st == GC::SS_FAILED) stat->increment_unsat_leaves();
    const option_t options = m->branching_options();
    const BrSolutionO brsln = std::get<BrSolutionO>(options);
    switch (brsln) {
    case BrSolutionO::all : find_all_solutions(m, printsol, maxunsatleaves, stat); break;
    default : find_one_solution(m, printsol, maxunsatleaves, stat);}
  }

}

#endif
