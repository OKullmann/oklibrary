// Oliver Kullmann, 24.5.2005 (Swansea)

// Basic concepts according to
// ISO/IEC 14882: 2003 (referred to as the "Standard")
// PLUS const-correctness.

/*!
  \file std_Basics.hpp
  \brief Basic concepts from ISO/IEC 14882: 2003 (plus const-correctness).
*/

#ifndef STDBASICS_pokcc34Ty

#define STDBASICS_pokcc34Ty

#include <boost/concept_check.hpp>

#include "ConceptsBase.hpp"

namespace OKlib {

  namespace Concepts {

    // According to Table 28 in the standard
    template <typename T>
    struct EqualityComparable {
      void constraints() {
        static_cast<bool>(x == y);
        static_cast<bool>(a == b);
        static_cast<bool>(a == y);
        static_cast<bool>(x == b);
      }
      T x, y;
      const T a, b;
    };
    struct EqualityComparable_tag : virtual ConceptsBase_tag {};
    //Semantics:
    // a == a
    // if a == b then b == a
    // if a == b and b == c then a == c.

    class EqualityComparable_Archetype {
      EqualityComparable_Archetype();
      EqualityComparable_Archetype(const EqualityComparable_Archetype&);
      EqualityComparable_Archetype& operator=(const EqualityComparable_Archetype&);
      ~EqualityComparable_Archetype();
    protected :
      struct convertible_to_bool {
        operator bool() {}
      };
    public :
      convertible_to_bool operator ==(const EqualityComparable_Archetype&) const {}
    };

    // --------------------------------------------------------------------------------------------------------------------------------

    // According to Table 29 in the standard
    template <typename T>
    struct LessThanComparable {
      void constraints() {
        static_cast<bool>(x < y);
        static_cast<bool>(a < b);
        static_cast<bool>(a < y);
        static_cast<bool>(x < b);
      }
      T x, y;
      const T a, b;
    };
    struct LessThanComparable_tag : virtual ConceptsBase_tag {};
    // Semantics:
    // not a < a
    // if a < b and b < c then a < c
    // define a ~ b as (not (a < b) and not (b < a)):
    // ~ is equivalence relation
    // Comment: This makes < basically a total order.

    class LessThanComparable_Archetype {
      LessThanComparable_Archetype();
      LessThanComparable_Archetype(const LessThanComparable_Archetype&);
      LessThanComparable_Archetype& operator=(const LessThanComparable_Archetype&);
      ~LessThanComparable_Archetype();
    protected :
      struct convertible_to_bool {
        operator bool() {}
      };
    public :
      convertible_to_bool operator <(const LessThanComparable_Archetype&) const {}
    };

    // --------------------------------------------------------------------------------------------------------------------------------

    // ADDITION

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

    template <class T>
    struct CopyConstructible {
      void constraints() {
        boost::function_requires<Destructible<T> >();
        T x(t);
        T y(a);
        static_cast<T*>(&t);
        static_cast<const T*>(&a);
      } 
      T t;
      const T a;
    };
    struct CopyConstructible_tag : virtual Destructible_tag {};
    // Semantics:
    // T(a) is "equivalent" to a (i.e., substitutable).
    // *&a is "identical" with a.

    class CopyConstructible_Archetype {
      CopyConstructible_Archetype();
      CopyConstructible_Archetype& operator=(const CopyConstructible_Archetype&);
    };

    // --------------------------------------------------------------------------------------------------------------------------------

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

    // According to Table 64 in the standard
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
    // Semantics:
    // After a = b the object a is "equivalent" to b (i.e., substitutable).

    class Assignable_Archetype {
      Assignable_Archetype();
      Assignable_Archetype(const Assignable_Archetype&);
      ~Assignable_Archetype();
    };

  }

}

#endif
