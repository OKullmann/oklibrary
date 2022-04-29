// Oliver Kullmann, 27.4.2022 (Swansea)
/* Copyright 2022 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#include <iostream>

#include <cassert>

#include <ProgramOptions/Environment.hpp>

#include "OrthogonalArrays.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.1.0",
        "29.4.2022",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Solvers/Gecode/MOLS/TestOrthogonalArrays.cpp",
        "GPL v3"};

  using namespace OrthogonalArrays;

  template <class X>
  constexpr bool eqp(const X& lhs, const X& rhs) noexcept {
    return lhs == rhs;
  }
}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv))
  return 0;

  {assert(max(ls_row_t{}) == 0);
   assert(max(ls_row_t{0,7,4}) == 7);
   assert(max(oa_t{}) == 0);
   assert(max(oa_t{{0,4,2},{7,2}}) == 7);
  }

  {// projection XXX

  }

  {assert(eqp(subsets(0,0), {{}}));
   assert(eqp(subsets(0,1), {}));
   assert(eqp(subsets(1,0), {{}}));
   assert(eqp(subsets(1,1), {{0}}));
   assert(eqp(subsets(1,2), {}));
   assert(eqp(subsets(2,0), {{}}));
   assert(eqp(subsets(2,1), {{0},{1}}));
   assert(eqp(subsets(2,2), {{0,1}}));
   assert(eqp(subsets(2,3), {}));
   assert(eqp(subsets(3,0), {{}}));
   assert(eqp(subsets(3,1), {{0},{1},{2}}));
   assert(eqp(subsets(3,2), {{0,1},{0,2},{1,2}}));
   assert(eqp(subsets(3,3), {{0,1,2}}));
   assert(eqp(subsets(3,4), {}));
   assert(eqp(subsets(4,0), {{}}));
   assert(eqp(subsets(4,1), {{0},{1},{2},{3}}));
   assert(eqp(subsets(4,2), {{0,1},{0,2},{1,2},{0,3},{1,3},{2,3}}));
   assert(eqp(subsets(4,3), {{0,1,2},{0,1,3},{0,2,3},{1,2,3}}));
   assert(eqp(subsets(4,4), {{0,1,2,3}}));
   assert(eqp(subsets(4,5), {}));
   {bool caught = false;
    try { subsets(68,34); }
    catch(std::runtime_error&) { caught = true; }
    assert(caught);
   }
  }

  {assert(GenLS<0>::create(3) == 3);
   assert(GenLS<1>::create(5) == ls_row_t(5,5));
   assert(GenLS<2>::create(7) == ls_t(7,ls_row_t(7,7)));
   assert(GenLS<3>::create(4) == GLS<3>(4, ls_t(4,ls_row_t(4,4))));
  }

  {// OrthArr XXX

  }

}
