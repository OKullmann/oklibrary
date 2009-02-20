// Oliver Kullmann, 6.11.2004 (Swansea)
/* Copyright 2004 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#ifndef NUMBERTHEORYTESTOBJECTS_dkknNnR43

#define NUMBERTHEORYTESTOBJECTS_dkknNnR43

#include <OKlib/General/NumberTheory_Concept_test.hpp>
#include <OKlib/General/NumberTheory_Models.hpp>

namespace NumberTheory_Testobjects {

  NumberTheory_Concept_test::Test_gcd<NumberTheory::Gcd> test_gcd;
  NumberTheory_Concept_test::Test_gcd_extended<NumberTheory::Gcd_extended, NumberTheory::Gcd> test_gcd_extended;

  NumberTheory_Concept_test::Test_Euler_phi<NumberTheory::Euler_phi_brute_force> test_Euler_phi_brute_force;
}

#endif
