// Oliver Kullmann, 24.5.2005 (Swansea)
/* Copyright 2005 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Concepts/Basics_Tests.hpp
  \brief Test classes to help with determining, whether a class models the basic concepts.

  \todo
   - There must be (at least basic) tests for the tests itself.
   - Complete update!
   - Updating tests for FullyLessThanComparable (similar to tests for EqualityComparable).
   - Updating tests for LinearOrder (similar to tests for EqualityComparable).
   - Besides "axiomatix tests" we need "sporadic tests" (with given outcome).
     A range of (non-empy) ranges is the input here, where in case of ==-testing the elements in
     the single ranges are ==, and elements from different ranges are (not ==), while
     in case of < elements of the single ranges are equivalent (i.e., incomparable),
     and we have < for immediately succeeding ranges. The idea here is, that
     the minimal checks for these assertions are done here, and then a sequence
     of all the objects is fed into the systematic test from the previous points
     (altogether this must then guarantee that all asserted relations hold; for a LinearOrder
     for example we only need to compare within a single range neighbouring elements for
     ==, and the first elements of immediately successive ranges for <).
     Thus, for example for testing of a model of FullyConstructibleLinearOrder, the user only
     needs to provide such a set of sporadic test cases, and all possible checks are carried out.
*/

#ifndef BASICSTESTS_oLzW151
#define BASICSTESTS_oLzW151

#include <cassert>

#include <OKlib/TestSystem/TestBaseClass.hpp>
#include <OKlib/TestSystem/TestExceptions.hpp>

#include <OKlib/Concepts/std_Basics_Tests.hpp>

namespace OKlib {

  namespace Concepts {

    /*!
      \class FullyEqualityComparable_Axiom_equal_vs_unequal
      \brief Checks for objects a, b whether either (a == b and not (a != b)) or (not (a == b) and a != b) holds.
    */

    template <typename T>
    struct FullyEqualityComparable_Axiom_equal_vs_unequal : OKlib::TestSystem::Test {
      typedef FullyEqualityComparable_Axiom_equal_vs_unequal test_type;
      FullyEqualityComparable_Axiom_equal_vs_unequal(const T& a, const T& b) : a(a), b(b) {}
    private :
      const T& a, b;
      void perform_test_trivial() {
        const bool a_eq_b = (a == b);
        const bool a_neq_b = (a != b);
        switch (a_eq_b) {
        case true :
          if (a_neq_b)
            OKLIB_THROW("a == b and a != b");
          break;
        case false :
          if (not a_neq_b)
            OKLIB_THROW("not a == b and not a != b");
          break;
        }
      }
    };
    
    /*!
      \class FullyEqualityComparable_basic_test_one_object
      \brief Basic test for FullyEqualityComparable, given one object.
    */

    template <typename T>
    struct FullyEqualityComparable_basic_test_one_object : OKlib::TestSystem::Test {
      typedef FullyEqualityComparable_basic_test_one_object test_type;
      FullyEqualityComparable_basic_test_one_object(const T& x) : x(x) {}
    private :
      const T& x;
      void perform_test_trivial() {
        OKLIB_TESTTRIVIAL_RETHROW(EqualityComparable_basic_test_one_object<T>(x));
        OKLIB_TESTTRIVIAL_RETHROW(FullyEqualityComparable_Axiom_equal_vs_unequal<T>(x, x));
      }
    };

    /*!
      \class FullyEqualityComparable_basic_test_two_objects
      \brief Basic test for FullyEqualityComparable, given two objects.
    */

    template <typename T>
    struct FullyEqualityComparable_basic_test_two_objects : OKlib::TestSystem::Test {
      typedef FullyEqualityComparable_basic_test_two_objects test_type;
      FullyEqualityComparable_basic_test_two_objects(const T& x, const T& y) : x(x), y(y) {}
    private :
      const T& x, y;
      void perform_test_trivial() {
        OKLIB_TESTTRIVIAL_RETHROW(EqualityComparable_basic_test_two_objects<T>(x, y));
        OKLIB_TESTTRIVIAL_RETHROW(FullyEqualityComparable_basic_test_one_object<T>(x));
        OKLIB_TESTTRIVIAL_RETHROW(FullyEqualityComparable_basic_test_one_object<T>(y));
        OKLIB_TESTTRIVIAL_RETHROW(FullyEqualityComparable_Axiom_equal_vs_unequal<T>(x, y));
      }
    };

