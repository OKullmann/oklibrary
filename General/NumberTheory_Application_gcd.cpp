// Oliver Kullmann, 6.11.2004 (Swansea)

#include <iostream>
#include <exception>

#include <boost/lexical_cast.hpp>

#include "ErrorHandling.hpp"

#include "NumberTheory_Models.hpp"

int main(const int argc, const char* const argv[]) {

  if (argc != 3) {
    std::cerr << "Exactly two integer arguments are needed.\n";
    // TO IMPROVE: return value
    return 1;
  }

  typedef int Int;
  Int a, b;
  try {
    a = boost::lexical_cast<Int>(argv[1]);
    b = boost::lexical_cast<Int>(argv[2]);
  }
  catch (const std::exception& e) {
    std::cerr << ErrorHandling::Error2string(e);
    return 1;
  }

  typedef NumberTheory::Gcd_extended<Int> Gcd;
  typedef Gcd::result_type result_type;
  Gcd gcd;
  
  const result_type r = gcd(a,b);
  std::cout << "a = " << a << ", b = " << b << "\ngcd = " << r.c << "\nx = " << r.x << ", y = " << r.y << "\n";
}
