// Oliver Kullmann, 15.5.2005

#include <iostream>

#include <Transitional/Testsystem/TestBaseClass.hpp>

#include <Transitional/Concepts/std_Iterators_Testobjects.hpp>
#include <Transitional/Concepts/Iterators_Testobjects.hpp>
#include <Transitional/Concepts/std_Basics_Testobjects.hpp>
#include <Transitional/Concepts/Basics_Testobjects.hpp>
#include <Transitional/Concepts/ConceptsMetafunctions_Testobjects.hpp>
#include <Transitional/Concepts/LibraryBasics_Testobjects.hpp>
#include <Transitional/Concepts/Variables_Testobjects.hpp>
#include <Transitional/Concepts/ResultElements_Testobjects.hpp>
#include <Transitional/Concepts/std_Container_Testobjects.hpp>
#include <Transitional/Concepts/Literals_Testobjects.hpp>
#include <Transitional/Concepts/Clauses_Testobjects.hpp>
#include <Transitional/Concepts/ClauseSets_Testobjects.hpp>
#include <Transitional/Concepts/Assignments_Testobjects.hpp>
#include <Transitional/Concepts/BipartiteGraphs_Testobjects.hpp>
#include <Transitional/Concepts/Hypergraphs_Testobjects.hpp>
#include <Transitional/Concepts/BigIntegers_Testobjects.hpp>
#include <Transitional/Concepts/RefactoringIncludeHandling_Testobjects.hpp>
#include <Transitional/Concepts/InputOutput_Testobjects.hpp>

int main() {
  return OKlib::TestSystem::TestBase::run_tests_default(std::cerr);
}