    /*!
      \class FullyEqualityComparable_basic_test_three_objects
      \brief Basic test for FullyEqualityComparable, given three objects.
    */

    template <typename T>
    struct FullyEqualityComparable_basic_test_three_objects : OKlib::TestSystem::Test {
      typedef FullyEqualityComparable_basic_test_three_objects test_type;
      FullyEqualityComparable_basic_test_three_objects(const T& x, const T& y, const T& z) : x(x), y(y), z(z) {}
    private :
      const T& x, y, z;
      void perform_test_trivial() {
        OKLIB_TESTTRIVIAL_RETHROW(EqualityComparable_basic_test_three_objects<T>(x, y, z));
        OKLIB_TESTTRIVIAL_RETHROW(FullyEqualityComparable_basic_test_two_objects<T>(x, y));
        OKLIB_TESTTRIVIAL_RETHROW(FullyEqualityComparable_basic_test_two_objects<T>(x, z));
        OKLIB_TESTTRIVIAL_RETHROW(FullyEqualityComparable_basic_test_two_objects<T>(y, z));
      }
    };

    // ############################################################

    /*!
      \class ConstructibleCEq_Axiom_copy_eq
      \brief Checks for object a whether for x copy-constructed from a we have x == a.
    */

    template <typename T>
    struct ConstructibleCEq_Axiom_copy_eq : OKlib::TestSystem::Test {
      typedef ConstructibleCEq_Axiom_copy_eq test_type;
      ConstructibleCEq_Axiom_copy_eq(const T& a) : a(a), x(a) {}
    private :
      const T& a;
      const T x;
      void perform_test_trivial() {
        if (not (a == x))
          OKLIB_THROW("not (a == x) after copy construction");
      }
    };

    /*!
      \class ConstructibleCEq_basic_test_one_object
      \brief Basic test for ConstructibleCEq, given one object.
    */

    template <typename T>
    struct ConstructibleCEq_basic_test_one_object : OKlib::TestSystem::Test {
      typedef ConstructibleCEq_basic_test_one_object test_type;
      ConstructibleCEq_basic_test_one_object(const T& x) : x(x) {}
    private :
      const T& x;
      void perform_test_trivial() {
        OKLIB_TESTTRIVIAL_RETHROW(FullyEqualityComparable_basic_test_one_object<T>(x));
        OKLIB_TESTTRIVIAL_RETHROW(ConstructibleCEq_Axiom_copy_eq<T>(x));
      }
    };

    /*!
      \class ConstructibleCEq_basic_test_two_objects
      \brief Basic test for ConstructibleCEq, given two objects.
    */

    template <typename T>
    struct ConstructibleCEq_basic_test_two_objects : OKlib::TestSystem::Test {
      typedef ConstructibleCEq_basic_test_two_objects test_type;
      ConstructibleCEq_basic_test_two_objects(const T& x, const T& y) : x(x), y(y) {}
    private :
      const T& x, y;
      void perform_test_trivial() {
        OKLIB_TESTTRIVIAL_RETHROW(ConstructibleCEq_basic_test_one_object<T>(x));
        OKLIB_TESTTRIVIAL_RETHROW(ConstructibleCEq_basic_test_one_object<T>(y));
        OKLIB_TESTTRIVIAL_RETHROW(FullyEqualityComparable_basic_test_two_objects<T>(x, y));
      }
    };

    /*!
      \class ConstructibleCEq_basic_test_three_objects
      \brief Basic test for ConstructibleCEq, given three objects.
    */

    template <typename T>
    struct ConstructibleCEq_basic_test_three_objects : OKlib::TestSystem::Test {
      typedef ConstructibleCEq_basic_test_three_objects test_type;
      ConstructibleCEq_basic_test_three_objects(const T& x, const T& y, const T& z) : x(x), y(y), z(z) {}
    private :
      const T& x, y, z;
      void perform_test_trivial() {
        OKLIB_TESTTRIVIAL_RETHROW(ConstructibleCEq_basic_test_two_objects<T>(x, y));
        OKLIB_TESTTRIVIAL_RETHROW(ConstructibleCEq_basic_test_two_objects<T>(x, z));
        OKLIB_TESTTRIVIAL_RETHROW(ConstructibleCEq_basic_test_two_objects<T>(y, z));
        OKLIB_TESTTRIVIAL_RETHROW(FullyEqualityComparable_basic_test_three_objects<T>(x, y, z));
      }
    };

