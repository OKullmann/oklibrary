// Oliver Kullmann, 12.8.2004 (Swansea)

#include <iostream>
#include <exception>

#include "ErrorHandling.hpp"

#include "Ramsey.hpp"
#include "Tests_Ramsey.hpp"

int main() {
  try {

    Tests_Ramsey::test_enumeration_hyperedges<Ramsey::Enumerate_hyperedges<> >();
  }
  catch (const ErrorHandling::Error& e) {
    std::cerr << ErrorHandling::Error2string(e) << "\n";
    return 1;
  }
  catch (const std::exception& e) {
    std::cerr << ErrorHandling::Error2string(e) << "\n";
    return 1;
  }
  catch (...) {
    std::cerr << "UNKNOWN EXCEPTION CAUGHT (neither derived from ErrorHandling::Error nor from std::exception)!\n";
    return 1;
  }
  std::cout << std::endl;

}
