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
        "0.1.14",
        "15.6.2019",
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
     assert(std::string_view(e.what()) == "VarInterval(int): n < 0");
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

  {RParam p{{3,6},3,10,Prob64{1,3}};
   assert(valid(p));
   assert(p == p);
   assert(p.n == VarInterval(3,6));
   assert(p.k == 3);
   assert(p.c == 10);
   assert(p.p.index() == 0);
   assert((std::get<0>(p.p) == pair64{1,3}));
  }
  {RParam p{10,4,7};
   assert(valid(p));
   assert((p.n == VarInterval(10)));
   assert(p.k == 4);
   assert(p.c == 7);
   assert(p.p.index() == 0);
   assert((std::get<0>(p.p) == pair64{1,2}));
   const RParam p2{12,3,8,Prob64{3,5}};
   assert(valid(p2));
   assert(p2.n == VarInterval(12));
   assert(p2.k == 3);
   assert(p2.c == 8);
   assert(p.p.index() == 0);
   assert(std::get<0>(p2.p) == Prob64(3,5));
   assert(p2 != p);
  }
  {RParam p{{3,8},3,7,2};
   assert(valid(p));
   assert(p.n == VarInterval(3,8));
   assert(p.k == 3);
   assert(p.c == 7);
   assert(p.p.index() == 1 and std::get<1>(p.p) == 2);
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
       assert(Environment::read<GParam>(s.str()) == p);
     }
   }

   {const Param p1({}, {});
    assert((p1.seeds() == vec_eseed_t{0, 0, 0, 0}));
    assert(((Param{GParam(1), {{10,3,15,Prob64{1,3}}}}).seeds() ==
      vec_eseed_t{0,1,1,0,  1,10,3,15,1,3}));
    assert(((Param{{SortO::sorted,RenameO::renamed}, {{{3,22},7,11}, {20,2,4,Prob64{4,16}}}}).seeds() == vec_eseed_t{0,7,2,0, 3,22,7,11,1,2, 1,20,2,4,1,4}));
   }
  }

  {RandGen_t g;
   assert((rand_clause(g, 3, 3, 0) == Clause{{1,-1},{2,-1},{3,-1}}));
   assert((rand_clause(g, 3, 3, 3) == Clause{{1,1},{2,1},{3,1}}));
   assert((rand_clause(g, 3, 3, Prob64{0,1}) == Clause{{1,-1},{2,-1},{3,-1}}));
   assert((rand_clause(g, 3, 3, Prob64{1,1}) == Clause{{1,1},{2,1},{3,1}}));
   assert((rand_clause(g, {4,6}, 3, 0) == Clause{{4,-1},{5,-1},{6,-1}}));
   assert((rand_clause(g, {10,12}, 3, Prob64{1,1}) == Clause{{10,1},{11,1},{12,1}}));
   assert(g == RandGen_t());
  }

  {RandGen_t g({0});
   assert((rand_clause(g,10,5,0) == Clause{{2,-1},{4,-1},{7,-1},{9,-1},{10,-1}}));
  }
  {RandGen_t g({0});
   assert((rand_clause(g,10,6,6) == Clause{{1,1},{3,1},{5,1},{6,1},{8,1},{9,1}}));
  }
  {RandGen_t g;
   assert((rand_clause(g,{3,5},3,Prob64{1,2}) == Clause{{3,1},{4,1},{5,1}}));
  }
  {RandGen_t g;
   assert((rand_clause(g,{3,5},3,Prob64{1,4}) == Clause{{3,1},{4,1},{5,-1}}));
  }
  {RandGen_t g;
   assert((rand_clause(g,{3,5},3,Prob64{1,8}) == Clause{{3,1},{4,-1},{5,-1}}));
  }
  {RandGen_t g;
   assert((rand_clause(g,{3,5},3,Prob64{1,16}) == Clause{{3,1},{4,-1},{5,-1}}));
  }
  {RandGen_t g;
   assert((rand_clause(g,{3,5},3,Prob64{1,32}) == Clause{{3,-1},{4,-1},{5,-1}}));
  }
  {RandGen_t g({0});
   assert((rand_clause(g,10,10,5) == Clause{{1,-1},{2,1},{3,-1},{4,1},{5,-1},{6,-1},{7,1},{8,-1},{9,1},{10,1}}));
  }
  {RandGen_t g({0});
   assert((rand_clause(g,10,10,6) == Clause{{1,1},{2,-1},{3,1},{4,-1},{5,1},{6,1},{7,-1},{8,1},{9,1},{10,-1}}));
  }

  {RandGen_t g;
std::cerr << std::pair{DimacsComments{{"clauselist"}}, rand_clauselist(g, {{4,0,0},{{2,7},3,5},{{9,12},4,8,2},{{1,4},3,5,Prob64{1,3}},{12,12,1,12},{{2,11},10,1,0}, {15,0,0}}, RenameO::maxindex)};
g = RandGen_t();
std::cerr << std::pair{DimacsComments{{"sortedclauselist"}}, rand_sortedclauselist(g, {{4,0,0},{{2,7},3,5},{{9,12},4,8,2},{{1,4},3,5,Prob64{1,3}},{12,12,1,12},{{2,11},10,1,0}, {15,0,0}}, RenameO::maxindex)};
g = RandGen_t();
std::cerr << std::pair{DimacsComments{{"clauseset"}}, rand_clauseset(g, {{4,0,0},{{2,7},3,5},{{9,12},4,8},{{1,4},3,5,Prob64{1,3}},{12,12,1,12},{{2,11},10,1,0}, {{15,15},1,1}}, RenameO::maxindex)};
  }

}
