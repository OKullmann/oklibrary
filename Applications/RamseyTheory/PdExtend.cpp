// Oliver Kullmann, 3.12.2010 (Swansea)
/* Copyright 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Applications/RamseyTheory/PdExtend.cpp
  \brief Extending a palindromic vdW-solution to a full solution

  Called with parameters N, reads a sequence of integers +-v for 
  1 <= v <= ceil(N/2) from standard input and outputs to standard output
  this sequence, followed by the sequence of +-v' for v' = N - v + 1,
  in backwards order.

*/

#include <iostream>
#include <vector>
#include <cstdlib>

#include <boost/lexical_cast.hpp>

namespace {

#  ifndef INT_TYPE_VDW
#    define INT_TYPE_VDW int
#  endif

  typedef INT_TYPE_VDW int_type;

  enum {
    errcode_parameter = 1,
    errcode_parameter_values = 2,
    errcode_neg_parameter = 3,
    errcode_null_literal = 4,
    errcode_too_big_var = 5
  };
        
  const std::string program = "PdExtend";
  const std::string err = "ERROR[" + program + "]: ";

}

int main(const int argc, const char* const argv[]) {

  if (argc != 2) {
    std::cerr << err << "Exactly one argument is needed, the number N of "
      "vertices.\n";
    return(errcode_parameter);
  }

  try {

    const int_type N = boost::lexical_cast<int_type>(argv[1]);
    if (N < 0) {
      std::cerr << err << "The parameter must be non-negative.\n";
      return(errcode_neg_parameter);
    }
    const int_type middle = (N+1)/2;

    typedef std::vector<int_type> v_type;
    v_type solution;

    // reading of palindromic solution:
    for (int_type x; std::cin >> x; solution.push_back(x)) {
      if (x == 0) {
        std::cerr << err << "Illegal literal 0.\n";
        return(errcode_null_literal);
      }
      if (std::abs(x) > middle) {
        std::cerr << err << "Variable too big: " << x << ".\n";
        return(errcode_too_big_var);
      }
    }

    // output of extended solution:
    {typedef v_type::const_iterator iterator;
     const iterator end = solution.end();
     for (iterator i = solution.begin(); i != end; ++i)
       std::cout << *i << " ";
    }
    {typedef v_type::const_reverse_iterator iterator;
     const iterator end = solution.rend();
     for (iterator i = solution.rbegin(); i != end; ++i)
       std::cout << ((*i > 0) ? N-*i+1 : -(N-(-*i)+1)) << " ";
    }
    std::cout << std::endl;
  }
  catch (boost::bad_lexical_cast&) {
    std::cerr << err << "The parameter must be an integer\n"
      "in the range given by INT_TYPE_VDW (default = int).\n";
    return(errcode_parameter_values);
  }

}

