// Tony H. Bao, 12.07.2005 (Swansea)

#include <iostream>

#include "TestBaseClass.hpp"

#include "DPv_Testobjects.hpp"
#include "Input_output_Testobjects.hpp"
#include "BasicDataStructure_Testobjects.hpp"
#include "Statistics_Testobjects.hpp"

int main(const int argc, const char* const argv[]) {
  return OKlib::TestSystem::TestBase::run_tests_default(std::cerr);
}
