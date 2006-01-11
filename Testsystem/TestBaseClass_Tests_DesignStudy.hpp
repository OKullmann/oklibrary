// Oliver Kullmann, 9.1.2006 (Swansea)

#ifndef TESTBASECLASSTESTSDESIGNSTUDY_iiUUyyTr5

#define TESTBASECLASSTESTSDESIGNSTUDY_iiUUyyTr5

#include "TestBaseClass_DesignStudy.hpp"
#include "TestExceptions_DesignStudy.hpp"

namespace OKlib {

  namespace TestSystem {

    /*!
      \class  Test_TestSystem
      \brief Testing the test system
      \todo Complete.
    */

    class Test_TestSystem :  public OKlib::TestSystem::TestBase<Test_TestSystem> {
      typedef Test_TestSystem test_type;
      void test(OKlib::TestSystem::Basic, std::ostream& log) {
        typedef OKlib::TestSystem::Basic level_type;
        log << "Test: OKlib::TestSystem::Test_TestSystem, Basic\n";
        // putting test objects into a private container, then running RunTests with this container
      }
    };

  }

}

#endif
