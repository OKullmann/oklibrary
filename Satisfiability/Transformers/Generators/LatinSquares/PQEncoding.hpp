// Oliver Kullmann, 18.2.2024 (Swansea)
/* Copyright 2024 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#ifndef PQENCODING_eK42F6xZuv
#define PQENCODING_eK42F6xZuv

#include <ostream>
#include <array>
#include <vector>

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

  // to control overflow (float80 strictly includes var_t):
  using float_t = Statistics::fdimacs_pars::float_t;


  constexpr float_t n_eo(const var_t N, const PQOptions::CT ct) noexcept {
    return ct == PQOptions::CT::prime ? 0 : Statistics::n_amo_seco(N);
  }
  constexpr float_t c_eo(const var_t N, const PQOptions::CT ct) noexcept {
    switch(ct) {
    case PQOptions::CT::prime : return Statistics::c_eo_primes(N);
    case PQOptions::CT::seco : return Statistics::c_eo_seco(N);
    case PQOptions::CT::secouep : return Statistics::c_eo_secouep(N);
    default : assert(false); return 0; }
  }

  struct cell_t {
    dim_t i, j;
  };
  constexpr bool valid(const cell_t& c, const dim_t N) noexcept {
    return c.i < N and c.j < N;
  }


  struct PEncoding {
    const var_t N;
    const var_t N2 = var_t(N) * N, N3 = N2 * N;
    const PQOptions::CT ct;

    const bool sudoku;
    // box-size, number of boxes horizontally and vertically, remainder:
    const var_t b, q, r;
    // Remark: rectangles 2x2 or smaller are ignored.
    // Volume and count:
    typedef std::array<var_t, 2> box_count_vol_t;
    // Main boxes, side boxes, corner box:
    typedef std::array<box_count_vol_t, 3> total_counts_vol_t;
    const total_counts_vol_t boxes;

    const Statistics::fdimacs_pars p;
  private :
    mutable var_t next = N3;
  public :

    constexpr PEncoding(const dim_t N, const PQOptions::CT ct,
                        const bool sudoku) noexcept :
    N(N), ct(ct), sudoku(sudoku), b(std::sqrt(N)), q(N/b), r(N%b),
    boxes(box_count()), p(pars()) {
      assert(N >= 1);
      assert(p.valid());
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

    constexpr total_counts_vol_t box_count() const noexcept {
      total_counts_vol_t res{};
      res[0][1] = b*b; res[1][1] = b*r; res[2][1] = r*r;
      if (b <= 2) return res;
      assert(q >= 3);
      res[0][0] = q*q;
      if (r >= 2) res[1][0] = 2 * q;
      if (r >= 3) res[2][0] = 1;
      return res;
    }
    static void output(std::ostream& out, const total_counts_vol_t& tc) {
      const auto o = [&out](const box_count_vol_t& c)->std::ostream&{
        return out << c[0] << "*" << c[1];
      };
      o(tc[0]) << " "; o(tc[1]) << " "; o(tc[2]);
    }
    constexpr Statistics::fdimacs_pars pars() const noexcept {
      const float_t num_cells = N2;
      const float_t num_vars_square = N3; // direct encoding

      /* First pure pandiagonal conditions: */

      // rows, columns, diagonals, antidiagonals:
      const float_t num_all_different = 4 * float_t(N);
      const float_t num_eos = num_cells + N * num_all_different;

      const float_t num_var_eo = n_eo(N, ct);
      const float_t num_var_alleos = num_eos * num_var_eo;
      const float_t n = num_vars_square + num_var_alleos;

      const float_t num_clauses_rred = N;
      const float_t num_clauses_eo = c_eo(N, ct);
      const float_t num_clauses_alleos = num_eos * num_clauses_eo;
      const float_t c = num_clauses_rred + num_clauses_alleos;

      if (not sudoku) return {n,c};
      /* Additional Sudoky conditions: */

      const auto [nsud, csud] = [this](){
        float_t nsud = 0, csud = 0;
        for (const auto& [count, size] : boxes) {
          nsud += count * n_eo(size, ct);
          csud += count * c_eo(size, ct);
        }
        return std::array{nsud, csud};}();
      return {n + nsud, c + csud};
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
