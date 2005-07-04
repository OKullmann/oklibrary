// Oliver Kullmann, 3.7.2005 (Swansea)

#ifndef STDCONTAINERTESTS_bbvcC

#define STDCONTAINERTESTS_bbvcC

#include <string>

#include <vector>
#include <list>
#include <deque>
#include <set>

#include <boost/concept_check.hpp>

#include "TestBaseClass.hpp"
#include "TestExceptions.hpp"

namespace OKlib {

  namespace Concepts {

    template <template <typename C> class ContainerConcept>
    class Test_ContainerConcept {
      void constraints() {
        boost::function_requires<Container<std::vector<int> > >();
        boost::function_requires<Container<std::vector<std::string> > >();
        boost::function_requires<Container<std::list<int> > >();
        boost::function_requires<Container<std::list<std::string> > >();
        boost::function_requires<Container<std::deque<int> > >();
        boost::function_requires<Container<std::deque<std::string> > >();
        boost::function_requires<Container<std::set<int> > >();
        boost::function_requires<Container<std::set<std::string> > >();
        boost::function_requires<Container<std::multiset<int> > >();
        boost::function_requires<Container<std::multiset<std::string> > >();
      }
    };

  }

}

#endif
