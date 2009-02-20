// Oliver Kullmann, 25.2.2006 (Swansea)
/* Copyright 2006 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file TestSystem/RunTest_Declarations.hpp
  \brief Declarations for the class ::OKlib::TestSystem::RunTest

  With this class, tests are first inserted into a global container, and then executed.
*/

#ifndef RUNTESTDECLARATIONS_jJJhhBc5
#define RUNTESTDECLARATIONS_jJJhhBc5

#include <ostream>
#include <string>

#include <boost/ptr_container/ptr_list.hpp>

#include <OKlib/TestSystem/TestFondement.hpp>

namespace OKlib {

  namespace TestSystem {

    /*!
      \class RunTest
      \brief Responsible for running tests

      Objects of type RunTest are initialised with a pointer to a TestSystem::Test object,
      and the pointer is put into a global container.
      Via the static member function RunTest::run_test all these tests are executed.

      Meaning of the three output-streams passed to run_tests:
       - err : error messages (extracted from the exceptions thrown by test-meta-functions)
       - messages : messages from the test system, reporting on the general progress
       - log : messages by the test-meta-functions (typically reporting on special test conditions).
    */

    class RunTest {
      RunTest(const RunTest&); // not available
      RunTest& operator =(const RunTest&); // not available

    public :

      //! The type of the test objects
      typedef ::OKlib::TestSystem::Test test_type;
      typedef test_type* test_pointer_type;
      typedef boost::ptr_list<test_type> container_type;

      //! The only constructor; inserts parameter "test" into the (singleton-)container of test-object(-pointers).
      /*! RunTest takes over ownership of the object pointed to by "test". */
      RunTest(test_pointer_type const test);

      //! Runs all the tests in the build-in test-container; return Unix error code
      static int run_tests(std::ostream& err, std::ostream& messages, std::ostream& log, ::OKlib::TestSystem::TestLevel& level);
      //! Runs all the tests in the provided container test_objects; return Unix error code
      static int run_tests(std::ostream& err, std::ostream& messages, std::ostream& log, ::OKlib::TestSystem::TestLevel& level, container_type& test_objects);

    private :

      //! Contains as static object the (singleton-)test-container (of type container_type); p is inserted into the container if not null.
      static container_type& handle_test_objects(const test_pointer_type p = 0);

    };

  }

}

#endif
