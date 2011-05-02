// Oliver Kullmann, 26.5.2005 (Swansea)
/* Copyright 2005 - 2007, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Concepts/Variables.hpp
  \brief Concepts for variables
*/

#ifndef VARIABLES_095tYb
#define VARIABLES_095tYb

#include <OKlib/traits/TypeTraits.hpp>

#include <OKlib/Concepts/LibraryBasics.hpp>
#include <OKlib/Satisfiability/ProblemInstances/Variables/traits/index_type.hpp>

namespace OKlib {
  namespace Concepts {

    struct Variables_tag : virtual BasicRequirements_tag, virtual FullyConstructible_tag,  virtual EqualitySubstitutable_tag, virtual LinearOrder_tag {};

    /*!
      \class Variables
      \brief Basic variables concept

      <ul>
       <li> Variables play the role of "tokens", and they are assumed to be
       lightweight objects. </li>
       <li> Variables are fully constructible and a(n)
       (implementation-defined) linear order < is defined on them. </li>
       <li> Variables can be "singular" and "non-singular", which can be found
       out by an implicit conversion into bool (true for non-singular). </li>
       <li> All singular variables are equal. </li>
       <li> Since built-in types can model variables, for variables in general
       the initialisation behaviour of built-in types must be assumed. </li>
      </ul>

      \todo Archetype
      <ul>
       <li> Shouldn't the conversion operator of Variables_Archetype return
       convertible_to_bool ? </li>
      </ul>
    */

    template <typename Var>
    struct Variables {
      void constraints() {
        OKLIB_MODELS_CONCEPT_TAG(Var, Variables);

        OKLIB_MODELS_CONCEPT_REQUIRES(Var, BasicRequirements);
        OKLIB_MODELS_CONCEPT_TAG(Var, BasicRequirements);
        OKLIB_MODELS_CONCEPT_REQUIRES(Var, FullyConstructible);
        OKLIB_MODELS_CONCEPT_TAG(Var, FullyConstructible);
        OKLIB_MODELS_CONCEPT_REQUIRES(Var, EqualitySubstitutable);
        OKLIB_MODELS_CONCEPT_TAG(Var, EqualitySubstitutable);
        OKLIB_MODELS_CONCEPT_REQUIRES(Var, LinearOrder);
        OKLIB_MODELS_CONCEPT_TAG(Var, LinearOrder);

        bool b;
        b = static_cast<bool>(v);
        b = static_cast<bool>(vc);
      }

      Var v;
      const Var vc;
    };

    class Variables_Archetype {
      typedef OKlib::Concepts::convertible_to_bool convertible_to_bool;
    public :
      typedef Variables_tag concept_tag;
      convertible_to_bool operator ==(const Variables_Archetype&) const { return convertible_to_bool(); }
      convertible_to_bool operator !=(const Variables_Archetype&) const { return convertible_to_bool(); }
      convertible_to_bool operator <(const Variables_Archetype&) const { return convertible_to_bool(); }
      convertible_to_bool operator >(const Variables_Archetype&) const { return convertible_to_bool(); }
      convertible_to_bool operator <=(const Variables_Archetype&) const { return convertible_to_bool(); }
      convertible_to_bool operator >=(const Variables_Archetype&) const { return convertible_to_bool(); }
      operator bool() const { return bool(); }
    };
    
    // --------------------------------------------------------------------------------------------------------------------------------------

    struct VariablesWithIndex_tag : virtual Variables_tag {};
    
    /*!
      \class VariablesWithIndex
      \brief Refinement of concept Concepts::Variables, adding an index_type, and variables
      can be implicitely converted to this index type
    */

    template <typename Var>
    struct VariablesWithIndex {

      typedef typename OKlib::Variables::traits::index_type<Var>::type index_type;
      BOOST_STATIC_ASSERT(::OKlib::traits::is_unqualified_signedunsigned_integral<index_type>::value);

      void constraints() {
        OKLIB_MODELS_CONCEPT_TAG(Var, VariablesWithIndex);

        OKLIB_MODELS_CONCEPT_REQUIRES(Var, Variables);
        OKLIB_MODELS_CONCEPT_TAG(Var, Variables);

        index_type ind;
        ind = static_cast<index_type>(v);
        ind = static_cast<index_type>(vc);
      }

      Var v;
      const Var vc;
    };

    struct VariablesWithIndex_Archetype : Variables_Archetype {
      typedef VariablesWithIndex_tag concept_tag;
      typedef int index_type; // ToDo: Can this be made more generic?
      operator index_type() const { return index_type(); }
    };

    // --------------------------------------------------------------------------------------------------------------------------------------

    struct VariablesAsIndex_tag : virtual VariablesWithIndex_tag {};
    
    /*!
      \class VariablesAsIndex
      \brief Refinement of concept Concepts::VariablesWithIndex, requiring a constructor
      of variables from index values.
    */

    template <typename Var>
    struct VariablesAsIndex {

      typedef typename OKlib::Variables::traits::index_type<Var>::type index_type;

      void constraints() {
        OKLIB_MODELS_CONCEPT_TAG(Var, VariablesAsIndex);

        OKLIB_MODELS_CONCEPT_REQUIRES(Var, VariablesWithIndex);
        OKLIB_MODELS_CONCEPT_TAG(Var, VariablesWithIndex);

        dummy_use_v(Var(i));
        dummy_use_v(Var(ic));
      }

      index_type i; const index_type ic;
      void dummy_use_v(const Var& v) {}
    };

    struct VariablesAsIndex_Archetype : VariablesWithIndex_Archetype {
      typedef VariablesAsIndex_tag concept_tag;
      VariablesAsIndex_Archetype() {}
      explicit VariablesAsIndex_Archetype(index_type) {}
    };
  }

}

#endif
