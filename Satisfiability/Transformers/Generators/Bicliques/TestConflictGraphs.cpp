// Oliver Kullmann, 27.2.2022 (Swansea)
/* Copyright 2022, 2023 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iterator>

#include <cassert>

#include <ProgramOptions/Environment.hpp>

#include <Transformers/Generators/Random/Numbers.hpp>
#include <Transformers/Generators/Random/Distributions.hpp>

#include "DimacsTools.hpp"

#include "ConflictGraphs.hpp"
#include "Generators.hpp"
#include "Bicliques2SAT.hpp"
#include "Algorithms.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.3.3",
        "19.3.2023",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Bicliques/TestConflictGraphs.cpp",
        "GPL v3"};

  using namespace ConflictGraphs;
  using namespace DimacsTools;
  using namespace Algorithms;

  template <class X>
  constexpr bool eqp(const X& lhs, const X& rhs) noexcept {
    return lhs == rhs;
  }
}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv))
  return 0;

  {Clause C;
   assert(eqp(ewcompl(C), {}));
   C.push_back(Lit{1,1});
   assert(eqp(ewcompl(C), {Lit{1,-1}}));
  }
  {ClauseList F;
   assert(eqp(ewcompl(F), {}));
   F.push_back({Lit{5,1}, Lit{3,-1}, Lit{0,2}});
   assert(eqp(ewcompl(F), {{-5, 3, Lit{0,-1}}}));
  }

  {std::stringstream ss;
   ss.str("p cnf 0 0\n");
   const auto F = read_strict_Dimacs(ss);
   const auto G = conflictgraph_bydef(F);
   assert(G.n() == 0);
   assert(G.m() == 0);
   assert(not G.with_names());
   assert(G == conflictgraph(F));
  }

  {std::stringstream ss;
   ss.str("p cnf 0 3\n0\n0\n0\n");
   const auto F = read_strict_Dimacs(ss);
   const auto G = conflictgraph_bydef(F);
   assert(G.n() == 3);
   assert(G.m() == 0);
   assert(not G.with_names());
   assert(G == conflictgraph(F));
  }

  {std::stringstream ss;
   ss.str("p cnf 3 3\n1 0\n1 0\n-1 0\n");
   const auto F = read_strict_Dimacs(ss);
   const auto G = conflictgraph_bydef(F);
   assert(G.n() == 3);
   assert(G.m() == 2);
   assert(not G.with_names());
   assert(eqp(G.graph(), {{2},{2},{0,1}}));
   assert(G == conflictgraph(F));
  }

  {OccVar o{};
   assert(o.o.size() == 2);
   assert(o[false].empty());
   assert(o[true].empty());
   assert(o.trivial());
   assert(not o.pure());
   assert(not o.singular());
   assert(not o.osingular());
   assert(not o.nonosingular());
   assert(not o.nonsingular());
   assert(o.deg() == 0);
   assert(o.muldeg() == 0);
   assert(o.maxdeg() == 0);
   assert(o.mindeg() == 0);
   assert(o.deg(false) == 0);
   assert(o.deg(true) == 0);
   assert(o.deg(Lit(1)) == 0);
   assert(o.deg(-Lit(1)) == 0);
   o[false].push_back(0);
   assert(eqp(o[false], {0}));
   assert(o[true].empty());
   assert(eqp(o.conflicts({77,1}), {0}));
   assert(not o.trivial());
   assert(o.pure());
   assert(not o.singular());
   assert(not o.osingular());
   assert(not o.nonosingular());
   assert(not o.nonsingular());
   assert(o.deg() == 1);
   assert(o.muldeg() == 0);
   assert(o.maxdeg() == 1);
   assert(o.mindeg() == 0);
   assert(o.deg(false) == 1);
   assert(o.deg(true) == 0);
   assert(o.deg(Lit(1)) == 0);
   assert(o.deg(-Lit(1)) == 1);
   o = {{1}, {3}};
   assert(not o.trivial());
   assert(not o.pure());
   assert(o.singular());
   assert(o.osingular());
   assert(not o.nonosingular());
   assert(not o.nonsingular());
   assert(o.deg() == 2);
   assert(o.muldeg() == 1);
   assert(o.maxdeg() == 1);
   assert(o.mindeg() == 1);
   assert(o.deg(false) == 1);
   assert(o.deg(true) == 1);
   assert(o.deg(Lit(1)) == 1);
   assert(o.deg(-Lit(1)) == 1);
   o = {{1}, {3,4}};
   assert(not o.trivial());
   assert(not o.pure());
   assert(o.singular());
   assert(not o.osingular());
   assert(o.nonosingular());
   assert(not o.nonsingular());
   assert(o.deg() == 3);
   assert(o.muldeg() == 2);
   assert(o.maxdeg() == 2);
   assert(o.mindeg() == 1);
   assert(o.deg(false) == 1);
   assert(o.deg(true) == 2);
   assert(o.deg(Lit(1)) == 2);
   assert(o.deg(-Lit(1)) == 1);
   o = {{1,2}, {4}};
   assert(not o.trivial());
   assert(not o.pure());
   assert(o.singular());
   assert(not o.osingular());
   assert(o.nonosingular());
   assert(not o.nonsingular());
   assert(not o.trivial());
   assert(not o.pure());
   assert(o.singular());
   assert(not o.osingular());
   assert(o.nonosingular());
   assert(not o.nonsingular());
   assert(o.deg() == 3);
   assert(o.muldeg() == 2);
   assert(o.maxdeg() == 2);
   assert(o.mindeg() == 1);
   assert(o.deg(false) == 2);
   assert(o.deg(true) == 1);
   assert(o.deg(Lit(1)) == 1);
   assert(o.deg(-Lit(1)) == 2);
   o = {{1,2}, {3,4}};
   assert(not o.trivial());
   assert(not o.pure());
   assert(not o.singular());
   assert(not o.osingular());
   assert(not o.nonosingular());
   assert(o.nonsingular());
   assert(o.deg() == 4);
   assert(o.muldeg() == 4);
   assert(o.maxdeg() == 2);
   assert(o.mindeg() == 2);
   assert(o.deg(false) == 2);
   assert(o.deg(true) == 2);
   assert(o.deg(Lit(1)) == 2);
   assert(o.deg(-Lit(1)) == 2);
  }

  {assert(eqp(allocc({}), {}));
   for (var_t n = 0; n <= 5; ++n)
     assert(eqp(allocc({{n,0},{}}), AllOcc(n)));
   for (var_t n = 0; n <= 5; ++n)
     for (var_t m = 0; m <= 5; ++m)
       assert(eqp(allocc({{n,m},ClauseList(m)}), AllOcc(n)));
   assert(eqp(allocc({{1,4},{{},{1},{-1},{1}}}),
              { {{{2},{1,3}}} }));
   assert(eqp(allocc({{2,5},{{},{1},{-1},{1, -2},{1}}}),
              { { {{2},{1,3,4}}, {{3},{}} } }));
  }

  {using DCL = DimacsClauseList;
   using CC = GraphTraversal::CCbyIndices;
   assert(eqp(cc_by_dfs(DCL{}), CC{}));
   assert(eqp(cc_by_dfs(DCL{{0,2},{{},{}}}), CC({1,2}, 2)));
   assert(eqp(cc_by_dfs(DCL{{10,3},{{},{},{}}}), CC({1,2,3}, 3)));
   for (var_t n = 0; n <= 10; ++n)
     assert(cc_by_dfs(Generators::cnf_clique(n)).numcc == std::min(n,var_t(1)));
   assert(eqp(cc_by_dfs(DCL{{10,3},{{1},{-1},{2}}}), CC({1,1,2}, 2)));
   assert(eqp(cc_by_dfs(DCL{{10,3},{{1},{-1},{2}}}), CC({1,1,2}, 2)));
   assert(eqp(cc_by_dfs(DCL{{10,4},{{1,2},{-1},{-2},{5}}}), CC({1,1,1,2}, 2)));
   assert(eqp(cc_by_dfs(DCL{{10,5},{{-5},{1,2},{-1},{-2},{5}}}), CC({1,2,2,2,1}, 2)));
  }

  {std::stringstream ss;
   ss.str("p cnf 2 8\n"
          "2 0\n"
          "-2 0\n"
          "2 0\n"
          "1 0\n"
          "-2 -1 0\n"
          "2 0\n"
          "1 0\n"
          "-2 -1 0\n");
   const DimacsClauseList F = read_strict_Dimacs(ss);
   const auto FC = ewcompl(F.second);
   assert(empty_intersection(F.second[3], FC[4]));
   const auto G1 = conflictgraph_bydef(F);
   const auto G2 = conflictgraph(F);
   assert(G1 == G2);
  }

  {using GRT = Graphs::AdjVecUInt;
   const auto und = Graphs::GT::und;
   std::istringstream is;
   is.str("p cnf 10 5\n"
          "a 3 7 5 8 0\n"
          "e  2 1  0\n"
          "a 4  9 10 6 0\n"
          "-1  10 0\n"               // 0 purely other           cc=1
          " -5 -3 0\n"               // 1 purely global : -3 -5  cc=2
          "-8 4 -10  3 0\n"          // 2 mixed         : 3 -8   cc=2
          "-7  10 -1  6 0\n"         // 3 mixed         : -7     cc=3
          "4 -9 7 0\n");             // 4 mixed         : 7      cc=3
   const GslicedCNF F = read_strict_GslicedCNF(is);
   const Bicliques2SAT::GlobRepl GR(F);
   assert(eqp(GR.F.G().first, {8,5}));
   assert(GR.numntcc == 2);
   assert(eqp(GR.ntcc, {2,3}));
   {const GRT G = conflictgraph({0,1,2,3,4}, {Var(3),Var(5),Var(7),Var(8)},
                                GR.occ);
    assert(G == conflictgraph(F.G()));
   }
   {const GRT G = conflictgraph({}, {Var(3),Var(5),Var(7),Var(8)},
                                GR.occ);
    assert(G == GRT(und));
   }
   {const GRT G = conflictgraph({0,1,2,3,4}, {},
                                GR.occ);
    assert(G == GRT(und,5));
   }
   {const GRT G = conflictgraph({1,2}, {Var(3),Var(5),Var(7),Var(8)},
                                GR.occ);
    assert(G == GRT(Generators::clique(2)));
    assert(G == GR.conflictgraph(0));
   }
   {const GRT G = conflictgraph({1,2}, {Var(3)},
                                GR.occ);
    assert(G == GRT(Generators::clique(2)));
    assert(G == GR.conflictgraph(0));
   }
   {const GRT G = conflictgraph({1,2}, {Var(5)},
                                GR.occ);
    assert(G == GRT(und, 2));
   }
   {const GRT G = conflictgraph({3,4}, {Var(7)},
                                GR.occ);
    assert(G == GRT(Generators::clique(2)));
    assert(G == GR.conflictgraph(1));
   }
  }

}
