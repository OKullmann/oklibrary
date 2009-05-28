// Oliver Kullmann, 22.5.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/FiniteFunctions/QuineMcCluskey.cpp
  \brief Application for computing all minimal resolvents from a given full clause-set

  One parameter is needed, the file containing the clause-set in DIMACS format.
  The result is printed to standard output (again a clause-set in DIMACS format).
*/

#include <fstream>
#include <iostream>
#include <string>

#include <OKlib/Satisfiability/FiniteFunctions/SATParser.hpp>
#include <OKlib/Satisfiability/FiniteFunctions/QuineMcCluskey.hpp>

namespace {

  const int error_parameters = 1;
  const int error_openfile = 2;
  const int error_readfile = 3;

  const std::string version = "0.1.0";

}

int main(const int argc, const char* const argv[]) {

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

  using namespace OKlib::Satisfiability;

  const FiniteFunctions::ClauseSet clause_set = FiniteFunctions::readDIMACSFormat(inputfile);
  if (not inputfile) {
    std::cerr << "ERROR[QuineMcCluskey]: Failure reading file " << filename << ".\n";
    return error_readfile;
  }
    
  FiniteFunctions::printClauseSet(FiniteFunctions::quineMcCluskey(clause_set));

}
