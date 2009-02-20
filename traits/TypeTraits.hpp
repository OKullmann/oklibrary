// Oliver Kullmann, 5.6.2005 (Swansea)
/* Copyright 2005 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file traits/TypeTraits.hpp
  \brief Extending the (general) type traits from Boost.
  \todo It seems we should guarantee that this file includes <boost/type_traits.hpp> ?!
*/

#ifndef TYPETRAITS_123RfVcX
#define TYPETRAITS_123RfVcX

#include <boost/mpl/or.hpp>
#include <boost/type_traits.hpp>

#include <OKlib/Programming/MetaProgramming/Logical.hpp>

namespace OKlib {
  namespace traits {

    template <typename T>
    struct is_unqualified_signed_integral : ::boost::mpl::bool_<false> {};
    template<> struct is_unqualified_signed_integral<signed char> : ::boost::mpl::bool_<true> {};
    template<> struct is_unqualified_signed_integral<short int> : ::boost::mpl::bool_<true> {};
    template<> struct is_unqualified_signed_integral<int> : ::boost::mpl::bool_<true> {};
    template<> struct is_unqualified_signed_integral<long int> : ::boost::mpl::bool_<true> {};

    template <typename T>
    struct is_unqualified_unsigned_integral : ::boost::mpl::bool_<false> {};
    template<> struct is_unqualified_unsigned_integral<unsigned char> : ::boost::mpl::bool_<true> {};
    template<> struct is_unqualified_unsigned_integral<unsigned short int> : ::boost::mpl::bool_<true> {};
    template<> struct is_unqualified_unsigned_integral<unsigned int> : ::boost::mpl::bool_<true> {};
    template<> struct is_unqualified_unsigned_integral<unsigned long int> : ::boost::mpl::bool_<true> {};

    template <typename T>
    struct is_unqualified_signedunsigned_integral : ::boost::mpl::or_< is_unqualified_signed_integral<T>, is_unqualified_unsigned_integral<T> > {};

    template <typename T>
    struct is_signed_integral : is_unqualified_signed_integral<typename ::boost::remove_cv<T>::type> {};
    template <typename T>
    struct is_unsigned_integral : is_unqualified_unsigned_integral<typename ::boost::remove_cv<T>::type> {};

  }
}

#endif
