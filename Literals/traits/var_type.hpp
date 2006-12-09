// Oliver Kullmann, 5.12.2006 (Swansea)

/*!
  \file Literals/traits/var_type.hpp
  \brief Traits-metafunctions to extract the variables type
*/

#ifndef VARTYPE_jdnNBv63
#define VARTYPE_jdnNBv63

namespace OKlib {
  namespace Literals {
    namespace traits {

      /*!
        \class var_type
        \brief Traits meta-function to extract the variables type.
      */
      
      template <typename L>
      struct var_type {
        typedef typename L::var_type type;
      };
      
    }
  }
}

#endif
