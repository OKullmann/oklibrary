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

#include <Transitional/AtomicConditions/traits/value_type.hpp>

namespace OKlib {

  namespace Concepts {

        struct AtomicCondition_tag : virtual BasicRequirements_tag, virtual FullyConstructible_tag,  virtual EqualitySubstitutable_tag, virtual LinearOrder_tag {};

    /*!
      \class AtomicCondition
      \brief Concept for basic atomic conditions

      Atomic conditions are fully constructible, and a(n) (implementation-defined) linear order
      is defined on them. They represent boolean-valued functions on their value type.

      So we have an associated type traits traits::value_type, and via the function
      AtomicConditions::eval the boolean value of the condition on a value is evaluated.
      Given a value, the atomic condition which is exactly true resp. false on this value
      can be constructed.

      Since built-in types can model atomic conditions, for tehm in general the initialisation
      behaviour of built-in types must be assumed.
    */

    template <typename AC>
    struct AtomicCondition {

      typedef typename OKlib::AtomicConditions::traits::value_type<AC>::type value_type;

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

        static_cast<bool>(OKlib::AtomicConditions::eval(cc, v));
        static_cast<bool>(OKlib::AtomicConditions::eval(c, v));

        AC(v);
        AC(v, true);
        AC(v, false);
      }

      AC c;
      const AC cc;
      const value_type v;
    };

  }

}

#endif

