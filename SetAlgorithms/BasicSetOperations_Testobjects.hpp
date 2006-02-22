// Oliver Kullmann, 19.5.2005 (Swansea)

#ifndef BASICSETOPERATIONSTESTOBJECTS_llLn4rT

#define BASICSETOPERATIONSTESTOBJECTS_llLn4rT

#include <Transitional/SetAlgorithms/BasicSetOperations.hpp>
#include <Transitional/SetAlgorithms/BasicSetOperations_Tests.hpp>

namespace OKlib {

  namespace SetAlgorithms {

    namespace Testobjects {

      Test_Union<Union> test_union;

      Test_Intersection<Intersection> test_intersection;

      // ToDo: Testing instantiation of Union and Intersection with archetypes.

    }

  }

}

#endif
