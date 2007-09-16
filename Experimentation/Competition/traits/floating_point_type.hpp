// Oliver Kullmann, 30.12.2005 (Swansea)
/* Copyright 2005 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Competition/traits/floating_point_type.hpp
  \brief Traits-metafunctions to extract the floating point type.
*/

#ifndef FLOATINGPOINTTYPE_lalHe3
#define FLOATING_POINTTYPE_lalHe3

namespace OKlib {
  namespace SATCompetition {
    namespace traits {

      /*!
        \class floating_point_type
        \brief Traits meta-function to extract the floating_point type.
      */

      template <typename T>
      struct floating_point_type {
        typedef typename T::floating_point_type type;
      };

    }

  }

}

#endif
