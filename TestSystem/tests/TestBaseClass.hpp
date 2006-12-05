// Oliver Kullmann, 9.1.2006 (Swansea)

/*!
  \file TestSystem/tests/TestBaseClass.hpp
  \brief Tests for the components from TestBaseClass_DesignStudy.hpp
*/

#ifndef TESTBASECLASSTESTSDESIGNSTUDY_iiUUyyTr5
#define TESTBASECLASSTESTSDESIGNSTUDY_iiUUyyTr5

#include <sstream>

#include <Transitional/Messages/Utilities/TrivialMessage.hpp>

#include <Transitional/TestSystem/TestBaseClass_DesignStudy.hpp>
#include <Transitional/TestSystem/RunTest_DesignStudy.hpp>
#include <Transitional/TestSystem/TestExceptions_DesignStudy.hpp>

namespace OKlib {
  namespace TestSystem {
    namespace tests {

      /*!
        \class TestSystem
        \brief Testing the test system
        \todo Complete it.
      */
      
      class TestSystem :  public ::OKlib::TestSystem::TestBase {
        typedef ::OKlib::TestSystem::TestBase base_type;
      public :
        TestSystem() : base_type(__FILE__, __LINE__, typeid(TestSystem).name()) {}
      private :

        struct LocalTest1 : public ::OKlib::TestSystem::TestBase {
          typedef ::OKlib::TestSystem::TestBase base_type;
          const int n;
          LocalTest1(const int n) : base_type(__FILE__, __LINE__, typeid(LocalTest1).name()), n(n) {}
          void test(::OKlib::TestSystem::Basic) {
            typedef OKlib::TestSystem::Basic level_type;
            OKLIB_TEST_EQUAL(n, 0);
          }
        };
      
        void test(::OKlib::TestSystem::Basic) {
          typedef ::OKlib::TestSystem::Basic level_type;
          typedef ::OKlib::TestSystem::RunTest::container_type container_type;
          ::OKlib::TestSystem::TestLevel& test_level(::OKlib::TestSystem::test_level(::OKlib::TestSystem::Basic()));
          container_type test_objects;

          using OKlib::Messages::Utilities::trivial_message;
          log(trivial_message("\nFIRST TEST\n"), __LINE__, __FILE__);
          {
            std::stringstream test_err, test_messages, test_log;
            test_objects.push_back(new LocalTest1(0));
            ::OKlib::TestSystem::RunTest::run_tests(test_err, test_messages, test_log, test_level, test_objects);
            log(trivial_message("content of test_err:\n" + test_err.str()), __LINE__, __FILE__);
            log(trivial_message("content of test_messages:\n" + test_messages.str()), __LINE__, __FILE__);
            log(trivial_message("content of test_log:\n" + test_log.str()), __LINE__, __FILE__);
            if (not test_err.str().empty())
              OKLIB_THROW("Error thrown!");
            if (test_messages.str().empty())
              OKLIB_THROW("No messages!");
            if (test_log.str().empty())
              OKLIB_THROW("No log!");
          }
          log(trivial_message("\nSECOND TEST\n"), __LINE__, __FILE__);
          {
            std::stringstream test_err, test_messages, test_log;
            test_objects.push_back(new LocalTest1(1));
            ::OKlib::TestSystem::RunTest::run_tests(test_err, test_messages, test_log, test_level, test_objects);
            log(trivial_message("content of test_err:\n" + test_err.str()), __LINE__, __FILE__);
            log(trivial_message("content of test_messages:\n" + test_messages.str()), __LINE__, __FILE__);
            log(trivial_message("content of test_log:\n" + test_log.str()), __LINE__, __FILE__);
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
}

#endif
