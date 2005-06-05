// Oliver Kullmann, 24.5.2005 (Swansea)

#ifndef BASICSTESTS_oLzW151

#define BASICSTESTS_oLzW151

#include <cassert>

#include "TestBaseClass.hpp"
#include "TestExceptions.hpp"

namespace OKlib {

  namespace Concepts {

    template <typename T>
    struct FullyEqualityComparable_Axiom_positive : OKlib::TestSystem::Test {
      typedef FullyEqualityComparable_Axiom_positive test_type;
      FullyEqualityComparable_Axiom_positive(const T& a, const T& b) : a(a), b(b) {
        assert(a == b);
      }
    private :
      const T& a, b;
      void perform_test_trivial() {
        if (a != b)
          OKLIB_THROW("a != b");
      }
    };
    
    template <typename T>
    struct FullyEqualityComparable_Axiom_negative : OKlib::TestSystem::Test {
      typedef FullyEqualityComparable_Axiom_negative test_type;
      FullyEqualityComparable_Axiom_negative(const T& a, const T& b) : a(a), b(b) {
        assert(a != b);
      }
    private :
      const T& a, b;
      void perform_test_trivial() {
        if (not (a == b))
          OKLIB_THROW("a == b");
      }
    };

    template <typename T>
    struct FullyEqualityComparable_basic_test_one_object : OKlib::TestSystem::Test {
      typedef FullyEqualityComparable_basic_test_one_object test_type;
      FullyEqualityComparable_basic_test_one_object(const T& x) : x(x) {}
    private :
      const T& x;
      void perform_test_trivial() {
        OKLIB_TESTTRIVIAL_RETHROW(FullyEqualityComparable_Axiom_positive<T>(x, x));
      }
    };

    // ----------------------------------------------------------------------------------------------------------------------

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

    // ----------------------------------------------------------------------------------------------------------------------

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
