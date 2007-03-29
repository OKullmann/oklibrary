// Oliver Kullmann, 3.7.2005 (Swansea)

#ifndef STDCONTAINERTESTOBJECTS_6654ama
#define STDCONTAINERTESTOBJECTS_6654ama

#include <Transitional/Concepts/LibraryBasics_Tests.hpp>

#include <Transitional/Concepts/std_Container.hpp>
#include <Transitional/Concepts/std_Container_Tests.hpp>

namespace OKlib {
  namespace Concepts {

    OKLIB_BASIC_CONCEPT_TEST_TAG(OKlib::Concepts::Container)
    OKLIB_BASIC_CONCEPT_TEST_ARCHETYPE(OKlib::Concepts::Container)

    template class OKlib::Concepts::Test_ContainerConcept<OKlib::Concepts::Container>;

  }

}

#endif
