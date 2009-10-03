// Oliver Kullmann, 8.9.2007 (Swansea)
/* Copyright 2007, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#include <OKlib/TestSystem/RunTest_DesignStudy.hpp>

#include <OKlib/Programming/Sequences/declarations.hpp>
#include <OKlib/Programming/Sequences/Comparisons.hpp>
#include <OKlib/Programming/Sequences/tests/Comparisons.hpp>

namespace {

  ::OKlib::TestSystem::RunTest test_common_part_std(new ::OKlib::Prg::Seq::tests::CommonPart< ::OKlib::Prg::Seq::Common_part, ::OKlib::Prg::Seq::Implementation_common_part_std>);

  ::OKlib::TestSystem::RunTest test_common_part_self(new ::OKlib::Prg::Seq::tests::CommonPart< ::OKlib::Prg::Seq::Common_part, ::OKlib::Prg::Seq::Implementation_common_part_self>);

}

