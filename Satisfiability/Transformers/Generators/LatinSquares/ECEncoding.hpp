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
      using float80 = FloatingPoint::float80;
#endif
      assert(n0 == float80(N) * m);
      assert(n0 + N * PQEncoding::n_amoaloeo(m, ct, PQOptions::CF::eo)
             == n);
      assert(N * PQEncoding::c_amoaloeo(m, ct, PQOptions::CF::eo) + cbin
             == c);
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


  struct EC1Encoding {
    using CF = PQOptions::CF;
    using CT = PQOptions::CT;
    using cell_t = PQEncoding::cell_t;

    const Cubing_t& C;
    const CF cf1;
    const CT ct1, ct2;

    const var_t N = C.N, m = C.m;

    const var_t N2 = N*N, N3 = N2*N;
    const var_t n0 = N3 + N * m; // primary variables
    const var_t naux1 = N2 *
      FloatingPoint::toUInt(PQEncoding::n_amoaloeo(N, ct1, cf1));
    const var_t naux2 = N *
      FloatingPoint::toUInt(PQEncoding::n_amoaloeo(m, ct2, CF::eo));
    const var_t n = n0 + naux1 + naux2;
    const var_t ceo1 =  N2 *
      FloatingPoint::toUInt(PQEncoding::c_amoaloeo(N, ct1, cf1));
    const var_t ceo2 =  N *
      FloatingPoint::toUInt(PQEncoding::c_amoaloeo(m, ct2, CF::eo));
    const var_t cbin = m * N2;
    const var_t c = ceo1 + ceo2 + cbin;
    const Statistics::dimacs_pars dp{n,c};

  protected :
    mutable var_t next = n0;
  public :

    EC1Encoding(const Cubing_t& C, const CF cf1, const CT ct1,
                const CT ct2) noexcept :
    C(C), cf1(cf1), ct1(ct1), ct2(ct2) {
      assert(C.valid());
#ifndef NDEBUG
      using float80 = FloatingPoint::float80;
#endif
      assert(N2 == float80(N) * N); assert(N3 == float80(N) * N2);
      assert(n0 == N3 + float80(N) * m);
      assert(naux1 == N2 * PQEncoding::n_amoaloeo(N, ct1, cf1));
      assert(naux2 == N * PQEncoding::n_amoaloeo(m, ct2, CF::eo));
      assert(n == float80(n0) + float80(naux1) + naux2);
      assert(ceo1 == N2 * PQEncoding::c_amoaloeo(N, ct1, cf1));
      assert(ceo2 == N * PQEncoding::c_amoaloeo(m, ct2, CF::eo));
      assert(cbin == float80(m) * N2);
      assert(c == float80(ceo1) + float80(ceo2) + cbin);

      assert(Statistics::fits_dim_t(N));
    }

    var_t operator()() const noexcept {
      return ++next;
    }

    // The primary variables first part (set digit k in cell c):
    constexpr var_t operator()(const cell_t& c, const dim_t k) const noexcept {
      assert(valid(c, N));
      assert(k < N);
      const var_t code = c.i * N2 + c.j * N + k;
      return 1 + code;
    }
    // The primary variables second part (choose solution p.cu for digit p.co):
    var_t operator()(const qplaces& p) const noexcept {
      assert(C.valid(p));
      return 1 + N3 + m*p.co + p.cu;
    }

  };


  template <class NVAR>
  void amoaloeo(std::ostream& out, const AloAmo::Clause& C, const NVAR& enc,
                const PQOptions::CF cf, const PQOptions::CT ct) {
    if (cf == PQOptions::CF::alo) { AloAmo::alo_primes(out, C); return; }
    else if (cf == PQOptions::CF::eo) {
      switch (ct) {
      case PQOptions::CT::prime : AloAmo::eo_primes(out, C); return;
      case PQOptions::CT::seco : AloAmo::eo_seco(out, C, enc); return;
      case PQOptions::CT::secouep : AloAmo::eo_secouep(out, C, enc); return;}
    }
    switch(ct) {
    case PQOptions::CT::prime : AloAmo::amo_primes(out, C); return;
    case PQOptions::CT::seco : AloAmo::amo_seco(out, C, enc); return;
    case PQOptions::CT::secouep : AloAmo::amo_secouep(out, C, enc); return;}
  }

  template <class ENC>
  void values(std::ostream& out, const ENC& enc) {
    for (dim_t j = 0; j < enc.N; ++j)
      for (dim_t i = 0; i < enc.N; ++i) {
        AloAmo::Clause C;
        for (dim_t k = 0; k < enc.N; ++k)
          C.push_back(AloAmo::Lit(enc({i,j},k)));
        amoaloeo(out, C, enc, enc.cf1, enc.ct1);
      }
  }
  template <class ENC>
  void cubes(std::ostream& out, const ENC& enc) {
    for (var_t i = 0; i < enc.N; ++i) {
      AloAmo::Clause C; C.reserve(enc.m);
      for (var_t j = 0; j < enc.m; ++j)
        C.push_back(enc({i,j}));
      amoaloeo(out, C, enc, PQOptions::CF::eo, enc.ct2);
    }
  }
  template <class ENC>
  void connections(std::ostream& out, const ENC& enc) {
    for (dim_t i = 0; i < enc.N; ++i)
      for (var_t j = 0; j < enc.m; ++j) {
        const Algorithms::qplaces p{i,j};
        const auto Q = enc.C.queens(p);
        assert(Q.size() == enc.N);
        for (dim_t k = 0; k < enc.N; ++k) {
          const PQEncoding::cell_t cell{k,dim_t(Q[k])};
          using AloAmo::Lit;
          out << AloAmo::Clause{-Lit(enc(p)), Lit(enc(cell, i))};
#ifndef NDEBUG
          ++running_counter;
#endif
        }
      }
  }

  void ecsat1(std::ostream& out, const EC1Encoding& enc) {
    out << enc.dp;
    values(out, enc);
    cubes(out, enc);
    connections(out, enc);
#ifndef NDEBUG
    assert(running_counter == enc.c);
#endif
  }


}

#endif
