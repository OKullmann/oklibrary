// Oliver Kullmann, 8.1.2006 (Swansea)

/*!
  \file TestSystem/tests/Testfunctions.hpp
  \brief Temporary file for trying out the new test system.
*/

#ifndef TESTFUNCTIONS_UJJhhgFR45
#define TESTFUNCTIONS_UJJhhgFR45

#include <exception>

#include <Transitional/TestSystem/TestBaseClass_DesignStudy.hpp>
#include <Transitional/TestSystem/TestExceptions_DesignStudy.hpp>

namespace OKlib {
  namespace TestSystem {
    namespace tests {

# define OKLIB_FILE_ID new ::OKlib::Messages::Utilities::FileIdentification \
      (__FILE__, __DATE__, __TIME__, "$Date: 2007/01/10 16:22:54 $", "$Revision: 1.4 $")
    
      class Test1 : public ::OKlib::TestSystem::TestBase {
        typedef ::OKlib::TestSystem::TestBase base_type;
      public :
        Test1() : base_type(OKLIB_FILE_ID, OKLIB_LINE, typeid(Test1).name()) {}
      private :
        void test(::OKlib::TestSystem::Basic) {
          typedef ::OKlib::TestSystem::Basic level_type;
          //throw std::runtime_error("In OKlib::TestSystem::Test1::test");
          //OKLIB_THROW("IN TEST1");
          //throw 5;
        }

      };

    }
  }
}

# undef OKLIB_FILE_ID
#endif
