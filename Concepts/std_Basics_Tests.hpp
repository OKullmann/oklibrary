// Oliver Kullmann, 24.5.2005 (Swansea)

/*!
  \file std_Basics_Tests.hpp
  \brief Test classes for the basic concepts from the standard.

  Concepts EqualityComparable and LessThanComparable enable semantical tests,
  but concepts Destructible, CopyConstructible, DefaultConstructible, Assignable have no
  semantical properties.
  \todo Complete update!
*/

#ifndef STDBASICSTESTS_8455tGb

#define STDBASICSTESTS_8455tGb

#include <cassert>
#include <functional>

#include "DerivedRelations.hpp"

#include "TestBaseClass.hpp"
#include "TestExceptions.hpp"

namespace OKlib {

  namespace Concepts {

    /*!
      \class EqualityComparable_Axiom_reflexivity
      \brief Checks for objects a whether a == a holds.
    */

    template <typename T>
    struct EqualityComparable_Axiom_reflexivity : OKlib::TestSystem::Test {
      typedef EqualityComparable_Axiom_reflexivity test_type;
      EqualityComparable_Axiom_reflexivity(const T& a) : a(a) {}
    private :
      const T& a;
      void perform_test_trivial() {
        if (not (a == a))
          OKLIB_THROW("not a == a");
      }
    };

    /*!
      \class EqualityComparable_Axiom_symmetry
      \brief Checks for objects a, b whether either a == b and b == a holds or not (a == b) and not (b == a).
    */

    template <typename T>
    struct EqualityComparable_Axiom_symmetry : OKlib::TestSystem::Test {
      typedef EqualityComparable_Axiom_symmetry test_type;
      EqualityComparable_Axiom_symmetry(const T& a, const T& b) : a(a), b(b) {}
    private :
      const T& a, b;
      void perform_test_trivial() {
        const bool ab = (a == b);
        const bool ba = (b == a);
        switch (ab) {
        case true :
          if (not ba)
            OKLIB_THROW("a == b and not b == a");
          break;
        case false :
          if (ba)
            OKLIB_THROW("not a == b and b == a");
          break;
        }
      }
    };

    /*!
      \class EqualityComparable_Axiom_transitivity
      \brief Checks for objects a, b, c with a == b and b == c, whether a == c holds.
    */

    template <typename T>
    struct EqualityComparable_Axiom_transitivity : OKlib::TestSystem::Test {
      typedef EqualityComparable_Axiom_transitivity test_type;
      EqualityComparable_Axiom_transitivity(const T& a, const T& b, const T& c) : a(a), b(b), c(c) {}
    private :
      const T& a, b, c;
      void perform_test_trivial() {
        const bool ab = (a == b);
        const bool ac = (a == c);
        const bool bc = (b == c);
        const int count_equal = ab + ac + bc;
        if (count_equal == 2)
          OKLIB_THROW("one equality missing");
      }
    };

    /*!
      \class EqualityComparable_basic_test_one_object
      \brief Basic test for EqualityComparable, given one object.
    */

    template <typename T>
    struct EqualityComparable_basic_test_one_object : OKlib::TestSystem::Test {
      typedef EqualityComparable_basic_test_one_object test_type;
      EqualityComparable_basic_test_one_object(const T& x) : x(x) {}
    private :
      const T& x;
      void perform_test_trivial() {
        OKLIB_TESTTRIVIAL_RETHROW(EqualityComparable_Axiom_reflexivity<T>(x));
      }
    };

    /*!
      \class EqualityComparable_basic_test_two_objects
      \brief Basic test for EqualityComparable, given two objects.
    */

    template <typename T>
    struct EqualityComparable_basic_test_two_objects : OKlib::TestSystem::Test {
      typedef EqualityComparable_basic_test_two_objects test_type;
      EqualityComparable_basic_test_two_objects(const T& x, const T& y) : x(x), y(y) {}
    private :
      const T& x, y;
      void perform_test_trivial() {
        OKLIB_TESTTRIVIAL_RETHROW(EqualityComparable_basic_test_one_object<T>(x));
        OKLIB_TESTTRIVIAL_RETHROW(EqualityComparable_basic_test_one_object<T>(y));
        OKLIB_TESTTRIVIAL_RETHROW(EqualityComparable_Axiom_symmetry<T>(x, y));
      }
    };

