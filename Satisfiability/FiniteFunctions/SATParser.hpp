// Oliver Kullmann, 22.5.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/FiniteFunctions/SATParser.hpp
  \brief Reading a file in DIMACS format
  \deprecated
*/

#ifndef SATPARSER_dBBx34E
#define SATPARSER_dBBx34E

#include <iostream>
#include <fstream>
#include <vector>

namespace OKlib {
  namespace Satisfiability {
    namespace FiniteFunctions {
      
      /*!
	\fn std::vector<std::vector<int> > readDIMACSFormat(std::ifstream& inputFile)
	\brief Reads a clause set given in Dimacs format on the given input stream and
	returns it as output.
       */
      std::vector<std::vector<int> > readDIMACSFormat(std::ifstream& inputFile) {
        int c = 0; // Character reading
        int rawLiteral = -1;
        std::vector<std::vector<int> > clauseSet;
        while (inputFile and (c = inputFile.peek()) != EOF) {
          switch(c) {
          case 'c' :
            inputFile.get();
            // Skip
            while(inputFile and (c = inputFile.get()) != '\n');
            break;
          case 'p' :
            inputFile.get();
            // Skip
            while(inputFile and (c = inputFile.get()) != '\n');
            break;
          default :
            std::vector<int> clause;
            inputFile >> rawLiteral;
            while (inputFile and rawLiteral) {
              clause.push_back(rawLiteral);
              inputFile >> rawLiteral;
            }
            clauseSet.push_back(clause);
            // Jump to newline
            c = inputFile.get();
            while(inputFile and (c != '\n') && (c != EOF)) {
              c = inputFile.get();
            }
            break;
          }
        }
        return clauseSet;
      }
      
    }
  }
}

#endif
