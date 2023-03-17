// Oliver Kullmann, 12.3.2022 (Swansea)
/* Copyright 2022, 2023 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#include <iostream>
#include <algorithm>
#include <sstream>

#include <cassert>

#include <ProgramOptions/Environment.hpp>

#include "GraphTraversal.hpp"
#include "Generators.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.1.2",
        "17.3.2023",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Bicliques/TestGraphTraversal.cpp",
        "GPL v3"};

  using namespace GraphTraversal;

  template <class X>
  constexpr bool eqp(const X& lhs, const X& rhs) noexcept {
    return lhs == rhs;
  }
}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv))
  return 0;

  {assert(valid(CCbyIndices()));
   assert(CCbyIndices().canonical());
   assert(not valid(CCbyIndices(0,1)));
   assert(not valid(CCbyIndices(1,1)));
   CCbyIndices cc(1,1);
   assert(not cc.canonical());
   assert(not valid(cc));
   cc.cv[0] = 1;
   assert(valid(cc));
   assert(cc.canonical());
  }

  {for (size_t n = 0; n <= 10; ++n) {
     const CCbyIndices res = cc_by_dfs(GR::AdjVecUInt(GR::GT::und, n));
     assert(valid(res));
     assert(res.canonical());
     assert(res.numcc == n);
     assert(res.cv.size() == n);
   }
  }
  {for (size_t n = 0; n <= 10; ++n) {
     const CCbyIndices res = cc_by_dfs(GR::AdjVecUInt(GR::GT::dir, n));
     assert(valid(res));
     assert(res.canonical());
     assert(res.numcc == n);
     assert(res.cv.size() == n);
   }
  }
  {for (size_t n = 0; n <= 10; ++n) {
      const CCbyIndices res = cc_by_dfs(GR::AdjVecUInt(Generators::clique(n)));
      assert(valid(res));
      assert(res.canonical());
      assert(res.numcc == Generators::numcc_clique(n));
      assert(res.cv.size() == n);
    }
  }
  {for (size_t n = 0; n <= 5; ++n)
     for (size_t m = 0; m <= 5; ++m) {
       const CCbyIndices res =
         cc_by_dfs(GR::AdjVecUInt(Generators::biclique(n,m)));
       assert(valid(res));
       assert(res.canonical());
       assert(res.numcc == Generators::numcc_biclique(n,m));
       assert(res.cv.size() == n+m);
     }
  }
  {for (size_t n = 0; n <= 5; ++n)
     for (size_t m = 0; m <= 5; ++m) {
       const CCbyIndices res =
         cc_by_dfs(GR::AdjVecUInt(Generators::grid(n,m)));
       assert(valid(res));
       assert(res.canonical());
       assert(res.numcc == Generators::numcc_grid(n,m));
       assert(res.cv.size() == n*m);
     }
  }
  {for (size_t n = 0; n <= 10; ++n) {
     const CCbyIndices res = cc_by_dfs(GR::AdjVecUInt(Generators::crown(n)));
     assert(valid(res));
     assert(res.canonical());
     assert(res.numcc == Generators::numcc_crown(n));
     assert(res.cv.size() == 2*n);
   }
  }

  {GR::AdjMapStr G0(GR::GT::und);
   std::istringstream is;
   is.str("a b c\nd c\ne f\nf g\n");
   G0.insert(is);
   GR::AdjVecUInt G(G0);
   const CCbyIndices CC = cc_by_dfs(G);
   assert(valid(CC));
   assert(CC.canonical());
   assert(CC.numcc == 2);
   assert(eqp(CC.cv, {1,1,1,1,2,2,2}));
   assert(eqp(CC.sizes(), {4,3}));
   assert(eqp(CC.components(), {{0,1,2,3},{4,5,6}}));
   assert(eqp(CC.components({4,3}), {{0,1,2,3},{4,5,6}}));
  }

  {CCbyIndices CC{{1,1,2,3,2,1,2,3,4,4},4};
   assert(valid(CC));
   assert(CC.canonical());
   CC.cv[2]=3;
   assert(valid(CC));
   assert(not CC.canonical());
  }

}
