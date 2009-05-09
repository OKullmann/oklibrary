// Oliver Kullmann, 9.5.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file OKlib/Structures/NumberTheory/PrimeNumbers/RankPrimes.cpp
  \brief Finding the index of prime numbers

  Reads an integer p_in from standard input, and computes the index i
  of the smallest prime number p with p >= p_in. For example for
  input p_in = 8 the output is i = 5, and the same output is obtained
  for 8 <= p_in <= 11.

*/

#include <iostream>

#include <gmpxx.h>

int main() {

  mpz_class p_in, i = 1;
  std::cin >> p_in;
  for (mpz_class p = 2;
       p < p_in;
       ++i, mpz_nextprime(p.get_mpz_t(), p.get_mpz_t())) {}

  std::cout << i << "\n";
  
}
