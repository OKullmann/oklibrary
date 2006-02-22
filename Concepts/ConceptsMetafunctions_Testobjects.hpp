// Oliver Kullmann, 25.5.2005 (Swansea)

#ifndef CONCEPTSMETAFUNCTIONSTESTOBJECTS_hnBBx36

#define CONCEPTSMETAFUNCTIONSTESTOBJECTS_hnBBx36

#include <Transitional/Concepts/ConceptsMetafunctions.hpp>
#include <Transitional/Concepts/ConceptsMetafunctions_Tests.hpp>

namespace OKlib {

  namespace Concepts {

    namespace Metafunction_tests {

      template class Test_ConceptTag_T<concept_tag>;

      template class Test_IsTagModel_T_concept_tag<IsTagModel>;

      template class Test_IsConceptTag_Tag<IsConceptTag>;

      template class Test_HasConceptTag_T<HasConceptTag>;

    }

  }

}

#endif
