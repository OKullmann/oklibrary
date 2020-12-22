// Oliver Kullmann, 19.12.2020 (Swansea)
/* Copyright 2020 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

TODOS:

1. Perhaps the use of the glocal precision should be avoided?

2. Write a general overview.


*/

#ifndef TAU_MPFR_BO5LzMX3oE
#define TAU_MPFR_BO5LzMX3oE

#include <string>
#include <algorithm>

// Guaranteed to be included:
#include <mpfr.h>

#include "FloatingPoint.hpp"

namespace Tau_mpfr {

  namespace FP = FloatingPoint;

  // Default rounding-mode:
  constexpr auto defrnd = MPFR_RNDN;


  static_assert(MPFR_PREC_MAX <= FP::P264m1);
  // A global precision is used, as set by mpfr_set_default_prec:
  constexpr mpfr_prec_t defprec = 100; // "enough" to handle float80
  static_assert(defprec >= MPFR_PREC_MIN);
  static_assert(defprec <= MPFR_PREC_MAX);

  void set_defprec() noexcept {
    mpfr_set_default_prec(defprec);
  }


  FP::float80 to_float80(const mpfr_t& x) {
    return mpfr_get_ld(x, defrnd);
  }
  FP::float64 to_float64(const mpfr_t& x) {
    return mpfr_get_d(x, defrnd);
  }


  inline void elem_lb(mpfr_t& rx) noexcept {
    mpfr_t log4;
    mpfr_init(log4);
    mpfr_const_log2(log4, defrnd);
    mpfr_mul_ui(log4, log4, 2, defrnd);
    mpfr_add_ui(rx, rx, 1, defrnd);
    mpfr_div(rx, log4, rx, defrnd);
    mpfr_clear(log4);
  }

  inline void lambertW0_lb(mpfr_t& x) noexcept {
    assert(mpfr_cmp_ui(x,1) > 0);
    mpfr_log(x, x, defrnd);
    mpfr_t llx;
    mpfr_init_set(llx, x, defrnd);
    mpfr_log(llx, llx, defrnd);
    mpfr_neg(llx, llx, defrnd);
    mpfr_t x1;
    mpfr_init_set(x1, x, defrnd);
    mpfr_add_ui(x1, x1, 1, defrnd);
    mpfr_div(x1, x, x1, defrnd);
    mpfr_fma(x, llx, x1, x, defrnd);
    mpfr_clear(llx); mpfr_clear(x1);
  }

  inline void wtau(mpfr_t& a) noexcept {
    assert(mpfr_cmp_ui(a,1) >= 0);
    if (mpfr_inf_p(a)) return;
    if (mpfr_cmp_ui(a,1) == 0) {
      mpfr_const_log2(a, defrnd);
      return;
    }
    mpfr_t x0, x1;
    mpfr_init(x1);
    if (mpfr_cmp_ld(a, Tau::tau_meaneqLW) <= 0) {
      mpfr_ui_div(a, 1, a, defrnd);
      mpfr_init_set(x0, a, defrnd);
      elem_lb(x0);
    }
    else {
      mpfr_init_set(x0, a, defrnd);
      lambertW0_lb(x0);
      mpfr_ui_div(a, 1, a, defrnd);
    }
    mpfr_t A, B, N, D;
    mpfr_init(A); mpfr_init(B); mpfr_init(N); mpfr_init(D);
    while (true) {
      mpfr_set(A, x0, defrnd);
      mpfr_neg(A, A, defrnd);
      mpfr_exp(A, A, defrnd);
      mpfr_mul(B, x0, a, defrnd);
      mpfr_neg(B, B, defrnd);
      mpfr_expm1(B, B, defrnd);
      mpfr_add(N, A, B, defrnd);
      if (mpfr_cmp_ui(N, 0) <= 0) {
        mpfr_set(a, x0, defrnd);
        goto End;
      }
      mpfr_fma(D, a,B,a, defrnd);
      mpfr_add(D, D, A, defrnd);
      mpfr_ui_div(D, 1, D, defrnd);
      assert(mpfr_cmp_ui(D, 0) > 0);
      mpfr_fma(x1, N,D,x0, defrnd);
      assert(mpfr_greaterequal_p(x1, x0));
      if (mpfr_equal_p(x1, x0)) {
        mpfr_set(a, x0, defrnd);
        goto End;
      }
      mpfr_set(x0, x1, defrnd);
    }
    End :
    mpfr_clear(x0); mpfr_clear(x1);
    mpfr_clear(A); mpfr_clear(B); mpfr_clear(N); mpfr_clear(D);
  }


  /* Wrappers */

  constexpr int base = 10;

  // Returning the result as a string, and clearing a:
  std::string wtau(mpfr_t& a, const FloatingPoint::UInt_t dec_prec) {
    if (mpfr_cmp_ui(a,1) >= 0) wtau(a);
    else {
      mpfr_t orig_a; mpfr_init_set(orig_a, a, defrnd);
      mpfr_ui_div(a,1,a,defrnd);
      wtau(a);
      mpfr_mul(a,a,orig_a,defrnd);
      mpfr_clear(orig_a);
    }
    mpfr_exp_t expo;
    char* const resp = mpfr_get_str(nullptr, &expo, base, dec_prec, a, defrnd);
    std::string res(resp);
    res = "0." + res + "e" + std::to_string(expo);

    mpfr_free_str(resp);
    mpfr_clear(a);
    return res;
  }

  constexpr FP::UInt_t multiplier = 4;

  std::string wtau(const FloatingPoint::float80 x, const FloatingPoint::UInt_t dec_prec) {
    namespace FP = FloatingPoint;

    if (FP::isnan(x)) return "NaN";
    if (x < 0) return "NaN";
    if (x == 0) return "0";
    if (x == FP::pinfinity) return "inf";

    if (dec_prec > MPFR_PREC_MAX / multiplier) return "ERROR:prec";
    mpfr_set_default_prec(std::max(mpfr_prec_t(multiplier*dec_prec), defprec));

    mpfr_t a; mpfr_init(a); mpfr_set_ld(a,x,defrnd);
    return wtau(a, dec_prec);
  }
  std::string wtau(const std::string x, const FloatingPoint::UInt_t dec_prec) {
    namespace FP = FloatingPoint;

    mpfr_t a; mpfr_init(a);
    const int parse = mpfr_set_str(a, x.c_str(), base, defrnd);
    if (parse == -1) return "ERROR:parse";
    assert(parse == 0);
    if (mpfr_nan_p(a)) return "NaN";
    if (mpfr_cmp_ui(a, 0) < 0) return "NaN";
    if (mpfr_zero_p(a)) return "0";
    if (mpfr_inf_p(a)) return "inf";

    if (dec_prec > MPFR_PREC_MAX / multiplier) return "ERROR:prec";
    mpfr_set_default_prec(std::max(mpfr_prec_t(multiplier*dec_prec), defprec));

    return wtau(a, dec_prec);
  }

}

#endif
