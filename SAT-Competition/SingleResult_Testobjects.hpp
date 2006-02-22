// Oliver Kullmann, 11.6.2005 (Swansea)

#ifndef SINGLERESULTTESTOBJECTS_BVcr678

#define SINGLERESULTTESTOBJECTS_BVcr678

#include <Transitional/Concepts/ResultElements_Tests.hpp>

#include <Transitional/SAT-Competition/SingleResult.hpp>
#include <Transitional/SAT-Competition/SingleResult_Tests.hpp>

namespace OKlib {

  namespace SATCompetition {

    ::OKlib::Concepts::ResultElement_basic_test<ResultElement> test_ResultElement;
    ::OKlib::Concepts::ResultElementWithName_basic_test<ResultElement_with_name> test_ResultElement_with_name;

  }

}

#endif
