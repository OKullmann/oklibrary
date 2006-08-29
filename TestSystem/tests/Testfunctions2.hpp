// Oliver Kullmann, 8.1.2006 (Swansea)

/*!
  \file TestSystem/tests/Testfunctions2.hpp
  \brief Temporary file with testfunctions to try out the new testsystem
  \todo A macro is needed for the fixed code in test-meta-functions.
*/

#ifndef TESTFUNCTIONS2_kHFcD3
#define TESTFUNCTIONS2_kHFcD3

#include <Transitional/TestSystem/TestBaseClass_DesignStudy.hpp>
#include <Transitional/TestSystem/TestExceptions_DesignStudy.hpp>

#include <Transitional/TestSystem/tests/Testfunctions.hpp>

namespace OKlib {
  namespace TestSystem {
    namespace tests {

      class Test2 : public ::OKlib::TestSystem::TestBase {
        typedef ::OKlib::TestSystem::TestBase base_type;
      public :
        Test2() : base_type(__FILE__, __LINE__, typeid(Test2).name()) {}
      private :
        void test(::OKlib::TestSystem::Basic, std::ostream& log) {
          typedef ::OKlib::TestSystem::Basic level_type;
          OKLIB_TEST_RETHROW(::OKlib::TestSystem::tests::Test1());
        }
      };

      class Test3 : public OKlib::TestSystem::TestBase {
        typedef ::OKlib::TestSystem::TestBase base_type;
      public :
        Test3() : base_type(__FILE__, __LINE__, typeid(Test2).name()) {}
      private :
        void test(OKlib::TestSystem::Basic, std::ostream& log) {
          typedef OKlib::TestSystem::Basic level_type;
          OKLIB_TEST_RETHROW(OKlib::TestSystem::tests::Test2());
        }
      };

    }
  }
}

#endif
