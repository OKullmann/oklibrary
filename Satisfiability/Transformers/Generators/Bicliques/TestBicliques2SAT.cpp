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
        "0.8.1",
        "1.5.2023",
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

   BC2SAT trans1(G, {DI::none, Bounds::choose_l{}, {1,false}});
   assert(trans1.num_basic_lit() == 2*4 + 3*36 + 1*6);
   for (unsigned e1 = 0; e1 < G.m(); ++e1)
     for (unsigned e2 = 0; e2 < G.m(); ++e2)
       assert(Bicliques::bccomp(trans1.edges[e1], trans1.edges[e2], G));
  }

  {AdjMapStr G(GT::und);
   G.add_clique(std::vector{"a", "b", "c", "d"});
   G.add_clique(std::vector{"e", "f", "g", "h"});
   AdjVecUInt Ga(G);

   {BC2SAT trans(Ga, {DI::none, Bounds::choose_l{}, {2,false}});
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
   BC2SAT trans(Ga, {DI::none, Bounds::choose_l{}, {2,false}});
   assert(trans.enc().n() == 64); // nb + B * 16 = 32 * B
   assert(trans.enc().nb() == 32); // B * 2 * 8
   assert(eqp(trans.edges[0], {0,1}));
   assert(eqp(trans.edges[3], {0,4}));
   assert(eqp(trans.edges[5], {1,2}));
   assert(eqp(trans.edges[9], {2,3}));
   assert(Bicliques::bccomp(trans.edges[3],trans.edges[5],Ga));
   assert(Bicliques::bccomp(trans.edges[5],trans.edges[3],Ga));
   assert(not Bicliques::bccomp(trans.edges[3],trans.edges[9],Ga));
   assert(not Bicliques::bccomp(trans.edges[9],trans.edges[3],Ga));

   RandGen::vec_eseed_t seeds(1);
   for (unsigned i = 1; i <= 100; ++i) {
     seeds[0] = i;
     auto res = trans.max_bcincomp(seeds);
     const auto s = res.size();
     assert(s==2 or s==3);
     if (s == 2) continue;
     std::ranges::sort(res);
     assert(std::ranges::includes(res, BC2SAT::vei_t{9,15}));
   }
   assert(trans.max_bcincomp(10,{}).v.size() == 2);
   assert(trans.max_bcincomp(11,{}).v.size() == 3);

   std::stringstream ss;
   ss << trans.nonedge_for_bc(0,1,0);
   assert(ss.str() == "-1 -10 0\n-2 -9 0\n");
   ss.str("");
   assert(trans.all_nonedges_for_bcs<std::ostream&>(ss)
          == 2 * (2 * (36 - 16) - 8)); // 64
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
   assert(trans.all_edges_def<std::ostream&>(ss) == 2 * 6 * 16); // 192
   assert(eqp(trans.edge_cov(0), {{{33,1},{49,1}}}));
   assert(trans.all_edges_cov<std::ostream&>(ss) == 16);
   assert(trans.all_basic_clauses<std::ostream&>(ss) == 64 + 192 + 16); // 272

   ss.str("");
   ss << trans.place_edge(0,0);
   assert(ss.str() ==
          "1 0\n"
          "10 0\n"
          "33 0\n");
   assert(trans.all_sbedges<std::ostream&>({0,1},ss) == 6);

   ss.str("");
   assert(eqp(trans.sat_translate(ss, {},
     {DC::without, DP::without, CS::without}, 1, {}), {64, 272 + 2*3})); // 278
   assert(ss.str().empty());
   ss.str("");
   assert(eqp(trans.sat_translate(ss,{},
     {DC::without,DP::without,CS::without},11,{}), {96, 409})); // {B * 32, B * 128 + 16 + 3*#units}

   ss.str("");
   assert(eqp(trans.sat_translate(ss, {},
     {DC::without, DP::with, CS::without}, 6, {}), {96, 406}));
   assert(ss.str() == "p cnf 96 406\n");
   trans.update_B(1);
   assert(trans.all_nonedges_for_bcs<std::ostream&>(ss)
          == 1 * (2 * (36 - 16) - 8)); // 32
   assert(trans.all_edges_def<std::ostream&>(ss) == 1 * 6 * 16); // 96
   assert(trans.all_edges_cov<std::ostream&>(ss) == 16);
   assert(trans.all_basic_clauses<std::ostream&>(ss) == 32 + 96 + 16); // 144
   assert(eqp(trans.sat_translate(ss,
     {SB::none,{}}, {DC::without, DP::with, CS::without}, 0, {}), {96, 400}));
  }

  {for (size_t n = 0; n < 6; ++n) {
     const auto G = BC2SAT::graph_t(clique(n));
     const size_t bcc = bcc_clique(n);
     for (size_t dist = 1; dist <= 4; ++dist) {
       const size_t B = bcc + dist;
       BC2SAT trans(G, {DI::downwards, Bounds::choose_u{}, {B,false}});
       std::stringstream out;
       const auto res = trans.sat_solve(nullptr, {}, 100, 1, {dist});
       assert(res.B == bcc);
       assert(res.init_B == (n<=1 ? 0 : B));
       assert(res.rt == ResultType::exact);
       assert(is_bcc(res.bcc, G));
     }
   }
   for (size_t n = 0; n < 6; ++n) {
     const auto G = BC2SAT::graph_t(biclique(n,n));
     const size_t bcc = bcc_biclique(n,n);
     for (size_t dist = 1; dist <= 4; ++dist) {
       const size_t B = bcc + dist;
       BC2SAT trans(G, {DI::downwards, Bounds::choose_u{}, {B,false}});
       std::stringstream out;
       const auto res = trans.sat_solve(nullptr, {}, 100, 1, {dist});
       assert(res.B == bcc);
       assert(res.init_B == (n==0 ? 0 : B));
       assert(res.rt == ResultType::exact);
       assert(is_bcc(res.bcc, G));
     }
   }
   for (size_t n = 0; n < 6; ++n) {
     const auto G = BC2SAT::graph_t(crown(n));
     const size_t bcc = bcc_crown(n);
     for (size_t dist = 1; dist <= 4; ++dist) {
       const size_t B = bcc + dist;
       BC2SAT trans(G, {DI::downwards, Bounds::choose_u{}, {B,false}});
       std::stringstream out;
       const auto res = trans.sat_solve(nullptr, {}, 100, 1, {dist});
       assert(res.B == bcc);
       assert(res.init_B == (n<=1 ? 0 : B));
       assert(res.rt == ResultType::exact);
       assert(is_bcc(res.bcc, G));
     }
   }
   for (size_t n = 0; n < 6; ++n)
     for (size_t m = 0; m < 6; ++m) {
       const auto G = BC2SAT::graph_t(grid(n,m));
       const size_t bcc = bcc_grid(n,m);
       for (size_t dist = 1; dist <= 4; ++dist) {
         const size_t B = bcc + dist;
         BC2SAT trans(G, {DI::downwards, Bounds::choose_u{}, {B,false}});
         std::stringstream out;
         const auto res = trans.sat_solve(nullptr, {}, 100, 1, {dist});
         assert(res.B == bcc);
         assert(res.init_B == (n*m <= 1 ? 0 : B));
         assert(res.rt == ResultType::exact);
         assert(is_bcc(res.bcc, G));
       }
     }
  }

  {for (size_t n = 0; n < 6; ++n) {
     const auto G = BC2SAT::graph_t(clique(n));
     const size_t bcp = bcp_clique(n);
     for (size_t dist = 1; dist <= 4; ++dist) {
       const size_t B = bcp + dist;
       BC2SAT trans(G, {DI::downwards, Bounds::choose_u{}, {B,false}});
       std::stringstream out;
       const auto res =
         trans.sat_solve(nullptr, {{},PT::partition2,{},{},{}}, 100, 1, {dist});
       assert(res.B == bcp);
       assert(res.init_B == (n<=1 ? 0 : B));
       assert(res.rt == ResultType::exact);
       assert(is_bcp(res.bcc, G));
     }
    }
  }
  {for (size_t n = 0; n < 6; ++n) {
     const auto G = BC2SAT::graph_t(biclique(n,n));
     const size_t bcp = bcp_biclique(n,n);
     for (size_t dist = 1; dist <= 4; ++dist) {
       const size_t B = bcp + dist;
       BC2SAT trans(G, {DI::downwards, Bounds::choose_u{}, {B,false}});
       std::stringstream out;
       const auto res =
         trans.sat_solve(nullptr, {{},PT::partition2,{},{},{}}, 100, 1, {dist});
       assert(res.B == bcp);
       assert(res.init_B == (n==0 ? 0 : B));
       assert(res.rt == ResultType::exact);
       assert(is_bcp(res.bcc, G));
     }
    }
  }

  {for (size_t n = 0; n < 6; ++n) {
     const auto G = BC2SAT::graph_t(grid(n,n));
     const size_t bcc = bcc_grid(n,n);
     for (size_t dist = 1; dist <= 4; ++dist) {
       const size_t B = bcc + dist;
       BC2SAT trans(G, {DI::downwards, Bounds::choose_u{}, {B,true}});
       std::stringstream out;
       const auto res =
         trans.sat_solve(nullptr, {}, 100, 1, {dist});
       assert(res.B == bcc);
       assert(res.init_B == (n<=1 ? 0 : B));
       assert(res.rt == ResultType::exact);
       assert(is_bcc(res.bcc, G));
     }
    }
  }

  {using namespace DimacsTools;
   std::istringstream is;
   is.str("p cnf 10 5\n"
          "a 3 5 7 8 0\n"
          "e 1 2 0\n"
          "a 4 6 9 10 0\n"
          "-1 10      0\n"          // 0 0
          "           -3 -5 0\n"    // 1 1 0
          "4 -10      3 -8 0\n"     // 2 1 0
          "-1 6 10    -7 0\n"       // 3 2 1
          "4 -9       7 0\n");      // 4 2 1
   const GslicedCNF F = read_strict_GslicedCNF(is);
   std::ostringstream os;
   os << F;
   assert(os);
   assert(os.str() ==
       "p cnf 10 5\n" "a 3 5 7 8 0\n" "e 1 2 0\n" "a 4 6 9 10 0\n"
       "-1 10 0\n" "-3 -5 0\n" "4 -10 3 -8 0\n" "-1 6 10 -7 0\n" "4 -9 7 0\n");
   const GlobRepl GR(F);
   assert(GR.F == F);
   {const ConflictGraphs::VarStatistics s(GR.occ);
    assert(s.n_total == 8);
    assert(s.n_max == 8);
    assert(s.n == 4);
    assert(s.num_trivial == 4); // 1, 2, 4, 6
    assert(s.num_pure == 2); // 5, 8
    assert(s.num_singular == 2); // 3, 7
    assert(s.num_osingular == 2);
    assert(s.num_nonosingular == 0);
    assert(s.num_nonsingular == 0);
    assert(s.freq_deg.num_inputs() == 4);
    assert(s.freq_deg.num_values() == 2);
    assert(eqp(s.freq_deg.cmap(), {{1,2},{2,2}}));
    assert(s.freq_muldeg.num_inputs() == 4);
    assert(s.freq_muldeg.num_values() == 2);
    assert(eqp(s.freq_muldeg.cmap(), {{0,2},{1,2}}));
    assert(s.freq_maxdeg.num_inputs() == 4);
    assert(s.freq_maxdeg.num_values() == 1);
    assert(eqp(s.freq_maxdeg.cmap(), {{1,4}}));
    assert(s.freq_mindeg.num_inputs() == 4);
    assert(s.freq_mindeg.num_values() == 2);
    assert(eqp(s.freq_mindeg.cmap(), {{0,2},{1,2}}));
    assert(s.freq_ldeg.num_inputs() == 8);
    assert(s.freq_ldeg.num_values() == 2);
    assert(eqp(s.freq_ldeg.cmap(), {{0,2},{1,6}}));
   }
   assert((GR.CC == GraphTraversal::CCbyIndices{{1,2,2,3,3},3}));
   assert(eqp(GR.sizes, {1,2,2}));
   assert(GR.numntcc == 2);
   assert(eqp(GR.ccvec, {{0},{1,2},{3,4}}));
   assert(eqp(GR.ntcc, {2,3}));
   assert(eqp(GR.ntvar, {{Var(3),Var(5),Var(8)}, {Var(7)}}));
   assert(eqp(GR.ntcc_map, {{{1,2},{1}}, {{3,2},{0}}}));
   os.clear(); os.str("");
   GR.E0(os);
   assert(os);
   assert(os.str() ==
          "p cnf 10 1\n" "-1 10 0\n");
   {auto it = GR.ntcc_map.begin();
    os.clear(); os.str("");
    GR.A(os, it, 0);
    assert(os.str() ==
           "p cnf 1 2\n" "-1 0\n" "1 0\n");
    os.clear(); os.str("");
    GR.E(os, it, 0);
    assert(os.str() ==
           "p cnf 10 2\n" "-1 6 10 0\n" "4 -9 0\n");
    ++it;
    os.clear(); os.str("");
    GR.A(os, it, 0);
    assert(os.str() ==
           "p cnf 3 2\n" "-1 -2 0\n" "1 -3 0\n");
    os.clear(); os.str("");
    GR.E(os, it, 0);
    assert(os.str() ==
           "p cnf 10 2\n" "0\n" "4 -10 0\n");
   }
   {const auto F = GR.solve_ntcc(0, nullptr, {}, 100, 1, {});
    assert(eqp(F, {}));
   }
   {const auto F = GR.solve_ntcc(1, nullptr, {}, 100, 1, {});
    assert(eqp(F, {}));
   }
   {const auto F = GR.solve(nullptr, {}, 100, 1, {});
    assert(eqp(F, {{{7,5}, {{}, {Lit(-3)}, {Lit(3)}, {Lit(-7)}, {Lit(7)}}},
                   {Var(3),Var(7)}}));
   }


   {const GlobRepl GR2(GR);
    // assert(GR2 == GR); GCC 10.3 compiler error CERR
    assert(gcg_equivalence(GR, GR2, nullptr) == GCGE::eq);
   }
   {GslicedCNF F2(F);
    F2.G().second[0].push_back(Lit(-5));
    assert(gcg_equivalence(GR, GlobRepl(F2), nullptr) == GCGE::eq);
    F2.G().second[0].push_back(Lit(-8));
    assert(gcg_equivalence(GR, GlobRepl(F2), nullptr) == GCGE::eq);
    F2.G().second[0].back().neg();
    assert(gcg_equivalence(GR, GlobRepl(F2), nullptr) == GCGE::diff_sizes);
   }

   {std::istringstream is;
    is.str("p cnf 10 5\n"
           "a 3 5 0\n"
           "e 1 2 0\n"
           "a 4 6 9 10 0\n"
           "-1 10      0\n"       // 0
           "           -3 0\n"    // 1
           "4 -10      3 0\n"     // 2
           "-1 6 10    -5 0\n"    // 3
           "4 -9       5 0\n");   // 4
    const auto F2 = read_strict_GslicedCNF(is);
    assert(gcg_equivalence(GR, GlobRepl(F2), nullptr) == GCGE::eq);
    {auto F3(F2);
     F3.O().second[1].push_back(Lit(2));
     std::ostringstream os;
     assert(gcg_equivalence(GR, GlobRepl(F3), &os) == GCGE::diff_O);
     assert(os.str() ==
            "other-parts different\n"
            "  clauses with index 2 differ:\n"
            "0\n"
            "2 0\n");
    }
   }

   {std::istringstream is;
    is.str("p cnf 10 5\n"
           "a 3 5 0\n"
           "e 1 2 0\n"
           "a 4 6 9 10 0\n"
           "-1 10      0\n"       // 0
           "           -3 0\n"    // 1
           "4 -10      -5 0\n"    // 2
           "-1 6 10    3 0\n"     // 3
           "4 -9       5 0\n");   // 4
    const auto F2 = read_strict_GslicedCNF(is);
    assert(gcg_equivalence(GR, GlobRepl(F2), nullptr) == GCGE::diff_comp);
   }

   {std::istringstream is;
    is.str("p cnf 10 5\n"
           "a 3 5 7 0\n"
           "e 1 2 0\n"
           "a 4 6 9 10 0\n"
           "-1 10      0\n"       // 0
           "           3 0\n"     // 1
           "4 -10      -3 5 0\n"  // 2
           "-1 6 10    -5 7 0\n"  // 3
           "4 -9       -7 0\n");  // 4
    const auto F = read_strict_GslicedCNF(is);
    is.str("p cnf 10 5\n"
           "a 3 5 7 0\n"
           "e 1 2 0\n"
           "a 4 6 9 10 0\n"
           "-1 10      0\n"       // 0
           "           3 0\n"     // 1
           "4 -10      -3 5 0\n"  // 2
           "-1 6 10    -5 7 0\n"  // 3
           "4 -9       -7 3 0\n");  // 4
    const auto F2 = read_strict_GslicedCNF(is);
    assert(gcg_equivalence(GlobRepl(F), GlobRepl(F2), nullptr)
           == GCGE::diff_cg);
   }

   {std::istringstream is;
    is.str("p cnf 7 6\n"
           "a 3 5 7 0\n"
           "e 1 2 4 6 0\n"
           "3 2 -4 0\n"        // 0 0
           "-3 5 1 6 0\n"      // 0 0
           "-2 -4 0\n"         // 1
           "-5 -6 0\n"         // 0 0
           "7 2 4 6 0\n"       // 2 1
           "-7 -2 -4 -6 0\n"); // 2 1
     const auto F = read_strict_GslicedCNF(is);
     const GlobRepl GR(F);
     {const ConflictGraphs::VarStatistics s(GR.occ);
      assert(s.n_total == 7);
      assert(s.n_max == 7);
      assert(s.n == 3); // 3, 5, 7
      assert(s.num_trivial == 4); // 1, 2, 4, 6
      assert(s.num_pure == 0);
      assert(s.num_singular == 3); // 3, 5, 7
      assert(s.num_osingular == 3);
      assert(s.num_nonosingular == 0);
      assert(s.num_nonsingular == 0);
      assert(s.freq_deg.num_inputs() == 3);
      assert(s.freq_deg.num_values() == 1);
      assert(eqp(s.freq_deg.cmap(), {{2,3}}));
      assert(s.freq_muldeg.num_inputs() == 3);
      assert(s.freq_muldeg.num_values() == 1);
      assert(eqp(s.freq_muldeg.cmap(), {{1,3}}));
      assert(s.freq_maxdeg.num_inputs() == 3);
      assert(s.freq_maxdeg.num_values() == 1);
      assert(eqp(s.freq_maxdeg.cmap(), {{1,3}}));
      assert(s.freq_mindeg.num_inputs() == 3);
      assert(s.freq_mindeg.num_values() == 1);
      assert(eqp(s.freq_mindeg.cmap(), {{1,3}}));
      assert(s.freq_ldeg.num_inputs() == 6);
      assert(s.freq_ldeg.num_values() == 1);
      assert(eqp(s.freq_ldeg.cmap(), {{1,6}}));
     }
     assert(eqp(GR.ntcc_map, {{{1,2},{1}}, {{2,3},{0}}}));
     std::ostringstream os;
     GR.E0(os);
     assert(os);
     assert(os.str() ==
            "p cnf 6 1\n" "-2 -4 0\n");
     auto it = GR.ntcc_map.begin();
     os.clear(); os.str("");
     GR.A(os, it, 0);
     assert(os.str() ==
            "p cnf 1 2\n" "1 0\n" "-1 0\n");
     os.clear(); os.str("");
     GR.E(os, it, 0);
     assert(os.str() ==
            "p cnf 6 2\n" "2 4 6 0\n" "-2 -4 -6 0\n");
     ++it;
     os.clear(); os.str("");
     GR.A(os, it, 0);
     assert(os.str() ==
            "p cnf 2 3\n" "1 0\n" "-1 2 0\n" "-2 0\n");
     os.clear(); os.str("");
     GR.E(os, it, 0);
     assert(os.str() ==
            "p cnf 6 3\n" "2 -4 0\n" "1 6 0\n" "-6 0\n");
     {const auto F = GR.solve_ntcc(0, nullptr, {}, 100, 1, {});
      assert(eqp(F, {{1,3}, {{Lit(1)}, {Lit(-1)}, {Lit(1)}}}));
     }
     {const auto F = GR.solve_ntcc(1, nullptr, {}, 100, 1, {});
      assert(eqp(F, {}));
     }
     {const auto F = GR.solve(nullptr, {}, 100, 1, {});
      assert(eqp(F,{{{7,6},
                    {{Lit(3)}, {Lit(-3)}, {}, {Lit(3)}, {Lit(7)}, {Lit(-7)}}},
                    {Var(3),Var(7)}}));
     }

   }

  }

  {std::istringstream is;
   is.str("p cnf 28 10\n"
          "a 1 2 3 4 5 6 7 8 9 10 11 12 13 0\n"
          "e 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 0\n"
          "1 4 10 -15 18 0\n"
          "-7 -9 12 15 18 0\n"
          "1 11 12 20 -21 0\n"
          "14 15 16 19 21 0\n"
          "5 9 -11 -15 -22 0\n"
          "1 -8 12 19 24 0\n"
          "9 -13 -15 -23 26 0\n"
          "2 3 -4 21 -27 0\n"
          "-6 8 -17 -25 -27 0\n"
          "-2 7 13 -18 -28 0\n");
   const auto F = DimacsTools::read_strict_GslicedCNF(is);
   const GlobRepl GR(F);
   {const ConflictGraphs::VarStatistics s(GR.occ);
    assert(s.n_total == 13);
    assert(s.n_max == 13);
    assert(s.n == 13);
    assert(s.num_trivial == 0);
    assert(s.num_pure == 6); // 1(3), 3(1), 5(1), 6(1), 10(1), 12(3)
    assert(s.num_singular == 7); // 2, 4, 7, 8, 9, 11, 13
    assert(s.num_osingular == 6); // 2, 4, 7, 8, 11, 13
    assert(s.num_nonosingular == 1); // 9(2,1)
    assert(s.num_nonsingular == 0);
    assert(s.freq_deg.num_inputs() == 13);
    assert(s.freq_deg.num_values() == 3);
    assert(eqp(s.freq_deg.cmap(), {{1,4},{2,6},{3,3}}));
    assert(s.freq_muldeg.num_inputs() == 13);
    assert(s.freq_muldeg.num_values() == 3);
    assert(eqp(s.freq_muldeg.cmap(), {{0,6},{1,6},{2,1}}));
    assert(s.freq_maxdeg.num_inputs() == 13);
    assert(s.freq_maxdeg.num_values() == 3);
    assert(eqp(s.freq_maxdeg.cmap(), {{1,10},{2,1},{3,2}}));
    assert(s.freq_mindeg.num_inputs() == 13);
    assert(s.freq_mindeg.num_values() == 2);
    assert(eqp(s.freq_mindeg.cmap(), {{0,6},{1,7}}));
    assert(s.freq_ldeg.num_inputs() == 26);
    assert(s.freq_ldeg.num_values() == 4);
    assert(eqp(s.freq_ldeg.cmap(), {{0,6},{1,17},{2,1},{3,2}}));
   }
   const auto R = GR.solve(nullptr, {{},{},{},SO::nopre,{}}, 100, 1, {});
   const DimacsTools::GslicedCNF F2(F.O(), R, F.other);
   const GlobRepl GR2(F2);
   assert(int(gcg_equivalence(GR, GR2, nullptr)) == 0);
  }

  {std::istringstream is;
   is.str("p cnf 20 10\n"
          "a 1 2 3 4 5 6 7 8 9 10 0\n"
          "e 11 12 13 14 15 16 17 18 19 20 0\n"
          "-5 8 9 11 -12 15 0\n"
          "-1 2 5 7 -14 16 0\n"
          "-9 -11 -12 13 -16 -17 0\n"
          "1 2 -12 -13 16 17 0\n"
          "4 -7 9 -14 -18 -19 0\n"
          "2 6 -8 14 18 -19 0\n"
          "-7 9 10 13 -16 -20 0\n"
          "10 -11 12 -14 -17 -20 0\n"
          "-3 -4 -9 15 -18 -20 0\n"
          "3 -7 -8 -12 -15 20 0\n");
   const auto F = DimacsTools::read_strict_GslicedCNF(is);
   const GlobRepl GR(F);
   const auto R = GR.solve(nullptr, {}, 100, 1, {});
   const DimacsTools::GslicedCNF F2(F.O(), R, F.other);
   const GlobRepl GR2(F2);
   assert(int(gcg_equivalence(GR, GR2, nullptr)) == 0);
  }

}
