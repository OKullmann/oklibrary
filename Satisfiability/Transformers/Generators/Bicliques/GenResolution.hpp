// Oliver Kullmann, 28.5.2025 (Swansea)
/* Copyright 2025 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Resolution for nonbolean clause-sets

  Tested in TestConflictGraphs.cpp.

TODOS:

0. Structures/Sets/SetAlgorithms/Subsumption.hpp :
  - Remove usage of boost/iterator/reverse_iterator and boost/mpl/if there
    (assuming that this should be easy for C++23).

1. Require for resolvent and ntresolvent that we have
    ranges of clauses.

*/

#ifndef GENRESOLUTION_QQznh3AQdm
#define GENRESOLUTION_QQznh3AQdm

#include <vector>
#include <algorithm>

#include <cassert>

#include <Transformers/Generators/Random/GenClauseSets.hpp>
#include <Numerics/Algorithms.hpp>

#include <Structures/Sets/SetAlgorithms/Subsumption.hpp>

#include "Algorithms.hpp"
#include "GenConflictGraphs.hpp"

namespace GenResolution {

  namespace GL = GenLit;
  namespace GC = GenClauses;
  namespace GCS = GenClauseSets;
  namespace GCG = GenConflictGraphs;


  // Remove from each F[i] the literal (v,i), and return the union;
  // works for a range of sorted GClauses, without assumptions on them;
  // std::set_union is used, and thus for repetitions the max-occurrences over
  // the F[i] is used:
  template <std::ranges::random_access_range RAN>
    requires std::ranges::sized_range<RAN>
  GC::GClause resolvent(const RAN& F, const GL::var_t v) {
    const auto D = F.size();
    if (D == 0) return {};
    using Algorithms::pointed_view;
    typedef decltype(pointed_view(F[0], 0)) range_t;
    std::vector<range_t> pointed_clauses; pointed_clauses.reserve(D);
    for (GL::val_t d = 0; d < D; ++d)
      pointed_clauses.push_back(pointed_view(F[d], F[d].isearch({v,d})));
    GC::GClause res(std::ranges::to<GC::gclause_t>(pointed_clauses[0]));
    for (GL::val_t d = 1; d < D; ++d) {
      GC::GClause temp;
      std::ranges::set_union(res, pointed_clauses[d], std::back_inserter(temp));
      res = std::move(temp);
    }
    return res;
  }

  // Returns the totally-singular clause in case of a tautology:
  template <std::ranges::random_access_range RAN>
    requires std::ranges::sized_range<RAN>
  GC::GClause ntresolvent(const RAN& F, const GL::var_t v) {
    const GC::GClause res = resolvent(F, v);
    if (GC::tautological(res)) return GC::totsingcl();
    else return res;
  }


  // Compute all resolution-combinations over resolution-variable v
  // with the clause-indices (into F) given by O (O.size() is assumed the
  // domain-size of v), in anti-lexicographical order:
  std::vector<GC::GClause> all_resolution_combinations(
      const GCG::GOccVar::var_occ_t& O,
      const GL::var_t v,
      const std::vector<GC::GClause>& F,
      const bool nt = true) {
    const auto D = O.size();
    const auto c = F.size();
    const auto cresult = Algorithms::prod_sizes(O);
    if (D == 0 or c == 0 or cresult == 0) return {};
    typedef GCG::GOccVar::lit_occ_t::const_iterator iterator;
    typedef std::vector<iterator> block_iterator;
    const auto [begin, end] = Algorithms::extract_beginend(O);
    block_iterator current = begin;
    std::vector<GC::GClause> res; if (not nt) res.reserve(cresult);
    do {
      const std::vector<GC::GClause> R =
        [c, &current, &F]{std::vector<GC::GClause> res;
                          res.reserve(current.size());
                          for (const auto& it : current) {assert(*it<c); res.push_back(F[*it]);}
                          return res;}();
      if (nt) {
        auto C = ntresolvent(R, v);
        if (not GenClauses::totsing(C)) res.push_back(std::move(C));
      }
      else res.push_back(resolvent(R, v));
    }
    while (Sampling::next_combination(current, begin, end));
    return res;
  }
  std::vector<GC::GClause> all_resolution_combinations(
      const GL::var_t v, const GCS::GClauseList& F,  const bool nt = true) {
    const GCG::GOccVar O(F,v);
    return all_resolution_combinations(O.O, v, F.F, nt);
  }


  typedef std::vector<GL::var_t> varlist_t;
  std::array<GCS::count_t, 4> DP_reduction(GCS::GClauseList& F,
                                           const varlist_t& V,
                                           const bool clauseset = false,
                                           const bool subsumption_elimination = false) {
    assert(F.is_fully_standardised());
    const auto n = F.n();
    GCS::count_t total_resolvents = 0, total_degrees = 0, total_duplicates = 0,
      subsumed = 0;
    for (const GL::var_t v : V) {
      if (v >= n) continue;
      const GCG::GOccVar O(F,v);
      if (O.dom() == 0) continue;
      total_degrees += O.deg();
      auto resolvents = all_resolution_combinations(O.O, v, F.F);
      total_resolvents += resolvents.size();
      Algorithms::erase_indices(F.F, O.all_occurrences());
      // F.F.append_range(resolvents | std::views::as_rvalue); ERROR with GCC 14.2 :
      F.F.insert(F.F.end(), std::make_move_iterator(resolvents.begin()),
                 std::make_move_iterator(resolvents.end()));
      F.sort_clauselist(); // ?? here "eager"; in case of subsumption_elimination one could also run subsumption_elimination here
      if (clauseset) total_duplicates += F.remove_consecutive_duplicated_clauses();
      F.dom[v] = 0;
    }
    if (subsumption_elimination) {
      if (not clauseset)
        total_duplicates += F.remove_consecutive_duplicated_clauses();
      const GCS::count_t old_size = F.c();
      Algorithms::forward_strictsubsumption_by_erase(F.F);
      // Remark: Since F.F is sorted, only forward-subsumption needed.
      subsumed = old_size - F.c();
    }
    return {total_resolvents, total_degrees, total_duplicates, subsumed};
  }

}

#endif
