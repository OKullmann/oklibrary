// Oliver Kullmann, 6.11.2004 (Swansea)

#ifndef NUMBERTHEORYTESTOBJECTS_dkknNnR43

#define NUMBERTHEORYTESTOBJECTS_dkknNnR43

#include <Transitional/General/NumberTheory_Concept_test.hpp>
#include <Transitional/General/NumberTheory_Models.hpp>

namespace NumberTheory_Testobjects {

  NumberTheory_Concept_test::Test_gcd<NumberTheory::Gcd> test_gcd;
  NumberTheory_Concept_test::Test_gcd_extended<NumberTheory::Gcd_extended, NumberTheory::Gcd> test_gcd_extended;

  NumberTheory_Concept_test::Test_Euler_phi<NumberTheory::Euler_phi_brute_force> test_Euler_phi_brute_force;
}

#endif
