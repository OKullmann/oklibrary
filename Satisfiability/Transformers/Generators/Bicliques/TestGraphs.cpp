// Oliver Kullmann, 20.2.2022 (Swansea)
/* Copyright 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#include <iostream>
#include <vector>
#include <sstream>

#include <ProgramOptions/Environment.hpp>

#include "Graphs.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.1.4",
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
   const auto i1 = G.insert("");
   assert(G.n() == 1);
   assert(G.m() == 0);
   assert(eqp(G.graph(), {{"",{}}}));
   assert(i1.second);
   assert(eqp(*i1.first, {"",{}}));
   const auto Gc(G);
   const auto i2 = G.insert("");
   assert(Gc == G);
   assert(not i2.second);
   assert(i2.first == i1.first);
  }

  {AdjMapStr G(GT::dir);
   assert(G.type() == GT::dir);
   assert(G.n() == 0);
   assert(G.m() == 0);
   assert(eqp(G.insert("a", "b"), {2,1}));
   assert(G.n() == 2);
   assert(G.m() == 1);
   assert(eqp(G.neighbours("a"), {"b"}));
   assert(eqp(G.neighbours("b"), {}));
   const auto Gc(G);
   assert(eqp(G.insert("a", "b"), {0,0}));
   assert(G.n() == 2);
   assert(G.m() == 1);
   assert(eqp(G.neighbours("a"), {"b"}));
   assert(eqp(G.neighbours("b"), {}));
   assert(Gc == G);
   assert(eqp(G.insert("a", "c"), {1,1}));
   assert(G.n() == 3);
   assert(G.m() == 2);
   assert(eqp(G.neighbours("a"), {"b","c"}));
   assert(eqp(G.neighbours("b"), {}));
   assert(eqp(G.neighbours("c"), {}));
   assert(Gc != G);
   assert(eqp(G.insert("b", "c"), {0,1}));
   assert(G.n() == 3);
   assert(G.m() == 3);
   assert(eqp(G.neighbours("a"), {"b","c"}));
   assert(eqp(G.neighbours("b"), {"c"}));
   assert(eqp(G.neighbours("c"), {}));
   assert(eqp(G.insert("c", "b"), {0,1}));
   assert(G.n() == 3);
   assert(G.m() == 4);
   assert(eqp(G.neighbours("a"), {"b","c"}));
   assert(eqp(G.neighbours("b"), {"c"}));
   assert(eqp(G.neighbours("c"), {"b"}));
   assert(eqp(G.insert("b", "d"), {1,1}));
   assert(G.n() == 4);
   assert(G.m() == 5);
   assert(eqp(G.neighbours("a"), {"b","c"}));
   assert(eqp(G.neighbours("b"), {"c","d"}));
   assert(eqp(G.neighbours("c"), {"b"}));
   assert(eqp(G.neighbours("d"), {}));
   assert(eqp(G.insert("e", "a"), {1,1}));
   assert(G.n() == 5);
   assert(G.m() == 6);
   assert(eqp(G.neighbours("a"), {"b","c"}));
   assert(eqp(G.neighbours("b"), {"c","d"}));
   assert(eqp(G.neighbours("c"), {"b"}));
   assert(eqp(G.neighbours("d"), {}));
   assert(eqp(G.neighbours("e"), {"a"}));
   assert(eqp(G.insert("f", "g"), {2,1}));
   assert(G.n() == 7);
   assert(G.m() == 7);
   assert(eqp(G.neighbours("a"), {"b","c"}));
   assert(eqp(G.neighbours("b"), {"c","d"}));
   assert(eqp(G.neighbours("c"), {"b"}));
   assert(eqp(G.neighbours("d"), {}));
   assert(eqp(G.neighbours("e"), {"a"}));
   assert(eqp(G.neighbours("f"), {"g"}));
   assert(eqp(G.neighbours("g"), {}));
   assert(eqp(G.insert("a", "a"), {0,1}));
   assert(G.n() == 7);
   assert(G.m() == 8);
   assert(eqp(G.neighbours("a"), {"a","b","c"}));
   assert(eqp(G.insert("a", "a"), {0,0}));
   assert(G.n() == 7);
   assert(G.m() == 8);
   assert(eqp(G.insert("h", "h"), {1,1}));
   assert(G.n() == 8);
   assert(G.m() == 9);
   assert(eqp(G.neighbours("h"), {"h"}));
  }

  {AdjMapStr G(GT::und);
   assert(G.type() == GT::und);
   assert(G.n() == 0);
   assert(G.m() == 0);
   assert(eqp(G.insert("a", "b"), {2,1}));
   assert(G.n() == 2);
   assert(G.m() == 1);
   assert(eqp(G.neighbours("a"), {"b"}));
   assert(eqp(G.neighbours("b"), {"a"}));
   const auto Gc(G);
   assert(eqp(G.insert("a", "b"), {0,0}));
   assert(G.n() == 2);
   assert(G.m() == 1);
   assert(eqp(G.neighbours("a"), {"b"}));
   assert(eqp(G.neighbours("b"), {"a"}));
   assert(Gc == G);
   assert(eqp(G.insert("b", "a"), {0,0}));
   assert(Gc == G);
   assert(eqp(G.insert("a", "c"), {1,1}));
   assert(G.n() == 3);
   assert(G.m() == 2);
   assert(eqp(G.neighbours("a"), {"b","c"}));
   assert(eqp(G.neighbours("b"), {"a"}));
   assert(eqp(G.neighbours("c"), {"a"}));
   assert(Gc != G);
   assert(eqp(G.insert("b", "c"), {0,1}));
   assert(G.n() == 3);
   assert(G.m() == 3);
   assert(eqp(G.neighbours("a"), {"b","c"}));
   assert(eqp(G.neighbours("b"), {"a","c"}));
   assert(eqp(G.neighbours("c"), {"a","b"}));
   assert(eqp(G.insert("d", "a"), {1,1}));
   assert(G.n() == 4);
   assert(G.m() == 4);
   assert(eqp(G.neighbours("a"), {"b","c","d"}));
   assert(eqp(G.neighbours("b"), {"a","c"}));
   assert(eqp(G.neighbours("c"), {"a","b"}));
   assert(eqp(G.neighbours("d"), {"a"}));
   assert(eqp(G.insert("a", "d"), {0,0}));
   assert(eqp(G.insert("e", "f"), {2,1}));
   assert(G.n() == 6);
   assert(G.m() == 5);
   assert(eqp(G.neighbours("a"), {"b","c","d"}));
   assert(eqp(G.neighbours("b"), {"a","c"}));
   assert(eqp(G.neighbours("c"), {"a","b"}));
   assert(eqp(G.neighbours("d"), {"a"}));
   assert(eqp(G.neighbours("e"), {"f"}));
   assert(eqp(G.neighbours("f"), {"e"}));
   assert(eqp(G.insert("a", "a"), {0,1}));
   assert(G.n() == 6);
   assert(G.m() == 6);
   assert(eqp(G.neighbours("a"), {"a","b","c","d"}));
   assert(eqp(G.insert("a", "a"), {0,0}));
   assert(G.n() == 6);
   assert(G.m() == 6);
   assert(eqp(G.insert("h", "h"), {1,1}));
   assert(G.n() == 7);
   assert(G.m() == 7);
   assert(eqp(G.neighbours("h"), {"h"}));
  }

  {AdjMapStr G(GT::dir);
   typedef std::vector<std::string> vvt;
   assert(eqp(G.insert("a", vvt{}), {1,0}));
   assert(G.n() == 1);
   assert(G.m() == 0);
   assert(eqp(G.insert("a", vvt{"b","b"}), {1,1}));
   assert(G.n() == 2);
   assert(G.m() == 1);
   assert(eqp(G.insert("a", vvt{"b","b"}), {0,0}));
   assert(G.n() == 2);
   assert(G.m() == 1);
   assert(eqp(G.insert("b", vvt{"a","c","a"}), {1,2}));
   assert(G.n() == 3);
   assert(G.m() == 3);
   assert(eqp(G.graph(), {{"a",{"b"}},{"b",{"a","c"}},{"c",{}}}));
   assert(eqp(G.insert("a", vvt{"a","b","c"}), {0,2}));
   assert(G.n() == 3);
   assert(G.m() == 5);
   assert(eqp(G.graph(), {{"a",{"a","b","c"}},{"b",{"a","c"}},{"c",{}}}));
  }

  {AdjMapStr G(GT::und);
   typedef std::vector<std::string> vvt;
   assert(eqp(G.insert("a", vvt{}), {1,0}));
   assert(G.n() == 1);
   assert(G.m() == 0);
   assert(eqp(G.insert("a", vvt{"b","b"}), {1,1}));
   assert(G.n() == 2);
   assert(G.m() == 1);
   assert(eqp(G.insert("a", vvt{"b","b"}), {0,0}));
   assert(G.n() == 2);
   assert(G.m() == 1);
   assert(eqp(G.insert("b", {"a","c","a"}), {1,1}));
   assert(G.n() == 3);
   assert(G.m() == 2);
   assert(eqp(G.graph(), {{"a",{"b"}},{"b",{"a","c"}},{"c",{"b"}}}));
   assert(eqp(G.insert("a", {"a","b","c"}), {0,2}));
   assert(G.n() == 3);
   assert(G.m() == 4);
   assert(eqp(G.graph(), {{"a",{"a","b","c"}},{"b",{"a","c"}},{"c",{"a","b"}}}));
  }

  {AdjMapStr G(GT::dir);
   typedef AdjMapStr::idv_t vvt;
   assert(eqp(G.insert("a", vvt{"b"}), {2,1}));
   assert(G.n() == 2);
   assert(G.m() == 1);
   assert(eqp(G.neighbours("a"), {"b"}));
   assert(eqp(G.neighbours("b"), {}));
   const auto Gc(G);
   assert(eqp(G.insert("a", vvt{"b"}), {0,0}));
   assert(G.n() == 2);
   assert(G.m() == 1);
   assert(eqp(G.neighbours("a"), {"b"}));
   assert(eqp(G.neighbours("b"), {}));
   assert(Gc == G);
   assert(eqp(G.insert("a", vvt{"c"}), {1,1}));
   assert(G.n() == 3);
   assert(G.m() == 2);
   assert(eqp(G.neighbours("a"), {"b","c"}));
   assert(eqp(G.neighbours("b"), {}));
   assert(eqp(G.neighbours("c"), {}));
   assert(Gc != G);
   assert(eqp(G.insert("b", vvt{"c"}), {0,1}));
   assert(G.n() == 3);
   assert(G.m() == 3);
   assert(eqp(G.neighbours("a"), {"b","c"}));
   assert(eqp(G.neighbours("b"), {"c"}));
   assert(eqp(G.neighbours("c"), {}));
   assert(eqp(G.insert("c", vvt{"b"}), {0,1}));
   assert(G.n() == 3);
   assert(G.m() == 4);
   assert(eqp(G.neighbours("a"), {"b","c"}));
   assert(eqp(G.neighbours("b"), {"c"}));
   assert(eqp(G.neighbours("c"), {"b"}));
   assert(eqp(G.insert("b", vvt{"d"}), {1,1}));
   assert(G.n() == 4);
   assert(G.m() == 5);
   assert(eqp(G.neighbours("a"), {"b","c"}));
   assert(eqp(G.neighbours("b"), {"c","d"}));
   assert(eqp(G.neighbours("c"), {"b"}));
   assert(eqp(G.neighbours("d"), {}));
   assert(eqp(G.insert("e", vvt{"a"}), {1,1}));
   assert(G.n() == 5);
   assert(G.m() == 6);
   assert(eqp(G.neighbours("a"), {"b","c"}));
   assert(eqp(G.neighbours("b"), {"c","d"}));
   assert(eqp(G.neighbours("c"), {"b"}));
   assert(eqp(G.neighbours("d"), {}));
   assert(eqp(G.neighbours("e"), {"a"}));
   assert(eqp(G.insert("f", vvt{"g"}), {2,1}));
   assert(G.n() == 7);
   assert(G.m() == 7);
   assert(eqp(G.neighbours("a"), {"b","c"}));
   assert(eqp(G.neighbours("b"), {"c","d"}));
   assert(eqp(G.neighbours("c"), {"b"}));
   assert(eqp(G.neighbours("d"), {}));
   assert(eqp(G.neighbours("e"), {"a"}));
   assert(eqp(G.neighbours("f"), {"g"}));
   assert(eqp(G.neighbours("g"), {}));
   assert(eqp(G.insert("a", vvt{"a"}), {0,1}));
   assert(G.n() == 7);
   assert(G.m() == 8);
   assert(eqp(G.neighbours("a"), {"a","b","c"}));
   assert(eqp(G.insert("a", vvt{"a"}), {0,0}));
   assert(G.n() == 7);
   assert(G.m() == 8);
   assert(eqp(G.insert("h", "h"), {1,1}));
   assert(G.n() == 8);
   assert(G.m() == 9);
   assert(eqp(G.neighbours("h"), {"h"}));
  }

  {AdjMapStr G(GT::und);
   typedef AdjMapStr::idv_t vvt;
   assert(eqp(G.insert("a", vvt{"b"}), {2,1}));
   assert(G.n() == 2);
   assert(G.m() == 1);
   assert(eqp(G.neighbours("a"), {"b"}));
   assert(eqp(G.neighbours("b"), {"a"}));
   const auto Gc(G);
   assert(eqp(G.insert("a", vvt{"b"}), {0,0}));
   assert(G.n() == 2);
   assert(G.m() == 1);
   assert(eqp(G.neighbours("a"), {"b"}));
   assert(eqp(G.neighbours("b"), {"a"}));
   assert(Gc == G);
   assert(eqp(G.insert("b", vvt{"a"}), {0,0}));
   assert(Gc == G);
   assert(eqp(G.insert("a", vvt{"c"}), {1,1}));
   assert(G.n() == 3);
   assert(G.m() == 2);
   assert(eqp(G.neighbours("a"), {"b","c"}));
   assert(eqp(G.neighbours("b"), {"a"}));
   assert(eqp(G.neighbours("c"), {"a"}));
   assert(Gc != G);
   assert(eqp(G.insert("b", vvt{"c"}), {0,1}));
   assert(G.n() == 3);
   assert(G.m() == 3);
   assert(eqp(G.neighbours("a"), {"b","c"}));
   assert(eqp(G.neighbours("b"), {"a","c"}));
   assert(eqp(G.neighbours("c"), {"a","b"}));
   assert(eqp(G.insert("d", vvt{"a"}), {1,1}));
   assert(G.n() == 4);
   assert(G.m() == 4);
   assert(eqp(G.neighbours("a"), {"b","c","d"}));
   assert(eqp(G.neighbours("b"), {"a","c"}));
   assert(eqp(G.neighbours("c"), {"a","b"}));
   assert(eqp(G.neighbours("d"), {"a"}));
   assert(eqp(G.insert("a", vvt{"d"}), {0,0}));
   assert(eqp(G.insert("e", vvt{"f"}), {2,1}));
   assert(G.n() == 6);
   assert(G.m() == 5);
   assert(eqp(G.neighbours("a"), {"b","c","d"}));
   assert(eqp(G.neighbours("b"), {"a","c"}));
   assert(eqp(G.neighbours("c"), {"a","b"}));
   assert(eqp(G.neighbours("d"), {"a"}));
   assert(eqp(G.neighbours("e"), {"f"}));
   assert(eqp(G.neighbours("f"), {"e"}));
   assert(eqp(G.insert("a", vvt{"a"}), {0,1}));
   assert(G.n() == 6);
   assert(G.m() == 6);
   assert(eqp(G.neighbours("a"), {"a","b","c","d"}));
   assert(eqp(G.insert("a", vvt{"a"}), {0,0}));
   assert(G.n() == 6);
   assert(G.m() == 6);
   assert(eqp(G.insert("h", vvt{"h"}), {1,1}));
   assert(G.n() == 7);
   assert(G.m() == 7);
   assert(eqp(G.neighbours("h"), {"h"}));
  }

  {AdjMapStr G(GT::dir);
   std::stringstream out;
   out << G;
   assert(out.str() == std::string("# 0 0 0\n"));
   out.str("");
   G.insert("a", {"a", "c", "d"});
   out << G;
   assert(out.str() == std::string("# 3 3 0\n"
     "a a c d\n"
     "c\n"
     "d\n"
                                   ));
   out.str("");
   G.insert("d", AdjMapStr::idv_t{"c", "e"});
   out << G;
   assert(out.str() == std::string("# 4 5 0\n"
     "a a c d\n"
     "c\n"
     "d c e\n"
     "e\n"
                                   ));
  }

}