    // ----------------------------------------------------------------------------------------------------------------------

    /*!
      \class ConstructibleCAEq_Axiom_assign_eq
      \brief Checks whether after assignment x = b we have x == b.
    */

    template <typename T>
    struct ConstructibleCAEq_Axiom_assign_eq : OKlib::TestSystem::Test {
      typedef ConstructibleCAEq_Axiom_assign_eq test_type;
      ConstructibleCAEq_Axiom_assign_eq(const T& a, const T& b) : a(a), b(b), x(a) {}
    private :
      const T& a, b;
      const T x;
      void perform_test_trivial() {
        x = b;
        if (not (x == b))
          OKLIB_THROW("not (x == b) after assignment x = b");
      }
    };

    /*!
      \class ConstructibleCAEq_basic_test_one_object
      \brief Basic test for ConstructibleCAEq, given one object.
    */

    template <typename T>
    struct ConstructibleCAEq_basic_test_one_object : OKlib::TestSystem::Test {
      typedef ConstructibleCAEq_basic_test_one_object test_type;
      ConstructibleCAEq_basic_test_one_object(const T& x) : x(x) {}
    private :
      const T& x;
      void perform_test_trivial() {
        OKLIB_TESTTRIVIAL_RETHROW(ConstructibleCEq_basic_test_one_object<T>(x));
        OKLIB_TESTTRIVIAL_RETHROW(ConstructibleCAEq_Axiom_assign_eq<T>(x, x));
      }
    };

    /*!
      \class ConstructibleCAEq_basic_test_two_objects
      \brief Basic test for ConstructibleCAEq, given two objects.
    */

    template <typename T>
    struct ConstructibleCAEq_basic_test_two_objects : OKlib::TestSystem::Test {
      typedef ConstructibleCAEq_basic_test_two_objects test_type;
      ConstructibleCAEq_basic_test_two_objects(const T& x, const T& y) : x(x), y(y) {}
    private :
      const T& x, y;
      void perform_test_trivial() {
        OKLIB_TESTTRIVIAL_RETHROW(ConstructibleCEq_basic_test_two_objects<T>(x, y));
        OKLIB_TESTTRIVIAL_RETHROW(ConstructibleCAEq_basic_test_one_object<T>(x));
        OKLIB_TESTTRIVIAL_RETHROW(ConstructibleCAEq_basic_test_one_object<T>(y));
        OKLIB_TESTTRIVIAL_RETHROW(ConstructibleCAEq_Axiom_assign_eq<T>(x, y));
      }
    };

    /*!
      \class ConstructibleCAEq_basic_test_three_objects
      \brief Basic test for ConstructibleCAEq, given three objects.
    */

    template <typename T>
    struct ConstructibleCAEq_basic_test_three_objects : OKlib::TestSystem::Test {
      typedef ConstructibleCAEq_basic_test_three_objects test_type;
      ConstructibleCAEq_basic_test_three_objects(const T& x, const T& y, const T& z) : x(x), y(y), z(z) {}
    private :
      const T& x, y, z;
      void perform_test_trivial() {
        OKLIB_TESTTRIVIAL_RETHROW(ConstructibleCEq_basic_test_three_objects<T>(x, y, z));
        OKLIB_TESTTRIVIAL_RETHROW(ConstructibleCAEq_basic_test_two_objects<T>(x, y));
        OKLIB_TESTTRIVIAL_RETHROW(ConstructibleCAEq_basic_test_two_objects<T>(x, z));
        OKLIB_TESTTRIVIAL_RETHROW(ConstructibleCAEq_basic_test_two_objects<T>(y, z));
      }
    };

    // ----------------------------------------------------------------------------------------------------------------------

    /*!
      \class ConstructibleDEq_Axiom_default_eq
      \brief Checks whether two default constructed objects are equal.
    */

