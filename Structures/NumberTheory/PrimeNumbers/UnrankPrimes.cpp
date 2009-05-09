// Oliver Kullmann, 9.5.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file OKlib/Structures/NumberTheory/PrimeNumbers/UnrankPrimes.cpp
  \brief Computing the ith prime number

  Reads an integer i from standard input, and computes the prime number p
  with index i. For example for input i = 3 the output is 7.

*/

#include <iostream>

#include <gmpxx.h>

int main() {

  mpz_class i, p = 2;
  std::cin >> i;
  for (mpz_class j = 1;
       j < i;
       ++j, mpz_nextprime(p.get_mpz_t(), p.get_mpz_t())) {}

  std::cout << p << "\n";
  
}
