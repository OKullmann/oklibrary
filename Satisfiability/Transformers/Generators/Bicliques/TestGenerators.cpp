// Oliver Kullmann, 26.2.2022 (Swansea)
/* Copyright 2022, 2023 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#include <iostream>
#include <algorithm>

#include <cassert>

#include <ProgramOptions/Environment.hpp>

#include "Generators.hpp"
#include "ConflictGraphs.hpp"
#include "Graphs.hpp"
#include "GraphTraversal.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.2.1",
        "23.6.2023",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Bicliques/TestGenerators.cpp",
        "GPL v3"};

  using namespace Generators;

  template <class X>
  constexpr bool eqp(const X& lhs, const X& rhs) noexcept {
    return lhs == rhs;
  }
}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv))
  return 0;

  {const auto G = clique(0);
   assert(G.n() == 0);
   assert(G.m() == 0);
  }
  {const auto G = clique(1);
   assert(G.n() == 1);
   assert(G.m() == 0);
   assert(eqp(G.graph(), {{"1",{}}}));
  }
  {const auto G = clique(2);
   assert(G.n() == 2);
   assert(G.m() == 1);
   assert(eqp(G.graph(), {{"1",{"2"}},{"2",{"1"}}}));
  }
  {const auto G = clique(3);
   assert(G.n() == 3);
   assert(G.m() == 3);
   assert(eqp(G.graph(), {{"1",{"2","3"}},{"2",{"1","3"}},{"3",{"1","2"}}}));
  }

  {assert(bcc_clique(0) == 0);
   assert(bcc_clique(1) == 0);
   assert(bcc_clique(2) == 1);
   assert(bcc_clique(3) == 2);
   assert(bcc_clique(4) == 2);
   assert(bcc_clique(5) == 3);
   assert(bcc_clique(32) == 5);
   assert(bcc_clique(33) == 6);
  }

  {for (unsigned n = 0; n < 10; ++n)
    for (unsigned m = 0; m < 10; ++m) {
      const auto G = biclique(n,m);
      assert(G.n() == n+m);
      assert(G.m() == n * m);
      assert(bcc_biclique(n,m) == std::min(n*m, 1u));
      const Graphs::AdjVecUInt GU(G);
      if (n == 0 or m == 0) {
        assert(GU.regular(0));
        assert(GU.regular());
        assert(GraphTraversal::is_connected(GU) == (n+m <= 1));
        assert(not GraphTraversal::is_cyclegraph(GU));
      }
      else {
        assert(GraphTraversal::is_connected(GU));
        assert(GraphTraversal::is_cyclegraph(GU) == (n==2 and m==2));
        if (n != m)
          assert(not GU.regular());
        else {
          assert(GU.regular(n));
          assert(GU.regular());
        }
      }
    }
  }

  {assert(bcc_grid(0,0) == 0);
   assert(bcc_grid(0,5) == 0);
   assert(bcc_grid(5,0) == 0);
   assert(bcc_grid(1,3) == 1);
   assert(bcc_grid(3,1) == 1);
   assert(bcc_grid(2,2) == 1);
   assert(bcc_grid(2,3) == 2);
   assert(bcc_grid(3,2) == 2);
  }

  {assert(eqp(acnf(0), {{0,1},{{}}}));
   using L = DT::Lit;
   assert(eqp(acnf(1), {{1,2},{{L(1,1)},{L(1,-1)}}}));
   assert(eqp(acnf(2), {{2,4},{{L(1,1),L(2,1)},{L(1,-1),L(2,1)},{L(1,1),L(2,-1)},{L(1,-1),L(2,-1)}}}));
  }

  {assert(eqp(cnf_clique(0), {}));
   assert(eqp(cnf_clique(1), {{0,1},{{}}}));
   using L = DT::Lit;
   assert(eqp(cnf_clique(2), {{1,2},{{L(1,1)},{L(1,-1)}}}));
   assert(eqp(cnf_clique(3), {{2,3},{{L(1,1),L(2,1)},{L(1,-1),L(2,1)},{L(1,1),L(2,-1)}}}));
  }

  {for (size_t n = 0; n <= 65; ++n) {
     const auto F = cnf_clique(n);
     assert(valid(F));
     const Graphs::AdjVecUInt G(clique(n));
     assert(ConflictGraphs::conflictgraph_bydef(F) == G);
     assert(ConflictGraphs::conflictgraph(F) == G);
   }
  }

  for (size_t n = 3; n <= 10; ++n) {
    const auto G = cycle(n);
    assert(G.n() == n);
    assert(G.m() == n);
    const Graphs::AdjVecUInt GU(G);
    assert(GraphTraversal::is_cyclegraph(GU));
  }

}
