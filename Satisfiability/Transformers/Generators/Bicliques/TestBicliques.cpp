// Oliver Kullmann, 28.2.2022 (Swansea)
/* Copyright 2022, 2023, 2025 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#include <iostream>

#include <cassert>

#include <ProgramOptions/Environment.hpp>

#include "TestTools.hpp"
#include "Bicliques.hpp"
#include "Graphs.hpp"
#include "Generators.hpp"
#include "GraphTraversal.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.3.10",
        "20.5.2025",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Bicliques/TestBicliques.cpp",
        "GPL v3"};

  using namespace TestTools;
  using namespace Bicliques;
  using namespace Generators;

}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv))
  return 0;

  {AdjVecUInt G(Graphs::GT::und, 0);
   assert(valid(list_t{}, G));
   assert(not valid(list_t{0}, G));
   assert(not valid0(list_t{0}, G));
   assert(valid({list_t{}, list_t{}}, G));
   assert(not valid({list_t{0}, list_t{}}, G));
   assert(not valid({list_t{}, list_t{0}}, G));
   assert(valid(Bcc_frame{}, G));
   assert(valid(Bcc_frame{{{},{},{}}}, G));
   assert(is_bcc({}, G));
   assert(is_bcc(Bcc_frame{{{},{},{}}}, G));
   assert(not valid(Bcc_frame{{{},{},{{0},{}}}}, G));
   assert(not valid(Bcc_frame{{{},{},{{},{0}}}}, G));
  }

  {AdjVecUInt G(Graphs::GT::und, 5);
   assert(valid({list_t{0,1}, list_t{2,3}}, G));
   assert(valid01(list_t{0,0,1,1,2}, G));
   assert(not valid2(list_t{0,0,1,1,2}));
   assert(valid1(list_t{}));
   assert(not valid1({0,0,1,1,2,1}));
   assert(valid1(bc_frame{}));
   assert(not valid1({{0},{1,0}}));
   assert(not valid1({{1,0},{0}}));
   assert(valid1({{0,1},{0,0}}));
   assert(not valid2({{0,1},{0,0}}));
   assert(not valid2({{0,0},{0,1}}));
   assert(valid2({{0,2},{0,1}}));
   assert(disjoint({{0,0},{1,1}}));
   assert(not disjoint({{0,1},{1,2}}));

   assert(not is_star(0, {1}, G));
   G.set({{1,2},{0,1,2},{0,1,3,4},{2,3,4},{2,3}});
   // 0 -> 1,2
   // 1 -> 0,1,2
   // 2 -> 0,1,3,4
   // 3 -> 2,3,4
   // 4 -> 2,3
   assert(G.n() == 5);
   assert(G.m() == 8);
   assert(is_star(0, {1}, G));
   assert(is_star(0, {1,2}, G));
   assert(not is_star(0, {1,2,3}, G));
   assert(is_bc({{0}, {1,2}}, G));
   assert(is_bc({{1,2}, {0}}, G));
   assert(is_bc({{0,1}, {1,2}}, G));
   assert(not is_bc({{0,1}, {1,2,3}}, G));
   assert(not is_bc({{1,2,3},{0,1}}, G));
   assert(is_bc({{}, {1,2,3}}, G));
   assert(is_bc({{1,2,3}, {}}, G));
   assert(not covers(bc_frame{}, 0,0));
   assert(covers({{0},{0}}, 0,0));
   assert(not covers({{0},{0}}, 1,2));
   assert(covers({{0,1},{0,2}}, 1,2));
   assert(covers({{0,2},{0,1}}, 1,2));
   using v_t = Bcc_frame::v_t;
   assert(is_bc(v_t{{{0}, {1,2}}}, G));
   assert(is_bc(v_t{{{0}, {1,2}},{}}, G));
   assert(is_bc(v_t{{{0}, {1,2}},{},{{2,3},{3,4}}}, G));
   assert(covers(v_t{{{0}, {1,2}},{},{{2,3},{3,4}}}, 4, 3));
   assert(covers(v_t{{{0}, {1,2}},{},{{2,3},{3,4}}}, 3, 4));
   assert(not covers(v_t{{{0}, {1,2}},{},{{2,3},{3,4}}}, 1, 1));
   assert(not is_cover(v_t{{{0}, {1,2}},{},{{2,3},{3,4}}}, G));
  }

  {std::stringstream ss;
   ss << bc_frame{};
   assert(ss.str() == " | ");
   ss.str(""); ss << bc_frame{{1},{}};
   assert(ss.str() == "1 | ");
   ss.str(""); ss << bc_frame{{},{2}};
   assert(ss.str() == " | 2");
   ss.str(""); ss << bc_frame{{1},{2}};
   assert(ss.str() == "1 | 2");
   ss.str(""); ss << bc_frame{{3,1},{2,4}};
   assert(ss.str() == "3 1 | 2 4");

   ss.str(""); ss << Bcc_frame{};
   assert(ss.str() == "");
   ss.str(""); ss << Bcc_frame{{{},{},{}}};
   assert(ss.str() == " | \n | \n | \n");
   ss.str(""); ss << Bcc_frame{{{{1,2},{3,4}},{},{}}};
   assert(ss.str() == "1 2 | 3 4\n | \n | \n");
  }

  {assert(numcl({}) == 0);
   assert(numcl({{{},{},{}}}) == 0);
   assert(numcl({{{{0,1},{0,2}},{},{{0,3},{3,4}}}}) == 5);
  }

  {assert(eqp(bcc2CNF({},0), {}));
   assert(numocc({}) == 0);
   assert(eqp(bcc2CNF({{{},{},{}}}, 0), {{3,0},{}}));
   assert(disjoint(Bcc_frame{{{},{},{}}}));
   assert(numocc({{{},{},{}}}) == 0);
   assert(eqp(bcc2CNF({{{{0,1},{0,2}},{},{{0,3},{3,4}}}}, 5),
              {{3,5}, {{{1,1},{1,-1},{3,1}},
                       {{1,1}},
                       {{1,-1}},
                       {{3,1},{3,-1}},
                       {{3,-1}}}}));
   assert(numocc({{{{0,1},{0,2}},{},{{0,3},{3,4}}}}) == 8);
   assert(not disjoint(Bcc_frame{{{{0,1},{0,2}},{},{{0,3},{3,4}}}}));
   assert(disjoint(Bcc_frame{{{{0,1},{2,3}},{},{{0,3},{1,2}}}}));
  }

  {assert(eqp(CNF2bcc({}), {}));
   assert(eqp(CNF2bcc({{3,0},{}}), {{{},{},{}}}));
   assert(eqp(CNF2bcc({{3,5},
                       {{{1,1},{1,-1},{3,1}},
                       {{1,1}},
                       {{1,-1}},
                       {{3,1},{3,-1}},
                       {{3,-1}}}}),
              {{{{0,1},{0,2}},{},{{0,3},{3,4}}}}));
  }

  {Bcc_frame B{};
   assert(triv_trim(B) == 0);
   assert(eqp(B, {}));
   B.L.assign({{},{},{}});
   assert(triv_trim(B) == 3);
   assert(eqp(B, {}));
   B.L.assign({{{1,2},{}},{{},{2}},{},{{0},{1}}});
   assert(triv_trim(B) == 3);
   assert(eqp(B, {{{{0},{1}}}}));
  }

  {Bcc_frame B;
   assert(trim(B) == 0);
   assert(eqp(B,{}));
   B = Bcc_frame{{{},{},{}}};
   assert(trim(B) == 0);
   assert(eqp(B,{}));
   B.L.assign({{{1,2},{}},{{},{2}},{},{{0},{1}}});
   assert(trim(B) == 3);
   assert(eqp(B, {{{{0},{1}}}}));
   B.L.assign({{{1,2},{3}},{{1},{3}},{{2},{3}},{{0},{1}}});
   assert(trim(B) == 3);
   assert(eqp(B, {{{{1},{3}},{{2},{3}},{{0},{1}}}}));
   B.L.assign({{{1,2},{3,4}},{{1},{3}},{{2},{3}},{{0},{1}}});
   assert(trim(B) == 1);
   assert(eqp(B, {{{{1,2},{4}},{{1},{3}},{{2},{3}},{{0},{1}}}}));
  }

  {using graph_t = Graphs::AdjVecUInt;
   const auto G = graph_t(grid(2,2));
   assert(bccomp_graph_bydef(G, {}, "") == graph_t(Graphs::GT::und, 0));
   assert(num_edges_bccomp_graph_bydef(G, {}) == 0);
   assert(bccomp_graph<0>(G, {}, "") == graph_t(Graphs::GT::und, 0));
   assert(bccomp_graph<1>(G, {}, "") == graph_t(Graphs::GT::und, 0));
   assert(bccomp_graph<2>(G, {}, "") == graph_t(Graphs::GT::und, 0));
   assert(bccomp_graph<3>(G, {}, "") == graph_t(Graphs::GT::und, 0));
   assert(bccomp_graph_bydef(G, G.alledges(), "") == graph_t(clique(4)));
   assert(num_edges_bccomp_graph_bydef(G, G.alledges()) == 6);
   assert(bccomp_graph<0>(G, G.alledges(), "") == graph_t(clique(4)));
   assert(bccomp_graph<1>(G, G.alledges(), "") == graph_t(clique(4)));
   assert(bccomp_graph<2>(G, G.alledges(), "") == graph_t(clique(4)));
   assert(bccomp_graph<3>(G, G.alledges(), "") == graph_t(clique(4)));
  }
  {using graph_t = Graphs::AdjVecUInt;
   const auto G = graph_t(grid(2,3));
   assert(bccomp_graph_bydef(G, {}, "") == graph_t(Graphs::GT::und, 0));
   assert(num_edges_bccomp_graph_bydef(G, {}) == 0);
   assert(bccomp_graph<0>(G, {}, "") == graph_t(Graphs::GT::und, 0));
   assert(bccomp_graph<1>(G, {}, "") == graph_t(Graphs::GT::und, 0));
   assert(bccomp_graph<2>(G, {}, "") == graph_t(Graphs::GT::und, 0));
   assert(bccomp_graph<3>(G, {}, "") == graph_t(Graphs::GT::und, 0));
   const auto m = G.m();
   assert(m == 7);
   const auto BG = bccomp_graph_bydef(G);
   assert(BG == bccomp_graph<0>(G));
   assert(BG == bccomp_graph<1>(G));
   assert(BG == bccomp_graph<2>(G));
   assert(BG == bccomp_graph<3>(G));
   assert(BG.n() == m);
   assert(BG.m() == (m*(m-1))/2 - (2+2+3));
   assert(num_edges_bccomp_graph_bydef(G) == (m*(m-1))/2 - (2+2+3));
  }
  {using graph_t = Graphs::AdjVecUInt;
   const auto G = graph_t(clique(4));
   assert(bccomp_graph_bydef(G, {}, "") == graph_t(Graphs::GT::und, 0));
   assert(bccomp_graph<0>(G, {}, "") == graph_t(Graphs::GT::und, 0));
   assert(bccomp_graph<1>(G, {}, "") == graph_t(Graphs::GT::und, 0));
   assert(bccomp_graph<2>(G, {}, "") == graph_t(Graphs::GT::und, 0));
   assert(bccomp_graph<3>(G, {}, "") == graph_t(Graphs::GT::und, 0));
   const auto BG = bccomp_graph_bydef(G, G.alledges(), "");
   assert(BG == graph_t(clique(6)));
   assert(BG == bccomp_graph<0>(G, G.alledges(), ""));
   assert(BG == bccomp_graph<1>(G, G.alledges(), ""));
   assert(BG == bccomp_graph<2>(G, G.alledges(), ""));
   assert(BG == bccomp_graph<3>(G, G.alledges(), ""));
   assert(BG.m() == num_edges_bccomp_graph_bydef(G));
  }
  {using graph_t = Graphs::AdjVecUInt;
   for (idv_t n = 0; n <= 10; ++n) {
     const auto G = graph_t(clique(n));
     const auto BG = bccomp_graph_bydef(G);
     assert(BG == bccomp_graph<0>(G));
     assert(BG == bccomp_graph<1>(G));
     assert(BG == bccomp_graph<2>(G));
     assert(BG == bccomp_graph<3>(G));
     assert(is_complete(BG));
     const idv_t m = (n*(n-1))/2;
     assert(BG == graph_t(clique(m)));
     assert(BG.m() == num_edges_bccomp_graph_bydef(G));
   }
  }
  {using graph_t = Graphs::AdjVecUInt;
   for (idv_t n = 0; n <= 10; ++n) {
     const auto G = graph_t(biclique(n,n));
     const auto BG = bccomp_graph_bydef(G);
     assert(BG == bccomp_graph<0>(G));
     assert(BG == bccomp_graph<1>(G));
     assert(BG == bccomp_graph<2>(G));
     assert(BG == bccomp_graph<3>(G));
     assert(is_complete(BG));
     const idv_t m = n*n;
     assert(BG == graph_t(clique(m)));
     assert(BG.m() == num_edges_bccomp_graph_bydef(G));
   }
  }
  {using graph_t = Graphs::AdjVecUInt;
   for (idv_t n = 2; n <= 10; ++n) {
     const auto G = graph_t(grid(n,1));
     const auto BG = bccomp_graph_bydef(G);
     assert(BG == bccomp_graph<0>(G));
     assert(BG == bccomp_graph<1>(G));
     assert(BG == bccomp_graph<2>(G));
     assert(BG == bccomp_graph<3>(G));
     assert(BG == graph_t(grid(n-1,1)));
     assert(BG.m() == num_edges_bccomp_graph_bydef(G));
   }
  }
  {using graph_t = Graphs::AdjVecUInt;
   for (idv_t n = 3; n <= 10; ++n) {
     const auto G = graph_t(cycle(n));
     const auto BG = bccomp_graph_bydef(G);
     assert(BG == bccomp_graph<0>(G));
     assert(BG == bccomp_graph<1>(G));
     assert(BG == bccomp_graph<2>(G));
     assert(BG == bccomp_graph<3>(G));
     assert(BG.n() == n);
     if (n <= 4) assert(is_complete(BG));
     else assert(GraphTraversal::is_cyclegraph(BG));
     assert(BG.m() == num_edges_bccomp_graph_bydef(G));
   }
  }
  {using graph_t = Graphs::AdjVecUInt;
   for (idv_t n = 0; n <= 2; ++n) {
     const auto G = graph_t(crown(n));
     const auto BG = bccomp_graph_bydef(G);
     assert(BG == bccomp_graph<0>(G));
     assert(BG == bccomp_graph<1>(G));
     assert(BG == bccomp_graph<2>(G));
     assert(BG == bccomp_graph<3>(G));
     assert(BG.m() == 0);
     assert(BG.m() == num_edges_bccomp_graph_bydef(G));
   }
  }
  {std::istringstream in("A B C\nB E\n");
   const Graphs::AdjVecUInt G(make_AdjMapStr(in, Graphs::GT::und));
   const auto Gc = bccomp_graph_bydef(G, "-");
   assert(Gc.m() == num_edges_bccomp_graph_bydef(G));
   {const auto Gc2 = bccomp_graph<0>(G, "-");
    assert(Gc == Gc2);
    assert(Gc.with_names() == Gc2.with_names());
    assert(Gc.allnames() == Gc2.allnames());
   }
   {const auto Gc2 = bccomp_graph<1>(G, "-");
    assert(Gc == Gc2);
    assert(Gc.with_names() == Gc2.with_names());
    assert(Gc.allnames() == Gc2.allnames());
   }
   {const auto Gc2 = bccomp_graph<2>(G, "-");
    assert(Gc == Gc2);
    assert(Gc.with_names() == Gc2.with_names());
    assert(Gc.allnames() == Gc2.allnames());
   }
   {const auto Gc2 = bccomp_graph<3>(G, "-");
    assert(Gc == Gc2);
    assert(Gc.with_names() == Gc2.with_names());
    assert(Gc.allnames() == Gc2.allnames());
   }
   assert(Gc.n() == 3);
   assert(Gc.with_names());
   assert(eqp(Gc.allnames(), {"A-B", "A-C", "B-E"}));
   assert(eqp(Gc.allindices(), {{"A-B",0},{"A-C",1},{"B-E",2}}));
   assert(eqp(Gc.graph(), {{1,2},{0},{0}}));
  }
}
