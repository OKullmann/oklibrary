// Oliver Kullmann, 18.4.2019 (Swansea)
/* Copyright 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#include <iostream>
#include <string_view>
#include <sstream>

#include <ProgramOptions/Environment.hpp>

#include "ClauseSets.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.2.1",
        "26.6.2019",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Transformers/Generators/Random/TestClauseSets.cpp",
        "GPL v3"};

  using namespace RandGen;

}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv))
  return 0;

  {bool has_thrown = false;
   try { VarInterval(2,1); }
   catch(const std::domain_error& e) {
     has_thrown = true;
     assert(std::string_view(e.what()) == "VarInterval(gen_uint_t,gen_uint_t): a > b");
   }
   assert(has_thrown);
   has_thrown = false;
   try { VarInterval(0,1); }
   catch(const std::domain_error& e) {
     has_thrown = true;
     assert(std::string_view(e.what()) == "VarInterval(gen_uint_t,gen_uint_t): a = 0");
   }
   assert(has_thrown);
   has_thrown = false;
   try { VarInterval(-1); }
   catch(const std::domain_error& e) {
     has_thrown = true;
     assert(std::string_view(e.what()) == "VarInterval(int): n = -1 < 0");
   }
   assert(has_thrown);
   has_thrown = false;
   try { VarInterval(0); }
   catch(const std::domain_error& e) {
     has_thrown = true;
     assert(std::string_view(e.what()) == "VarInterval(gen_uint_t,gen_uint_t): a > b");
   }
  }
  {const VarInterval x{3,4};
   assert(x.a() == 3 and x.b() == 4);
   const VarInterval y{3,4};
   assert(x == y);
   VarInterval z{3,5};
   assert(x != z);
   const auto [a,b] = pair64(z);
   assert(a == 3 and b == 5);
   VarInterval u(4);
   assert((u == VarInterval{1,4}));
   assert((pair64(u) == pair64{1,4}));
   z = y;
   assert(y == z);
   assert((pair64(VarInterval(gen_uint_t(-1))) == pair64{1,randgen_max}));
  }
  {Prob64 p0{2,4};
   VarInterval p1{pair64(p0)};
   VarInterval p2(1,2);
   VarInterval p3(p1);
   assert(p3 == p2);
  }
  {assert(VarInterval("1") == VarInterval(1,1));
   assert(VarInterval("77") == VarInterval(1,77));
   assert(VarInterval("05-018446744073709551615") == VarInterval(5,-1));
  }
  {RandGen_t g;
   assert(VarInterval(77,77).random_element(uniform_range(g)) == 77);
   assert(g == RandGen_t());
   uniform_range u(g);
   assert(VarInterval(1,1).random_element(u) == 1);
   assert(g == RandGen_t());
   assert(VarInterval(1,2).random_element(uniform_range(g)) == 1 + (valempty_1 >= max_half_p1));
   assert(VarInterval(1,1024).random_element(u) == 1 + (valempty_2 / iexp2(64-10)));
   assert(VarInterval(1,65536).random_element(u) == 1 + (valempty_3 / iexp2(64-16)));
   g.discard(9997);
   assert(VarInterval(6,517).random_element(uniform_range(g)) == 6 + (valempty_10001 / iexp2(64-9)));
   g.discard(9999);
   assert(VarInterval(10,521).random_element(uniform_range(g)) == 10 + (valempty_20001 / iexp2(64-9)));
   g.discard(9999);
   assert(VarInterval(10,1033).random_element(u) == 10 + (valempty_30001 / iexp2(64-10)));
  }

  {RParam p{{{{3,6},3,Prob64{1,3}}},10};
   assert(valid(p));
   assert(p == p);
   {const ClausePart& cp{p.cps.front()};
    assert(cp.n == VarInterval(3,6));
    assert(cp.k == 3);
    assert(cp.p.index() == 0);
    assert((std::get<0>(cp.p) == pair64{1,3}));}
   assert(p.c == 10);
  }
  {RParam p{{{10,4}},7};
   assert(valid(p));
   {const ClausePart& cp{p.cps.front()};
    assert((cp.n == VarInterval(10)));
    assert(cp.k == 4);
    assert(cp.p.index() == 0);
    assert((std::get<0>(cp.p) == pair64{1,2}));}
   assert(p.c == 7);
   const RParam p2{{{12,3,Prob64{3,5}}},8};
   assert(valid(p2));
   {const ClausePart& cp{p2.cps.front()};
    assert(cp.n == VarInterval(12));
    assert(cp.k == 3);
    assert(cp.p.index() == 0);
    assert(std::get<0>(cp.p) == Prob64(3,5));}
   assert(p2.c == 8);
   assert(p2 != p);
  }
  {assert((RParam{{{10,3,Prob64{0,1}}},20} != RParam{{{10,3,0}},20}));
  assert((not valid(RParam{{{{3,5},4}},5})));
  assert((valid(RParam{{{{3,6},4}},5})));
  assert((valid(RParam{{{{3,6},4,Prob64{0,1}}},5})));
  assert((not valid(RParam{{{{3,6},4,5}},5})));
  assert((valid(RParam{{{{3,6},4,4}},5})));
  }
  {RParam p{{{{3,8},3,2}},7};
   assert(valid(p));
   {const ClausePart& cp{p.cps.front()};
    assert(cp.n == VarInterval(3,8));
    assert(cp.k == 3);
    assert(cp.p.index() == 1 and std::get<1>(cp.p) == 2);}
   assert(p.c == 7);
  }
  {assert((read_rparam_v("\n 7 * 3 - 8, 3 ,  \t2") == rparam_v{{{{{3,8},3,2}},7}}));
   assert((read_rparam_v("7*3-8,3,2; 6*2-66,4,1/3") == rparam_v{
     {{{{3,8},3,2}},7},
     {{{{2,66},4,Prob64{1,3}}}, 6}
   }));
   assert((read_rparam_v("7*3-8,3,2|6-10,2,1/4; 6*2-66,4,1/3") == rparam_v{
     {{{{3,8},3,2}, {{6,10},2,Prob64{1,4}}}, 7},
     {{{{2,66},4,Prob64{1,3}}}, 6}
   }));
  }

  {constexpr auto size_s = GParam::size_s;
   constexpr auto size_r = GParam::size_r;
   using pair_t = GParam::pair_t;
   for (int i = 0, k = 0; i < size_r; ++i) {
     const auto ro = RenameO(i);
     for (int j = 0; j < size_s; ++j) {
       const auto so = SortO(j);
       const GParam p(k++);
       assert(p == GParam(so, ro));
       assert((pair_t(p) == pair_t{so,ro}));
       std::stringstream s;
       s << p;
       const auto parts = Environment::split(s.str(), sep);
       assert(parts.size() == 2);
       assert(parts[0].size() >= 2);
       assert(parts[1].size() >= 2);
       assert(GParam{Environment::translate<option_t>()(parts[0].substr(1,1) + sep + parts[1].substr(0,1), sep)} == p);
     }
   }

   {const Param p1({}, {});
    assert((seeds(p1) == vec_eseed_t{0, 0, 0, 0}));
    assert((seeds(Param{GParam(1), {{{{10,3,Prob64{1,3}}},15}}}) ==
      vec_eseed_t{0,1,1,0,  15,1,1,10,3,1,3}));
    assert((seeds(Param{{SortO::sorted,RenameO::maxindex}, {{{{{3,22},7}},11}, {{{20,2,Prob64{4,16}}},4}}}) == vec_eseed_t{0,4,2,0, 11,1,3,22,7,1,2, 4,1,1,20,2,1,4}));
   }
  }

  {RandGen_t g;
   Clause C; C.reserve(3);
   rand_clause(g,C, 3, 3, 0);
   assert((C == Clause{{1,-1},{2,-1},{3,-1}}));
   C.clear();
   rand_clause(g,C, 3, 3, 3);
   assert((C == Clause{{1,1},{2,1},{3,1}}));
   C.clear();
   rand_clause(g,C, 3, 3, Prob64{0,1});
   assert((C == Clause{{1,-1},{2,-1},{3,-1}}));
   C.clear();
   rand_clause(g,C, 3, 3, Prob64{1,1});
   assert((C == Clause{{1,1},{2,1},{3,1}}));
   C.clear();
   rand_clause(g,C, {4,6}, 3, 0);
   assert((C == Clause{{4,-1},{5,-1},{6,-1}}));
   C.clear();
   rand_clause(g,C, {10,12}, 3, Prob64{1,1});
   assert((C == Clause{{10,1},{11,1},{12,1}}));
   assert(g == RandGen_t());
  }

  {RandGen_t g({0});
   Clause C; C.reserve(5);
   rand_clause(g,C,10,5,0);
   assert((C == Clause{{2,-1},{4,-1},{7,-1},{9,-1},{10,-1}}));
  }
  {RandGen_t g({0});
   Clause C; C.reserve(6);
   rand_clause(g,C,10,6,6);
   assert((C == Clause{{1,1},{3,1},{5,1},{6,1},{8,1},{9,1}}));
  }
  {RandGen_t g;
   Clause C; C.reserve(3);
   rand_clause(g,C,{3,5},3,Prob64{1,2});
   assert((C == Clause{{3,1},{4,1},{5,1}}));
  }
  {RandGen_t g;
   Clause C; C.reserve(3);
   rand_clause(g,C,{3,5},3,Prob64{1,4});
   assert((C == Clause{{3,1},{4,1},{5,-1}}));
  }
  {RandGen_t g;
   Clause C; C.reserve(3);
   rand_clause(g,C,{3,5},3,Prob64{1,8});
   assert((C == Clause{{3,1},{4,-1},{5,-1}}));
  }
  {RandGen_t g;
   Clause C; C.reserve(3);
   rand_clause(g,C,{3,5},3,Prob64{1,16});
   assert((C == Clause{{3,1},{4,-1},{5,-1}}));
  }
  {RandGen_t g;
   Clause C; C.reserve(3);
   rand_clause(g,C,{3,5},3,Prob64{1,32});
   assert((C == Clause{{3,-1},{4,-1},{5,-1}}));
  }
  {RandGen_t g({0});
   Clause C; C.reserve(10);
   rand_clause(g,C,10,10,5);
   assert((C == Clause{{1,-1},{2,1},{3,-1},{4,1},{5,-1},{6,-1},{7,1},{8,-1},{9,1},{10,1}}));
  }
  {RandGen_t g({0});
   Clause C; C.reserve(10);
   rand_clause(g,C,10,10,6);
   assert((C == Clause{{1,1},{2,-1},{3,1},{4,-1},{5,1},{6,1},{7,-1},{8,1},{9,1},{10,-1}}));
  }

  {RandGen_t g;
   const auto F = rand_clauselist(g, {{{{4,0}},0},{{{{2,7},3}},5},{{{{9,12},4,2}},8},{{{{1,4},3,Prob64{1,3}}},5},{{{12,12,12}},1},{{{{2,11},10,0}},1}, {{{{15,15},1}},1},{{{{20,20},1}},1}}, RenameO::renamed);
   assert(F.first.first == 14);
   assert(F.first.second == 22);
   assert(F.second.size() == 22);
   assert((F.second[0] == Clause{{2,-1},{3,1},{7,-1}}));
   assert((F.second[5] == Clause{{9,-1},{10,1},{11,1},{12,-1}}));
   assert((F.second[13] == Clause{{1,-1},{3,1},{4,1}}));
   assert((F.second[18] == Clause{{1,1},{2,1},{3,1},{4,1},{5,1},{6,1},{7,1},{8,1},{9,1},{10,1},{11,1},{12,1}}));
   assert((F.second[19] == Clause{{2,-1},{3,-1},{4,-1},{5,-1},{6,-1},{7,-1},{8,-1},{9,-1},{10,-1},{11,-1}}));
   assert((F.second[20] == Clause{{13,-1}}));
   assert((F.second[21] == Clause{{14,1}}));
  }
  {RandGen_t g;
   const auto F = rand_sortedclauselist(g, {{{{4,0}},0},{{{{2,7},3}},5},{{{{9,12},4,2}},8},{{{{1,4},3,Prob64{1,3}}},5},{{{12,12,12}},1},{{{{2,11},10,0}},1}, {{{{15,15},1}},1}}, RenameO::renamed);
   assert(F.first.first == 13);
   assert(F.first.second == 16);
   assert((F.second.front() == Clause{{1,-1},{2,1},{3,-1}}));
   assert((F.second.back() == Clause{{13,-1}}));
  }
  {RandGen_t g;
   const auto F = rand_clauseset(g, {{{{4,0}},0},{{{{2,7},3}},5},{{{{9,12},4}},8},{{{{1,4},3,Prob64{1,3}}},5},{{{12,12,12}},1},{{{{2,11},10,0}},1}, {{{{15,15},1}},1}}, RenameO::renamed);
   assert(F.first.first == 13);
   assert(F.first.second == 21);
   assert((F.second.front() == Clause{{1,-1},{2,-1},{3,-1}}));
   assert((F.second.back() == Clause{{13,-1}}));

  }

}
