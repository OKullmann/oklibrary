// Oliver Kullmann, 15.2.2019 (Swansea)
/* Copyright 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* Components related to translations */

#ifndef TRANSLATIONS_PMRf27zyz5
#define TRANSLATIONS_PMRf27zyz5

#include <cassert>

#include "VarLit.hpp"
#include "ClauseSets.hpp"
#include "Encodings.hpp"

namespace Translations {

  struct Translation {

    using Var = VarLit::Var;
    using Lit = VarLit::Lit;
    using Pol = VarLit::Pol;
    using Clause = ClauseSets::Clause;
    using Encoding = Encodings::Encoding;

    const ClauseSets::DClauseSet& F;
    const Encoding& enc;

    mutable ClauseSets::Count_t c_cs=0, c_palr=0, c_parl=0, c_P=0, c_N=0, c_amo=0, litocc=0;

    Translation(const ClauseSets::DClauseSet& F, const Encoding& enc) noexcept : F(F), enc(enc) {}

    ClauseSets::CLS operator()() const {
      ClauseSets::CLS G;
      // Non-triviality clause:
      {Clause C;
       for (Encoding::clause_index_t i = 0; i < enc.ncs; ++i)
         C.insert(Lit(enc.csvar(i), Pol::p));
       assert(C.size() == enc.ncs);
       G.push_back(std::move(C)); ++c_cs; litocc += enc.ncs;
      }
      // Defining the pass's:
      {for (auto it = enc.all_solutions.first.begin(); it != enc.all_solutions.first.end(); ++it) {
        const Encoding::Pass_p phi_p = &*it;
        const ClauseSets::Pass& phi = *it;
        const Var tphi = enc.pavar(phi_p);
        // from left to right, i.e., t(phi) -> and_{v in var(phi)} t(v,phi(v)):
        {const Lit negtphi = Lit(tphi, Pol::n);
         for (const auto& pair : phi) {
           Clause C; C.insert(negtphi);
           C.insert(Lit(enc.bfvar(pair.first, pair.second), Pol::p));
           assert(C.size() == 2);
           G.push_back(std::move(C)); ++c_palr; litocc += 2;
         }
        }
        // from right to left, i.e., (and_{v in var(phi)} t(v,phi(v))) -> t(phi):
        {Clause C; C.insert(Lit(tphi,Pol::p));
         for (const auto& pair : phi)
           C.insert(Lit(enc.bfvar(pair.first, pair.second), Pol::n));
         assert(C.size() ==  1 + phi.size());
         G.push_back(std::move(C)); ++c_parl; litocc += 1 + phi.size();
        }
       }
      }
      {for (Encoding::clause_index_t i = 0; i < enc.ncs; ++i) {
         // t(C) -> P(C):
         const Var tc = enc.csvar(i);
         {Clause C; C.insert(Lit(tc,Pol::n));
          const auto& S = enc.all_solutions.second[i];
          for (const Encoding::Pass_p& phi_p : S)
            C.insert(Lit(enc.pavar(phi_p),Pol::p));
          assert(C.size() == 1 + S.size());
          G.push_back(std::move(C)); ++c_P; litocc += 1 + S.size();
         }
         // -t(C) -> N(C):
         for (const Lit x : enc.dclauses[i]->P.second) {
           const Var v = enc.E_index[var(x)];
           assert(v < F.ne);
           for (Var bfi = enc.bfvar_indices[v]; bfi < enc.bfvar_indices[v+1]; ++bfi) {
             Clause C; C.insert(Lit(tc,Pol::p)); C.insert(Lit(bfi,Pol::n));
             assert(C.size() == 2);
             G.push_back(std::move(C)); ++c_N; litocc += 2;
           }
         }
       }
      }
      // Amo-clauses for bf-variables:
      {for (Var i = 0; i < F.ne; ++i) {
         const Var beg = enc.bfvar_indices[i], end = enc.bfvar_indices[i+1];
         for (Var v = beg; v < end; ++v)
           for (Var w = v+1; w < end; ++w) {
             Clause C; C.insert(Lit(v,Pol::n)), C.insert(Lit(w,Pol::n));
             assert(C.size() == 2);
             G.push_back(std::move(C)); ++c_amo; litocc += 2;
           }
       }
      }
      return G;
    }

  };

}

#endif
