// Oliver Kullmann, 22.2.2022 (Swansea)
/* Copyright 2022, 2023 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#include <iostream>
#include <sstream>
#include <algorithm>
#include <vector>

#include <cassert>

#include <ProgramOptions/Environment.hpp>

#include "Bicliques2SAT.hpp"
#include "Graphs.hpp"
#include "Generators.hpp"
#include "Bicliques.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.4.0",
        "8.3.2023",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Bicliques/TestBicliques2SAT.cpp",
        "GPL v3"};

  using namespace Bicliques2SAT;
  using namespace Graphs;
  using namespace Generators;
  using namespace Bicliques;

  template <class X>
  constexpr bool eqp(const X& lhs, const X& rhs) noexcept {
    return lhs == rhs;
  }
}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv))
  return 0;

  {std::stringstream in;
   const std::string inp =
     "a b c d\n"
     "b c d\n"
     "c d\n"; // K_4 on a,b,c,d
   in << inp;
   const auto G = make_AdjVecUInt(in, GT::und);
   assert(G.n() == 4);
   assert(G.m() == 6);

   {VarEncoding enc(G, 1);
    assert(enc.V == 4);
    assert(enc.E == 6);
    assert(enc.B() == 1);
    assert(enc.nb() == 8);
    assert(enc.ne() == 6);
    assert(enc.n() == 14);
    for (unsigned i = 0; i < 4; ++i)
      assert(enc.left(i,0) == 1+i);
    for (unsigned i = 0; i < 4; ++i)
      assert(enc.right(i,0) == 5+i);
    for (unsigned i = 0; i < 6; ++i)
      assert(enc.edge(i,0) == 9+i);
    assert(eqp(enc.inv(1), {0,true,0}));
    for (var_t v = 1; v <= 8; ++v) {
      const auto el = enc.inv(v);
      if (el.left) assert(v == enc.left(el.v, el.b));
      else assert(v == enc.right(el.v, el.b));
    }
    assert(eqp(enc.inv(1), {0,true,0}));
    assert(eqp(enc.inv(2), {0,true,1}));
    assert(eqp(enc.inv(7), {0,false,2}));
    std::stringstream ss("1 2 -3 -4 7 -8 0\n");
    assert(eqp(enc.core_extraction(ss), { {{{0,1},{2}}} }));
    ss.str("7 2 1 -3 -4 -8 0\n");
    assert(eqp(enc.core_extraction(ss), { {{{0,1},{2}}} }));
    using Lit = VarEncoding::Lit;
    assert(eqp(enc.core_extraction(
               {Lit{1,1},Lit{2,1},Lit{7,1}}),
               { {{{0,1},{2}}} }));
    ss.str("0\n");
    assert(eqp(enc.core_extraction(ss), {}));
    assert(eqp(enc.core_extraction({}),{}));
    enc.update_B(0);
    assert(enc.B() == 0);
    assert(enc.nb() == 0);
    assert(enc.ne() == 0);
    assert(enc.n() == 0);
   }

   VarEncoding enc2(G, 2);
   assert(enc2.V == 4);
   assert(enc2.E == 6);
   assert(enc2.B() == 2);
   assert(enc2.nb() == 16);
   assert(enc2.ne() == 12);
   assert(enc2.n() == 28);
   for (unsigned i = 0; i < 4; ++i)
     assert(enc2.left(i,0) == 1+i);
   for (unsigned i = 0; i < 4; ++i)
     assert(enc2.right(i,0) == 5+i);
   for (unsigned i = 0; i < 4; ++i)
     assert(enc2.left(i,1) == 9+i);
   for (unsigned i = 0; i < 4; ++i)
     assert(enc2.right(i,1) == 13+i);
   for (unsigned i = 0; i < 6; ++i)
     assert(enc2.edge(i,0) == 17+i);
   for (unsigned i = 0; i < 6; ++i)
     assert(enc2.edge(i,1) == 23+i);
   for (var_t v = 1; v <= 16; ++v) {
     const auto el = enc2.inv(v);
     if (el.left) assert(v == enc2.left(el.v, el.b));
     else assert(v == enc2.right(el.v, el.b));
   }
   std::stringstream ss("0\n");
   assert(eqp(enc2.core_extraction(ss), {}));
   ss.str("17 -18 0\n");
   assert(eqp(enc2.core_extraction(ss), {}));
   ss.str("9 10 16 1 2 -3 -4 7 -8 0\n");
   assert(eqp(enc2.core_extraction(ss), { { {{0,1},{2}}, {{0,1},{3}} } }));
   ss.str("9 10 15 1 2 -3 -4 7 -8 4 16 0\n");
   assert(eqp(enc2.core_extraction(ss), { { {{0,1,3},{2}}, {{0,1},{2,3}} } }));

   BC2SAT trans1(G, 1);
   assert(trans1.num_basic_lit() == 2*4 + 3*36 + 1*6);
   for (unsigned e1 = 0; e1 < G.m(); ++e1)
     for (unsigned e2 = 0; e2 < G.m(); ++e2)
       assert(Bicliques::bccomp(trans1.edges[e1], trans1.edges[e2], G));
  }

  {AdjMapStr G(GT::und);
   G.add_clique(std::vector{"a", "b", "c", "d"});
   G.add_clique(std::vector{"e", "f", "g", "h"});
   AdjVecUInt Ga(G);

   {BC2SAT trans(Ga, 2);
    assert(trans.enc().V == 8);
    assert(trans.enc().E == 12);
    assert(trans.enc().n() == 56);
    assert(trans.enc().nb() == 32);
    for (var_t v = 1; v <= 32; ++v) {
      const auto el = trans.enc().inv(v);
      if (el.left) assert(v == trans.enc().left(el.v, el.b));
      else assert(v == trans.enc().right(el.v, el.b));
    }

    for (unsigned i = 0; i < 12; ++i)
      for (unsigned j = 0; j < 12; ++j)
        assert(Bicliques::bccomp(trans.edges[i],trans.edges[j],Ga) == ((i<6 and j<6) or (i>=6 and j>=6)));
   }

   assert(eqp(G.add_clique(std::vector{"a","b","e","f"}), {0,4}));
   Ga = AdjVecUInt(G);
   assert(Ga.n() == 8);
   assert(Ga.m() == 16);
   BC2SAT trans(Ga,2);
   assert(trans.enc().n() == 64);
   assert(trans.enc().nb() == 32);
   assert(eqp(trans.edges[0], {0,1}));
   assert(eqp(trans.edges[3], {0,4}));
   assert(eqp(trans.edges[5], {1,2}));
   assert(eqp(trans.edges[9], {2,3}));
   assert(Bicliques::bccomp(trans.edges[3],trans.edges[5],Ga));
   assert(Bicliques::bccomp(trans.edges[5],trans.edges[3],Ga));
   assert(not Bicliques::bccomp(trans.edges[3],trans.edges[9],Ga));
   assert(not Bicliques::bccomp(trans.edges[9],trans.edges[3],Ga));

   RandGen::RandGen_t g;
   for (unsigned i = 0; i < 100; ++i) {
     auto res = trans.max_bcincomp(g);
     const auto s = res.size();
     assert(s==2 or s==3);
     if (s == 2) continue;
     std::ranges::sort(res);
     assert(std::ranges::includes(res, BC2SAT::vei_t{9,15}));
   }
   assert(trans.max_bcincomp(3,g).first.size() == 3);

   std::stringstream ss;
   ss << trans.nonedge_for_bc(0,1,0);
   assert(ss.str() == "-1 -10 0\n-2 -9 0\n");
   ss.str("");
   assert(trans.all_nonedges_for_bcs(ss) == 2 * (2 * (36 - 16) - 8)); // 64
   ss.str("");
   ss << trans.edge_def(0,0);
   assert(ss.str() ==
          "-33 1 2 0\n"
          "-33 1 9 0\n"
          "-33 10 2 0\n"
          "-33 10 9 0\n"
          "-1 -10 33 0\n"
          "-2 -9 33 0\n");
   ss.str("");
   assert(trans.all_edges_def(ss) == 2 * 6 * 16); // 192
   assert(eqp(trans.edge_cov(0), {{{33,1},{49,1}}}));
   assert(trans.all_edges_cov(ss) == 16);
   assert(trans.all_basic_clauses(ss) == 64 + 192 + 16); // 272

   ss.str("");
   ss << trans.place_edge(0,0);
   assert(ss.str() ==
          "1 0\n"
          "10 0\n"
          "33 0\n");
   assert(trans.all_sbedges({0,1},ss) == 6);

   ss.str("");
   assert(eqp(trans(ss, {SB::basic}, {DC::without, DP::without, CS::without}, 1, {}), {64, 272 + 2*3})); // 278
   assert(ss.str().empty());
   ss.str("");
   {bool caught = false;
    try {
      trans(ss, {SB::basic}, {DC::without, DP::without, CS::without}, 7, {});
    }
    catch(const BC2SAT::Unsatisfiable& exc) {
      caught = true;
      assert(exc.B == 2);
      assert(exc.incomp.size() == 3);
    }
    assert(caught);
   }
   ss.str("");
   assert(eqp(trans(ss, {SB::basic}, {DC::without, DP::with, CS::without}, 6, {}), {64, 278}));
   assert(ss.str() == "p cnf 64 278\n");
   trans.update_B(1);
   assert(trans.all_nonedges_for_bcs(ss) == 1 * (2 * (36 - 16) - 8)); // 32
   assert(trans.all_edges_def(ss) == 1 * 6 * 16); // 96
   assert(trans.all_edges_cov(ss) == 16);
   assert(trans.all_basic_clauses(ss) == 32 + 96 + 16); // 144
   assert(eqp(trans(ss, {SB::none}, {DC::without, DP::with, CS::without}, 0, {}), {32, 144}));
  }

  {typedef std::vector<int> v_t;
   v_t v;
   const auto tr = [](const auto&){return true;};
   const auto fa = [](const auto&){return false;};
   assert(erase_if_byswap(v, tr) == 0);
   assert(v.empty());
   v.assign({1,2,3});
   assert(erase_if_byswap(v, fa) == 0);
   assert(v.size() == 3);
   assert(erase_if_byswap(v, tr) == 3);
   assert(v.empty());
   v.assign({1,2,3,4,5});
   const auto t35 = [](const auto& x){return x==3 or x==5;};
   assert(erase_if_byswap(v, t35) == 2);
   assert(eqp(v, {1,2,4}));
  }

  {for (size_t n = 0; n < 6; ++n) {
     const auto G = BC2SAT::graph_t(clique(n));
     const size_t bcc = bcc_clique(n);
     for (size_t dist = 0; dist < 4; ++dist) {
       const size_t B = bcc + dist;
       BC2SAT trans(G, B);
       std::stringstream out;
       const auto res = trans(nullptr, {}, 100, 1, {dist});
       assert(res.B == bcc);
       assert(res.init_B == B);
       assert(res.rt == ResultType::exact);
       assert(is_bcc(res.bcc, G));
     }
   }
   for (size_t n = 0; n < 6; ++n) {
     const auto G = BC2SAT::graph_t(biclique(n,n));
     const size_t bcc = bcc_biclique(n,n);
     for (size_t dist = 0; dist < 4; ++dist) {
       const size_t B = bcc + dist;
       BC2SAT trans(G, B);
       std::stringstream out;
       const auto res = trans(nullptr, {}, 100, 1, {dist});
       assert(res.B == bcc);
       assert(res.init_B == B);
       assert(res.rt == ResultType::exact);
       assert(is_bcc(res.bcc, G));
     }
   }
   for (size_t n = 0; n < 6; ++n) {
     const auto G = BC2SAT::graph_t(crown(n));
     const size_t bcc = bcc_crown(n);
     for (size_t dist = 0; dist < 4; ++dist) {
       const size_t B = bcc + dist;
       BC2SAT trans(G, B);
       std::stringstream out;
       const auto res = trans(nullptr, {}, 100, 1, {dist});
       assert(res.B == bcc);
       assert(res.init_B == B);
       assert(res.rt == ResultType::exact);
       assert(is_bcc(res.bcc, G));
     }
   }
   for (size_t n = 0; n < 6; ++n)
     for (size_t m = 0; m < 6; ++m) {
       const auto G = BC2SAT::graph_t(grid(n,m));
       const size_t bcc = bcc_grid(n,m);
       for (size_t dist = 0; dist < 4; ++dist) {
         const size_t B = bcc + dist;
         BC2SAT trans(G, B);
         std::stringstream out;
         const auto res = trans(nullptr, {}, 100, 1, {dist});
         assert(res.B == bcc);
         assert(res.init_B == B);
         assert(res.rt == ResultType::exact);
         assert(is_bcc(res.bcc, G));
       }
     }
  }

}
