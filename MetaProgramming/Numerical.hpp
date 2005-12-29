// Oliver Kullmann, 27.12.2005 (Swansea)

/*!
  \file Numerical.hpp
  \brief Numerical metafunctions
  \todo Write Factorial.
  \todo What about the generalisations discussed for Power?
*/

#ifndef NUMERICAL_iiJJhy6T

#define NUMERICAL_iiJJhy6T

#include <boost/mpl/long.hpp>

namespace OKlib {

  namespace MetaProgramming {

    /*!
      \class Power
      \brief Numerical template metafunction: Power<b,e>::value = b^e.
      \todo How to generalise Power (and other similar functions), so that
      boost::mpl::long_ is no longer hardcoded? One problem is, that a corresponding
      template template parameter X should be defaultet to boost::mpl::long_, and so
      must come first, but the types of b and e need to be X::value_type ?
      Perhaps a helper construction is needed.
      Another problem is, whether we can introduce X as a template template parameter,
      and write nevertheless X::value_type ? This likely is not possible.
      Like the concept of a metafunction class, perhaps we should have X as a *class*,
      with a nested template "apply", which yields the "concrete value" ?!
      So X is a class with type member X::value_type, and X::template apply<n> yields
      the concrete value (type) ?! X would be a model of the concept IntegralConstantMetafunction.
      Power then would be
      template <class ICM, typename ICM::value_type b, unsigned e>
      struct Power : ICM::template apply<b * Power<ICM, b, e-1>::value> {};
      template <class ICM, typename ICM::value_type b>
      struct Power<ICM, b, (unsigned 0)> : ICM::template apply<1> {};
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
