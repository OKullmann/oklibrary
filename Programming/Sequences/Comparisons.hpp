// Oliver Kullmann, 8.9.2007 (Swansea)

/*!
  \file Sequences/Comparisons.hpp
  \brief Generic algorithms comparing sequences

*/

#ifndef COMPARISONS_HffDEr46n
#define COMPARISONS_HffDEr46n

#include <algorithm>

#include <boost/range/iterator.hpp>
#include <boost/range/size.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>

namespace OKlib {
  namespace Programming {
    namespace Sequences {

      /*!
        \class Common_part
        \brief Functor class for extracting the largest common prefix from two sequences

        Sequences::Common_part()(range1, range2) return an iterator c into range 1
        such that the sequence [begin(range1), c) is identical to the sequence
        [begin(range2), begin(range2) + (c - begin(range1))], where the second
        sequence is fully defined, and such that c - begin(range1) is maximal
        with this property.

        The convenience function Sequences::common_part(r1, r2) performs type
        deduction and the choice of implementation.

        Implementation issues:
        <ul>
         <li> The implementation associated with tag Sequences::Implementation_common_part_std
         just used std::mismatch, which in general might run twice over the sequences
         (to determine the size first). </li>
        </ul>

      */

      struct Implementation_common_part {};
      struct Implementation_common_part_std : virtual Implementation_common_part {};
      struct Implementation_common_part_self : virtual Implementation_common_part {};

      template
      <class Range1, class Range2, class ImpPol>
      class Common_part;

      template
      <class Range1, class Range2>
      struct Common_part<Range1, Range2, Implementation_common_part_std> {
        typedef typename boost::range_iterator<Range1>::type iterator_type;
        iterator_type operator()(const Range1& r1, const Range2& r2) const {
          if (boost::size(r1) <= boost::size(r2))
            return std::mismatch(boost::begin(r1), boost::end(r1), boost::begin(r2)).first;
          else
            return std::mismatch(boost::begin(r2), boost::end(r2), boost::begin(r1)).second;
        }
      };
      template
      <class Range1, class Range2>
      struct Common_part<Range1, Range2, Implementation_common_part_self> {
        typedef typename boost::range_iterator<Range1>::type iterator_type;
        iterator_type operator()(const Range1& r1, const Range2& r2) const {
          typedef typename boost::range_iterator<Range2>::type iterator2_type;
          const iterator_type& end(boost::end(r1));
          const iterator2_type& end2(boost::end(r2));
          iterator_type it(boost::begin(r1));
          iterator2_type it2(boost::begin(r2));
          for (; it != end and it2 != end2; ++it, ++it2)
            if (*it != *it2) break;
          return it;
        }
      };

      template
      <class Range1, class Range2>
      inline typename boost::range_iterator<Range1>::type
      common_part(const Range1& r1, const Range2& r2) {
        return common_part(r1, r2, Implementation_common_part_std());
      }
      template
      <class Range1, class Range2, class ImpPol>
      inline typename boost::range_iterator<Range1>::type
      common_part(const Range1& r1, const Range2& r2, ImpPol) {
        return Common_part<Range1, Range2, ImpPol>(r1, r2);
      }

    }
  }
}

#endif
