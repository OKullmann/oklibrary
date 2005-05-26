// Oliver Kullmann, 26.5.2005 (Swansea)

#ifndef LOGICAL_84846ty

#define LOGICAL_84846ty

#include <boost/mpl/bool.hpp>

namespace OKlib {

  namespace MetaProgramming {

    template <class T>
    struct True {
      typedef ::boost::mpl::bool_<true> type;
      typedef typename type::value_type value_type;
      static const value_type value = type::value;
    };

    template <class T>
    struct False {
      typedef ::boost::mpl::bool_<false> type;
      typedef typename type::value_type value_type;
      static const value_type value = type::value;
    };

  }

}

#endif
