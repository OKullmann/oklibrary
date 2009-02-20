// Oliver Kullmann, 5.12.2006 (Swansea)
/* Copyright 2006 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Concepts/AtomicConditions.hpp
  \brief Concepts for atomic conditions (predicates)
*/

#ifndef ATOMICCONDITIONS_303045439jhg
#define ATOMICCONDITIONS_303045439jhg

#include <OKlib/Concepts/LibraryBasics.hpp>

#include <OKlib/Satisfiability/ProblemInstances/Conditions/AtomicConditions/traits/value_type.hpp>
#include <OKlib/Satisfiability/ProblemInstances/Conditions/AtomicConditions/eval.hpp>
#include <OKlib/Satisfiability/ProblemInstances/Conditions/AtomicConditions/complement.hpp>

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
      Given a value, the atomic condition which is exactly true on this value
      can be constructed. An atomic condition can be complemented.

      Since built-in types can model atomic conditions, for them in general the initialisation
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

        OKLIB_MODELS_CONCEPT_REQUIRES(value_type, FullyConstructible);
        OKLIB_MODELS_CONCEPT_REQUIRES(value_type, EqualitySubstitutable);
        OKLIB_MODELS_CONCEPT_REQUIRES(value_type, LinearOrder);

        static_cast<bool>(OKlib::AtomicConditions::eval(cc, v));
        static_cast<bool>(OKlib::AtomicConditions::eval(c, v));

        OKlib::AtomicConditions::complement(c);

        dummy_use(AC(v));
      }

      AC c;
      const AC cc;
      const value_type v;
      void dummy_use(const AC& ac) {}
    };

    class AtomicCondition_Archetype {
      typedef OKlib::Concepts::convertible_to_bool convertible_to_bool;
    public :
      typedef AtomicCondition_tag concept_tag;
      convertible_to_bool operator ==(const AtomicCondition_Archetype&) const { return convertible_to_bool(); }
      convertible_to_bool operator !=(const AtomicCondition_Archetype&) const { return convertible_to_bool(); }
      convertible_to_bool operator <(const AtomicCondition_Archetype&) const { return convertible_to_bool(); }
      convertible_to_bool operator >(const AtomicCondition_Archetype&) const { return convertible_to_bool(); }
      convertible_to_bool operator <=(const AtomicCondition_Archetype&) const { return convertible_to_bool(); }
      convertible_to_bool operator >=(const AtomicCondition_Archetype&) const { return convertible_to_bool(); }

      struct value_type {};
      friend convertible_to_bool operator ==(const value_type&, const value_type&) { return convertible_to_bool(); }
      friend convertible_to_bool operator !=(const value_type&, const value_type&) { return convertible_to_bool(); }
      friend convertible_to_bool operator <(const value_type&, const value_type&) { return convertible_to_bool(); }
      friend convertible_to_bool operator >(const value_type&, const value_type&) { return convertible_to_bool(); }
      friend convertible_to_bool operator <=(const value_type&, const value_type&) { return convertible_to_bool(); }
      friend convertible_to_bool operator >=(const value_type&, const value_type&) { return convertible_to_bool(); }

      AtomicCondition_Archetype() {}
      explicit AtomicCondition_Archetype(const value_type) {}
    };
  }
  namespace AtomicConditions {
    template <>
    inline bool eval(const OKlib::Concepts::AtomicCondition_Archetype&, const OKlib::AtomicConditions::traits::value_type<OKlib::Concepts::AtomicCondition_Archetype>::type&) {
      return ::OKlib::Concepts::convertible_to_bool();
    }
    template <>
    inline void complement(OKlib::Concepts::AtomicCondition_Archetype&) {}
    
  }

}

#endif

