// Oliver Kullmann, 27.2.2006 (Swansea)
/* Copyright 2006 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file TestSystem/plans/TestSystem.hpp
  \brief Plans for the test system (as a whole)

  \todo Documentation:
   - especially the division of labour between the three streams,
     and how the log-facilities work at the different verbosity-levels
     need to be explained.

  \todo Build system:
   - enable linking of only updated testobject-files
   - is it possible to make out of the testobject.o-files
     a *dynamic* link library (a static link library is possible
     via ar)?

  \todo Compiler warnings
  <ul>
   <li> Somewhere else there should be something on the problem of how
   to eliminate all compiler warnings? </li>
   <li> The warning about comparison of signed and unsigned etc.
   can be disabled with -Wno-type-limits. </li>
  </ul>

*/

/*!
  \namespace OKlib::TestSystem
  \brief The components for higher-order unit-testing
*/

namespace OKlib {
  namespace TestSystem {
  }
}

