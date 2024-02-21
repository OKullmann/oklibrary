// Oliver Kullmann, 18.2.2024 (Swansea)
/* Copyright 2024 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#ifndef PQENCODING_eK42F6xZuv
#define PQENCODING_eK42F6xZuv

#include <ostream>

#include <cassert>

#include "Statistics.hpp"
#include "PQOptions.hpp"

#ifndef NDEBUG
   Statistics::var_t running_counter = 0;
# define INCCLAUSE ++running_counter;
#else
# define INCCLAUSE
#endif
#include "AloAmo.hpp"

namespace PQEncoding {

  using dim_t = Statistics::dim_t;
  using var_t = Statistics::var_t;

  struct cell_t {
    dim_t i, j;
  };
  constexpr bool valid(const cell_t& c, const dim_t N) noexcept {
    return c.i < N and c.j < N;
  }


  struct PEncoding {
    const dim_t N;
    const var_t N2 = var_t(N) * N, N3 = N2 * N;
    const PQOptions::CT ct;
    const Statistics::fdimacs_pars p;
  private :
    mutable var_t next = N3;
  public :

    constexpr PEncoding(const dim_t N, const PQOptions::CT ct) noexcept :
    N(N), ct(ct), p(pars(N,N2,N3,ct)) { assert(p.valid()); }

    constexpr Statistics::fdimacs_pars pars(const dim_t N0,
                                            const var_t N02, const var_t N03,
                                            const PQOptions::CT ct) noexcept {
      // to control overflow (float80 strictly includes var_t):
      using float_t = Statistics::fdimacs_pars::float_t;
      float_t N(N0), N2(N02), N3(N03);
      const float_t num_cells = N2;
      // rows, columns, diagonals, antidiagonals:
      const float_t num_all_different = 4 * N;
      const float_t num_eos = num_cells + N * num_all_different;

      const float_t num_vars_square = N3; // direct encoding
      const float_t num_var_eo = ct==PQOptions::CT::prime ? 0 :
        Statistics::n_amo_seco(N);
      const float_t num_var_alleos = num_eos * num_var_eo;
      const float_t n = num_vars_square + num_var_alleos;

      const float_t num_clauses_rred = N;
      const float_t num_clauses_eo = ct==PQOptions::CT::prime ?
       Statistics::c_eo_primes(N) :
       ct==PQOptions::CT::seco ? Statistics::c_eo_seco(N) :
        Statistics::c_eo_secouep(N);
      const float_t num_clauses_alleos = num_eos * num_clauses_eo;
      const float_t c = num_clauses_rred + num_clauses_alleos;

      return {n,c};
    }

    var_t operator()() const noexcept {
      return ++next;
    }
    var_t operator()(const cell_t& c, const dim_t k) const noexcept {
      assert(valid(c, N));
      assert(k < N);
      const var_t code = c.i * N2 + c.j * N + k;
      assert(code < N3);
      return 1 + code;
    }

  };


  template <class NVAR>
  void eo(std::ostream& out, const AloAmo::Clause& C, const NVAR& enc) {
    switch(enc.ct) {
      case PQOptions::CT::prime : AloAmo::eo_primes(out, C); return;
      case PQOptions::CT::seco : AloAmo::eo_seco(out, C, enc); return;
      case PQOptions::CT::secouep : AloAmo::eo_secouep(out, C, enc); return;
    }
  }


  void pandiagonal(std::ostream& out, const PEncoding& enc) {
    out << Statistics::dimacs_pars(enc.p);
    const auto N = enc.N;
    using Clause = AloAmo::Clause;
    using Lit = AloAmo::Lit;

    for (dim_t j = 0; j < N; ++j)
      out << Clause{Lit(enc({0,j},j))};
#ifndef NDEBUG
    running_counter += N;
#endif

    for (dim_t i = 0; i < N; ++i)
      for (dim_t j = 0; j < N; ++j) {
        Clause C;
        for (dim_t k = 0; k < N; ++k)
          C.push_back(Lit(enc({i,j},k)));
        eo(out, C, enc);
      }

    for (dim_t i = 0; i < N; ++i)
      for (dim_t k = 0; k < N; ++k) {
        Clause C;
        for (dim_t j = 0; j < N; ++j)
          C.push_back(Lit(enc({i,j},k)));
        eo(out, C, enc);
      }
    for (dim_t j = 0; j < N; ++j)
      for (dim_t k = 0; k < N; ++k) {
        Clause C;
        for (dim_t i = 0; i < N; ++i)
          C.push_back(Lit(enc({i,j},k)));
        eo(out, C, enc);
      }

    for (dim_t diff = 0; diff < N; ++diff)
      for (dim_t k = 0; k < N; ++k) {
        Clause C;
        for (dim_t i = 0; i < N; ++i) {
          const dim_t j = (var_t(diff) + i) % N;
          C.push_back(Lit(enc({i,j},k)));
        }
        eo(out, C, enc);
      }
    for (var_t sum = N; sum < 2*var_t(N); ++sum)
      for (dim_t k = 0; k < N; ++k) {
        Clause C;
        for (dim_t i = 0; i < N; ++i) {
          const dim_t j = (sum - i) % N;
          C.push_back(Lit(enc({i,j},k)));
        }
        eo(out, C, enc);
      }

#ifndef NDEBUG
    assert(running_counter == enc.p.c);
#endif

  }

}

#endif
