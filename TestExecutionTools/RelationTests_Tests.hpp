// Oliver Kullmann, 27.12.2005 (Swansea)

/*!
  \file RelationTests_Tests.hpp
  \brief Tests for  the tests on relations regarding properties like symmetry and transitivity.
*/

#ifndef RELATIONTESTSTESTS_j5rfvp0

#define RELATIONTESTSTESTS_j5rfvp0

#include <boost/iterator/counting_iterator.hpp>
#include <boost/range/iterator_range.hpp>

#include "TestBaseClass.hpp"
#include "TestExceptions.hpp"

#include "ExampleRelations.hpp"

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
          const id_type id;
          OKLIB_TESTTRIVIAL_RETHROW((TestReflexivity<range_type,id_type>(range, id)));
        }
        {
          typedef Total<value_type> total_type;
          const total_type total;
          OKLIB_TESTTRIVIAL_RETHROW((TestReflexivity<range_type,total_type>(range, total)));
        }
        {
          typedef Empty<value_type> empty_type;
          const empty_type empty;
          bool thrown = false;
          try {
            OKLIB_TESTTRIVIAL_RETHROW((TestReflexivity<range_type,empty_type>(range, empty)));
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
          typedef Id<value_type> id_type;
          const id_type id;
          bool thrown = false;
          try {
            OKLIB_TESTTRIVIAL_RETHROW((TestIrreflexivity<range_type,id_type>(range, id)));
          }
          catch(const OKlib::TestSystem::TestException& e) {
            thrown = true;
          }
          if (not thrown)
            OKLIB_THROW("not thrown");
        }
        {
          typedef Total<value_type> total_type;
          const total_type total;
          bool thrown = false;
          try {
            OKLIB_TESTTRIVIAL_RETHROW((TestIrreflexivity<range_type,total_type>(range, total)));
          }
          catch(const OKlib::TestSystem::TestException& e) {
            thrown = true;
          }
          if (not thrown)
            OKLIB_THROW("not thrown");
        }
        {
          typedef Empty<value_type> empty_type;
          const empty_type empty;
          OKLIB_TESTTRIVIAL_RETHROW((TestIrreflexivity<range_type,empty_type>(range, empty)));
        }
      }
    };

  }

}

#endif
