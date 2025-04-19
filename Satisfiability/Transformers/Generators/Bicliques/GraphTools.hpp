// Oliver Kullmann, 10.4.2025 (Swansea)
/* Copyright 2025 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  Tools for using external graph-tools

    Constants:
     - default_redumis_seed : int
     - default_redumis_timeout : double


    Functor-class Redumis_call for computing large independent sets
    of a graph G (deployed in MaxIndependentRedumis.cpp):

      Data-members:
       - const reference G to graph_type = AdjVecUInt
       - int seeed, double timeout
       - string path_use_redumis (path to the wrapper for redumis)
       - string additional_options

      Via member-function
        command() -> string
      one obtains the string for the system-(like-)command.

      Constructors:
       - Redumis_call(G)
       - Redumis_call(G,seed,timeout,path,options)
       - Redumis_call(G,seed,timeout,path).
      If the path-argument is empty, then
         default_path_use_redumis = "use_redumis"
      is used.

      Operator ()() -> vertex_list
        calls use_redumis (via a pipe to standard input), and returns
        an independent set/list (empty in case of any error).

      With
        check_independence(vertex_list) -> bool
      one can check the result.


    Functor-class BC_incomp_by_redumis computes a large biclique-incompatible
    set of edge-indices for a graph G and its edge-list E:

     - Constructed with G, E, a Redumis_call-object RC is created, holding
       the biclique-compatibility-graph (stored as BG).
     - The function-call-operator then simply returns RC().

TODOS:

1. Functor class FastVC_call for computing small vertex covers

2. Functor class BC_incomp_by_redumis:
  - Having the biclique-compatability-graph stored in BG can be useful for
    inspection, but we also need a version, which does not store this graph,
    but pipes it directly to standard-input of use_redumis.

*/

#ifndef GRAPHTOOLS_UOmMwSdFsy
#define GRAPHTOOLS_UOmMwSdFsy

#include <string>
#include <sstream>
#include <utility>

#include <cassert>
#include <cstdio> // for std::FILE, std::fputs
#include <cinttypes> // for PRIu64

#include <SystemSpecifics/SystemCalls.hpp>
#include <Numerics/NumInOut.hpp>

#include "Graphs.hpp"
#include "Bicliques.hpp"

namespace GraphTools {

  constexpr int default_redumis_seed = 0;
  constexpr double default_redumis_timeout = 10;
  const std::string default_path_use_redumis = "use_redumis";


  struct Redumis_call {
    typedef Graphs::AdjVecUInt graph_type;
    typedef Graphs::AdjVecUInt::list_t vertex_list;
    typedef graph_type::id_t id_t;

    const graph_type& G;

    int seed = default_redumis_seed;
    double timeout = default_redumis_timeout;
    std::string path_use_redumis = default_path_use_redumis;
    std::string additional_options = "--disable_checks";

    std::string command() const {
      std::stringstream out;
      out << "timelimit=" << timeout << " " << path_use_redumis
          << " --seed=" << seed << " " << additional_options;
      return out.str();
    }

    Redumis_call(const graph_type& G) noexcept : G(G) {}
    Redumis_call(const graph_type& G,
                 const int seed, const double timeout,
                 std::string path, std::string options) noexcept :
    G(G), seed(seed), timeout(timeout),
    path_use_redumis(path.empty()?default_path_use_redumis:std::move(path)),
    additional_options(std::move(options)) {}
    Redumis_call(const graph_type& G,
                 const int seed, const double timeout,
                 std::string path) noexcept :
    G(G), seed(seed), timeout(timeout),
    path_use_redumis(path.empty()?default_path_use_redumis:std::move(path)) {}

    bool check_independence(const vertex_list& V) const noexcept {
      return Graphs::is_independent(V, G);
    }

    // Returning the empty list in case of error:
    vertex_list operator()() const {
      const std::string com = command();
      SystemCalls::Popen po(com);
      struct handle_format { // set and reset the graph-format
        const Graphs::AdjVecUInt& G;
        const Graphs::GrFo old;
        handle_format(const Graphs::AdjVecUInt& G) noexcept
        : G(G), old(G.format()) { G.format(Graphs::GrFo::metis); }
        ~handle_format() noexcept { G.format(old); }
      };
      const handle_format H(G);
      const auto res = po.etransfer(Graphs::AdjVecUIntref_put(G));
      if (res.rv.error() or not res.err.empty()) return {};
      vertex_list ind = FloatingPoint::to_vec_unsigned<id_t>(res.out, ' ');
      for (id_t& v : ind)
        if (v == 0 or v > G.n()) return {};
        else --v;
      return ind;
    }

  };

  struct BC_incomp_by_redumis {
    using graph_type = Redumis_call::graph_type;
    using vertex_list = Redumis_call::vertex_list;
    typedef Graphs::AdjVecUInt::vecedges_t vecedges_t;

    const graph_type BG;
    const Redumis_call RC;

    static graph_type produce_BG(const graph_type& G, const vecedges_t& E) {
      return Bicliques::bccomp_graph<Bicliques::best_version_bccomp>(G,E,"");
    }

    BC_incomp_by_redumis(const graph_type& G, const vecedges_t& E) :
      BG(produce_BG(G,E)), RC(BG) {}
    BC_incomp_by_redumis(const graph_type& G, const vecedges_t& E,
                         const int seed, const double timeout,
                         std::string path, std::string options) :
      BG(produce_BG(G,E)), RC(BG,seed,timeout,path,options) {}
    BC_incomp_by_redumis(const graph_type& G, const vecedges_t& E,
                         const int seed, const double timeout,
                         std::string path) :
      BG(produce_BG(G,E)), RC(BG,seed,timeout,path) {}

    vertex_list operator()() const { return RC(); }
  };

}

#endif
