// Oliver Kullmann, 28.5.2025 (Swansea)
/* Copyright 2025 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Resolution for nonbolean clause-sets

  Tested in TestConflictGraphs.cpp.

*/

#ifndef GENRESOLUTION_QQznh3AQdm
#define GENRESOLUTION_QQznh3AQdm

#include <vector>

#include <Transformers/Generators/Random/GenClauseSets.hpp>

#include "Algorithms.hpp"
#include "GenConflictGraphs.hpp"

namespace GenResolution {

  namespace GL = GenLit;
  namespace GC = GenClauses;
  namespace GCS = GenClauseSets;


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

}

#endif
