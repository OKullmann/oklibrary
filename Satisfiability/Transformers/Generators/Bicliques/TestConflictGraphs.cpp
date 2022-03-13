// Oliver Kullmann, 27.2.2022 (Swansea)
/* Copyright 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#include <iostream>
#include <vector>
#include <algorithm>
#include <vector>
#include <sstream>

#include <cassert>

#include <ProgramOptions/Environment.hpp>

#include <Transformers/Generators/Random/Numbers.hpp>
#include <Transformers/Generators/Random/Distributions.hpp>

#include "DimacsTools.hpp"

#include "ConflictGraphs.hpp"
#include "Generators.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.2.1",
        "13.3.2022",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Bicliques/TestConflictGraphs.cpp",
        "GPL v3"};

  using namespace ConflictGraphs;
  using namespace DimacsTools;

  template <class X>
  constexpr bool eqp(const X& lhs, const X& rhs) noexcept {
    return lhs == rhs;
  }
}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv))
  return 0;

  {typedef std::vector<int> v_t;
   assert(empty_intersection(v_t{}, v_t{}));
   assert(empty_intersection(v_t{1}, v_t{}));
   assert(empty_intersection(v_t{}, v_t{1}));
   assert(not empty_intersection(v_t{1}, v_t{1}));
   assert(not empty_intersection(v_t{1}, v_t{0,1,2}));
   assert(not empty_intersection(v_t{0,1,2}, v_t{1}));
   const auto empty_int = [](const v_t& v1, const v_t& v2){
     v_t res;
     std::ranges::set_intersection(v1, v2, std::back_inserter(res));
     return res.empty();
   };
   RandGen::RandGen_t g({77});
   const auto size_dist = RandGen::UniformRange(g, 10);
   const auto val1_dist = RandGen::UniformRange(g, 10);
   const auto val2_dist = RandGen::UniformRange(g, 10, 5);
   for (unsigned i = 0; i < 10000; ++i) {
     const auto N1 = size_dist(); const auto N2 = size_dist();
     v_t v1; v1.reserve(N1); v_t v2; v2.reserve(N2);
     for (unsigned j = 0; j < N1; ++j) v1.push_back(val1_dist());
     for (unsigned j = 0; j < N2; ++j) v2.push_back(val2_dist());
     assert(empty_int(v1, v2) == empty_intersection(v1, v2));
   }
  }


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
   o[false].push_back(0);
   assert(eqp(o[false], {0}));
   assert(o[true].empty());
   assert(eqp(o.conflicts({77,1}), {0}));
   o = {{1,2}, {3,4}};
   assert(eqp(o[false], {1,2}));
   assert(eqp(o[true], {3,4}));
   assert(eqp(o, {{1,2}, {3,4}}));
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

}
