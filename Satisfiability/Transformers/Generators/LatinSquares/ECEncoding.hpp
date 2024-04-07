// Oliver Kullmann, 6.4.2024 (Swansea)
/* Copyright 2024 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Components related to encoding and translating problems based on
  Queens-cubes.

*/

#ifndef ECENCODING_0diQ1XZHGf
#define ECENCODING_0diQ1XZHGf

#include <cassert>

#include <Numerics/NumTypes.hpp>
#include <Numerics/Conversions.hpp>

#include "Statistics.hpp"
#include "PQEncoding.hpp"
#include "Algorithms.hpp"
#include "AloAmo.hpp"

namespace ECEncoding {

  using Statistics::dim_t;
  using Statistics::var_t;

  using Algorithms::qplaces;
  using Algorithms::Cubing_t;

  struct EC0Encoding {
    const Cubing_t& C;
    const PQOptions::CT ct;
    // For convenience:
    const var_t N = C.N, m = C.m;

    const var_t n0 = N * m; // primary variables
    const var_t naux = N *
      FloatingPoint::toUInt(PQEncoding::n_amoaloeo(m, ct, PQOptions::CF::eo));
    const var_t n = n0 + naux;
    const var_t ceo =  N *
      FloatingPoint::toUInt(PQEncoding::c_amoaloeo(m, ct, PQOptions::CF::eo));
    const var_t cbin = count_clashes(C);
    const var_t c = ceo + cbin;
    const Statistics::dimacs_pars dp{n,c};

  protected :
    mutable var_t next = n0;
  public :

    EC0Encoding(const Cubing_t& C, const PQOptions::CT ct) noexcept :
    C(C), ct(ct) {
      assert(C.valid());
#ifndef NDEBUG
      const FloatingPoint::float80 Nf = N;
      assert(Nf * m + Nf * PQEncoding::n_amoaloeo(m, ct, PQOptions::CF::eo)
             == n);
      assert(Nf * PQEncoding::c_amoaloeo(m, ct, PQOptions::CF::eo) + cbin
             == c);
#endif
    }
    static var_t count_clashes(const Cubing_t& C) noexcept {
      if (C.N <= 1 or C.m <= 1) return 0;
      Statistics::fdimacs_pars::float_t res = 0;
      for (var_t i = 0; i < C.N-1; ++i)
        for (var_t cui = 0; cui < C.m; ++cui) {
          const qplaces p1{i,cui};
          for (var_t j = i+1; j < C.N; ++j)
            for (var_t cuj = 0; cuj < C.m; ++cuj)
              res += (cui != cuj) and not C.disjoint(p1, {j,cuj});
        }
      assert(res <= FloatingPoint::P264m1);
      return res;
    }

    var_t operator()() const noexcept {
      return ++next;
    }

    // The primary variables (choose solution p.cu for digit p.co):
    var_t operator()(const qplaces& p) const noexcept {
      assert(C.valid(p));
      return 1 + m*p.co + p.cu;
    }

  };


  template <class ENC>
  void eocubes(std::ostream& out, const ENC& enc) {
    for (var_t i = 0; i < enc.N; ++i) {
      AloAmo::Clause C; C.reserve(enc.m);
      for (var_t j = 0; j < enc.m; ++j)
        C.push_back(enc({i,j}));
      PQEncoding::eo(out, C, enc);
    }
  }
  template <class ENC>
  void clashes(std::ostream& out, const ENC& enc) {
    if (enc.N <= 1 or enc.m <= 1) return;
    for (var_t i = 0; i < enc.N-1; ++i)
      for (var_t cui = 0; cui < enc.m; ++cui) {
        const qplaces p1{i,cui};
        for (var_t j = i+1; j < enc.N; ++j)
          for (var_t cuj = 0; cuj < enc.m; ++cuj) {
            if (cui == cuj) continue;
            const qplaces p2{j,cuj};
            if (not enc.C.disjoint(p1,p2)) {
              using AloAmo::Lit;
              out << AloAmo::Clause{-Lit(enc(p1)),-Lit(enc(p2))};
#ifndef NDEBUG
              ++running_counter;
#endif
            }
          }
      }
  }

  void ecsat0(std::ostream& out, const EC0Encoding& enc) {
    out << enc.dp;
    eocubes(out, enc);
    clashes(out, enc);
#ifndef NDEBUG
    assert(running_counter == enc.c);
#endif
  }

}

#endif
