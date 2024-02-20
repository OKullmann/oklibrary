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
    const var_t N2 = var_t(N)*N;
    const var_t N3 = N2 * N;
    const PQOptions::CT ct;
    const Statistics::fdimacs_pars p;
  private :
    mutable var_t next = N3;
  public :

    constexpr PEncoding(const dim_t N, const PQOptions::CT ct) noexcept :
    N(N), ct(ct), p(pars(N,N2,N3,ct)) { assert(p.valid()); }

    constexpr Statistics::fdimacs_pars pars(const dim_t N,
                                            const var_t N2, const var_t N3,
                                            const PQOptions::CT ct) noexcept {
       const var_t num_cells = N2;
      // rows, columns, diagonals, antidiagonals:
      const var_t num_all_different = 4 * N;
      const var_t num_eos = num_cells + N * num_all_different;

      const var_t num_vars_square = N3; // direct encoding
      const var_t num_var_eo = ct==PQOptions::CT::prime ? 0 :
        Statistics::n_amo_seco(N);
      const var_t num_var_alleos = num_eos * num_var_eo;
      const var_t n = num_vars_square + num_var_alleos;

      const var_t num_clauses_rred = N;
      const var_t num_clauses_eo = ct==PQOptions::CT::prime ?
       Statistics::c_eo_primes(N) :
       ct==PQOptions::CT::seco ? Statistics::c_eo_seco(N) :
        Statistics::c_eo_secouep(N);
      const var_t num_clauses_alleos = num_eos * num_clauses_eo;
      const var_t c = num_clauses_rred + num_clauses_alleos;

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
