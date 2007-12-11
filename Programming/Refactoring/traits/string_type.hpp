// Oliver Kullmann, 30.12.2005 (Swansea)
/* Copyright 2005 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Programming/Refactoring/traits/string_type.hpp
  \brief Traits-metafunctions to extract the string type.
*/

#ifndef STRINGTYPE_HDHDH554E
#define STRINGTYPE_HDHDH554E

namespace OKlib {
  namespace Refactoring {
    namespace traits {

      /*!
        \class string_type
        \brief Traits meta-function to extract the string type.
      */

      template <typename T>
      struct string_type {
        typedef typename T::string_type type;
      };

    }

  }

}

#endif
