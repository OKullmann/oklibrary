// Oliver Kullmann, 28.8.2005 (Swansea)

#ifndef INCLUDEHANDLINGTESTOBJECTS_plmJJk

#define INCLUDEHANDLINGTESTOBJECTS_plmJJk

#include <string>
#include <istream>

#include "IncludeHandling.hpp"
#include "IncludeHandling_Tests.hpp"

namespace OKlib {

  namespace Refactoring {
   
    Test_IncludeDirective<IncludeDirective> test_include_directive;
    Test_Program_grammar<Program_grammar> test_programm_grammar;
    Test_Extract_include_directives<Extract_include_directives> test_extract_include_directives;

  }

}

#endif
