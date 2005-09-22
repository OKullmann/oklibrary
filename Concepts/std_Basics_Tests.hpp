// Oliver Kullmann, 24.5.2005 (Swansea)

/*!
  \file std_Basics_Tests.hpp
  \brief Test classes for the basic concepts from the standard.
  \todo Tests for LessThanComparable
*/

#ifndef STDBASICSTESTS_8455tGb

#define STDBASICSTESTS_8455tGb

#include <cassert>

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
        if ((a == b) and (b == c) and not (a == c))
          OKLIB_THROW("a == b and b == c and not a == c");
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

    // -----------------------------------------------------------------------------------------------------------------------------

  }

}

#endif
