// Oliver Kullmann, 22.5.2009 (Swansea)
/* Copyright 2009, 2010, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/FiniteFunctions/QuineMcCluskey.cpp
  \brief Application for computing all minimal resolvents for a full clause-set

  One parameter is needed, the file containing the clause-set in DIMACS format.
  The result is printed to standard output (again a clause-set in DIMACS format).
*/

#include <fstream>
#include <iostream>
#include <string>

#include <OKlib/Satisfiability/Interfaces/InputOutput/Dimacs.hpp>
#include <OKlib/Satisfiability/Interfaces/InputOutput/ClauseSetAdaptors.hpp>
#include <OKlib/Satisfiability/FiniteFunctions/QuineMcCluskey.hpp>

namespace {

  enum {
    error_parameters = 1,
    error_openfile = 2,
    error_readfile = 3
  };

  const std::string version = "0.1.14";

}

// Move this elsewhere      
#ifdef NUMBER_VARIABLES
        const int num_vars = NUMBER_VARIABLES;
#else
        const int num_vars = 4;
#endif

int main(const int argc, const char* const argv[]) {
  typedef OKlib::InputOutput::RawDimacsCLSAdaptor<> CLSAdaptor;
  typedef OKlib::InputOutput::StandardDIMACSInput<CLSAdaptor> CLSInput;

  if (argc != 2) {
    std::cerr << "ERROR[QuineMcCluskey]: Exactly one input is required, the "
      "name of the file\n with the clause-set in DIMACS-format.\n"
      "However, the actual number of input parameters was " << argc-1 << ".\n";
    return error_parameters;
  }

  const std::string filename = argv[1];
  std::ifstream inputfile(filename.c_str());
  if (not inputfile) {
    std::cerr << "ERROR[QuineMcCluskey]: Failure opening file " << filename << ".\n";
    return error_openfile;
  }

  CLSAdaptor cls_F;
  const CLSInput input_F(inputfile, cls_F); inputfile.close();

  
  const std::string comment("All prime implicates for " + filename);

  using namespace OKlib::Satisfiability::FiniteFunctions;
  OKlib::InputOutput::List2DIMACSOutput(
    quine_mccluskey<num_vars>(cls_F.clause_set),std::cout,comment.c_str());

}
