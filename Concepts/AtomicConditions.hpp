// Oliver Kullmann, 5.12.2006

/*!
  \file Concepts/AtomicConditions.hpp
  \brief Concepts for atomic conditions (predicates)

  \todo To be completed:
   - Create a prototype of the concept.
   - Create (and test) archetype.
*/

#ifndef ATOMICCONDITIONS_303045439jhg
#define ATOMICCONDITIONS_303045439jhg

#include <Transitional/Concepts/LibraryBasics.hpp>

namespace OKlib {

  namespace Concepts {

        struct AtomicCondition_tag : virtual BasicRequirements_tag, virtual FullyConstructible_tag,  virtual EqualitySubstitutable_tag, virtual LinearOrder_tag {};

    template <typename AC>
    struct AtomicCondition {

      void constraints() {
        OKLIB_MODELS_CONCEPT_TAG(AC, AtomicCondition);

        OKLIB_MODELS_CONCEPT_REQUIRES(AC, BasicRequirements);
        OKLIB_MODELS_CONCEPT_TAG(AC, BasicRequirements);
        OKLIB_MODELS_CONCEPT_REQUIRES(AC, FullyConstructible);
        OKLIB_MODELS_CONCEPT_TAG(AC, FullyConstructible);
        OKLIB_MODELS_CONCEPT_REQUIRES(AC, EqualitySubstitutable);
        OKLIB_MODELS_CONCEPT_TAG(AC, EqualitySubstitutable);
        OKLIB_MODELS_CONCEPT_REQUIRES(AC, LinearOrder);
        OKLIB_MODELS_CONCEPT_TAG(AC, LinearOrder);

      }

    };

  }

}

#endif

