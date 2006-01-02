// Oliver Kullmann, 30.12.2005 (Swansea)

/*!
  \file string_type.hpp
  \brief Traits-metafunctions to extract the string type.
  \todo This should go to OKlib/Variables/Traits
*/

#ifndef STRINGTYPE_101019HHy

#define STRINGTYPE_101019HHy

namespace OKlib {

  namespace Concepts {

    /*!
      \class string_type
      \brief Traits meta-function to extract the string type.
    */

    template <typename T>
    struct string_type {
      typedef typename T::string_type type;
    };

  }

}

#endif
