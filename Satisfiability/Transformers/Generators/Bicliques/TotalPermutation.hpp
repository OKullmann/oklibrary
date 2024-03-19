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

  template <class CLS>
  RandGen::vec_eseed_t seeds(const CLS& F) noexcept {
    RandGen::vec_eseed_t res;
    res.push_back(F.first.n); res.push_back(F.first.c);
    namespace FP = FloatingPoint;
    res.push_back(FP::hash_UInt_range().apply(F.second,
      [](const auto& C)noexcept{return FP::hash(C.size());}));
    {FP::UInt_t seed = FP::hash_UInt_range()(res);
     for (const auto& C : F.second) {
       const auto cseed = FP::hash_UInt_range().apply(C,
         [](const DimacsTools::Lit& x)noexcept{
              return FP::hash_Int(x.convert());});
       FP::hash_combine(seed, cseed);
     }
     res.push_back(seed);
    }
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
