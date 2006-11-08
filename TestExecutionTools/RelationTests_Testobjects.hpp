// Oliver Kullmann, 27.12.2005 (Swansea)

#ifndef RELATIONTESTSTESTOBJECTS_99iu5
#define RELATIONTESTSTESTOBJECTS_99iu5

#include <Transitional/TestExecutionTools/RelationTests.hpp>
#include <Transitional/TestExecutionTools/RelationTests_Tests.hpp>

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
