// Oliver Kullmann, 26.5.2005 (Swansea)
/* Copyright 2005 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#ifndef LOGICALTESTS_kJhBv5
#define LOGICALTESTS_kJhBv5

#include <boost/static_assert.hpp>

namespace OKlib {
  namespace MetaProgramming {

    template <template <class T> class True>
    class Test_True_T {
      struct X {};
      BOOST_STATIC_ASSERT(True<X>::value);
      BOOST_STATIC_ASSERT(True<int>::value);
    };

    template <template <class T> class False>
    class Test_False_T {
      struct X {};
      BOOST_STATIC_ASSERT(not False<X>::value);
      BOOST_STATIC_ASSERT(not False<int>::value);
    };
  }

}

#endif
