// Oliver Kullmann, 6.11.2004 (Swansea)
/* Copyright 2004 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Algebra_Applications_ModulareExponentiation.cpp
  \brief Used for CS-232.
*/

#include <iostream>
#include <exception>
#include <cstdlib>

#include <boost/lexical_cast.hpp>

#include <OKlib/General/ErrorHandling.hpp>

#include <OKlib/General/Algebra_Models.hpp>
#include <OKlib/General/Algorithms.hpp>

int main(const int argc, const char* const argv[]) {

  if (argc != 4) {
    std::cerr << "Exactly three integer arguments are needed (modulus, basis, exponent).\n";
    return EXIT_FAILURE;
  }

  typedef long int Int;
  Int n, b, e;
  try {
    n = boost::lexical_cast<Int>(argv[1]);
    b = boost::lexical_cast<Int>(argv[2]);
    e = boost::lexical_cast<Int>(argv[3]);
  }
  catch (const std::exception& e) {
    std::cerr << ErrorHandling::Error2string(e);
    return EXIT_FAILURE;
  }

  if (n <= 0) {
    std::cerr << "The modulus must be a positive number, while n = " << n << " is non-positive.\n";
    return EXIT_FAILURE;
  }
  if (e < 0) {
    std::cerr << "The exponent must be a non-negative number, while e = " << e << " is negative.\n";
    return EXIT_FAILURE;
  }

  typedef Algebra::Zmodn<Int> mod_type;
  mod_type::modulus = n;
  std::cout << "modulus n = " << n << "\nbasis b = " << b << "\nexponent e = " << e << "\nResult = " << Algorithms::power_natural(mod_type(b), e) << "\n";
}
