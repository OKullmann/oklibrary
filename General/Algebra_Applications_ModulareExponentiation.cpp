// Oliver Kullmann, 6.11.2004 (Swansea)

/*!
  \file Algebra_Applications_ModulareExponentiation.cpp
  \brief Used for CS-232.
*/

#include <iostream>
#include <exception>
#include <cstdlib>

#include <boost/lexical_cast.hpp>

#include "ErrorHandling.hpp"

#include "Algebra_Models.hpp"
#include "Algorithms.hpp"

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
