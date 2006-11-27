// Oliver Kullmann, 25.5.2005 (Swansea)

#ifndef LIBRARYBASICSTESTOBJECTS_nnttrqr367
#define LIBRARYBASICSTESTOBJECTS_nnttrqr367

#include <Transitional/Concepts/LibraryBasics.hpp>
#include <Transitional/Concepts/LibraryBasics_Tests.hpp>

namespace OKlib {

  namespace Concepts {

      OKLIB_BASIC_CONCEPT_TEST_TAG(WithConceptTag)
      OKLIB_BASIC_CONCEPT_TEST_ARCHETYPE(WithConceptTag)

      OKLIB_BASIC_CONCEPT_TEST_TAG(BasicRequirements)
      OKLIB_BASIC_CONCEPT_TEST_ARCHETYPE(BasicRequirements)    

    namespace Macro_tests {

      template <typename>
      struct Test_OKLIB_MODELS_CONCEPT {
        struct c : virtual ConceptsBase_tag {};
        struct test {
          typedef c concept_tag;
        };
        void f() {
          OKLIB_MODELS_CONCEPT_REQUIRES(test, WithConceptTag);
          OKLIB_MODELS_CONCEPT_TAG(test, WithConceptTag);
        }
      };
      
    }

  }

}

#endif
