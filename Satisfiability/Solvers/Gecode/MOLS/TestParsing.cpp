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
#include "Parsing.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.1.3",
        "15.3.2022",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/OKlib-MOLS/blob/master/Satisfiability/Solvers/Gecode/MOLS/TestParsing.cpp",
        "GPL v3"};

  using namespace Parsing;
  using namespace Conditions;

  template <class X>
  constexpr bool eqp(const X& lhs, const X& rhs) noexcept {
    return lhs == rhs;
  }
}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv))
  return 0;

  {std::stringstream ss;
   ss << UC::idem;
   assert(ss.str() == "idem");
  }

  {std::stringstream ss;
   ss << VS::at;
   assert(ss.str() == "at");
   ss.str("");
   ss << VS::id;
   assert(ss.str() == "id");
  }

  {Versions vs;
   std::stringstream ss;
   ss << vs;
   assert(ss.str() == "id");
   vs = {VS::c213};
   ss.str(""); ss << vs;
   assert(ss.str() == "id c213");
   vs = {VS::at, VS::c321};
   ss.str(""); ss << vs;
   assert(ss.str() == "id c321 at");
  }

  {Square s(0);
   std::stringstream ss;
   ss << s;
   assert(ss.str() == "0");
   s = {77,VS::c231};
   ss.str(""); ss << s;
   assert(ss.str() == "c231 77");
  }

  {std::stringstream ss;
   ss << Equation{0,1};
   assert(ss.str() == "= 0  1");
   ss.str("");
   ss << Equation{{0,VS::c231},1};
   assert(ss.str() == "= c231 0  1");
   ss.str("");
   ss << Equation{567,{0,VS::c231}};
   assert(ss.str() == "= c231 0  567");
   ss.str("");
   ss << Equation{567,{568,VS::c231}};
   assert(ss.str() == "= 567  c231 568");
   ss.str("");
   ss << Equation{{567,VS::at},{568,VS::c231}};
   assert(ss.str() == "= at 567  c231 568");
  }

  {assert(eqp(classify("="), {CT::equation, 0}));
   for (size_t uc = 1; uc <= maxUC; ++uc)
     assert(eqp(classify(strUC[uc]), {CT::unary, uc}));
   assert(eqp(classify("rprod"), {CT::prod_equation, 1}));
   assert(eqp(classify("cprod"), {CT::prod_equation, 2}));
   assert(eqp(classify("x"), {CT::unknown, 0}));
   assert(eqp(classify("UNDEF"), {CT::unknown, 0}));
  }

  {std::istringstream ss;
   bool caught = false;
   try {const auto AC = ReadAC()(ss);}
   catch (const Error& e) {caught = true;}
   assert(caught);
  }
  {std::istringstream ss("x");
   bool caught = false;
   try {const auto AC = ReadAC()(ss);}
   catch (const Error& e) {caught = true;}
   assert(caught);
  }
  {std::istringstream ss("squares");
   bool caught = false;
   try {const auto AC = ReadAC()(ss);}
   catch (const Error& e) {caught = true;}
   assert(caught);
  }
  {std::istringstream ss("squares A");
   assert(ReadAC()(ss) == AConditions(1));
  }
  {std::istringstream ss("  \n # \n  \t squares A xy 67&8() # \n\n\n");
   assert(ReadAC()(ss) == AConditions(3));
  }

}
