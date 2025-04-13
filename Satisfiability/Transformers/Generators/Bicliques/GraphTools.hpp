// Oliver Kullmann, 10.4.2025 (Swansea)
/* Copyright 2025 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  Tools for using external graph-tools

TODOS:

- Functor-class Redumis_call organising, given a graph, computing an
  independent set for it.
   - DONE Constructed with AdjVecUInt.
   - DONE Predefined string for path to use_redumis (can be changed).
   - DONE Operator() -> vector of vertices (independent sets).
     Uses Popen; in case of error returns the empty vector.
   - DONE Also the seeds need to be handled.
    - redumis allows
      --seed=<int>                             Seed to use for the PRNG.
    - "int" is the C++-type.
   - DONE And possibly some options for redumis.
    - There are "config, kahip-mode, kernelization, red_thres"
    - Perhaps these are just strings, and when empty then nothing is done.
    - Simplest to collect all that into one string.
   - DONE For this internal deployment "--disable_checks" should be used.
   - DONE
     So use_redumis needs to be extended, to allow for additional arguments,
     to be passed to redumis.
   - DONE check(vertex-vector) -> bool checks whether the vector is
     independent.


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

  constexpr int default_seed = 0;
  constexpr double default_timeout = 10;

  struct Redumis_call {
    typedef Graphs::AdjVecUInt graph_type;
    typedef Graphs::AdjVecUInt::list_t vertex_list;
    typedef graph_type::id_t id_t;

    const graph_type& G;

    int seed = default_seed;
    double timeout = default_timeout;
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
