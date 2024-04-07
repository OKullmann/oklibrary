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
      FloatingPoint::toUInt(PQEncoding::n_amoaloeo(N, ct, PQOptions::CF::eo));
    const var_t n = n0 + naux;
    const var_t ceo =  N *
      FloatingPoint::toUInt(PQEncoding::c_amoaloeo(N, ct, PQOptions::CF::eo));
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
      assert(Nf * m + Nf * PQEncoding::n_amoaloeo(N, ct, PQOptions::CF::eo)
             == n);
      assert(Nf * PQEncoding::c_amoaloeo(N, ct, PQOptions::CF::eo) + cbin
             == c);
#endif
    }
    static var_t count_clashes(const Cubing_t& C) noexcept {
      if (C.N <= 1 or C.m <= 1) return 0;
      Statistics::fdimacs_pars::float_t res = 0;
      for (var_t i = 0; i < C.N-1; ++i)
        for (var_t j = i+1; j < C.N; ++j)
          for (var_t cui = 0; cui < C.m-1; ++cui)
            for (var_t cuj = cui+1; cuj < C.m; ++cuj)
              if (not C.disjoint({i,cui}, {j,cuj})) ++res;
      assert(res <= FloatingPoint::P264m1);
      return res;
    }

    var_t operator()() const noexcept {
      return ++next;
    }

    // The primary variables (choose solution p.cu for digit p.co):
    var_t operator()(const qplaces& p) const noexcept {
      assert(C.valid(p));
      return m*p.co + p.cu;
    }

  };

}

#endif
