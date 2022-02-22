// Oliver Kullmann, 22.2.2022 (Swansea)
/* Copyright 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  Translating biclique-related problems to SAT

*/

#ifndef BICLIQUES2SAT_MI3iJYZoB5
#define BICLIQUES2SAT_MI3iJYZoB5

#include <limits>
#include <exception>
#include <string>

#include <cstdint>
#include <cassert>

#include "Graphs.hpp"

namespace Bicliques2SAT {

  typedef std::uint64_t var_t;
  constexpr var_t MaxN = std::numeric_limits<var_t>::max() - 1;


  struct Param {
    var_t V; // number of vertices
    var_t E; // number of edges
    var_t B; // number of bicliques

    static constexpr var_t MaxV =
      std::numeric_limits<std::uint32_t>::max() - 1;
  };
  constexpr bool valid(const Param p) noexcept {
    if (p.V > Param::MaxV) return false;
    if (p.B > p.E) return false;
    if (p.E > (p.V * (p.V - 1)) / 2) return false;
    const var_t bound = MaxN / 2 / p.B;
    if (p.V > bound / 2) return false; // see numvarbic below
    if (p.E > bound) return false; // see numvaredg below
    return true;
  }


  struct VarEncoding {
    typedef Graphs::AdjVecUInt graph_t;
    const graph_t& G;

    const var_t V, E, B; // vertices 0 <= v < V, edges are pairs of vertices,
                         // bicliques 0 <= b < B
    const var_t nb, ne; // number of variables for bicliques and edges
    const var_t n;

    explicit VarEncoding(const graph_t& G, const var_t B)
      : G(G), V(G.n()), E(G.m()), B(B),
        nb(numvarbic(V,B)), ne(numvaredg(E,B)), n(nb+ne) {
      if (G.type() != Graphs::GT::und)
        throw std::domain_error("ERROR[VarEncoding]: only undirected graphs");
      if (not valid(Param{V,E,B}))
        throw std::domain_error("ERROR[VarEncoding]: parameters"
                                " V=" + std::to_string(V) +
                                " E=" + std::to_string(E) +
                                " B=" + std::to_string(B));
      assert(nb <= MaxN and ne <= MaxN and n <= MaxN);
    }

    static constexpr var_t numvarbic(const var_t V, const var_t B) noexcept {
      return 2*V*B;
    }
    static constexpr var_t numvaredg(const var_t E, const var_t B) noexcept {
      return E*B;
    }

    typedef graph_t::id_t id_t;
    var_t left(const id_t v, const id_t b) noexcept {
      assert(v < V);
      assert(b < B);
      return 1 + b*2*V + v;
    }
    var_t right(const id_t v, const id_t b) noexcept {
      assert(v < V);
      assert(b < B);
      return 1 + b*2*V + V + v;
    }
    var_t edge(const id_t e, const id_t b) noexcept {
      assert(e < E);
      assert(b < B);
      return 1 + nb + b * E + e;
    }

  };

}

#endif
