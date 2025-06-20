// Oliver Kullmann, 13.3.2022 (Swansea)
/* Copyright 2022, 2025 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#include <iostream>
#include <sstream>
#include <vector>

#include <cassert>

#include <Transformers/Generators/Bicliques/TestTools.hpp>
#include <ProgramOptions/Environment.hpp>

#include "VarLit.hpp"
#include "GenLit.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.2.0",
        "5.6.2025",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Random/TestVarLit.cpp",
        "GPL v3"};

  using namespace TestTools;
  using namespace RandGen;
  using namespace GenLit;

  std::vector<VarVal> read_genlits(const std::string& s) {
    std::istringstream ss(s);
    std::vector<VarVal> res;
    for (VarVal v; not ss.eof(); ss >> v, res.push_back(v));
    assert(all_read(ss));
    return res;
  }
  std::vector<VarVal> read_optgenlits(const std::string& s) {
    std::istringstream ss(s);
    std::vector<VarVal> res;
    for (optVarVal ov; ss >> ov, ov; res.push_back(ov.value()));
    assert(all_read(ss));
    return res;
  }
}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv))
  return 0;

  {std::ostringstream ss;
   ss << Var(0) << " " << Var(2);
   assert(ss.str() == "0 2");
   ss.str("");
   ss << Lit(0,0) << " " << Lit(0,1) << " " << Lit(1,-1) << " " << Lit(1,1);
   assert(ss.str() == "-0 0 -1 1");
  }
  {const auto [v,s] = Lit(5,0);
   assert(v == Var(5));
   assert(s == false);
  }

  {std::ostringstream ss;
   ss << VarVal{} << " " << VarVal{7,11};
   assert(ss.str() == "0:0 7:11");
  }
  {VarVal v; // left uninitialised
   // assert(v != VarVal{}); // holds "mostly"
   v = VarVal{66};
   assert((v == VarVal{66,0}));
   const VarVal w(v);
   assert((w == VarVal{66,0}));
   const auto [a,b] = w;
   assert(a == 66); assert(b == 0);
  }
  {const std::vector<VarVal> V(5);
   assert(eqp(V, {{},{},{},{},{}}));
  }

  assert(eqp(read_genlits("0:0 5:77 -1:-1"), {{}, {5,77}, totsingvv}));
  assert(eqp(read_optgenlits(" 0:3 \n 3:66  -2:-2 0"), {{0,3}, {3,66}, {singvar-1,singval-1}}));

  {assert(eqp(std::ranges::to<std::vector<VarVal>>(
                  make_VarVal_range(3,4,6)),
              {{3,4},{3,5}}));
   assert(std::is_sorted(VarVal_it({0,3}), VarVal_it({0,10})));
   const std::vector<VarVal> C(VarVal_it({2,4}), VarVal_it({2,7}));
   assert(eqp(C, {{2,4},{2,5},{2,6}}));
  }

}
