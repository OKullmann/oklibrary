// Oliver Kullmann, 6.12.2006 (Swansea)

/*!
  \file AtomicConditions/traits/value_type.hpp
  \brief Traits-metafunctions to extract the value type
*/

#ifndef VALUETYPE_jjdbbgsgs5Li
#define VALUETYPE_jjdbbgsgs5Li

namespace OKlib {
  namespace AtomicConditions {
    namespace traits {

      /*!
        \class value_type
        \brief Traits meta-function to extract the value type.
      */
      
      template <typename T>
      struct value_type {
        typedef typename T::value_type type;
      };
    }
  }
}

#endif
