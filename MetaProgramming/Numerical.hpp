// Oliver Kullmann, 27.12.2005 (Swansea)

/*!
  \file Numerical.hpp
  \brief Numerical metafunctions
*/

#ifndef NUMERICAL_iiJJhy6T

#define NUMERICAL_iiJJhy6T

#include <boost/mpl/long.hpp>

namespace OKlib {

  namespace MetaProgramming {

    /*!
      \class Power
      \brief Numerical template metafunction: Power<b,e>::value = b^e.
    */

    template <long b, unsigned e>
    struct Power : boost::mpl::long_<b * Power<b, e-1>::value> {};

    template <long b>
    struct Power<b, (unsigned) 0> : boost::mpl::long_<1> {};

    /*!
      \class Binomial
      \brief Numerical template metafunction: Binomial(n,k>::value = n over k.
    */

    template <unsigned long n, unsigned long k, bool n_less_k>
    struct Binomial_helper;

    template <unsigned long n, unsigned long k> 
    struct Binomial : boost::mpl::long_<Binomial_helper<n, k,  n<k>::value> {};

    template <unsigned long n, unsigned long k>
    struct Binomial_helper<n,k,true> : boost::mpl::long_<0> {};

    template <unsigned long n, unsigned long k>
    struct Binomial_helper<n,k,false> : boost::mpl::long_<Binomial_helper<n-1, k, n-1<k>::value + Binomial_helper<n-1,k-1,false>::value> {};

    template <unsigned long n>
    struct Binomial_helper<n,0,false> : boost::mpl::long_<1> {};

  }

}

#endif
