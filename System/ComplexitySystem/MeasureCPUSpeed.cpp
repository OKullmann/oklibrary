// Oliver Kullmann, 12.7.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file System/ComplexitySystem/MeasureCPUSpeed.cpp
  \brief Application to measure CPU speed for simple operations

  <ul>
   <li> One parameter is accepted, the number N of loop-iterations until a time
   measurement is printed; default value is 10^9. </li>
   <li> Runs N times through a simple arithmetic operation, outputting timing
   data. </li>
   <li> Runs in an endless loop. </li>
   <li> Output in R-format: header "te tew tw". </li>
   <li> For "elapsed time", "elapsed wall-clock time", "wall-clock time". </li>
  </ul>
*/

#include <iostream>
#include <string>
#include <iomanip>

#include <boost/lexical_cast.hpp>
#include <boost/timer.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

namespace {
  
  enum {
    error_parameters = 1,
    error_impossible = 2
  };

  const std::string program = "MeasureCPUSpeed";
  const std::string err = "ERROR[" + program + "]: ";

  const std::string version = "0.1.2";

  typedef unsigned long uint_type;
  const uint_type N_default = 1000000000;
  const unsigned int digits_fractional_part = 2;

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

  std::cout << "# Program: " << program << "\n# N = " << N <<
    "\n# time-elapsed(s) time-elapsed-wall-clock time-wall-clock\n" <<
    " te tew tw\n";
  std::cout << std::fixed << std::showpoint << std::setprecision(digits_fractional_part);
  {boost::timer T;
   boost::posix_time::ptime old_time = boost::posix_time::microsec_clock::universal_time();
   for (uint_type i = 1; true; ++i) {
     T.restart();
     {uint_type result = 2;
      for (uint_type j = 0; j < N; ++j) result *= i;
      if (result % 2 != 0) {
        std::cerr << "This can never happen.\n"
        " But otherwise the computation is optimised away.\n";
        return error_impossible;
      }
     }
     const double elapsed = T.elapsed();
     const boost::posix_time::ptime new_time = boost::posix_time::microsec_clock::universal_time();
     std::cout << i << " " << elapsed << " " << new_time - old_time << " "
       << new_time.time_of_day() << std::endl;
     old_time = new_time;
   }
  }

}
