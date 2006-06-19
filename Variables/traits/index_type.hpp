// Oliver Kullmann, 30.12.2005 (Swansea)

/*!
  \file Variables/traits/index_type.hpp
  \brief Traits-metafunctions to extract the index type
*/

#ifndef INDEXTYPE_772020l
#define INDEXTYPE_772020l

namespace OKlib {
  namespace Variables {
    namespace traits {

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

}

#endif
