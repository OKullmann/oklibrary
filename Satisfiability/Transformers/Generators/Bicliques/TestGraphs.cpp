// Oliver Kullmann, 20.2.2022 (Swansea)
/* Copyright 2022, 2023, 2025 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#include <iostream>
#include <vector>
#include <sstream>
#include <ranges>

#include <cassert>

#include <ProgramOptions/Environment.hpp>
#include <Numerics/Statistics.hpp>

#include "TestTools.hpp"
#include "Graphs.hpp"
#include "Generators.hpp"
#include "RandomGraphs.hpp"
#include "Algorithms.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.5.5",
        "20.5.2025",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Bicliques/TestGraphs.cpp",
        "GPL v3"};

  using namespace TestTools;
  using namespace Graphs;
  namespace FP = FloatingPoint;

  void test_edgefunctions(const AdjVecUInt& G) {
    assert(G.valid(G.graph()));
    const bool loops = has_loops(G);
    const auto E = G.alledges();
    assert(E.size() == G.m());
    assert(std::ranges::is_sorted(E));
    assert(std::ranges::adjacent_find(E) == E.end());
    using size_t = AdjVecUInt::size_t;
    if (G.type() == GT::und) {
      size_t sum = 0;
      for (size_t v = 0; v < G.n(); ++v) sum += G.proper_size(v);
      assert(G.m() == sum);
    }
    for (size_t i = 0; i < E.size(); ++i) {
      const auto& e = E[i];
      const auto [v,w] = e;
      assert(v < G.n()); assert(w < G.n());
      if (G.type() == GT::und)
        if (loops) assert(v <= w); else assert(v < w);
      assert(G.adjacent(v,w));
      if (G.index2edge(i) != e) {
        std::cerr << G;
        std::cerr << "i=" << i << "; E:\n";
        Environment::out_pairs(std::cerr, E);
        std::cerr << "e="; Environment::out_pair(std::cerr, e);
        std::cerr << "\nindex2edge="; Environment::out_pair(std::cerr, G.index2edge(i));
        std::cerr << "\n";
      }
      assert(G.index2edge(i) == e);
      assert(G.edge2index(e) == i);
    }
    {struct PrEd {
       const AdjVecUInt::vecedges_t& E;
       size_t i = 0;
       void operator()(const AdjVecUInt::edge_t& e) noexcept {
         assert(E[i++] == e);
       }
     };
     PrEd pe{E};
     G.process_alledges(pe);
     assert(pe.i == G.m());
    }
    {struct PrEd2 {
       const AdjVecUInt::vecedges_t& E;
       const size_t size;
       size_t i = 0;
       void operator()(const AdjVecUInt::edge_t& e) noexcept {
         assert(E[i++] == e);
       }
       bool abort() const noexcept { return i >= size; }
     };
     for (size_t size = 0; size <= G.m(); ++size) {
       PrEd2 pe2{E,size};
       G.process_alledges(pe2);
       assert(pe2.i == size);
     }
    }
    assert(G == make_AdjVecUInt(G.type(), G.n(), G.alledges(), true));
    for (const bool b : {false, true}) {
      const AdjVecUInt K = complement(G, b);
      assert(K.alledges() == G.allnonedges(b));
    }
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
   using edvec_t = AdjMapStr::edvec_t;
   assert(G.remove_edges(edvec_t{{"a","d"}}) == 0);
   assert(G.remove_edges(edvec_t{{"a","b"},{"b","c"}}) == 2);
   assert(eqp(G.graph(), {{"a",{"a","c"}},{"b",{"a"}},{"c",{}}}));
   typedef std::vector<std::string> vvt;
   assert(eqp(G.add_biclique(vvt{"a","b"}, vvt{"a","c"}), {0,1}));
   assert(eqp(G.add_biclique(vvt{}, vvt{"a","c"}), {0,0}));
   assert(eqp(G.add_biclique(vvt{"c","d"}, vvt{"c","d"}), {1,4}));
   assert(eqp(G.graph(), {{"a",{"a","c"}},{"b",{"a","c"}},{"c",{"c","d"}},{"d",{"c","d"}}}));
   assert(eqp(G.add_path(vvt{}), {0,0}));
   assert(eqp(G.add_path(vvt{"a"}), {0,0}));
   assert(eqp(G.add_path(vvt{"e"}), {1,0}));
   assert(eqp(G.graph(), {{"a",{"a","c"}},{"b",{"a","c"}},{"c",{"c","d"}},{"d",{"c","d"}},{"e",{}}}));
   assert(eqp(G.add_path(vvt{"e","a","b","c"}), {0,2}));
   assert(eqp(G.graph(), {{"a",{"a","b","c"}},{"b",{"a","c"}},{"c",{"c","d"}},{"d",{"c","d"}},{"e",{"a"}}}));
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
   using edvec_t = AdjMapStr::edvec_t;
   assert(G.remove_edges(edvec_t{{"a","d"}}) == 0);
   assert(G.remove_edges(edvec_t{{"a","b"},{"b","c"}}) == 2);
   assert(eqp(G.graph(), {{"a",{"a","c"}},{"b",{}},{"c",{"a"}}}));
   typedef AdjMapStr::idvec_t vvt;
   assert(eqp(G.add_biclique(vvt{"a","b"}, vvt{"a","c"}), {0,2}));
   assert(eqp(G.add_biclique(vvt{}, vvt{"a","c"}), {0,0}));
   assert(eqp(G.add_biclique(vvt{"c","d"}, vvt{"c","d"}), {1,3}));
   assert(eqp(G.graph(), {{"a",{"a","b","c"}},{"b",{"a","c"}},{"c",{"a","b","c","d"}},{"d",{"c","d"}}}));   
   assert(eqp(G.add_path(vvt{}), {0,0}));
   assert(eqp(G.add_path(vvt{"a"}), {0,0}));
   assert(eqp(G.add_path(vvt{"e"}), {1,0}));
   assert(eqp(G.graph(), {{"a",{"a","b","c"}},{"b",{"a","c"}},{"c",{"a","b","c","d"}},{"d",{"c","d"}},{"e",{}}})); 
   assert(eqp(G.add_path(vvt{"e","a","b","c","e"}), {0,2}));
   assert(eqp(G.graph(), {{"a",{"a","b","c","e"}},{"b",{"a","c"}},{"c",{"a","b","c","d","e"}},{"d",{"c","d"}},{"e",{"a","c"}}}));
  }

  {AdjMapStr G(GT::dir);
   typedef AdjMapStr::idvec_t vvt;
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
   typedef AdjMapStr::idvec_t vvt;
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
   G.insert("d", AdjMapStr::idvec_t{"c", "e"});
   out << G;
   assert(out.str() == std::string("# 4 5 0\n"
     "a a c d\n"
     "c\n"
     "d c e\n"
     "e\n"
                                   ));
  }

  {AdjMapStr G(GT::dir);
   std::stringstream in;
   in << "# \n # \n   \t \n \n a b c # \n \t\t b a d\n#\n\n x # b";
   assert(eqp(G.insert(in), {5,4}));
   assert(eqp(G.graph(), {{"a",{"b","c"}},{"b",{"a","d"}},{"c",{}},{"d",{}},{"x",{}}}));
   in.clear();
   in << "  x a # b \n# \ny x";
   assert(eqp(G.insert(in), {1,2}));
   assert(eqp(G.neighbours("x"), {"a"}));
   assert(eqp(G.neighbours("y"), {"x"}));
   in.clear();
   in << "a x a\nb y b\n";
   assert(eqp(G.insert(in), {0,4}));
   assert(eqp(G.neighbours("a"), {"a","b","c","x"}));
   assert(eqp(G.neighbours("b"), {"a","d","b","y"}));

   AdjVecUInt G2(G); // vertices a, b, c, d, x, y
   assert(G2.n() == 6);
   assert(G2.m() == 10);
   assert(G2.loops() == 2);
   assert(G2.with_names());
   assert(eqp(G2.alledges(), {{0,0},{0,1},{0,2},{0,4},{1,0},{1,1},{1,3},{1,5},{4,0},{5,4}}));
   assert(eqp(G2.allnonedges(), {{0,3},{0,5}, {1,2},{1,4}, {2,0},{2,1},{2,3},{2,4},{2,5}, {3,0},{3,1},{3,2},{3,4},{3,5}, {4,1},{4,2},{4,3},{4,5}, {5,0},{5,1},{5,2},{5,3}}));
   assert(eqp(G2.allnonedges(true), {{0,3},{0,5}, {1,2},{1,4}, {2,0},{2,1},{2,2},{2,3},{2,4},{2,5}, {3,0},{3,1},{3,2},{3,3},{3,4},{3,5}, {4,1},{4,2},{4,3},{4,4},{4,5}, {5,0},{5,1},{5,2},{5,3},{5,5}}));
   for (const auto& e : G2.alledges())
     assert(G2.adjacent(e.first, e.second));
   assert(not G2.adjacent(0,3));
   assert(not G2.adjacent(4,4));
   assert(has_loops(G2));

   G2.set({{0,3},{0,5},{},{1,2},{1,4},{}});
   assert(G2.n() == 6);
   assert(G2.m() == 8);
   assert(eqp(G2.alledges(), {{0,0},{0,3},{1,0},{1,5},{3,1},{3,2},{4,1},{4,4}}));
   assert(G2.valid(G2.graph()));
   assert(not G2.valid({}));
   assert(G2.valid(AdjVecUInt::adjlist_t(6)));
   std::stringstream out;
   out << G2;
   assert(G2.with_names());
   assert(out.str() ==
          "# 6 8 0\n"
          "a a d\n"
          "b a y\n"
          "c\n"
          "d b c\n"
          "x b x\n"
          "y\n");
   G2.unset_names(); assert(not G2.with_names());
   out.str("");
   out << G2;
   assert(out.str() ==
          "# 6 8 0\n"
          "0 0 3\n"
          "1 0 5\n"
          "2\n"
          "3 1 2\n"
          "4 1 4\n"
          "5\n");
   G2.set_names(); assert(G2.with_names());
   {const auto S = degree_statistics(G2);
    assert(S.num_inputs() == 6);
    assert(S.num_values() == 2);
    assert(eqp(S.cmap(), {{0,2},{2,4}}));
   }
  }

  {AdjMapStr G(GT::und);
   std::stringstream in;
   in << "a b c d\nb d e\nf";
   assert(eqp(G.insert(in), {6,5}));
   AdjVecUInt G2(G);
   assert(G2.type() == GT::und);
   assert(G2.n() == 6);
   assert(G2.m() == 5);
   assert(G2.loops() == 0);
   assert(eqp(G2.neighbours(0), {1,2,3})); // a
   assert(eqp(G2.neighbours(1), {0,3,4})); // b
   assert(eqp(G2.neighbours(2), {0})); // c
   assert(eqp(G2.neighbours(3), {0,1})); // d
   assert(eqp(G2.neighbours(4), {1})); // e
   assert(eqp(G2.neighbours(5), {})); // f
   assert(eqp(G2.graph(), {{1,2,3},{0,3,4},{0},{0,1},{1},{}}));
   {const auto S = degree_statistics(G2);
    assert(S.num_inputs() == 6);
    assert(S.num_values() == 4);
    assert(eqp(S.cmap(), {{0,1},{1,2},{2,1},{3,2}}));
   }
   assert(G2.name(0) == std::string("a"));
   assert(G2.name(1) == std::string("b"));
   assert(G2.name(2) == std::string("c"));
   assert(G2.name(3) == std::string("d"));
   assert(G2.name(4) == std::string("e"));
   assert(G2.name(5) == std::string("f"));
   assert(eqp(G2.allnames(), {"a","b","c","d","e","f"}));
   assert(G2.index("a") == 0);
   assert(G2.index("b") == 1);
   assert(G2.index("c") == 2);
   assert(G2.index("d") == 3);
   assert(G2.index("e") == 4);
   assert(G2.index("f") == 5);
   assert(G2.index("") == 6);
   assert(eqp(G2.allindices(),
              {{"a",0},{"b",1},{"c",2},{"d",3},{"e",4},{"f",5}}));
   G2.set_name(2, "XXX");
   assert(eqp(G2.allnames(), {"a","b","XXX","d","e","f"}));
   assert(eqp(G2.allindices(),
              {{"XXX",2},{"a",0},{"b",1},{"d",3},{"e",4},{"f",5}}));
   assert(eqp(G2.alledges(), {{0,1},{0,2},{0,3},{1,3},{1,4}}));
   assert(eqp(G2.allnonedges(),
              {{0,4},{0,5},{1,2},{1,5},{2,3},{2,4},{2,5},{3,4},{3,5},{4,5}}));
   assert(eqp(G2.allnonedges(true), {{0,0},{0,4},{0,5},{1,1},{1,2},{1,5},{2,2},{2,3},{2,4},{2,5},{3,3},{3,4},{3,5},{4,4},{4,5},{5,5}}));
   for (const auto& e : G2.alledges()) {
     assert(G2.adjacent(e.first, e.second));
     assert(G2.adjacent(e.second, e.first));
   }
   assert(not G2.adjacent(0,4));
   assert(not has_loops(G2));
   assert(G2.with_names());
   G2.set({{0,2},{3,4},{0},{1},{1},{}});
   assert(G2.with_names());
   assert(G2.n() == 6);
   assert(G2.m() == 4);
   assert(has_loops(G2));
  }

  {AdjMapStr G(GT::und);
   assert(eqp(G.add_clique(std::vector{"a", "b", "c"}), {3,3}));
   assert(eqp(G.add_clique(std::vector{"a", "b", "c"}), {0,0}));
   assert(eqp(G.add_clique(std::vector{"d", "e", "f"}), {3,3}));
  }

  {using at = AdjVecUInt::adjlist_t;
   using id_t = AdjVecUInt::id_t;
   typedef std::vector<id_t> vid;
   at A;
   add_biclique(A, GT::und, vid{}, vid{});
   assert(A == at{});
   A = at(10);
   add_biclique(A, GT::dir, vid{0,1,4}, vid{0,2,3});
   assert(eqp(A[0], {0,2,3}));
   assert(eqp(A[1], {0,2,3}));
   assert(eqp(A[2], {}));
   assert(eqp(A[3], {}));
   assert(eqp(A[4], {0,2,3}));
   add_biclique(A, GT::und, vid{0,1,4}, vid{0,2,3});
   assert(eqp(A[0], {0,2,3,0,2,3,0,1,4}));
   assert(eqp(A[1], {0,2,3,0,2,3}));
   assert(eqp(A[2], {0,1,4}));
   assert(eqp(A[3], {0,1,4}));
   assert(eqp(A[4], {0,2,3,0,2,3}));
   assert(eqp(A[5], {}));
  }

  {const auto G0 = AdjVecUInt(Generators::grid(5,6));
   AdjVecUInt G1{G0.type(), G0.graph()};
   assert(G1 == G0);
   assert(G0.with_names());
   assert(G0.name(0) == "1,1");
   assert(not G1.with_names());
   G1.set_names();
   assert(G1.with_names());
   assert(G1.allnames().size() == 30);
   assert(G1.allindices().empty());
   G1.set_name(0, "XXX");
   assert(G1.name(0) == "XXX");
   assert(eqp(G1.allindices(), {{"XXX",0}}));
  }

  {for (const GT t : {GT::dir, GT::und})
     for (const bool with_loops : {false, true})
       for (AdjVecUInt::id_t n = 0; n <= 6; ++n) {
         const auto G = make_complete_AdjVecUInt(t,with_loops,n);
         assert(is_complete(G));
         assert(AdjVecUInt(G.type(), G.graph()) == G);
       }
  }

  {for (unsigned n = 0; n < 6; ++n) {
      const AdjVecUInt G(GT::und, n);
      assert(is_independent(std::ranges::iota_view(0u, n), G));
      assert(is_vertexcover(AdjVecUInt::list_t{}, G));
      assert(is_vertexcover(std::ranges::iota_view(0u, n), G));
    }
  }
  {for (unsigned n = 0; n < 6; ++n) {
     const auto G = make_complete_AdjVecUInt(GT::und, false, n);
     assert(is_independent(std::ranges::iota_view(0u, 0u), G));
     for (unsigned i = 0; i < n; ++i)
       assert(is_independent(std::ranges::iota_view(i, i+1), G));
     if (n == 0) continue;
     for (unsigned i = 0; i < n-1; ++i)
       assert(not is_independent(std::ranges::iota_view(i, i+2), G));
     using Set = std::set<unsigned>;
     const Set V0 = G.vcon<Set>();
     assert(is_vertexcover(V0, G));
     for (unsigned i = 0; i < n; ++i) {
       const Set V([&V0,i]{Set V(V0); V.erase(i); return V;}());
       assert(is_vertexcover(V, G));
       for (const unsigned j : V) {
         const Set V2([&V,j]{Set V2(V); V2.erase(j); return V2;}());
         assert(not is_vertexcover(V2, G));
       }
     }
   }
  }
  {for (unsigned n = 0; n < 6; ++n) {
     const auto G = make_complete_AdjVecUInt(GT::und, true, n);
     assert(is_independent(std::ranges::iota_view(0u, 0u), G));
     assert(is_vertexcover(G.vertex_range, G));
     // see GCC 11.4 ERROR above:
     {
       using Set = std::set<unsigned>;
       const Set V0 = G.vcon<Set>();
       assert(is_vertexcover(V0, G));
       for (unsigned i = 0; i < n; ++i) {
         const Set V([&V0,i]{Set V(V0); V.erase(i); return V;}());
         assert(not is_vertexcover(V, G));
       }
     }
     for (unsigned i = 0; i < n; ++i)
       assert(not is_independent(std::ranges::iota_view(i, i+1), G));
     if (n == 0) continue;
     for (unsigned i = 0; i < n-1; ++i)
       assert(not is_independent(std::ranges::iota_view(i, i+2), G));
   }
  }
  {for (unsigned n = 0; n < 6; ++n) {
     const auto G = AdjVecUInt(Generators::biclique(n,n));
     const auto L = std::ranges::iota_view(0u, n);
     const auto R = std::ranges::iota_view(n, 2*n);
     assert(is_independent(L, G));
     assert(is_vertexcover(L, G));
     assert(is_independent(R, G));
     assert(is_vertexcover(R, G));
     if (n == 0) continue;
     assert(not is_independent(std::ranges::iota_view(n-1, n+1), G));
   }
  }

  {for (unsigned n = 0; n < 6; ++n) {
     const AdjVecUInt G(GT::und, n);
     auto res = maximal_independent_greedy_simplest(G, {});
     std::ranges::sort(res);
     assert(std::ranges::equal(res, G.vertex_range));
   }
  }
  {const  AdjVecUInt G(Generators::clique(10));
   GenStats::FreqStats<unsigned, FP::float80> S;
   for (unsigned i = 0; i < 1000; ++i) {
     const auto res = maximal_independent_greedy_simplest(G, {i});
     assert(res.size() == 1);
     S += res[0];
   }
   assert(eqp(S.cmap(),
              {{0,91},{1,99},{2,106},{3,98},{4,101},{5,114},
               {6,116},{7,103},{8,86},{9,86}}));
  }
  {for (unsigned n = 0; n < 11; ++n) {
     const AdjVecUInt G(Generators::grid(n,1));
     auto res = maximal_independent_greedy_simplest(G, {});
     assert(res.size() == (n+1)/2);
   }
  }
  {for (unsigned n = 0; n < 11; ++n) {
     const AdjVecUInt G(Generators::biclique(n,n));
     auto res = maximal_independent_greedy_simplest(G, {});
     assert(res.size() == n);
   }
  }
  {const AdjVecUInt G(Generators::grid(5,1));
   std::vector<std::vector<unsigned>> freq_stats(3, std::vector<unsigned>(5));
   for (unsigned T = 0; T < 1000; ++T) {
     const auto I = maximal_independent_greedy_simplest(G, {T});
     assert(I.size() == 3);
     for (unsigned i = 0; i < 3; ++i) ++freq_stats[i][I[i]];
   }
   assert(eqp(freq_stats[0], {495,0,0,0,505}));
   assert(eqp(freq_stats[1], {263,0,484,0,253}));
   assert(eqp(freq_stats[2], {242,0,516,0,242}));
  }

  {const AdjVecUInt G(Generators::clique(0));
   std::ostringstream os;
   output_matrix(G, os);
   assert(os.good());
   assert(os.str() == "0 0\n\n");
  }
  {const AdjVecUInt G(Generators::clique(1));
   std::ostringstream os;
   output_matrix(G, os);
   assert(os.good());
   assert(os.str() == "1 1\n\n0\n");
  }
  {const AdjVecUInt G(Generators::clique(2));
   std::ostringstream os;
   output_matrix(G, os);
   assert(os.good());
   assert(os.str() == "2 2\n\n0 1\n1 0\n");
  }
  {const AdjVecUInt G(Generators::clique(3));
   std::ostringstream os;
   output_matrix(G, os);
   assert(os.good());
   assert(os.str() == "3 3\n\n0 1 1\n1 0 1\n1 1 0\n");
  }
  {const AdjVecUInt G(GT::und, 2);
   std::ostringstream os;
   output_matrix(G, os);
   assert(os.good());
   assert(os.str() == "2 2\n\n0 0\n0 0\n");
  }
  {const AdjVecUInt G(GT::und, 3);
   std::ostringstream os;
   output_matrix(G, os);
   assert(os.good());
   assert(os.str() == "3 3\n\n0 0 0\n0 0 0\n0 0 0\n");
  }
  {const AdjVecUInt G(Generators::grid(1,3));
   std::ostringstream os;
   output_matrix(G, os);
   assert(os.good());
   assert(os.str() == "3 3\n\n0 1 0\n1 0 1\n0 1 0\n");
  }
  {const AdjVecUInt G(Generators::cycle(3));
   std::ostringstream os;
   output_matrix(G, os);
   assert(os.good());
   assert(os.str() == "3 3\n\n0 1 1\n1 0 1\n1 1 0\n");
  }

  {using v_t = AdjVecUInt::vecedges_t;
   using r_t = AdjVecUInt::list_t;
   assert(edge_index(v_t{}, v_t{}) == r_t{});
   assert(edge_index(v_t{{}}, v_t{}) == r_t{});
   assert(edge_index(v_t{{}}, v_t{{}}) == r_t{0});
   assert((edge_index(v_t{{},{0,1},{0,2},{1,3}}, v_t{{},{0,2}}) == r_t{0,2}));
   assert((edge_index(v_t{{},{0,1},{0,2},{1,3}}, v_t{{},{0,1},{0,2},{1,3}}) == r_t{0,1,2,3}));
  }

  {RandGen::RandGen_t g;
   for (RandGen::gen_uint_t n = 0; n < 100; ++n) {
     const auto G = RandomGraphs::binomial_rgr(n, {1,2}, g);
     struct FUN {
       AdjVecUInt::vecedges_t res;
       void operator()(const AdjVecUInt::edge_t& e) noexcept {
         res.push_back(e);
       }
     };
     FUN F;
     G.process_alledges(F);
     assert(F.res == G.alledges());
   }
  }

  {const Vector_2cols V(0);
   assert(V.result().empty());
   assert(V.size() == 0);
   assert(V.is_bipartite(AdjVecUInt(GT::und)));
   assert(V == Vector_2cols(0));
   assert(V == Vector_2cols(Vector_2cols::vi_t{}));
   std::ostringstream ss;
   ss << V;
   assert(ss.good());
   assert(ss.str().empty());
  }
  {const Vector_2cols V(1);
   assert(eqp(V.result(), {1}));
   assert(V.size() == 1);
   assert(V.is_bipartite(AdjVecUInt(GT::und, 1)));
   assert(V == Vector_2cols(1));
   assert(V == Vector_2cols(Vector_2cols::vi_t{1}));
   std::ostringstream ss;
   ss << V;
   assert(ss.good());
   assert(ss.str() == "1");
  }

  {using size_t = Generators::size_t;
   for (size_t n = 1; n <= 6; ++n)
     for (size_t m = 1; m <= 6; ++m) {
       const AdjVecUInt G(Generators::grid(n,m));
       const auto res = bipart_0comp(G);
       assert(res.n == G.n());
       assert(res.size() == G.n());
     }
   for (size_t n = 1; n <= 6; ++n) {
     const AdjVecUInt G(Generators::clique(n));
     const auto res = bipart_0comp(G);
     assert(res.n == G.n());
     assert(res.size() == (n <= 2 ? n : 0));
   }
  }

  {using size_t = Generators::size_t;
   {RandGen::RandGen_t g;
    for (size_t n = 0; n < 51; ++n)
      for (const size_t d : {2,5,10}) {
        auto G = RandomGraphs::binomial_rgr(n, {1,d}, g, true);
        test_edgefunctions(G);
        G = RandomGraphs::binomial_rgr(n, {1,d}, g, false);
        test_edgefunctions(G);
      }
   }
   for (const GT t : {GT::dir, GT::und})
     for (const bool with_loops : {false, true})
       for (size_t n = 0; n <= 6; ++n)
         test_edgefunctions(make_complete_AdjVecUInt(t,with_loops,n));
   for (size_t n = 0; n < 11; ++n)
     test_edgefunctions(AdjVecUInt(Generators::biclique(n,n)));
   for (size_t n = 0; n < 11; ++n)
     for (size_t m = 0; m < 11; ++m)
       test_edgefunctions(AdjVecUInt(Generators::grid(n,m)));
   for (size_t n = 3; n < 11; ++n)
     test_edgefunctions(AdjVecUInt(Generators::cycle(n)));
   for (size_t n = 0; n < 11; ++n)
     test_edgefunctions(AdjVecUInt(Generators::crown(n)));
  }

  {for (const GT t : {GT::dir, GT::und})
     for (size_t n = 0; n <= 3; ++n)
       for (const bool sorted : {false, true})
         assert(make_AdjVecUInt(t, n, {}, sorted) == AdjVecUInt(t,n));
  }
  {const AdjVecUInt G(Generators::cycle(3));
   assert(G == make_AdjVecUInt(GT::und, 3, {{2,0},{1,0},{1,2}}));
   assert(complement(G) == AdjVecUInt(GT::und, 3));
   assert(complement(G,true) ==
          make_AdjVecUInt(GT::und, 3, {{2,2},{1,1},{0,0}}));
  }
  {for (const GT t : {GT::dir, GT::und})
     for (size_t n = 0; n <= 4; ++n)
       for (const bool withloops : {false, true}) {
         assert(complement(AdjVecUInt(t,n), withloops) ==
                make_complete_AdjVecUInt(t, withloops, n));
         assert(complement(make_complete_AdjVecUInt(t, withloops, n)) ==
                AdjVecUInt(t,n));
       }
  }

  {assert(eqp(boolvec_colexicographic(AdjVecUInt(GT::und)), {}));
   assert(colex_index(AdjVecUInt(GT::und)) == 0);
   assert(eqp(boolvec_colexicographic(AdjVecUInt(GT::und),1), {}));
   assert(colex_index(AdjVecUInt(GT::und,2)) == 0);
   for (size_t n = 0; n <= 5; ++n)
     for (const bool with_loops : {false,true}) {
       const AdjVecUInt G = make_complete_AdjVecUInt(GT::und, with_loops, n);
       const auto B = boolvec_colexicographic(G, with_loops);
       const auto max_m = Sizes::max_m(n, GT::und, with_loops);
       assert(B.size() == max_m);
       for (const bool v : B) assert(v);
       assert(colex_index(G, with_loops) == FP::pow(2, max_m)-1);
     }
   {const auto G = make_AdjVecUInt(GT::und, 4, {{0,2},{1,3}});
    assert(eqp(boolvec_colexicographic(G), {false,true,false,false,true}));
    assert(eqp(boolvec_colexicographic(G,true), {false,false,false,true,false,false,false,true}));
   }
   {for (size_t n = 2; n <= 10; ++n) {
      auto G = make_AdjVecUInt(GT::und, n, {{0,1}});
      assert(eqp(boolvec_colexicographic(G), {true}));
      assert(colex_index(G) == 1);
      assert(eqp(boolvec_colexicographic(G,true), {false,true}));
      assert(colex_index(G,true) == 2);
      G = make_AdjVecUInt(GT::und, n, {{n-2,n-1}});
      {const auto m = Sizes::max_m(n,G.type(),false);
       std::vector<bool> B(m);
       B.back() = true;
       assert(boolvec_colexicographic(G) == B);
       assert(colex_index(G) == FP::pow(2, m-1));}
      {const auto m = Sizes::max_m(n,G.type(),true);
       std::vector<bool> B(m-1);
       B.back() = true;
       assert(boolvec_colexicographic(G,true) == B);
       assert(colex_index(G,true) == FP::pow(2, m-2));}
    }
   }
   {const auto G = make_AdjVecUInt(GT::und, 12, {{8,11}});
    {std::vector<bool> B(64); B.back() = true;
     assert(boolvec_colexicographic(G) == B);}
    assert(colex_index(G) == FP::pow(2,63));
   }
   {const auto G = make_AdjVecUInt(GT::und, 12, {{9,11}});
    {std::vector<bool> B(65); B.back() = true;
     assert(boolvec_colexicographic(G) == B);}
    assert(colex_index(G) == 0);
   }
  }

  {const AdjVecUInt G(GT::dir);
   assert(G.vertex_range.empty());
  }
  {typedef std::vector<int> VT;
   const AdjVecUInt G(GT::und, 5);
   assert(G.vertex_range.size() == G.n());
   assert(std::ranges::equal(G.vertex_range, std::views::iota(0,5)));
   assert(eqp(G.vcon<VT>(), {0,1,2,3,4}));
  }
  {typedef std::set<unsigned> ST;
   const AdjVecUInt G(GT::und, 5);
   assert(eqp(G.vcon<ST>(), {0,1,2,3,4}));
  }
}
