// Oliver Kullmann, 5.12.2006 (Swansea)
/* Copyright 2006 - 2007, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file OKlib/Satisfiability/ProblemInstances/Literals/traits/cond_type.hpp
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
