// Oliver Kullmann, 6.11.2023 (Swansea)
/* Copyright 2023 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Randomly permuting
   - the variables and their signs
   - the clause-order
   - the clauses themselves
  of an input-CNF F, computing the seeds deterministically from F.

EXAMPLES:

Bicliques> echo -e "p cnf 5 3\n1 2 -3 0\n2 3 -5 -1 0\n-2 0\n" | ./TotalPermutation
c 5 3 17204549642661511693 6920067891657613469
p cnf 5 3
2 5 -4 0
-4 -2 3 -5 0
4 0

Bicliques> echo -e "p cnf 6 3\n1 2 -3 0\n2 3 -5 -1 0\n-2 0\n" | ./TotalPermutation
c 6 3 17204549642661511693 5128273459892383319
p cnf 6 3
-4 -2 -1 0
1 0
-6 -1 4 2 0


TODOS:

1. If an argument is given, then this is the filename showing the
   isomorphism.

2. This should go to Generators/Random.
    - Then generalise to non-strict inputs.

*/

#include <iostream>
#include <vector>

#include <cassert>

#include <ProgramOptions/Environment.hpp>
#include <Transformers/Generators/Random/Numbers.hpp>
#include <Transformers/Generators/Random/Algorithms.hpp>
#include <Transformers/Generators/Random/Distributions.hpp>
#include <Numerics/NumBasicFunctions.hpp>

#include "DimacsTools.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.0.9",
        "6.11.2023",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Bicliques/TotalPermutation.cpp",
        "GPL v3"};

  using namespace DimacsTools;

  const std::string error = "ERROR[" + proginfo.prg + "]: ";

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
    "> " << proginfo.prg
         << "\n\n"
    " reads a strict Dimacs-file from standard input, and prints the"
    " permuted CNF to standard output.\n\n"
;
    return true;
  }

  RandGen::vec_eseed_t seeds(const DimacsClauseList& F) noexcept {
    RandGen::vec_eseed_t res;
    res.push_back(F.first.n); res.push_back(F.first.c);
    namespace FP = FloatingPoint;
    res.push_back(FP::hash_UInt_range().apply(F.second,
      [](const Clause& C)noexcept{return FP::hash(C.size());}));
    {FP::UInt_t seed = FP::hash_UInt_range()(res);
     for (const auto& C : F.second) {
       const auto cseed = FP::hash_UInt_range().apply(C,
         [](const Lit& x)noexcept{return FP::hash_Int(x.convert());});
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

  void output(std::ostream& out,
              const DimacsClauseList& F, const RandGen::vec_eseed_t& s,
              RandGen::RandGen_t& g,
              const varvec_t& V, const signvec_t& S) {
    assert(V.size() == S.size());
    out << "c ";
    Environment::out_line(out, s);
    out << "\n" << F.first;
    for (Clause C : F.second) {
      RandGen::shuffle(C.begin(), C.end(), g);
      for (const Lit& x : C) {
        const Var v = var(x);
        assert(v.v >= 1);
        assert(v.v-1 < V.size());
        const Lit nx(V[v.v-1], x.sign() * S[v.v-1]);
        out << nx << " ";
      }
      out << "0\n";
    }
  }

}

int main(const int argc, const char* const argv[]) {

  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  if (argc != 1) {
    std::cerr << error <<
      "No arguments needed, but " << argc-1 << " provided.\n";
    return 1;
  }

  DimacsClauseList F = read_strict_Dimacs(std::cin);
  const auto s = seeds(F);
  RandGen::RandGen_t g(s);
  RandGen::shuffle(F.second.begin(), F.second.end(), g);

  const varvec_t V = random_variables(g, F.first.n);
  const signvec_t S = random_signs(g, F.first.n);
  output(std::cout, F, s, g, V, S);
}
