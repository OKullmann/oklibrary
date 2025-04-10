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
   - Operator() -> vector of vertices (independent sets).
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
   - check(vertex-vector) -> bool checks whether the vector is independent.
     Returns also false if a vertex is invalid.


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

#include "Graphs.hpp"

namespace GraphTools {

  constexpr int default_seed = 0;
  constexpr double default_timeout = 10;

  struct Redumis_call {
    const Graphs::AdjVecUInt& G;

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

    Redumis_call(const Graphs::AdjVecUInt& G) noexcept : G(G) {}
    Redumis_call(const Graphs::AdjVecUInt& G,
                 const int seed, const double timeout,
                 std::string path, std::string options) noexcept :
    G(G), seed(seed), timeout(timeout),
    path_use_redumis(std::move(path)), additional_options(std::move(options))
    {}
    Redumis_call(const Graphs::AdjVecUInt& G,
                 const int seed, const double timeout,
                 std::string path) noexcept :
    G(G), seed(seed), timeout(timeout), path_use_redumis(std::move(path)) {}

  };

}

#endif
