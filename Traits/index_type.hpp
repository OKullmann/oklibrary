// Oliver Kullmann, 30.12.2005 (Swansea)

/*!
  \file index_type.hpp
  \brief Traits-metafunctions to extract the index type
  \todo This should go to OKlib/Variables/Traits
*/

#ifndef INDEXTYPE_772020l

#define INDEXTYPE_772020l

namespace OKlib {

  namespace Concepts {

    /*!
      \class index_type
      \brief Traits meta-function to extract the index type.
    */

    template <typename T>
    struct index_type {
      typedef typename T::index_type type;
    };

  }

}

#endif
