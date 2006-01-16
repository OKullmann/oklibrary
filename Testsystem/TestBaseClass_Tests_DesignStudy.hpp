// Oliver Kullmann, 9.1.2006 (Swansea)

#ifndef TESTBASECLASSTESTSDESIGNSTUDY_iiUUyyTr5

#define TESTBASECLASSTESTSDESIGNSTUDY_iiUUyyTr5

#include <sstream>
#include <memory>

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

      struct LocalTest1 : public OKlib::TestSystem::TestBase<Test_TestSystem> {
        typedef LocalTest1 test_type;
        const int n;
        LocalTest1(int n) : n(n) {}
        void test(OKlib::TestSystem::Basic, std::ostream& log) {
          log << "Test: OKlib::TestSystem::Test_TestSystem::LocalTest1, Basic\n";
          typedef OKlib::TestSystem::Basic level_type;
          OKLIB_TEST_EQUAL(n, 0);
        }
      };
      
      void test(OKlib::TestSystem::Basic, std::ostream& log) {
        typedef OKlib::TestSystem::Basic level_type;
        log << "Test: OKlib::TestSystem::Test_TestSystem, Basic\n";
        typedef ::OKlib::TestSystem::RunTest::container_type container_type;
        std::auto_ptr<const ::OKlib::TestSystem::TestLevel> test_level(new ::OKlib::TestSystem::Basic);
        container_type test_objects;

        log << "First test\n";
        {
          std::stringstream test_err, test_messages, test_log;
          test_objects.push_back(new LocalTest1(0));
          ::OKlib::TestSystem::RunTest::run_tests(test_err, test_messages, test_log, test_level.get(), test_objects);
          log << "test_err:\n" << test_err.str() << "\n";
          log << "test_messages:\n" << test_messages.str() << "\n";
          log << "test_log:\n" << test_log.str() << std::endl;
          if (not test_err.str().empty())
            OKLIB_THROW("Error thrown!");
         if (test_messages.str().empty())
            OKLIB_THROW("No messages!");
         if (test_log.str().empty())
            OKLIB_THROW("No log!");
        }
        log << "Second test\n";
        {
          std::stringstream test_err, test_messages, test_log;
          test_objects.push_back(new LocalTest1(1));
          ::OKlib::TestSystem::RunTest::run_tests(test_err, test_messages, test_log, test_level.get(), test_objects);
          log << "test_err:\n" << test_err.str() << "\n";
          log << "test_messages:\n" << test_messages.str() << "\n";
          log << "test_log:\n" << test_log.str() << std::endl;
          if (test_err.str().empty())
            OKLIB_THROW("No error!");
          if (test_messages.str().empty())
            OKLIB_THROW("No messages!");
          if (test_log.str().empty())
            OKLIB_THROW("No log!");
        }
      }
    };

  }

}

#endif
