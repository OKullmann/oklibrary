// Oliver Kullmann, 21.3.2022 (Swansea)
/* Copyright 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#include <iostream>
#include <sstream>
#include <string>

#include <cassert>

#include <ProgramOptions/Environment.hpp>

#include "PartialSquares.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.2.3",
        "25.4.2022",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/OKlib-MOLS/blob/master/Satisfiability/Solvers/Gecode/MOLS/TestPartialSquares.cpp",
        "GPL v3"};

  using namespace PartialSquares;

  template <class X>
  constexpr bool eqp(const X& lhs, const X& rhs) noexcept {
    return lhs == rhs;
  }
}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv))
  return 0;

  {assert(valid(prow_t(0)));
   assert(not valid(prow_t(1)));
   assert(valid(psquare_t(0)));
   assert(not valid(psquare_t(1)));
   for (size_t N = 0; N <= 5; ++N) {
     assert(valid(empty_prow(N), N));
     assert(valid(empty_psquare(N), N));
     assert(valid(PSquare(N),N));
   }
  }

  {assert(Cell() == Cell(0));
   assert(Cell{} == Cell());
   assert(Cell{{}} == Cell());
   assert(Cell({0}) == Cell());
   assert(Cell({0,0}) == Cell(2));
   assert(Cell({1,1}).first() == 2);
   assert(Cell({1,0}).first() == 1);
   assert(Cell({0,1}).first() == 0);

   for (unsigned n = 0; n <= 5; ++n) {
     assert(Cell(n).size() == n);
     assert(Cell(cell_t{bool(n)}).size() == 1);
     assert(Cell(n).consistent() == (n != 0));
     assert(Cell(n).unit() == (n == 1));
     assert(Cell(n).first() == 0);
   }
   assert(Cell(1) == Cell(cell_t{0}));
   assert(Cell(1) == Cell({1}));
   assert(Cell(1) > Cell({0}));
   assert(Cell(cell_t{1}) == Cell(1).flip());
  }

  {assert(eqp(empty_prow(0), {}));
   assert(eqp(empty_prow(1), {Cell(1)}));
   assert(eqp(empty_prow(2), {Cell(2),Cell(2)}));
   assert(eqp(empty_psquare(0), {}));
   assert(eqp(empty_psquare(1), {{Cell(1)}}));
   assert(eqp(empty_psquare(2), {{Cell(2),Cell(2)}, {Cell(2),Cell(2)}}));
  }

  {std::istringstream ss;
   const PSquares p(2, ss);
   assert(p.N == 2);
   assert(p.psqs.empty());
  }
  {std::istringstream ss("\n\t\n # \n #");
   const PSquares p(2, ss);
   assert(p.N == 2);
   assert(p.psqs.empty());
  }
  {std::istringstream ss("\n\t\n # \n #\n gh");
   bool caught = false;
   try {PSquares p(2, ss);}
   catch (const PSquares::Error& e) {
     caught = true;
     assert(std::string(e.what()) ==
            "ERROR[PSquares]: The number of lines should be a"
            " multiple of N+1=3, but is 1.");
   }
   assert(caught);
  }
  {std::istringstream ss("x\nx\nx\n");
   bool caught = false;
   try {PSquares p(2, ss);}
   catch (const PSquares::Error& e) {
     caught = true;
     assert(std::string(e.what()) ==
            "ERROR[PSquares]: Invalid name of square number 1"
            " in the header-line: \"x\".");
   }
   assert(caught);
  }
  {std::istringstream ss("0 x\nx\nx\n");
   bool caught = false;
   try {PSquares p(2, ss);}
   catch (const PSquares::Error& e) {
     caught = true;
     assert(std::string(e.what()) ==
            "ERROR[PSquares]: In square number 1 there is content after"
            " the name \"0\" of the square.");
   }
   assert(caught);
  }
  {std::istringstream ss("0\nx\nx\n");
   bool caught = false;
   try {PSquares p(2, ss);}
   catch (const PSquares::Error& e) {
     caught = true;
     assert(std::string(e.what()) ==
            "ERROR[PSquares]: In square number 1 in row 1 there should"
            " be exactly 2 entries, but there are 1;\n"
            " the whole line is: \"x\".");
   }
   assert(caught);
  }
  {std::istringstream ss("0\n* *\n* *\n");
   const PSquares p(2, ss);
   assert(p.N == 2);
   assert(eqp(p.psqs, {2}));
  }
  {std::istringstream ss("0\n- -\n- -\n");
   const PSquares p(2, ss);
   assert(p.N == 2);
   assert(eqp(p.psqs, {2}));
  }
  {std::istringstream ss("0\n+ +\n+ +\n");
   const PSquares p(2, ss);
   assert(p.N == 2);
   assert(eqp(p.psqs, {flip(PSquare{2})}));
  }
  {std::istringstream ss("0\n+0,1 *\n* +1,x\n");
   bool caught = false;
   try {PSquares p(2, ss);}
   catch (const PSquares::Error& e) {
     caught = true;
     assert(std::string(e.what()).starts_with(
            "ERROR[PSquares]: In square number 1, row 2, the cell 2"
            " is \"+1,x\".\n Its item \"x\" could not be parsed as a proper"
            " natural number:"));
   }
   assert(caught);
  }
  {std::istringstream ss("0\n+0,1 *\n* +1,-1\n");
   bool caught = false;
   try {PSquares p(2, ss);}
   catch (const PSquares::Error& e) {
     caught = true;
     assert(std::string(e.what()) ==
            "ERROR[PSquares]: In square number 1, row 2, cell 2, the item "
            "\"-1\" has value 18446744073709551615 >= 2.");
   }
   assert(caught);
  }
  {std::istringstream ss("0\n+0,1 *\n* +1,2\n");
   bool caught = false;
   try {PSquares p(2, ss);}
   catch (const PSquares::Error& e) {
     caught = true;
     assert(std::string(e.what()) ==
            "ERROR[PSquares]: In square number 1, row 2, cell 2, the item "
            "\"2\" has value 2 >= 2.");
   }
   assert(caught);
  }
  {std::istringstream ss("c213 789\n+ -\n-0,1 +1\n"
                         " 12 # c\n\n* \t 0\n0,1 -1\n");
   const PSquares p(2, ss);
   assert(p.N == 2);
   const PSquare p0({{Cell({1,1}),Cell(2)},{Cell({1,1}),Cell({1,0})}},
                    {789, CD::VS::c213});
   assert(valid(p0,2));
   const PSquare p1({{Cell(2),Cell({0,1})},{Cell(2),Cell({0,1})}},
                    12);
   assert(valid(p1,2));
   assert(p == PSquares(2,{p0,p1}));
  }

}
