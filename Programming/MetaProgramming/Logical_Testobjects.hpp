// Oliver Kullmann, 26.5.2005 (Swansea)
/* Copyright 2005 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file MetaProgramming/Logical_Testobjects.hpp

  \todo Likely the special namespace Metafunction_tests should be eliminated.
*/

#ifndef LOGICALTESTOBJECTS_847ncVb
#define LOGICALTESTOBJECTS_847ncVb

#include <OKlib/Programming/MetaProgramming/Logical.hpp>
#include <OKlib/Programming/MetaProgramming/Logical_Tests.hpp>

namespace OKlib {
  namespace MetaProgramming {

      template class Test_True_T<True>;

      template class Test_False_T<False>;

  }

}

#endif
