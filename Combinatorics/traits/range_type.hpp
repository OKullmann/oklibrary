// Oliver Kullmann, 18.6.2006 (Swansea)

/*!
  \file Combinatorics/traits/range_type.hpp
  \brief Traits-metafunction to extract the range type

  \todo Where is this needed??
*/

#ifndef COMBINATORICSRANGETYPE_kattrE4
#define COMBINATORICSRANGETYPE_kattrE4

namespace OKlib {
  namespace Combinatorics {
    namespace traits {

      /*!
        \class range_type
        \brief Traits meta-function to extract the range type
      */
      
      template <typename T>
      struct range_type {
        typedef typename T::range_type type;
      };
      
    }
    
  }

}

#endif
