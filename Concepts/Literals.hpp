// Oliver Kullmann, 25.11.2006

/*!
  \file Concepts/Literals.hpp
  \brief Concepts for literals

  \todo To be completed:
   - Create (and test) archetype.
*/

#ifndef LITERALS_JJhala7t4
#define LITERALS_JJhala7t4

#include <Transitional/Concepts/LibraryBasics.hpp>

#include <Transitional/Concepts/Variables.hpp>
#include <Transitional/Concepts/AtomicConditions.hpp>

#include <Transitional/Literals/traits/var_type.hpp>
#include <Transitional/Literals/traits/cond_type.hpp>

namespace OKlib {

  namespace Concepts {

    struct Literals_tag : virtual BasicRequirements_tag, virtual FullyConstructible_tag,  virtual EqualitySubstitutable_tag, virtual LinearOrder_tag {};

    /*!
      \class Literals
      \brief Basic literals concept

      Literals are (conceptually) just pairs of variables and atomic conditions, where the atomic
      condition is never always true and never always false.

      So we have associated type traits traits::var_type and traits::cond_type, and via the
      function Literals::var the variable is extracted, while by Literals::cond the (atomic)
      condition is extracted; for the latter a reference is returned in case the literal
      is not const. Given a variable and an atomic condition, a literal can be constructed.

      Since built-in types can model literals, for literals in general the initialisation
      behaviour of built-in types must be assumed.
    */

    template <typename Lit>
    struct Literals {

      typedef typename OKlib::Literals::traits::var_type<Lit>::type var_type;
      typedef typename OKlib::Literals::traits::cond_type<Lit>::type cond_type;

      void constraints() {
        OKLIB_MODELS_CONCEPT_TAG(Lit, Literals);

        OKLIB_MODELS_CONCEPT_REQUIRES(Lit, BasicRequirements);
        OKLIB_MODELS_CONCEPT_TAG(Lit, BasicRequirements);
        OKLIB_MODELS_CONCEPT_REQUIRES(Lit, FullyConstructible);
        OKLIB_MODELS_CONCEPT_TAG(Lit, FullyConstructible);
        OKLIB_MODELS_CONCEPT_REQUIRES(Lit, EqualitySubstitutable);
        OKLIB_MODELS_CONCEPT_TAG(Lit, EqualitySubstitutable);
        OKLIB_MODELS_CONCEPT_REQUIRES(Lit, LinearOrder);
        OKLIB_MODELS_CONCEPT_TAG(Lit, LinearOrder);

        OKLIB_MODELS_CONCEPT_REQUIRES(var_type, Concepts::Variables);
        OKLIB_MODELS_CONCEPT_REQUIRES(cond_type, Concepts::AtomicCondition);
        
        static_cast<var_type>(OKlib::Literals::var(lc));
        static_cast<var_type>(OKlib::Literals::var(l));

        static_cast<cond_type>(OKlib::Literals::cond(lc));
        static_cast<cond_type&>(OKlib::Literals::cond(l));

        Lit(v, cond);
      }

      Lit l;
      const Lit lc;
      const var_type v;
      const cond_type cond;
    };


  }

}

#endif
