// Oliver Kullmann, 14.3.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/Transformers/Generators/LinInequal.cpp
  \brief Application for encoding linear inequalities as CNF

  Zero or one input parameter are to be used:
   - Two further parameters n, bound (unsigned integers) are read from
     standard input.
   - The clause-set expressing that the sum of n variables is at most
     bound is output (in Dimacs-format, but only the clauses).
   - The variable names are V1, ...., Vn, if no parameter is given, while
     otherwise V is replaced by this parameter (a string).
   - So in order to obtain variable-names 1,...,n, use '""' as parameter.

  Further input parameters are ignored.

*/

#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <string>

#include <OKlib/Satisfiability/Transformers/Generators/LinInequal.hpp>

#include <OKlib/General/StringHandling.hpp>

int main(const int argc, const char* const argv[]) {

  const std::string input_prefix = "V";
  const std::string output_prefix = "S";
  const std::string auxiliary_prefix = "H";
  const std::string var = (argc == 2) ? argv[1] : input_prefix;
  unsigned int n, bound;
  std::cin >> n >> bound;
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
      LinInequal::compvar_le(S.begin(), S.end(), bound, std::cout);
    }
  }
}

