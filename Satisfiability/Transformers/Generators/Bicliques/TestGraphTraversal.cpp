// Oliver Kullmann, 12.3.2022 (Swansea)
/* Copyright 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#include <iostream>
#include <algorithm>

#include <cassert>

#include <ProgramOptions/Environment.hpp>

#include "GraphTraversal.hpp"
#include "Generators.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.1.0",
        "12.3.2022",
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
   assert(not valid(CCbyIndices(0,1)));
   assert(not valid(CCbyIndices(1,1)));
   CCbyIndices cc(1,1);
   assert(not valid(cc));
   cc.cv[0] = 1;
   assert(valid(cc));
  }

  {for (size_t n = 0; n <= 10; ++n) {
     const CCbyIndices res = cc_by_dfs(GR::AdjVecUInt(GR::GT::und, n));
     assert(valid(res));
     assert(res.numcc == n);
     assert(res.cv.size() == n);
   }
  }
  {for (size_t n = 0; n <= 10; ++n) {
     const CCbyIndices res = cc_by_dfs(GR::AdjVecUInt(GR::GT::dir, n));
     assert(valid(res));
     assert(res.numcc == n);
     assert(res.cv.size() == n);
   }
  }
  {for (size_t n = 0; n <= 10; ++n) {
      const CCbyIndices res = cc_by_dfs(GR::AdjVecUInt(Generators::clique(n)));
      assert(valid(res));
      assert(res.numcc == Generators::numcc_clique(n));
      assert(res.cv.size() == n);
    }
  }
  {for (size_t n = 0; n <= 5; ++n)
     for (size_t m = 0; m <= 5; ++m) {
       const CCbyIndices res =
         cc_by_dfs(GR::AdjVecUInt(Generators::biclique(n,m)));
       assert(valid(res));
       assert(res.numcc == Generators::numcc_biclique(n,m));
       assert(res.cv.size() == n+m);
     }
  }
  {for (size_t n = 0; n <= 5; ++n)
     for (size_t m = 0; m <= 5; ++m) {
       const CCbyIndices res =
         cc_by_dfs(GR::AdjVecUInt(Generators::grid(n,m)));
       assert(valid(res));
       assert(res.numcc == Generators::numcc_grid(n,m));
       assert(res.cv.size() == n*m);
     }
  }
  {for (size_t n = 0; n <= 10; ++n) {
     const CCbyIndices res = cc_by_dfs(GR::AdjVecUInt(Generators::crown(n)));
     assert(valid(res));
     assert(res.numcc == Generators::numcc_crown(n));
     assert(res.cv.size() == 2*n);
   }
  }

}
