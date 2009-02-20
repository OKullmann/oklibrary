// Oliver Kullmann, 8.12.2006 (Swansea)
/* Copyright 2006 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ProblemInstances/Literals/cond.hpp
  \brief Primary function template for extracting the (atomic) condition from a literal
*/

#ifndef COND_hahsVVbq715
#define COND_hahsVVbq715

#include <OKlib/Satisfiability/ProblemInstances/Literals/traits/cond_type.hpp>

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
