// Oliver Kullmann, 8.12.2006 (Swansea)
/* Copyright 2006 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file AtomicConditions/testobjects/TrivialAtomicConditions.cpp
*/

#include <OKlib/TestSystem/RunTest_DesignStudy.hpp>

#include <OKlib/Concepts/tests/AtomicConditions.hpp>

#include <OKlib/Satisfiability/ProblemInstances/Conditions/AtomicConditions/TrivialAtomicConditions.hpp>

namespace {

  ::OKlib::TestSystem::RunTest test_trivialatomiccondition(new OKlib::Concepts::tests::AtomicCondition_basic<OKlib::AtomicConditions::AC_bool>);

}

