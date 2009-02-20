// Oliver Kullmann, 11.6.2005 (Swansea)
/* Copyright 2005 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Concepts/ResultElements.hpp
  \brief Concepts for result elements, representing results of SAT solvers (especially
  for competitions).

  \todo To be completed (see module SATCompetition, and there mainly SATCompetition/SingleResult.hpp).
  \todo Once sub-directories of modules are fully supported, these concepts should
  go to Concepts/CompetitionEvaluation.
*/

#ifndef RESULTELEMENT_6Tg5Yh
#define RESULTELEMENT_6Tg5Yh

#include <OKlib/Concepts/LibraryBasics.hpp>

#include <OKlib/Experimentation/Competition/traits/string_type.hpp>
#include <OKlib/Experimentation/Competition/traits/floating_point_type.hpp>
#include <OKlib/Experimentation/Competition/traits/natural_number_type.hpp>

namespace OKlib {

  namespace Concepts {

    /*!
      \class ResultElement
      \brief Root class for the concept hierarchy about result elements (requiring only types).

      Requires string_type, floating_point_type, natural_number_type.
    */

    struct ResultElement_tag : virtual BasicRequirements_tag, virtual FullyConstructible_tag {};

    template <typename Res>
    struct ResultElement {
      typedef typename OKlib::SATCompetition::traits::string_type<Res>::type string_type;
      typedef typename OKlib::SATCompetition::traits::floating_point_type<Res>::type floating_point_type;
      typedef typename OKlib::SATCompetition::traits::natural_number_type<Res>::type natural_number_type;
      void constraints() {
        OKLIB_MODELS_CONCEPT_TAG(Res, ResultElement);

        OKLIB_MODELS_CONCEPT_REQUIRES(Res, BasicRequirements);
        OKLIB_MODELS_CONCEPT_TAG(Res, BasicRequirements);
        OKLIB_MODELS_CONCEPT_REQUIRES(Res, FullyConstructible);
        OKLIB_MODELS_CONCEPT_TAG(Res, FullyConstructible);
      }
    };

    class ResultElement_Archetype {
      struct X {};
      struct Y {};
      struct Z {};
    public :
      typedef ResultElement_tag concept_tag;
      typedef X string_type;
      typedef Y floating_point_type;
      typedef Z natural_number_type;
    };
    
    // --------------------------------------------------------------------------------------------------------------------------------------

    /*!
      \class ResultElementWithOrder
      \brief Refines concept ResultElement by requiring a linear order on result elements.
    */

    struct ResultElementWithOrder_tag : virtual ResultElement_tag, virtual EqualitySubstitutable_tag, virtual LinearOrder_tag {};

    template <typename Res>
    struct ResultElementWithOrder {
      void constraints() {
        OKLIB_MODELS_CONCEPT_TAG(Res, ResultElementWithOrder);

        OKLIB_MODELS_CONCEPT_REQUIRES(Res, ResultElement);
        OKLIB_MODELS_CONCEPT_TAG(Res, ResultElement);
        OKLIB_MODELS_CONCEPT_REQUIRES(Res, EqualitySubstitutable);
        OKLIB_MODELS_CONCEPT_TAG(Res, EqualitySubstitutable);
        OKLIB_MODELS_CONCEPT_REQUIRES(Res, LinearOrder);
        OKLIB_MODELS_CONCEPT_TAG(Res, LinearOrder);
      }
    };

    class ResultElementWithOrder_Archetype : public ResultElement_Archetype {
    protected :
      struct convertible_to_bool {
        operator bool() { return bool(); }
      };
    public :
      typedef ResultElementWithOrder_tag concept_tag;
      convertible_to_bool operator ==(const ResultElementWithOrder_Archetype&) const { return convertible_to_bool(); }
      convertible_to_bool operator !=(const ResultElementWithOrder_Archetype&) const { return convertible_to_bool(); }
      convertible_to_bool operator <(const ResultElementWithOrder_Archetype&) const { return convertible_to_bool(); }
      convertible_to_bool operator >(const ResultElementWithOrder_Archetype&) const { return convertible_to_bool(); }
      convertible_to_bool operator <=(const ResultElementWithOrder_Archetype&) const { return convertible_to_bool(); }
      convertible_to_bool operator >=(const ResultElementWithOrder_Archetype&) const { return convertible_to_bool(); }
    };
    
    // --------------------------------------------------------------------------------------------------------------------------------------

    /*!
      \class ResultElementWithName
      \brief Refines concept ResultElementWithOrder by adding the possibility to
      construct result elements with a given name, and to query this name.
      The order on such result elements must be given by the natural order on the names.
    */

    struct ResultElementWithName_tag : virtual ResultElementWithOrder_tag {};

    template <typename Res>
    struct ResultElementWithName {
      typedef typename ::OKlib::SATCompetition::traits::string_type<Res>::type string_type;
      void constraints() {
        OKLIB_MODELS_CONCEPT_TAG(Res, ResultElementWithName);

        OKLIB_MODELS_CONCEPT_REQUIRES(Res, ResultElementWithOrder);
        OKLIB_MODELS_CONCEPT_TAG(Res, ResultElementWithOrder);

        OKLIB_MODELS_CONCEPT_REQUIRES(string_type, ConstCorrect);
        OKLIB_MODELS_CONCEPT_REQUIRES(string_type, EqualitySubstitutable);
        OKLIB_MODELS_CONCEPT_REQUIRES(string_type, LinearOrder);
        OKLIB_MODELS_CONCEPT_REQUIRES(string_type, FullyConstructible);

        Res(s);
        Res(sc);
        static_cast<string_type>(r.name());
        static_cast<string_type>(rc.name());
      }
      string_type s; const string_type sc;
      Res r; const Res rc;
    };

    class ResultElementWithName_Archetype : public ResultElementWithOrder_Archetype {
    public :
      struct string_type {
    protected :
        struct convertible_to_bool {
          operator bool() { return bool(); }
        };
      public :
        convertible_to_bool operator ==(const string_type&) const { return convertible_to_bool(); }
        convertible_to_bool operator !=(const string_type&) const { return convertible_to_bool(); }
        convertible_to_bool operator <(const string_type&) const { return convertible_to_bool(); }
        convertible_to_bool operator >(const string_type&) const { return convertible_to_bool(); }
        convertible_to_bool operator <=(const string_type&) const { return convertible_to_bool(); }
        convertible_to_bool operator >=(const string_type&) const { return convertible_to_bool(); }
      };
      typedef ResultElementWithName_tag concept_tag;
      ResultElementWithName_Archetype() {}
      ResultElementWithName_Archetype(const string_type&) {}
      string_type name() const { return string_type(); }
    };
    
    // --------------------------------------------------------------------------------------------------------------------------------------

    
  }

}

#endif
