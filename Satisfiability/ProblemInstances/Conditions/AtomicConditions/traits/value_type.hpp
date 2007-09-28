// Oliver Kullmann, 6.12.2006 (Swansea)
/* Copyright 2006 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file AtomicConditions/traits/value_type.hpp
  \brief Traits-metafunctions to extract the value type
*/

#ifndef VALUETYPE_jjdbbgsgs5Li
#define VALUETYPE_jjdbbgsgs5Li

namespace OKlib {
  namespace AtomicConditions {
    namespace traits {

      /*!
        \class value_type
        \brief Traits meta-function to extract the value type.
      */
      
      template <typename T>
      struct value_type {
        typedef typename T::value_type type;
      };
    }
  }
}

#endif
