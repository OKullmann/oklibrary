// Oliver Kullmann, 19.5.2005 (Swansea)

#ifndef BASICSETOPERATIONSTESTS_78uBcz2

#define BASICSETOPERATIONSTESTS_78uBcz2

#include <set>
#include <vector>
#include <utility>

#include <boost/lexical_cast.hpp>

#include "TestBaseClass.hpp"
#include "TestExceptions.hpp"

namespace OKlib {

  namespace SetAlgorithms {

    template <template <typename InputIterator_sets, typename InputIterator_elements, typename OutputIterator> class Union>
    class Test_Union : public ::OKlib::TestSystem::TestBase {
    public :
      typedef Test_Union test_type;
      Test_Union() {
        insert(this);
      }
    private :

      void perform_test_trivial() {
        {
          typedef int value_type;
          typedef std::set<value_type> Set;
          typedef Set::iterator set_iterator;
          typedef std::pair<set_iterator, set_iterator> Range;
          typedef std::vector<Range> VectorSets;
          typedef VectorSets::iterator vector_iterator;
          typedef std::vector<value_type> VectorValues;
          typedef VectorValues::iterator value_iterator;
          typedef Union<vector_iterator, set_iterator, value_iterator> union_type;

          union_type union_f;

          struct check_union : public ::OKlib::TestSystem::TestBase {
            typedef check_union test_type;
            VectorSets& sets;
            VectorValues& output;
            Set& reference;
            union_type union_f;
            check_union(VectorSets& sets, VectorValues& output, Set& reference, union_type union_f) : sets(sets), output(output), reference(reference), union_f(union_f) {}
            void perform_test_trivial() {
              if (sets.size() > output.size()) output.resize(sets.size());
              const value_iterator end = union_f(sets.begin(), sets.end(), output.begin());
              const VectorValues::size_type size = end - output.begin();
              if (size != reference.size())
                throw ::OKlib::TestSystem::TestException("Size is " + boost::lexical_cast<std::string>(size) + ", and not " + boost::lexical_cast<std::string>(reference.size())).add(OKLIB_TESTDESCRIPTION);
            }
          };

          VectorSets sets;
          VectorValues output;
          Set reference;

          check_union check(sets, output, reference, union_f);            

          OKLIB_TESTTRIVIAL_RETHROW(check);

          Set s1; s1.insert(2); s1.insert(4); s1.insert(6);
          sets.push_back(Range(s1.begin(), s1.end()));
          

          Set s2; s2.insert(2); s2.insert(-4); s2.insert(6);
          Set s3; s3.insert(3); s3.insert(-4); s3.insert(5);
           sets.push_back(Range(s2.begin(), s2.end())); sets.push_back(Range(s3.begin(), s3.end()));

        }
      }
    };

  }

}

#endif
