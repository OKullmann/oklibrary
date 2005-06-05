// Oliver Kullmann, 19.5.2005 (Swansea)

#ifndef BASICSETOPERATIONSTESTS_78uBcz2

#define BASICSETOPERATIONSTESTS_78uBcz2

#include <set>
#include <vector>
#include <utility>
#include <algorithm>
#include <sstream>
#include <iterator>
#include <cstddef>

#include <boost/lexical_cast.hpp>
#include <boost/tuple/tuple.hpp>

#include "TestBaseClass.hpp"
#include "TestExceptions.hpp"

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

          OKLIB_TESTTRIVIAL_RETHROW(check);
          Set s1; s1.insert(2); s1.insert(4); s1.insert(6); s1.insert(10);
          sets.push_back(Range(s1.begin(), s1.end()));
          reference = s1;
          OKLIB_TESTTRIVIAL_RETHROW(check);
          Set s2; s2.insert(2); s2.insert(-4); s2.insert(6); s2.insert(10);
          sets.push_back(Range(s2.begin(), s2.end()));
          reference.clear();
          std::set_union(s1.begin(), s1.end(), s2.begin(), s2.end(), std::inserter(reference, reference.begin()));
          OKLIB_TESTTRIVIAL_RETHROW(check);
          sets.push_back(Range(s2.begin(), s2.begin()));
          OKLIB_TESTTRIVIAL_RETHROW(check);
          Set s3; s3.insert(3); s3.insert(-4); s3.insert(5);
          sets.push_back(Range(s3.begin(), s3.end()));
          Set temp;
          std::set_union(reference.begin(), reference.end(), s3.begin(), s3.end(), std::inserter(temp, temp.begin()));
          temp.swap(reference);
          OKLIB_TESTTRIVIAL_RETHROW(check);
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
      }

    };

  }

}

#endif
