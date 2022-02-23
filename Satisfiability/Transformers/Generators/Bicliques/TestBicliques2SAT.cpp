// Oliver Kullmann, 22.2.2022 (Swansea)
/* Copyright 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#include <iostream>
#include <sstream>

#include <cassert>

#include <ProgramOptions/Environment.hpp>

#include "Bicliques2SAT.hpp"
#include "Graphs.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.0.3",
        "23.2.2022",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Bicliques/TestBicliques2SAT.cpp",
        "GPL v3"};

  using namespace Bicliques2SAT;
  using namespace Graphs;

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

   VarEncoding enc(G, 1);
   for (unsigned i = 0; i < 4; ++i)
     assert(enc.left(i,0) == 1+i);
   for (unsigned i = 0; i < 4; ++i)
     assert(enc.right(i,0) == 5+i);
   for (unsigned i = 0; i < 6; ++i)
     assert(enc.edge(i,0) == 9+i);
   assert(enc.V == 4);
   assert(enc.E == 6);
   assert(enc.B == 1);
   assert(enc.nb == 8);
   assert(enc.ne == 6);
   assert(enc.n == 14);

   VarEncoding enc2(G, 2);
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
   assert(enc2.V == 4);
   assert(enc2.E == 6);
   assert(enc2.B == 2);
   assert(enc2.nb == 16);
   assert(enc2.ne == 12);
   assert(enc2.n == 28);

   BC2SAT trans1(G, 1);
   for (unsigned e1 = 0; e1 < G.m(); ++e1)
     for (unsigned e2 = 0; e2 < G.m(); ++e2)
       assert(trans1.bccomp(e1,2));
  }

  {AdjMapStr G(GT::und);
   G.add_clique(std::vector{"a", "b", "c", "d"});
   G.add_clique(std::vector{"e", "f", "g", "h"});
   AdjVecUInt Ga(G);

   {BC2SAT trans(Ga, 2);
    assert(trans.enc.V == 8);
    assert(trans.enc.E == 12);

    for (unsigned i = 0; i < 12; ++i)
      for (unsigned j = 0; j < 12; ++j)
        assert(trans.bccomp(i,j) == ((i<6 and j<6) or (i>=6 and j>=6)));
   }

   assert(eqp(G.add_clique(std::vector{"a","b","e","f"}), {0,4}));
   Ga = AdjVecUInt(G);
   BC2SAT trans(Ga,2);
   assert(eqp(trans.edges[0], {0,1}));
   assert(eqp(trans.edges[3], {0,4}));
   assert(eqp(trans.edges[5], {1,2}));
   assert(not trans.bccomp(3,5));
   assert(not trans.bccomp(5,3));
  }


}
