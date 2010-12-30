// Oliver Kullmann, 30.12.2010 (Swansea)
/* Copyright 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/Reductions/Bases/RUcpBase.cpp
  \brief Application for computing a random UCP-base of a clause-set

  For the specification see  rand_rbase_cs in
  ComputerAlgebra/Satisfiability/Lisp/Reductions/RBases.mac.

  <ul>
   <li> The input is a clause-list in DIMACS format on standard input. </li>
   <li> An r_1-base is output to standard output. </li>
   <li> One optional argument is allowed, an integer used as seed for the
   random-number generator; the default-seed is 1. </li>
  </ul>

  \todo Provide implementation

  \todo Improve implementation
  <ul>
   <li> Use messages. </li>
   <li> The Boost-error-message is not very informative. </li>
  </ul>

*/

#include <string>
#include <iostream>

#include <boost/lexical_cast.hpp>

namespace {

  enum { errcode_parameter = 1, errcode_parameter_value = 2 };
        
  const std::string program = "RUcpBase";
  const std::string err = "ERROR[" + program + "]: ";

  const std::string version = "0.0.2";

}

int main(const int argc, const char* const argv[]) {
  if (argc >= 3) {
    std::cerr << err << "At most one arguments is allowed "
      "(the seed for the random-number generator).\n";
    return errcode_parameter;
  }

  try {

    const int seed = (argc == 1) ? 1 : boost::lexical_cast<int>(argv[1]);

   }
  catch (const boost::bad_lexical_cast& e) {
    std::cerr << err << "The seed \"" << argv[1] << "\" must be an integer fitting into type int.\n";
    std::cerr << "The exception message is \n  " << e.what() << "\n";
    return(errcode_parameter_value);
  }

}

 
