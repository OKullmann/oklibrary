// Oliver Kullmann, 11.9.2004 (Swansea)
/* Copyright 2004 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#ifndef NUMERICS_TESTOBJECTS_bbzqQal9461

#define NUMERICS_TESTOBJECTS_bbzqQal9461

#include <OKlib/General/Numerics.hpp>
#include <OKlib/General/Numerics_Tests.hpp>

namespace Numerics_Testobjects {

  Numerics_Tests::test_integer_exponentiation<Numerics::Integer_exponentiation> test_integer_exponentiation_1;
  Numerics_Tests::test_round<Numerics::Round> test_round_1;
  Numerics_Tests::test_nat_1_2<Numerics::Nat_1_2_diag> test_nat_1_2_1;
  Numerics_Tests::test_graph_edges<Numerics::Graph_edges> test_graph_edges_1;

}

#endif


