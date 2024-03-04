// Oliver Kullmann, 18.2.2024 (Swansea)
/* Copyright 2024 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Components related to encoding and translating pandiagional- and
  queensdiagonal problems.

  Imported types:

   - dim_t, var_t, float_t from Statistics

  Sizes of amo-alo-eo-translations:

   - n_amoaloeo(N, ct, cf) -> float_t
   - c_amoaloeo(N, ct, cf) -> float_t

  Encoding of Pandiagonal:

   - struct cell_t : wrapper for i, j : dim_t
   - valid(cell_t, dim_t N) -> bool

   - struct PEncoding
    - data members
        N, N2, N3, ct, sudoku, b, q, r,
        boxes (information on sizes, counts and encodings of boxes)
        p (dimacs-parameters)
    - operator () -> var_t (next new variable)
    - operator (cell_t c, dim_t k) -> var_t (index of variable for cell c
      and value k)

  Translation of Pandiagonal:

   - eo(ostream&, Clause C, NVAR enc) :
     output exactly-one for C, using enc.ct and enc();
     more general:
   - amoeo(ostream&, Clause C, NVAR enc, CF cf)
     (not for alo)

   - rowreduced(out, enc)
   - eovalues(out, enc)
   - eorows(out, enc)
   - eocolumns(out, enc)
   - eodiagonals(out, enc)
   - eoantidiagonals(out, enc)
   - amoeosudoku(out, enc);

   - pandiagonal(ostream&, PEncoding enc, bool sudoku)

*/

#ifndef PQENCODING_eK42F6xZuv
#define PQENCODING_eK42F6xZuv

