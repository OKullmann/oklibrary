// Oliver Kullmann, 19.12.2020 (Swansea)
/* Copyright 2020 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*



*/

#ifndef TAU_MPFR_BO5LzMX3oE
#define TAU_MPFR_BO5LzMX3oE

#include <mpfr.h>

#include "FloatingPoint.hpp"

namespace Tau_mpfr {

  namespace FP = FloatingPoint;

  // Default rounding-mode:
  constexpr auto defrnd = MPFR_RNDN;
  // A global precision is used, as set by mpfr_set_default_prec:
  constexpr mpfr_prec_t defprec = 100; // "enough" to handle float80

  void mpfr_set_defprec() noexcept {
    mpfr_set_default_prec(defprec);
  }

  FP::float80 to_float80(const mpfr_t& x) {
    return mpfr_get_ld(x, defrnd);
  }
  FP::float64 to_float64(const mpfr_t& x) {
    return mpfr_get_d(x, defrnd);
  }


  inline void mpfr_elem_lb(mpfr_t& rx) noexcept {
    mpfr_t log4;
    mpfr_init(log4);
    mpfr_const_log2(log4, defrnd);
    mpfr_mul_ui(log4, log4, 2, defrnd);
    mpfr_add_ui(rx, rx, 1, defrnd);
    mpfr_div(rx, log4, rx, defrnd);
    mpfr_clear(log4);
  }

}

#endif
