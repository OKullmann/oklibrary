// Oliver Kullmann, 8.3.2022 (Swansea)
/* Copyright 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#include <iostream>

#include <cassert>

#include <ProgramOptions/Environment.hpp>

#include "Conditions.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.1.3",
        "9.3.2022",
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

  {UConditions uc;
   assert(uc.cond().empty());
   assert(eqp(uc, {}));
   assert(not uc.contains(UCL::diag));
   assert(uc.insert(UCL::diag));
   assert(UConditions{} < uc);
   assert(uc.contains(UCL::diag));
   assert(uc.cond().size() == 1);
   const UConditions old(uc);
   assert(old == uc);
   assert(not uc.insert(UCL::diag));
   assert(old == uc);
   assert(uc.insert(UCL::antidiag));
   assert(uc.cond().size() == 2);
   assert(old < uc);
  }

  {Versions v;
   assert(v.choices().size() == 1);
   assert(eqp(v, {}));
   assert(eqp(v.choices(), {VS::id}));
   assert(not v.insert(VS::id));
   assert(v.contains(VS::id));
   assert(not v.contains(VS::at));
   assert(v.insert(VS::at));
   assert(v.choices().size() == 2);
   assert(v.contains(VS::at));
   assert(Versions{} < v);
   const Versions old(v);
   assert(not v.insert(VS::at));
   assert(v == old);
   Versions v2(v), v3(v);
   assert(v2.insert(VS::c213));
   assert(v3.insert(VS::c312));
   assert(v2 != v3);
   assert((v2 < v3));
   assert(not (v3 <= v2));
  }

  {// Square s; not allowed
   Square s{1};
   assert(s == Square(1));
   assert(eqp(s, {1}));
   assert(s == Square(1,VS::id));
   assert(s.v == VS::id);
   const Square s2(1, VS::at);
   assert(s < s2);
   s.v = VS::at;
   assert(s == s2);
   assert(eqp(s, {1,VS::at}));
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
  }

  {AConditions C(0);
   assert(C.k == 0);
   assert(eqp(C.versions(), {}));
   assert(eqp(C.map(), {}));
   assert(eqp(C.eq(), {}));
   assert(eqp(C.orth(), {}));
   assert(C.num_squares() == 0);
   assert(not C.contains({0}));
   assert(not C.contains({0}, UCL::diag));
   assert(not C.contains({{0}, {0}}));
   assert(not C.valid(Square{0}));
   assert(not C.valid(Equation{0,0}));
   assert(C.valid(AConditions::orth_t{}));
   assert(not C.valid(AConditions::orth_t{0}));
  }

}
