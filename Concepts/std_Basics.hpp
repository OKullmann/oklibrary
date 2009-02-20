// Oliver Kullmann, 24.5.2005 (Swansea)
/* Copyright 2005 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Concepts/std_Basics.hpp
  \brief Basic concepts from ISO/IEC 14882: 2003 (the "Standard")
  plus const-correctness.

  The following concepts are defined:
  - Concepts::EqualityComparable
  - Concepts::LessThanComparable
  - Concepts::Destructible
  - Concepts::CopyConstructible
  - Concepts::DefaultConstructible
  - Concepts::Assignable.

  \todo Complete doxygen-documentation.
*/

#ifndef STDBASICS_pokcc34Ty
#define STDBASICS_pokcc34Ty

#include <boost/concept_check.hpp>

#include <OKlib/Concepts/ConceptsBase.hpp>

namespace OKlib {
  namespace Concepts {

    /*!
      \class EqualityComparable
      \brief Concept EqualityComparable according to Table 28 in the standard (plus const-correctness).

      Semantical requirements are that of an equivalence relation, that is
      - a == a
      - if a == b then b == a
      - if a == b and b == c then a == c.

      \todo LessThanComparable_Archetype should use the provided
      type convertible_to_bool.
    */
      
    template <typename T>
    struct EqualityComparable {
      void constraints() {
        bool bo;
        bo = static_cast<bool>(x == y);
        bo = static_cast<bool>(a == b);
        bo = static_cast<bool>(a == y);
        bo = static_cast<bool>(x == b);
      }
      T x, y;
      const T a, b;
    };
    struct EqualityComparable_tag : virtual ConceptsBase_tag {};

    class EqualityComparable_Archetype {
      EqualityComparable_Archetype();
      EqualityComparable_Archetype(const EqualityComparable_Archetype&);
      EqualityComparable_Archetype& operator=(const EqualityComparable_Archetype&);
      ~EqualityComparable_Archetype();
    protected :
      struct convertible_to_bool {
        operator bool() { return false; }
      };
    public :
      convertible_to_bool operator ==(const EqualityComparable_Archetype&) const { return convertible_to_bool(); }
    };

    // --------------------------------------------------------------------------------------------------------------------------------

    /*!
      \class LessThanComparable
      \brief LessThanComparable according to Table 29 in the standard (plus const-correctness)

      Semantical requirements:
      - not a < a
      - if a < b and b < c then a < c
      - define a ~ b as (not (a < b) and not (b < a)): ~ is an equivalence relation
      Comment: This makes < "basically" a total order.

      \todo LessThanComparable_Archetype should use the provided
      type convertible_to_bool.
    */

    template <typename T>
    struct LessThanComparable {
      void constraints() {
        bool bo;
        bo = static_cast<bool>(x < y);
        bo = static_cast<bool>(a < b);
        bo = static_cast<bool>(a < y);
        bo = static_cast<bool>(x < b);
      }
      T x, y;
      const T a, b;
    };
    struct LessThanComparable_tag : virtual ConceptsBase_tag {};

    class LessThanComparable_Archetype {
      LessThanComparable_Archetype();
      LessThanComparable_Archetype(const LessThanComparable_Archetype&);
      LessThanComparable_Archetype& operator=(const LessThanComparable_Archetype&);
      ~LessThanComparable_Archetype();
    protected :
      struct convertible_to_bool {
        operator bool() { return false; }
      };
    public :
      convertible_to_bool operator <(const LessThanComparable_Archetype&) const { return convertible_to_bool(); }
    };

    // --------------------------------------------------------------------------------------------------------------------------------

    /*!
      \class Destructible
      \brief Concept Destructible (an obvious addition to the standard).
    */

    template <class T>
    struct Destructible {
      void constraints() {
        t.~T();
      }
      T t;
    };
    struct Destructible_tag : virtual ConceptsBase_tag {};

    class Destructible_Archetype {
      Destructible_Archetype();
      Destructible_Archetype(const Destructible_Archetype&);
      Destructible_Archetype& operator=(const Destructible_Archetype&);
    public :
      ~Destructible_Archetype() {}
    };
    
    // --------------------------------------------------------------------------------------------------------------------------------

    /*!
      \class CopyConstructible
      \brief Concept CopyConstructible.

      Refines concept Destructible. Semantics:
      - T(a) is "equivalent" to a (i.e., substitutable).
      - *&a is "identical" with a.

    */

    template <class T>
    struct CopyConstructible {
      void constraints() {
        boost::function_requires<Destructible<T> >();
        T x(t);
        T y(a);
        T* tp;
        tp = static_cast<T*>(&t);
        tp = &x; tp = &y;
        const T* ctp;
        ctp = static_cast<const T*>(&a);
      } 
      T t;
      const T a;
    };
    struct CopyConstructible_tag : virtual Destructible_tag {};

    class CopyConstructible_Archetype {
      CopyConstructible_Archetype();
      CopyConstructible_Archetype& operator=(const CopyConstructible_Archetype&);
    };

    // --------------------------------------------------------------------------------------------------------------------------------

    /*!
      \class DefaultConstructible
      \brief Concept DefaultConstructible.
    */

    template <typename T>
    struct DefaultConstructible {
      void constraints() {
        new T;
      }
    };
    struct DefaultConstructible_tag : virtual ConceptsBase_tag {};

    class DefaultConstructible_Archetype {
      DefaultConstructible_Archetype(const DefaultConstructible_Archetype&);
      DefaultConstructible_Archetype& operator=(const DefaultConstructible_Archetype&);
      ~DefaultConstructible_Archetype();
    public :
      DefaultConstructible_Archetype() {}
    };
    
    // --------------------------------------------------------------------------------------------------------------------------------

    /*!
      \class Assignable
      \brief Concept Assignable according to Table 64 in the standard.

      Semantics:
      - After a = b the object a is "equivalent" to b (i.e., substitutable).

      \todo Shouldn't there be a concept "CopyAssignable", which also requires
      that copying and assigning have the same effect?

    */

    template <typename T>
    struct Assignable {
      void constraints() {
        static_cast<T&>(a = a);
        static_cast<T&>(a = b);
      }
      T a;
      const T b; 
    };
    struct Assignable_tag : virtual ConceptsBase_tag {};

    class Assignable_Archetype {
      Assignable_Archetype();
      Assignable_Archetype(const Assignable_Archetype&);
      ~Assignable_Archetype();
    };

  }

}

#endif
