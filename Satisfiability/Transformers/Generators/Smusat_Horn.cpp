// Oliver Kullmann, 13.12.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/Transformers/Generators/Smusat_Horn.hpp
  \brief Application for creating the saturated minimally unsatisfiable Horn clause-sets (in Dimacs format)

  The Maxima-specification is smusat_horn_stdfcl(k) (see
  ComputerAlgebra/Satisfiability/Lisp/ConflictCombinatorics/HittingClauseSets.mac).

  \todo Write application tests

*/

#include <string>
#include <iostream>

#include <boost/lexical_cast.hpp>

namespace {

  typedef int literal_type;

  enum { errcode_parameter = 1, errcode_neg_parameter = 2,
         errcode_parameter_values = 3 };

  const std::string program = "Smusat_Horn";
  const std::string err = "ERROR[" + program + "]: ";

}

int main(const int argc, const char* const argv[]) {
    if (argc != 2) {
    std::cerr << err << "Exactly one argument is needed, the number k of variables.\n";
    return errcode_parameter;
  }

  try {

    const literal_type k = boost::lexical_cast<literal_type>(argv[1]);
    if (k < 0) {
      std::cerr << err << "The parameter must be non-negative.\n";
      return errcode_neg_parameter;
    }

    std::cout << "c Saturated minimally unsatisfiable Horn clause-set with " << k << " variables.\n";
    std::cout << "p cnf " << k << " " << k+1 << "\n";

    for (literal_type i = 1; i <= k; ++i) {
      for (literal_type j = 1; j <= i-1; ++j)
        std::cout << -j << " ";
      std::cout << i << " 0\n";
    }
    for (literal_type i = 1; i <= k; ++i)
      std::cout << -i << " ";
    std::cout << "0\n";
    
  }
  catch (boost::bad_lexical_cast&) {
    std::cerr << err << "All parameters must be integers fitting into type int.\n";
    return(errcode_parameter_values);
  }

}
