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

    //template <class PrefixContainer>
    class Test_AssociativePrefixContainer : public ::OKlib::TestSystem::Test {
    public :
      // typedef Test_AssociativePrefixContainer test_type;
    private :
      void perform_test_trivial() {
        AssociativePrefixContainer<std::string> prefix_container;
        prefix_container.insert("test.hpp/dir1/dir0/");
	prefix_container.insert("test.hpp/dir1");
        prefix_container.end();
        prefix_container.begin();
        prefix_container.first_extension("test.hpp");
      }
    };

  }

}

#endif
