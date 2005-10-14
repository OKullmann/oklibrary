// Oliver Kullmann, 28.8.2005 (Swansea)

/*!
  \file IncludeHandling_Tests.hpp
  \brief Tests regarding include statement handling.
*/

#ifndef INCLUDEHANDLINGTESTS_77665r

#define INCLUDEHANDLINGTESTS_77665r

#include <string>
#include <iostream>
#include <fstream>
#include <utility>
#include <vector>

#include <boost/spirit.hpp>
#include <boost/filesystem/fstream.hpp>

#include "TestBaseClass.hpp"
#include "TestExceptions.hpp"
#include "ParserBase_Tests.hpp"

namespace OKlib {

  namespace Refactoring {

    /*!
      \class Test_IncludeDirective
      \brief Testing classes representing one include directive.
    */

    template <template <class String> class Include_Directive>
    class Test_IncludeDirective : public ::OKlib::TestSystem::TestBase {
    public :
      typedef Test_IncludeDirective test_type;
      Test_IncludeDirective() {
        insert(this);
      }
    private :
      void perform_test_trivial() {
        {
          typedef Include_Directive<std::string> IncludeDirective;
	  int spaces_after_hash = 0;
	  int spaces_after_include = 1;
          std::string header = "iostream";
	  Include_forms include_form = system_header;
          IncludeDirective include_directive(header,spaces_after_hash,spaces_after_include,include_form);
          // \todo Test output streaming of IncludeDirective
        }
      }
    };

    // ------------------------------------------------------------------
    
    /*!
      \class Test_IncludeParsingGrammar
      \brief Testing grammars for parsing source code units in order to extract include directives.
    */

    template <class IncludeParsingGrammar>
    class Test_IncludeParsingGrammar : public ::OKlib::TestSystem::TestBase {
    public :
      typedef Test_IncludeParsingGrammar test_type;
      Test_IncludeParsingGrammar() {
        insert(this);
      }
    private :
      void perform_test_trivial() {
        IncludeParsingGrammar g;
        typedef std::pair<std::string, OKlib::Parser::Matching_possibilities> test_element_type;
        typedef std::vector<test_element_type> test_vector_type;
        test_vector_type test_vector;
        using OKlib::Parser::match_full;
        using OKlib::Parser::match_not_full;

        test_vector.push_back(test_element_type("", match_full));
        test_vector.push_back(test_element_type("\"", match_full));
        test_vector.push_back(test_element_type("\\*", match_full));
        test_vector.push_back(test_element_type("#include \"abc\"", match_full));

        typedef test_vector_type::const_iterator iterator;
        const iterator& end(test_vector.end());
        for (test_vector_type::const_iterator i = test_vector.begin(); i != end; ++i)
          switch (i -> second) {
          case match_full :
            if (not parse(i -> first.c_str(), g).full)
              OKLIB_THROW("String \"" + i -> first + "\" was not accepted");
            break;
          case match_not_full :
            if (parse(i -> first.c_str(), g).full)
              OKLIB_THROW("String \"" + i -> first + "\" was accepted");
            break;
          }
      }
    };    

    
    // ------------------------------------------------------------------

    template <template <class String> class Extract_include_directives>
    class Test_Extract_include_directives : public ::OKlib::TestSystem::TestBase {
    public :
      typedef Test_Extract_include_directives test_type;
      Test_Extract_include_directives() {
        insert(this);
      }
    private :

      void perform_test_trivial() {
       
      }
    };

  }

}

#endif
