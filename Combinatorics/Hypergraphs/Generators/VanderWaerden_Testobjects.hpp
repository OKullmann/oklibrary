// Oliver Kullmann, 15.5.2005 (Swansea)
/* Copyright 2005 - 2007, 2009, 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#ifndef VANDERWAERDENHYPERGRAPHTESTOBJECTSS_1U8VvT56
#define VANDERWAERDENHYPERGRAPHTESTOBJECTSS_1U8VvT56

#include <OKlib/Combinatorics/Hypergraphs/Generators/VanderWaerden.hpp>
#include <OKlib/Combinatorics/Hypergraphs/Generators/VanderWaerden_Tests.hpp>

namespace OKlib {
  namespace Van_der_Waerden_hypergraph {
    namespace Testobjects {

      Test_Iterator_arithmetic_progression_Integer<OKlib::Combinatorics::Hypergraphs::Generators::Iterator_arithmetic_progression> test_iterator_arithmetic_progression;

      Test_Arithmetic_progression_Integer<OKlib::Combinatorics::Hypergraphs::Generators::Arithmetic_progression> test_arithmetic_progression;
      
    }
  }
}

#endif
