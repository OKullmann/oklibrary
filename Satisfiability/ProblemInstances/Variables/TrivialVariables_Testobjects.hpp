// Oliver Kullmann, 5.6.2005 (Swansea)
/* Copyright 2005 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#ifndef TRIVIALVARIABLESTESTOBJECTS_90845TgH
#define TRIVIALVARIABLESTESTOBJECTS_90845TgH

#include <OKlib/Concepts/Variables_Tests.hpp>

#include <OKlib/Satisfiability/ProblemInstances/Variables/TrivialVariables.hpp>
#include <OKlib/Satisfiability/ProblemInstances/Variables/TrivialVariables_Tests.hpp>

namespace OKlib {
  namespace Variables {

    ::OKlib::Concepts::VariablesAsIndex_basic_test<Variables_signed_char> test_Variables_signed_char;
    ::OKlib::Concepts::VariablesAsIndex_basic_test<Variables_short_int> test_Variables_short_int;
    ::OKlib::Concepts::VariablesAsIndex_basic_test<Variables_int> test_Variables_int;
    ::OKlib::Concepts::VariablesAsIndex_basic_test<Variables_long_int> test_Variables_long_int;

    ::OKlib::Concepts::VariablesAsIndex_basic_test<Variables_unsigned_char> test_Variables_unsigned_char;
    ::OKlib::Concepts::VariablesAsIndex_basic_test<Variables_unsigned_short_int> test_Variables_unsigned_short_int;
    ::OKlib::Concepts::VariablesAsIndex_basic_test<Variables_unsigned_int> test_Variables_unsigned_int;
    ::OKlib::Concepts::VariablesAsIndex_basic_test<Variables_unsigned_long_int> test_Variables_unsigned_long_int;

  }
}

#endif
