// Oliver Kullmann, 9.12.2006 (Swansea)
/* Copyright 2006 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ProblemInstances/Literals/set_cond.hpp
  \brief Primary function template to set the (atomic) condition in a literal
*/

#ifndef SETCOND_2772ahhaBnm
#define SETCOND_2772ahhaBnm

#include <OKlib/Satisfiability/ProblemInstances/Literals/traits/cond_type.hpp>

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
