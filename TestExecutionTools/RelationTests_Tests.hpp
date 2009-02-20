// Oliver Kullmann, 27.12.2005 (Swansea)
/* Copyright 2005 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file TestExecutionTools/RelationTests_Tests.hpp
  \brief Tests for  the tests on relations regarding properties like symmetry and transitivity.

  \todo TestQuasiorder should use Test_TestEquivalence which in turn uses
  Test_TestReflexivity !
*/

#ifndef RELATIONTESTSTESTS_j5rfvp0
#define RELATIONTESTSTESTS_j5rfvp0

#include <functional>

#include <boost/iterator/counting_iterator.hpp>
#include <boost/range/iterator_range.hpp>

#include <OKlib/TestSystem/TestBaseClass.hpp>
#include <OKlib/TestSystem/TestExceptions.hpp>

#include <OKlib/TestExecutionTools/ExampleRelations.hpp>

namespace OKlib {

  namespace TestExecutionTools {

    /*!
      \class Test_TestReflexivity
      \brief Tests the test whether a binary predicate on a range of objects is reflexiv.
    */

    template <template <typename Range, class BinaryPredicate> class TestReflexivity>
    class Test_TestReflexivity : OKlib::TestSystem::TestBase {
    public :
      typedef Test_TestReflexivity test_type;
      Test_TestReflexivity() {
        insert(this);
      }
    private :
      void perform_test_trivial() {
        typedef int value_type;
        typedef boost::counting_iterator<value_type> iterator_type;
        typedef boost::iterator_range<iterator_type> range_type;
        const value_type range_size = 100;
        const range_type range(0, range_size);
        {
          typedef Id<value_type> id_type;
          OKLIB_TESTTRIVIAL_RETHROW((TestReflexivity<range_type,id_type>(range, id_type())));
        }
        {
          typedef Total<value_type> predicate_type;
          OKLIB_TESTTRIVIAL_RETHROW((TestReflexivity<range_type,predicate_type>(range, predicate_type())));
        }
        {
          typedef Empty<value_type> predicate_type;
          bool thrown = false;
          try {
            OKLIB_TESTTRIVIAL_RETHROW((TestReflexivity<range_type,predicate_type>(range, predicate_type())));
          }
          catch(const OKlib::TestSystem::TestException& e) {
            thrown = true;
          }
          if (not thrown)
            OKLIB_THROW("not thrown");
        }
      }
    };

    /*!
      \class Test_TestIrreflexivity
      \brief Tests the test whether a binary predicate on a range of objects is irreflexiv.
    */

    template <template <typename Range, class BinaryPredicate> class TestIrreflexivity>
    class Test_TestIrreflexivity : OKlib::TestSystem::TestBase {
    public :
      typedef Test_TestIrreflexivity test_type;
      Test_TestIrreflexivity() {
        insert(this);
      }
    private :
      void perform_test_trivial() {
        typedef int value_type;
        typedef boost::counting_iterator<value_type> iterator_type;
        typedef boost::iterator_range<iterator_type> range_type;
        const value_type range_size = 100;
        const range_type range(0, range_size);
        {
          typedef Id<value_type> predicate_type;
          bool thrown = false;
          try {
            OKLIB_TESTTRIVIAL_RETHROW((TestIrreflexivity<range_type,predicate_type>(range, predicate_type())));
          }
          catch(const OKlib::TestSystem::TestException& e) {
            thrown = true;
          }
          if (not thrown)
            OKLIB_THROW("not thrown");
        }
        {
          typedef Total<value_type> predicate_type;
          bool thrown = false;
          try {
            OKLIB_TESTTRIVIAL_RETHROW((TestIrreflexivity<range_type,predicate_type>(range, predicate_type())));
          }
          catch(const OKlib::TestSystem::TestException& e) {
            thrown = true;
          }
          if (not thrown)
            OKLIB_THROW("not thrown");
        }
        {
          typedef Empty<value_type> predicate_type;
          OKLIB_TESTTRIVIAL_RETHROW((TestIrreflexivity<range_type,predicate_type>(range, predicate_type())));
        }
      }
    };

    // #######################################################

    /*!
      \class Test_TestSymmetry
      \brief Tests the test whether a binary predicate on a range of objects is symmetric.
    */

