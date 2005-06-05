// Oliver Kullmann, 5.6.2005 (Swansea)

#ifndef TYPETRAITS_123RfVcX

#define TYPETRAITS_123RfVcX

#include <boost/mpl/or.hpp>

#include "Logical.hpp"

namespace OKlib {

  namespace MetaProgramming {

    template <typename T>
    struct is_unqualified_signed_integral {
      OKLIB_META_FALSE;
    };
    template<> struct is_unqualified_signed_integral<signed char> { OKLIB_META_TRUE; };
    template<> struct is_unqualified_signed_integral<short int> { OKLIB_META_TRUE; };
    template<> struct is_unqualified_signed_integral<int> { OKLIB_META_TRUE; };
    template<> struct is_unqualified_signed_integral<long int> { OKLIB_META_TRUE; };

    template <typename T>
    struct is_unqualified_unsigned_integral {
      OKLIB_META_FALSE;
    };
    template<> struct is_unqualified_unsigned_integral<unsigned char> { OKLIB_META_TRUE; };
    template<> struct is_unqualified_unsigned_integral<unsigned short int> { OKLIB_META_TRUE; };
    template<> struct is_unqualified_unsigned_integral<unsigned int> { OKLIB_META_TRUE; };
    template<> struct is_unqualified_unsigned_integral<unsigned long int> { OKLIB_META_TRUE; };

    template <typename T>
    struct is_unqualified_signedunsigned_integral {
      typedef typename
      ::boost::mpl::or_< is_unqualified_signed_integral<T>, is_unqualified_unsigned_integral<T> >::type type;
      OKLIB_META_VALUE_T;
    };

  }

}

#endif
