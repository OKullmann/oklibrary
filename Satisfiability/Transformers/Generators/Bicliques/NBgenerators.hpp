// Oliver Kullmann, 4.6.2025 (Swansea)
/* Copyright 2025 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Generators for nonbolean (multi-)clause-sets

*/

#ifndef NBGENERATORS_AoX6P27dqY
#define NBGENERATORS_AoX6P27dqY

#include <Numerics/Algorithms.hpp>
#include <Transformers/Generators/Random/GenClauseSets.hpp>

#include "Algorithms.hpp"
#include "GenConflictGraphs.hpp"

namespace NBgenerators {

  namespace GL = GenLit;
  namespace GC = GenClauses;
  namespace GCS = GenClauseSets;

  GCS::gclauseset_t full_range(const GC::GClause& C, const GC::GClause& D) {
    if (not varequal_valleq(C,D)) return {};
    const GL::var_t size = [&C,&D]{GL::var_t res = 1;
                                   for (GL::var_t i = 0; i < C.size(); ++i)
                                     res *= val(D[i])-val(C[i]);
                                   return res;}();
    if (size == 0) return {};
    GCS::gclauseset_t res; res.reserve(size);
    GC::gclause_t current(C.C);
    do res.push_back(current);
    while (Sampling::next_combination(current, C.C, D.C));
    return res;
  }
  GCS::GClauseList full_nbcls(const GL::var_t n, const GL::val_t dom) {
    GC::GClause C, D; C.C.reserve(n); D.C.reserve(n);
    for (GL::var_t i = 0; i < n; ++i) {
      C.push_back({i,0}); D.push_back({i,dom});
    }
    return GCS::GClauseList(full_range(C,D));
  }

}

#endif
