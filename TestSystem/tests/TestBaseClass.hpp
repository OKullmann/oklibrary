// Oliver Kullmann, 9.1.2006 (Swansea)
/* Copyright 2006 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file TestSystem/tests/TestBaseClass.hpp
  \brief Tests for the components from TestBaseClass_DesignStudy.hpp

  \todo Improve the include list: Make clear what are library files,
  and what is to be tested.

*/

#ifndef TESTBASECLASSTESTSDESIGNSTUDY_iiUUyyTr5
#define TESTBASECLASSTESTSDESIGNSTUDY_iiUUyyTr5

#include <sstream>

#include <OKlib/Programming/Messages/Utilities/TrivialMessage.hpp>
#include <OKlib/Programming/Messages/messages/Levels.hpp>

#include <OKlib/TestSystem/TestBaseClass_DesignStudy.hpp>
#include <OKlib/TestSystem/RunTest_DesignStudy.hpp>
#include <OKlib/TestSystem/TestExceptions_DesignStudy.hpp>

namespace OKlib {
  namespace TestSystem {
    namespace tests {

# define OKLIB_FILE_ID new ::OKlib::Messages::Utilities::FileIdentification \
      (__FILE__, __DATE__, __TIME__, "$Date: 2007/02/06 18:37:08 $", "$Revision: 1.8 $")

      /*!
        \class TestSystem
        \brief Testing the test system
        
        \todo Complete it.
        \todo Doxygen documentation.
        \todo Improve the output (a more systematical approach for better structuring; and
        replacing the use of trivial messages).
      */
      
      OKLIB_TEST_CLASS(TestSystem) {
        OKLIB_TEST_CLASS_C(TestSystem) {}
      private :

        OKLIB_TEST_CLASS(LocalTest1) {
          OKLIB_TEST_CLASS_C1(LocalTest1, const int, n) {}
        private :
          void test(::OKlib::TestSystem::Basic) {
            OKLIB_TEST_EQUAL(n, 0);
          }
        };
      
        void test(::OKlib::TestSystem::Basic) {
          typedef ::OKlib::TestSystem::RunTest::container_type container_type;
          ::OKlib::TestSystem::TestLevel& test_level(::OKlib::TestSystem::test_level(::OKlib::TestSystem::Basic()));
          container_type test_objects;

          using OKlib::Messages::Utilities::trivial_message;
          OKLIB_FULL_LOG(trivial_message("\nFIRST TEST\n"));
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
            OKLIB_FULL_LOG(trivial_message("Output of the three streams after RunTest::run_tests:\n"));
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
          OKLIB_FULL_LOG(trivial_message("\nSECOND TEST\n"));
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
            OKLIB_FULL_LOG(trivial_message("Output of the three streams after RunTest::run_tests:\n"));
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
