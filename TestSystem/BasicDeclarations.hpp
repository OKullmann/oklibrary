// Oliver Kullmann, 19.1.2006 (Swansea)
/* Copyright 2006 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file TestSystem/BasicDeclarations.hpp
  \brief Basic declarations for the test system

  Both TestSystem/TestBaseClass.hpp and TestSystem/TestExceptions.hpp need these types, but
  these two modules are independent of each other.
*/

#ifndef BASICDECLARATIONS_kJhYt65
#define BASICDECLARATIONS_kJhYt65

namespace OKlib {
  namespace TestSystem {

    /*!
      \typedef depth_number_type
      \brief Unsigned integral type to represent the level of
      nesting of test-metafunctions-calls.
    */

    typedef unsigned long int depth_number_type;

    /*!
      \typedef  line_number_type;
      \brief Unsigned integral type to represent source-code line numbers.
    */

    typedef unsigned long int line_number_type;

  }

}

#endif
