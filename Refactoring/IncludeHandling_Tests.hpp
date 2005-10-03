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

#include <boost/filesystem/fstream.hpp>

#include "TestBaseClass.hpp"
#include "TestExceptions.hpp"

namespace OKlib {

  namespace Refactoring {

    /*!
      \class Test_IncludeHandling
      \todo Replacing files by stringstreams.
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

    template <template <class String> class Extract_include_directives>
    class Test_Extract_include_directives : public ::OKlib::TestSystem::TestBase {
    public :
      typedef Test_Extract_include_directives test_type;
      Test_Extract_include_directives() {
        insert(this);
      }
    private :
      void perform_test_trivial() {
        {
//           std::ifstream input("IncludeHandling.hpp");
//           Extract_include_directives<std::string> extract;
//           std::ofstream output("output.txt");
//           extract.preface() = "The preface...";
//           input >> extract;
//           output << extract;

//           typedef IncludeDirective<std::string> IncludeDirective;
//           int spaces_after_hash = 0;
//           int spaces_after_include = 1;
//           std::string header = "iostream";
//           Include_forms include_form = system_header;
//           IncludeDirective include_directive(header,spaces_after_hash,spaces_after_include,include_form);
//           std::string test_string("Some following text..");
//           std::pair<IncludeDirective,std::string> test_pair(include_directive,test_string);
//           extract.push_back(test_pair);
          
                
        }
      }
    };

  }

}

#endif
