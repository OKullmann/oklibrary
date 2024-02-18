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
   - disj_equiv(ostream, Clause, Lit) (clause equivalent literal)

*/

#ifndef ALOAMO_9gVdQiL7EV
#define ALOAMO_9gVdQiL7EV

#include <ostream>

#include <cassert>

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
  // The disjunction over B is equivalent to w:
  inline void disj_equiv(std::ostream& out, const Clause& B, const Lit w) {
    disj_impl(out, B, w);
    out << -w << " "; out << B; INCCLAUSE;
  }

  // As seco_amov2cl(L,V) in CardinalityConstraints.mac;
  // object enc of NVAR: enc() returns new variable:
  template <class NVAR>
  void amo_seco(std::ostream& out, Clause C, const NVAR& enc) {
    Clause B(3);
    while (C.size() > 4) {
      const Lit w{enc(), 1};
      B.assign(C.end()-3, C.end());
      C.resize(C.size()-3);
      C.push_back(w);
      amo_primes(out, B);
      disj_impl(out, B, w);
    }
    amo_primes(out, C);
  }
  // Combining seco_amovuep2cl(L,V) and seco_amouep_co(L)
  // from CardinalityConstraints.mac:
  template <class NVAR>
  RandGen::var_t amouep_seco(std::ostream& out, Clause C, const NVAR& enc) {
    RandGen::var_t final_v = 0;
    Clause B(3);
    while (C.size() > 4) {
      final_v = enc();
      const Lit w{final_v, 1};
      B.assign(C.end()-3, C.end());
      C.resize(C.size()-3);
      C.push_back(w);
      amo_primes(out, B);
      disj_equiv(out, B, w);
    }
    amo_primes(out, C);
    return final_v;
  }
  template <class NVAR>
  void eo_seco(std::ostream& out, const Clause& C, const NVAR& enc) {
    amo_seco(out, C, enc);
    out << C; INCCLAUSE;
  }

  template <class NVAR>
  void eouep_seco(std::ostream& out, const Clause& C, const NVAR& enc) {
    const RandGen::var_t final_v = amouep_seco(out, C, enc);
    if (final_v == 0) {
      assert(C.size() <= 4);
      out << C; INCCLAUSE;
    }
    else {
      assert(C.size() >= 5);
      const RandGen::var_t missing = C.size()%2==1 ? 2 : 3;
      out << Lit{final_v,1} << " " << Clause(C.begin(), C.begin()+missing);
      INCCLAUSE;
    }
  }

}

#endif
