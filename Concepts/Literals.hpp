// Oliver Kullmann, 25.11.2006 (Swansea)
/* Copyright 2006 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Concepts/Literals.hpp
  \brief Concepts for literals
*/

#ifndef LITERALS_JJhala7t4
#define LITERALS_JJhala7t4

#include <OKlib/Concepts/LibraryBasics.hpp>

#include <OKlib/Concepts/Variables.hpp>
#include <OKlib/Concepts/AtomicConditions.hpp>

#include <OKlib/Satisfiability/ProblemInstances/Literals/traits/var_type.hpp>
#include <OKlib/Satisfiability/ProblemInstances/Literals/traits/cond_type.hpp>
#include <OKlib/Satisfiability/ProblemInstances/Literals/var.hpp>
#include <OKlib/Satisfiability/ProblemInstances/Literals/cond.hpp>
#include <OKlib/Satisfiability/ProblemInstances/Literals/set_cond.hpp>

namespace OKlib {
  namespace Concepts {

    struct Literals_tag : virtual BasicRequirements_tag, virtual FullyConstructible_tag,  virtual EqualitySubstitutable_tag, virtual LinearOrder_tag {};

    /*!
      \class Literals
      \brief Basic literals concept

      Literals are (conceptually) just pairs of variables and atomic conditions, where the atomic
      condition is never always true and never always false.

      So we have associated type traits
      <ul>
       <li> <code> OKlib::Literals::traits::var_type </code> </li>
       <li> <code> OKlib::Literals::traits::cond_type </code> </li>
      </ul>
      and via
     <ul>
      <li> function <code> OKlib::Literals::var </code> the variable is extracted, </li>
      <li> while by <code>OKlib::Literals::cond </code> the (atomic) condition is extracted. </li>
     </ul>
     By <code> Literals::set_cond(x, condition) </code> we can set the condition
      in literal x, while from a variable a literal can be constructed.

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
        OKLIB_MODELS_CONCEPT_TAG(var_type, Concepts::Variables);
        OKLIB_MODELS_CONCEPT_REQUIRES(cond_type, Concepts::AtomicCondition);
        OKLIB_MODELS_CONCEPT_TAG(cond_type, Concepts::AtomicCondition);
        
        static_cast<var_type>(OKlib::Literals::var(lc));
        static_cast<var_type>(OKlib::Literals::var(l));

        static_cast<cond_type>(OKlib::Literals::cond(lc));

        OKlib::Literals::set_cond(l, c);

        dummy_use(Lit(v));
      }

      Lit l;
      const Lit lc;
      const var_type v;
      const cond_type c;
      void dummy_use(const Lit& l) {}
    };

    class Literals_Archetype {
      typedef OKlib::Concepts::convertible_to_bool convertible_to_bool;
    public :
      typedef Literals_tag concept_tag;
      typedef OKlib::Concepts::Variables_Archetype var_type;
      typedef OKlib::Concepts::AtomicCondition_Archetype cond_type;
      Literals_Archetype() {}
      Literals_Archetype(var_type) {}
      convertible_to_bool operator ==(const Literals_Archetype&) const { return convertible_to_bool(); }
      convertible_to_bool operator !=(const Literals_Archetype&) const { return convertible_to_bool(); }
      convertible_to_bool operator <(const Literals_Archetype&) const { return convertible_to_bool(); }
      convertible_to_bool operator >(const Literals_Archetype&) const { return convertible_to_bool(); }
      convertible_to_bool operator <=(const Literals_Archetype&) const { return convertible_to_bool(); }
      convertible_to_bool operator >=(const Literals_Archetype&) const { return convertible_to_bool(); }

    };
  }
  namespace Literals {
    template <>
    inline OKlib::Concepts::Literals_Archetype::var_type var(const OKlib::Concepts::Literals_Archetype&) { return OKlib::Concepts::Literals_Archetype::var_type(); }
    template <>
    inline OKlib::Concepts::Literals_Archetype::cond_type cond(const OKlib::Concepts::Literals_Archetype&) { return OKlib::Concepts::Literals_Archetype::cond_type(); }
    template <>
    inline void set_cond(OKlib::Concepts::Literals_Archetype&, const OKlib::Concepts::Literals_Archetype::cond_type&) {}
  }
  
}


#endif
