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

#include "ConflictGraphs.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.1.0",
        "27.2.2022",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Bicliques/TestConflictGraphs.cpp",
        "GPL v3"};

  using namespace ConflictGraphs;

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

  {std::stringstream ss;
   ss.str("p cnf 0 0\n");
   assert(eqp(read_strict_dimacs_pars(ss), {0,0}));
   ss.str("p cnf 55 77\n");
   assert(eqp(read_strict_dimacs_pars(ss), {55,77}));
   ss.str("c\nc xx\np cnf 0 88\n");
   assert(eqp(read_strict_dimacs_pars(ss), {0,88}));
  }

  {std::stringstream ss;
   ss.str("0\n");
   assert((read_strict_literal(ss) == Lit{0,1}));
   ss.str("4 ");
   assert((read_strict_literal(ss) == Lit{4,1}));
   ss.str("-77 ");
   assert((read_strict_literal(ss) == Lit{77,-1}));
  }

  {std::stringstream ss;
   ss.str("0\n");
   assert(eqp(read_strict_clause(ss), {}));
   ss.str("3 -4 5 -6 0\n");
   assert(eqp(read_strict_clause(ss), {Lit{3,1}, Lit{4,-1}, Lit{5,1}, Lit{6,-1}}));
  }

  {std::stringstream ss;
   ss.str("p cnf 0 0\n");
   assert(eqp(read_strict_Dimacs(ss), {{0,0},{}}));
   ss.str("p cnf 5 0\n");
   assert(eqp(read_strict_Dimacs(ss), {{5,0},{}}));
   ss.str("p cnf 5 1\n0\n");
   assert(eqp(read_strict_Dimacs(ss), {{5,1},{{}}}));
   ss.str("p cnf 5 1\n1 2 -3 0\n");
   assert(eqp(read_strict_Dimacs(ss), {{5,1},{{Lit{1,1}, Lit{2,1}, Lit{3,-1}}}}));
   ss.str("p cnf 5 2\n1 2 -3 0\n0\n");
   assert(eqp(read_strict_Dimacs(ss), {{5,2},{{Lit{1,1}, Lit{2,1}, Lit{3,-1}},{}}}));
   ss.str("p cnf 5 2\n1 2 -3 0\n-4 5 0\n");
   assert(eqp(read_strict_Dimacs(ss), {{5,2},{{Lit{1,1}, Lit{2,1}, Lit{3,-1}},{Lit{4,-1},Lit{5,1}}}}));
   ss.str("p cnf 5 3\n1 2 -3 0\n0\n-4 5 0\n");
   assert(eqp(read_strict_Dimacs(ss), {{5,3},{{Lit{1,1}, Lit{2,1}, Lit{3,-1}},{},{Lit{4,-1},Lit{5,1}}}}));
   ss.str("p cnf 5 4\n1 2 -3 0\n0\n-4 5 0\n0\n");
   assert(eqp(read_strict_Dimacs(ss), {{5,4},{{Lit{1,1}, Lit{2,1}, Lit{3,-1}},{},{Lit{4,-1},Lit{5,1}},{}}}));
   ss.str("p cnf 5 4\n1 2 -3 0\n1 5 0\n-4 5 0\n0\n");
   assert(eqp(read_strict_Dimacs(ss), {{5,4},{{Lit{1,1}, Lit{2,1}, Lit{3,-1}},{Lit{1,1},Lit{5,1}},{Lit{4,-1},Lit{5,1}},{}}}));
  }
}
