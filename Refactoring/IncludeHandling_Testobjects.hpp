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
    Test_IncludeParsingGrammar<IncludeParsingGrammar> test_include_parsing_grammar;

  }

}

#endif
