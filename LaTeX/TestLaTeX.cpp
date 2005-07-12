// Tony H. Bao, 12.07.2005 (Swansea)

#include <iostream>

#include "TestBaseClass.hpp"

#include "Doc_Gen_Testobjects.hpp"
#include "Doc_Object_Implementation_Testobjects.hpp"
#include "Latex_Handler_Implementation_Testobjects.hpp"
#include "Table_Object_Implementation_Testobjects.hpp"

int main() {
  return OKlib::TestSystem::TestBase::run_tests_default(std::cerr);
}
