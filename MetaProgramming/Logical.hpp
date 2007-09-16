// Oliver Kullmann, 26.5.2005 (Swansea)
/* Copyright 2005 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file MetaProgramming/Logical.hpp
  \brief Logical metafunctions, and some macros for defining metafunctions.

  \todo Likely the macros are superfluous, and should be removed.
*/

#ifndef LOGICAL_84846ty
#define LOGICAL_84846ty

#include <boost/mpl/bool.hpp>

namespace OKlib {

  namespace MetaProgramming {

    namespace MacrosForMetaValues {

#define OKLIB_META_VALUE typedef type::value_type value_type; \
            static const value_type value = type::value;
#define OKLIB_META_VALUE_T typedef typename type::value_type value_type; \
            static const value_type value = type::value;

#define OKLIB_META_TRUE typedef ::boost::mpl::bool_<true> type; OKLIB_META_VALUE
#define OKLIB_META_FALSE typedef ::boost::mpl::bool_<false> type; OKLIB_META_VALUE
#define OKLIB_META_TRUE_T typedef ::boost::mpl::bool_<true> type; OKLIB_META_VALUE_T
#define OKLIB_META_FALSE_T typedef ::boost::mpl::bool_<false> type; OKLIB_META_VALUE_T

    }

    template <class T>
    struct True : ::boost::mpl::bool_<true> {};

    template <class T>
    struct False  : ::boost::mpl::bool_<false> {};

  }

}

#endif
