// Oliver Kullmann, 5.6.2005 (Swansea)

/*!
  \file TypeTraits.hpp
  \brief Extending the (general) type traits from Boost.
  \todo Namespace should be changed to OKlib::Traits.
  \todo It seems we should guarantee that this file includes <boost/type_traits.hpp> ?!
*/

#ifndef TYPETRAITS_123RfVcX

#define TYPETRAITS_123RfVcX

#include <boost/mpl/or.hpp>
#include <boost/type_traits.hpp>

#include "Logical.hpp"

namespace OKlib {

  namespace MetaProgramming {

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
