// Oliver Kullmann, 26.12.2005 (Swansea)
/* Copyright 2005 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file TestExecutionTools/SecondOrderTestTools_Tests.hpp
  \brief Tests for the tools for testing the test system
*/

#ifndef SECONDORDERTESTTOOLSTESTS_vzvzlLKj8
#define SECONDORDERTESTTOOLSTESTS_vzvzlLKj8

#include <stdint.h>

#include <OKlib/TestSystem/TestBaseClass.hpp>
#include <OKlib/TestSystem/TestExceptions.hpp>

namespace OKlib {

  namespace TestExecutionTools {

    /*!
      \class Test_TestCounter
      \brief Tests the test counter by calling it some times.
    */

    template <template <class Dummy, typename IntType = uint_fast64_t> class TestCounter>
    class Test_TestCounter : public ::OKlib::TestSystem::TestBase {
    public :
      typedef Test_TestCounter test_type;
      Test_TestCounter() {
        insert(this);
      }
    private :
      void perform_test_trivial() {
        typedef TestCounter<Test_TestCounter> testcounter_type;
        typedef typename testcounter_type::int_type int_type;
        OKLIB_TEST_EQUAL(testcounter_type::counter, 0U);
        OKLIB_TEST_EQUAL(testcounter_type::throw_value, 0);
        const int_type number_iterations(1000);
        for (int_type i = 0; i < number_iterations; ++i)
          OKLIB_TESTTRIVIAL_RETHROW(testcounter_type());
        OKLIB_TEST_EQUAL(testcounter_type::counter, number_iterations);
        OKLIB_TEST_EQUAL(testcounter_type::throw_value, 0);
        testcounter_type::throw_value = number_iterations + 1;
        bool has_thrown = false;
        try {
          OKLIB_TESTTRIVIAL_RETHROW(testcounter_type());
        }
        catch(const OKlib::TestSystem::TestException& e) {
          has_thrown = true;
        }
        OKLIB_TEST_EQUAL(has_thrown, true);
        for (int_type i = 0; i < number_iterations; ++i)
          OKLIB_TESTTRIVIAL_RETHROW(testcounter_type());
        OKLIB_TEST_EQUAL(testcounter_type::counter, 2 * number_iterations + 1);
        OKLIB_TEST_EQUAL(testcounter_type::throw_value, number_iterations + 1);
        testcounter_type::throw_value = 2 * number_iterations + 2;
        has_thrown = false;
        try {
          OKLIB_TESTTRIVIAL_RETHROW(testcounter_type());
        }
        catch(const OKlib::TestSystem::TestException& e) {
          has_thrown = true;
        }
        OKLIB_TEST_EQUAL(has_thrown, true);
      }
    };

  }

}

#endif