    template <template <typename Range, class BinaryPredicate> class TestSymmetry>
    class Test_TestSymmetry : OKlib::TestSystem::TestBase {
    public :
      typedef Test_TestSymmetry test_type;
      Test_TestSymmetry() {
        insert(this);
      }
    private :
      void perform_test_trivial() {
        typedef int value_type;
        typedef boost::counting_iterator<value_type> iterator_type;
        typedef boost::iterator_range<iterator_type> range_type;
        const value_type range_size = 100;
        const range_type range(0, range_size);
        {
          typedef Id<value_type> predicate_type;
          OKLIB_TESTTRIVIAL_RETHROW((TestSymmetry<range_type,predicate_type>(range, predicate_type())));
        }
        {
          typedef Total<value_type> predicate_type;
          OKLIB_TESTTRIVIAL_RETHROW((TestSymmetry<range_type,predicate_type>(range, predicate_type())));
        }
        {
          typedef Empty<value_type> predicate_type;
          OKLIB_TESTTRIVIAL_RETHROW((TestSymmetry<range_type,predicate_type>(range, predicate_type())));
        }
        {
          typedef std::less<value_type> predicate_type;
          bool thrown = false;
          try {
            OKLIB_TESTTRIVIAL_RETHROW((TestSymmetry<range_type,predicate_type>(range, predicate_type())));
          }
          catch(const OKlib::TestSystem::TestException& e) {
            thrown = true;
          }
          if (not thrown)
            OKLIB_THROW("not thrown");
        }
      }
    };

    /*!
      \class Test_TestAsymmetry
      \brief Tests the test whether a binary predicate on a range of objects is asymmetric.
    */

    template <template <typename Range, class BinaryPredicate> class TestAsymmetry>
    class Test_TestAsymmetry : OKlib::TestSystem::TestBase {
    public :
      typedef Test_TestAsymmetry test_type;
      Test_TestAsymmetry() {
        insert(this);
      }
    private :
      void perform_test_trivial() {
        typedef int value_type;
        typedef boost::counting_iterator<value_type> iterator_type;
        typedef boost::iterator_range<iterator_type> range_type;
        const value_type range_size = 100;
        const range_type range(0, range_size);
        {
          typedef Id<value_type> predicate_type;
          bool thrown = false;
          try {
            OKLIB_TESTTRIVIAL_RETHROW((TestAsymmetry<range_type,predicate_type>(range, predicate_type())));
          }
          catch(const OKlib::TestSystem::TestException& e) {
            thrown = true;
          }
          if (not thrown)
            OKLIB_THROW("not thrown");
        }
        {
          typedef Total<value_type> predicate_type;
          bool thrown = false;
          try {
            OKLIB_TESTTRIVIAL_RETHROW((TestAsymmetry<range_type,predicate_type>(range, predicate_type())));
          }
          catch(const OKlib::TestSystem::TestException& e) {
            thrown = true;
          }
          if (not thrown)
            OKLIB_THROW("not thrown");
        }
        {
          typedef Empty<value_type> predicate_type;
          OKLIB_TESTTRIVIAL_RETHROW((TestAsymmetry<range_type,predicate_type>(range, predicate_type())));
        }
        {
          typedef std::less<value_type> predicate_type;
          OKLIB_TESTTRIVIAL_RETHROW((TestAsymmetry<range_type,predicate_type>(range, predicate_type())));
        }
      }
    };

    /*!
      \class Test_TestAntisymmetry
      \brief Tests the test whether a binary predicate on a range of objects is antisymmetric.
    */

