// Oliver Kullmann, 3.7.2005 (Swansea)

/*!
  \file Concepts/std_Container_Tests.hpp
  \brief Generic tests related to container concepts from the standard.
  \deprecated Move to the new test system.

  \todo The only test available yet, Test_ContainerConcept, actually
  is a meta-test (testing the concept), so this should go (with the
  new test system, and once arbitrary directory-nesting is avaiable)
  to Concepts/tests/tests.
  \todo Write tests for Concepts::Container.
*/

#ifndef STDCONTAINERTESTS_bbvcC
#define STDCONTAINERTESTS_bbvcC

#include <string>

#include <vector>
#include <list>
#include <deque>
#include <set>

#include <boost/concept_check.hpp>

#include <Transitional/TestSystem/TestBaseClass.hpp>
#include <Transitional/TestSystem/TestExceptions.hpp>

namespace OKlib {
  namespace Concepts {

    /*!
      \class Test_ContainerConcept
      \brief Test for a container concept by instantiating container-types
      from the standard library.
    */

    template <template <typename C> class ContainerConcept>
    class Test_ContainerConcept {
      void constraints() {
        boost::function_requires<ContainerConcept<std::vector<int> > >();
        boost::function_requires<ContainerConcept<std::vector<std::string> > >();
        boost::function_requires<ContainerConcept<std::list<int> > >();
        boost::function_requires<ContainerConcept<std::list<std::string> > >();
        boost::function_requires<ContainerConcept<std::deque<int> > >();
        boost::function_requires<ContainerConcept<std::deque<std::string> > >();
        boost::function_requires<ContainerConcept<std::set<int> > >();
        boost::function_requires<ContainerConcept<std::set<std::string> > >();
        boost::function_requires<ContainerConcept<std::multiset<int> > >();
        boost::function_requires<ContainerConcept<std::multiset<std::string> > >();
      }
    };

  }

}

#endif
