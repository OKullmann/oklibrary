// Oliver Kullmann, 11.6.2005 (Swansea)

#ifndef RESULTELEMENTTESTS_40098Ujk

#define RESULTELEMENTTESTS_40098Ujk

#include "TestBaseClass.hpp"
#include "TestExceptions.hpp"

#include "LibraryBasics.hpp"
#include "Basics_Tests.hpp"

#include "ResultElements.hpp"

namespace OKlib {

  namespace Concepts {

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

    template <typename Res>
    struct ResultElementWithName_basic_test : OKlib::TestSystem::TestBase {
      typedef ResultElementWithName_basic_test test_type;
      ResultElementWithName_basic_test() {
        insert(this);
      }
    private :
      void perform_test_trivial() {
        OKLIB_MODELS_CONCEPT_REQUIRES(Res, ResultElementWithName);
        // ToDo: To be completed.
      }
    };

    // -----------------------------------------------------------------------------------------------------------------

    // ToDo: to be completed.
   
  }

}

#endif
