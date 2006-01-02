// Oliver Kullmann, 30.12.2005 (Swansea)

/*!
  \file size_type.hpp
  \brief Traits-metafunctions to extract the size type.
*/

#ifndef SIZETYPE_885Rt4

#define SIZETYPE_885Rt4

namespace OKlib {

  namespace Concepts {

    /*!
      \class size_type
      \brief Traits meta-function to extract the size type.
    */

    template <typename T>
    struct size_type {
      typedef typename T::size_type type;
    };

  }

}

#endif
