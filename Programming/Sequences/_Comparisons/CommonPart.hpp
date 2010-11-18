// Oliver Kullmann, 9.9.2007 (Swansea)
/* Copyright 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Sequences/_Comparisons/CommonPart.hpp
  \brief Generic algorithm for finding the common prefix of two sequences

  The main class is the functor class template Common_part<...>:
  <ul>
   <li> Sequences::Common_part()(range1, range2) returns a std::pair of iterators c1, c2
   into the two ranges such that the two sequences from the begin of range1/2
   to c1/2 are identical, and such that this common sequence is longest. </li>
   <li> Sequences::Sequences::Common_part_c is for constant ranges (returning pairs of
   const-iterators). </li>
  </ul>
  
  The convenience function Sequences::common_part(r1, r2) performs type
  deduction (including the differentiation between const and non-const)
  and the choice of implementation.
  
  %Implementation issues:
  <ul>
   <li> Sequences::Common_part and Sequences::Common_part_c are std::binary_function's. </li>
   <li> Currently two different implementations are offered:
    <ol>
     <li> The implementation associated with tag Sequences::Implementation_common_part_std
     just used std::mismatch, which in general might run twice over the sequences
     (to determine the size first). </li>
     <li> The implementation associated with tag Sequences::Implementation_common_part_self
     just performs the (straight-forward) algorithm. </li>
    </ol>
    In principle, std::mismatch just does the job, but its input is only range1 and
    the begin of range2, so it assumes that range2 is at least as long as range1
    (or a mismatch is found before). Our std-implementation just arranges for this
    condition.
   </li>
   <li> The convenience function Sequences::common_part(r1, r2) just chooses
   Sequences::Implementation_common_part_self. </li>
   <li> The three-arguments function Sequences::common_part(r1, r2, pol)
   is a helper function, implementing the special policy pol; it also exists
   in the const-version. </li>
   <li> The meta-function Sequences::Common_part_res_t (with const-version
   Sequences::Common_part_c_res_t) is used to compute the return type. </li>
  </ul>


  \todo How can we combine the const- and the non-const variant (avoiding copy-and-paste) ?
  <ul>
   <li> One could define a macro for the common code-block; this would be ugly. </li>
   <li> The problem appears to be just the different overload resolution for the boost-range
   operations; one should parameterise and explicitly control this. </li>
  </ul>
  
*/

#ifndef COMMONPART_jJbe4rt
#define COMMONPART_jJbe4rt

#include <algorithm>
#include <functional>
#include <tr1/tuple>
#include <utility>

#include <boost/range/iterator.hpp>
#include <boost/range/size_type.hpp>
#include <boost/range/size.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>

#include <OKlib/Programming/Sequences/declarations.hpp>

namespace OKlib {
  namespace Programming {
    namespace Sequences {

      template <class Range1, class Range2, class ImpPol>
      class Common_part;
      template <class Range1, class Range2, class ImpPol>
      class Common_part_c;
      
      template <class Range1, class Range2>
      struct Common_part_res_t {
        typedef std::tr1::tuple<
          typename boost::range_iterator<Range1>::type,
          typename boost::range_iterator<Range2>::type,
          typename boost::range_size<Range1>::type> type;
      };
      template <class Range1, class Range2>
      struct Common_part_c_res_t {
        typedef std::tr1::tuple<
          typename boost::range_const_iterator<Range1>::type,
          typename boost::range_const_iterator<Range2>::type,
          typename boost::range_size<Range1>::type> type;
      };
      
      struct Implementation_common_part {};
      struct Implementation_common_part_std : virtual Implementation_common_part {};
      struct Implementation_common_part_self : virtual Implementation_common_part {};

