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
      
      /*!
        \class AtomicCondition_basic
        \brief Basic tests for models of concept AtomicCondition

        \todo Create basic semantic tests.
      */

      template <class AC>
      class AtomicCondition_basic : public ::OKlib::TestSystem::TestBase {
        typedef ::OKlib::TestSystem::TestBase base_type;
      public :
        AtomicCondition_basic() : base_type(__FILE__, __LINE__, typeid(AtomicCondition_basic).name()) {}
      private :
        void test(::OKlib::TestSystem::Basic) {
          OKLIB_MODELS_CONCEPT_REQUIRES(AC, AtomicCondition);
          OKLIB_MODELS_CONCEPT_TAG(AC, AtomicCondition);
        }
      };

    }
  }
}

#endif
