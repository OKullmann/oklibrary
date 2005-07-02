#include <iostream>

#include "TestBaseClass.hpp"

#include "DerivedRelations_Testobjects.hpp"
#include "OrderConstructions_Testobjects.hpp"

int main() {
  return OKlib::TestSystem::TestBase::run_tests_default(std::cout);
}
