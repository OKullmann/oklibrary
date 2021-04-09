// Oleg Zaikin, 5.4.2021 (Irkutsk)
/* Copyright 2021 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* Lookahead for the Gecode library
*/

#ifndef LOOKAHEAD_lNFKYYpHQ8
#define LOOKAHEAD_lNFKYYpHQ8

#include <cstdint>
#include <cmath>

#include <gecode/int.hh>

namespace Lookahead {

  typedef std::uint64_t sz_t;
  static_assert(std::is_same_v<sz_t, std::uint64_t>);

  inline sz_t mu0(const Gecode::IntVarArray V) noexcept {
    sz_t s = 0;
    for (unsigned i = 0; i < (unsigned)V.size(); ++i)
      s += V[i].size() - 1;
    return s;
  }

  inline sz_t mu1(const Gecode::IntVarArray V) noexcept {
    sz_t s = 0;
    for (unsigned i = 0; i < (unsigned)V.size(); ++i)
      s += std::log2((double)V[i].size());
    return s;
  }

}

#endif
