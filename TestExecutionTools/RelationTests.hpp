// Oliver Kullmann, 27.12.2005 (Swansea)

/*!
  \file RelationTests.hpp
  \brief Tests for relations, like symmetry and transitivity.
*/

#ifndef RELATIONTESTS_zmzmznVb6

#define RELATIONTESTS_zmzmznVb6

#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <boost/range/const_iterator.hpp>

#include "TestBaseClass.hpp"
#include "TestExceptions.hpp"

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


  }

}

#endif
