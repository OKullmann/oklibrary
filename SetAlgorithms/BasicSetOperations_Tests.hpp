// Oliver Kullmann, 19.5.2005 (Swansea)

#ifndef BASICSETOPERATIONSTESTS_78uBcz2

#define BASICSETOPERATIONSTESTS_78uBcz2

#include <set>
#include <vector>
#include <utility>

#include "TestBaseClass.hpp"
#include "TestExceptions.hpp"

namespace OKlib {

  namespace SetAlgorithms {

    template <template <typename InputIterator_sets, typename InputIterator_elements, typename OutputIterator> class Intersection>
    class Test_Intersection : public ::OKlib::TestSystem::TestBase {
    public :
      typedef Test_Intersection test_type;
      Test_Intersection() {
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
          typedef Intersection<vector_iterator, set_iterator, value_iterator> intersection_type;

          intersection_type intersection;

          VectorSets sets;
          VectorValues output;

          if (sets.size() > output.size())
            output.resize(sets.size());
          const value_iterator end = intersection(sets.begin(), sets.end(), output.begin());
          // XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
        }
      }
    };

  }

}

#endif
