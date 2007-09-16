// Oliver Kullmann, 5.6.2005 (Swansea)
/* Copyright 2005 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#ifndef TYPETRAITSTESTS_5767uvL
#define TYPETRAITSTESTS_5767uvL

#include <boost/static_assert.hpp>

namespace OKlib {

  namespace traits {

    template <template <typename T> class is_unqualified_signed_integral>
    class Test_is_unqualified_signed_integral_T {
      BOOST_STATIC_ASSERT(is_unqualified_signed_integral<signed char>::value);
      BOOST_STATIC_ASSERT(is_unqualified_signed_integral<short int>::value);
      BOOST_STATIC_ASSERT(is_unqualified_signed_integral<int>::value);
      BOOST_STATIC_ASSERT(is_unqualified_signed_integral<long int>::value);
      BOOST_STATIC_ASSERT(not is_unqualified_signed_integral<unsigned int>::value);
      struct X {};
      BOOST_STATIC_ASSERT(not is_unqualified_signed_integral<X>::value);
    };

    template <template <typename T> class is_unqualified_unsigned_integral>
    class Test_is_unqualified_unsigned_integral_T {
      BOOST_STATIC_ASSERT(is_unqualified_unsigned_integral<unsigned char>::value);
      BOOST_STATIC_ASSERT(is_unqualified_unsigned_integral<unsigned short int>::value);
      BOOST_STATIC_ASSERT(is_unqualified_unsigned_integral<unsigned int>::value);
      BOOST_STATIC_ASSERT(is_unqualified_unsigned_integral<unsigned long int>::value);
      BOOST_STATIC_ASSERT(not is_unqualified_unsigned_integral<int>::value);
      struct X {};
      BOOST_STATIC_ASSERT(not is_unqualified_unsigned_integral<X>::value);
    };

    template <template <typename T> class is_unqualified_signedunsigned_integral>
    class Test_is_unqualified_signedunsigned_integral_T {
      BOOST_STATIC_ASSERT(is_unqualified_signedunsigned_integral<signed char>::value);
      BOOST_STATIC_ASSERT(is_unqualified_signedunsigned_integral<short int>::value);
      BOOST_STATIC_ASSERT(is_unqualified_signedunsigned_integral<int>::value);
      BOOST_STATIC_ASSERT(is_unqualified_signedunsigned_integral<long int>::value);
      BOOST_STATIC_ASSERT(is_unqualified_signedunsigned_integral<unsigned char>::value);
      BOOST_STATIC_ASSERT(is_unqualified_signedunsigned_integral<unsigned short int>::value);
      BOOST_STATIC_ASSERT(is_unqualified_signedunsigned_integral<unsigned int>::value);
      BOOST_STATIC_ASSERT(is_unqualified_signedunsigned_integral<unsigned long int>::value);

      BOOST_STATIC_ASSERT(not is_unqualified_signedunsigned_integral<bool>::value);
      struct X {};
      BOOST_STATIC_ASSERT(not is_unqualified_signedunsigned_integral<X>::value);
    };

  }

}

#endif
