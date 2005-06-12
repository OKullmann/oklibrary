// Oliver Kullmann, 5.6.2005 (Swansea)

#ifndef TRIVIALVARIABLESTESTOBJECTS_90845TgH

#define TRIVIALVARIABLESTESTOBJECTS_90845TgH

#include "Variables_Tests.hpp"

#include "TrivialVariables.hpp"
#include "TrivialVariables_Tests.hpp"

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
