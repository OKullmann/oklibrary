#include <iostream>

#include "TestBaseClass.hpp"

#include "ParserBase_Testobjects.hpp"

int main(const int argc, const char* const argv[]) {
  return OKlib::TestSystem::TestBase::run_tests_default(std::cerr);
}
