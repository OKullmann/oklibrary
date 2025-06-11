// Oliver Kullmann, 19.3.2024 (Swansea)
/* Copyright 2024 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#ifndef TOTALPERMUTATION_SDXN2AHpi1
#define TOTALPERMUTATION_SDXN2AHpi1

#include <string>
#include <vector>
#include <ostream>

#include <Transformers/Generators/Random/Numbers.hpp>
#include <Numerics/NumBasicFunctions.hpp>

#include "DimacsTools.hpp"

namespace TotalPermutation {

  std::string assignment_file(const int argc, const char* const argv[]) {
    if (argc >= 2) return argv[1];
    else return "";
  }

  /*
    The seed of a CLS F is an eseed-vector of length 4:
     1. n
     2. c
     3. h0 : Combining the hashes of C.size() for C in F, seeded with c.
     4. h1 : Seeding with FP::hash_UInt_range()({n,c,h0}), combining the
        hashes of the literals of F (as int's, in the given order).

    Concept for F:
      - as DimacsClauseList, has F.first and F.second
      - F.first has .n and .c
      - F.second is a range of ranges CL, where CL is a range
        of DimacsTools::Lit.
  */
  template <class CLS>
  RandGen::vec_eseed_t seeds(const CLS& F) noexcept {
    RandGen::vec_eseed_t res;
    res.push_back(F.first.n); res.push_back(F.first.c);
    namespace FP = FloatingPoint;
    res.push_back(FP::hash_sizes(F.second));
    res.push_back(FP::hash_ranges(F.second, FP::hash_UInt_range()(res)));
    return res;
  }

  typedef std::vector<RandGen::gen_uint_t> varvec_t;
  varvec_t random_variables(RandGen::RandGen_t& g,
                            const RandGen::gen_uint_t n) {
    varvec_t res = RandGen::random_permutation<varvec_t>(n, g);
    for (RandGen::gen_uint_t& v : res) ++v;
    return res;
  }

  typedef std::vector<RandGen::lit_t> signvec_t;
  signvec_t random_signs(RandGen::RandGen_t& g,
                         const RandGen::gen_uint_t n) {
    signvec_t res; res.reserve(n);
    for (RandGen::gen_uint_t i = 0; i < n; ++i)
      res.push_back(RandGen::bernoulli(g) ? 1 : -1);
    return res;
  }

  template <class CLS>
  void output(std::ostream& out,
              const CLS& F,
              const RandGen::vec_eseed_t& s,
              RandGen::RandGen_t& g,
              const varvec_t& V, const signvec_t& S) {
    assert(V.size() == S.size());
    out << "c ";
    Environment::out_line(out, s);
    out << "\n" << F.first;
    for (auto C : F.second) {
      RandGen::shuffle(C.begin(), C.end(), g);
      for (const DimacsTools::Lit& x : C) {
        const DimacsTools::Var v = var(x);
        assert(v.v >= 1);
        assert(v.v-1 < V.size());
        const DimacsTools::Lit nx(V[v.v-1], x.sign() * S[v.v-1]);
        out << nx << " ";
      }
      out << "0\n";
    }
  }

}

#endif
