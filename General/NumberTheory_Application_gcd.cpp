// Oliver Kullmann, 6.11.2004 (Swansea)
/* Copyright 2004 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file NumberTheory_Application_gcd.cpp
  \brief Used for CS-232.
*/

#include <iostream>
#include <exception>
#include <cstdlib>
#include <sstream>

#include <boost/lexical_cast.hpp>

#include <OKlib/General/ErrorHandling.hpp>

#include <OKlib/General/NumberTheory_Models.hpp>

int main(const int argc, const char* const argv[]) {

  if (argc != 3) {
    std::cerr << "Exactly two integer arguments are needed.\n";
    // TO IMPROVE: return value
    return EXIT_FAILURE;
  }

  typedef int Int;
  Int a, b;
  try {
    a = boost::lexical_cast<Int>(argv[1]);
    b = boost::lexical_cast<Int>(argv[2]);
  }
  catch (const std::exception& e) {
    std::cerr << ErrorHandling::Error2string(e);
    return EXIT_FAILURE;
  }

  typedef NumberTheory::GcdExtVisitor_output<Int> visitor_type;
  typedef NumberTheory::Gcd_extended<Int, visitor_type> Gcd_ext;
  typedef Gcd_ext::result_type result_type;

  std::ostringstream euc_seq;
  std::ostringstream euc_ext_seq;
  visitor_type visitor(euc_seq, euc_ext_seq);
  Gcd_ext gcd_ext(visitor);
  
  const result_type r = gcd_ext(a,b);
  std::cout << "a = " << a << ", b = " << b << "\ngcd = " << r.c << "\nx = " << r.x << ", y = " << r.y << "\n";
  std::cout << "Euclidian sequence:\n" << euc_seq.str() << "\n";
  std::cout << "Euclidian extension sequence:\n" << euc_ext_seq.str() << "\n";
}