    /*!
      \class EqualityComparable_basic_test_three_objects
      \brief Basic test for EqualityComparable, given two objects.
    */

    template <typename T>
    struct EqualityComparable_basic_test_three_objects : OKlib::TestSystem::Test {
      typedef EqualityComparable_basic_test_three_objects test_type;
      EqualityComparable_basic_test_three_objects(const T& x, const T& y) : x(x), y(y), z(z) {}
    private :
      const T& x, y, z;
      void perform_test_trivial() {
        OKLIB_TESTTRIVIAL_RETHROW(EqualityComparable_basic_test_two_objects<T>(x, y));
        OKLIB_TESTTRIVIAL_RETHROW(EqualityComparable_basic_test_two_objects<T>(x, z));
        OKLIB_TESTTRIVIAL_RETHROW(EqualityComparable_basic_test_two_objects<T>(y, z));
        OKLIB_TESTTRIVIAL_RETHROW(EqualityComparable_Axiom_transitivity<T>(x, y, z));
      }
    };

    // ############################################################

    /*!
      \class LessThanComparable_Axiom_irreflexivity
      \brief Checks for objects a whether not (a < a) holds.
    */
    
    template <typename T>
    struct LessThanComparable_Axiom_irreflexivity : OKlib::TestSystem::Test {
      typedef LessThanComparable_Axiom_irreflexivity test_type;
      LessThanComparable_Axiom_irreflexivity(const T& a) : a(a) {}
    private :
      const T& a;
      void perform_test_trivial() {
        if (a < a)
          OKLIB_THROW("a < a");
      }
    };

    /*!
      \class LessThanComparable_Axiom_asymmetry
      \brief Checks that not a < b and b < a holds.
    */

    template <typename T>
    struct LessThanComparable_Axiom_asymmetry : OKlib::TestSystem::Test {
      typedef LessThanComparable_Axiom_asymmetry test_type;
      LessThanComparable_Axiom_asymmetry(const T& a, const T& b) : a(a), b(b) {}
    private :
      const T& a, b;
      void perform_test_trivial() {
        if (a < b and b < a)
            OKLIB_THROW("a < b and b < a");
      }
    };

    /*!
      \class LessThanComparable_Axiom_transitivity
      \brief Checks for objects a, b, c with a < b and b < c, whether a < c holds.
    */

    template <typename T>
    struct LessThanComparable_Axiom_transitivity : OKlib::TestSystem::Test {
      typedef LessThanComparable_Axiom_transitivity test_type;
      LessThanComparable_Axiom_transitivity(const T& a, const T& b, const T& c) : a(a), b(b), c(c) {}
    private :
      const T& a, b, c;
      void perform_test_trivial() {
        const int ab = (a < b) + (b < a);
        assert(ab < 2); // this and the following asserts must habe been checked before
        const int ac = (a < c) + (c < a);
        assert(ac < 2); 
        const int bc = (b < c) + (c < b);
        assert(bc < 2);
        const int count_less = ab + ac + bc;
        // ?????????????????????????????????????????????????????????
          

      }
    };

    /*!
      \class LessThanComparable_Axiom_equivalence_reflexivity
      \brief Checks for objects a whether a ~ a holds.

      Is always fulfilled if < is known to be irreflexive.
    */
    
    template <typename T>
    struct LessThanComparable_Axiom_equivalence_reflexivity : OKlib::TestSystem::Test {
      typedef LessThanComparable_Axiom_equivalence_reflexivity test_type;
      LessThanComparable_Axiom_equivalence_reflexivity(const T& a) : a(a) {}
    private :
      const T& a;
      typedef OKlib::OrderRelations::Equivalence<std::less<T> > equivalence;
      equivalence equivalent;
      void perform_test_trivial() {
        if (not equivalent(a,a))
          OKLIB_THROW("not a ~ a");
      }
    };

    /*!
      \class LessThanComparable_Axiom_equivalence_symmetry
      \brief Checks for objects a whether a ~  b => b ~ a holds.

      Is fulfilled if const correctness can be assumed.
    */
    
