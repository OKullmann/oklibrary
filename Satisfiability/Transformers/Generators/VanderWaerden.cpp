// Oliver Kullmann, 31.5.2004 (Swansea)
/* Copyright 2004 - 2007, 2009, 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/Transformers/Generators/VanderWaerden.hpp
  \brief Application for creating SAT instances expressing van-der-Waerden problems
  \deprecated Needs a complete update.

  \details
  <ul>
   <li> Yet only binary diagonal vdW-problems can be generated. </li>
   <li> The two command-line parameters are the progression size and the
   number of vertices. </li>
  </ul>

  \todo Complete update needed
  <ul>
   <li> Use InputOutput::HypergraphTwoColouring in
   Interfaces/InputOutput/Colouring.hpp. </li>
  </ul>
*/

#include <iostream>

#include <boost/lexical_cast.hpp>

#include <OKlib/General/ErrorHandling.hpp>

#include <OKlib/Satisfiability/Transformers/Generators/VanderWaerden.hpp>

int main(const int argc, const char* const argv[]) {
    if (argc <= 2) {
    std::cerr << "ERROR[VanderWaerden]: Two arguments are needed: The size of the arithmetic progression, and the number of elements.\n";
    return 1;
  }
  try {
    typedef unsigned int Index;
    const Index m = boost::lexical_cast<unsigned int>(argv[1]);
    const Index n = boost::lexical_cast<unsigned int>(argv[2]);
    VanderWaerden::VanderWaerden_TwoParts_1 vdW(m,n, std::cout);
    vdW();
  }
  catch (const std::exception& e) {
    std::cerr << ErrorHandling::Error2string(e);
  }

}
