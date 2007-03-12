// Oliver Kullmann, 11.6.2005 (Swansea)

#ifndef SINGLERESULTTESTOBJECTS_BVcr678
#define SINGLERESULTTESTOBJECTS_BVcr678

#include <Transitional/Concepts/ResultElements_Tests.hpp>

#include <Transitional/SATCompetition/SingleResult.hpp>
#include <Transitional/SATCompetition/SingleResult_Tests.hpp>

namespace OKlib {

  namespace SATCompetition {

    ::OKlib::Concepts::ResultElement_basic_test<SATCompetition::ResultElement> test_ResultElement;
    ::OKlib::Concepts::ResultElementWithName_basic_test<SATCompetition::ResultElement_with_name> test_ResultElement_with_name;

  }

}

#endif