    template <typename T>
    struct LessThanComparable_Axiom_equivalence_symmetry : OKlib::TestSystem::Test {
      typedef LessThanComparable_Axiom_equivalence_symmetry test_type;
      LessThanComparable_Axiom_equivalence_symmetry(const T& a, const T& b) : a(a), b(b) {}
    private :
      const T& a, b;
      typedef OKlib::OrderRelations::Equivalence<std::less<T> > equivalence;
      equivalence equivalent;
      void perform_test_trivial() {
        const bool ab = equivalent(a,b);
        const bool ba = equivalent(b,a);
        switch (ab) {
        case true :
          if (not ba)
            OKLIB_THROW("a ~ b and not b ~ a");
          break;
        case false :
          if (ba)
            OKLIB_THROW("not a ~ b and b ~ a");
          break;
        }
      }
    };

    /*!
      \class LessThanComparable_Axiom_equivalence_transitivity
      \brief Checks for objects a, b, c with a ~ b and b ~ c, whether a ~ c holds.
    */

    template <typename T>
    struct LessThanComparable_Axiom_equivalence_transitivity : OKlib::TestSystem::Test {
      typedef LessThanComparable_Axiom_equivalence_transitivity test_type;
      LessThanComparable_Axiom_equivalence_transitivity(const T& a, const T& b, const T& c) : a(a), b(b), c(c) {}
    private :
      const T& a, b, c;
      typedef OKlib::OrderRelations::Equivalence<std::less<T> > equivalence;
      equivalence equivalent;
      void perform_test_trivial() {
        if (equivalent(a,b) and equivalent(b,c) and not equivalent(a,c))
          OKLIB_THROW("a ~ b and b ~ c and not a ~ c");
      }
    };

    /*!
      \class LessThanComparable_basic_test_one_object
      \brief Basic test for LessThanComparable, given one object.
    */

    template <typename T>
    struct LessThanComparable_basic_test_one_object : OKlib::TestSystem::Test {
      typedef LessThanComparable_basic_test_one_object test_type;
      LessThanComparable_basic_test_one_object(const T& x) : x(x) {}
    private :
      const T& x;
      void perform_test_trivial() {
        OKLIB_TESTTRIVIAL_RETHROW(LessThanComparable_Axiom_irreflexivity<T>(x));
      }
    };

    /*!
      \class LessThanComparable_basic_test_two_objects
      \brief Basic test for LessThanComparable, given two objects.
    */

    template <typename T>
    struct LessThanComparable_basic_test_two_objects : OKlib::TestSystem::Test {
      typedef LessThanComparable_basic_test_two_objects test_type;
      LessThanComparable_basic_test_two_objects(const T& x, const T& y) : x(x), y(y) {}
    private :
      const T& x, y;
      void perform_test_trivial() {
        OKLIB_TESTTRIVIAL_RETHROW(LessThanComparable_basic_test_one_object<T>(x));
        OKLIB_TESTTRIVIAL_RETHROW(LessThanComparable_basic_test_one_object<T>(y));
        OKLIB_TESTTRIVIAL_RETHROW(LessThanComparable_Axiom_asymmetry<T>(x, y));
      }
    };

    /*!
      \class LessThanComparable_basic_test_three_objects
      \brief Basic test for LessThanComparable, given two objects.
    */

    template <typename T>
    struct LessThanComparable_basic_test_three_objects : OKlib::TestSystem::Test {
      typedef LessThanComparable_basic_test_three_objects test_type;
      LessThanComparable_basic_test_three_objects(const T& x, const T& y) : x(x), y(y), z(z) {}
    private :
      const T& x, y, z;
      void perform_test_trivial() {
        OKLIB_TESTTRIVIAL_RETHROW(LessThanComparable_basic_test_two_objects<T>(x, y));
        OKLIB_TESTTRIVIAL_RETHROW(LessThanComparable_basic_test_two_objects<T>(x, z));
        OKLIB_TESTTRIVIAL_RETHROW(LessThanComparable_basic_test_two_objects<T>(y, z));
        OKLIB_TESTTRIVIAL_RETHROW(LessThanComparable_Axiom_transitivity<T>(x, y, z));
      }
    };


  }

}

#endif
