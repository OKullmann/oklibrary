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
        "0.1.5",
        "19.5.2019",
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

  {RParam p{{3,6},3,10,{1,3}};
   assert(p.n == VarInterval(3,6));
   assert(p.k == 3);
   assert(p.c == 10);
   assert((p.p == pair64{1,3}));
  }
  {RParam p{10,4,7};
   assert((p.n == VarInterval(10)));
   assert(p.k == 4);
   assert(p.c == 7);
   assert((p.p == pair64{1,2}));
   const RParam p2{12,3,8,{3,5}};
   assert(p2.n == VarInterval(12));
   assert(p2.k == 3);
   assert(p2.c == 8);
   assert(p2.p == Prob64(3,5));
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
    assert((p1.seeds() == vec_eseed_t{0, 0, 0}));
    assert(((Param{GParam(1), {{10,3,15,{1,3}}}}).seeds() == vec_eseed_t{0,1,1, 1,10,3,15,1,3}));
    assert(((Param{{SortO::sorted,RenameO::renamed}, {{{3,22},7,11}, {20,2,4,{4,16}}}}).seeds() == vec_eseed_t{0,4,2, 3,22,7,11,1,2, 1,20,2,4,1,4}));
   }
  }
}
