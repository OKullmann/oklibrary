// Oliver Kullmann, 13.2.2024 (Swansea)
/* Copyright 2024 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  At-least-one, At-most-one and exactly-one in various forms

   - amo_primes(ostream, Clause)
   - alo_primes(ostream, Clause)
   - eo_primes(ostream, Clause)

   - disj_impl(ostream, Clause, Lit) (clause implies literal)

*/

#ifndef ALOAMO_9gVdQiL7EV
#define ALOAMO_9gVdQiL7EV

#include <ostream>

#include "../Random/ClauseSets.hpp"

#ifndef INCCLAUSE
# define INCCLAUSE
#endif

namespace AloAmo {

  using RandGen::Var;
  using RandGen::Lit;
  using RandGen::Clause;

  void amo_primes(std::ostream& out, const Clause& C) {
    if (C.size() >= 2) {
      auto current_end = C.cbegin(); ++current_end;
      do {
        const Lit y = -*current_end;
        for (auto i = C.cbegin(); i != current_end; ++i) {
          out << Clause{-*i, y}; INCCLAUSE;
        }
      } while (++current_end != C.end());
    }
  }
  void alo_primes(std::ostream& out, const Clause& C) {
    out << C; INCCLAUSE;
  }
  void eo_primes(std::ostream& out, const Clause& C) {
    amo_primes(out, C);
    alo_primes(out, C);
  }

  // The disjunction over B implies w:
  inline void disj_impl(std::ostream& out, const Clause& B, const Lit w) {
    for (const Lit& x : B) { out << Clause{-x, w}; INCCLAUSE; }
  }

}

#endif
