// Oliver Kullmann, 12.7.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file System/ComplexitySystem/MeasureCPUSpeed.cpp
  \brief Application to measure CPU speed for simple operations

  One parameter is accepted, the number of loop-iterations until a time
  measurement is printed; default value is 10^9.
  Output in R-format: header "t".
*/

#include <iostream>
#include <string>

#include <boost/lexical_cast.hpp>
#include <boost/timer.hpp>

namespace {
  
  enum {
    error_parameters = 1,
    error_impossible = 2
  };

  const std::string program = "MeasureCPUSpeed";
  const std::string err = "ERROR[" + program + "]: ";

  const std::string version = "0.0.6";

  typedef unsigned long uint_type;
  const uint_type N_default = 1000000000;

}

int main(const int argc, const char* const argv[]) {
  if (argc > 2) {
    std::cerr << err << "At most one parameter can be used, the "
      "number of loop-iterations until time-measurement.\n"
      "However, the actual number of input parameters was " << argc-1 << ".\n";
    return error_parameters;
  }

  const uint_type N = (argc == 2) ?
    boost::lexical_cast<uint_type>(argv[1]) : N_default;

  std::cout << "# N = " << N << "\n t\n";
  {
   boost::timer T;
   for (uint_type i = 1; true; ++i) {
     T.restart();
     uint_type result = 0;
     for (uint_type j = 0; j < N; ++j) result *= i;
     if (result != 0) {
       std::cerr << "This can never happen.\n"
       " But otherwise the computation is optimised away.\n";
       return error_impossible;
     }
     std::cout << i << " " << T.elapsed() << std::endl;
   }
 }

}
