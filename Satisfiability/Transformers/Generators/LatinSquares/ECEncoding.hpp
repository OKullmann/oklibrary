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
#include <ostream>

#include <Numerics/NumTypes.hpp>
#include <Numerics/Conversions.hpp>

#include "PQOptions.hpp"
#include "ECOptions.hpp"
#include "Statistics.hpp"
#include "PQEncoding.hpp"
#include "Algorithms.hpp"
#include "AloAmo.hpp"
#include "EQOptions.hpp"

namespace ECEncoding {

  using Statistics::dim_t;
  using Statistics::var_t;

  using Algorithms::qplaces;
  using Algorithms::Cubing_t;

  struct EC0Encoding {
    const Cubing_t& C;
    const PQOptions::CT ct;
    const ECOptions::NC nc;

    const var_t N = C.N, m = C.m;

    const var_t n0 = N * m; // primary variables
    const var_t naux = N *
      FloatingPoint::toUInt(PQEncoding::n_amoaloeo(m, ct, PQOptions::CF::eo));
    const var_t n = n0 + naux;
    const var_t ceo =  N *
      FloatingPoint::toUInt(PQEncoding::c_amoaloeo(m, ct, PQOptions::CF::eo));
    const var_t cbin = count_clashes(C);
    const var_t cnoncyclic = nc == ECOptions::NC:: none ? 0 : m;
    const var_t c = ceo + cbin + cnoncyclic;
    const Statistics::dimacs_pars dp{n,c};

  protected :
    mutable var_t next = n0;
  public :

