// Oliver Kullmann, 31.5.2004

#include <iostream>

#include "ErrorHandling.hpp"

#include "VanderWaerden.hpp"

int main(const int argc, const char* const argv[]) {
    if (argc <= 2) {
    std::cerr << "Two arguments are needed: The size of the arithmetic progression, and the number of elements.\n";
    return 1;
  }
  try {
    typedef unsigned int Index;
    const Index m = boost::lexical_cast<unsigned int>(argv[1]);
    const Index n = boost::lexical_cast<unsigned int>(argv[2]);
    VanderWaerden::VanderWaerden_TwoParts_1 vdW(m,n, std::cout);
    vdW();
  }
  catch (const std::exception& e) {
    std::cerr << ErrorHandling::Error2string(e);
  }

}
