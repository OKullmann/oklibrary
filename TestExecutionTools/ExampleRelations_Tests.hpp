// Oliver Kullmann, 27.12.2005 (Swansea)
/* Copyright 2005 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file TestExecutionTools/ExampleRelations_Tests.hpp
  \brief Tests for the examples for binary predicates.
*/

#ifndef EXAMPLERELATIONSTESTS_kkLoIu
#define EXAMPLERELATIONSTESTS_kkLoIu

#include <vector>

#include <boost/iterator/counting_iterator.hpp>

#include <OKlib/TestSystem/TestBaseClass.hpp>
#include <OKlib/TestSystem/TestExceptions.hpp>

namespace OKlib {

  namespace TestExecutionTools {

    /*!
      \class Test_Id
      \brief Testing the identity relation
      \todo Test also the concept.
      \todo Error reporting: Indices.
    */

    template <template <typename T> class Id>
    class Test_Id : OKlib::TestSystem::TestBase {
    public :
      typedef Test_Id test_type;
      Test_Id() {
        insert(this);
      }
    private :
      void perform_test_trivial() {
        {
          typedef int T;
          typedef Id<T> id_type;
          id_type p;

          typedef std::vector<T> vector_t;
          const T number_elements = 100;
          vector_t test_vector(boost::make_counting_iterator(0), boost::make_counting_iterator(number_elements));
          typedef vector_t::const_iterator iterator;

          typedef iterator T2;
          typedef Id<T2> id2_type;
          id2_type p2;

          const iterator& begin(test_vector.begin());
          const iterator& end(test_vector.end());
          for (iterator i = begin; i != end; ++i)
            for (iterator j = begin; j != end; ++j) {
              OKLIB_TEST_EQUAL(p(*i,*j), p2(i,j));
              OKLIB_TEST_EQUAL(p(*i,*j), i == j);
              OKLIB_TEST_EQUAL(p2(i,j), i == j);
            }
        }
      }
    };

    /*!
      \class Test_Total
      \brief Testing the total relation
      \todo Test also the concept.
      \todo Error reporting: Indices.
    */

    template <template <typename T> class Total>
    class Test_Total : OKlib::TestSystem::TestBase {
    public :
      typedef Test_Total test_type;
      Test_Total() {
        insert(this);
      }
    private :
      void perform_test_trivial() {
        {
          typedef int T;
          typedef Total<T> total_type;
          total_type p;

          typedef std::vector<T> vector_t;
          const T number_elements = 100;
          typedef boost::counting_iterator<T> iterator;
          const iterator& begin(0);
          const iterator& end(number_elements);
          for (iterator i = begin; i != end; ++i)
            for (iterator j = begin; j != end; ++j)
              OKLIB_TEST_EQUAL(p(*i,*j), true);
        }
      }
    };

    /*!
      \class Test_Empty
      \brief Testing the empty relation
      \todo Test also the concept.
      \todo Error reporting: Indices.
    */

    template <template <typename T> class Empty>
    class Test_Empty : OKlib::TestSystem::TestBase {
    public :
      typedef Test_Empty test_type;
      Test_Empty() {
        insert(this);
      }
    private :
      void perform_test_trivial() {
        {
          typedef int T;
          typedef Empty<T> empty_type;
          empty_type p;

          typedef std::vector<T> vector_t;
          const T number_elements = 100;
          typedef boost::counting_iterator<T> iterator;
          const iterator& begin(0);
          const iterator& end(number_elements);
          for (iterator i = begin; i != end; ++i)
            for (iterator j = begin; j != end; ++j)
              OKLIB_TEST_EQUAL(p(*i,*j), false);
        }
      }
    };

  }

}

#endif
