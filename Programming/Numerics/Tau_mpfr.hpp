// Oliver Kullmann, 19.12.2020 (Swansea)
/* Copyright 2020 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

TODOS:

1. Implement tau etc.

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


  /* General tools */

  // Default rounding-mode:
  constexpr auto defrnd = MPFR_RNDN;


  static_assert(MPFR_PREC_MAX <= FP::P264m1);
  // A global precision is used, as set by mpfr_set_default_prec:
  constexpr mpfr_prec_t defprec = 100; // "enough" to handle float80
  static_assert(defprec >= MPFR_PREC_MIN);
  static_assert(defprec <= MPFR_PREC_MAX);
  inline void dinit(mpfr_t& x) noexcept {mpfr_init2(x, defprec);}

  constexpr FP::UInt_t multiplier = 4;
  constexpr bool valid_dec_prec(const FloatingPoint::UInt_t dec_prec) {
    return dec_prec <= MPFR_PREC_MAX / multiplier;
  }
  constexpr mpfr_prec_t dec2bin_prec(const FloatingPoint::UInt_t dec_prec) {
    return std::max(mpfr_prec_t(multiplier*dec_prec), defprec);
  }


  inline FP::float80 to_float80(const mpfr_t& x) {
    return mpfr_get_ld(x, defrnd);
  }
  inline FP::float64 to_float64(const mpfr_t& x) {
    return mpfr_get_d(x, defrnd);
  }

  constexpr int base = 10;
  inline std::string to_string(const mpfr_t& x, const FloatingPoint::UInt_t dec_prec) {
    mpfr_exp_t expo;
    char* const resp = mpfr_get_str(nullptr, &expo, base, dec_prec, x, defrnd);
    std::string res(resp);
    res = "0." + res + "e" + std::to_string(expo);
    mpfr_free_str(resp);
    return res;
  }


  /* Computations around the tau-function */

  inline void elem_lb(mpfr_t& rx) noexcept {
    const auto prec = mpfr_get_prec(rx);
    mpfr_t log4;
    mpfr_init2(log4,prec);
    mpfr_const_log2(log4, defrnd);
    mpfr_mul_ui(log4, log4, 2, defrnd);
    mpfr_add_ui(rx, rx, 1, defrnd);
    mpfr_div(rx, log4, rx, defrnd);
    mpfr_clear(log4);
  }

  inline void lambertW0_lb(mpfr_t& x) noexcept {
    assert(mpfr_cmp_ui(x,1) > 0);
    const auto prec = mpfr_get_prec(x);
    mpfr_log(x, x, defrnd);
    mpfr_t llx;
    mpfr_init2(llx, prec); mpfr_set(llx, x, defrnd);
    mpfr_log(llx, llx, defrnd);
    mpfr_neg(llx, llx, defrnd);
    mpfr_t x1;
    mpfr_init2(x1, prec); mpfr_set(x1, x, defrnd);
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
    const auto prec = mpfr_get_prec(a);
    mpfr_t x0, x1;
    mpfr_init2(x0,prec); mpfr_init2(x1,prec);
    if (mpfr_cmp_ld(a, Tau::tau_meaneqLW) <= 0) {
      mpfr_ui_div(a, 1, a, defrnd);
      mpfr_set(x0, a, defrnd);
      elem_lb(x0);
    }
    else {
      mpfr_set(x0, a, defrnd);
      lambertW0_lb(x0);
      mpfr_ui_div(a, 1, a, defrnd);
    }
    mpfr_t A, B, N, D;
    mpfr_inits2(prec, A,B,N,D, mpfr_ptr(nullptr));
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
    mpfr_clears(x0, x1, A, B, N, D, mpfr_ptr(nullptr));
  }

  inline void ltau(mpfr_t& a, mpfr_t& b) noexcept {
    assert(mpfr_cmp_ui(a,0) >= 0);
    assert(mpfr_cmp_ui(b,0) >= 0);
    if (mpfr_greater_p(a,b) > 0) mpfr_swap(a,b);
    if (mpfr_zero_p(a)) {
      if (mpfr_inf_p(b)) mpfr_set_nan(a);
      else mpfr_set_inf(a,1);
      return;
    }
    if (mpfr_inf_p(b)) {mpfr_set_zero(a,0); return;}
    if (mpfr_equal_p(a,b)) {
      mpfr_const_log2(a,defrnd); mpfr_div(a,a,b,defrnd);
      return;
    }
    mpfr_div(a,b,a,defrnd);
    wtau(a);
    mpfr_div(a,a,b,defrnd);
  }


  /* Special values */

  // tau(1,1) = 2:
  inline void tau11(mpfr_t& a) noexcept {
    mpfr_set_ui(a,2,defrnd);
  }
  inline void ltau11(mpfr_t& a) noexcept {
    tau11(a);
    mpfr_log(a,a,defrnd);
  }

  // The golden ratio tau(1,2) ("phi"):
  inline void tau12(mpfr_t& a) noexcept {
    mpfr_sqrt_ui(a,5,defrnd);
    mpfr_add_ui(a,a,1,defrnd);
    mpfr_div_ui(a,a,2,defrnd);
  }
  inline void ltau12(mpfr_t& a) noexcept {
    tau12(a);
    mpfr_log(a,a,defrnd);
  }

  // tau(1,3):
  inline void tau13(mpfr_t& a) {
    const auto prec = mpfr_get_prec(a);
    mpfr_t b;
    mpfr_init2(b, prec);
    mpfr_sqrt_ui(a,31,defrnd);
    mpfr_sqrt_ui(b,3,defrnd);
    mpfr_pow_ui(b,b,3,defrnd);
    mpfr_mul_ui(b,b,2,defrnd);
    mpfr_div(a,a,b,defrnd);
    mpfr_set_ui(b,29,defrnd);
    mpfr_div_ui(b,b,54,defrnd);
    mpfr_add(a,a,b,defrnd);
    mpfr_cbrt(a,a,defrnd);
    mpfr_set(b,a,defrnd);
    mpfr_mul_ui(b,b,9,defrnd);
    mpfr_ui_div(b,1,b,defrnd);
    mpfr_add(a,a,b,defrnd);
    mpfr_set_ui(b,3,defrnd);
    mpfr_ui_div(b,1,b,defrnd);
    mpfr_add(a,a,b,defrnd);
    mpfr_clear(b);
  }
  inline void ltau13(mpfr_t& a) {
    tau13(a);
    mpfr_log(a,a,defrnd);
  }

  // tau(1,4):
  inline void tau14(mpfr_t& a) {
    mpfr_sqrt_ui(a,3,defrnd);     // a completed
    const auto prec = mpfr_get_prec(a);
    mpfr_t b; mpfr_init2(b, prec);
    mpfr_sqrt_ui(b,283,defrnd);
    mpfr_t c; mpfr_init2(c, prec);
    mpfr_pow_ui(c,a,3,defrnd);
    mpfr_mul_ui(c,c,2,defrnd);
    mpfr_div(b,b,c,defrnd);
    mpfr_set_si(c,-1,defrnd);
    mpfr_div_ui(c,c,2,defrnd);
    mpfr_add(b,b,c,defrnd);       // b completed
    mpfr_set(c,b,defrnd);
#if MPFR_VERSION_MAJOR >= 4
    mpfr_rootn_ui(c,c,6,defrnd);  // c completed
#else
    mpfr_root(c,c,6,defrnd);  // c completed
#endif
    mpfr_t d; mpfr_init2(d, prec);
    mpfr_set(d,b,defrnd);
    mpfr_cbrt(d,d,defrnd);        // d completed
    mpfr_t e; mpfr_init2(e, prec);
    mpfr_set(e,d,defrnd);
    mpfr_mul_ui(e,e,3,defrnd);
    mpfr_sub_ui(e,e,16,defrnd);
    mpfr_t f; mpfr_init2(f, prec);
    mpfr_set(f,b,defrnd);
    mpfr_sqr(f,f,defrnd);
    mpfr_cbrt(f,f,defrnd);
    mpfr_mul_ui(f,f,12,defrnd);
    mpfr_add(e,e,f,defrnd);
    mpfr_sqrt(e,e,defrnd);        // e completed
    mpfr_set(f,a,defrnd);
    mpfr_mul(f,f,c,defrnd);
    mpfr_div_ui(f,f,2,defrnd);
    mpfr_div(f,f,e,defrnd);
    mpfr_sub(f,f,d,defrnd);
    mpfr_t g; mpfr_init2(g, prec);
    mpfr_set_ui(g,4,defrnd);
    mpfr_div_ui(g,g,3,defrnd);
    mpfr_div(g,g,d,defrnd);
    mpfr_add(f,f,g,defrnd);
    mpfr_set_ui(g,1,defrnd);
    mpfr_div_ui(g,g,2,defrnd);
    mpfr_add(f,f,g,defrnd);
    mpfr_sqrt(f,f,defrnd);
    mpfr_div_ui(f,f,2,defrnd);
    mpfr_set(g,e,defrnd);
    mpfr_div_ui(g,g,4,defrnd);
    mpfr_div(g,g,a,defrnd);
    mpfr_div(g,g,c,defrnd);
    mpfr_add(f,f,g,defrnd);
    mpfr_set_ui(g,1,defrnd);
    mpfr_div_ui(g,g,4,defrnd);
    mpfr_add(f,f,g,defrnd);
    mpfr_set(a,f,defrnd);
    mpfr_clears(b,c,d,e,f,g, mpfr_ptr(nullptr));
  }
  inline void ltau14(mpfr_t& a) {
    tau14(a);
    mpfr_log(a,a,defrnd);
  }

  // tau(1,5):
  inline void tau15(mpfr_t& a) {
    const auto prec = mpfr_get_prec(a);
    mpfr_t b; mpfr_init2(b, prec);
    mpfr_sqrt_ui(a,23,defrnd);
    mpfr_sqrt_ui(b,3,defrnd);
    mpfr_pow_ui(b,b,3,defrnd);
    mpfr_mul_ui(b,b,2,defrnd);
    mpfr_div(a,a,b,defrnd);
    mpfr_set_ui(b,1,defrnd);
    mpfr_div_ui(b,b,2,defrnd);
    mpfr_add(a,a,b,defrnd);
    mpfr_cbrt(a,a,defrnd);
    mpfr_set(b,a,defrnd);
    mpfr_mul_ui(b,b,3,defrnd);
    mpfr_ui_div(b,1,b,defrnd);
    mpfr_add(a,a,b,defrnd);
    mpfr_clear(b);
  }
  inline void ltau15(mpfr_t& a) {
    tau15(a);
    mpfr_log(a,a,defrnd);
  }

  std::string const_func(void f(mpfr_t&), const FloatingPoint::UInt_t dec_prec) {
    if (not valid_dec_prec(dec_prec)) return "ERROR:prec";
    const mpfr_prec_t prec = dec2bin_prec(dec_prec);
    mpfr_t a; mpfr_init2(a,prec);
    f(a);
    const std::string res = to_string(a, dec_prec);
    mpfr_clear(a);
    return res;
  }


  /* Wrappers */

  // Returning the result as a string, and clearing a:
  std::string wtau(mpfr_t& a, const FloatingPoint::UInt_t dec_prec) {
    if (mpfr_cmp_ui(a,1) >= 0) wtau(a);
    else {
      mpfr_t orig_a;
      const auto prec = mpfr_get_prec(a);
      mpfr_init2(orig_a, prec); mpfr_set(orig_a, a, defrnd);
      mpfr_ui_div(a,1,a,defrnd);
      wtau(a);
      mpfr_mul(a,a,orig_a,defrnd);
      mpfr_clear(orig_a);
    }
    const std::string res = to_string(a, dec_prec);
    mpfr_clear(a);
    return res;
  }

  std::string wtau(const FloatingPoint::float80 x, const FloatingPoint::UInt_t dec_prec) {
    namespace FP = FloatingPoint;

    if (FP::isnan(x)) return "NaN";
    if (x < 0) return "NaN";
    if (x == 0) return "0";
    if (x == FP::pinfinity) return "inf";

    if (not valid_dec_prec(dec_prec)) return "ERROR:prec";
    const mpfr_prec_t prec = dec2bin_prec(dec_prec);

    mpfr_t a; mpfr_init2(a,prec); mpfr_set_ld(a,x,defrnd);
    return wtau(a, dec_prec);
  }
  std::string wtau(const std::string& x, const FloatingPoint::UInt_t dec_prec) {
    if (not valid_dec_prec(dec_prec)) return "ERROR:prec";
    const mpfr_prec_t prec = dec2bin_prec(dec_prec);

    mpfr_t a; mpfr_init2(a,prec);
    const int parse = mpfr_set_str(a, x.c_str(), base, defrnd);
    if (parse == -1) return "ERROR:parse";
    assert(parse == 0);
    if (mpfr_nan_p(a)) return "NaN";
    if (mpfr_cmp_ui(a, 0) < 0) return "NaN";
    if (mpfr_zero_p(a)) return "0";
    if (mpfr_inf_p(a)) return "inf";

    return wtau(a, dec_prec);
  }


  // Returning the result as a string, and clearing a, b:
  std::string ltau(mpfr_t& a, mpfr_t& b, const FloatingPoint::UInt_t dec_prec) {
    ltau(a,b);
    const std::string res = to_string(a, dec_prec);
    mpfr_clear(a); mpfr_clear(b);
    return res;
  }

  std::string ltau(const FloatingPoint::float80 x, const FloatingPoint::float80 y, const FloatingPoint::UInt_t dec_prec) {
    namespace FP = FloatingPoint;
    if (FP::isnan(x) or FP::isnan(y)) return "NaN";
    if (x < 0 or y < 0) return "NaN";
    if ((x == FP::pinfinity and y == 0) or (x == 0 and y == FP::pinfinity))
      return "NaN";
    if (x == 0 or y == 0) return "inf";
    if (x == FP::pinfinity or y == FP::pinfinity) return "0";
    if (not valid_dec_prec(dec_prec)) return "ERROR:prec";
    const mpfr_prec_t prec = dec2bin_prec(dec_prec);
    mpfr_t a, b; mpfr_init2(a,prec); mpfr_init2(b,prec);
    mpfr_set_ld(a,x,defrnd); mpfr_set_ld(b,y,defrnd);
    return ltau(a, b, dec_prec);
  }
  std::string ltau(const std::string& x, const std::string& y, const FloatingPoint::UInt_t dec_prec) {
    if (not valid_dec_prec(dec_prec)) return "ERROR:prec";
    const mpfr_prec_t prec = dec2bin_prec(dec_prec);

    mpfr_t a; mpfr_init2(a,prec);
    {const int parse = mpfr_set_str(a, x.c_str(), base, defrnd);
     if (parse == -1) return "ERROR:parse";
     assert(parse == 0);
    }
    mpfr_t b; mpfr_init2(b,prec);
    {const int parse = mpfr_set_str(b, y.c_str(), base, defrnd);
     if (parse == -1) return "ERROR:parse";
     assert(parse == 0);
    }

    return ltau(a, b, dec_prec);
  }

}

#endif
