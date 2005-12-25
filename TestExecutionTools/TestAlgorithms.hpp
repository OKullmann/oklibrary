// Oliver Kullmann, 24.12.2005 (Swansea)

/*!
  \file TestAlgorithms.hpp
  \brief Algorithms for performing (systematic) tests.
*/

#ifndef TESTALGORITHMS_uuTTr4

#define TESTALGORITHMS_uuTTr4

#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <boost/range/const_iterator.hpp>
#include <boost/utility.hpp>

namespace OKlib {

  namespace TestExecutionTools {

    /*!
      \class AllCombinations
      \brief Test function, depending on the dimension dim, which takes a range of objects and
      a test function F, and performs all tests F(x_1, ..., x_dim), where {x_1, ..., x_dim}
      runs through all dim-element subsets of the set of objects given by the range.
      \todo Test it!
    */

    
    template <int Dim, class InputRange, class Functor>
    struct AllCombinationsBase : OKlib::TestSystem::Test {
      typedef InputRange input_range_type;
      typedef Functor functor_type;
      enum { dim = Dim };
      typedef typename boost::range_const_iterator<input_range_type>::type iterator;
      AllCombinationsBase(const input_range_type& r_) : r(r_), begin(boost::begin(r)), end(boost::end(r)) {}
    protected :
      const input_range_type& r;
      const iterator begin;
      const iterator end;
    };

    template <int Dim, class InputRange, class Functor>
    struct AllCombinations : AllCombinationsBase<Dim, InputRange, Functor> {};

    template <class InputRange, class Functor>
    struct AllCombinations<1, InputRange, Functor> : AllCombinationsBase<1, InputRange, Functor> {
      typedef AllCombinationsBase<1, InputRange, Functor> Base;
      typedef AllCombinations test_type;
      AllCombinations(const typename Base::input_range_type& r) : Base(r) {}
    private :
      void perform_test_trivial() {
        for (typename Base::iterator i(this -> begin); i != this -> end; ++i)
          OKLIB_TESTTRIVIAL_RETHROW(typename Base::functor_type(*i));
      }
    };

    template <class InputRange, class Functor>
    struct AllCombinations<2, InputRange, Functor> : AllCombinationsBase<2, InputRange, Functor> {
      typedef AllCombinationsBase<2, InputRange, Functor> Base;
      typedef AllCombinations test_type;
      AllCombinations(const typename Base::input_range_type& r) : Base(r) {}
    private :
      void perform_test_trivial() {
        for (typename Base::iterator i(this -> begin); i != this -> end; ++i)
          for (typename Base::iterator j(boost::next(i)); j != this -> end; ++j)
            OKLIB_TESTTRIVIAL_RETHROW((typename Base::functor_type(*i, *j)));
      }
    };

    template <class InputRange, class Functor>
    struct AllCombinations<3, InputRange, Functor> : AllCombinationsBase<3, InputRange, Functor> {
      typedef AllCombinationsBase<3, InputRange, Functor> Base;
      typedef AllCombinations test_type;
      AllCombinations(const typename Base::input_range_type& r) : Base(r) {}
    private :
      void perform_test_trivial() {
        for (typename Base::iterator i(this -> begin); i != this -> end; ++i)
          for (typename Base::iterator j(boost::next(i)); j != this -> end; ++j)
            for (typename Base::iterator k(boost::next(j)); k != this -> end; ++k)
              OKLIB_TESTTRIVIAL_RETHROW((typename Base::functor_type(*i, *j, *k)));
      }
    };

  }

}

#endif
