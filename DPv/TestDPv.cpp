// Tony H. Bao, 12.07.2005 (Swansea)

#include <iostream>

#include <Transitional/Testsystem/TestBaseClass.hpp>

#include <Transitional/DPv/DPv_Testobjects.hpp>
#include <Transitional/DPv/Input_output_Testobjects.hpp>
#include <Transitional/DPv/BasicDataStructure_Testobjects.hpp>
#include <Transitional/DPv/Statistics_Testobjects.hpp>

int main(const int argc, const char* const argv[]) {
  return OKlib::TestSystem::TestBase::run_tests_default(std::cerr);
}
