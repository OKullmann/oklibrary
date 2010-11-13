// Oliver Kullmann, 31.5.2004 (Swansea)
/* Copyright 2004 - 2007, 2009, 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/Transformers/Generators/VanderWaerden.cpp
  \brief Application for creating SAT instances expressing van-der-Waerden problems
  \deprecated Needs a complete update.

  \details
  <ul>
   <li> Either two command-line parameters "k n" or three "k1 k2 n",
   where the first case is the diagonal case, the second case is the mixed
   case, "k" denotes progression size and "n" the number of vertices. </li>
  </ul>

  \todo Complete update needed
  <ul>
   <li> Use InputOutput::HypergraphTwoColouring in
   Interfaces/InputOutput/Colouring.hpp. </li>
  </ul>
*/

#include <iostream>
#include <string>

#include <boost/lexical_cast.hpp>

#include <OKlib/General/ErrorHandling.hpp>

#include <OKlib/Satisfiability/Transformers/Generators/VanderWaerden.hpp>

namespace {

  enum {
    error_parameters = 1,
    error_length_ap = 2,
    error_num_vert = 3
  };

  const std::string version = "0.3.3";

}

int main(const int argc, const char* const argv[]) {
  if (argc != 3 and argc != 4) {
    std::cerr << "ERROR[VanderWaerden]: Either two or three arguments are needed:\n"
    " One or two arithmetic progression sizes (diagonal or mixed case), and the number of elements.\n";
    return error_parameters;
  }
  try {
    typedef unsigned int Index;
    const Index k = boost::lexical_cast<unsigned int>(argv[1]);
    const Index k2 = (argc == 4) ? boost::lexical_cast<unsigned int>(argv[2]) : k;
    if (k < 2 or k2 < 2) {
      std::cerr << "ERROR[VanderWaerden]: Arithmetic progression lengths must be at least 2.\n";
      return error_length_ap;
    }
    const Index n = (argc == 3) ? boost::lexical_cast<unsigned int>(argv[2]) : boost::lexical_cast<unsigned int>(argv[3]);
    if (n < 2) {
      std::cerr << "ERROR[VanderWaerden]: At least two vertices are required.\n";
      return error_num_vert;
    }
    OKlib::Satisfiability::Transformers::Generators::VanderWaerden_TwoParts_1
      <Index,
       OKlib::Satisfiability::Transformers::Generators::Arithmetical_progressions_colex>
     (k,k2, n, std::cout)();
  }
  catch (const std::exception& e) {
    std::cerr << ErrorHandling::Error2string(e);
  }

}
