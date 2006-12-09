// Oliver Kullmann, 8.12.2006 (Swansea)

/*!
  \file Literals/testobjects/TrivialLiterals.cpp
*/

#include <Transitional/TestSystem/RunTest_DesignStudy.hpp>

#include <Transitional/Concepts/tests/Literals.hpp>

#include <Transitional/Literals/TrivialLiterals.hpp>

namespace {

  ::OKlib::TestSystem::RunTest test_trivialliteral_signed_char(new OKlib::Concepts::tests::Literals_basic<OKlib::Literals::Literals_signed_char>);
  ::OKlib::TestSystem::RunTest test_trivialliteral_short_int(new OKlib::Concepts::tests::Literals_basic<OKlib::Literals::Literals_short_int>);
  ::OKlib::TestSystem::RunTest test_trivialliteral_int(new OKlib::Concepts::tests::Literals_basic<OKlib::Literals::Literals_int>);
  ::OKlib::TestSystem::RunTest test_trivialliteral_long_int(new OKlib::Concepts::tests::Literals_basic<OKlib::Literals::Literals_long_int>);

}

