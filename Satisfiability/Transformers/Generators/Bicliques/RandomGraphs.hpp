// Oliver Kullmann, 16.8.2023 (Swansea)
/* Copyright 2023, 2025 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Functionality related to random graphs

   - binomial_rgr(n, p, RandGen::RandGen_t&) -> AdjVecUInt
     (the Gilbert-model G(n,p), with independently chosen edges)

   - uniform_rgr(n, m, RandGen::RandGen_t&) -> AdjVecUInt
     (the Erdoes-Renyi-model G(n,m), with m edges chosen uniformly random)


TODOS:

1. Also supply the directed cases.
  - Now called "D(n,p)" resp. "D(n,m)" (and still "binomial" resp. "uniform").
  - Perhaps a further argument of type GT, with default GT::und.
  - In the binomial model, one needs additionally to consider the
    "back edges".
  - In the uniform model, just the scope of "all edges" is extended.

2. Use views to avoid in uniform_rgr the double-storage of the edges.



See plans/general.txt.

*/

#ifndef RANDOMGRAPHS_t6peLPimmH
#define RANDOMGRAPHS_t6peLPimmH

#include <exception>
#include <sstream>
#include <utility>

#include <Transformers/Generators/Random/Distributions.hpp>
#include <Transformers/Generators/Random/Algorithms.hpp>

#include "Graphs.hpp"
#include "Combinatorics.hpp"

namespace RandomGraphs {

  typedef Graphs::AdjVecUInt AdjVecUInt;

  typedef FloatingPoint::UInt_t UInt_t;

  constexpr FloatingPoint::float80 max_number_edges = FloatingPoint::pow(2,63);


  // Creating a "binomial random graph" (undirected), or in other
  // words, using the "Gilbert-model G(n,p)":
  AdjVecUInt binomial_rgr(const UInt_t n, const RandGen::Prob64 p,
                               RandGen::RandGen_t& g,
                               const bool no_loops = true) {
    const auto type = Graphs::GT::und;
    if (n <= 0) return AdjVecUInt(type);
    if (p.zero()) return AdjVecUInt(type, n);
    const FloatingPoint::float80 expected_E = no_loops ?
      FloatingPoint::float80(p) * n * (n-1) / 2 :
      FloatingPoint::float80(p) * n * (n+1) / 2;
    if (expected_E >= max_number_edges) {
      std::ostringstream m;
      m << "ERROR[RandomGraphs::binomial_rgr]: for n=" << n << " and "
        << "p=" << p << " the expected number of edges " << expected_E
        << " is at least " << max_number_edges << ".";
      throw std::overflow_error(m.str());
    }
    if (p.one()) return Graphs::make_complete_AdjVecUInt(type,
                                                         not no_loops, n);
    AdjVecUInt::adjlist_t A(n);
    RandGen::Bernoulli B(g, p);
    for (UInt_t v = 0; v < n - no_loops; ++v) {
      auto& L = A[v];
      for (UInt_t w = v + no_loops; w < n; ++w)
        if (B()) {
          L.push_back(w);
          if (no_loops or v != w) A[w].push_back(v);
        }
    }
    return {type, std::move(A)};
  }


  // Creating a "uniform random graph" (undirected), or in other
  // words, using the "Erdoes-Renyi-model G(n,m)"):
  AdjVecUInt uniform_rgr(const Graphs::Sizes::uint32_t n0, const UInt_t m,
                         RandGen::RandGen_t& g,
                         const bool no_loops = true) {
    const UInt_t max_m =
      Graphs::Sizes::max_m(n0, Graphs::GT::und, not no_loops);
    assert(m <= max_m);
    const UInt_t n = n0;
    assert(Graphs::Sizes::allops(n)); // holds automatically due to type of n0
    const auto edge_indices = RandGen::choose_kn(m, max_m, g);
    AdjVecUInt::vecedges_t E; E.reserve(m);
    const Combinatorics::CoLexicographic CL(no_loops ?
      Combinatorics::PaTy::sortedneq : Combinatorics::PaTy::sorted);
    for (const UInt_t i : edge_indices) E.push_back(CL(i));
    return Graphs::make_AdjVecUInt(Graphs::GT::und, n, E);
  }

}

#endif
