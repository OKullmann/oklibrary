// Oliver Kullmann, 18.6.2006 (Swansea)
/* Copyright 2006 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Combinatorics/traits/range_type.hpp
  \brief Traits-metafunction to extract the range type

  \todo Where is this needed??
*/

#ifndef COMBINATORICSRANGETYPE_kattrE4
#define COMBINATORICSRANGETYPE_kattrE4

namespace OKlib {
  namespace Combinatorics {
    namespace traits {

      /*!
        \class range_type
        \brief Traits meta-function to extract the range type
      */
      
      template <typename T>
      struct range_type {
        typedef typename T::range_type type;
      };
      
    }
    
  }

}

#endif
