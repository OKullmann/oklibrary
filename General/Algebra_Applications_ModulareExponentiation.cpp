// Oliver Kullmann, 6.11.2004 (Swansea)

#include <iostream>
#include <exception>

#include <boost/lexical_cast.hpp>

#include "ErrorHandling.hpp"

#include "Algebra_Models.hpp"
#include "Algorithms.hpp"

int main(const int argc, const char* const argv[]) {

  if (argc != 4) {
    std::cerr << "Exactly three integer arguments are needed (modulus, basis, exponent).\n";
    // TO IMPROVE: return value
    return 1;
  }

  typedef int Int;
  Int n, b, e;
  try {
    n = boost::lexical_cast<Int>(argv[1]);
    b = boost::lexical_cast<Int>(argv[2]);
    e = boost::lexical_cast<Int>(argv[3]);
  }
  catch (const std::exception& e) {
    std::cerr << ErrorHandling::Error2string(e);
    return 1;
  }

  if (n <= 0) {
    std::cerr << "The modulus must be a positive number, while n = " << n << " is non-positive.\n";
    return 1;
  }
  if (e < 0) {
    std::cerr << "The exponent must be a non-negative number, while e = " << n << " is negative.\n";
    return 1;
  }

  const Int n_fixed = 3953; // TO IMPROVE: runtime determination of modulus!

  std::cout << "modulus n = " << n_fixed << "\nbasis b = " << b << "\nexponent e = " << e << "\nResult = " << Algorithms::power_natural(Algebra::Z_mod_n<n_fixed, Int>(b), e) << "\n";
}
