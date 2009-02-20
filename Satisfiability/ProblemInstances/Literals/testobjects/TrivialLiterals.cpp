// Oliver Kullmann, 8.12.2006 (Swansea)
/* Copyright 2006 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ProblemInstances/Literals/testobjects/TrivialLiterals.cpp
*/

#include <OKlib/TestSystem/RunTest_DesignStudy.hpp>

#include <OKlib/Concepts/tests/Literals.hpp>

#include <OKlib/Satisfiability/ProblemInstances/Literals/TrivialLiterals.hpp>

namespace {

  ::OKlib::TestSystem::RunTest test_trivialliteral_signed_char(new OKlib::Concepts::tests::Literals_basic<OKlib::Literals::Literals_signed_char>);
  ::OKlib::TestSystem::RunTest test_trivialliteral_short_int(new OKlib::Concepts::tests::Literals_basic<OKlib::Literals::Literals_short_int>);
  ::OKlib::TestSystem::RunTest test_trivialliteral_int(new OKlib::Concepts::tests::Literals_basic<OKlib::Literals::Literals_int>);
  ::OKlib::TestSystem::RunTest test_trivialliteral_long_int(new OKlib::Concepts::tests::Literals_basic<OKlib::Literals::Literals_long_int>);

}

