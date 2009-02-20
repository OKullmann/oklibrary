// Oliver Kullmann, 27.12.2005 (Swansea)
/* Copyright 2005 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#ifndef NUMERICALTESTOBJECTS_uHnBfqpq
#define NUMERICALTESTOBJECTS_uHnBfqpq

#include <OKlib/Programming/MetaProgramming/Numerical.hpp>
#include <OKlib/Programming/MetaProgramming/Numerical_Tests.hpp>

namespace OKlib {
  namespace MetaProgramming {

    template class Test_Power<Power>;
    template class Test_Binomial<Binomial>;
  }

}

#endif
