// Oliver Kullmann, 19.5.2005 (Swansea)
/* Copyright 2005 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#ifndef BASICSETOPERATIONSTESTS_78uBcz2
#define BASICSETOPERATIONSTESTS_78uBcz2

#include <set>
#include <vector>
#include <utility>
#include <algorithm>
#include <sstream>
#include <iterator>
#include <cstddef>
#include <string>

#include <boost/lexical_cast.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/iterator/counting_iterator.hpp>
#include <boost/range/iterator_range.hpp>

#include <OKlib/TestSystem/TestBaseClass.hpp>
#include <OKlib/TestSystem/TestExceptions.hpp>

namespace OKlib {
  namespace SetAlgorithms {

    template <class SetCollection, class ReferenceSet, template <class OutputIterator> class Operation>
    class TestOperation : public ::OKlib::TestSystem::Test {
      // ToDo: Conceptualisation
      SetCollection& sets;
      ReferenceSet& reference;
    public :
      typedef TestOperation test_type;
      TestOperation(SetCollection& sets, ReferenceSet& reference) : sets(sets), reference(reference) {}
    private :
      void perform_test_trivial() {
        typedef typename ReferenceSet::value_type value_type;
        typedef std::vector<value_type> Vector;
        typedef typename Vector::iterator value_iterator;
        Vector output;
        const typename ReferenceSet::size_type reference_size = reference.size();
        output.reserve(reference_size);
        typedef std::back_insert_iterator<Vector> back_iterator;
        const Operation<back_iterator> op;
        op(sets.begin(), sets.end(), back_iterator(output));
        const value_iterator begin = output.begin();
        const value_iterator end = output.end();
        const typename Vector::size_type size = end - begin;
        if (size != reference_size) {
                std::stringstream message;
                message << "Size is " << size << ", and not " << reference_size << "; the computed sequence is ";
                std::copy(begin, end, std::ostream_iterator<value_type>(message, ","));
                message << ", and the reference sequence is ";
                std::copy(reference.begin(), reference.end(), std::ostream_iterator<value_type>(message, ","));
                OKLIB_THROW(message.str());
              }
              if (not std::equal(begin, end, reference.begin())) {
                std::stringstream message;
                message << "Computed sequence is ";
                std::copy(begin, end, std::ostream_iterator<value_type>(message, ","));
                message << ", and not ";
                std::copy(reference.begin(), reference.end(), std::ostream_iterator<value_type>(message, ","));
                OKLIB_THROW(message.str());
              }
      }
    };

    // -------------------------------------------------------------------------------------------------------------------------------
    // -------------------------------------------------------------------------------------------------------------------------------

    template <template <typename InputIterator_sets, typename OutputIterator> class Union>
    class Test_Union : public ::OKlib::TestSystem::TestBase {
    public :
      typedef Test_Union test_type;
      Test_Union() {
        insert(this);
      }
    private :

      typedef int value_type;
      typedef std::set<value_type> Set;
      typedef Set::iterator set_iterator;
      typedef std::pair<set_iterator, set_iterator> Range;
      typedef std::vector<Range> VectorSets;
      typedef VectorSets::iterator vector_iterator;
      typedef std::vector<value_type> VectorValues;
      typedef VectorValues::iterator value_iterator;
      template <class OutputIt> struct union_type : Union<vector_iterator, OutputIt> {};
 
      void perform_test_trivial() {
        {
          VectorSets sets;
          Set reference;
          TestOperation<VectorSets, Set, union_type> check(sets, reference);

          typedef boost::tuple<Set, Set, Set> SetTriple;
          typedef std::vector<SetTriple> VectorTestCases;
          VectorTestCases test_cases;
          const size_t size = sizeof(value_type);
          {
            const value_type array1[] = {1};
            const value_type array2[] = {2};
            const value_type array3[] = {3};
            test_cases.push_back(SetTriple(Set(array1, array1 + (sizeof(array1) / size)), Set(array2, array2 + (sizeof(array2) / size)),  Set(array3, array3 + (sizeof(array3) / size))));
          }
          {
            const value_type array1[] = {1};
            const value_type array2[] = {1};
            const value_type array3[] = {1};
            test_cases.push_back(SetTriple(Set(array1, array1 + (sizeof(array1) / size)), Set(array2, array2 + (sizeof(array2) / size)),  Set(array3, array3 + (sizeof(array3) / size))));
          }
          {
            const value_type array1[] = {1,2};
            const value_type array2[] = {1,2};
            const value_type array3[] = {1,2};
            test_cases.push_back(SetTriple(Set(array1, array1 + (sizeof(array1) / size)), Set(array2, array2 + (sizeof(array2) / size)),  Set(array3, array3 + (sizeof(array3) / size))));
          }
          {
            const value_type array1[] = {1,2};
            const value_type array2[] = {1,3};
            const value_type array3[] = {2,3};
            test_cases.push_back(SetTriple(Set(array1, array1 + (sizeof(array1) / size)), Set(array2, array2 + (sizeof(array2) / size)),  Set(array3, array3 + (sizeof(array3) / size))));
          }
          {
            const value_type array1[] = {2,4,6,10};
            const value_type array2[] = {2,-4,6,10};
            const value_type array3[] = {2,3,-4,5};
            test_cases.push_back(SetTriple(Set(array1, array1 + (sizeof(array1) / size)), Set(array2, array2 + (sizeof(array2) / size)),  Set(array3, array3 + (sizeof(array3) / size))));
          }
          {
            const value_type array2[] = {2,-4,6,10};
            const value_type array3[] = {2,3,-4,5};
            test_cases.push_back(SetTriple(Set(), Set(array2, array2 + (sizeof(array2) / size)),  Set(array3, array3 + (sizeof(array3) / size))));
          }
          {
            const value_type array1[] = {1,2,3,4,5};
            const value_type array2[] = {-1,0,1,2};
            const value_type array3[] = {0,1,2,3,4,5,6};
            test_cases.push_back(SetTriple(Set(array1, array1 + (sizeof(array1) / size)), Set(array2, array2 + (sizeof(array2) / size)),  Set(array3, array3 + (sizeof(array3) / size))));
          }
          {
            const value_type array1[] = {1,2,3,4,5};
            const value_type array2[] = {1};
            const value_type array3[] = {2};
            test_cases.push_back(SetTriple(Set(array1, array1 + (sizeof(array1) / size)), Set(array2, array2 + (sizeof(array2) / size)),  Set(array3, array3 + (sizeof(array3) / size))));
          }
          {
            const value_type array1[] = {1,2,3,4,5};
            const value_type array2[] = {1};
            const value_type array3[] = {2,6};
            test_cases.push_back(SetTriple(Set(array1, array1 + (sizeof(array1) / size)), Set(array2, array2 + (sizeof(array2) / size)),  Set(array3, array3 + (sizeof(array3) / size))));
          }

          for (VectorTestCases::iterator i = test_cases.begin(); i != test_cases.end(); ++i, sets.clear()) {
            const Set s1(i -> get<0>());
            sets.push_back(Range(s1.begin(), s1.end()));
            assert(sets.size() == 1);
            reference = s1;
            OKLIB_TESTTRIVIAL_RETHROW(check);
            const Set s2(i -> get<1>());
            sets.push_back(Range(s2.begin(), s2.end()));
            assert(sets.size() == 2);
            reference.clear();
            std::set_union(s1.begin(), s1.end(), s2.begin(), s2.end(), std::inserter(reference, reference.begin()));
            OKLIB_TESTTRIVIAL_RETHROW(check);
            sets.push_back(Range(s2.begin(), s2.end()));
            assert(sets.size() == 3);
            OKLIB_TESTTRIVIAL_RETHROW(check);
            const Set s3(i -> get<2>());
            sets.push_back(Range(s3.begin(), s3.end()));
            assert(sets.size() == 4);
            Set temp;
            std::set_union(reference.begin(), reference.end(), s3.begin(), s3.end(), std::inserter(temp, temp.begin()));
            temp.swap(reference);
            OKLIB_TESTTRIVIAL_RETHROW(check);
          }
        }
        {
          typedef int integer_type;
          typedef boost::counting_iterator<integer_type> counting_iterator;
          typedef boost::iterator_range<counting_iterator> iterator_range;
          typedef std::vector<iterator_range> vector_type;
          vector_type test_vector;
          const integer_type step = 10;
          const integer_type count = 100;
          const integer_type max_add = 5;
          for (integer_type a = 0; a != max_add; ++a) {
            for (integer_type i = 0, left = 0, right = step; i < count; ++i, left = right, right += step)
              test_vector.push_back(iterator_range(left, right+a));
            test_vector.push_back(iterator_range(0,0));
            typedef std::vector<integer_type> result_vector_type;
            result_vector_type result;
            union_sets(test_vector.begin(), test_vector.end(), std::back_inserter(result));
            OKLIB_TEST_EQUAL_RANGES(result, iterator_range(0, step * count + a));
          }
        }
        {
          typedef std::string value_type;
          typedef std::set<value_type> set_type;
          typedef std::vector<set_type> vector_type;
          typedef vector_type::const_iterator vector_iterator;
          typedef std::vector<value_type> result_type;
          set_type s1; s1.insert("abc"); s1.insert("xy"); s1.insert("uv");
          set_type s2; s2.insert("ab"); s2.insert("abc"); s2.insert("xy"); s2.insert("klm");
          set_type s3; s3.insert("abc"); s3.insert("xy"); s3.insert("h");
          vector_type v; v.reserve(3);
          v.push_back(s1); v.push_back(s2); v.push_back(s3);
          result_type ref;
          ref.push_back("ab"); ref.push_back("abc"); ref.push_back("h"); ref.push_back("klm"); ref.push_back("uv"); ref.push_back("xy");
          result_type res;
          Union<vector_iterator, std::back_insert_iterator<result_type> > U;
          U(v.begin(), v.end(), std::back_inserter(res));
          OKLIB_TEST_EQUAL_W(ref, res);
        }
      }
    };

    // -------------------------------------------------------------------------------------------------------------------------------

    template <template <typename InputIterator_sets, typename OutputIterator> class Intersection>
    class Test_Intersection : public ::OKlib::TestSystem::TestBase {
    public :
      typedef Test_Intersection test_type;
      Test_Intersection() {
        insert(this);
      }
    private :

      typedef int value_type;
      typedef std::set<value_type> Set;
      typedef Set::iterator set_iterator;
      typedef std::pair<set_iterator, set_iterator> Range;
      typedef std::vector<Range> VectorSets;
      typedef VectorSets::iterator vector_iterator;
      typedef std::vector<value_type> VectorValues;
      typedef VectorValues::iterator value_iterator;
      template <class OutputIt> struct intersection_type : Intersection<vector_iterator, OutputIt> {};
 
      void perform_test_trivial() {
        {
          VectorSets sets;
          Set reference;
          TestOperation<VectorSets, Set, intersection_type> check(sets, reference);

          typedef boost::tuple<Set, Set, Set> SetTriple;
          typedef std::vector<SetTriple> VectorTestCases;
          VectorTestCases test_cases;
          const size_t size = sizeof(value_type);
          {
            const value_type array1[] = {1};
            const value_type array2[] = {1};
            const value_type array3[] = {1};
            test_cases.push_back(SetTriple(Set(array1, array1 + (sizeof(array1) / size)), Set(array2, array2 + (sizeof(array2) / size)),  Set(array3, array3 + (sizeof(array3) / size))));
          }
          {
            const value_type array1[] = {1,2};
            const value_type array2[] = {1,2};
            const value_type array3[] = {1,2};
            test_cases.push_back(SetTriple(Set(array1, array1 + (sizeof(array1) / size)), Set(array2, array2 + (sizeof(array2) / size)),  Set(array3, array3 + (sizeof(array3) / size))));
          }
          {
            const value_type array1[] = {1,2};
            const value_type array2[] = {1,3};
            const value_type array3[] = {2,3};
            test_cases.push_back(SetTriple(Set(array1, array1 + (sizeof(array1) / size)), Set(array2, array2 + (sizeof(array2) / size)),  Set(array3, array3 + (sizeof(array3) / size))));
          }
          {
            const value_type array1[] = {2,4,6,10};
            const value_type array2[] = {2,-4,6,10};
            const value_type array3[] = {2,3,-4,5};
            test_cases.push_back(SetTriple(Set(array1, array1 + (sizeof(array1) / size)), Set(array2, array2 + (sizeof(array2) / size)),  Set(array3, array3 + (sizeof(array3) / size))));
          }
          {
            const value_type array2[] = {2,-4,6,10};
            const value_type array3[] = {2,3,-4,5};
            test_cases.push_back(SetTriple(Set(), Set(array2, array2 + (sizeof(array2) / size)),  Set(array3, array3 + (sizeof(array3) / size))));
          }
          {
            const value_type array1[] = {1,2,3,4,5};
            const value_type array2[] = {-1,0,1,2};
            const value_type array3[] = {0,1,2,3,4,5,6};
            test_cases.push_back(SetTriple(Set(array1, array1 + (sizeof(array1) / size)), Set(array2, array2 + (sizeof(array2) / size)),  Set(array3, array3 + (sizeof(array3) / size))));
          }
           
          for (VectorTestCases::iterator i = test_cases.begin(); i != test_cases.end(); ++i, sets.clear()) {
            const Set s1(i -> get<0>());
            sets.push_back(Range(s1.begin(), s1.end()));
            assert(sets.size() == 1);
            reference = s1;
            OKLIB_TESTTRIVIAL_RETHROW(check);
            const Set s2(i -> get<1>());
            sets.push_back(Range(s2.begin(), s2.end()));
            assert(sets.size() == 2);
            reference.clear();
            std::set_intersection(s1.begin(), s1.end(), s2.begin(), s2.end(), std::inserter(reference, reference.begin()));
            OKLIB_TESTTRIVIAL_RETHROW(check);
            sets.push_back(Range(s2.begin(), s2.end()));
            assert(sets.size() == 3);
            OKLIB_TESTTRIVIAL_RETHROW(check);
            const Set s3(i -> get<2>());
            sets.push_back(Range(s3.begin(), s3.end()));
            assert(sets.size() == 4);
            Set temp;
            std::set_intersection(reference.begin(), reference.end(), s3.begin(), s3.end(), std::inserter(temp, temp.begin()));
            temp.swap(reference);
            OKLIB_TESTTRIVIAL_RETHROW(check);
          }
        }
        {
          typedef int integer_type;
          typedef boost::counting_iterator<integer_type> counting_iterator;
          typedef boost::iterator_range<counting_iterator> iterator_range;
          typedef std::vector<iterator_range> vector_type;
          vector_type test_vector;
          const integer_type count = 100;
          const integer_type first_left = 0;
          const integer_type first_right = 20;
          for (integer_type i = 0, left = first_left, right = first_right; i < count; ++i, ++right, --left)
              test_vector.push_back(iterator_range(left, right));
          test_vector.push_back(iterator_range(first_left-10, first_right+10));
          typedef std::vector<integer_type> result_vector_type;
          result_vector_type result;
          intersection_sets(test_vector.begin(), test_vector.end(), std::back_inserter(result));
          OKLIB_TEST_EQUAL_RANGES(result, iterator_range(first_left, first_right));
        }
        {
          typedef std::string value_type;
          typedef std::set<value_type> set_type;
          typedef std::vector<set_type> vector_type;
          typedef vector_type::const_iterator vector_iterator;
          typedef std::vector<value_type> result_type;
          set_type s1; s1.insert("abc"); s1.insert("xy"); s1.insert("uv");
          set_type s2; s2.insert("ab"); s2.insert("abc"); s2.insert("xy"); s2.insert("klm");
          set_type s3; s3.insert("abc"); s3.insert("xy"); s3.insert("h");
          vector_type v; v.reserve(3);
          v.push_back(s1); v.push_back(s2); v.push_back(s3);
          result_type ref;
          ref.push_back("abc"); ref.push_back("xy");
          result_type res;
          Intersection<vector_iterator, std::back_insert_iterator<result_type> > I;
          I(v.begin(), v.end(), std::back_inserter(res));
          OKLIB_TEST_EQUAL_W(ref, res);
        }
      }
    };

  }

}

#endif