    EC0Encoding(const Cubing_t& C, const PQOptions::CT ct,
                const ECOptions::NC nc) noexcept :
    C(C), ct(ct), nc(nc) {
      assert(C.valid());
#ifndef NDEBUG
      using f80 = FloatingPoint::float80;
#endif
      assert(n0 == f80(N) * m);
      assert(n0 + N * PQEncoding::n_amoaloeo(m, ct, PQOptions::CF::eo)
             == n);
      assert(N * PQEncoding::c_amoaloeo(m, ct, PQOptions::CF::eo) + f80(cbin)
             + cnoncyclic
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

    struct DimacsEC0 {
      std::ostream& out;
      const var_t m;
      using vector_t = Algorithms::vector_t;
      void operator()(const vector_t& v) const {
        out << "v";
        for (var_t i = 0; i < v.size(); ++i)
          out << " " << 1 + i*m + v[i];
        out << " 0\n";
      }
    };

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
  template <class ENC>
  void noncyclic(std::ostream& out, const ENC& enc) {
    if (enc.nc != ECOptions::NC::noncyclic) return;
    for (var_t i = 0; i < enc.m; ++i) {
      AloAmo::Clause C; C.reserve(enc.N);
      for (var_t j = 0; j < enc.N; ++j) {
        const qplaces p{j,i};
        C.push_back(-AloAmo::Lit(enc(p)));
      }
      out << C;
#ifndef NDEBUG
      ++running_counter;
#endif
    }
  }

  void ecsat0(std::ostream& out, const EC0Encoding& enc) {
    out << enc.dp;
    eocubes(out, enc);
    clashes(out, enc);
    noncyclic(out, enc);
#ifndef NDEBUG
    assert(running_counter == enc.c);
#endif
  }


  constexpr bool valid(const PQOptions::CF cf1, const PQOptions::CT ct1,
                       const ECOptions::DQ dq) {
    return cf1 != PQOptions::CF::alo or
      (ct1 == PQOptions::CT::prime and dq != ECOptions::DQ::none);
  }

  struct EC1Encoding {
    using CF = PQOptions::CF; using CT = PQOptions::CT;
    using NC = ECOptions::NC;using CC = ECOptions::CC;using DQ = ECOptions::DQ;
    using cell_t = PQEncoding::cell_t;

    const Cubing_t& C;
    const CF cf1;
    const CT ct1, ct2;
    const NC nc; const CC cc; const DQ dq;

    const var_t N = C.N, m = C.m;

    const var_t N2 = N*N, N3 = N2*N;
    const var_t nprimecells = N3;
    const var_t nprimequeens = N * m;
    const var_t n0 = nprimecells + nprimequeens; // primary variables
    const var_t nauxcells = N2 *
      FloatingPoint::toUInt(PQEncoding::n_amoaloeo(N, ct1, cf1));
    const var_t nauxqueens = N *
      FloatingPoint::toUInt(PQEncoding::n_amoaloeo(m, ct2, CF::eo));
    const var_t n = n0 + nauxcells + nauxqueens;
    const var_t ceocells =  N2 *
      FloatingPoint::toUInt(PQEncoding::c_amoaloeo(N, ct1, cf1));
    const var_t ceoqueens =  N *
      FloatingPoint::toUInt(PQEncoding::c_amoaloeo(m, ct2, CF::eo));
    const var_t cbin = m * N2;
    const var_t cnoncyclic = nc == NC:: none ? 0 : m;
    const var_t cconjcells = cc == CC:: none ? 0 : N * m;
    const var_t cdisjqc = dq == DQ:: none ? 0 : N3;
    const var_t c = ceocells + ceoqueens + cbin +
      cnoncyclic + cconjcells + cdisjqc;
    const Statistics::dimacs_pars dp{n,c};

  protected :
    mutable var_t next = n0;
  public :

    EC1Encoding(const Cubing_t& C, const CF cf1, const CT ct1, const CT ct2,
                const NC nc, const CC cc, const DQ dq) noexcept :
    C(C), cf1(cf1), ct1(ct1), ct2(ct2), nc(nc), cc(cc), dq(dq) {
      assert(C.valid());
      assert(valid(cf1, ct1, dq));
#ifndef NDEBUG
      using f80 = FloatingPoint::float80;
#endif
      assert(N2 == f80(N) * N); assert(N3 == f80(N) * N2);
      assert(n0 == N3 + f80(N) * m);
      assert(nauxcells == N2 * PQEncoding::n_amoaloeo(N, ct1, cf1));
      assert(nauxqueens == N * PQEncoding::n_amoaloeo(m, ct2, CF::eo));
      assert(n == f80(n0) + f80(nauxcells) + nauxqueens);
      assert(ceocells == N2 * PQEncoding::c_amoaloeo(N, ct1, cf1));
      assert(ceoqueens == N * PQEncoding::c_amoaloeo(m, ct2, CF::eo));
      assert(cbin == f80(m) * N2);
      assert(c == f80(ceocells) + f80(ceoqueens) + f80(cbin)
             + f80(cnoncyclic) + f80(cconjcells) + f80(cdisjqc));

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

    struct DimacsEC1 {
      std::ostream& out;
      const var_t N, m, N3 = N*N*N;
      using vector_t = Algorithms::vector_t;
      void operator()(const vector_t& v) const {
        out << "v";
        for (var_t i = 0; i < v.size(); ++i)
          out << " " << 1 + N3 + i*m + v[i];
        out << " 0\n";
      }
    };

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
  // The reverse of connections(out, enc):
  template <class ENC>
  void cell_conjunctions(std::ostream& out, const ENC& enc) {
    if (enc.cc != ECOptions::CC::cell_conjunctions) return;
    for (dim_t i = 0; i < enc.N; ++i)
      for (var_t j = 0; j < enc.m; ++j) {
        const Algorithms::qplaces p{i,j};
        const auto Q = enc.C.queens(p);
        assert(Q.size() == enc.N);
        AloAmo::Clause C; C.reserve(enc.N+1);
        using AloAmo::Lit;
        for (dim_t k = 0; k < enc.N; ++k) {
          const PQEncoding::cell_t cell{k,dim_t(Q[k])};
          C.push_back(-Lit(enc(cell, i)));
        }
        C.push_back(Lit(enc(p)));
        out << C;
#ifndef NDEBUG
        ++running_counter;
#endif
      }
  }
  template <class ENC>
  void qc_disjunctions(std::ostream& out, const ENC& enc) {
    if (enc.dq != ECOptions::DQ::qc_disjunctions) return;
    for (dim_t i = 0; i < enc.N; ++i)
      for (dim_t j = 0; j < enc.N; ++j)
        for (dim_t k = 0; k < enc.N; ++k) {
          const PQEncoding::cell_t cell{i,j};
          using AloAmo::Lit;
          AloAmo::Clause C({-Lit(enc(cell,k))});
          for (var_t cu = 0; cu < enc.m; ++cu) {
            const Algorithms::qplaces p{k,cu};
            if (enc.C.queen(p, i) == j) C.push_back(Lit(enc(p)));
          }
          out << C;
#ifndef NDEBUG
          ++running_counter;
#endif
        }
  }

  void ecsat1(std::ostream& out, const EC1Encoding& enc) {
    out << enc.dp;
    values(out, enc);
    cubes(out, enc);
    connections(out, enc);
    noncyclic(out, enc);
    cell_conjunctions(out, enc);
    qc_disjunctions(out, enc);
#ifndef NDEBUG
    assert(running_counter == enc.c);
#endif
  }


  struct EC2Encoding {
    using CF = PQOptions::CF; using CT = PQOptions::CT;
    using NC = ECOptions::NC;using CC = ECOptions::CC;using DQ = ECOptions::DQ;
    using cell_t = PQEncoding::cell_t;

    const Cubing_t& C;
    const CF cf1;
    const CT ct1, ct2;
    const NC nc; const CC cc; const DQ dq;

    const var_t N = C.N, m = C.m;

    const var_t N2 = N*N, N3 = N2*N;
    const var_t nprimecells = N3;
    const var_t nprimequeens = N * m;
    const var_t n0 = nprimecells + nprimequeens; // primary variables
    const var_t nauxcells = 5 * N2 *
      FloatingPoint::toUInt(PQEncoding::n_amoaloeo(N, ct1, cf1));
    const var_t nauxqueens = N *
      FloatingPoint::toUInt(PQEncoding::n_amoaloeo(m, ct2, CF::eo));
    const var_t n = n0 + nauxcells + nauxqueens;
    const var_t crred = N;
    const var_t ceocells =  5 * N2 *
      FloatingPoint::toUInt(PQEncoding::c_amoaloeo(N, ct1, cf1));
    const var_t ceoqueens =  N *
      FloatingPoint::toUInt(PQEncoding::c_amoaloeo(m, ct2, CF::eo));
    const var_t cbin = m * N2;
    const var_t cnoncyclic = nc == NC:: none ? 0 : m;
    const var_t cconjcells = cc == CC:: none ? 0 : N * m;
    const var_t cdisjqc = dq == DQ:: none ? 0 : N3;
    const var_t c = crred + ceocells + ceoqueens + cbin +
      cnoncyclic + cconjcells + cdisjqc;
    const Statistics::dimacs_pars dp{n,c};

  protected :
    mutable var_t next = n0;
  public :

    EC2Encoding(const Cubing_t& C, const CF cf1, const CT ct1, const CT ct2,
                const ECOptions::NC nc, const CC cc, const DQ dq) noexcept :
    C(C), cf1(cf1), ct1(ct1), ct2(ct2), nc(nc), cc(cc), dq(dq) {
      assert(C.valid());
      assert(valid(cf1, ct1, dq));
#ifndef NDEBUG
      using f80 = FloatingPoint::float80;
#endif
      assert(N2 == f80(N) * N); assert(N3 == f80(N) * N2);
      assert(n0 == N3 + f80(N) * m);
      assert(nauxcells == 5 * N2 * PQEncoding::n_amoaloeo(N, ct1, cf1));
      assert(nauxqueens == N * PQEncoding::n_amoaloeo(m, ct2, CF::eo));
      assert(n == f80(n0) + f80(nauxcells) + nauxqueens);
      assert(ceocells == 5 * N2 * PQEncoding::c_amoaloeo(N, ct1, cf1));
      assert(ceoqueens == N * PQEncoding::c_amoaloeo(m, ct2, CF::eo));
      assert(cbin == f80(m) * N2);
      assert(c == f80(crred) + f80(ceocells) + f80(ceoqueens) + f80(cbin)
             + f80(cnoncyclic) + f80(cconjcells) + f80(cdisjqc));

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

  void rreduced(std::ostream& out, const EC2Encoding& enc) {
    for (dim_t j = 0; j < enc.N; ++j)
      out << AloAmo::Clause{AloAmo::Lit(enc({0,j},j))};
#ifndef NDEBUG
    running_counter += enc.N;
#endif
  }
  void rows(std::ostream& out, const EC2Encoding& enc) {
    for (dim_t i = 0; i < enc.N; ++i)
      for (dim_t k = 0; k < enc.N; ++k) {
        AloAmo::Clause C;
        for (dim_t j = 0; j < enc.N; ++j)
          C.push_back(AloAmo::Lit(enc({i,j},k)));
        amoaloeo(out, C, enc, enc.cf1, enc.ct1);
      }
  }
  void columns(std::ostream& out, const EC2Encoding& enc) {
    for (dim_t j = 0; j < enc.N; ++j)
      for (dim_t k = 0; k < enc.N; ++k) {
        AloAmo::Clause C;
        for (dim_t i = 0; i < enc.N; ++i)
          C.push_back(AloAmo::Lit(enc({i,j},k)));
        amoaloeo(out, C, enc, enc.cf1, enc.ct1);
      }
  }
  void diagonals(std::ostream& out, const EC2Encoding& enc) {
    for (dim_t diff = 0; diff < enc.N; ++diff)
      for (dim_t k = 0; k < enc.N; ++k) {
        AloAmo::Clause C;
        for (dim_t i = 0; i < enc.N; ++i) {
          const dim_t j = (var_t(diff) + i) % enc.N;
          C.push_back(AloAmo::Lit(enc({i,j},k)));
        }
        amoaloeo(out, C, enc, enc.cf1, enc.ct1);
      }
  }
  void antidiagonals(std::ostream& out, const EC2Encoding& enc) {
    for (var_t sum = enc.N; sum < 2*enc.N; ++sum)
      for (dim_t k = 0; k < enc.N; ++k) {
        AloAmo::Clause C;
        for (dim_t i = 0; i < enc.N; ++i) {
          const dim_t j = (sum - i) % enc.N;
          C.push_back(AloAmo::Lit(enc({i,j},k)));
        }
        amoaloeo(out, C, enc, enc.cf1, enc.ct1);
      }
  }

  void ecsat2(std::ostream& out, const EC2Encoding& enc) {
    out << enc.dp;
    rreduced(out, enc);
    values(out, enc);
    rows(out, enc); columns(out, enc);
    diagonals(out, enc); antidiagonals(out, enc);
    cubes(out, enc);
    connections(out, enc);
    noncyclic(out, enc);
    cell_conjunctions(out, enc);
    qc_disjunctions(out, enc);
#ifndef NDEBUG
    assert(running_counter == enc.c);
#endif
  }


  void all_solutions(const Cubing_t& C, const var_t k,
                     std::ostream& out, const EQOptions::OT ot) {
    switch (ot) {
    case EQOptions::OT::dimacs : {
      Algorithms::DimacsPandiagonal dp{out,C};
      all_solutions(C, k, {}, dp); return; }
    case EQOptions::OT::dimacs_ec0 : {
      EC0Encoding::DimacsEC0 dec0{out,C.m};
      all_solutions(C, k, {}, dec0); return; }
    case EQOptions::OT::dimacs_ec1 : {
      EC1Encoding::DimacsEC1 dec1{out,C.N,C.m};
      all_solutions(C, k, {}, dec1); return; }
    case EQOptions::OT::cube_index : {
      Algorithms::CubeIndices ci{out};
      all_solutions(C, k, {}, ci); return; }
    case EQOptions::OT::count_only : {
      Algorithms::CountOnly co{Algorithms::expand_total_count};
      all_solutions(C, k, {}, co); return; }
    };
  }
  void all_solutions(const Cubing_t& C, std::ostream& out,
                     const EQOptions::OT ot) {
    all_solutions(C, C.N, out, ot);
  }


}

#endif
