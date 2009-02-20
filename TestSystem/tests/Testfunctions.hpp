// Oliver Kullmann, 8.1.2006 (Swansea)
/* Copyright 2006 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file TestSystem/tests/Testfunctions.hpp
  \brief Temporary file for trying out the new test system.
  \deprecated
*/

#ifndef TESTFUNCTIONS_UJJhhgFR45
#define TESTFUNCTIONS_UJJhhgFR45

#include <exception>

#include <OKlib/TestSystem/TestBaseClass_DesignStudy.hpp>
#include <OKlib/TestSystem/TestExceptions_DesignStudy.hpp>

namespace OKlib {
  namespace TestSystem {
    namespace tests {

# define OKLIB_FILE_ID new ::OKlib::Messages::Utilities::FileIdentification \
      (__FILE__, __DATE__, __TIME__, "$Date: 2007/02/04 19:10:08 $", "$Revision: 1.6 $")

      OKLIB_TEST_CLASS(Test1) {
        OKLIB_TEST_CLASS_C(Test1) {}
      private :
        void test(::OKlib::TestSystem::Basic) {
          //throw std::runtime_error("In OKlib::TestSystem::Test1::test");
          //OKLIB_THROW("IN TEST1");
          //throw 5;
        }

      };

    }
  }
}

# undef OKLIB_FILE_ID
#endif
