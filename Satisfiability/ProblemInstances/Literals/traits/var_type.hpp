// Oliver Kullmann, 5.12.2006 (Swansea)
/* Copyright 2006 - 2007, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file OKlib/Satisfiability/ProblemInstances/Literals/traits/var_type.hpp
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
