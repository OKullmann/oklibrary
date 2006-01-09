// Oliver Kullmann, 8.1.2006 (Swansea)

#ifndef TESTFUNCTIONS2_kHFcD3

#define TESTFUNCTIONS2_kHFcD3

#include "TestBaseClass_DesignStudy.hpp"
#include "TestExceptions_DesignStudy.hpp"

#include "Testfunctions_DesignStudy.hpp"

namespace OKlib {

  namespace TestSystem {

    class Test2 : public OKlib::TestSystem::TestBase<Test2> {
      typedef Test2 test_type;
      void test(OKlib::TestSystem::Basic, std::ostream& log) {
        typedef OKlib::TestSystem::Basic level_type;
        log << "Test: OKlib::TestSystem::Test2, Basic\n";
        OKLIB_TEST_RETHROW(OKlib::TestSystem::Test1());
      }
    };

    class Test3 : public OKlib::TestSystem::TestBase<Test3> {
      typedef Test3 test_type;
      void test(OKlib::TestSystem::Basic, std::ostream& log) {
        log << "Test: OKlib::TestSystem::Test3, Basic\n";
        typedef OKlib::TestSystem::Basic level_type;
        OKLIB_TEST_RETHROW(OKlib::TestSystem::Test2());
      }
    };

    
  }

}

#endif
