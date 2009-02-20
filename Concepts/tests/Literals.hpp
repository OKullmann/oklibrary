// Oliver Kullmann, 8.12.2006 (Swansea)
/* Copyright 2006 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Concepts/tests/Literals.hpp
  \brief Generic tests for models of concept Literals
*/

#ifndef LITERALSTESTS_jjgdgtet4TyTr
#define LITERALSTESTS_jjgdgtet4TyTr

#include <OKlib/TestSystem/TestBaseClass_DesignStudy.hpp>
#include <OKlib/Concepts/LibraryBasics.hpp>

#include <OKlib/Concepts/Literals.hpp>

namespace OKlib {
  namespace Concepts {
    namespace tests {
      
# define OKLIB_FILE_ID new ::OKlib::Messages::Utilities::FileIdentification \
      (__FILE__, __DATE__, __TIME__, "$Date: 2007/01/10 16:14:00 $", "$Revision: 1.2 $")

      /*!
        \class Literals_basic
        \brief Basic tests for models of concept Literals

        \todo Create basic semantic tests.
      */

      template <class Lit>
      OKLIB_TEST_CLASS(Literals_basic) {
        OKLIB_TEST_CLASS_C(Literals_basic) {}
      private :
        void test(::OKlib::TestSystem::Basic) {
          OKLIB_MODELS_CONCEPT_REQUIRES(Lit, Literals);
          OKLIB_MODELS_CONCEPT_TAG(Lit, Literals);
        }
      };

    }
  }
}

# undef OKLIB_FILE_ID
#endif
