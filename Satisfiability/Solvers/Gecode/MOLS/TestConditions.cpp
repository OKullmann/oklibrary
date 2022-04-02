// Oliver Kullmann, 8.3.2022 (Swansea)
/* Copyright 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#include <iostream>
#include <sstream>

#include <cassert>

#include <ProgramOptions/Environment.hpp>

#include "Conditions.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.4.2",
        "2.4.2022",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/OKlib-MOLS/blob/master/Satisfiability/Solvers/Gecode/MOLS/TestConditions.cpp",
        "GPL v3"};

  using namespace Conditions;

  template <class X>
  constexpr bool eqp(const X& lhs, const X& rhs) noexcept {
    return lhs == rhs;
  }
}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv))
  return 0;

  {assert(toUC("") == UC(0));
   assert(toUC("UNDEF") == UC(0));
   assert(toUC("rls") == UC(1));
   assert(toUC("antisymm") == UC(maxUC));
  }

  {assert(toVS("") == VS::id);
   assert(toVS("id") == VS::id);
   assert(toVS("at") == VS::at);
  }
  {assert(maxVS == 11); // 11 = 3*4 - 1
   for (size_t i = 0; i <= maxVS; ++i)
     assert(size_t(toVS(strVS[i])) == i);
  }
  {assert(main_rep(VS::id) == VS::id);
   assert(main_rep(VS::atc213) == VS::id);
   assert(main_rep(VS::c312) == VS::c312);
   assert(main_rep(VS::c321) == VS::c312);
  }

  {Versions v;
   assert(v.choices().size() == 1);
   assert(eqp(v, {}));
   assert(eqp(v.choices(), {VS::id}));
   assert(not v.insert(VS::id));
   assert(v.contains(VS::id));
   assert(v.contains(VS::at));
   assert(not v.contains(VS::c231));
   assert(v.index(VS::id) == 0);
   assert(v.index(VS::at) == 0);
   assert(not v.insert(VS::at));
   assert(v.choices().size() == 1);
   assert(v.contains(VS::at));
   assert(v.index(VS::id) == 0);
   assert(v.index(VS::at) == 0);
   assert(v.index(VS::c213) == 0);
   assert(Versions{} == v);
   const Versions old(v);
   assert(not v.insert(VS::at));
   assert(v == old);
   Versions v2(v), v3(v);
   assert(not v2.insert(VS::c213));
   assert(v2.index(VS::c213) == 0);
   assert(v2.index(VS::at) == 0);
   assert(v3.insert(VS::c312));
   assert(v2 != v3);
   assert((v2 < v3));
   assert(not (v3 <= v2));
   assert((v3 == Versions{VS::id, VS::at, VS::c312}));
   assert((v3 == Versions({VS::c312})));
  }

  {// Square s; not allowed
   Square s{1};
   assert(s == Square(1));
   assert(eqp(s, {1}));
   assert(s == Square(1,VS::id));
   assert(s.v == VS::id);
   assert(s.primary());
   const Square s2(1, VS::at);
   assert(s < s2);
   s.v = VS::at;
   assert(s == s2);
   assert(eqp(s, {1,VS::at}));
   assert(not s.primary());
  }

  {assert(Square::is.first.empty());
   assert(Square::is.second.empty());
   using Environment::tokens_t;
   using osq_t = Square::osq_t;
   tokens_t t;
   size_t j = 0;
   assert(eqp(Square::read(t,j), {}));
   assert(j == 0);
   t.push_back("x77");
   assert(eqp(Square::read(t,j), {}));
   assert(j == 0);
   t[0] = "+0000";
   assert(eqp(Square::read(t,j), osq_t(Square(0))));
   assert(j == 1);
   assert(eqp(Square::read(t,j), {}));
   assert(j == 1);
   j = 0;
   t[0] = "+000987";
   assert(eqp(Square::read(t,j), osq_t(Square(987))));
   assert(j == 1);
   j = 0;
   t[0] = "+000987x";
   assert(eqp(Square::read(t,j), {}));
   assert(j == 0);
   t[0] = "id 0";
   assert(eqp(Square::read(t,j), {}));
   assert(j == 0);
   t[0] = "c213";
   assert(eqp(Square::read(t,j), {}));
   assert(j == 1);
   t.push_back("77");
   j = 0;
   assert(eqp(Square::read(t,j), osq_t(Square(77, VS::c213))));
   assert(j == 2);
   t[1] = "x";
   j = 0;
   assert(eqp(Square::read(t,j), {}));
   assert(j == 1);

   // Using names now:
   Square::is = {{"x77"}, {{"x77",999}}};
   j = 0;
   t[0] = "0";
   assert(eqp(Square::read(t,j), {}));
   assert(j == 0);
   t[0] = "x77";
   assert(eqp(Square::read(t,j), osq_t(Square(999))));
   assert(j == 1);
   t[0] = "at"; t[1] = "x77"; j = 0;
   assert(eqp(Square::read(t,j), osq_t(Square(999, VS::at))));
   assert(j == 2);
   t[1] = "x7"; j = 0;
   assert(eqp(Square::read(t,j), {}));
   assert(j == 1);
   Square::is = {};
  }

  {const Equation e({0},{1});
   assert(e == Equation({1},{0}));
   assert(eqp(e.lhs(), {0}));
   assert(eqp(e.rhs(), {1}));
   const Equation e2({5,VS::c321},{3,VS::c231});
   assert(eqp(e2.lhs(), {3,VS::c231}));
   assert(eqp(e2.rhs(), {5,VS::c321}));
   assert(e < e2);
   static_assert(Equation{{0},{1}} == Equation{{1},{0}});
   std::ostringstream ss;
   ss << e;
   assert(ss.str() == "= 0  1");
   ss.str("");
   ss << e2;
   assert(ss.str() == "= c231 3  c321 5");
  }

  {assert(toPT("") == PT(0));
   assert(toPT("rprod") == PT::rprod);
   assert(toPT("cprod") == PT::cprod);
  }

  {const ProdEq pe({0},{1},{2});
   assert(eqp(pe.r(), {0}));
   assert(eqp(pe.f2(), {1}));
   assert(eqp(pe.f1(), {2}));
   assert(pe.pt() == PT::rprod);
   assert(pe == ProdEq({0},{1},{2}));
   const ProdEq pe2({0},{1},{2}, PT::cprod);
   assert(pe < pe2);
   std::ostringstream ss;
   ss << pe;
   assert(ss.str() == "rprod 0  1  2");
   ss.str("");
   ss << pe2;
   assert(ss.str() == "cprod 0  1  2");
  }

  {const AConditions C(0);
   assert(C.k == 0);
   assert(eqp(C.versions(), {}));
   assert(eqp(C.map(), {}));
   assert(eqp(C.eq(), {}));
   assert(eqp(C.peq(), {}));
   assert(C.num_squares() == 0);
   assert(not C.contains({0}));
   assert(not C.contains(UC::diag, {0}));
   assert(not C.contains({{0}, {0}}));
   assert(not C.valid(Square{0}));
   assert(not C.valid(Equation{0,0}));
   assert(not C.valid(ProdEq{0,0,0}));
   std::ostringstream ss;
   ss << C;
   assert(ss.str() == "squares\n");
   assert(not ss.bad());
  }

  {const AConditions C(1);
   assert(C.k == 1);
   assert(eqp(C.versions(), {{}}));
   assert(C.map().empty());
   assert(eqp(C.eq(), {}));
   assert(eqp(C.peq(), {}));
   assert(C.num_squares() == 1);
   assert(C.contains({0}));
   assert(C.index({0}) == 0);
   assert(not C.contains(UC::diag, {0}));
   assert(not C.contains({{0}, {0}}));
   assert(C.valid(Square{0}));
   assert(C.valid(Equation{0,0}));
   assert(C.valid(ProdEq{0,0,0}));
   {std::ostringstream ss;
    ss << C;
    assert(ss.str() == "squares 0\n");
    assert(not ss.bad());
   }
   AConditions C2(C);
   assert(C2 == C);
   assert(not C2.insert(Square{0}));
   assert(C2 == C);
   assert(not C2.insert(Square{0,VS::at}));
   assert(C2.index({0}) == 0);
   assert(C2.index({0,VS::at}) == 0);
   assert(C2.map().empty());
   assert(C2 == C);
   assert(C2.num_squares() == 1);
   assert(C2.contains(Square{0,VS::at}));
   assert(C2.sqs(UC::diag).empty());
   assert(C2.insert(UC::diag, Square{0,VS::at}));
   assert(C2.map().size() == 1);
   assert(eqp(C2.sqs(UC::diag), {{0,VS::at}}));
   assert(C2.num_squares() == 1);
   assert(not C2.contains(Equation{0,0}));
   {std::ostringstream ss;
    ss << C2;
    assert(ss.str() == "squares 0\ndiag \tat 0\n");
    assert(not ss.bad());
   }
   const AConditions C3(C2);
   assert(C2.insert(Equation{0,0}));
   assert(C3 != C2);
   assert(C2.contains(Equation{0,0}));
   assert(C2.insert(Equation{0,{0,VS::at}}));
   const AConditions C4(C2);
   assert(not C2.contains(ProdEq{0,0,0}));
   assert(C2.insert(ProdEq{0,0,0}));
   assert(eqp(C2.peq(), {{0,0,0}}));
   assert(C4 != C2);
   {std::ostringstream ss;
    ss << C2;
    assert(ss.str() == "squares 0\ndiag \tat 0\n= 0  0\n= 0  at 0\nrprod 0  0  0\n");
    assert(not ss.bad());
   }
   assert(C2.insert(UC::diag, Square{0,VS::c312}));
   assert(C2.insert(UC::ls, Square{0,VS::c321}));
   {std::ostringstream ss;
    ss << C2;
    assert(ss.str() == "squares 0\nls \tc321 0\ndiag \tat 0  c312 0\n= 0  0\n= 0  at 0\nrprod 0  0  0\n");
    assert(not ss.bad());
   }
   AConditions C5(C2);
   assert(C5.k == 1);
   assert(C5.num_squares() == 2);
   assert(C5.index({0,VS::id}) == 0);
   assert(C5.index({0,VS::at}) == 0);
   assert(C5.index({0,VS::c312}) == 1);
   assert(C5.index({0,VS::c321}) == 1);
  }

  {AConditions ac(4);
   assert(ac.num_squares() == 4);
   assert(ac.insert(ProdEq{{0,VS::at}, {1,VS::c312}, {2,VS::c231}}));
   assert(ac.num_squares() == 6);
   assert(ac.index({0}) == 0);
   assert(ac.index({0,VS::at}) == 0);
   assert(ac.index({1}) == 1);
   assert(ac.index({1,VS::c312}) == 2);
   assert(ac.index({2}) == 3);
   assert(ac.index({2,VS::c231}) == 4);
   assert(ac.index({3}) == 5);
  }

}
