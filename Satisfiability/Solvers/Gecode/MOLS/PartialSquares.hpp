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

#include <ProgramOptions/Strings.hpp>
#include <Numerics/NumInOut.hpp>

#include "Conditions.hpp"

namespace PartialSquares {

  namespace CD = Conditions;

  typedef CD::size_t size_t;

  // index i being true means value i is *disabled*:
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
    PSquare(const size_t N) : ps(empty_psquare(N)), s(0) {}

    bool operator ==(const PSquare&) const = default;
    auto operator <=>(const PSquare&) const = default;
  };

  bool valid(const PSquare& ps, const size_t N) {
    return valid(ps.ps, N);
  }


  struct PSquares {
    typedef std::vector<PSquare> psquares_t;
    psquares_t psqs;
    const size_t N;

    PSquares(const size_t N, std::istream& in) : psqs(read(in,N)), N(N) {}
    PSquares(const size_t N, const psquares_t p) : psqs(p), N(N) {
      assert(std::ranges::all_of(psqs, [&N](const auto& s){
                                   return valid(s,N);}));
    }

    // For the stream-input, the values are numbers 1, ..., N:
    static psquares_t read(std::istream& in, const size_t N) {
      const auto lines = Environment::split2_cutoff(in, '\n', '#');
      if (lines.size() % (N+1) != 0) {
        // XXX
      }
      const size_t k = lines.size() / (N+1);
      psquares_t res(k, N);
      for (size_t i = 0; i < k; ++i) {
        const size_t i0 = i*(N+1);
        size_t j = 0;
        const auto osq = CD::Square::read(lines[i0], j);
        if (j < lines[i0].size()) {
          // XXX
        }
        if (not osq) {
          // XXX
        }
        res[i].s = osq.value();
        for (size_t ip = 0; ip < N; ++ip) {
          const size_t i1 = i0+1 + ip;
          const auto& line = lines[i1];
          if (line.size() != N) {
            // XXX
          }
          for (size_t j = 0; j < N; ++j) {
            const auto item = Environment::split(line[j], ',');
            const auto first = FloatingPoint::to_F80ai(item[0]);
            if (not first.isint) {
              // XXX
            }
            if (first.x == 0) {
              // XXX
            }
            const bool exclude = first.x < 0;
            if (not exclude) res[i].ps[ip][j].flip();
            {const auto x = FloatingPoint::abs(first.x);
             if (x > N) {
               // XXX
             }
             res[i].ps[ip][j][x-1] = exclude;}
            for (size_t jp = 1; jp < N; ++j) {
              const size_t x = FloatingPoint::toUInt(item[j]);
              if (x == 0) {
                // XXX
              }
              if (x > N) {
                // XXX
              }
              res[i].ps[ip][j][x-1] = exclude;
            }
          }
        }
      }
      return res;
    }
  };

}

#endif