      template <class Range1, class Range2>
      struct Common_part<Range1, Range2, Implementation_common_part_std>
        : std::binary_function<Range1&, Range2&,
                               typename Common_part_res_t<Range1, Range2>::type> {

        typedef typename boost::range_iterator<Range1>::type iterator1_type;
        typedef typename boost::range_iterator<Range2>::type iterator2_type;
        typedef typename boost::range_size<Range1>::type size_type;

        std::tr1::tuple<iterator1_type, iterator2_type, size_type> operator()(Range1& r1, Range2& r2) const {
          if (boost::size(r1) <= boost::size(r2)) {
            const iterator1_type b1(boost::begin(r1));
            typedef std::pair<iterator1_type, iterator2_type> pair_type;
            const pair_type res(std::mismatch(b1, boost::end(r1), boost::begin(r2)));
            return std::tr1::make_tuple(res.first, res.second, std::distance(b1, res.first));
          }
          else {
            const iterator1_type b1(boost::begin(r1));
            typedef std::pair<iterator2_type, iterator1_type> pair_type;
            const pair_type res(std::mismatch(boost::begin(r2), boost::end(r2), b1));
            return std::tr1::make_tuple(res.second, res.first, std::distance(b1, res.second));
          }
        }
      };
      template <class Range1, class Range2>
      struct Common_part_c<Range1, Range2, Implementation_common_part_std>
        : std::binary_function<const Range1&, const Range2&,
                               typename Common_part_c_res_t<Range1, Range2>::type> {

        typedef typename boost::range_const_iterator<Range1>::type iterator1_type;
        typedef typename boost::range_const_iterator<Range2>::type iterator2_type;
        typedef typename boost::range_size<Range1>::type size_type;

        std::tr1::tuple<iterator1_type, iterator2_type, size_type> operator()(const Range1& r1, const Range2& r2) const {
          if (boost::size(r1) <= boost::size(r2)) {
            const iterator1_type b1(boost::begin(r1));
            typedef std::pair<iterator1_type, iterator2_type> pair_type;
            const pair_type res(std::mismatch(b1, boost::end(r1), boost::begin(r2)));
            return std::tr1::make_tuple(res.first, res.second, std::distance(b1, res.first));
          }
          else {
            const iterator1_type b1(boost::begin(r1));
            typedef std::pair<iterator2_type, iterator1_type> pair_type;
            const pair_type res(std::mismatch(boost::begin(r2), boost::end(r2), b1));
            return std::tr1::make_tuple(res.second, res.first, std::distance(b1, res.second));
          }
        }
      };

      template
      <class Range1, class Range2>
      struct Common_part<Range1, Range2, Implementation_common_part_self>
        : std::binary_function<const Range1&, const Range2&,
                               typename Common_part_res_t<Range1, Range2>::type> {

        typedef typename boost::range_iterator<Range1>::type iterator1_type;
        typedef typename boost::range_iterator<Range2>::type iterator2_type;
        typedef typename boost::range_size<Range1>::type size_type;

        std::tr1::tuple<iterator1_type, iterator2_type, size_type> operator()(Range1& r1, Range2& r2) const {
          const iterator1_type& end1(boost::end(r1));
          const iterator2_type& end2(boost::end(r2));
          iterator1_type it1(boost::begin(r1));
          iterator2_type it2(boost::begin(r2));
          size_type s = 0;
          for (; it1 != end1 and it2 != end2; ++it1, ++it2, ++s)
            if (*it1 != *it2) break;
          return std::tr1::make_tuple(it1, it2, s);
        }
      };
      template
      <class Range1, class Range2>
      struct Common_part_c<Range1, Range2, Implementation_common_part_self> :
        std::binary_function<const Range1&, const Range2&,
                             typename Common_part_c_res_t<Range1, Range2>::type > {

        typedef typename boost::range_const_iterator<Range1>::type iterator1_type;
        typedef typename boost::range_const_iterator<Range2>::type iterator2_type;
        typedef typename boost::range_size<Range1>::type size_type;

         std::tr1::tuple<iterator1_type, iterator2_type, size_type> operator()(const Range1& r1, const Range2& r2) const {
          const iterator1_type& end1(boost::end(r1));
          const iterator2_type& end2(boost::end(r2));
          iterator1_type it1(boost::begin(r1));
          iterator2_type it2(boost::begin(r2));
          size_type s = 0;
          for (; it1 != end1 and it2 != end2; ++it1, ++it2, ++s)
            if (*it1 != *it2) break;
          return std::tr1::make_tuple(it1, it2, s);
        }
      };

      template
      <class R1, class R2>
      inline typename Common_part_res_t<R1,R2>::type
      common_part(R1& r1, R2& r2) {
        return common_part(r1, r2, Implementation_common_part_self());
      }
      template
      <class R1, class R2>
      inline typename Common_part_c_res_t<R1,R2>::type
      common_part(const R1& r1, const R2& r2) {
        return common_part_c(r1, r2, Implementation_common_part_self());
      }

      template
      <class R1, class R2, class ImpPol>
      inline typename Common_part_res_t<R1,R2>::type
      common_part(R1& r1, R2& r2, ImpPol) {
        return Common_part<R1, R2, ImpPol>()(r1, r2);
      }
      template
      <class R1, class R2, class ImpPol>
      inline typename Common_part_c_res_t<R1,R2>::type
      common_part_c(const R1& r1, const R2& r2, ImpPol) {
        return Common_part_c<R1, R2, ImpPol>()(r1, r2);
      }

    }
  }
}

#endif
