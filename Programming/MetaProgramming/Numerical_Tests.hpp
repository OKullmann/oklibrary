// Oliver Kullmann, 27.12.2005 (Swansea)
/* Copyright 2005 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Programming/MetaProgramming/Numerical_Tests.hpp
  \brief Tests for numerical metafunctions
*/

#ifndef NUMERICALTESTS_98862819jjGf
#define NUMERICALTESTS_98862819jjGf

#include <boost/static_assert.hpp>

namespace OKlib {
  namespace MetaProgramming {

    /*!
      \class Test_Power
      \brief Testing the compiler power function (ad-hoc tests).
    */

    template <template <long b, unsigned e> class Power>
    class Test_Power {
      BOOST_STATIC_ASSERT((Power<0,0>::value == 1));
      BOOST_STATIC_ASSERT((Power<10,0>::value == 1));
      BOOST_STATIC_ASSERT((Power<0,10>::value == 0));
      BOOST_STATIC_ASSERT((Power<3,19>::value == 1162261467));
    };

    /*!
      \class Test_Binomial
      \brief Testing the compiler binomial function (ad-hoc tests).
    */

    template <template <unsigned long n, unsigned long k> class Binom>
    class Test_Binomial {
      BOOST_STATIC_ASSERT((Binomial<0,0>::value == 1));
      BOOST_STATIC_ASSERT((Binomial<10,0>::value == 1));
      BOOST_STATIC_ASSERT((Binomial<10,1>::value == 10));
      BOOST_STATIC_ASSERT((Binomial<10,2>::value == 45));
      BOOST_STATIC_ASSERT((Binomial<35,22>::value == 1476337800));
    };

  }

}

#endif
