// Oliver Kullmann, 13.12.2009 (Swansea)
/* Copyright 2009, 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/Transformers/Generators/Musatd2.cpp
  \brief Application for creating the saturated minimally unsatisfiable clause-sets of deficiency 2 (in Dimacs format)

  The Maxima-specification is musatd2_fcl(n) (see
  ComputerAlgebra/Satisfiability/Lisp/MinimalUnsatisfiability/Deficiency2.mac).

  If a second parameter is given, then this must be a literal, and this literal
  is added as a unit-clause; in case x=0 actually nothing is added.

  \todo Write application tests

*/

#include <string>
#include <iostream>

#include <boost/lexical_cast.hpp>

namespace {

  typedef int literal_type;

  enum { errcode_parameter = 1, errcode_neg_parameter = 2,
         errcode_parameter_values = 3 };

  const std::string program = "Musatd2";
  const std::string err = "ERROR[" + program + "]: ";

}

int main(const int argc, const char* const argv[]) {
    if (argc < 2 or argc > 3) {
    std::cerr << err << "One argument is needed, the number n of variables, while a second parameter x is optional"
    "\n(this literal is added as a unit-clause).\n";
    return errcode_parameter;
  }

  try {

    const literal_type n = boost::lexical_cast<literal_type>(argv[1]);
    if (n < 2) {
      std::cerr << err << "The parameter must be at least 2.\n";
      return errcode_neg_parameter;
    }

    const literal_type x = (argc == 3) ? boost::lexical_cast<literal_type>(argv[2]) : 0;

    std::cout << "c Saturated minimally unsatisfiable clause-set of deficiency 2 with " << n << " variables";
    if (x == 0) std::cout << ".\n";
    else std::cout << ",\nc and with added unit-clause " << x << ".\n";

    std::cout << "p cnf " << n << " ";
    if (x == 0) std::cout << n+2;
    else std::cout << n+3;
    std::cout << "\n";

    for (literal_type i = 1; i <= n; ++i)
      std::cout << i << " ";
    std::cout << "0\n";
    for (literal_type i = 1; i <= n; ++i)
      std::cout << -i << " ";
    std::cout << "0\n";
    for (literal_type i = 1; i < n; ++i)
      std::cout << -i << " " << i+1 << " 0\n";
    std::cout << -n << " " << 1 << " 0\n";

    if (x != 0)
      std::cout << x << " 0\n";
  }
  catch (boost::bad_lexical_cast&) {
    std::cerr << err << "All parameters must be integers fitting into type int.\n";
    return(errcode_parameter_values);
  }

}