    template <typename T>
    struct ConstructibleDEq_Axiom_default_eq : OKlib::TestSystem::Test {
      typedef ConstructibleDEq_Axiom_default_eq test_type;
      ConstructibleDEq_Axiom_default_eq() {}
    private :
      void perform_test_trivial() {
        if (not (T() == T()))
          OKLIB_THROW("not (T() == T())");
      }
    };

    /*!
      \class ConstructibleDEq_basic_test_no_object
      \brief Basic test for ConstructibleDEq (not requiring any object).
    */

    template <typename T>
    struct ConstructibleDEq_basic_test_no_object : OKlib::TestSystem::Test {
      typedef ConstructibleDEq_basic_test_no_object test_type;
      ConstructibleDEq_basic_test_no_object() {}
    private :
      void perform_test_trivial() {
        OKLIB_TESTTRIVIAL_RETHROW(ConstructibleDEq_Axiom_default_eq<T>());
      }
    };

    /*!
      \class ConstructibleDEq_basic_test_one_object
      \brief Basic test for ConstructibleDEq, given one object.
    */

    template <typename T>
    struct ConstructibleDEq_basic_test_one_object : OKlib::TestSystem::Test {
      typedef ConstructibleDEq_basic_test_one_object test_type;
      ConstructibleDEq_basic_test_one_object(const T& x) : x(x) {}
    private :
      const T& x;
      void perform_test_trivial() {
        OKLIB_TESTTRIVIAL_RETHROW(ConstructibleDEq_basic_test_no_object<T>());
        OKLIB_TESTTRIVIAL_RETHROW(FullyEqualityComparable_basic_test_one_object<T>(x));
      }
    };

    /*!
      \class ConstructibleDEq_basic_test_two_objects
      \brief Basic test for ConstructibleDEq, given two objects.
    */

    template <typename T>
    struct ConstructibleDEq_basic_test_two_objects : OKlib::TestSystem::Test {
      typedef ConstructibleDEq_basic_test_two_objects test_type;
      ConstructibleDEq_basic_test_two_objects(const T& x, const T& y) : x(x), y(y) {}
    private :
      const T& x, y;
      void perform_test_trivial() {
        OKLIB_TESTTRIVIAL_RETHROW(ConstructibleDEq_basic_test_one_object<T>(x));
        OKLIB_TESTTRIVIAL_RETHROW(ConstructibleDEq_basic_test_one_object<T>(y));
        OKLIB_TESTTRIVIAL_RETHROW(FullyEqualityComparable_basic_test_two_objects<T>(x, y));
      }
    };

    /*!
      \class ConstructibleDEq_basic_test_three_objects
      \brief Basic test for ConstructibleDEq, given three objects.
    */

    template <typename T>
    struct ConstructibleDEq_basic_test_three_objects : OKlib::TestSystem::Test {
      typedef ConstructibleDEq_basic_test_three_objects test_type;
      ConstructibleDEq_basic_test_three_objects(const T& x, const T& y, const T& z) : x(x), y(y), z(z) {}
    private :
      const T& x, y, z;
      void perform_test_trivial() {
        OKLIB_TESTTRIVIAL_RETHROW(ConstructibleDEq_basic_test_two_objects<T>(x, y));
        OKLIB_TESTTRIVIAL_RETHROW(ConstructibleDEq_basic_test_two_objects<T>(x, z));
        OKLIB_TESTTRIVIAL_RETHROW(ConstructibleDEq_basic_test_two_objects<T>(y, z));
        OKLIB_TESTTRIVIAL_RETHROW(FullyEqualityComparable_basic_test_three_objects<T>(x, y, z));
      }
    };

    // ----------------------------------------------------------------------------------------------------------------------

    /*!
      \class FullyConstructibleEq_basic_test_no_object
      \brief Basic test for FullyConstructibleEq (not requiring any object).
    */

    template <typename T>
    struct FullyConstructibleEq_basic_test_no_object : OKlib::TestSystem::Test {
      typedef FullyConstructibleEq_basic_test_no_object test_type;
      FullyConstructibleEq_basic_test_no_object() {}
    private :
      void perform_test_trivial() {
        OKLIB_TESTTRIVIAL_RETHROW(ConstructibleDEq_basic_test_no_object<T>());
      }
    };

