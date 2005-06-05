// Oliver Kullmann, 26.5.2005 (Swansea)

#ifndef LOGICAL_84846ty

#define LOGICAL_84846ty

#include <boost/mpl/bool.hpp>

namespace OKlib {

  namespace MetaProgramming {

    namespace MacrosForMetaValues {

#define OKLIB_META_VALUE typedef type::value_type value_type; static const value_type value = type::value;
#define OKLIB_META_VALUE_T typedef typename type::value_type value_type; static const value_type value = type::value;

#define OKLIB_META_TRUE typedef ::boost::mpl::bool_<true> type; OKLIB_META_VALUE
#define OKLIB_META_FALSE typedef ::boost::mpl::bool_<false> type; OKLIB_META_VALUE
#define OKLIB_META_TRUE_T typedef ::boost::mpl::bool_<true> type; OKLIB_META_VALUE_T
#define OKLIB_META_FALSE_T typedef ::boost::mpl::bool_<false> type; OKLIB_META_VALUE_T

    }

    template <class T>
    struct True {
      OKLIB_META_TRUE;
    };

    template <class T>
    struct False {
      OKLIB_META_FALSE;
    };

  }

}

#endif
