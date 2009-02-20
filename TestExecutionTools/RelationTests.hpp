// Oliver Kullmann, 27.12.2005 (Swansea)
/* Copyright 2005 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file TestExecutionTools/RelationTests.hpp
  \brief Tests for relations, like symmetry and transitivity.

  \todo Implement (similar to TestReflexivity) tests for:
   - partial order (quasi-order, antisymmetric)
   - linear order (partial order, total)
   - strict order (irreflexiv, transitiv)
   - strict total order (strict order, trichotomy)
   - strict total order with equivalence ("strict weak ordering" in the language
     of the C++ standard) (strict total order, the induced similarity relation
     x ~ y <=> not (x < y) and not (y < x)
     is an equivalence relation).
  \todo Implement tests for
   - R' is the complement of R
   - R' is the inverse of R
   - R = R'.
  \todo Implement the following induced relations (in module OrderRelations)
   - given a quasi-order R, the canonical equivalence E is
     x E y <=> x R y and y R x
   - given a strict order R, the associated partial order P is
     x P y <=> x R y or x == y
   - given a strict order R, the induced similarity relation S is
     x S y <=> not (x < y) and not (y < x)
   - given a strict order R, the associated total quasi-order Q is
     x Q y <=> x R y or x S y.
  \todo Improve the tests (so that they become more meaningful).
*/

#ifndef RELATIONTESTS_zmzmznVb6
#define RELATIONTESTS_zmzmznVb6

#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <boost/range/const_iterator.hpp>
#include <boost/utility.hpp>

#include <OKlib/TestSystem/TestBaseClass.hpp>
#include <OKlib/TestSystem/TestExceptions.hpp>

namespace OKlib {

  namespace TestExecutionTools {

    /*!
      \class TestReflexivity
      \brief Tests whether a binary predicate on a range of objects is reflexiv.
      \todo Error reporting: report the index.
    */

    template <typename Range, class BinaryPredicate>
    struct TestReflexivity : OKlib::TestSystem::Test {
      typedef TestReflexivity test_type;
      TestReflexivity(const Range& r, const BinaryPredicate p) : r(r), p(p) {}
    private :
      const Range& r;
      const BinaryPredicate p;
      void perform_test_trivial() {
        typedef typename boost::range_const_iterator<Range>::type iterator;
        const iterator& end(boost::end(r));
        for (iterator i(boost::begin(r)); i != end; ++i)
          if (not p(*i, *i))
            OKLIB_THROW("irreflexive element found");
      }
    };

    /*!
      \class TestIrreflexivity
      \brief Tests whether a binary predicate on a range of objects is irreflexiv.
      \todo Error reporting: report the index.
    */

    template <typename Range, class BinaryPredicate>
    struct TestIrreflexivity : OKlib::TestSystem::Test {
      typedef TestIrreflexivity test_type;
      TestIrreflexivity(const Range& r, const BinaryPredicate p) : r(r), p(p) {}
    private :
      const Range& r;
      const BinaryPredicate p;
      void perform_test_trivial() {
        typedef typename boost::range_const_iterator<Range>::type iterator;
        const iterator& end(boost::end(r));
        for (iterator i(boost::begin(r)); i != end; ++i)
          if (p(*i, *i))
            OKLIB_THROW("reflexive element found");
      }
    };

    // ###################################################

    /*!
      \class TestSymmetry
      \brief Tests whether a binary predicate on a range of objects is symmetric.
      \todo Error reporting: report the indices.
    */

    template <typename Range, class BinaryPredicate>
    struct TestSymmetry : OKlib::TestSystem::Test {
      typedef TestSymmetry test_type;
      TestSymmetry(const Range& r, const BinaryPredicate p) : r(r), p(p) {}
    private :
      const Range& r;
      const BinaryPredicate p;
      void perform_test_trivial() {
        typedef typename boost::range_const_iterator<Range>::type iterator;
        const iterator& end(boost::end(r));
        for (iterator i(boost::begin(r)); i != end; ++i)
          for (iterator j(boost::next(i)); j != end; ++j)
            if (bool(p(*i,*j)) + bool(p(*j,*i)) == 1)
              OKLIB_THROW("non-symmetric pair found");
      }
    };

    /*!
      \class TestAsymmetry
      \brief Tests whether a binary predicate on a range of objects is asymmetric.
      \todo Error reporting: report the indices.
    */

    template <typename Range, class BinaryPredicate>
    struct TestAsymmetry : OKlib::TestSystem::Test {
      typedef TestAsymmetry test_type;
      TestAsymmetry(const Range& r, const BinaryPredicate p) : r(r), p(p) {}
    private :
      const Range& r;
      const BinaryPredicate p;
      void perform_test_trivial() {
        typedef typename boost::range_const_iterator<Range>::type iterator;
        const iterator& end(boost::end(r));
        for (iterator i(boost::begin(r)); i != end; ++i)
          for (iterator j(i); j != end; ++j)
            if (bool(p(*i,*j)) + bool(p(*j,*i)) == 2)
              OKLIB_THROW("symmetric pair found");
      }
    };

    /*!
      \class TestAntisymmetry
      \brief Tests whether a binary predicate on a range of objects is antisymmetric.
      \todo Error reporting: report the indices.
    */

