// Oliver Kullmann, 15.5.2005

#include <iostream>

#include "TestBaseClass.hpp"

#include "std_Iterators_Testobjects.hpp"
#include "Iterators_Testobjects.hpp"
#include "std_Basics_Testobjects.hpp"
#include "Basics_Testobjects.hpp"
#include "ConceptsMetafunctions_Testobjects.hpp"
#include "LibraryBasics_Testobjects.hpp"
#include "Variables_Testobjects.hpp"

int main() {
  return OKlib::TestSystem::TestBase::run_tests_default(std::cerr);
}
