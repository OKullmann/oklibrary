// Oliver Kullmann, 28.8.2005 (Swansea)

/*!
  \file IncludeHandling_Tests.hpp
  \brief Tests regarding include statement handling.
*/

#ifndef INCLUDEHANDLINGTESTS_77665r

#define INCLUDEHANDLINGTESTS_77665r

#include <string>

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
    class Test_IncludeHandling : public ::OKlib::TestSystem::TestBase {
    public :
      typedef Test_IncludeHandling test_type;
      Test_IncludeHandling() {
        insert(this);
      }
    private :
      void perform_test_trivial() {
        {
          typedef Include_Directive<std::string> ID;
	  int a = 3;
	  int b = 4;
	  std::string c = "iostream";
	  Include_forms d = system_header;
          ID include_directive(c,a,b,d);
  	  boost::filesystem::ofstream file( "/home/csmatthew/test.hpp" );
          file << include_directive;
        }
      }
    };

  }

}

#endif
