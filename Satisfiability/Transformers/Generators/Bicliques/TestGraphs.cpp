// Oliver Kullmann, 20.2.2022 (Swansea)
/* Copyright 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#include <iostream>

#include <ProgramOptions/Environment.hpp>

#include "Graphs.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.1.1",
        "21.2.2022",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Bicliques/TestGraphs.cpp",
        "GPL v3"};

  using namespace Graphs;

  template <class X>
  constexpr bool eqp(const X& lhs, const X& rhs) noexcept {
    return lhs == rhs;
  }
}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv))
  return 0;

  {AdjMapStr G(GT::dir);
   assert(G.type() == GT::dir);
   assert(G.n() == 0);
   assert(G.m() == 0);
   assert(G.insert("a", "b"));
   assert(G.n() == 2);
   assert(G.m() == 1);
   assert(eqp(G.neighbours("a"), {"b"}));
   assert(eqp(G.neighbours("b"), {}));
   const auto Gc(G);
   assert(not G.insert("a", "b"));
   assert(G.n() == 2);
   assert(G.m() == 1);
   assert(eqp(G.neighbours("a"), {"b"}));
   assert(eqp(G.neighbours("b"), {}));
   assert(Gc == G);
   assert(G.insert("a", "c"));
   assert(G.n() == 3);
   assert(G.m() == 2);
   assert(eqp(G.neighbours("a"), {"b","c"}));
   assert(eqp(G.neighbours("b"), {}));
   assert(eqp(G.neighbours("c"), {}));
   assert(Gc != G);
   assert(G.insert("b", "c"));
   assert(G.n() == 3);
   assert(G.m() == 3);
   assert(eqp(G.neighbours("a"), {"b","c"}));
   assert(eqp(G.neighbours("b"), {"c"}));
   assert(eqp(G.neighbours("c"), {}));
   assert(G.insert("c", "b"));
   assert(G.n() == 3);
   assert(G.m() == 4);
   assert(eqp(G.neighbours("a"), {"b","c"}));
   assert(eqp(G.neighbours("b"), {"c"}));
   assert(eqp(G.neighbours("c"), {"b"}));
   assert(G.insert("b", "d"));
   assert(G.n() == 4);
   assert(G.m() == 5);
   assert(eqp(G.neighbours("a"), {"b","c"}));
   assert(eqp(G.neighbours("b"), {"c","d"}));
   assert(eqp(G.neighbours("c"), {"b"}));
   assert(eqp(G.neighbours("d"), {}));
   assert(G.insert("e", "a"));
   assert(G.n() == 5);
   assert(G.m() == 6);
   assert(eqp(G.neighbours("a"), {"b","c"}));
   assert(eqp(G.neighbours("b"), {"c","d"}));
   assert(eqp(G.neighbours("c"), {"b"}));
   assert(eqp(G.neighbours("d"), {}));
   assert(eqp(G.neighbours("e"), {"a"}));
   assert(G.insert("f", "g"));
   assert(G.n() == 7);
   assert(G.m() == 7);
   assert(eqp(G.neighbours("a"), {"b","c"}));
   assert(eqp(G.neighbours("b"), {"c","d"}));
   assert(eqp(G.neighbours("c"), {"b"}));
   assert(eqp(G.neighbours("d"), {}));
   assert(eqp(G.neighbours("e"), {"a"}));
   assert(eqp(G.neighbours("f"), {"g"}));
   assert(eqp(G.neighbours("g"), {}));
  }

  {AdjMapStr G(GT::und);
   assert(G.type() == GT::und);
   assert(G.n() == 0);
   assert(G.m() == 0);
   assert(G.insert("a", "b"));
   assert(G.n() == 2);
   assert(G.m() == 1);
   assert(eqp(G.neighbours("a"), {"b"}));
   assert(eqp(G.neighbours("b"), {"a"}));
   const auto Gc(G);
   assert(not G.insert("a", "b"));
   assert(G.n() == 2);
   assert(G.m() == 1);
   assert(eqp(G.neighbours("a"), {"b"}));
   assert(eqp(G.neighbours("b"), {"a"}));
   assert(Gc == G);
   assert(not G.insert("b", "a"));
   assert(Gc == G);
   assert(G.insert("a", "c"));
   assert(G.n() == 3);
   assert(G.m() == 2);
   assert(eqp(G.neighbours("a"), {"b","c"}));
   assert(eqp(G.neighbours("b"), {"a"}));
   assert(eqp(G.neighbours("c"), {"a"}));
   assert(Gc != G);
   assert(G.insert("b", "c"));
   assert(G.n() == 3);
   assert(G.m() == 3);
   assert(eqp(G.neighbours("a"), {"b","c"}));
   assert(eqp(G.neighbours("b"), {"a","c"}));
   assert(eqp(G.neighbours("c"), {"a","b"}));
   assert(G.insert("d", "a"));
   assert(G.n() == 4);
   assert(G.m() == 4);
   assert(eqp(G.neighbours("a"), {"b","c","d"}));
   assert(eqp(G.neighbours("b"), {"a","c"}));
   assert(eqp(G.neighbours("c"), {"a","b"}));
   assert(eqp(G.neighbours("d"), {"a"}));
   assert(not G.insert("a", "d"));
   assert(G.insert("e", "f"));
   assert(G.n() == 6);
   assert(G.m() == 5);
   assert(eqp(G.neighbours("a"), {"b","c","d"}));
   assert(eqp(G.neighbours("b"), {"a","c"}));
   assert(eqp(G.neighbours("c"), {"a","b"}));
   assert(eqp(G.neighbours("d"), {"a"}));
   assert(eqp(G.neighbours("e"), {"f"}));
   assert(eqp(G.neighbours("f"), {"e"}));
  }
}
