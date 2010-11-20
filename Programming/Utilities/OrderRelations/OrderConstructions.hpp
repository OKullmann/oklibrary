// Oliver Kullmann, 2.7.2005 (Swansea)
/* Copyright 2005 - 2007, 2009, 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Programming/Utilities/OrderRelations/OrderConstructions.hpp
  \brief Components for the construction of order relations
*/

#ifndef ORDERCONSTRUCTIONS_jdjJn6
#define ORDERCONSTRUCTIONS_jdjJn6

#include <functional>

#include <boost/functional.hpp>

namespace OKlib {
 namespace Programming {
  namespace Utilities {
   namespace OrderRelations {

     /*!
       \class SizeLessThan
       \brief Predicate-functor, which turns a container-comparator into one
        which compares first the sizes.
  
       If < is the comparison for containers A, B, then we have A <' B,
       where <' is the obtained comparison, iff either A.size() < B.size()
       or A.size() = B.size() and A < B.

       \todo Write tests
     */
     template <class LessThanRelation>
     struct SizeLessThan : std::binary_function<
       typename boost::binary_traits<LessThanRelation>::first_argument_type,
       typename boost::binary_traits<LessThanRelation>::second_argument_type,
       bool> {
       bool operator() (const typename boost::binary_traits<LessThanRelation>::first_argument_type& arg1, const typename boost::binary_traits<LessThanRelation>::second_argument_type& arg2) const {
         typedef typename boost::binary_traits<LessThanRelation>::first_argument_type first_argument_type;
         typedef typename boost::binary_traits<LessThanRelation>::second_argument_type second_argument_type;
         typedef typename first_argument_type::size_type first_size_type;
         typedef typename second_argument_type::size_type second_size_type;
         const first_size_type& size1(arg1.size());
         const second_size_type& size2(arg2.size());
         return size1 < size2 or (size1 == size2 and LessThanRelation()(arg1, arg2));
       }
     };
    }
   }
  }
}

#endif
