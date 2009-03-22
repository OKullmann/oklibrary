// Oliver Kullmann, 14.3.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/Transformers/Generators/LinInequal.cpp
  \brief Application for encoding linear inequalities as CNF

  Three or four input parameter are to be used:
   - Two first two parameters are n, bound (unsigned integers).
   - Then we have the comparison operator comp, one of "<=", "==".
   - The clause-set expressing that the sum of n variables compares
     to the bound according to comp is output (in Dimacs-format, but only the
     clauses).
   - The variable names are V1, ...., Vn, if no additional parameter is given,
     while otherwise V is replaced by this parameter (a string).
   - So in order to obtain variable-names 1,...,n, use '""' as additional
     parameter.

  Further input parameters are ignored.

*/

#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <string>

#include <boost/lexical_cast.hpp>

#include <OKlib/General/ErrorHandling.hpp>

#include <OKlib/Satisfiability/Transformers/Generators/LinInequal.hpp>

int main(const int argc, const char* const argv[]) {

  if (argc <= 3) {
    std::cerr << "Three arguments are needed: The number of variables, "
      "the bound, and the comparison operator\n"
      "(one of \"<=\", \"==\").\n";
    return 1;
  }
  const std::string comps = argv[3];
  if (comps != "<=" and comps != "==") {
    std::cerr << "The third argument must be either the string "
      "\"<=\" or the string \"==\".\n";
    return 1;
  }

  try {
    const unsigned int n = boost::lexical_cast<unsigned int>(argv[1]);
    const unsigned int bound = boost::lexical_cast<unsigned int>(argv[2]);
    enum comparisons {le, eq};
    comparisons comp;
    if (comps == "<=") comp = le;
    else comp = eq;
    const std::string input_prefix = "V";
    const std::string output_prefix = "S";
    const std::string auxiliary_prefix = "H";
    const std::string var = (argc >= 5 ) ? argv[4] : input_prefix;
    
    {
      typedef std::vector<std::string> st_vec;
      {
        st_vec X; X.reserve(n);
        for (unsigned int i = 1; i <= n; ++i)
          X.push_back(LinInequal::P(var,i));
        LinInequal::AddVar(X.begin(), X.end(), output_prefix, auxiliary_prefix, std::cout);
      }
      {
        const unsigned int blL = LinInequal::bin_length(n);
        st_vec S; S.reserve(blL);
        for (unsigned int i = 1; i <= blL; ++i)
          S.push_back(LinInequal::P(output_prefix, i));
        switch (comp) {
        case le :
          LinInequal::compvar_le(S.begin(), S.end(), bound, std::cout);
          break;
        case eq :
          LinInequal::compvar_eq(S.begin(), S.end(), bound, std::cout);
          break;
        }
      }
    }
  }
  catch (const std::exception& e) {
    std::cerr << ErrorHandling::Error2string(e);
  }
}

