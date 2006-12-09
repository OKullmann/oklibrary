// Oliver Kullmann, 8.12.2006 (Swansea)

/*!
  \file AtomicConditions/testobjects/TrivialAtomicConditions.cpp
*/

#include <Transitional/TestSystem/RunTest_DesignStudy.hpp>

#include <Transitional/Concepts/tests/AtomicConditions.hpp>

#include <Transitional/AtomicConditions/TrivialAtomicConditions.hpp>

namespace {

  ::OKlib::TestSystem::RunTest test_trivialatomiccondition(new OKlib::Concepts::tests::AtomicCondition_basic<OKlib::AtomicConditions::AC_bool>);

}

