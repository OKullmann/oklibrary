// Oliver Kullmann, 27.12.2005 (Swansea)
/* Copyright 2005 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#ifndef RELATIONTESTSTESTOBJECTS_99iu5
#define RELATIONTESTSTESTOBJECTS_99iu5

#include <OKlib/TestExecutionTools/RelationTests.hpp>
#include <OKlib/TestExecutionTools/RelationTests_Tests.hpp>

namespace OKlib {

  namespace TestExecutionTools {

    Test_TestReflexivity<TestReflexivity> test_testreflexivity;
    Test_TestIrreflexivity<TestIrreflexivity> test_testirreflexivity;

    Test_TestSymmetry<TestSymmetry> test_testsymmetry;
    Test_TestAsymmetry<TestAsymmetry> test_testasymmetry;
    Test_TestAntisymmetry<TestAntisymmetry> test_testantisymmetry;

    Test_TestTotality<TestTotality> test_testtotality;
    Test_TestTrichotomy<TestTrichotomy> test_testtrichotomy;

    Test_TestTransitivity<TestTransitivity> test_testtransitivity;

    Test_TestQuasiorder<TestQuasiorder> test_testquasiorder;
    Test_TestEquivalence<TestEquivalence> test_testequivalence;
  }

}

#endif
