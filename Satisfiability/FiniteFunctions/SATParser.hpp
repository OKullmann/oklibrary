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

#include <OKlib/Satisfiability/FiniteFunctions/QuineMcCluskey.hpp>

namespace OKlib {
  namespace Satisfiability {
    namespace FiniteFunctions {
      
      /*!
	\brief Reads a clause-set given in Dimacs format on the given input
        stream and returns it as output.
      */
      ClauseSets read_dimacs_format(std::ifstream& input_file) {
        int c = 0; // Character reading
        Literals raw_literal = -1;
        ClauseSets clause_set;
        while (input_file and (c = input_file.peek()) != EOF) {
          switch(c) {
          case 'c' :
            input_file.get();
            // Skip
            while(input_file and (c = input_file.get()) != '\n');
            break;
          case 'p' :
            input_file.get();
            // Skip
            while(input_file and (c = input_file.get()) != '\n');
            break;
          default :
            Clauses clause;
            input_file >> raw_literal;
            while (input_file and raw_literal) {
              clause.push_back(raw_literal);
              input_file >> raw_literal;
            }
            clause_set.push_back(clause);
            // Jump to newline
            c = input_file.get();
            while(input_file and (c != '\n') && (c != EOF)) {
              c = input_file.get();
            }
            break;
          }
        }
        return clause_set;
      }
      
    }
  }
}

#endif
