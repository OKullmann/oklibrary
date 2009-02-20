// Oliver Kullmann, 24.12.2005 (Swansea)
/* Copyright 2005 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file TestExecutionTools/TestAlgorithms_Tests.hpp
  \brief Tests for the algorithms for performing (systematic) tests.
*/

#ifndef TESTALGORITHMSTESTS_887766tg
#define TESTALGORITHMSTESTS_887766tg

#include <boost/iterator/counting_iterator.hpp>
#include <boost/range/iterator_range.hpp>

#include <OKlib/Programming/MetaProgramming/Numerical.hpp>

#include <OKlib/TestSystem/TestBaseClass.hpp>
#include <OKlib/TestSystem/TestExceptions.hpp>

#include <OKlib/TestExecutionTools/SecondOrderTestTools.hpp>

namespace OKlib {
  namespace TestExecutionTools {

    /*!
      \class Test_AllCombinations
      \brief Tests a facility AllCombinations by executing it and counting the number of sub-tests
      executed.
    */

    template <template <int Dim, class InputRange, class Functor> class AllCombinations>
    class Test_AllCombinations : public ::OKlib::TestSystem::TestBase {
    public :
      typedef Test_AllCombinations test_type;
      Test_AllCombinations() {
        insert(this);
      }
    private :

      typedef TestCounter<Test_AllCombinations, long> test_counter_type;
      typedef typename test_counter_type::int_type int_type;
      typedef boost::counting_iterator<int_type> counting_iterator_type;
      typedef boost::iterator_range<counting_iterator_type> range_type;

      void perform_test_trivial() {
        test_fixed_dimension<1>();
        test_fixed_dimension<2>();
        test_fixed_dimension<3>();
      }

      template <int Dim>
      void test_fixed_dimension() {
        typedef AllCombinations<Dim, range_type, test_counter_type> all_combinations_type;
        const int_type number_iterations = 100;
        test_counter_type::counter = 0;
        OKLIB_TESTTRIVIAL_RETHROW((all_combinations_type(range_type(int_type(0), number_iterations))));
        OKLIB_TEST_EQUAL(test_counter_type::counter, (MetaProgramming::Binomial<number_iterations, Dim>::value));
      }
    };

    /*!
      \class Test_AllVariations
      \brief Tests a facility AllVariations by executing it and counting the number of sub-tests
      executed.
    */

    template <template <int Dim, class InputRange, class Functor> class AllVariations>
    class Test_AllVariations : public ::OKlib::TestSystem::TestBase {
    public :
      typedef Test_AllVariations test_type;
      Test_AllVariations() {
        insert(this);
      }
    private :

      typedef TestCounter<Test_AllVariations, long> test_counter_type;
      typedef typename test_counter_type::int_type int_type;
      typedef boost::counting_iterator<int_type> counting_iterator_type;
      typedef boost::iterator_range<counting_iterator_type> range_type;

      void perform_test_trivial() {
        test_fixed_dimension<1>();
        test_fixed_dimension<2>();
        test_fixed_dimension<3>();
      }

      template <int Dim>
      void test_fixed_dimension() {
        typedef AllVariations<Dim, range_type, test_counter_type> all_variations_type;
        const int_type number_iterations = 100;
        test_counter_type::counter = 0;
        OKLIB_TESTTRIVIAL_RETHROW((all_variations_type(range_type(int_type(0), number_iterations))));
        OKLIB_TEST_EQUAL(test_counter_type::counter, (MetaProgramming::Power<number_iterations, Dim>::value));
      }
    };


  }

}

#endif
