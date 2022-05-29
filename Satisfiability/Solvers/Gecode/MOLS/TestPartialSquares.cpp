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
        "0.2.9",
        "29.5.2022",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Solvers/Gecode/MOLS/TestPartialSquares.cpp",
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

  {const cell_t c1{0,0,0}, c2{1,0,0};
   assert(c1 != c2);
   assert(c1 < c2);
   assert(not c1[0]);
   assert(c2[0]);
  }

  {assert(Cell() == Cell(0));
   assert(Cell() == Cell{});
   assert(Cell() == Cell({}));
   assert(Cell() == Cell(0,0));
   assert(Cell{{}} == Cell({0}));
   assert(Cell({0}) == Cell{0});
   assert(Cell({0,0}) == Cell(2));
   assert((Cell{0,0} == Cell(2)));
   assert(Cell({1,1}).first() == 2);
   assert(Cell({1,0}).first() == 1);
   assert(Cell({0,1}).first() == 0);
   assert((Cell({0,1}) == Cell{0,1}));
  }
  {for (unsigned n = 0; n <= 5; ++n) {
     assert(Cell(n).size() == n);
     assert(Cell(cell_t{bool(n)}).size() == 1);
     assert(Cell(n).consistent() == (n != 0));
     assert(Cell(n).unit() == (n == 1));
     assert(Cell(n).first() == 0);
     assert(Cell(n).elimvals() == 0);
     assert(Cell(n).vals() == n);
     assert(not Cell(n).restricted());
     Cell C(n);
     assert(C == Cell(n));
     assert(C.first() == 0);
     assert(C.consistent() == (n != 0));
     Cell C2(C);
     assert(C2 == C);
     C2.flip();
     assert(C2 == Cell(n).flip());
     assert((C2 == C) == (n == 0));
     assert(C2.first() == n);
     assert(not C2.unit());
     assert(not C2.consistent());
     assert((C < C2) == (n != 0));
     assert(C2.elimvals() == n);
     assert(C2.vals() == 0);
     assert(C2.restricted() == (n != 0));
     swap(C, C2);
     assert(C2 == Cell(n));
     assert(C == Cell(n).flip());
     assert((C > C2) == (n != 0));
     C.swap(C2);
     assert(C == Cell(n));
     assert(C2 == Cell(n).flip());
     for (size_t i = 0; i <= n; ++i) {
       const Cell A(n,i);
       assert(A.size() == n);
       assert(A.unit() == (i < n));
       assert(A.first() == i);
       assert(A.elimvals() == (i==n ? n : n-1));
       assert(A.vals() == (i==n ? 0 : 1));
       assert(A.restricted() == (n >= 2 or (n == 1 and i == 1)));
       const Cell fA = flip(A);
       assert(fA.size() == n);
       assert(fA.unit() == ((n==2 and i<n) or (n==1 and i==1)));
       assert(fA.first() == (i==0 and i<n ? 1 : 0));
       assert(fA.elimvals() == (i<n ? 1 : 0));
       assert(fA.vals() == (i==n ? n : n-1));
       assert(fA.restricted() == (i<n));
       assert((A & fA) == Cell(n).flip());
       assert((A | fA) == Cell(n));
     }
   }
   assert(Cell(1) == Cell(cell_t{0}));
   assert(Cell(1).flip() == Cell({1}));
   assert(Cell(1) == Cell{0});
   assert(Cell(1) < Cell{1});
   assert(Cell(cell_t{1}) == Cell(1).flip());
  }
  {assert(((Cell{0,1,0,1} & Cell{1,0,1,0}) == Cell(4).flip()));
   assert(((Cell{0,1,0,1} | Cell{1,0,1,0}) == Cell(4)));
  }

  {for (size_t N = 0; N <= 3; ++N) {
     const prow_t er = empty_prow(N);
     assert(consistent(er));
     assert(unit(er) == (N <= 1));
     const prow_t fr = full_prow(N);
     assert((er < fr) == (N != 0));
     assert(consistent(fr) == (N == 0));
     assert(unit(fr) == (N == 0));
     assert(flip(er) == fr);
     assert(flip(fr) == er);

     const psquare_t es = empty_psquare(N);
     assert(consistent(es));
     assert(unit(es) == (N <= 1));
     const psquare_t fs = full_psquare(N);
     assert((es < fs) == (N != 0));
     assert(consistent(fs) == (N == 0));
     assert(unit(fs) == (N == 0));
     assert(flip(es) == fs);
     assert(flip(fs) == es);
   }
  }

  {assert(eqp(empty_prow(0), {}));
   assert(eqp(empty_prow(1), {Cell(1)}));
   assert(eqp(empty_prow(2), {Cell(2),Cell(2)}));
   assert(eqp(empty_psquare(0), {}));
   assert(eqp(empty_psquare(1), {{Cell(1)}}));
   assert(eqp(empty_psquare(2), {{Cell(2),Cell(2)}, {Cell(2),Cell(2)}}));
  }
  {assert(eqp(full_prow(0), {}));
   assert(eqp(full_prow(1), {Cell(1).flip()}));
   assert(eqp(full_prow(2), {Cell(2).flip(),Cell(2).flip()}));
   assert(eqp(full_psquare(0), {}));
   assert(eqp(full_psquare(1), {{Cell(1).flip()}}));
   const Cell c2 = Cell(2).flip();
   assert(eqp(full_psquare(2), {{c2,c2}, {c2,c2}}));
  }

  {for (size_t N = 0; N <= 4; ++N) {
     const PSquare p(N);
     assert(p.ps.size() == N);
     assert(p.consistent());
     assert(p.unit() == (N <= 1));
     assert(p.elimvals() == 0);
     assert(not p.restricted());
     assert(p.restricted_count() == 0);
     assert(p == PSquare(ps_map_t{}, N, 0));
   }
  }
  {PSquare p(
             ps_map_t { {{0,1}, Cell{1,0,0,0,1}}, {{4,4},flip(Cell(4))} },
             4, 0);
   assert(p.consistent());
   assert(not p.unit());
   assert(p.elimvals() == 1);
   assert(p.restricted());
   assert(p.restricted_count() == 1);
   assert(p.ps[0][1] == flip(Cell(4,0)));
  }

  {std::istringstream ss;
   const PSquares p(2, ss);
   assert(p.N == 2);
   assert(p.psqs.empty());
   assert(p.elimvals() == 0);
   assert(not p.restricted());
   assert(p.restricted_count() == 0);
  }
  {std::istringstream ss("\n\t\n # \n #");
   const PSquares p(2, ss);
   assert(p.N == 2);
   assert(p.psqs.empty());
   assert(p.elimvals() == 0);
   assert(not p.restricted());
   assert(p.restricted_count() == 0);
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
   assert(p.elimvals() == 0);
  }
  {std::istringstream ss("0\n- -\n- -\n");
   const PSquares p(2, ss);
   assert(p.N == 2);
   assert(eqp(p.psqs, {2}));
   assert(p.elimvals() == 0);
  }
  {std::istringstream ss("0\n+ +\n+ +\n");
   const PSquares p(2, ss);
   assert(p.N == 2);
   assert(p.elimvals() == 8);
   assert(p.restricted());
   assert(p.restricted_count() == 4);
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
   assert(p0.elimvals() == 2 + 2 + 1);
   assert(p0.restricted());
   assert(p0.restricted_count() == 3);
   const PSquare p1({{Cell(2),Cell({0,1})},{Cell(2),Cell({0,1})}},
                    12);
   assert(valid(p1,2));
   assert(p1.elimvals() == 1 + 1);
   assert(p1.restricted());
   assert(p1.restricted_count() == 2);
   assert(p == PSquares(2,{p0,p1}));
   assert(p.elimvals() == 5 + 2);
   assert(p.restricted());
   assert(p.restricted_count() == 3 + 2);
  }

}
