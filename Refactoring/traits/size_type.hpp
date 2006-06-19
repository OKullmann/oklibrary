// Oliver Kullmann, 30.12.2005 (Swansea)

/*!
  \file Refactoring/traits/size_type.hpp
  \brief Traits-metafunctions to extract the size type.
*/

#ifndef SIZETYPE_JJDTTE54R
#define SIZETYPE_JJDTTE54R

namespace OKlib {
  namespace Refactoring {
    namespace traits {

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

}
  
#endif
