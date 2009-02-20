// Oliver Kullmann, 26.12.2005 (Swansea)
/* Copyright 2005 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Programming/Iterators/ImplicitConversionsBoost_Tests.hpp
  \brief Tests for the extensions of boost iterators by implicit conversions
*/

#ifndef IMPLICITCONVERSIONSBOOSTTESTS_jnnznml5
#define IMPLICITCONVERSIONSBOOSTTESTS_jnnznml5

#include <vector>
#include <numeric>

#include <OKlib/TestSystem/TestBaseClass.hpp>
#include <OKlib/TestSystem/TestExceptions.hpp>

namespace OKlib {
  namespace Iterators {

    /*!
      \class Test_counting_iterator
      \brief Testing the extended counting iterator.

      \todo Yet the tests are very sporadic:
       - Use the generic test facilities to check whether counting_iterator is
         actually a forward iterator etc.
       - Test whether all specific functionality of boost::counting_iterator
         has been transferred.
    */

     template <template <class Incrementable, class CategoryOrTraversal = boost::use_default, class Difference = boost::use_default> class counting_iterator>
     class Test_counting_iterator : public ::OKlib::TestSystem::TestBase {
    public :
      typedef Test_counting_iterator test_type;
      Test_counting_iterator () {
        insert(this);
      }
    private :
       void perform_test_trivial() {
         {
           typedef int int_type;
           typedef counting_iterator<int_type> counting_iterator_type;
           const int max = 100;
           std::vector<int_type> fill_vec(counting_iterator_type(0), counting_iterator_type(max));
           OKLIB_TEST_EQUAL(std::accumulate(fill_vec.begin(), fill_vec.end(), 0), ((max - 1) * max) / 2);
           typedef std::vector<counting_iterator_type> vector_iterators_type;
           vector_iterators_type vec(fill_vec.begin(), fill_vec.end());
           {
             typedef boost::counting_iterator<int_type> counting_iterator_type;
             typedef std::vector<counting_iterator_type> vector_iterators_type;
             vector_iterators_type vec2(fill_vec.begin(), fill_vec.end());
             OKLIB_TEST_EQUAL_RANGES(vec, vec2);
           }
           {
             counting_iterator_type i(77);
             i = 88;
             if (not (i == counting_iterator_type(88)))
               OKLIB_THROW("i != counting_iterator_type(88)");
           }
           {
             counting_iterator_type i;
           }
         }
       }
     };

  }

}

#endif
