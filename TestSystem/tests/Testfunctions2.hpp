// Oliver Kullmann, 8.1.2006 (Swansea)
/* Copyright 2006 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file TestSystem/tests/Testfunctions2.hpp
  \brief Temporary file with testfunctions to try out the new testsystem
  \deprecated
*/

#ifndef TESTFUNCTIONS2_kHFcD3
#define TESTFUNCTIONS2_kHFcD3

#include <OKlib/TestSystem/TestBaseClass_DesignStudy.hpp>
#include <OKlib/TestSystem/TestExceptions_DesignStudy.hpp>

#include <OKlib/TestSystem/tests/Testfunctions.hpp>

namespace OKlib {
  namespace TestSystem {
    namespace tests {

# define OKLIB_FILE_ID new ::OKlib::Messages::Utilities::FileIdentification \
      (__FILE__, __DATE__, __TIME__, "$Date: 2007/02/04 19:10:08 $", "$Revision: 1.5 $")

      OKLIB_TEST_CLASS(Test2) {
        OKLIB_TEST_CLASS_C(Test2) {}
      private :
        void test(::OKlib::TestSystem::Basic) {
          OKLIB_TEST_RETHROW(::OKlib::TestSystem::tests::Test1());
        }
      };

      OKLIB_TEST_CLASS(Test3) {
        OKLIB_TEST_CLASS_C(Test3) {}
      private :
        void test(OKlib::TestSystem::Basic) {
          OKLIB_TEST_RETHROW(OKlib::TestSystem::tests::Test2());
        }
      };

    }
  }
}

# undef OKLIB_FILE_ID
#endif
