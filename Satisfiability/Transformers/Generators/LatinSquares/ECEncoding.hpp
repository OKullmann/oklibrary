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

#include "Statistics.hpp"
#include "PQEncoding.hpp"
#include "Algorithms.hpp"

namespace ECEncoding {

  using dim_t = Statistics::dim_t;
  using var_t = Statistics::var_t;

  using float_t = Statistics::fdimacs_pars::float_t;

  struct EC0Encoding {
    const Algorithms::Cubing_t& C;
    const PQOptions::CT ct;
    const var_t N;
    const var_t m;
    const var_t n0 = N * m; // primary variables
    const Statistics::fdimacs_pars p;

  protected :
    mutable var_t next = n0;
  public :

    EC0Encoding(const Algorithms::Cubing_t& C,
                const PQOptions::CT ct) noexcept :
    C(C), ct(ct), N(C.N), m(C.m) {
      // XXX
    }

    var_t operator()() const noexcept {
      return ++next;
    }

  };

}

#endif
