// Oliver Kullmann, 26.5.2005 (Swansea)
/* Copyright 2005 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Concepts/Variables_Tests.hpp
  \brief Generic tests for models of variable concepts

  \deprecated Move to the new test system (and then using better error messages).

  \todo Perhaps within namespace Concepts::tests we have namespace Variables?
  And further differentiations are conceivable, corresponding to the
  refinements?

*/

#ifndef VARIABLESTESTS_kkgFre3
#define VARIABLESTESTS_kkgFre3

#include <OKlib/TestSystem/TestBaseClass.hpp>
#include <OKlib/TestSystem/TestExceptions.hpp>

#include <OKlib/Concepts/LibraryBasics.hpp>
#include <OKlib/Concepts/Basics_Tests.hpp>

#include <OKlib/traits/TypeTraits.hpp>

#include <OKlib/Concepts/Variables.hpp>
#include <OKlib/Satisfiability/ProblemInstances/Variables/traits/index_type.hpp>

namespace OKlib {
  namespace Concepts {

    /*!
      \class Variables_Axiom_singular_variables_identity
      \brief Test function for Concepts::Variables, checking that two
      singular variables are identical.
    */

    /*!
      \class Variables_Axiom_singular_variables_false
      \brief Test function for Concepts::Variables, checking that default-created variables are singular.

      Since in general the initialisation-behaviour of built-in types must be
      assumed, a local variable created by
      Var v;
      in general has an undefined value, and needs further initialisation before use.
    */

    template <typename Var>
    struct Variables_Axiom_singular_variables_false : OKlib::TestSystem::Test {
      typedef Variables_Axiom_singular_variables_false test_type;
    private :
      void perform_test_trivial() {
        if (Var())
          OKLIB_THROW("not not bool(Var())");
      }
    };

    template <typename Var>
    struct Variables_Axiom_singular_variables_identity : OKlib::TestSystem::Test {
      typedef Variables_Axiom_singular_variables_identity test_type;
      Variables_Axiom_singular_variables_identity(const Var& v, const Var& w) : v(v), w(w) {
        assert(not v);
        assert(not w);
      }
    private :
      const Var& v; const Var& w;
      void perform_test_trivial() {
        if (v != w)
          OKLIB_THROW("v != w");
      }
    };

    /*!
      \class Variables_basic_test
      \brief Basic test function for Concepts::Variables, which checks syntax,
      singularity of default-constructed variables, and applies the basic
      test for the linear order.
    */

    template <typename Var>
    struct Variables_basic_test : OKlib::TestSystem::TestBase {
      typedef Variables_basic_test test_type;
      Variables_basic_test() {
        insert(this);
      }
    private :
      void perform_test_trivial() {
        OKLIB_MODELS_CONCEPT_REQUIRES(Var, OKlib::Concepts::Variables);
        OKLIB_MODELS_CONCEPT_TAG(Var, OKlib::Concepts::Variables);
        OKLIB_TESTTRIVIAL_RETHROW(LinearOrder_basic_test_one_object<Var>(Var()));
        OKLIB_TESTTRIVIAL_RETHROW(Variables_Axiom_singular_variables_false<Var>());
        OKLIB_TESTTRIVIAL_RETHROW(Variables_Axiom_singular_variables_identity<Var>(Var(), Var()));
      }
    };

    // ---------------------------------------------------------------------------------------------------------------------

    /*!
      \class VariablesWithIndex_Axiom_index_nonnegative
      \brief Test function for concept Concepts::VariablesWithIndex, checking that
      indices of variables are not negative.
    */

    template <typename Var>
    struct VariablesWithIndex_Axiom_index_nonnegative : OKlib::TestSystem::Test {
      typedef VariablesWithIndex_Axiom_index_nonnegative test_type;
      VariablesWithIndex_Axiom_index_nonnegative(const Var& v) : v(v) {}
    private :
      const Var& v;
      typedef typename OKlib::Variables::traits::index_type<Var>::type index_type;
      void perform_test_trivial() {
        if (index_type(v) < 0)
          OKLIB_THROW("index(v) < 0");
      }
    };

    /*!
      \class VariablesWithIndex_Axiom_index_zero_positive
      \brief Test function for concept Concepts::VariablesWithIndex, checking
      that singular variables have index 0.
    */
  
    template <typename Var>
    struct VariablesWithIndex_Axiom_index_zero_positive : OKlib::TestSystem::Test {
      typedef VariablesWithIndex_Axiom_index_zero_positive test_type;
      VariablesWithIndex_Axiom_index_zero_positive(const Var& v) : v(v) {
        assert(not v);
      }
    private :
      const Var& v;
      typedef typename OKlib::Variables::traits::index_type<Var>::type index_type;
      void perform_test_trivial() {
        if (index_type(v) != 0)
          OKLIB_THROW("index(v) != 0");
      }
    };

    /*!
      \class VariablesWithIndex_Axiom_index_zero_negative
      \brief Test function for concept Concepts::VariablesWithIndex, checking that
      if the index is zero, then the variable is singular.
    */

