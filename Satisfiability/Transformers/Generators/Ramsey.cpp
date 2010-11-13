// Oliver Kullmann, 27.7.2004 (Turin)
/* Copyright 2004 - 2007, 2009, 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/Transformers/Generators/Ramsey.cpp
  \brief Application for creating 2-colour non-diagonal Ramsey instances
  (arbitrary hypergraph size)
  \deprecated Needs a complete update.

  \details
  For the diagonal case (q1=q2=q) this corresponds to
  output_ramsey2_stdname(q,r,N) in
  ComputerAlgebra/Satisfiability/Lisp/Generators/RamseyTheory/RamseyProblems.mac.
*/

#include <iostream>
#include <exception>

#include <OKlib/General/ErrorHandling.hpp>

#include <OKlib/Satisfiability/Transformers/Generators/Ramsey.hpp>

int main(const int argc, const char* const argv[]) {
    if (argc <= 4) {
    std::cerr << "Four arguments are needed: The size of the two monochromatic subsets, the size of the hyperedges, and the number of vertices.\n";
    return 1;
  }
  try {
    typedef unsigned int Index;
    const Index q1 = boost::lexical_cast<unsigned int>(argv[1]);
    const Index q2 = boost::lexical_cast<unsigned int>(argv[2]);
    const Index r = boost::lexical_cast<unsigned int>(argv[3]);
    const Index N = boost::lexical_cast<unsigned int>(argv[4]);
    Ramsey::Ramsey_TwoColours_1<> R(q1, q2, r, N, std::cout);
    R();
  }
  catch (const std::exception& e) {
    std::cerr << ErrorHandling::Error2string(e);
    return 1;
  }

}
