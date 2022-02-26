// Oliver Kullmann, 26.2.2022 (Swansea)
/* Copyright 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  Graph generators

*/

#ifndef GENERATORS_1twuS7HUpM
#define GENERATORS_1twuS7HUpM

#include <string>
#include <vector>

#include <cassert>

#include "Graphs.hpp"

namespace Generators {

  typedef Graphs::AdjMapStr AdjMapStr;

  typedef AdjMapStr::size_t size_t;


  AdjMapStr clique(const size_t n) {
    AdjMapStr G(Graphs::GT::und);
    if (n == 0) return G;
    std::vector<std::string> V; V.reserve(n);
    for (size_t i = 0; i < n; ++i) V.push_back(std::to_string(i+1));
    [[maybe_unused]] const auto res = G.add_clique(V);
    assert(res.first == n and res.second == (n*(n-1)) / 2);
    return G;
  }

}

#endif
