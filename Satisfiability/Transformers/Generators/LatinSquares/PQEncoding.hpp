// Oliver Kullmann, 18.2.2024 (Swansea)
/* Copyright 2024 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#ifndef PQENCODING_eK42F6xZuv
#define PQENCODING_eK42F6xZuv

#include <cassert>

#include "Statistics.hpp"

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
      return c.i * N2 + c.j * N + k;
    }

  };

}

#endif