    template <template <typename Range, class BinaryPredicate> class TestAntisymmetry>
    class Test_TestAntisymmetry : OKlib::TestSystem::TestBase {
    public :
      typedef Test_TestAntisymmetry test_type;
      Test_TestAntisymmetry() {
        insert(this);
      }
    private :
      void perform_test_trivial() {
        typedef int value_type;
        typedef boost::counting_iterator<value_type> iterator_type;
        typedef boost::iterator_range<iterator_type> range_type;
        const value_type range_size = 100;
        const range_type range(0, range_size);
        {
          typedef Id<value_type> predicate_type;
          OKLIB_TESTTRIVIAL_RETHROW((TestAntisymmetry<range_type,predicate_type>(range, predicate_type())));
        }
        {
          typedef Total<value_type> predicate_type;
          bool thrown = false;
          try {
            OKLIB_TESTTRIVIAL_RETHROW((TestAntisymmetry<range_type,predicate_type>(range, predicate_type())));
          }
          catch(const OKlib::TestSystem::TestException& e) {
            thrown = true;
          }
          if (not thrown)
            OKLIB_THROW("not thrown");
        }
        {
          typedef Empty<value_type> predicate_type;
          OKLIB_TESTTRIVIAL_RETHROW((TestAntisymmetry<range_type,predicate_type>(range, predicate_type())));
        }
        {
          typedef std::less_equal<value_type> predicate_type;
          OKLIB_TESTTRIVIAL_RETHROW((TestAntisymmetry<range_type,predicate_type>(range, predicate_type())));
        }
      }
    };

    // #######################################################

    /*!
      \class Test_TestTotality
      \brief Tests the test whether a binary predicate on a range of objects is total.
    */

    template <template <typename Range, class BinaryPredicate> class TestTotality>
    class Test_TestTotality : OKlib::TestSystem::TestBase {
    public :
      typedef Test_TestTotality test_type;
      Test_TestTotality() {
        insert(this);
      }
    private :
      void perform_test_trivial() {
        typedef int value_type;
        typedef boost::counting_iterator<value_type> iterator_type;
        typedef boost::iterator_range<iterator_type> range_type;
        const value_type range_size = 100;
        const range_type range(0, range_size);
        {
          typedef Id<value_type> predicate_type;
          bool thrown = false;
          try {
            OKLIB_TESTTRIVIAL_RETHROW((TestTotality<range_type,predicate_type>(range, predicate_type())));
          }
          catch(const OKlib::TestSystem::TestException& e) {
            thrown = true;
          }
          if (not thrown)
            OKLIB_THROW("not thrown");
        }
        {
          typedef Total<value_type> predicate_type;
          OKLIB_TESTTRIVIAL_RETHROW((TestTotality<range_type,predicate_type>(range, predicate_type())));
        }
        {
          typedef Empty<value_type> predicate_type;
          bool thrown = false;
          try {
            OKLIB_TESTTRIVIAL_RETHROW((TestTotality<range_type,predicate_type>(range, predicate_type())));
          }
          catch(const OKlib::TestSystem::TestException& e) {
            thrown = true;
          }
          if (not thrown)
            OKLIB_THROW("not thrown");
        }
        {
          typedef std::less<value_type> predicate_type;
          bool thrown = false;
          try {
            OKLIB_TESTTRIVIAL_RETHROW((TestTotality<range_type,predicate_type>(range, predicate_type())));
          }
          catch(const OKlib::TestSystem::TestException& e) {
            thrown = true;
          }
          if (not thrown)
            OKLIB_THROW("not thrown");
        }
        {
          typedef std::less_equal<value_type> predicate_type;
          OKLIB_TESTTRIVIAL_RETHROW((TestTotality<range_type,predicate_type>(range, predicate_type())));
        }
      }
    };

    /*!
      \class Test_TestTrichotomy
      \brief Tests the test whether a binary predicate on a range of objects fulfils trochotomy.
    */

