// Oliver Kullmann, 19.5.2005 (Swansea)

#ifndef BASICSETOPERATIONS_lJnB45

#define BASICSETOPERATIONS_lJnB45

#include <set>
#include <utility>
#include <vector>
#include <algorithm>
#include <iterator>
#include <cassert>

namespace OKlib {

  namespace SetAlgorithms {

    template <typename InputIterator_sets, typename OutputIterator>
    struct Union {
      // ToDo: Adding specification of Union as functor class
      // ToDo: Adding concept etc.
      // InputIterator_sets::value_type::first_type = InputIterator_sets::value_type::second_type
      // Comment: When OutputIterator is std::back_insert_iterator etc., then return value is useless (except for further insertions).
      // ToDo: Concept of ranges.
    private :
      typedef typename InputIterator_sets::value_type::first_type InputIterator_elements;
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
      Union() {} // to enable constant objects
      OutputIterator operator() (const InputIterator_sets begin_sets, const InputIterator_sets end_sets, OutputIterator out) const {
        Multiset first_elements;
        for (InputIterator_sets i = begin_sets; i != end_sets; ++i) {
          const Range r(*i);
          if (r.first != r.second)
            first_elements.insert(r);
        }
        Vector  to_be_updated;
        to_be_updated.reserve(first_elements.size());
        for (; not first_elements.empty(); to_be_updated.clear()) {
          const multiset_iterator begin_new_elements(first_elements.begin());
          const Range r(*begin_new_elements);
          const multiset_iterator end_new_elements(first_elements.upper_bound(r));
          const value_type e(*(r.first));
          *(out++) = e;
          std::copy(begin_new_elements, end_new_elements, std::back_inserter(to_be_updated));
          first_elements.erase(begin_new_elements, end_new_elements);
          const vector_iterator update_end(to_be_updated.end());
          for (vector_iterator i(to_be_updated.begin()); i != update_end; ++i) {
            Range r(*i);
            if (++r.first != r.second)
              first_elements.insert(r);
          }
        }
        return out;
      }
    };

    template <typename InputIterator_sets, typename OutputIterator>
    inline OutputIterator union_sets(const InputIterator_sets begin_sets, const InputIterator_sets end_sets, const OutputIterator out) {
      return Union<InputIterator_sets, OutputIterator>()(begin_sets, end_sets, out);
    }

    // --------------------------------------------------------------------------------------------------------------------------

    template <typename InputIterator_sets, typename OutputIterator>
    struct Intersection {
      // ToDo: see Union
    private :
      typedef typename InputIterator_sets::value_type::first_type InputIterator_elements;
      typedef std::pair<InputIterator_elements, InputIterator_elements> Range;
      struct comparison : std::unary_function<Range, bool> {
        bool operator() (const Range& r1, const Range& r2) const {
          return *r1.first < *r2.first;
        }
      };
      typedef std::multiset<Range, comparison> Multiset;
      typedef typename Multiset::iterator multiset_iterator;
      typedef typename Multiset::size_type multi_set_size_type;
      typedef typename InputIterator_elements::value_type value_type;
      typedef std::vector<Range> Vector;
      typedef typename Vector::const_iterator vector_iterator;
    public :
      Intersection() {} // to enable constant objects
      OutputIterator operator() (const InputIterator_sets begin, const InputIterator_sets end, OutputIterator out) const {
        assert(begin != end);
        Multiset first_elements;
        for (InputIterator_sets i = begin; i != end; ++i) {
          const Range r(*i);
          if (r.first != r.second)
            first_elements.insert(r);
          else
            return out;
          // ToDo: Using STL
        }
        const multi_set_size_type number_sets = first_elements.size();
        Vector to_be_updated;
        to_be_updated.reserve(number_sets);
        for (;;) {
          const multiset_iterator begin_new_elements(first_elements.begin());
          const Range r(*begin_new_elements);
          const multi_set_size_type number_equal_elements = first_elements.count(r);
          const value_type e(*(r.first));
          if (number_equal_elements == number_sets) {
            const multiset_iterator end_new_elements = first_elements.end();
            *(out++) = e;
            std::copy(begin_new_elements, end_new_elements, std::back_inserter(to_be_updated));
            first_elements.clear();
          }
          else {
            const multiset_iterator end_new_elements = first_elements.upper_bound(r);
            std::copy(begin_new_elements, end_new_elements, std::back_inserter(to_be_updated));
            first_elements.erase(begin_new_elements, end_new_elements);
          }
          const vector_iterator update_end(to_be_updated.end());
          for (vector_iterator begin(to_be_updated.begin()); begin != update_end; ++begin) {
            Range r(*begin);
            ++r.first;
            if (r.first != r.second)
              first_elements.insert(r);
            else
              return out;
            // ToDo: Using STL
          }
          to_be_updated.clear();
        }
        return out;
      }
    };

    template <typename InputIterator_sets, typename OutputIterator>
    inline OutputIterator intersection_sets(const InputIterator_sets begin_sets, const InputIterator_sets end_sets, const OutputIterator out) {
      return Intersection<InputIterator_sets, OutputIterator>()(begin_sets, end_sets, out);
    }
    
  }

}

#endif
