// Oliver Kullmann, 12.3.2022 (Swansea)
/* Copyright 2022, 2023 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*
  Graph traversal (BFS, DFS, etc.), for various purposes

  - namespace-abbreviation "GR" for "Graphs"

  - typedef size_t (unsigned 64-bit)
  - constant max_size : size_t

  - struct CCbyIndices : the mapping of a vertex to its cc-index
    ("cc": connected component)
   - typedefs indvec_t, sizes_t : both vector of size_t
   - members cv : indvec_t (vertex-index -> cc-index), numcc : size_t
   - Constructors (n, cc) and (indvec_t, cc)
   - sizes() -> sizes_t : for every cc its size
   - components(sizes_t), components() : the vector of the sorted components
     (the first version, supplied with the sizes_t-vector, applies reserve)
   - operator ==
  - valid(CCbyIndices)

  - struct StatsCC : statistics
   - constructor from CCbyIndices

  - cc_by_dfs(AdjVecUInt) -> CCbyIndices.

*/

#include <vector>
#include <algorithm>
#include <stack>
#include <ostream>

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
    indvec_t cv; // mapping every vertex-index to its cc-index (>= 1)
    // the valid vertices are the valid indices of cv, the cc's are the values
    size_t numcc; // number of connected components
    // the valid cc-indices are 1, ..., numcc

    explicit CCbyIndices(const size_t n = 0, const size_t cc = 0)
      : cv(n), numcc(cc) {}
    CCbyIndices(indvec_t cv, const size_t cc) : cv(cv), numcc(cc) {}

    bool valid_cc(const size_t i) const noexcept {
      return 1 <= i and i <= numcc;
    }

    typedef std::vector<size_t> sizes_t;
    // mapping  cc-index - 1  ->  size
    sizes_t sizes() const {
      sizes_t res(numcc);
      for (const auto c : cv) {
        assert(valid_cc(c));
        ++res[c-1];
      }
      return res;
    }

    typedef std::vector<indvec_t> components_t;
    components_t components(const sizes_t& S) const {
      assert(S.size() == numcc);
      components_t res(numcc);
      for (size_t i = 0; i < numcc; ++i) res[i].reserve(S[i]);
      for (size_t i = 0; i < cv.size(); ++i) {
        const size_t c = cv[i];
        assert(valid_cc(c));
        res[c-1].push_back(i);
      }
      return res;
    }
    components_t components() const {
      components_t res(numcc);
      for (size_t i = 0; i < cv.size(); ++i) {
        const size_t c = cv[i];
        assert(valid_cc(c));
        res[c-1].push_back(i);
      }
      return res;
    }

    // The first cc is 1, the next one 2, and so on:
    bool canonical() const noexcept {
      if (cv.empty()) return true;
      if (cv[0] != 1) return false;
      for (size_t next = 2; const size_t x : cv) {
        if (x > next) return false;
        else if (x == next) ++next;
      }
      return true;
    }

    bool operator ==(const CCbyIndices&) const noexcept = default;

    friend std::ostream& operator <<(std::ostream& out, const CCbyIndices& C) {
      out << C.numcc << ";";
      for (size_t i = 0; i < C.cv.size(); ++i)
        out << " " << i << ":" << C.cv[i];
      return out << "\n";
    }
  };
  // Checking that the elements of cv are exactly the numbers in
  // [1, ..., numcc]:
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

    friend std::ostream& operator <<(std::ostream& out, const StatsCC& s) {
      return out << s.S;
    }

  };


  /*
     The vertices reachable from vertex v=0 have cc-number 1, the
     vertices reachable from the first open vertex then cc-number 2,
     and so on:
  */
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
    assert(res.canonical());
    return res;
  }

}

#endif
