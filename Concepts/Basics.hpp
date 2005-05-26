// Oliver Kullmann, 24.5.2005 (Swansea)

#ifndef BASICS_haharw786

#define BASICS_haharw786

#include <boost/concept_check.hpp>

#include "ConceptsBase.hpp"
#include "std_Basics.hpp"

namespace OKlib {

  namespace Concepts {

    template <typename T>
    struct FullyEqualityComparable {
      void constraints() {
        boost::function_requires<EqualityComparable<T> >();
        static_cast<bool>(x != y);
        static_cast<bool>(a != b);
        static_cast<bool>(a != y);
        static_cast<bool>(x != b);
      }
      T x, y;
      const T a, b;
    };
    struct FullyEqualityComparable_tag : virtual EqualityComparable_tag {};
    // Semantics:
    // a != b iff not (a == b).

    class FullyEqualityComparable_Archetype : public EqualityComparable_Archetype {
      FullyEqualityComparable_Archetype();
      FullyEqualityComparable_Archetype(const FullyEqualityComparable_Archetype&);
      FullyEqualityComparable_Archetype& operator=(const FullyEqualityComparable_Archetype&);
      ~FullyEqualityComparable_Archetype();
    public :
      convertible_to_bool operator !=(const FullyEqualityComparable_Archetype&) const {}
    };

    // ----------------------------------------------------------------------------------------------------------------------

    template <typename T>
    struct FullyLessThanComparable {
      void constraints() {
        boost::function_requires<LessThanComparable<T> >();
        static_cast<bool>(x > y); static_cast<bool>(a > b); static_cast<bool>(a > y); static_cast<bool>(x > b);
        static_cast<bool>(x <= y); static_cast<bool>(a <= b); static_cast<bool>(a <= y); static_cast<bool>(x <= b);
        static_cast<bool>(x >= y); static_cast<bool>(a >= b); static_cast<bool>(a >= y); static_cast<bool>(x >= b);
      }
      T x, y;
      const T a, b;
    };
    struct FullyLessThanComparable_tag : virtual LessThanComparable_tag {};
    // Semantics: 
    // a > b iff b < a
    // a <= b iff not (a > b)
    // a >= b iff not (a < b).

    class FullyLessThanComparable_Archetype : public LessThanComparable_Archetype {
      FullyLessThanComparable_Archetype();
      FullyLessThanComparable_Archetype(const FullyLessThanComparable_Archetype&);
      FullyLessThanComparable_Archetype& operator=(const FullyLessThanComparable_Archetype&);
      ~FullyLessThanComparable_Archetype();
    public :
      convertible_to_bool operator >(const FullyLessThanComparable_Archetype&) const {}
      convertible_to_bool operator <=(const FullyLessThanComparable_Archetype&) const {}
      convertible_to_bool operator >=(const FullyLessThanComparable_Archetype&) const {}
    }; 

    // ----------------------------------------------------------------------------------------------------------------------

    template <typename T>
    struct LinearOrder {
      void constraints() {
        boost::function_requires<FullyLessThanComparable<T> >();
        boost::function_requires<FullyEqualityComparable<T> >();
      }
    };
    struct LinearOrder_tag : virtual FullyLessThanComparable_tag, virtual FullyEqualityComparable_tag {};
    // Semantics:
    // a == b iff (not (a < b) and not (b > a)).

    class LinearOrder_Archetype : public FullyLessThanComparable_Archetype, public FullyEqualityComparable_Archetype {
      LinearOrder_Archetype();
      LinearOrder_Archetype(const LinearOrder_Archetype&);
      LinearOrder_Archetype& operator=(const LinearOrder_Archetype&);
      ~LinearOrder_Archetype();
    };

    // ----------------------------------------------------------------------------------------------------------------------

    template <typename T>
    struct FullyConstructible {
      void constraints() {
        boost::function_requires<Destructible<T> >();
        boost::function_requires<CopyConstructible<T> >();
        boost::function_requires<DefaultConstructible<T> >();
        boost::function_requires<Assignable<T> >();
      }
    };
    struct FullyConstructible_tag : virtual Destructible_tag, virtual CopyConstructible_tag, virtual DefaultConstructible_tag, virtual Assignable_tag {};

    class FullyConstructible_Archetype {};

    // ----------------------------------------------------------------------------------------------------------------------

    template <typename T>
    struct EqualitySubstitutable {
      void constraints() {
        boost::function_requires<EqualityComparable<T> >();
      }
    };
    struct EqualitySubstitutable_tag : virtual EqualityComparable_tag {};
    // Semantics:
    // if a == b, then b can be substituted for a everywhere.

    class EqualitySubstitutable_Archetype : public EqualityComparable_Archetype {
      EqualitySubstitutable_Archetype();
      EqualitySubstitutable_Archetype(const EqualitySubstitutable_Archetype&);
      EqualitySubstitutable_Archetype& operator=(const EqualitySubstitutable_Archetype&);
      ~EqualitySubstitutable_Archetype();
    };

    template <typename T>
    struct ConstCorrect {
      void constraints() {}
    };
    struct ConstCorrect_tag : virtual ConceptsBase_tag {};
    // Semantics:
    // Operations allowed for const T objects applied to const T objects maintain substitutability.

    class ConstCorrect_Archetype {
      ConstCorrect_Archetype();
      ConstCorrect_Archetype(const ConstCorrect_Archetype&);
      ConstCorrect_Archetype& operator=(const ConstCorrect_Archetype&);
      ~ConstCorrect_Archetype();
    };

  }

}

#endif
