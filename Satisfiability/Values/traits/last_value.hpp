// Oliver Kullmann, 12.12.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file OKlib/Satisfiability/Values/traits/last_value.hpp
  \brief Metafunction for the last value

*/

#ifndef LASTVALUE_1820KgT
#define LASTVALUE_1820KgT

namespace OKlib {
  namespace Satisfiability {
    namespace Values {
      namespace traits {

        /*!
          \class last_value
          \brief last_value<V>::value is the last value for value-type V
        */
        template <typename V>
        struct last_value;

      }
    }
  }
}

#endif
