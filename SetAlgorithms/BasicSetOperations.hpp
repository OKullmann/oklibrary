// Oliver Kullmann, 19.5.2005 (Swansea)

#ifndef BASICSETOPERATIONS_lJnB45

#define BASICSETOPERATIONS_lJnB45

#include <set>
#include <utility>
#include <vector>
#include <algorithm>
#include <iterator>

namespace OKlib {

  namespace SetAlgorithms {

    template <typename InputIterator_sets, typename InputIterator_elements, typename OutputIterator>
    struct Union {
      // ToDo: Adding concept etc.
      // InputIterator::value_type is std::pair<InputIterator_elements, InputIterator_elements>.
    private :
      typedef std::pair<InputIterator_elements, InputIterator_elements> Range;
      struct comparison : std::unary_function<Range, bool> {
        bool operator() (const Range& r1, const Range& r2) const {
          return *r1.first < *r2.first;
        }
      };
      typedef std::multiset<Range, comparison> Multiset;
      typedef typename Multiset::iterator multiset_iterator;
      typedef typename InputIterator_elements::value_type value_type;
      typedef std::vector<Range> Vector;
      typedef typename Vector::const_iterator vector_iterator;
    public :
      OutputIterator operator() (const InputIterator_sets begin, const InputIterator_sets end, OutputIterator out) const {
        Multiset first_elements;
        for (InputIterator_sets i = begin; i != end; ++i) {
          const Range r(*i);
          if (r.first != r.second)
            first_elements.insert(r);
          // ToDo: Using STL
        }
        Vector  to_be_updated;
        to_be_updated.reserve(first_elements.size());
        while (not first_elements.empty()) {
          multiset_iterator begin(first_elements.begin());
          const Range r(*begin);
          const multiset_iterator end(first_elements.upper_bound(r));
          const value_type e(*(r.first));
          *(out++) = e;
          std::copy(begin, end, std::back_inserter(to_be_updated));
          first_elements.erase(begin, end);
          const vector_iterator update_end(to_be_updated.end());
          for (vector_iterator begin(to_be_updated.begin()); begin != update_end; ++begin) {
            Range r(*begin);
            ++r.first;
            if (r.first != r.second)
              first_elements.insert(r);
            // ToDo: Using STL
          }
          to_be_updated.clear();
        }
        return out;
      }
    };
    
  }

}

#endif
