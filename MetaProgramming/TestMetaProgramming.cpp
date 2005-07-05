// Oliver Kullmann, 26.5.2005

#include <iostream>

#include "TestBaseClass.hpp"

#include "Logical_Testobjects.hpp"
#include "TypeTraits_Testobjects.hpp"
#include "TaggingPolymorphism_Testobjects.hpp"
#include "TypeTraitsContainer_Testobjects.hpp"

int main() {
  return OKlib::TestSystem::TestBase::run_tests_default(std::cerr);
}
