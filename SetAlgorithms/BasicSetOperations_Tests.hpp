// Oliver Kullmann, 19.5.2005 (Swansea)

#ifndef BASICSETOPERATIONSTESTS_78uBcz2

#define BASICSETOPERATIONSTESTS_78uBcz2

#include <set>
#include <vector>
#include <utility>
#include <algorithm>
#include <sstream>
#include <iterator>

#include <boost/lexical_cast.hpp>

#include "TestBaseClass.hpp"
#include "TestExceptions.hpp"

namespace OKlib {

  namespace SetAlgorithms {

    template <class SetCollection, class ReferenceSet, template <class OutputIterator> class Operation>
    class TestOperation : public ::OKlib::TestSystem::TestBase {
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
                throw ::OKlib::TestSystem::TestException(message.str()).add(OKLIB_TESTDESCRIPTION);
              }
              if (not std::equal(begin, end, reference.begin())) {
                std::stringstream message;
                message << "Computed sequence is ";
                std::copy(begin, end, std::ostream_iterator<value_type>(message, ","));
                message << ", and not ";
                std::copy(reference.begin(), reference.end(), std::ostream_iterator<value_type>(message, ","));
                throw ::OKlib::TestSystem::TestException(message.str()).add(OKLIB_TESTDESCRIPTION);
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
          Set s1; s1.insert(2); s1.insert(4); s1.insert(6);
          sets.push_back(Range(s1.begin(), s1.end()));
          reference = s1;
          OKLIB_TESTTRIVIAL_RETHROW(check);
          Set s2; s2.insert(2); s2.insert(-4); s2.insert(6);
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

          Set s1; s1.insert(2); s1.insert(4); s1.insert(6);
          sets.push_back(Range(s1.begin(), s1.end()));
          reference = s1;
          OKLIB_TESTTRIVIAL_RETHROW(check);
          Set s2; s2.insert(2); s2.insert(-4); s2.insert(6);
          sets.push_back(Range(s2.begin(), s2.end()));
          reference.clear();
          std::set_intersection(s1.begin(), s1.end(), s2.begin(), s2.end(), std::inserter(reference, reference.begin()));
          OKLIB_TESTTRIVIAL_RETHROW(check);
          sets.push_back(Range(s2.begin(), s2.begin()));
          Set temp;;
          reference.swap(temp);
          OKLIB_TESTTRIVIAL_RETHROW(check);
          reference.swap(temp);
          Set s3; s3.insert(3); s3.insert(-4); s3.insert(5);
          sets.push_back(Range(s3.begin(), s3.end()));
          std::set_intersection(reference.begin(), reference.end(), s3.begin(), s3.end(), std::inserter(temp, temp.begin()));
          temp.swap(reference);
          OKLIB_TESTTRIVIAL_RETHROW(check);
        }
      }
    };

  }

}

#endif
