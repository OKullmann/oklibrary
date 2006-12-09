// Oliver Kullmann, 8.12.2006 (Swansea)

/*!
  \file Literals/var.hpp
  \brief Primary function template for extracting the variable from a literal
*/

#ifndef VAR_jJJn1010156
#define VAR_jJJn1010156

#include <Transitional/Literals/traits/var_type.hpp>

namespace OKlib {
  namespace Literals {

    /*!
      \brief Primary function template to extract variables from literals
    */

    template <typename L>
    typename OKlib::Literals::traits::var_type<L>::type var(const L& x);

  }
}

#endif
