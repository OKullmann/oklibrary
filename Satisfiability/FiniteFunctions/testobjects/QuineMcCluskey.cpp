// Matthew Gwynne, 4.10.2010 (Swansea)
/* Copyright 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#include <OKlib/TestSystem/RunTest_DesignStudy.hpp>

#include <OKlib/Satisfiability/FiniteFunctions/tests/QuineMcCluskey.hpp>

namespace {

  ::OKlib::TestSystem::RunTest test_QuineMcCluskey(new ::OKlib::Satisfiability::FiniteFunctions::tests::Test_QuineMcCluskey< OKlib::Satisfiability::FiniteFunctions::QuineMcCluskey>());

}

