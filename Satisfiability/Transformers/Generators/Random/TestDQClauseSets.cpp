// Oliver Kullmann, 17.7.2019 (Swansea)
/* Copyright 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#include <iostream>

#include <cassert>

#include <ProgramOptions/Environment.hpp>

#include "Numbers.hpp"
#include "DQClauseSets.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.1.0",
        "26.10.2019",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Random/TestDQClauseSets.cpp",
        "GPL v3"};

  using namespace RandGen;

  std::ostream& operator <<(std::ostream& out, const ae_pair p) {
    return out << "(" << p.first << "," << p.second << ")";
  }
  [[maybe_unused]] std::ostream& operator <<(std::ostream& out, const dep_edges& depe) {
    for (const auto e : depe) out << " " << e;
    return out;
  }

}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv))
    return 0;

  {assert((extract_numvars({{2,Q::both},{1,Q::fa},{{2,2},Q::ex}}) == ae_numvars{1,1}));
   assert((extract_numvars({{7,Q::both},{2,Q::fa},{{3,5},Q::ex},{{6,6},Q::fa},{{7,7},Q::ex}}) == ae_numvars{3,4}));
  }

  {assert((num_dependencies({{1,Q::ex},{1,Q::ex}}) == 0));
   assert((num_dependencies({{10,Q::ex},{1,Q::ex},{{2,10},Q::ex}}) == 0));
   assert((num_dependencies({{10,Q::both},{9,Q::fa},{{10,10},Q::ex}}) == 9));
   assert((num_dependencies({{20,Q::both},{9,Q::fa},{{10,10},Q::ex},{{11,15},Q::ex},{{16,16},Q::fa},{{17,20},Q::ex}}) == 6*9 + 4*10));
  }

  {assert((extract(block_v{{1,Q::ex},{1,Q::ex}}) == ablock_v{}));
   assert((extract(block_v{{10,Q::ex},{1,Q::ex},{{2,10},Q::ex}}) == ablock_v{}));
   assert((extract(block_v{ {21,Q::both},{9,Q::fa},{{10,10},Q::ex},{{11,15},Q::ex},{{16,16},Q::fa},{{17,20},Q::fa}, {{21,21},Q::ex} }) == ablock_v{ {{1,9},{1,9}}, {{16,16},{10,10}}, {{17,20},{11,14}} }));
  }

  {AccessA aa(block_v{{1,Q::ex},{1,Q::ex}});
   assert(aa.max == 0);
   assert(aa.na == 0);
   assert(aa(0) == 0);
   assert(aa[0] == 0);
  }
  {AccessA aa(block_v{ {21,Q::both},{9,Q::fa},{{10,10},Q::ex},{{11,15},Q::ex},{{16,16},Q::fa},{{17,20},Q::fa}, {{21,21},Q::ex} });
   assert(aa.max == 20);
   assert(aa.na == 14);
   assert(aa(0) == 14);
   assert(aa(1) == 1);
   assert(aa(9) == 9);
   assert(aa(16) == 10);
   assert(aa(17) == 11);
   assert(aa(20) == 14);
   assert(aa[0] == 20);
   assert(aa[1] == 1);
   assert(aa[9] == 9);
   assert(aa[10] == 16);
   assert(aa[11] == 17);
   assert(aa[14] == 20);
  }
  {assert((translate(vec_eseed_t{}, 0, 1, block_v{ {1,Q::ex},{1,Q::ex} }, DepOp::from_scratch) == dep_edges{}));
   assert((translate(vec_eseed_t{}, 1, 1, block_v{ {2,Q::both},{1,Q::fa},{{2,2},Q::ex} }, DepOp::from_scratch) == dep_edges{}));
   assert((translate(vec_eseed_t{0}, 1, 1, block_v{ {2,Q::both},{1,Q::fa},{{2,2},Q::ex} }, DepOp::from_scratch) == dep_edges{{1,0}}));
   assert((translate(vec_eseed_t{0,3}, 2, 2, block_v{ {4,Q::both},{2,Q::fa},{{3,4},Q::ex} }, DepOp::from_scratch) == dep_edges{{1,0}, {2,1}}));
   assert((translate(vec_eseed_t{0,1,3,4,22,24}, 5, 5, block_v{ {10,Q::both},{2,Q::fa},{{3,4},Q::ex},{{5,7},Q::fa},{{8,10},Q::ex} }, DepOp::from_scratch) == dep_edges{{1,0},{2,0},{6,0},{7,0}, {5,4},{7,4}}));
   assert((translate(vec_eseed_t{0,1,3,4,22,24}, 5, 8, block_v{ {13,Q::both},{3,Q::ex},{{4,5},Q::fa},{{6,7},Q::ex},{{8,10},Q::fa},{{11,13},Q::ex} }, DepOp::from_scratch) == dep_edges{{4,0},{5,0},{9,0},{10,0}, {8,4},{10,4}}));

   assert((translate(vec_eseed_t{}, 0, 1, block_v{ {1,Q::ex},{1,Q::ex} }, DepOp::subtract) == dep_edges{}));
   assert((translate(vec_eseed_t{}, 1, 1, block_v{ {2,Q::both},{1,Q::fa},{{2,2},Q::ex} }, DepOp::subtract) == dep_edges{}));
   assert((translate(vec_eseed_t{0}, 1, 1, block_v{ {2,Q::both},{1,Q::fa},{{2,2},Q::ex} }, DepOp::subtract) == dep_edges{{1,0}}));
   assert((translate(vec_eseed_t{0,3}, 2, 2, block_v{ {4,Q::both},{2,Q::fa},{{3,4},Q::ex} }, DepOp::subtract) == dep_edges{{1,0}, {2,1}}));
   assert((translate(vec_eseed_t{0,1,3,4,15,18}, 5, 5, block_v{ {10,Q::both},{2,Q::fa},{{3,4},Q::ex},{{5,7},Q::fa},{{8,10},Q::ex} }, DepOp::subtract) == dep_edges{{1,0},{2,0},{2,1}, {1,2},{2,4},{7,4}}));
   assert((translate(vec_eseed_t{14}, 5, 5, block_v{ {10,Q::both},{2,Q::fa},{{3,4},Q::ex},{{5,7},Q::fa},{{8,10},Q::ex} }, DepOp::subtract) == dep_edges{{1,4}}));
   assert((translate(vec_eseed_t{0,1,3,4,15,18}, 5, 7, block_v{ {12,Q::both},{2,Q::ex},{{3,4},Q::fa},{{5,6},Q::ex},{{7,9},Q::fa},{{10,12},Q::ex} }, DepOp::subtract) == dep_edges{{3,2},{4,2},{4,3}, {3,4},{4,6},{9,6}}));
   assert((translate(vec_eseed_t{0,49}, 8, 9, block_v{ {17,Q::both},{2,Q::ex},{{3,7},Q::fa}, {{8,9},Q::ex}, {{10,12},Q::fa}, {{13,17},Q::ex} }, DepOp::subtract) == dep_edges{{3,2},{12,8}}));

   assert((translate(vec_eseed_t{}, 0, 1, block_v{ {1,Q::ex},{1,Q::ex} }, DepOp::add) == dep_edges{}));
   assert((translate(vec_eseed_t{}, 1, 1, block_v{ {2,Q::both},{1,Q::fa},{{2,2},Q::ex} }, DepOp::add) == dep_edges{}));
   assert((translate(vec_eseed_t{0}, 1, 2, block_v{ {3,Q::both},{1,Q::ex},{{2,2},Q::fa},{{3,3},Q::ex} }, DepOp::add) == dep_edges{{2,0}}));
   assert((translate(vec_eseed_t{0,4}, 4, 6, block_v{ {10,Q::both},{2,Q::fa},{{3,7},Q::ex},{{8,9},Q::fa},{{10,10},Q::ex} }, DepOp::add) == dep_edges{{8,0},{8,2}}));
   assert((translate(vec_eseed_t{0,4,5,6,9}, 4, 6, block_v{ {10,Q::both},{2,Q::fa},{{3,7},Q::ex},{{8,9},Q::fa},{{10,10},Q::ex} }, DepOp::add) == dep_edges{{8,0},{8,2},{9,2},{8,3},{9,4}}));
   assert((translate(vec_eseed_t{0,3,4,5,6,9,13}, 4, 7, block_v{ {11,Q::both},{1,Q::ex},{{2,3},Q::fa},{{4,8},Q::ex},{{9,10},Q::fa},{{11,11},Q::ex} }, DepOp::add) == dep_edges{{2,0},{10,0},{9,1},{10,1},{9,2},{10,3},{10,5}}));
  }

  {const auto [S,V] = create_dependencies(dep_edges{}, block_v{{1,Q::ex},{1,Q::ex}}, DepOp::from_scratch);
   assert(S.size() == 1);
   const auto f = S.find(AVarset{});
   assert(f != S.end());
   assert(V.size() == 2);
   assert(V[0] == nullptr);
   assert(V[1] == &*f);
  }
  {const auto [S,V] = create_dependencies(dep_edges{}, block_v{{2,Q::both},{1,Q::fa},{{2,2},Q::ex}}, DepOp::from_scratch);
   assert(S.size() == 1);
   const auto f = S.find(AVarset{});
   assert(f != S.end());
   assert(V.size() == 3);
   assert(V[0] == nullptr);
   assert(V[1] == nullptr);
   assert(V[2] == &*f);
  }
  {const auto [S,V] = create_dependencies(dep_edges{{1,0}}, block_v{{2,Q::both},{1,Q::fa},{{2,2},Q::ex}}, DepOp::from_scratch);
   assert(S.size() == 1);
   const auto f = S.find(AVarset{1});
   assert(f != S.end());
   assert(V.size() == 3);
   assert(V[0] == nullptr);
   assert(V[1] == nullptr);
   assert(V[2] == &*f);
  }
  {const auto [S,V] = create_dependencies(dep_edges{{1,0}}, block_v{{3,Q::both},{1,Q::fa},{{2,3},Q::ex}}, DepOp::from_scratch);
   assert(S.size() == 2);
   const auto f2 = S.find(AVarset{1});
   assert(f2 != S.end());
   const auto f3 = S.find(AVarset{});
   assert(f3 != S.end());
   assert(V.size() == 4);
   assert(V[0] == nullptr);
   assert(V[1] == nullptr);
   assert(V[2] == &*f2);
   assert(V[3] == &*f3);
  }
  {const auto [S,V] = create_dependencies(dep_edges{{1,1}}, block_v{{3,Q::both},{1,Q::fa},{{2,3},Q::ex}}, DepOp::from_scratch);
   assert(S.size() == 2);
   const auto f2 = S.find(AVarset{});
   assert(f2 != S.end());
   const auto f3 = S.find(AVarset{1});
   assert(f3 != S.end());
   assert(V.size() == 4);
   assert(V[0] == nullptr);
   assert(V[1] == nullptr);
   assert(V[2] == &*f2);
   assert(V[3] == &*f3);
  }
  {const auto [S,V] = create_dependencies(dep_edges{{1,0},{1,1}}, block_v{{3,Q::both},{1,Q::fa},{{2,3},Q::ex}}, DepOp::from_scratch);
   assert(S.size() == 1);
   const auto f = S.find(AVarset{1});
   assert(f != S.end());
   assert(V.size() == 4);
   assert(V[0] == nullptr);
   assert(V[1] == nullptr);
   assert(V[2] == &*f);
   assert(V[3] == &*f);
  }
  {const auto [S,V] = create_dependencies(dep_edges{{2,0},{8,0},{10,5}}, block_v{{11,Q::both},{1,Q::ex},{{2,2},Q::fa},{{3,7},Q::ex},{{8,10},Q::fa},{{11,11},Q::ex}}, DepOp::from_scratch);
   assert(S.size() == 3);
   const auto f1 = S.find(AVarset{2,8});
   assert(f1 != S.end());
   const auto f3 = S.find(AVarset{});
   assert(f3 != S.end());
   const auto f7 = S.find(AVarset{10});
   assert(f7 != S.end());
   assert(V.size() == 12);
   assert(V[0] == nullptr);
   assert(V[1] == &*f1);
   assert(V[2] == nullptr);
   assert(V[3] == &*f3);
   assert(V[4] == &*f3);
   assert(V[5] == &*f3);
   assert(V[6] == &*f3);
   assert(V[7] == &*f7);
   assert(V[8] == nullptr);
   assert(V[9] == nullptr);
   assert(V[10] == nullptr);
   assert(V[11] == &*f3);
  }

}