    template <typename Var>
    struct VariablesWithIndex_Axiom_index_zero_negative : OKlib::TestSystem::Test {
      typedef VariablesWithIndex_Axiom_index_zero_negative test_type;
      VariablesWithIndex_Axiom_index_zero_negative(const Var& v) : v(v) {
        typedef typename OKlib::Variables::traits::index_type<Var>::type index_type;
        assert(index_type(v) == 0);
      }
    private :
      const Var& v;
      void perform_test_trivial() {
        if (bool(v))
          OKLIB_THROW("not not bool(v)");
      }
    };

    /*!
      \class VariablesWithIndex_Axiom_index_identity
      \brief Test function for concept Concepts::VariablesWithIndex, checking that
      variables with identical indices are identical.

      \todo This axiom actually should go into a refinement (variables with equal
      indices could be different, for example when used in different contexts) ?!?
    */

    template <typename Var>
    struct VariablesWithIndex_Axiom_index_identity : OKlib::TestSystem::Test {
      typedef VariablesWithIndex_Axiom_index_identity test_type;
      VariablesWithIndex_Axiom_index_identity(const Var& v, const Var& w) : v(v), w(w) {
        typedef typename OKlib::Variables::traits::index_type<Var>::type index_type;
        assert(index_type(v) == index_type(w));
      }
    private :
      const Var& v; const Var& w;
      void perform_test_trivial() {
        if (v != w)
          OKLIB_THROW("v != w");
      }
    };

    /*!
      \class VariablesWithIndex_basic_test
      \brief Basis text function for concept Concepts::VariablesWithIndex, checking
      syntax and that singular variables have index 0, and applying the basic test
      for concept Concept::Variables.

      The other tests are not applicable here, since (still) non-singular variables
      cannot be constructed.
    */

    template <typename Var>
    struct VariablesWithIndex_basic_test : OKlib::TestSystem::TestBase {
      typedef VariablesWithIndex_basic_test test_type;
      VariablesWithIndex_basic_test() {
        insert(this);
      }
    private :
      void perform_test_trivial() {
        OKLIB_MODELS_CONCEPT_REQUIRES(Var, OKlib::Concepts::VariablesWithIndex);
        OKLIB_MODELS_CONCEPT_TAG(Var, OKlib::Concepts::VariablesWithIndex);
        OKLIB_TESTTRIVIAL_RETHROW(Variables_basic_test<Var>());
        OKLIB_TESTTRIVIAL_RETHROW(VariablesWithIndex_Axiom_index_zero_positive<Var>(Var()));
      }
    };

    // ---------------------------------------------------------------------------------------------------------------------

    namespace TestIndex {
      template <typename index_type, bool is_signed>
      struct assert_index {
        assert_index(const index_type& i) {}
      };
      template <typename index_type>
      struct assert_index<index_type, true> {
        assert_index(const index_type& i) {
          assert(i >= 0);
        }
      };
      template <typename index_type>
      void assert_ge0(const index_type& i) {
        (assert_index<index_type, OKlib::traits::is_signed_integral<index_type>::value>(i));
      }
    }
    
    /*!
      \class VariablesAsIndex_Axiom
      \brief Test function for concept Concepts::VariablesAsIndex, checks that
      variables constructed from an index return the same index.
    */

    template <typename Var>
    struct VariablesAsIndex_Axiom : OKlib::TestSystem::Test {
      typedef VariablesAsIndex_Axiom test_type;
      typedef typename OKlib::Variables::traits::index_type<Var>::type index_type;
      VariablesAsIndex_Axiom(const index_type& i) : i(i) {
        TestIndex::assert_ge0(i);
      }
    private :
      const index_type& i;
      void perform_test_trivial() {
        if (index_type(Var(i)) != i)
          OKLIB_THROW("index(Var(i)) != i");
      }
    };

    /*!
      \class VariablesAsIndex_basic_test
      \brief Basic test function for concept Concepts::VariablesAsIndex, checking
      syntax, applying the basic test for Concepts::VariablesWithIndex, and
      checking creation of variables from indices.
    */

    template <typename Var>
    struct VariablesAsIndex_basic_test : OKlib::TestSystem::TestBase {
      typedef VariablesAsIndex_basic_test test_type;
      VariablesAsIndex_basic_test() {
        insert(this);
      }
    private :
      typedef typename OKlib::Variables::traits::index_type<Var>::type index_type;
      void perform_test_trivial() {
        OKLIB_MODELS_CONCEPT_REQUIRES(Var, OKlib::Concepts::VariablesAsIndex);
        OKLIB_MODELS_CONCEPT_TAG(Var, OKlib::Concepts::VariablesAsIndex);
        OKLIB_TESTTRIVIAL_RETHROW(VariablesWithIndex_basic_test<Var>());
        {
          const index_type max_index = 2;
          for (index_type i = 0; i <= max_index; ++i)
            OKLIB_TESTTRIVIAL_RETHROW(VariablesAsIndex_Axiom<Var>(i));
        }
        // XXXXXXXXXXXXXXXXXXXXXXXXXX
      }
    };

  }

}

#endif
