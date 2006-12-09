// Oliver Kullmann, 5.12.2006 (Swansea)

/*!
  \file Literals/traits/cond_type.hpp
  \brief Traits-metafunctions to extract the condition type
*/

#ifndef CONDTYPE_akTEQW101n
#define CONDTYPE_akTEQW101n

namespace OKlib {
  namespace Literals {
    namespace traits {

      /*!
        \class cond_type
        \brief Traits meta-function to extract the condition type.
      */
      
      template <typename T>
      struct cond_type {
        typedef typename T::cond_type type;
      };
      
    }
  }
}

#endif
