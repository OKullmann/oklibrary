// Oliver Kullmann, 28.8.2005 (Swansea)
/* Copyright 2005 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#ifndef INCLUDEHANDLINGTESTOBJECTS_plmJJk
#define INCLUDEHANDLINGTESTOBJECTS_plmJJk

#include <OKlib/Programming/Refactoring/IncludeHandling.hpp>
#include <OKlib/Programming/Refactoring/IncludeHandling_Tests.hpp>

namespace OKlib {
  namespace Refactoring {
   
    Test_BoostFilesystemFix test_boost_filesystem_fix;
    Test_IncludeDirective<IncludeDirective> test_include_directive;
    Test_IncludeParsingGrammar<IncludeParsingGrammar> test_include_parsing_grammar;
    Test_ProgramRepresentationIncludes<ProgramRepresentationIncludes> test_program_representation_includes;
    Test_Parsing<ProgramRepresentationIncludes, StreamExtractor_by_istream_iterator> test_parsing_istream_iterator; // ERROR VALGRIND ?????????????????????????????
    Test_Parsing<ProgramRepresentationIncludes, StreamExtractor_by_copy> test_parsing_copy;
    // Test_ExtendIncludeDirectives<ExtendIncludeDirectives> test_extend_include_directives; // ERROR PROVISIONAL
    //Test_ExtendIncludeDirectivesTwoRanges<ExtendIncludeDirectivesTwoRanges> test_extend_include_directives_two_ranges; // ERROR PROVISIONAL
    Test_ExtendIncludeDirectivesTwoDirectories<ExtendIncludeDirectivesTwoDirectories> test_extend_include_directives_two_directories;
  }

}

#endif
