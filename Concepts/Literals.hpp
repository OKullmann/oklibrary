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
