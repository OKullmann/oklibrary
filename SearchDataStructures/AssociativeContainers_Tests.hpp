/*!
  \file AssociativeContainers_Tests.hpp
  \brief Tests for search data structures.
*/

#ifndef ASSOCIATIVECONTAINERSTESTS_okMMnh

#define ASSOCIATIVECONTAINERSTESTS_okMMnh

#include "TestBaseClass.hpp"
#include "TestExceptions.hpp"

namespace OKlib {

  namespace SearchDataStructures {

    template <class PrefixContainer>
    class Test_AssociativePrefixContainer : public ::OKlib::TestSystem::Test {
      //PrefixContainer& container;
      const std::string test_string;
    public :
      typedef Test_AssociativePrefixContainer test_type;
      Test_AssociativePrefixContainer(const std::string& test_string) : test_string(test_string) {}
    private :
      void perform_test_trivial() {
        AssociativePrefixContainer<std::string> prefix_container;
        prefix_container.insert("Hello");
      }
    };

  }

}

#endif
