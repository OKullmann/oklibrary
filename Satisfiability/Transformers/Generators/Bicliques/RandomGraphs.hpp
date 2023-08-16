// Oliver Kullmann, 16.8.2023 (Swansea)
/* Copyright 2023 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Functionality related to random graphs

   - independent_edges(n, p, RandGen::RandGen_t&) -> AdjVecUInt

*/

#ifndef RANDOMGRAPHS_t6peLPimmH
#define RANDOMGRAPHS_t6peLPimmH

#include <Transformers/Generators/Random/Distributions.hpp>

#include "Graphs.hpp"

namespace RandomGraphs {

  typedef Graphs::AdjVecUInt AdjVecUInt;

  typedef AdjVecUInt::size_t size_t;


  // Creating an undirected graph without loops:
  AdjVecUInt independent_edges(const size_t n, const RandGen::Prob64 p,
                               RandGen::RandGen_t& g) {
    const auto type = Graphs::GT::und;
    if (n <= 1) return AdjVecUInt(type);
    if (p.zero()) return AdjVecUInt(type, n);
    if (p.one()) return Graphs::make_complete_AdjVecUInt(type, false, n);
    AdjVecUInt::adjlist_t A(n);
    RandGen::Bernoulli B(g, p);
    for (size_t v = 0; v < n-1; ++v) {
      auto& L = A[v];
      for (size_t w = v+1; w < n; ++w)
        if (B()) { L.push_back(w); A[w].push_back(v); }
    }
    return {type, std::move(A)};
  }

}

#endif
