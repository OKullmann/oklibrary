// Oliver Kullmann, 8.12.2006 (Swansea)

/*!
  \file Concepts/tests/Literals.hpp
  \brief Generic tests for models of concept Literals
*/

#ifndef LITERALSTESTS_jjgdgtet4TyTr
#define LITERALSTESTS_jjgdgtet4TyTr

#include <Transitional/TestSystem/TestBaseClass_DesignStudy.hpp>
#include <Transitional/Concepts/LibraryBasics.hpp>

#include <Transitional/Concepts/Literals.hpp>

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
