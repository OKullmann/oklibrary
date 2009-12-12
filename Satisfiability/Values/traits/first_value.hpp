// Oliver Kullmann, 12.12.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file OKlib/Satisfiability/Values/traits/first_value.hpp
  \brief Metafunction for the first value

*/

#ifndef FIRSTVALUE_qOyTrE3T5
#define FIRSTVALUE_qOyTrE3T5

namespace OKlib {
  namespace Satisfiability {
    namespace Values {
      namespace traits {

        /*!
          \class first_value
          \brief first_value<V>::value is the first value for value-type V
        */
        template <typename V>
        struct first_value;

      }
    }
  }
}

#endif
