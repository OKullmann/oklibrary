// Oliver Kullmann, 26.5.2005 (Swansea)

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
