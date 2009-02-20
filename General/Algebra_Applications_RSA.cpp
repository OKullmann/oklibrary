// Oliver Kullmann, 1.11.2005 (Swansea)
/* Copyright 2005 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file General/Algebra_Applications_RSA.cpp
  \brief Used for CS-232.

  \todo Use messages.
  \todo Complete input checking.
*/

#include <iostream>
#include <exception>
#include <cstdlib>
#include <string>

#include <boost/lexical_cast.hpp>

#include <OKlib/General/ErrorHandling.hpp>

#include <OKlib/General/Algebra_Models.hpp>
#include <OKlib/General/Algorithms.hpp>

int main(const int argc, const char* const argv[]) {

  if (argc != 4) {
    std::cerr << "Exactly three integer arguments are needed (p, q, e).\nHere p, q are different prime numbers and e is invertible modulo phi(p * q).\nThen you can enter pairs like \"e 55\" (for encrypting the plain text 55),\n and \"d 87\" (for decrypting the cipher text 87).\n";
    return EXIT_FAILURE;
  }

  typedef long int Int;
  Int p, q, e;
  try {
    p = boost::lexical_cast<Int>(argv[1]);
    q = boost::lexical_cast<Int>(argv[2]);
    e = boost::lexical_cast<Int>(argv[3]);
  }
  catch (const std::exception& e) {
    std::cerr << ErrorHandling::Error2string(e);
    return EXIT_FAILURE;
  }

  if (p <= 1) {
    std::cerr << "The first secret prime number must be a at least 2, while p = " << p << ".\n";
    return EXIT_FAILURE;
  }
  if (q <= 1) {
    std::cerr << "The second secret prime number must be a at least 2, while q = " << q << ".\n";
    return EXIT_FAILURE;
  }
  if (e <= 0) {
    std::cerr << "The public key must be a positive number, while e = " << e << " is negative.\n";
    return EXIT_FAILURE;
  }

  typedef Algebra::RSA<Int> RSA;
  RSA rsa(p,q,e);

  std::cout << "p = " << p << "\nq = " << q << "\ne = " << e << "\nd = " << rsa.d << "\nn = " << rsa.n << "\nN = " << rsa.N << "\n";

  for (;;) {
    std::string control;
    std::cin >> control;
    if (control == "e") {
      Int x;
      std::cin >> x;
      std::cout << "enc: " << rsa.encrypt(x) << "\n";
    }
    else if (control == "d") {
      Int x;
      std::cin >> x;
      std::cout << "dec: " << rsa.decrypt(x) << "\n";
    }
    else return 0;
  }
}
