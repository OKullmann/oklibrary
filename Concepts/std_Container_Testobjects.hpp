// Oliver Kullmann, 3.7.2005 (Swansea)

#ifndef STDCONTAINERTESTOBJECTS_6654ama

#define STDCONTAINERTESTOBJECTS_6654ama

#include <Transitional/Concepts/LibraryBasics_Tests.hpp>

#include <Transitional/Concepts/std_Container.hpp>
#include <Transitional/Concepts/std_Container_Tests.hpp>

namespace OKlib {

  namespace Concepts {

    OKLIB_BASIC_CONCEPT_TEST_TAG(Container)
    OKLIB_BASIC_CONCEPT_TEST_ARCHETYPE(Container)

    template class Test_ContainerConcept<Container>;

  }

}

#endif
