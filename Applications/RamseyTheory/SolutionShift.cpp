// Oliver Kullmann, 4.12.2010 (Swansea)
/* Copyright 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Applications/RamseyTheory/SolutionShift.cpp
  \brief Adding d>=1 to all variables in a partial assignment

  Called with parameters d, reads a sequence of integers +-v, v<>0,
  from standard input and outputs to standard output the sequence of +-(v+d).
  If no argument is given, the default d=1 is used.

  The solution is stored so that interleaving of input and output is avoided,
  when both come from a terminal.

*/

#include <iostream>
#include <vector>

#include <boost/lexical_cast.hpp>

namespace {

#  ifndef INT_TYPE_SOL
#    define INT_TYPE_SOL int
#  endif

  typedef INT_TYPE_SOL int_type;

  enum {
    errcode_parameter = 1,
    errcode_parameter_values = 2,
    errcode_neg_parameter = 3,
    errcode_null_literal = 4
  };
        
  const std::string program = "SolutionShift";
  const std::string err = "ERROR[" + program + "]: ";

}

int main(const int argc, const char* const argv[]) {

  if (argc > 2) {
    std::cerr << err << "At most one argument is needed, the shift d "
      "(default is d=1).\n";
    return(errcode_parameter);
  }

  try {

    const int_type d = (argc == 1) ? 1 : boost::lexical_cast<int_type>(argv[1]);
    if (d < 0) {
      std::cerr << err << "The parameter must be non-negative.\n";
      return(errcode_neg_parameter);
    }

    typedef std::vector<int_type> v_type;
    v_type solution;

    // reading of solution:
    for (int_type x; std::cin >> x; solution.push_back(x))
      if (x == 0) {
        std::cerr << err << "Illegal literal 0.\n";
        return(errcode_null_literal);
      }

    // output of shifted solution:
    {typedef v_type::const_iterator iterator;
     const iterator end = solution.end();
     for (iterator i = solution.begin(); i != end; ++i)
       std::cout << ((*i > 0) ? *i+d : -((-*i)+d)) << " ";
    }
    std::cout << std::endl;
  }
  catch (boost::bad_lexical_cast&) {
    std::cerr << err << "The parameter must be an integer\n"
      "in the range given by INT_TYPE_SOL (default = int).\n";
    return(errcode_parameter_values);
  }

}

