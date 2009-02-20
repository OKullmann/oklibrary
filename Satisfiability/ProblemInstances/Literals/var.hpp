// Oliver Kullmann, 8.12.2006 (Swansea)
/* Copyright 2006 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ProblemInstances/Literals/var.hpp
  \brief Primary function template for extracting the variable from a literal

  \todo For the future perhaps we should use a call traits instead of the fixed
  argument type const L& ?
*/

#ifndef VAR_jJJn1010156
#define VAR_jJJn1010156

#include <OKlib/Satisfiability/ProblemInstances/Literals/traits/var_type.hpp>

namespace OKlib {
  namespace Literals {

    /*!
      \brief Primary function template to extract variables from literals
    */

    template <typename L>
    inline typename OKlib::Literals::traits::var_type<L>::type var(const L& x);

  }
}

#endif
