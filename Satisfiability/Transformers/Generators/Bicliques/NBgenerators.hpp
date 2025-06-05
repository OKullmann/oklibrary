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
#include <Numerics/NumBasicFunctions.hpp>
#include <Transformers/Generators/Random/GenClauseSets.hpp>

#include "Algorithms.hpp"
#include "GenConflictGraphs.hpp"

namespace NBgenerators {

  namespace GL = GenLit;
  namespace GC = GenClauses;
  namespace GCS = GenClauseSets;

  GCS::gclauseset_t full_range(const GC::GClause& C, const GC::GClause& D,
                               const GL::var_t mult = 1) {
    if (not varequal_valleq(C,D) or mult == 0) return {};
    const GL::var_t size = [&C,&D]{GL::var_t res = 1;
                                   for (GL::var_t i = 0; i < C.size(); ++i)
                                     res *= val(D[i])-val(C[i]);
                                   return res;}();
    if (size == 0) return {};
    GCS::gclauseset_t res; res.reserve(mult*size);
    GC::gclause_t current(C.C);
    do
      if (mult == 1) res.push_back(current);
      else res.insert(res.end(), mult, current);
    while (Sampling::next_combination(current, C.C, D.C));
    return res;
  }
  constexpr FloatingPoint::float80
  numcl_full_nbcls(const GL::var_t n, const GL::val_t dom,
                   const GL::var_t mult) noexcept {
    return FloatingPoint::pow(dom,n) * mult;
  }
  GCS::GClauseList full_nbcls(const GL::var_t n, const GL::val_t dom,
                              const GL::var_t mult = 1) {
    GCS::GClauseList res; res.dom.assign(n, dom);
    if ((dom == 0 and n >= 1) or mult == 0) return res;
    GC::GClause C, D; C.C.reserve(n); D.C.reserve(n);
    for (GL::var_t i = 0; i < n; ++i) {
      C.push_back({i,0}); D.push_back({i,dom});
    }
    res.F = full_range(C,D,mult);
    assert(res.c() == numcl_full_nbcls(n,dom,mult));
    assert((mult == 1 and res.is_clauseset()) or res.is_fully_standardised());
    return res;
  }

}

#endif
