// Oliver Kullmann, 24.5.2005 (Swansea)

#include <iostream>

#include <Transitional/TestSystem/TestBaseClass.hpp>

#include <Transitional/Parsing/ParserBase_Testobjects.hpp>

int main(const int argc, const char* const argv[]) {
  return OKlib::TestSystem::TestBase::run_tests_default(std::cerr);
}
