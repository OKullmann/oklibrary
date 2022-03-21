// Oliver Kullmann, 21.3.2022 (Swansea)
/* Copyright 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Partial squares


TODOS:

1. Clarify the relations to Random/LatinSquares.hpp
 - Especially Random::ls_ip_t is similar to PSquares.
 - Though the context is different: in Random/LatinSquares.hpp one is
   concerned with direct computation with latin squares, while here
   we are concerned with representing conditions on (arbitrary) squares.

*/

#ifndef PARTIALSQUARES_xfWDmQELnQ
#define PARTIALSQUARES_xfWDmQELnQ

#include <vector>
#include <algorithm>
#include <istream>

#include "Conditions.hpp"

namespace PartialSquares {

  namespace CD = Conditions;

  typedef CD::size_t size_t;

  typedef std::vector<bool> cell_t;
  typedef std::vector<cell_t> prow_t;
  typedef std::vector<prow_t> psquare_t;

  bool valid(const prow_t& pr, const size_t N) noexcept {
    return pr.size() == N and
      std::ranges::all_of(pr, [&N](const cell_t& c){return c.size()==N;});
  }
  bool valid(const prow_t& pr) noexcept {
    return valid(pr, pr.size());
  }
  bool valid(const psquare_t& ps, const size_t N) noexcept {
    return ps.size() == N and
      std::ranges::all_of(ps, [&N](const prow_t& r){return valid(r,N);});
  }
  bool valid(const psquare_t& ps) noexcept {
    return valid(ps, ps.size());
  }

  prow_t empty_prow(const size_t N) {
    const cell_t c(N);
    return prow_t(N, c);
  }
  psquare_t empty_psquare(const size_t N) {
    const prow_t r = empty_prow(N);
    return psquare_t(N, r);
  }

  struct PSquare {
    psquare_t ps;
    CD::Square s;
    const size_t N;
    PSquare(const size_t N) : ps(empty_psquare(N)), s(0), N(N) {}

    // Reads s:
    void read_square_header(std::istream& in) {
      // XXX
    }
    // Reads ps:
    void read_square_body(std::istream& in) {
      // XXX
    }
    void read(std::istream& in) {
      read_square_header(in);
      read_square_body(in);
    }

    bool operator ==(const PSquare&) const = default;
    auto operator <=>(const PSquare&) const = default;
  };

  bool valid(const PSquare& ps) {
    return valid(ps.ps, ps.N);
  }

}

#endif


