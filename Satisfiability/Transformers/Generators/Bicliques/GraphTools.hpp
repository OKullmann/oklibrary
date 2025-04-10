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
   - Constructed with AdjVecUInt.
   - Predefined string for path to use_redumis (can be changed).
   - Operator(Unsigned) -> vector of vertices (independent sets).
     Uses Popen; in case of error returns the empty vector.
     The input is the timeout (seconds).
   - Also the seeds need to be handled.
    - redumis allows
      --seed=<int>                             Seed to use for the PRNG.
    - Unclear what "int" means.
   - And possibly some options for redumis.
    - There are "config, kahip-mode, kernelization, red_thres"
    - Perhaps these are just strings, and when empty then nothing is done.
    - Simplest to collect all that into one string.
   - For this internal deployment "--disable_checks" should be used.
   - So use_redumis needs to be extended, to allow for additional arguments,
     to be passed to redumis.
   - check(vertex-vector) -> bool checks whether the vector is independent.
     Returns also false if a vertex is invalid.


*/

#ifndef GRAPHTOOLS_UOmMwSdFsy
#define GRAPHTOOLS_UOmMwSdFsy

#include <cassert>
#include <cstdio> // for std::FILE, std::fputs
#include <cinttypes> // for PRIu64

#include <SystemSpecifics/SystemCalls.hpp>

#include "Graphs.hpp"

namespace GraphTools {

  struct Redumis_call {

  };

}

#endif