#include <ostream>
#include <array>
#include <vector>
#include <utility>

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


  constexpr float_t n_amoaloeo(const var_t N, const PQOptions::CT ct,
                            const PQOptions::CF cf) noexcept {
    if (ct == PQOptions::CT::prime or cf == PQOptions::CF::alo)
      return 0;
    else
      return Statistics::n_amo_seco(N);
  }
  constexpr float_t c_amoaloeo(const var_t N, const PQOptions::CT ct,
                            const PQOptions::CF cf) noexcept {
    if (cf == PQOptions::CF::alo) return 1;
    const float_t not_eo = cf != PQOptions::CF::eo;
    switch(ct) {
    case PQOptions::CT::prime : return Statistics::c_eo_primes(N) - not_eo;
    case PQOptions::CT::seco : return Statistics::c_eo_seco(N) - not_eo;
    case PQOptions::CT::secouep : return Statistics::c_eo_secouep(N) - not_eo;
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
    const bool allows_eo = b*b == N;
    // Volume and count:
    typedef std::array<var_t, 2> box_count_vol_t;
    typedef std::pair<box_count_vol_t, PQOptions::CF> box_desc_t;
    // Main boxes, side boxes, corner box:
    typedef std::array<box_desc_t, 3> total_counts_vol_t;
    const total_counts_vol_t boxes;

    const Statistics::fdimacs_pars p;

  protected :
    mutable var_t next;
  public :

    constexpr PEncoding(const dim_t N, const PQOptions::CT ct,
                        const bool sudoku, const var_t next) noexcept :
    N(N), ct(ct), sudoku(sudoku), b(std::sqrt(N)), q(N/b), r(N%b),
    boxes(box_count()), p(pars()), next(next) {
      assert(N >= 1);
      assert(p.valid());
      assert(next <= p.n);
    }
    constexpr PEncoding(const dim_t N, const PQOptions::CT ct,
                        const bool sudoku) noexcept :
    N(N), ct(ct), sudoku(sudoku), b(std::sqrt(N)), q(N/b), r(N%b),
    boxes(box_count()), p(pars()), next(N3) {
      assert(N >= 1);
      assert(p.valid());
    }

    var_t operator()() const noexcept {
      return ++next;
    }
    constexpr var_t operator()(const cell_t& c, const dim_t k) const noexcept {
      assert(valid(c, N));
      assert(k < N);
      const var_t code = c.i * N2 + c.j * N + k;
      assert(code < N3);
      return 1 + code;
    }

    constexpr total_counts_vol_t box_count() const noexcept {
      total_counts_vol_t res{};
      res[0].first[1] = b*b; res[1].first[1] = b*r; res[2].first[1] = r*r;
      if (allows_eo) res[0].second = PQOptions::CF::eo;
      if (b <= 2) return res;
      assert(q >= 3);
      res[0].first[0] = q*q;
      if (r >= 2) res[1].first[0] = 2 * q;
      if (r >= 3) res[2].first[0] = 1;
      return res;
    }
    static void output(std::ostream& out, const total_counts_vol_t& tc) {
      const auto o = [&out](const box_desc_t& c)->std::ostream&{
        return out << c.first[0] << "*" << c.first[1] << ":" << c.second;
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

      const float_t num_var_eo = n_amoaloeo(N, ct, PQOptions::CF::eo);
      const float_t num_var_alleos = num_eos * num_var_eo;
      const float_t n = num_vars_square + num_var_alleos;

      const float_t num_clauses_rred = N;
      const float_t num_clauses_eo = c_amoaloeo(N, ct, PQOptions::CF::eo);
      const float_t num_clauses_alleos = num_eos * num_clauses_eo;
      const float_t c = num_clauses_rred + num_clauses_alleos;

      if (not sudoku) return {n,c};
      /* Additional Sudoky conditions: */

      const auto [nsud, csud] = [this](){
        float_t nsud = 0, csud = 0;
        for (const auto& [p, cf] : boxes) {
          const auto& [count, size] = p;
          nsud += count * n_amoaloeo(size, ct, cf) * N;
          csud += count * c_amoaloeo(size, ct, cf) * N;
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
  // Only amo or eo (controlled by cf):
  template <class NVAR>
  void amoeo(std::ostream& out, const AloAmo::Clause& C, const NVAR& enc,
             const PQOptions::CF cf) {
    assert(cf != PQOptions::CF::alo);
    if (cf == PQOptions::CF::eo) { eo(out, C, enc); return; }
    switch(enc.ct) {
      case PQOptions::CT::prime : AloAmo::amo_primes(out, C); return;
      case PQOptions::CT::seco : AloAmo::amo_seco(out, C, enc); return;
      case PQOptions::CT::secouep : AloAmo::amo_secouep(out, C, enc); return;
    }
  }


  template <class ENC>
  void rowreduced(std::ostream& out, const ENC& enc, const var_t bound = 0) {
    const var_t B = bound == 0 ? enc.N : bound;
    for (dim_t j = 0; j < B; ++j)
      out << AloAmo::Clause{AloAmo::Lit(enc({0,j},j))};
#ifndef NDEBUG
    running_counter += B;
#endif
  }
  template <class ENC>
  void eovalues(std::ostream& out, const ENC& enc, const var_t bound = 0) {
    const var_t B = bound == 0 ? enc.N : bound;
    for (dim_t j = 0; j < B; ++j)
      for (dim_t i = 0; i < enc.N; ++i) {
        AloAmo::Clause C;
        for (dim_t k = 0; k < enc.N; ++k)
          C.push_back(AloAmo::Lit(enc({i,j},k)));
        eo(out, C, enc);
      }
  }
  void eorows(std::ostream& out, const PEncoding& enc) {
    for (dim_t i = 0; i < enc.N; ++i)
      for (dim_t k = 0; k < enc.N; ++k) {
        AloAmo::Clause C;
        for (dim_t j = 0; j < enc.N; ++j)
          C.push_back(AloAmo::Lit(enc({i,j},k)));
        eo(out, C, enc);
      }
  }
  template <class ENC>
  void eocolumns(std::ostream& out, const ENC& enc, const var_t bound = 0) {
    const var_t B = bound == 0 ? enc.N : bound;
    for (dim_t j = 0; j < B; ++j)
      for (dim_t k = 0; k < enc.N; ++k) {
        AloAmo::Clause C;
        for (dim_t i = 0; i < enc.N; ++i)
          C.push_back(AloAmo::Lit(enc({i,j},k)));
        eo(out, C, enc);
      }
  }
  template <class ENC>
  void eodiagonals(std::ostream& out, const ENC& enc, const var_t bound = 0) {
    const var_t B = bound == 0 ? enc.N : bound;
    for (dim_t diff = 0; diff < B; ++diff)
      for (dim_t k = 0; k < enc.N; ++k) {
        AloAmo::Clause C;
        for (dim_t i = 0; i < enc.N; ++i) {
          const dim_t j = (var_t(diff) + i) % enc.N;
          C.push_back(AloAmo::Lit(enc({i,j},k)));
        }
        eo(out, C, enc);
      }
  }
  template <class ENC>
  void eoantidiagonals(std::ostream& out, const ENC& enc,
                       const var_t bound = 0) {
    const var_t B = bound == 0 ? enc.N : bound;
    for (var_t sum = enc.N; sum < enc.N+B; ++sum)
      for (dim_t k = 0; k < enc.N; ++k) {
        AloAmo::Clause C;
        for (dim_t i = 0; i < enc.N; ++i) {
          const dim_t j = (sum - i) % enc.N;
          C.push_back(AloAmo::Lit(enc({i,j},k)));
        }
        eo(out, C, enc);
      }
  }
  template <class ENC>
  void amoeosudoku(std::ostream& out, const ENC& enc) {
    const auto N = enc.N;
    using Clause = AloAmo::Clause;
    using Lit = AloAmo::Lit;
    const auto b = enc.b, q = enc.q, r = enc.r;
    const auto cf = enc.allows_eo ? PQOptions::CF::eo : PQOptions::CF::amo;

    for (dim_t i = 0; i < q; ++i) {
      const dim_t x = i*b;
      for (dim_t j = 0; j < q; ++j) {
        const dim_t y = j*b;
        for (dim_t k = 0; k < N; ++k) {
          Clause C;
          for (dim_t i1 = 0; i1 < b; ++i1) {
            const dim_t x1 = x+i1;
            for (dim_t j1 = 0; j1 < b; ++j1) {
              const dim_t y1 = y+j1;
              C.push_back(Lit(enc({x1,y1},k)));
            }
          }
          assert(C.size() == b*b);
          amoeo(out, C, enc, cf);
        }
      }
    }

    if (r >= 2) {
      // right edge:
      for (dim_t i = 0; i < q; ++i) {
        const dim_t x = i*b, y = q*b; // top-left
        for (dim_t k = 0; k < N; ++k) {
          Clause C;
          for (dim_t i1 = 0; i1 < b; ++i1) {
            const dim_t x1 = x+i1;
            for (dim_t j1 = 0; j1 < r; ++j1) {
              const dim_t y1 = y+j1;
              C.push_back(Lit(enc({x1,y1},k)));
            }
          }
          assert(C.size() == b*r);
          amoeo(out, C, enc, PQOptions::CF::amo);
        }
      }
      // bottom edge:
      for (dim_t j = 0; j < q; ++j) {
        const dim_t x = q*b, y = j*b; // bottom-left
        for (dim_t k = 0; k < N; ++k) {
          Clause C;
          for (dim_t i1 = 0; i1 < r; ++i1) {
            const dim_t x1 = x+i1;
            for (dim_t j1 = 0; j1 < b; ++j1) {
              const dim_t y1 = y+j1;
              C.push_back(Lit(enc({x1,y1},k)));
            }
          }
          assert(C.size() == b*r);
          amoeo(out, C, enc, PQOptions::CF::amo);
        }
      }
    }

    if (r >= 3) { // bottom corner
      const dim_t x = q*b, y = q*b; // top-left
      for (dim_t k = 0; k < N; ++k) {
        Clause C;
        for (dim_t i1 = 0; i1 < r; ++i1) {
          const dim_t x1 = x+i1;
          for (dim_t j1 = 0; j1 < r; ++j1) {
            const dim_t y1 = y+j1;
            C.push_back(Lit(enc({x1,y1},k)));
          }
        }
        assert(C.size() == r*r);
        amoeo(out, C, enc, PQOptions::CF::amo);
      }
    }
  }

  void pandiagonal(std::ostream& out, const PEncoding& enc,
                   const bool sudoku) {
    out << Statistics::dimacs_pars(enc.p);

    rowreduced(out, enc);
    eovalues(out, enc);
    eorows(out, enc);
    eocolumns(out, enc);
    eodiagonals(out, enc);
    eoantidiagonals(out, enc);

    if (sudoku and enc.N >= 9) amoeosudoku(out, enc);

#ifndef NDEBUG
    assert(running_counter == enc.p.c);
#endif

  }


  struct CEncoding : PEncoding {
    const Statistics::fdimacs_pars pc;
    constexpr CEncoding(const dim_t N, const PQOptions::CT ct,
                        const bool sudoku) noexcept :
    PEncoding(N, ct, sudoku, 0), pc(parsc()) {
      next = N2; assert(next <= p.n);
    }

   var_t operator()() const noexcept {
      return PEncoding::operator()();
    }
    // The real variables are those of the first column:
    constexpr var_t index(const dim_t i, const dim_t k) const noexcept {
      assert(i < N); assert(k < N);
      return 1 + i*N + k;
    }
    constexpr var_t operator()(const cell_t& c, const dim_t k) const noexcept {
      assert(valid(c, N)); assert(k < N);
      const var_t k2 = ((N - var_t(c.j)) + var_t(k)) % N; // ???
      return index(c.i, k2);
    }

    constexpr Statistics::fdimacs_pars parsc() const noexcept {
      const float_t num_cells = N;
      const float_t num_vars_square = N2; // direct encoding

      /* Pure pandiagonal conditions: */
      const float_t num_all_different = 3;
      const float_t num_eos = num_cells + N * num_all_different;

      const float_t num_var_eo = n_amoaloeo(N, ct, PQOptions::CF::eo);
      const float_t num_var_alleos = num_eos * num_var_eo;
      const float_t n = num_vars_square + num_var_alleos;

      const float_t num_clauses_rred = 1;
      const float_t num_clauses_eo = c_amoaloeo(N, ct, PQOptions::CF::eo);
      const float_t num_clauses_alleos = num_eos * num_clauses_eo;
      const float_t c = num_clauses_rred + num_clauses_alleos;

      if (not sudoku) return {n,c};
      /* Additional Sudoky conditions: */

      const auto [nsud, csud] = [this](){
        float_t nsud = 0, csud = 0;
        for (const auto& [p, cf] : boxes) {
          const auto& [count, size] = p;
          nsud += count * n_amoaloeo(size, ct, cf) * N;
          csud += count * c_amoaloeo(size, ct, cf) * N;
        }
        return std::array{nsud, csud};}();
      return {n + nsud, c + csud};
    }

  };

  void cpandiagonal(std::ostream& out, const CEncoding& enc,
                    const bool sudoku) {
    out << Statistics::dimacs_pars(enc.pc);

    rowreduced(out, enc, 1);
    eovalues(out, enc, 1);
    eocolumns(out, enc, 1);
    eodiagonals(out, enc, 1);
    eoantidiagonals(out, enc, 1);

    if (sudoku and enc.N >= 9) amoeosudoku(out, enc);

#ifndef NDEBUG
    assert(running_counter == enc.pc.c);
#endif

  }

}

#endif
