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

    Functor-class Redumis_call (deployed in MaxIndependentRedumis.cpp):

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
       - Redumis_call(G,seed,timeout,path) .

      Operator ()() -> vertex_list
        calls use_redumis (via a pipe to standard input), and returns
        an independent set/list (empty in case of any error).

      With
        check_independence(vertex_list) -> bool
      one can check the result.

TODOS:

  - Functor class BC_incomp_by_redumis:
   - Constructor by graph G : AdjVecUInt and its edge-list.
   - Plus the arguments needed for Redumis_call.
   - Operator ()() constructs the biclique-compatibility graph, and returns
     the list of indices as computed by Redumis_call for this graph.

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

namespace GraphTools {

  constexpr int default_redumis_seed = 0;
  constexpr double default_redumis_timeout = 10;

  struct Redumis_call {
    typedef Graphs::AdjVecUInt graph_type;
    typedef Graphs::AdjVecUInt::list_t vertex_list;
    typedef graph_type::id_t id_t;

    const graph_type& G;

    int seed = default_redumis_seed;
    double timeout = default_redumis_timeout;
    std::string path_use_redumis = "use_redumis";
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
    path_use_redumis(std::move(path)), additional_options(std::move(options))
    {}
    Redumis_call(const graph_type& G,
                 const int seed, const double timeout,
                 std::string path) noexcept :
    G(G), seed(seed), timeout(timeout), path_use_redumis(std::move(path)) {}

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

}

#endif
