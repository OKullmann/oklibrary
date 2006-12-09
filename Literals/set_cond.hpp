// Oliver Kullmann, 9.12.2006 (Swansea)

/*!
  \file Literals/set_cond.hpp
  \brief Primary function template to set the (atomic) condition in a literal
*/

#ifndef SETCOND_2772ahhaBnm
#define SETCOND_2772ahhaBnm

#include <Transitional/Literals/traits/cond_type.hpp>

namespace OKlib {
  namespace Literals {

    /*!
      \brief Primary function template to set conditions in literals
    */

    template <typename L>
    inline void set_cond(L& x, const typename OKlib::Literals::traits::cond_type<L>::type& cond);
  }
}

#endif
