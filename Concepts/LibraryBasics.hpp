// Oliver Kullmann, 25.5.2005 (Swansea)
/* Copyright 2005 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Concepts/LibraryBasics.hpp
  \brief The very basic concepts for the library. Every unit where concepts are defined
  and/or checked should include this file. Provides macros OKLIB_MODELS_CONCEPT_REQUIRES
  and OKLIB_MODELS_CONCEPT_TAG for checking whether a class models a concept and
  has the required concept tag. Every "own concept" of the library must be a refinement of
  BasicRequirements.

  Includes boost/concept_check, boost/static_assert, boost/type_traits, and
  ConceptsBase, ConceptsMetafunctions, Basics.

  \todo Update the above description (several macros below are not captured yet).
*/

#ifndef LIBRARYBASICS_oLkG5675
#define LIBRARYBASICS_oLkG5675

#include <tr1/type_traits>

#include <boost/concept_check.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits.hpp>
// This file is guaranteed to include the last four files, so that OKLIB_HAS_CONCEPT_TAG and OKLIB_MODELS_CONCEPT always works.

#include <OKlib/Concepts/ConceptsBase.hpp>
#include <OKlib/Concepts/ConceptsMetafunctions.hpp>
#include <OKlib/Concepts/Basics.hpp>
// This file is guaranteed to include the last three files.

namespace OKlib {
  namespace Concepts {

    /*!
      \class WithConceptTag
      \brief Basic internal concept, statically asserting the concept tag.
    */

    template <typename T>
    struct WithConceptTag {
      BOOST_STATIC_ASSERT(HasConceptTag<T>::value);
      void constraints() {}
    };
    struct WithConceptTag_tag : virtual ConceptsBase_tag {};

    class WithConceptTag_Archetype {
      WithConceptTag_Archetype();
      WithConceptTag_Archetype(const WithConceptTag_Archetype&);
      WithConceptTag_Archetype& operator=(const WithConceptTag_Archetype&);
      ~WithConceptTag_Archetype();
      struct concept_tag_ : virtual ConceptsBase_tag {};
    public :
      typedef concept_tag_ concept_tag;
    };

    /*!
      \class BasicRequirements
      \brief Root of the concept hierarchy for the library, requiring a concept tag and const correctness.
    */

    template <typename T>
    struct BasicRequirements {
      void constraints() {
        ::boost::function_requires<WithConceptTag<T> >();
        ::boost::function_requires<ConstCorrect<T> >();
      }
    };
    struct BasicRequirements_tag : virtual WithConceptTag_tag, virtual ConstCorrect_tag {};

    class BasicRequirements_Archetype : public WithConceptTag_Archetype {};

    // -------------------------------------------------------------------------------------------------------------------------------

    /*!
      \def OKLIB_MODELS_CONCEPT_REQUIRES
      \brief Use OKLIB_MODELS_CONCEPT_REQUIRES(C, concept) in order to require that
      class C models "concept".

      Normally used together with OKLIB_MODELS_CONCEPT_TAG.
    */
    
#define OKLIB_MODELS_CONCEPT_REQUIRES(C, concept) ::boost::function_requires<concept<C> >();

    /*!
      \def OKLIB_MODELS_CONCEPT_TAG
      \brief Use OKLIB_MODELS_CONCEPT_TAG(C, concept) in order to require that
      class C has a concept tag derived from "concept_tag".

      Normally used together with OKLIB_MODELS_CONCEPT_REQUIRES.
    */

#define OKLIB_MODELS_CONCEPT_TAG(C, concept) BOOST_STATIC_ASSERT((::OKlib::Concepts::IsTagModel< C, concept ## _tag >::value));

    /*!
      \def OKLIB_MODELS_CONCEPT_TAG_T
      \brief Use OKLIB_MODELS_CONCEPT_TAG_T(tag, concept) in order to require that
       tag is derived from "concept_tag".

      Used in cases where just a concept-tag has to be checked against a concept
      (and no enclosing model-class is given).
    */

#define OKLIB_MODELS_CONCEPT_TAG_T(Tag, concept) BOOST_STATIC_ASSERT((std::tr1::is_base_of<concept ## _tag, Tag>::value));

    // ----------------------------------------------------------------------------------------------------------------------------------
    // Types for archetype definitions
    // ----------------------------------------------------------------------------------------------------------------------------------

    /*!
      \class convertible_to_bool
      \brief Fully constructible class, implicitely convertible to bool
      
      Used in archetypes, where functions must return values convertible to bool.
      A disadvantage or providing here this public definition is, that so "convertible_to_bool"
      now in principle is a known type --- so this type must not be used outside of
      archetype definitions.
    */
    struct convertible_to_bool {
      operator bool() { return bool(); }
    };

  }

}

#endif
