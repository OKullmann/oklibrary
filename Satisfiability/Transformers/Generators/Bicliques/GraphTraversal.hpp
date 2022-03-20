// Oliver Kullmann, 12.3.2022 (Swansea)
/* Copyright 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Graph traversal (BFS, DFS, etc.), for various purposes

*/

#include <vector>
#include <algorithm>
#include <stack>

#include <cassert>
#include <cstdint>

#include <Numerics/Statistics.hpp>

#include "Graphs.hpp"

#ifndef GRAPHTRAVERSAL_ipqWptxVwS
#define GRAPHTRAVERSAL_ipqWptxVwS

namespace GraphTraversal {

  namespace GR = Graphs;

  typedef std::uint64_t size_t;
  constexpr size_t max_size = 18446744073709551615UL; // 2^64-1


  struct CCbyIndices {
    typedef std::vector<size_t> indvec_t;
    indvec_t cv; // mapping every vertex-index to its cc-index
    size_t numcc; // number of connected components

    explicit CCbyIndices(const size_t n = 0, const size_t cc = 0)
      : cv(n), numcc(cc) {}
    CCbyIndices(indvec_t cv, const size_t cc) : cv(cv), numcc(cc) {}

    typedef std::vector<size_t> sizes_t;
    sizes_t sizes() const {
      sizes_t res(numcc);
      for (const auto c : cv) {
        assert(1 <= c and c <= numcc);
        ++res[c-1];
      }
      return res;
    }

    bool operator ==(const CCbyIndices&) const noexcept = default;
  };
  // The elements of cv are exactly the numbers in [1, ..., numcc]:
  inline bool valid(const CCbyIndices& cc) {
    const size_t C = cc.numcc;
    if (C == 0) return cc.cv.empty();
    std::vector<size_t> found(C);
    for (const size_t i : cc.cv)
      if (i == 0 or i > C) return false;
      else ++found[i-1];
    return std::ranges::find(found, 0) == found.end();
  }

  // Statistics on the connected components:
  struct StatsCC {
    const size_t N; // number of components
    typedef CCbyIndices::sizes_t sizes_t;
    sizes_t sizes; // sizes of the components
    typedef GenStats::FreqStats<size_t, FloatingPoint::float80> stats_t;
    stats_t S;

    StatsCC(const CCbyIndices& cc)
      : N(cc.numcc), sizes(cc.sizes()), S(sizes) {}

  };


  CCbyIndices cc_by_dfs(const GR::AdjVecUInt& G) {
    const size_t n = G.n();
    CCbyIndices res(n);
    using id_t = GR::AdjVecUInt::id_t;
    for (id_t v = 0; v < n; ++v) {
      if (res.cv[v] != 0) continue;
      res.cv[v] = ++res.numcc;
      std::stack<id_t> S;
      for (const id_t w : G.neighbours(v)) if (res.cv[w] == 0) S.push(w);
      while (not S.empty()) {
        const id_t v = S.top(); S.pop();
        if (res.cv[v] != 0) continue;
        res.cv[v] = res.numcc;
        for (const id_t w : G.neighbours(v)) if (res.cv[w] == 0) S.push(w);
      }
    }
    assert(valid(res));
    return res;
  }

}

#endif