    template <template <typename Range, class BinaryPredicate> class TestTrichotomy>
    class Test_TestTrichotomy : OKlib::TestSystem::TestBase {
    public :
      typedef Test_TestTrichotomy test_type;
      Test_TestTrichotomy() {
        insert(this);
      }
    private :
      void perform_test_trivial() {
        typedef int value_type;
        typedef boost::counting_iterator<value_type> iterator_type;
        typedef boost::iterator_range<iterator_type> range_type;
        const value_type range_size = 100;
        const range_type range(0, range_size);
        {
          typedef Id<value_type> predicate_type;
          bool thrown = false;
          try {
            OKLIB_TESTTRIVIAL_RETHROW((TestTrichotomy<range_type,predicate_type>(range, predicate_type())));
          }
          catch(const OKlib::TestSystem::TestException& e) {
            thrown = true;
          }
          if (not thrown)
            OKLIB_THROW("not thrown");
        }
        {
          typedef Total<value_type> predicate_type;
          bool thrown = false;
          try {
          OKLIB_TESTTRIVIAL_RETHROW((TestTrichotomy<range_type,predicate_type>(range, predicate_type())));
          }
          catch(const OKlib::TestSystem::TestException& e) {
            thrown = true;
          }
          if (not thrown)
            OKLIB_THROW("not thrown");
        }
        {
          typedef Empty<value_type> predicate_type;
          bool thrown = false;
          try {
            OKLIB_TESTTRIVIAL_RETHROW((TestTrichotomy<range_type,predicate_type>(range, predicate_type())));
          }
          catch(const OKlib::TestSystem::TestException& e) {
            thrown = true;
          }
          if (not thrown)
            OKLIB_THROW("not thrown");
        }
        {
          typedef std::less<value_type> predicate_type;
          OKLIB_TESTTRIVIAL_RETHROW((TestTrichotomy<range_type,predicate_type>(range, predicate_type())));
        }
        {
          typedef std::less_equal<value_type> predicate_type;
          bool thrown = false;
          try {
            OKLIB_TESTTRIVIAL_RETHROW((TestTrichotomy<range_type,predicate_type>(range, predicate_type())));
          }
          catch(const OKlib::TestSystem::TestException& e) {
            thrown = true;
          }
          if (not thrown)
            OKLIB_THROW("not thrown");
        }
      }
    };

    // #######################################################

    /*!
      \class Test_TestTransitivity
      \brief Tests the test whether a binary predicate on a range of objects is transitiv.
    */

    template <template <typename Range, class BinaryPredicate> class TestTransitivity>
    class Test_TestTransitivity : OKlib::TestSystem::TestBase {
    public :
      typedef Test_TestTransitivity test_type;
      Test_TestTransitivity() {
        insert(this);
      }
    private :
      void perform_test_trivial() {
        typedef int value_type;
        typedef boost::counting_iterator<value_type> iterator_type;
        typedef boost::iterator_range<iterator_type> range_type;
        const value_type range_size = 50;
        const range_type range(0, range_size);
        {
          typedef Id<value_type> predicate_type;
          OKLIB_TESTTRIVIAL_RETHROW((TestTransitivity<range_type,predicate_type>(range, predicate_type())));
        }
        {
          typedef Total<value_type> predicate_type;
         OKLIB_TESTTRIVIAL_RETHROW((TestTransitivity<range_type,predicate_type>(range, predicate_type())));
        }
        {
          typedef Empty<value_type> predicate_type;
          OKLIB_TESTTRIVIAL_RETHROW((TestTransitivity<range_type,predicate_type>(range, predicate_type())));
        }
        {
          typedef std::less<value_type> predicate_type;
          OKLIB_TESTTRIVIAL_RETHROW((TestTransitivity<range_type,predicate_type>(range, predicate_type())));
        }
        {
          typedef std::less_equal<value_type> predicate_type;
          OKLIB_TESTTRIVIAL_RETHROW((TestTransitivity<range_type,predicate_type>(range, predicate_type())));
        }
        {
          typedef std::not_equal_to<value_type> predicate_type;
          bool thrown = false;
          try {
            OKLIB_TESTTRIVIAL_RETHROW((TestTransitivity<range_type,predicate_type>(range, predicate_type())));
          }
          catch(const OKlib::TestSystem::TestException& e) {
            thrown = true;
          }
          if (not thrown)
            OKLIB_THROW("not thrown");
        }
      }
    };

    // #######################################################

    /*!
      \class Test_TestQuasiorder
      \brief Tests the test whether a binary predicate on a range of objects is a quasiorder.
    */

