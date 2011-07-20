// Oliver Kullmann, 12.7.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file System/ComplexitySystem/MeasureCPUSpeedMem.cpp
  \brief Application to measure CPU speed for simple memory-intensive operations

  <ul>
   <li> One parameter is accepted, the size N of the vector of long integers;
   default value is 10^8. </li>
   <li> Assigns to the cells of the vector the current loop-count 10 times.
   </li>
   <li> Runs in an endless loop. </li>
   <li> Output in R-format: header "te tew tw". </li>
   <li> For "elapsed time", "elapsed wall-clock time", "wall-clock time". </li>
  </ul>
*/

#include <iostream>
#include <string>
#include <iomanip>
#include <vector>

#include <boost/lexical_cast.hpp>
#include <boost/timer.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

namespace {
  
  enum {
    error_parameters = 1,
    error_impossible = 2
  };

  const std::string program = "MeasureCPUSpeedMem";
  const std::string err = "ERROR[" + program + "]: ";

  const std::string version = "0.0.9";

  typedef unsigned long uint_type;
  const uint_type N_default = 100000000;
  const uint_type M = 10;
  const unsigned int digits_fractional_part = 2;

}

int main(const int argc, const char* const argv[]) {
  if (argc > 2) {
    std::cerr << err << "At most one parameter can be used, the "
      "size of the integer-vector.\n"
      "However, the actual number of input parameters was " << argc-1 << ".\n";
    return error_parameters;
  }

  const uint_type N = (argc == 2) ?
    boost::lexical_cast<uint_type>(argv[1]) : N_default;

  std::cout << "# N = " << N << "\n# time-elapsed(s) time-elapsed-wall-clock time-wall-clock\n";
  std::cout << " te tew tw\n";
  std::cout << std::fixed << std::showpoint << std::setprecision(digits_fractional_part);
  {std::vector<uint_type> V;
   V.reserve(N);
   boost::timer T;
   boost::posix_time::ptime old_time = boost::posix_time::microsec_clock::universal_time();
   for (uint_type i = 1; true; ++i) {
     T.restart();
     for (uint_type j = 0; j < M; ++j)
       for (uint_type k = 0; k < N; ++k) V[k] = i;
     const double elapsed = T.elapsed();
     const boost::posix_time::ptime new_time = boost::posix_time::microsec_clock::universal_time();
     std::cout << i << " " << elapsed << " " << new_time - old_time << " "
       << new_time.time_of_day() << std::endl;
     old_time = new_time;
   }
  }

}
