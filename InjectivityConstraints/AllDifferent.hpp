// Matthew Henderson, 24.4.2006 (Paderborn)

/*!
  \file InjectivityConstraints/AllDifferent.hpp
  \brief Algorithms for alldifferent constraint.
*/

#ifndef ALLDIFFERENT_08s7dg8O2

#define ALLDIFFERENT_08s7dg8O2

#include <set>
#include <algorithm>

#include <boost/range/value_type.hpp>
#include <boost/range/const_iterator.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <boost/range/size.hpp>

namespace OKlib {
  namespace InjectivityConstraints {

    class AllDifferentBySetSizeComparison {
    public:
      template <typename Range>
      bool operator() (Range R) {
        typedef typename boost::range_value<Range>::type value_type;
        typedef typename boost::range_const_iterator<Range>::type const_iterator;
        std::set<value_type> set_of_unique_elements;
        const const_iterator& end(boost::end(R));
        for (const_iterator begin(boost::begin(R)); begin!=end; ++begin) {
          set_of_unique_elements.insert(*begin);
        };        
        return (boost::size(R) == boost::size(set_of_unique_elements));
      }
    };

    class AllDifferentBySeenBeforeVectorFind {
    public:
      template <typename Range>
      bool operator() (Range R) {
        typedef typename boost::range_value<Range>::type value_type;
        typedef typename boost::range_const_iterator<Range>::type const_iterator;
        typedef std::vector<value_type> vector_t;
        vector_t seen_before_vector;
        const const_iterator& end(boost::end(R));
        for (const_iterator begin(boost::begin(R)); begin!=end; ++begin) {
          typename vector_t::iterator seen_before = std::find(seen_before_vector.begin(),seen_before_vector.end(),*begin);
          if ( seen_before == seen_before_vector.end() ) {
            seen_before_vector.push_back(*begin);
          }
          else {
            return false;
          }
        };  
        return true;
      }
    };
    
    template <typename Range>
    bool all_different(Range R) {
      return ::OKlib::InjectivityConstraints::AllDifferentBySeenBeforeVectorFind()(R);
    }
    
  }
}

#endif