    /*!
      \class FullyConstructibleEq_basic_test_one_object
      \brief Basic test for FullyConstructibleEq, given one object.
    */

    template <typename T>
    struct FullyConstructibleEq_basic_test_one_object : OKlib::TestSystem::Test {
      typedef FullyConstructibleEq_basic_test_one_object test_type;
      FullyConstructibleEq_basic_test_one_object(const T& x) : x(x) {}
    private :
      const T& x;
      void perform_test_trivial() {
        OKLIB_TESTTRIVIAL_RETHROW(ConstructibleDEq_basic_test_no_object<T>());
        OKLIB_TESTTRIVIAL_RETHROW(ConstructibleCAEq_basic_test_one_object<T>(x));
      }
    };

    /*!
      \class FullyConstructibleEq_basic_test_two_objects
      \brief Basic test for FullyEqualityComparable, given two objects.
    */

    template <typename T>
    struct FullyConstructibleEq_basic_test_two_objects : OKlib::TestSystem::Test {
      typedef FullyConstructibleEq_basic_test_two_objects test_type;
      FullyConstructibleEq_basic_test_two_objects(const T& x, const T& y) : x(x), y(y) {}
    private :
      const T& x, y;
      void perform_test_trivial() {
        OKLIB_TESTTRIVIAL_RETHROW(FullyConstructibleEq_basic_test_one_object<T>(x));
        OKLIB_TESTTRIVIAL_RETHROW(FullyConstructibleEq_basic_test_one_object<T>(y));
        OKLIB_TESTTRIVIAL_RETHROW(ConstructibleCAEq_basic_test_two_objects<T>(x, y));
      }
    };

    /*!
      \class FullyConstructibleEq_basic_test_three_objects
      \brief Basic test for FullyEqualityComparable, given three objects.
    */

    template <typename T>
    struct FullyConstructibleEq_basic_test_three_objects : OKlib::TestSystem::Test {
      typedef FullyConstructibleEq_basic_test_three_objects test_type;
      FullyConstructibleEq_basic_test_three_objects(const T& x, const T& y, const T& z) : x(x), y(y), z(z) {}
    private :
      const T& x, y, z;
      void perform_test_trivial() {
        OKLIB_TESTTRIVIAL_RETHROW(FullyConstructibleEq_basic_test_two_objects<T>(x, y));
        OKLIB_TESTTRIVIAL_RETHROW(FullyConstructibleEq_basic_test_two_objects<T>(x, z));
        OKLIB_TESTTRIVIAL_RETHROW(FullyConstructibleEq_basic_test_two_objects<T>(y, z));
        OKLIB_TESTTRIVIAL_RETHROW(ConstructibleCAEq_basic_test_three_objects<T>(x, y, z));
      }
    };

    // ############################################################

    template <typename T>
    struct FullyLessThanComparable_Axiom_greater_positive : OKlib::TestSystem::Test {
      typedef FullyLessThanComparable_Axiom_greater_positive test_type;
      FullyLessThanComparable_Axiom_greater_positive(const T& a, const T& b) : a(a), b(b) {
        assert(b < a);
      }
    private :
      const T& a, b;
      void perform_test_trivial() {
        if (not (a > b))
          OKLIB_THROW("not a > b");
      }
    };

    template <typename T>
    struct FullyLessThanComparable_Axiom_greater_negative : OKlib::TestSystem::Test {
      typedef FullyLessThanComparable_Axiom_greater_negative test_type;
      FullyLessThanComparable_Axiom_greater_negative(const T& a, const T& b) : a(a), b(b) {
        assert(a > b);
      }
    private :
      const T& a, b;
      void perform_test_trivial() {
        if (not (b < a))
          OKLIB_THROW("not b < a");
      }
    };
  
    template <typename T>
    struct FullyLessThanComparable_Axiom_less_equal_positive : OKlib::TestSystem::Test {
      typedef FullyLessThanComparable_Axiom_less_equal_positive test_type;
      FullyLessThanComparable_Axiom_less_equal_positive(const T& a, const T& b) : a(a), b(b) {
        assert(not (a > b));
      }
    private :
      const T& a, b;
      void perform_test_trivial() {
        if (not (a <= b))
          OKLIB_THROW("not a <= b");
      }
    };

