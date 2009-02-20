// Oliver Kullmann, 8.1.2006 (Swansea)
/* Copyright 2006 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#include <OKlib/TestSystem/RunTest_DesignStudy.hpp>

#include <OKlib/TestSystem/tests/Testfunctions2.hpp>

namespace {

  ::OKlib::TestSystem::RunTest test1(new ::OKlib::TestSystem::tests::Test2);
  ::OKlib::TestSystem::RunTest test3(new ::OKlib::TestSystem::tests::Test3);

}
