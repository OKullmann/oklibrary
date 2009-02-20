// Oliver Kullmann, 24.12.2005 (Swansea)
/* Copyright 2005 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file TestExecutionTools/TestAlgorithms.hpp
  \brief Algorithms for performing (systematic) tests.
*/

#ifndef TESTALGORITHMS_uuTTr4
#define TESTALGORITHMS_uuTTr4

#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <boost/range/const_iterator.hpp>
#include <boost/utility.hpp>

#include <OKlib/TestSystem/TestBaseClass.hpp>
#include <OKlib/TestSystem/TestExceptions.hpp>

namespace OKlib {

  namespace TestExecutionTools {

    /*!
      \class EnumerationBase
      \brief Helper base class for enumerating test cases
    */

    template <int Dim, class InputRange, class Functor>
    struct EnumerationBase : OKlib::TestSystem::Test {
      typedef InputRange input_range_type;
      typedef Functor functor_type;
      enum { dim = Dim };
      typedef typename boost::range_const_iterator<input_range_type>::type iterator;
      EnumerationBase(const input_range_type& r_) : r(r_), begin(boost::begin(r)), end(boost::end(r)) {}
    protected :
      const input_range_type& r;
      const iterator begin;
      const iterator end;
    };

    /*!
      \class AllCombinations
      \brief Test function, depending on the dimension dim, which takes a range of objects and
      a test function F, and performs all tests F(x_1, ..., x_dim), where {x_1, ..., x_dim}
      runs through all dim-element subsets of the set of objects given by the range.
    */
    
    template <int Dim, class InputRange, class Functor>
    struct AllCombinations : EnumerationBase<Dim, InputRange, Functor> {};

    template <class InputRange, class Functor>
    struct AllCombinations<1, InputRange, Functor> : EnumerationBase<1, InputRange, Functor> {
      typedef EnumerationBase<1, InputRange, Functor> Base;
      typedef AllCombinations test_type;
      AllCombinations(const typename Base::input_range_type& r) : Base(r) {}
    private :
      void perform_test_trivial() {
        for (typename Base::iterator i(this -> begin); i != this -> end; ++i)
          OKLIB_TESTTRIVIAL_RETHROW(typename Base::functor_type(*i));
      }
    };

    template <class InputRange, class Functor>
    struct AllCombinations<2, InputRange, Functor> : EnumerationBase<2, InputRange, Functor> {
      typedef EnumerationBase<2, InputRange, Functor> Base;
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
    struct AllCombinations<3, InputRange, Functor> : EnumerationBase<3, InputRange, Functor> {
      typedef EnumerationBase<3, InputRange, Functor> Base;
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

    /*!
      \class AllVariations
      \brief Test function, depending on the dimension dim, which takes a range of objects and
      a test function F, and performs all tests F(x_1, ..., x_dim), where (x_1, ..., x_dim)
      runs through all possibilities.
    */
    
    template <int Dim, class InputRange, class Functor>
    struct AllVariations : EnumerationBase<Dim, InputRange, Functor> {};

    template <class InputRange, class Functor>
    struct AllVariations<1, InputRange, Functor> : EnumerationBase<1, InputRange, Functor> {
      typedef EnumerationBase<1, InputRange, Functor> Base;
      typedef AllVariations test_type;
      AllVariations(const typename Base::input_range_type& r) : Base(r) {}
    private :
      void perform_test_trivial() {
        for (typename Base::iterator i(this -> begin); i != this -> end; ++i)
          OKLIB_TESTTRIVIAL_RETHROW(typename Base::functor_type(*i));
      }
    };

    template <class InputRange, class Functor>
    struct AllVariations<2, InputRange, Functor> : EnumerationBase<2, InputRange, Functor> {
      typedef EnumerationBase<2, InputRange, Functor> Base;
      typedef AllVariations test_type;
      AllVariations(const typename Base::input_range_type& r) : Base(r) {}
    private :
      void perform_test_trivial() {
        for (typename Base::iterator i(this -> begin); i != this -> end; ++i)
          for (typename Base::iterator j(this -> begin); j != this -> end; ++j)
            OKLIB_TESTTRIVIAL_RETHROW((typename Base::functor_type(*i, *j)));
      }
    };

    template <class InputRange, class Functor>
    struct AllVariations<3, InputRange, Functor> : EnumerationBase<3, InputRange, Functor> {
      typedef EnumerationBase<3, InputRange, Functor> Base;
      typedef AllVariations test_type;
      AllVariations(const typename Base::input_range_type& r) : Base(r) {}
    private :
      void perform_test_trivial() {
        for (typename Base::iterator i(this -> begin); i != this -> end; ++i)
          for (typename Base::iterator j(this -> begin); j != this -> end; ++j)
            for (typename Base::iterator k(this -> begin); k != this -> end; ++k)
              OKLIB_TESTTRIVIAL_RETHROW((typename Base::functor_type(*i, *j, *k)));
      }
    };

  }

}

#endif
