// Oliver Kullmann, 30.12.2005 (Swansea)
/* Copyright 2005 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Competition/traits/natural_number_type.hpp
  \brief Traits-metafunctions to extract the natural_number type.
*/

#ifndef NATURALNUMBERTYPE_bzbXe45
#define NATURALNUMBERTYPE_bzbXe45

namespace OKlib {
  namespace SATCompetition {
    namespace traits {

      /*!
        \class natural_number_type
        \brief Traits meta-function to extract the natural-number type.
      */

      template <typename T>
      struct natural_number_type {
        typedef typename T::natural_number_type type;
      };

    }

  }

}

#endif
