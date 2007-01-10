// Oliver Kullmann, 8.12.2006 (Swansea)

/*!
  \file Concepts/tests/AtomicConditions.hpp
  \brief Generic tests for models of concept AtomicCondition
*/

#ifndef ATOMICCONDITIONSTESTS_jajazbY6
#define ATOMICCONDITIONSTESTS_jajazbY6

#include <Transitional/TestSystem/TestBaseClass_DesignStudy.hpp>
#include <Transitional/Concepts/LibraryBasics.hpp>

#include <Transitional/Concepts/AtomicConditions.hpp>

namespace OKlib {
  namespace Concepts {
    namespace tests {
      
# define OKLIB_FILE_ID new ::OKlib::Messages::Utilities::FileIdentification \
      (__FILE__, __DATE__, __TIME__, "$Date: 2007/01/10 16:14:00 $", "$Revision: 1.3 $")

      /*!
        \class AtomicCondition_basic
        \brief Basic tests for models of concept AtomicCondition

        \todo Create basic semantic tests.
      */

      template <class AC>
      OKLIB_TEST_CLASS(AtomicCondition_basic) {
        OKLIB_TEST_CLASS_C(AtomicCondition_basic) {}
      private :
        void test(::OKlib::TestSystem::Basic) {
          OKLIB_MODELS_CONCEPT_REQUIRES(AC, AtomicCondition);
          OKLIB_MODELS_CONCEPT_TAG(AC, AtomicCondition);
        }
      };

    }
  }
}

# undef OKLIB_FILE_ID
#endif
