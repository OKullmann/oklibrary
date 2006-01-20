// Oliver Kullmann, 8.1.2006 (Swansea)

#ifndef TESTFUNCTIONS2_kHFcD3

#define TESTFUNCTIONS2_kHFcD3

#include "TestBaseClass_DesignStudy.hpp"
#include "TestExceptions_DesignStudy.hpp"

#include "Testfunctions_DesignStudy.hpp"

namespace OKlib {

  namespace TestSystem {

    class Test2 : public ::OKlib::TestSystem::TestBase<Test2> {
      typedef ::OKlib::TestSystem::TestBase<Test2> base_type;
    public :
      Test2() : base_type(__FILE__, __LINE__) {}
    private :
      typedef Test2 test_type;
      void test(::OKlib::TestSystem::Basic, std::ostream& log) {
        typedef ::OKlib::TestSystem::Basic level_type;
        OKLIB_TEST_RETHROW(::OKlib::TestSystem::Test1());
      }
    };

    class Test3 : public OKlib::TestSystem::TestBase<Test3> {
      typedef ::OKlib::TestSystem::TestBase<Test3> base_type;
    public :
      Test3() : base_type(__FILE__, __LINE__) {}
    private :
      typedef Test3 test_type;
      void test(OKlib::TestSystem::Basic, std::ostream& log) {
        typedef OKlib::TestSystem::Basic level_type;
        OKLIB_TEST_RETHROW(OKlib::TestSystem::Test2());
      }
    };

    
  }

}

#endif