    template <typename Range, class BinaryPredicate>
    struct TestAntisymmetry : OKlib::TestSystem::Test {
      typedef TestAntisymmetry test_type;
      TestAntisymmetry(const Range& r, const BinaryPredicate p) : r(r), p(p) {}
    private :
      const Range& r;
      const BinaryPredicate p;
      void perform_test_trivial() {
        typedef typename boost::range_const_iterator<Range>::type iterator;
        const iterator& end(boost::end(r));
        for (iterator i(boost::begin(r)); i != end; ++i)
          for (iterator j(i); j != end; ++j)
            if (p(*i,*j) and p(*j,*i) and not (*i == *j))
              OKLIB_THROW("symmetric but unequal pair found");
      }
    };

    // ###################################################

    /*!
      \class TestTotality
      \brief Tests whether a binary predicate on a range of objects is total.
      \todo Error reporting: report the indices.
    */

    template <typename Range, class BinaryPredicate>
    struct TestTotality : OKlib::TestSystem::Test {
      typedef TestTotality test_type;
      TestTotality(const Range& r, const BinaryPredicate p) : r(r), p(p) {}
    private :
      const Range& r;
      const BinaryPredicate p;
      void perform_test_trivial() {
        typedef typename boost::range_const_iterator<Range>::type iterator;
        const iterator& end(boost::end(r));
        for (iterator i(boost::begin(r)); i != end; ++i)
          for (iterator j(i); j != end; ++j)
            if (bool(p(*i,*j)) + bool(p(*j,*i)) == 0)
              OKLIB_THROW("incomparable pair found");
      }
    };

    /*!
      \class TestTrichotomy
      \brief Tests whether a binary predicate on a range of objects fulfils trichotomy.
      \todo Error reporting: report the indices.
    */

    template <typename Range, class BinaryPredicate>
    struct TestTrichotomy : OKlib::TestSystem::Test {
      typedef TestTrichotomy test_type;
      TestTrichotomy(const Range& r, const BinaryPredicate p) : r(r), p(p) {}
    private :
      const Range& r;
      const BinaryPredicate p;
      void perform_test_trivial() {
        typedef typename boost::range_const_iterator<Range>::type iterator;
        const iterator& end(boost::end(r));
        for (iterator i(boost::begin(r)); i != end; ++i)
          for (iterator j(i); j != end; ++j)
            if (bool(p(*i,*j)) + bool(p(*j,*i)) + bool(*i == *j) != 1)
              OKLIB_THROW("non-trichotomic pair found");
      }
    };

    // ###################################################

    /*!
      \class TestTransitivity
      \brief Tests whether a binary predicate on a range of objects is transitiv.
      \todo Error reporting: report the indices.
    */

    template <typename Range, class BinaryPredicate>
    struct TestTransitivity : OKlib::TestSystem::Test {
      typedef TestTransitivity test_type;
      TestTransitivity(const Range& r, const BinaryPredicate p) : r(r), p(p) {}
    private :
      const Range& r;
      const BinaryPredicate p;
      void perform_test_trivial() {
        typedef typename boost::range_const_iterator<Range>::type iterator;
        const iterator& end(boost::end(r));
        const iterator& begin(boost::begin(r));
        for (iterator i(begin); i != end; ++i)
          for (iterator j(begin); j != end; ++j)
            for (iterator k(begin); k != end; ++k)
              if (p(*i,*j) and p(*j,*k) and not p(*i,*k))
                OKLIB_THROW("non-transitive triple found");
      }
    };

    // ###################################################

    /*!
      \class TestQuasiorder
      \brief Tests whether a binary predicate on a range of objects is a quasiorder.
      \todo Error reporting: report the indices.
    */

    template <typename Range, class BinaryPredicate>
    struct TestQuasiorder : OKlib::TestSystem::Test {
      typedef TestQuasiorder test_type;
      TestQuasiorder(const Range& r, const BinaryPredicate p) : r(r), p(p) {}
    private :
      const Range& r;
      const BinaryPredicate p;
      void perform_test_trivial() {
        typedef typename boost::range_const_iterator<Range>::type iterator;
        OKLIB_TESTTRIVIAL_RETHROW((TestReflexivity<Range,BinaryPredicate>(r,p)));
        OKLIB_TESTTRIVIAL_RETHROW((TestTransitivity<Range,BinaryPredicate>(r,p)));
      }
    };

    /*!
      \class TestEquivalence
      \brief Tests whether a binary predicate on a range of objects is an equivalence.
      \todo Error reporting: report the indices.
    */

    template <typename Range, class BinaryPredicate>
    struct TestEquivalence : OKlib::TestSystem::Test {
      typedef TestEquivalence test_type;
      TestEquivalence(const Range& r, const BinaryPredicate p) : r(r), p(p) {}
    private :
      const Range& r;
      const BinaryPredicate p;
      void perform_test_trivial() {
        typedef typename boost::range_const_iterator<Range>::type iterator;
        OKLIB_TESTTRIVIAL_RETHROW((TestQuasiorder<Range,BinaryPredicate>(r,p)));
        OKLIB_TESTTRIVIAL_RETHROW((TestSymmetry<Range,BinaryPredicate>(r,p)));
      }
    };

  }

}

#endif