    template <typename T>
    struct FullyLessThanComparable_Axiom_less_equal_negative : OKlib::TestSystem::Test {
      typedef FullyLessThanComparable_Axiom_less_equal_negative test_type;
      FullyLessThanComparable_Axiom_less_equal_negative(const T& a, const T& b) : a(a), b(b) {
        assert(a <= b);
      }
    private :
      const T& a, b;
      void perform_test_trivial() {
        if (a > b)
          OKLIB_THROW("not not a > b");
      }
    };

    template <typename T>
    struct FullyLessThanComparable_Axiom_greater_equal_positive : OKlib::TestSystem::Test {
      typedef FullyLessThanComparable_Axiom_greater_equal_positive test_type;
      FullyLessThanComparable_Axiom_greater_equal_positive(const T& a, const T& b) : a(a), b(b) {
        assert(b <= a);
      }
    private :
      const T& a, b;
      void perform_test_trivial() {
        if (not (a >= b))
          OKLIB_THROW("not a >= b");
      }
    };

    template <typename T>
    struct FullyLessThanComparable_Axiom_greater_equal_negative : OKlib::TestSystem::Test {
      typedef FullyLessThanComparable_Axiom_greater_equal_negative test_type;
      FullyLessThanComparable_Axiom_greater_equal_negative(const T& a, const T& b) : a(a), b(b) {
        assert(a >= b);
      }
    private :
      const T& a, b;
      void perform_test_trivial() {
        if (not (b <= a))
          OKLIB_THROW("not b <= a");
      }
    };

    template <typename T>
    struct FullyLessThanComparable_basic_test_one_object : OKlib::TestSystem::Test {
      typedef FullyLessThanComparable_basic_test_one_object test_type;
      FullyLessThanComparable_basic_test_one_object(const T& x) : x(x) {}
    private :
      const T& x;
      void perform_test_trivial() {
        OKLIB_TESTTRIVIAL_RETHROW(FullyLessThanComparable_Axiom_less_equal_positive<T>(x, x));
        OKLIB_TESTTRIVIAL_RETHROW(FullyLessThanComparable_Axiom_less_equal_negative<T>(x, x));
        OKLIB_TESTTRIVIAL_RETHROW(FullyLessThanComparable_Axiom_greater_equal_positive<T>(x, x));
        OKLIB_TESTTRIVIAL_RETHROW(FullyLessThanComparable_Axiom_greater_equal_negative<T>(x, x));
      }
    };

    // ############################################################

    template <typename T>
    struct LinearOrder_Axiom_positive : OKlib::TestSystem::Test {
      typedef LinearOrder_Axiom_positive test_type;
      LinearOrder_Axiom_positive(const T& a, const T& b) : a(a), b(b) {
        assert(not (a < b) and not (b < a));
      }
    private :
      const T& a, b;
      void perform_test_trivial() {
        if (not (a == b))
          OKLIB_THROW("not a == b");
      }
    };

    template <typename T>
    struct LinearOrder_Axiom_negative : OKlib::TestSystem::Test {
      typedef LinearOrder_Axiom_negative test_type;
      LinearOrder_Axiom_negative(const T& a, const T& b) : a(a), b(b) {
        assert(a == b);
      }
    private :
      const T& a, b;
      void perform_test_trivial() {
        if (a < b or b < a)
          OKLIB_THROW("a < b or b < a");
      }
    };

    template <typename T>
    struct LinearOrder_basic_test_one_object : OKlib::TestSystem::Test {
      typedef LinearOrder_basic_test_one_object test_type;
      LinearOrder_basic_test_one_object(const T& x) : x(x) {}
    private :
      const T& x;
      void perform_test_trivial() {
        OKLIB_TESTTRIVIAL_RETHROW(FullyLessThanComparable_basic_test_one_object<T>(x));
        OKLIB_TESTTRIVIAL_RETHROW(FullyEqualityComparable_basic_test_one_object<T>(x));
        OKLIB_TESTTRIVIAL_RETHROW(LinearOrder_Axiom_negative<T>(x, x));
      }
    };
    
  }

}

#endif
