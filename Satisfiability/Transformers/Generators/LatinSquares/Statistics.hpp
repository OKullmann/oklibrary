// Oliver Kullmann, 3.8.2020 (Swansea)
/* Copyright 2020 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#ifndef STATISTICS_U6pYV2w4jk
#define STATISTICS_U6pYV2w4jk

#include <cstdint>

#include "Errors.hpp"
#include "Options.hpp"

namespace Statistics {

  typedef std::uint16_t dim_t;
  typedef std::uint64_t var_t;


  constexpr dim_t N_default = 3;
  constexpr dim_t k_default = 1;

  struct Param {
    dim_t N;
    dim_t k;
  };
  std::ostream& operator <<(std::ostream& out, const Param p) {
    return out << p.N << "," << p.k;
  }


  struct NumVarsCls {
    var_t nbls1, nbls2, // number of variables per single latin-square,
                        // first instance and others
          nls, // total number of variables for all single latin-squares
          nbes1, nbes2, // number of primary variables for single euler-square,
                        // for p=0 and p>=1
          npes, // total number of primary variables for all euler-squares
          n0, // number of all primary variables
          nbaux1, nbaux2, // number of auxiliary variables for single es
          naux, // number of all auxiliary variables (amo/eo for euler-squares)
          n; // number of variables in total
    var_t cls, ces, c;
  };
  struct fNumVarsCls {
    FloatingPoint::float80 nbls1, nbls2, nls, nbes1, nbes2, npes, n0, nbaux1, nbaux2, naux, n, cls, ces, c;
    constexpr bool valid() const noexcept {
      return FloatingPoint::isUInt({nbls1, nbls2, nls, nbes1, nbes2, npes, n0, nbaux1, nbaux2, naux, n, cls, ces, c});
    }
    constexpr operator NumVarsCls() const noexcept {
      assert(valid());
      return {var_t(nbls1), var_t(nbls2), var_t(nls), var_t(nbes1), var_t(nbes2), var_t(npes), var_t(n0), var_t(nbaux1), var_t(nbaux2), var_t(naux), var_t(n), var_t(cls), var_t(ces), var_t(c)};
    }
  };

  // Handling of pairs n, c, handling overflows by embedding into
  // FloatingPoint::float80
  using RandGen::dimacs_pars;
  struct fdimacs_pars {
    FloatingPoint::float80 n, c;
    constexpr fdimacs_pars() noexcept : n(0), c(0) {}
    constexpr fdimacs_pars(const var_t n, const var_t c) noexcept : n(n), c(c) {}
    constexpr fdimacs_pars(const FloatingPoint::float80 n, const FloatingPoint::float80 c) noexcept : n(n), c(c) {}
    constexpr bool valid() const noexcept {
      return FloatingPoint::isUInt({n,c});
    }
    constexpr operator dimacs_pars() const noexcept {
      assert(valid());
      return {var_t(n), var_t(c)};
    }
  };


  constexpr fdimacs_pars pars_eo_primes(const var_t m) noexcept {
    return {0, 1 + FloatingPoint::fbinomial_coeff(m,2)};
  }
  constexpr fdimacs_pars pars_amo_primes(const var_t m) noexcept {
    return {0, FloatingPoint::fbinomial_coeff(m,2)};
  }
  constexpr fdimacs_pars pars_alo_primes(const var_t) noexcept {
    return {0, 1.0L};
  }

  constexpr FloatingPoint::float80 n_amo_seco(const var_t m) noexcept {
    if (m <= 2) return 0;
    return (m-1)/2-1L;
  }
  constexpr FloatingPoint::float80 c_amo_seco(const var_t m, const Options::EAloP ealoopt) noexcept {
    if (m <= 1) return 0;
    if (m == 2) return 1;
    const FloatingPoint::float80 c = 3L*m - 6;
    if (not has_uep(ealoopt)) return c;
    else return c + n_amo_seco(m);
  }


  constexpr NumVarsCls numvarscls(const Param p, const Options::SymP symopt, const Options::EAloP ealoopt, const Options::EulP eulopt, const Options::PrimeP primopt) noexcept {
    const FloatingPoint::float80 N = p.N, k = p.k;
    const auto N2 = N*N;
    const auto N3 = N2*N;
    const auto N4 = N2*N2;
    const var_t vk{p.k};
    using FloatingPoint::fbinomial_coeff;

    if (symopt == Options::SymP::full) {
      fNumVarsCls r{};
      r.nbls1 = N3;
      r.nls = r.nbls1 * k;
      r.nbes1 = N4;
      r.npes = r.nbes1 * fbinomial_coeff(p.k, 2);
      r.n0 = r.nls + r.npes;
      r.n = r.n0;
      r.nbaux1 = N2 * n_amo_seco(N2);
      r.naux = r.nbaux1 * fbinomial_coeff(p.k, 2);
      r.n = r.n0 + r.naux;
      if (r.n >= FloatingPoint::P264) {
        std::cerr << Errors::error << "Parameters " << p << " yield total number of variables >= 2^64.\n";
        std::exit(int(Errors::Error::too_big));
      }

      const auto cbls = primopt==Options::PrimeP::full ? 3 * N2 * pars_eo_primes(N).c :
        N2 * pars_alo_primes(N).c + 2 * N2 * pars_amo_primes(N).c;
      r.cls = cbls * k;
      const auto cbes = (eulopt==Options::EulP::full ? 3 : 1) * N4 +
        (has_pair(ealoopt) ?
          N2 : 0) +
        (has_val(ealoopt) ?
          N2 : 0);
      r.ces = cbes * fbinomial_coeff(p.k, 2);
      r.ces += fbinomial_coeff(p.k, 2) * N2 * c_amo_seco(N2, ealoopt);
      r.c = r.cls + r.ces;
      if (r.c >= FloatingPoint::P264) {
        std::cerr << Errors::error << "Parameters " << p << " yield total number of clauses >= 2^64.\n";
        std::exit(int(Errors::Error::too_big));
      }
      return r;
    }
    else { // SymP::reduced

      fNumVarsCls r{};
      r.nbls1 = (N-1)*((N-1) + (N-2)*(N-2));
      r.nbls2 = (N-1)*(N*(N-1) - 1);
      r.nls = r.nbls1 + r.nbls2 * (k - 1);
      r.nbes1 = (N-2)*(N-1)*(N2 - 3*N + 3);
      r.nbes2 = (N-2)*(N-2)*(N2 - 1);
      //r.nbes2 = (N-1)*(N-1)*N*(N-2) - (N-1)*(2*N-5);
      r.npes = r.nbes1 * (k-1) + r.nbes2 * fbinomial_coeff(vk-1, 2);
      r.n0 = r.nls + r.npes;
      r.nbaux1 = (N-1) * n_amo_seco((N-1)*(N-2)) +
                 (N-1) * n_amo_seco((N-2)*(N-2)) +
                 (N-1)*(N-2) * n_amo_seco((N-3)*(N-2) + 1);
      r.nbaux2 = 2*(N-1) * n_amo_seco((N-1)*(N-2)) +
                 (N-1)*(N-2) * n_amo_seco(N*(N-3));
      r.naux = (k - 1) * r.nbaux1 + fbinomial_coeff(vk-1, 2) * r.nbaux2;
      r.n = r.n0 + r.naux;
      if (r.n >= FloatingPoint::P264) {
        std::cerr << Errors::error << "Parameters " << p << " yield total number of variables >= 2^64.\n";
        std::exit(int(Errors::Error::too_big));
      }

      const auto peop1 = pars_eo_primes(N-1).c;
      const auto palop1 = pars_alo_primes(N-1).c;
      const auto pamop1 = pars_amo_primes(N-1).c;
      const auto peop2 = pars_eo_primes(N-2).c;
      const auto palop2 = pars_alo_primes(N-2).c;
      const auto pamop2 = pars_amo_primes(N-2).c;
      const auto cbls1 = primopt==Options::PrimeP::full ?
        (N-1)*peop1 + (N-1)*(N-2)*peop2 + 2*(N-1)*(peop1+(N-2)*peop2) :
        (N-1)*((N-2)*palop2+palop1)+2*(N-1)*((N-2)*pamop2+pamop1);
      const auto cbls2 = primopt==Options::PrimeP::full ?
        (N-1)*peop2 + (N-1)*(N-1)*peop1 + 2*(N-1)*(peop2+(N-1)*peop1) :
        (N-1) * N * (palop1 + 2 * pamop1); // ???
      r.cls = cbls1 + cbls2 * (k - 1);
      const auto cdefs = (eulopt==Options::EulP::full ? 3 : 1) * r.npes + // ???
        (k-1) * ((N-1)*(N-2)*(N-2) + (N-1)*(N-1)) +
        fbinomial_coeff(vk-1, 2) * (N-1)*(N-1)*(N-1);
      const auto cbes1 = (has_val(ealoopt) ? (N-1)*(N-1) : 0) +
                         (has_pair(ealoopt) ? N*(N-1) : 0);
      const auto cbes2 = (has_val(ealoopt) ? (N-1)*N : 0) +
                         (has_pair(ealoopt) ? N*(N-1) : 0);
      r.ces = cdefs + cbes1 * (vk-1) + cbes2 * fbinomial_coeff(vk-1, 2);
      r.ces += (k - 1) * // ???
                ((N-1) * c_amo_seco((N-2)*(N-1), ealoopt) +
                 (N-1) * c_amo_seco((N-2)*(N-2), ealoopt) +
                 (N-1)*(N-2) * c_amo_seco((N-3)*(N-2) + 1, ealoopt));
      r.ces += fbinomial_coeff(vk-1, 2) * (N2 - N) * c_amo_seco((N-2)*(N-1), ealoopt);
      r.c = r.cls + r.ces;
      if (r.c >= FloatingPoint::P264) {
        std::cerr << Errors::error << "Parameters " << p << " yield total number of clauses >= 2^64.\n";
        std::exit(int(Errors::Error::too_big));
      }
      return r;
    }
  }

}

#endif
