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
    Test_ProgramRepresentationIncludes<ProgramRepresentationIncludes> test_program_representation_includes;
    Test_Parsing<ProgramRepresentationIncludes, StreamExtractor_by_istream_iterator> test_parsing_istream_iterator;
     Test_Parsing<ProgramRepresentationIncludes, StreamExtractor_by_copy> test_parsing_copy;
  }

}

#endif
