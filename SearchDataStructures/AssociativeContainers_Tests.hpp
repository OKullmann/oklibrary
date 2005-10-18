/*!
  \file AssociativeContainers_Tests.hpp
  \brief Tests for search data structures.
*/

#ifndef ASSOCIATIVECONTAINERSTESTS_okMMnh

#define ASSOCIATIVECONTAINERSTESTS_okMMnh

#include <string>

#include "TestBaseClass.hpp"
#include "TestExceptions.hpp"

namespace OKlib {

  namespace SearchDataStructures {

    /*!
      \class Test_AssociativePrefixContainer
      \brief Test for associative prefix container
      \todo More systematic testing (using vectors of test data).
      \todo Once a concept has been created, test the syntax
    */

    template <template <class Range> class PrefixContainer>
    class Test_AssociativePrefixContainer : public ::OKlib::TestSystem::TestBase {
    public :
      typedef Test_AssociativePrefixContainer test_type;
      Test_AssociativePrefixContainer() {
        insert(this);
      }
    private :
      void perform_test_trivial() {
        {
          typedef PrefixContainer<std::string> APC;
          typedef typename APC::iterator iterator;

          APC prefix_container;

          if (prefix_container.begin() != prefix_container.end())
            OKLIB_THROW("prefix_container.begin() != prefix_container.end()");
          
          const std::string test_string("test.hpp");
          
          {
            const iterator& found(prefix_container.first_extension(test_string));
            const iterator& end(prefix_container.end());
            if (found != end)
              OKLIB_THROW("found != end");
          }

          const std::string word1("test.hpp/dir1/dir0/");
          prefix_container.insert(word1);
          if (prefix_container.begin() == prefix_container.end())
            OKLIB_THROW("prefix_container.begin() == prefix_container.end()");

          {
            const iterator& found(prefix_container.first_extension(test_string));
            const iterator& end(prefix_container.end());
            OKLIB_TEST_NOTEQUAL(found, end);
            OKLIB_TEST_EQUAL(*found, word1);
          }

          const std::string word2("test.hpp/dir1");
          prefix_container.insert(word2);

          {
            const iterator& found(prefix_container.first_extension(test_string));
            const iterator& end(prefix_container.end());
            OKLIB_TEST_NOTEQUAL(found, end);
            OKLIB_TEST_EQUAL(*found, word2);
          }
        }
      }
    };

  }

}

#endif
