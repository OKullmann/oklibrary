// Oliver Kullmann, 12.12.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file OKlib/Satisfiability/Values/traits/exceptional_value.hpp
  \brief Metafunction for the exceptional value

*/

#ifndef EXCEPTIONALVALUE_J6FcS3
#define EXCEPTIONALVALUE_J6FcS3

namespace OKlib {
  namespace Satisfiability {
    namespace Values {
      namespace traits {

        /*!
          \class exceptional_value
          \brief exceptional_value<V>::value is the exceptional value for value-type V
        */
        template <typename V>
        struct exceptional_value;

      }
    }
  }
}

#endif
