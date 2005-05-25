// Oliver Kullmann, 15.5.2005

#include <iostream>

#include "TestBaseClass.hpp"

#include "std_Iterators_Tests.hpp"
#include "Iterators_Tests.hpp"
#include "std_Basics_Tests.hpp"
#include "Basics_Tests.hpp"

#include "ConceptsMetafunctions_Testobjects.hpp"
#include "LibraryBasics_Testobjects.hpp"

int main() {
  return OKlib::TestSystem::TestBase::run_tests_default(std::cerr);
}
