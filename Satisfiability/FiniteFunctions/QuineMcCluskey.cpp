// Oliver Kullmann, 22.5.2009 (Swansea)
/* Copyright 2009, 2010, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/FiniteFunctions/QuineMcCluskey.cpp
  \brief Application for computing all minimal resolvents for a full clause-set

  <ul>
   <li> One parameter is needed, the file with the full clause-set in DIMACS
   format. </li>
   <li> The result is printed to standard output (again a clause-set in DIMACS
   format). </li>
  </ul>
*/

#include <fstream>
#include <iostream>
#include <string>

#include <OKlib/Satisfiability/Interfaces/InputOutput/Dimacs.hpp>
#include <OKlib/Satisfiability/Interfaces/InputOutput/ClauseSetAdaptors.hpp>
#include <OKlib/Satisfiability/FiniteFunctions/QuineMcCluskey.hpp>

namespace {
  
#ifdef NUMBER_VARIABLES
  const int num_vars = NUMBER_VARIABLES;
#else
  const int num_vars = 4;
#endif

  enum {
    error_parameters = 1,
    error_openfile = 2
  };

  const std::string program = "QuineMcCluskey";
  const std::string err = "ERROR[" + program + "]: ";

  const std::string version = "0.2.1";

}

int main(const int argc, const char* const argv[]) {
  if (argc != 2) {
    std::cerr << err <<
      "Exactly one parameter is required, the name of the file\n"
      " with the (full) clause-set in DIMACS-format.\n"
      "However, the actual number of input parameters was " << argc-1 << ".\n";
    return error_parameters;
  }

  const std::string filename = argv[1];
  std::ifstream inputfile(filename.c_str());
  if (not inputfile) {
    std::cerr << err << "Failure opening file " << filename << ".\n";
    return error_openfile;
  }

  typedef OKlib::InputOutput::RawDimacsCLSAdaptor<> CLSAdaptor;
  CLSAdaptor cls_F;
  const OKlib::InputOutput::StandardDIMACSInput<CLSAdaptor> input_F(inputfile, cls_F);
  inputfile.close();
  const std::string comment("All prime implicates for " + filename);
  OKlib::InputOutput::List2DIMACSOutput(
    OKlib::Satisfiability::FiniteFunctions::quine_mccluskey<num_vars>(cls_F.clause_set),
    std::cout,
    comment.c_str());

}
