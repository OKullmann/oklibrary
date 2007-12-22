// Oliver Kullmann, 11.6.2005 (Swansea)
/* Copyright 2005 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file OrderRelations/DerivedRelations.hpp
  \brief Functors and macros for order and comparison relations derived from < and == in the standard way.
*/

#ifndef DERIVEDRELATIONS_oPlK510v
#define DERIVEDRELATIONS_oPlK510v

#include <functional>
#include <boost/functional.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits.hpp>

namespace OKlib {
  namespace OrderRelations {

    /*!
      \class GreaterThan
      \brief Defines a > b as b < a.
    */

    template <class LessThanRelation>
    struct GreaterThan : std::binary_function<
      typename boost::binary_traits<LessThanRelation>::second_argument_type,
      typename boost::binary_traits<LessThanRelation>::first_argument_type,
      bool> {
      bool operator() (const typename boost::binary_traits<LessThanRelation>::second_argument_type& arg1, const typename boost::binary_traits<LessThanRelation>::first_argument_type& arg2) const {
        return LessThanRelation()(arg2, arg1);
      }
    };

    /*!
      \class GreaterEqualThan
      \brief Defines a >= b as not (a < b).
    */

    template <class LessThanRelation>
    struct GreaterEqualThan : std::binary_function<
      typename boost::binary_traits<LessThanRelation>::first_argument_type,
      typename boost::binary_traits<LessThanRelation>::second_argument_type,
      bool> {
      bool operator() (const typename boost::binary_traits<LessThanRelation>::first_argument_type& arg1, const typename boost::binary_traits<LessThanRelation>::second_argument_type& arg2) const {
        return not LessThanRelation()(arg1, arg2);
      }
    };

    /*!
      \class LessEqualThan
      \brief Defines a <= b as not (b < a).
    */

    template <class LessThanRelation>
    struct LessEqualThan : std::binary_function<
      typename boost::binary_traits<LessThanRelation>::second_argument_type,
      typename boost::binary_traits<LessThanRelation>::first_argument_type,
      bool> {
      bool operator() (const typename boost::binary_traits<LessThanRelation>::second_argument_type& arg1, const typename boost::binary_traits<LessThanRelation>::first_argument_type& arg2) const {
        return not LessThanRelation()(arg2, arg1);
      }
    };

    /*!
      \class Equivalence
      \brief Defines a ~ b as not (a < b) and not (b < a).
    */

    template <class LessThanRelation>
    struct Equivalence : std::binary_function<
      typename boost::binary_traits<LessThanRelation>::first_argument_type,
      typename boost::binary_traits<LessThanRelation>::first_argument_type,
      bool> {
      BOOST_STATIC_ASSERT((boost::is_same<typename boost::binary_traits<LessThanRelation>::first_argument_type, typename boost::binary_traits<LessThanRelation>::second_argument_type>::value));
      bool operator() (const typename boost::binary_traits<LessThanRelation>::first_argument_type& arg1, const typename boost::binary_traits<LessThanRelation>::first_argument_type& arg2) const {
        return not LessThanRelation()(arg1, arg2) and not LessThanRelation(arg2, arg1);
      }
    };
  
#define OKLIB_DERIVED_UNEQUAL(type) inline bool operator !=(const type& lhs, const type& rhs) { return not(lhs == rhs); }

#define OKLIB_DERIVED_GREATER(type) inline bool operator >(const type& lhs, const type& rhs) { return rhs < lhs; }
#define OKLIB_DERIVED_LESSEQUAL(type) inline bool operator <=(const type& lhs, const type& rhs) { return not (rhs < lhs); }
#define OKLIB_DERIVED_GREATEREQUAL(type) inline bool operator >=(const type& lhs, const type& rhs) { return not (lhs < rhs); }

#define OKLIB_DERIVED_ORDERRELATIONS(type) \
    OKLIB_DERIVED_GREATER(type)            \
    OKLIB_DERIVED_LESSEQUAL(type)          \
    OKLIB_DERIVED_GREATEREQUAL(type)

#define OKLIB_DERIVED_ORDERRELATIONS_FRIENDS(type) \
    friend OKLIB_DERIVED_GREATER(type)             \
    friend OKLIB_DERIVED_LESSEQUAL(type)           \
    friend OKLIB_DERIVED_GREATEREQUAL(type)

#define OKLIB_DERIVED_RELATIONS(type) \
    OKLIB_DERIVED_UNEQUAL(type)       \
    OKLIB_DERIVED_ORDERRELATIONS(type)

#define OKLIB_DERIVED_RELATIONS_FRIENDS(type) \
    friend OKLIB_DERIVED_UNEQUAL(type)        \
    OKLIB_DERIVED_ORDERRELATIONS_FRIENDS(type)


#define OKLIB_DERIVED_UNEQUAL_TEMPLATE1(type) \
    template <typename T> OKLIB_DERIVED_UNEQUAL(type<T>)
#define OKLIB_DERIVED_UNEQUAL_TEMPLATE2(type) \
    template <typename T1, typename T2> OKLIB_DERIVED_UNEQUAL((type<T1, T2>))


#define OKLIB_DERIVED_ORDERRELATIONS_TEMPLATE1(type)       \
    template <typename T> OKLIB_DERIVED_GREATER(type<T>)   \
    template <typename T> OKLIB_DERIVED_LESSEQUAL(type<T>) \
    template <typename T> OKLIB_DERIVED_GREATEREQUAL(type<T>)


  }
}

#endif
