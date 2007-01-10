// Oliver Kullmann, 9.1.2006 (Swansea)

/*!
  \file TestSystem/tests/TestBaseClass.hpp
  \brief Tests for the components from TestBaseClass_DesignStudy.hpp
*/

#ifndef TESTBASECLASSTESTSDESIGNSTUDY_iiUUyyTr5
#define TESTBASECLASSTESTSDESIGNSTUDY_iiUUyyTr5

#include <sstream>

#include <Transitional/Messages/Utilities/TrivialMessage.hpp>
#include <Transitional/Messages/messages/Levels.hpp>

#include <Transitional/TestSystem/TestBaseClass_DesignStudy.hpp>
#include <Transitional/TestSystem/RunTest_DesignStudy.hpp>
#include <Transitional/TestSystem/TestExceptions_DesignStudy.hpp>

namespace OKlib {
  namespace TestSystem {
    namespace tests {

# define OKLIB_FILE_ID new ::OKlib::Messages::Utilities::FileIdentification \
      (__FILE__, __DATE__, __TIME__, "$Date: 2007/01/10 16:22:54 $", "$Revision: 1.5 $")

      /*!
        \class TestSystem
        \brief Testing the test system
        
        \todo Complete it.
        \todo Doxygen documentation.
        \todo Improve the output (a more systematical approach for better structuring; and replacing the use of
        trivial messages).
      */
      
//       class TestSystem :  public ::OKlib::TestSystem::TestBase {
//         typedef ::OKlib::TestSystem::TestBase base_type;
//       public :
//         TestSystem() : base_type(OKLIB_FILE_ID, OKLIB_LINE, typeid(TestSystem).name()) {}
//       private :

      OKLIB_TEST_CLASS(TestSystem) {
        OKLIB_TEST_CLASS_C(TestSystem) {}
      private :

        OKLIB_TEST_CLASS(LocalTest1) {
          typedef ::OKlib::TestSystem::TestBase base_type;
          const int n;
        public :
          LocalTest1(const int n) : base_type(OKLIB_FILE_ID, OKLIB_LINE, typeid(LocalTest1).name()), n(n) {}
        private :
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
            OKlib::Messages::MessagesBase::set(test_log, OKlib::Messages::MessagesBase::level(log_stream()));
            OKlib::Messages::MessagesBase::set(test_log, OKlib::Messages::MessagesBase::language(log_stream()));
            OKlib::Messages::MessagesBase::set(test_err, OKlib::Messages::MessagesBase::level(log_stream()));
            OKlib::Messages::MessagesBase::set(test_err, OKlib::Messages::MessagesBase::language(log_stream()));
            OKlib::Messages::MessagesBase::set(test_messages, OKlib::Messages::MessagesBase::level(log_stream()));
            OKlib::Messages::MessagesBase::set(test_messages, OKlib::Messages::MessagesBase::language(log_stream()));
            test_objects.push_back(new LocalTest1(0));
            ::OKlib::TestSystem::RunTest::run_tests(test_err, test_messages, test_log, test_level, test_objects);
            log(trivial_message("Output of the three streams after RunTest::run_tests:\n"), __LINE__, __FILE__);
            log(trivial_message("content of test_err:\n" + test_err.str()));
            log(trivial_message("content of test_messages:\n" + test_messages.str()));
            log(trivial_message("test_log level = "));
            log(OKlib::Messages::messages::LevelNames(OKlib::Messages::MessagesBase::level(test_log)));
            log(trivial_message("\ncontent of test_log:\n" + test_log.str()));
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
            OKlib::Messages::MessagesBase::set(test_log, OKlib::Messages::MessagesBase::level(log_stream()));
            OKlib::Messages::MessagesBase::set(test_log, OKlib::Messages::MessagesBase::language(log_stream()));
            OKlib::Messages::MessagesBase::set(test_err, OKlib::Messages::MessagesBase::level(log_stream()));
            OKlib::Messages::MessagesBase::set(test_err, OKlib::Messages::MessagesBase::language(log_stream()));
            OKlib::Messages::MessagesBase::set(test_messages, OKlib::Messages::MessagesBase::level(log_stream()));
            OKlib::Messages::MessagesBase::set(test_messages, OKlib::Messages::MessagesBase::language(log_stream()));
 
            test_objects.push_back(new LocalTest1(1));
            ::OKlib::TestSystem::RunTest::run_tests(test_err, test_messages, test_log, test_level, test_objects);
            log(trivial_message("Output of the three streams after RunTest::run_tests:\n"), __LINE__, __FILE__);
            log(trivial_message("content of test_err:\n" + test_err.str()));
            log(trivial_message("content of test_messages:\n" + test_messages.str()));
            log(trivial_message("test_log level = "));
            log(OKlib::Messages::messages::LevelNames(OKlib::Messages::MessagesBase::level(test_log)));
            log(trivial_message("\ncontent of test_log:\n" + test_log.str()));
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

# undef OKLIB_FILE_ID
#endif
