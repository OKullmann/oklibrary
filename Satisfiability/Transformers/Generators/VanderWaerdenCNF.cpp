// Oliver Kullmann, 31.5.2004 (Swansea)
/* Copyright 2004 - 2007, 2009, 2010, 2011, 2021 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/Transformers/Generators/VanderWaerdenCNF.cpp
  \brief Application for creating SAT instances expressing van-der-Waerden problems
  \deprecated Needs update.

  \details
  <ul>
   <li> Three command-line parameters "k1 k2 n" are needed for the two
   progression sizes k1, k2 and for the number n of vertices. </li>
   <li> The output by default is put into file "VanDerWaerden_2-k1-k2_n.cnf".
   </li>
   <li> An optional fourth argument S is treated as follows:
    <ol>
     <li> If S="-", then output is put to standard output. </li>
     <li> Otherwise the output is put into a file named S. </li>
    </ol>
   </li>
   <li> For the Maxima specification see
   output_vanderwaerden2nd_stdname(k1,k2,n) in
   ComputerAlgebra/Satisfiability/Lisp/Generators/RamseyTheory/VanderWaerdenProblems.mac.
   </li>
  </ul>

  \todo Update needed
  <ul>
   <li> Update to the new system (local). </li>
   <li> Use InputOutput::HypergraphTwoColouring in
   Interfaces/InputOutput/Colouring.hpp. </li>
   <li> Improve error-output in case an exception is thrown. </li>
  </ul>
*/

#include <iostream>
#include <string>
#include <fstream>

#include <boost/lexical_cast.hpp>

#include <OKlib/General/ErrorHandling.hpp>

#include <OKlib/Combinatorics/Hypergraphs/Generators/VanderWaerden.hpp>
#include <OKlib/Satisfiability/Transformers/Generators/VanderWaerdenCNF.hpp>

namespace {

  enum {
    error_parameters = 1,
    error_length_ap = 2,
    error_num_vert = 3,
    error_file = 4
  };

  const std::string program = "VanderWaerdenCNF";
  const std::string err = "ERROR[" + program + "]: ";

  const std::string version = "0.3.7";

}

int main(const int argc, const char* const argv[]) {
  if (argc != 4 and argc != 5) {
    std::cerr << err << "Either three or four arguments are needed:\n"
    " Two arithmetic progression sizes, the number of elements,\n"
    " and optionally either \"-\" for output to standard output or a different string for a filename.\n";
    return error_parameters;
  }
  try {
    typedef unsigned int Index;
    const Index k1 = boost::lexical_cast<unsigned int>(argv[1]);
    const Index k2 = boost::lexical_cast<unsigned int>(argv[2]);
    if (k1 < 2 or k2 < 2) {
      std::cerr << err << "Arithmetic progression lengths must be at least 2.\n";
      return error_length_ap;
    }
    const Index n = boost::lexical_cast<unsigned int>(argv[3]);
    if (n < 2) {
      std::cerr << err << "At least two vertices are required.\n";
      return error_num_vert;
    }

    const std::string filename = (argc == 5) ?
      (std::string(argv[4]) == "-" ? "" : argv[4]) :
      std::string("VanDerWaerden_2-") + argv[1] + "-" + argv[2] + "_" + argv[3] + ".cnf";

    if (filename.empty())
      OKlib::Satisfiability::Transformers::Generators::VanderWaerden_TwoParts_1
        <Index,
         OKlib::Combinatorics::Hypergraphs::Generators::Arithmetical_progressions_colex>
       (k1,k2, n, std::cout)();
    else {
      std::ofstream out(filename.c_str());
      if (not out) {
        std::cerr << err << "Couldn't open file " << filename << " for writing.\n";
        return error_file;
      }
      OKlib::Satisfiability::Transformers::Generators::VanderWaerden_TwoParts_1
        <Index,
         OKlib::Combinatorics::Hypergraphs::Generators::Arithmetical_progressions_colex>
       (k1,k2, n, out)();
    }
  }
  catch (const std::exception& e) {
    std::cerr << ErrorHandling::Error2string(e);
  }

}
