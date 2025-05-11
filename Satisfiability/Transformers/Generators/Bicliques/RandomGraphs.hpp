// Oliver Kullmann, 16.8.2023 (Swansea)
/* Copyright 2023, 2025 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Functionality related to random graphs

   - independent_edges(n, p, RandGen::RandGen_t&) -> AdjVecUInt
     (the Gilbert-model, with independently chosen edges)


TODOS:

See plans/general.txt.

*/

#ifndef RANDOMGRAPHS_t6peLPimmH
#define RANDOMGRAPHS_t6peLPimmH

#include <exception>
#include <sstream>

#include <Transformers/Generators/Random/Distributions.hpp>

#include "Graphs.hpp"

namespace RandomGraphs {

  typedef Graphs::AdjVecUInt AdjVecUInt;

  typedef AdjVecUInt::size_t size_t;


  constexpr FloatingPoint::float80 max_number_edges = FloatingPoint::pow(2,63);

  // Creating an undirected graph (a "binomial random graph", or in other
  // words, using the "Gilbert-model G(n,p)"):
  AdjVecUInt independent_edges(const size_t n, const RandGen::Prob64 p,
                               RandGen::RandGen_t& g,
                               const bool no_loops = true) {
    const auto type = Graphs::GT::und;
    if (n <= 1) return AdjVecUInt(type);
    if (p.zero()) return AdjVecUInt(type, n);
    const FloatingPoint::float80 expected_E = no_loops ?
      FloatingPoint::float80(p) * n * (n-1) / 2 :
      FloatingPoint::float80(p) * n * (n+1) / 2;
    if (expected_E >= max_number_edges) {
      std::ostringstream m;
      m << "ERROR[RandomGraphs::independent_edges]: for n=" << n << " and "
        << "p=" << p << " the expected number of edges " << expected_E
        << " is at least " << max_number_edges << ".";
      throw std::overflow_error(m.str());
    }
    if (p.one()) return Graphs::make_complete_AdjVecUInt(type,
                                                         not no_loops, n);
    AdjVecUInt::adjlist_t A(n);
    RandGen::Bernoulli B(g, p);
    for (size_t v = 0; v < n - no_loops; ++v) {
      auto& L = A[v];
      for (size_t w = v + no_loops; w < n; ++w)
        if (B()) {
          L.push_back(w);
          if (no_loops or v != w) A[w].push_back(v);
        }
    }
    return {type, std::move(A)};
  }

}

#endif
