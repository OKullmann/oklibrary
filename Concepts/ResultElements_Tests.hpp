// Oliver Kullmann, 11.6.2005 (Swansea)

/*!
  \file Concepts/ResultElements_Tests.hpp
  \brief Testing alleged models of (refinements of) concept Concepts::ResultElement

  \deprecated Move to the new test system.
  \todo Once the concepts in Concepts/ResultElements.hpp have been extended, they
  need to be tested.
*/

#ifndef RESULTELEMENTTESTS_40098Ujk
#define RESULTELEMENTTESTS_40098Ujk

#include <Transitional/TestSystem/TestBaseClass.hpp>
#include <Transitional/TestSystem/TestExceptions.hpp>

#include <Transitional/Concepts/LibraryBasics.hpp>
#include <Transitional/Concepts/Basics_Tests.hpp>

#include <Transitional/Concepts/ResultElements.hpp>

namespace OKlib {

  namespace Concepts {

    /*!
      \class ResultElement_basic_test
      \brief Test function for Concepts::ResultElement, which just checks the syntactic requirements.
    */

    template <typename Res>
    struct ResultElement_basic_test : OKlib::TestSystem::TestBase {
      typedef ResultElement_basic_test test_type;
      ResultElement_basic_test() {
        insert(this);
      }
    private :
      void perform_test_trivial() {
        OKLIB_MODELS_CONCEPT_REQUIRES(Res, ResultElement);
      }
    };

    // -----------------------------------------------------------------------------------------------------------------

    /*!
      \class ResultElementWithOrder_basic_test
      \brief Test function for Concepts::ResultElementWithOrder, checking syntax and applying
      the basic tests for linear order using one object.
    */

    template <typename Res>
    struct ResultElementWithOrder_basic_test : OKlib::TestSystem::TestBase {
      typedef ResultElementWithOrder_basic_test test_type;
      ResultElementWithOrder_basic_test() {
        insert(this);
      }
    private :
      void perform_test_trivial() {
        OKLIB_MODELS_CONCEPT_REQUIRES(Res, ResultElementWithOrder);
        OKLIB_TESTTRIVIAL_RETHROW(LinearOrder_basic_test_one_object<Res>(Res()));
      }
    };

    // -----------------------------------------------------------------------------------------------------------------

    /*!
      \class ResultElementWithName_Axiom_name_equality
      \brief Sub-test function for Concepts::ResultElementWithName, testing that equal names
      yield equal result elements.
    */

    template <typename Res>
    struct ResultElementWithName_Axiom_name_equality : OKlib::TestSystem::Test {
      typedef ResultElementWithName_Axiom_name_equality test_type;
      ResultElementWithName_Axiom_name_equality(const Res& a, const Res& b) : a(a), b(b) {
        assert(a.name() == b.name());
      }
    private :
      const Res& a; const Res& b;
      void perform_test_trivial() {
        if (a != b)
          OKLIB_THROW("a != b");
      }
    };

    /*!
      \class ResultElementWithName_Axiom_name_lessthan
      \brief Sub-test function for Concepts::ResultElementWithName, testing that if the name
      is less, then the result element is less.
    */

    template <typename Res>
    struct ResultElementWithName_Axiom_name_lessthan : OKlib::TestSystem::Test {
      typedef ResultElementWithName_Axiom_name_lessthan test_type;
      ResultElementWithName_Axiom_name_lessthan(const Res& a, const Res& b) : a(a), b(b) {
        assert(a.name() < b.name());
      }
    private :
      const Res& a; const Res& b;
      void perform_test_trivial() {
        if (not (a < b))
          OKLIB_THROW("not a < b");
      }
    };

    /*!
      \class ResultElementWithName_basic_test
      \brief Test function for Concepts::ResultElementWithName, checking the syntax and that the order
      on the names carries over to the result elements.

      \todo To be completed, by creating suitable names and result elements from those names,
      and calling the above sub-tests; furthermore the available general tests on linear orders
      shall be employed.
    */

    template <typename Res>
    struct ResultElementWithName_basic_test : OKlib::TestSystem::TestBase {
      typedef ResultElementWithName_basic_test test_type;
      ResultElementWithName_basic_test() {
        insert(this);
      }
    private :
      void perform_test_trivial() {
        OKLIB_MODELS_CONCEPT_REQUIRES(Res, ResultElementWithName);
        // XXXXXXXXX
      }
    };

    // -----------------------------------------------------------------------------------------------------------------
   
  }

}

#endif
