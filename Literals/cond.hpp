// Oliver Kullmann, 8.12.2006 (Swansea)

/*!
  \file Literals/cond.hpp
  \brief Primary function template for extracting the (atomic) condition from a literal
*/

#ifndef COND_hahsVVbq715
#define COND_hahsVVbq715

#include <Transitional/Literals/traits/cond_type.hpp>

namespace OKlib {
  namespace Literals {

    /*!
      \brief Primary function template to extract conditions from literals
    */

    template <typename L>
    inline typename OKlib::Literals::traits::cond_type<L>::type cond(const L& x);
  }
}

#endif