    template <template <typename Range, class BinaryPredicate> class TestQuasiorder>
    class Test_TestQuasiorder : OKlib::TestSystem::TestBase {
    public :
      typedef Test_TestQuasiorder test_type;
      Test_TestQuasiorder() {
        insert(this);
      }
    private :
      void perform_test_trivial() {
        typedef int value_type;
        typedef boost::counting_iterator<value_type> iterator_type;
        typedef boost::iterator_range<iterator_type> range_type;
        const value_type range_size = 50;
        const range_type range(0, range_size);
        {
          typedef Id<value_type> predicate_type;
          OKLIB_TESTTRIVIAL_RETHROW((TestQuasiorder<range_type,predicate_type>(range, predicate_type())));
        }
        {
          typedef Total<value_type> predicate_type;
          OKLIB_TESTTRIVIAL_RETHROW((TestQuasiorder<range_type,predicate_type>(range, predicate_type())));
        }
        {
          typedef Empty<value_type> predicate_type;
          bool thrown = false;
          try {
            OKLIB_TESTTRIVIAL_RETHROW((TestQuasiorder<range_type,predicate_type>(range, predicate_type())));
          }
          catch(const OKlib::TestSystem::TestException& e) {
            thrown = true;
          }
          if (not thrown)
            OKLIB_THROW("not thrown");
        }
        {
          typedef std::less<value_type> predicate_type;
          bool thrown = false;
          try {
            OKLIB_TESTTRIVIAL_RETHROW((TestQuasiorder<range_type,predicate_type>(range, predicate_type())));
          }
          catch(const OKlib::TestSystem::TestException& e) {
            thrown = true;
          }
          if (not thrown)
            OKLIB_THROW("not thrown");
        }
        {
          typedef std::less_equal<value_type> predicate_type;
          OKLIB_TESTTRIVIAL_RETHROW((TestQuasiorder<range_type,predicate_type>(range, predicate_type())));
        }
        {
          typedef std::not_equal_to<value_type> predicate_type;
          bool thrown = false;
          try {
            OKLIB_TESTTRIVIAL_RETHROW((TestQuasiorder<range_type,predicate_type>(range, predicate_type())));
          }
          catch(const OKlib::TestSystem::TestException& e) {
            thrown = true;
          }
          if (not thrown)
            OKLIB_THROW("not thrown");
        }
      }
    };

    /*!
      \class Test_TestEquivalence
      \brief Tests the test whether a binary predicate on a range of objects is an equivalence.
    */

    template <template <typename Range, class BinaryPredicate> class TestEquivalence>
    class Test_TestEquivalence : OKlib::TestSystem::TestBase {
    public :
      typedef Test_TestEquivalence test_type;
      Test_TestEquivalence() {
        insert(this);
      }
    private :
      void perform_test_trivial() {
        typedef int value_type;
        typedef boost::counting_iterator<value_type> iterator_type;
        typedef boost::iterator_range<iterator_type> range_type;
        const value_type range_size = 50;
        const range_type range(0, range_size);
        {
          typedef Id<value_type> predicate_type;
          OKLIB_TESTTRIVIAL_RETHROW((TestEquivalence<range_type,predicate_type>(range, predicate_type())));
        }
        {
          typedef Total<value_type> predicate_type;
          OKLIB_TESTTRIVIAL_RETHROW((TestEquivalence<range_type,predicate_type>(range, predicate_type())));
        }
        {
          typedef Empty<value_type> predicate_type;
          bool thrown = false;
          try {
            OKLIB_TESTTRIVIAL_RETHROW((TestEquivalence<range_type,predicate_type>(range, predicate_type())));
          }
          catch(const OKlib::TestSystem::TestException& e) {
            thrown = true;
          }
          if (not thrown)
            OKLIB_THROW("not thrown");
        }
        {
          typedef std::less<value_type> predicate_type;
          bool thrown = false;
          try {
            OKLIB_TESTTRIVIAL_RETHROW((TestEquivalence<range_type,predicate_type>(range, predicate_type())));
          }
          catch(const OKlib::TestSystem::TestException& e) {
            thrown = true;
          }
          if (not thrown)
            OKLIB_THROW("not thrown");
        }
        {
          typedef std::less_equal<value_type> predicate_type;
          bool thrown = false;
          try {
          OKLIB_TESTTRIVIAL_RETHROW((TestEquivalence<range_type,predicate_type>(range, predicate_type())));
          }
          catch(const OKlib::TestSystem::TestException& e) {
            thrown = true;
          }
          if (not thrown)
            OKLIB_THROW("not thrown");
        }
        {
          typedef std::not_equal_to<value_type> predicate_type;
          bool thrown = false;
          try {
            OKLIB_TESTTRIVIAL_RETHROW((TestEquivalence<range_type,predicate_type>(range, predicate_type())));
          }
          catch(const OKlib::TestSystem::TestException& e) {
            thrown = true;
          }
          if (not thrown)
            OKLIB_THROW("not thrown");
        }
      }
    };

  }

}

#endif
