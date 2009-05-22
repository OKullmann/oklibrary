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
  The result is printed to standard output, also a clause-set in DIMACS format.
*/

#include <fstream>
#include <iostream>

#include <OKlib/Satisfiability/FiniteFunctions/SATParser.hpp>
#include <OKlib/Satisfiability/FiniteFunctions/QuineMcCluskey.hpp>

int main(int argc, const char* argv[]) {
  std::ifstream inputFile (argv[1]);
  std::vector<std::vector<int> > clauseSet = readDIMACSFormat(&inputFile); 
  std::vector<std::vector<int> > resultSet = quineMcCluskey(clauseSet);
  // List Clauses
  for (std::vector<std::vector<int> >::iterator iter = resultSet.begin(); iter != resultSet.end(); iter++) {
    printClause(*iter); 
  }
}
