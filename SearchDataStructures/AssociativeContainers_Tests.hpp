/*!
  \file AssociativeContainers_Tests.hpp
  \brief Tests for search data structures.
*/

#ifndef ASSOCIATIVECONTAINERSTESTS_okMMnh

#define ASSOCIATIVECONTAINERSTESTS_okMMnh

#include <string>
#include <set>

#include <boost/assign/std/set.hpp>
#include <boost/assign/std/vector.hpp>
#include <boost/tuple/tuple.hpp>

#include "TestBaseClass.hpp"
#include "TestExceptions.hpp"

namespace OKlib {

  namespace SearchDataStructures {

    /*!
      \class BaseTestData
      \brief Base class for testing data.
    */

    class BaseTestData {
    public:
      typedef std::string value_type;
    };

    /*!
      \class PrefixTestData
      \brief A vector of strings, each of which is a string representation
      of the full path of some header.
    */

    class PrefixTestData : BaseTestData {
      
      typedef std::vector<value_type> prefix_vector_type;
      
    public:

      typedef prefix_vector_type::const_iterator const_iterator;
      static prefix_vector_type prefix_vector;

      PrefixTestData() {
        using namespace boost::assign;
        typedef value_type el_t;
        prefix_vector +=
          el_t("test001.hpp/dir001/"),
          el_t("test002.hpp/dir001/dir002/"), 
          el_t("test002.hpp/dir001/dir002/dir003/");
      }
      
    };

    PrefixTestData::prefix_vector_type PrefixTestData::prefix_vector;

    /*!
      \class ExtensionTestData
      \brief A vector of tuples. The first element of each tuple is a 
      header, the second is the first extension (according to the data
      in PrefixTestData) and the final is a boolean which is true if
      and only if the extension is unique.
    */

    class ExtensionTestData : BaseTestData {
     
      typedef boost::tuple<value_type, value_type, bool> tuple_type;
      typedef std::vector<tuple_type> extension_vector_type;

    public:

      typedef extension_vector_type::const_iterator const_iterator;
      static extension_vector_type extension_vector;

      ExtensionTestData() {
        using namespace boost::assign;
        typedef tuple_type el_t;
        extension_vector +=
          el_t("test001.hpp","test001.hpp/dir001/",true),
          el_t("test002.hpp","test002.hpp/dir001/dir002/",false);
      }

      value_type header(const_iterator i) {
        return i -> get<0>();
      }

      value_type extension_value(const_iterator i) {
        return i -> get<1>();
      }

      bool uniqueness(const_iterator i) {
        return i -> get<2>();
      }

    };

    ExtensionTestData::extension_vector_type ExtensionTestData::extension_vector;

    /*!
      \class Test_AssociativePrefixContainer
      \brief Test for associative prefix container
      \todo Test for extensions, where no extension is possible.
      \todo More test cases.
      \todo Replace old tests.
      \todo Once a concept has been created, test the syntax
      \todo Provide testing of assign member function in test_assign private member function.
      \todo Provide testing of begin member function in test_begin private member function.
      \todo Provide testing of end member function in test_end private member function.
      \todo Provide testing of insert member function in test_insert private member function.
    */

    template <template <class Range> class PrefixContainer>
    class Test_AssociativePrefixContainer : public ::OKlib::TestSystem::TestBase {
    public :

      typedef Test_AssociativePrefixContainer test_type;
      Test_AssociativePrefixContainer() {
        insert(this);
      }

    private :

      PrefixTestData prefix_test_data;
      ExtensionTestData extension_test_data;

      void test_first_extension() {

        typedef typename BaseTestData::value_type value_type;
        typedef AssociativePrefixContainer<value_type> APC_t;
        typedef ExtensionTestData::const_iterator iterator;
        APC_t prefix_container(prefix_test_data.prefix_vector);
        const iterator& end(extension_test_data.extension_vector.end());
        for (iterator begin(extension_test_data.extension_vector.begin()); begin != end; ++begin) {
          value_type header(extension_test_data.header(begin));
          value_type expected_extension(extension_test_data.extension_value(begin));
          typedef APC_t::const_iterator iterator;
          iterator first_extension(prefix_container.first_extension(header));
          iterator end(prefix_container.end());
          if (first_extension != end)
            OKLIB_TEST_EQUAL(*first_extension,expected_extension);
        }

      }

      void test_first_extension_uniqueness_checked() {
        
        typedef typename BaseTestData::value_type value_type;
        typedef AssociativePrefixContainer<value_type> APC_t;
        typedef ExtensionTestData::const_iterator iterator;
        APC_t prefix_container(prefix_test_data.prefix_vector);
        const iterator& end(extension_test_data.extension_vector.end());
        for (iterator begin(extension_test_data.extension_vector.begin()); begin != end; ++begin) {
          value_type header(extension_test_data.header(begin));
          value_type expected_extension(extension_test_data.extension_value(begin));
          typedef APC_t::const_iterator iterator;
          typedef APC_t::checked_iterator_type checked_iterator_type;
          checked_iterator_type first_extension(prefix_container.first_extension_uniqueness_checked(header));
          iterator end(prefix_container.end());
          if (first_extension.first != end)
            OKLIB_TEST_EQUAL(*(first_extension.first),expected_extension);
            
        }
        
      }

      void perform_test_trivial() {

        { 
          test_first_extension();
          test_first_extension_uniqueness_checked();
        }

        // ########################################################################################

        { // Testing of empty APC.

          typedef PrefixContainer<std::string> APC;
          typedef typename APC::iterator iterator;
          typedef typename APC::checked_iterator_type checked_iterator_type;

          APC prefix_container;

          if (prefix_container.begin() != prefix_container.end())
            OKLIB_THROW("prefix_container.begin() != prefix_container.end()");
          
          const std::string test_string("test.hpp");
          
          { // Test first_extension function.
            const iterator& found(prefix_container.first_extension(test_string));
            const iterator& end(prefix_container.end());
            if (found != end)
              OKLIB_THROW("found != end");
          }

          { // Test first_extension_uniqueness_checked function.
            const checked_iterator_type& found(prefix_container.first_extension_uniqueness_checked(test_string));
            OKLIB_TEST_EQUAL(found.second, true);
            if (found.second != true)
              OKLIB_THROW("found.second != true");
            const iterator& end(prefix_container.end());
            if (found.first != end)
              OKLIB_THROW("found.first != end");
          }
        }

        // #############################################################

        { // Testing of filled prefix container.

          typedef std::string string_t;
          typedef PrefixContainer<string_t> APC;
          typedef typename APC::iterator iterator;
          typedef typename APC::checked_iterator_type checked_iterator_type;
          typedef std::set<string_t> set_t;
          typedef set_t::const_iterator const_iterator_t;

          APC prefix_container;
          set_t expected_result_set;
          set_t result_set;

          using namespace boost::assign;   
          typedef string_t s_t;       

          { // Fill a set with prefix strings.
            expected_result_set += 
              s_t("/"), 
              s_t("test001.hpp/dir001/"), 
              s_t("test001.hpp/dir001/"), 
              s_t("test002.hpp/dir001/dir002/"), 
              s_t("test002.hpp/dir001/dir002/dir003/")
              ;
          }

          { // Fill prefix_container with same prefix_strings.
            const const_iterator_t& end(expected_result_set.end());
            for(const_iterator_t begin(expected_result_set.begin()); begin!=end; ++begin)
              {
                string_t prefix(*begin);
                checked_iterator_type result(prefix_container.insert(prefix));
                if (*(result.first) != prefix)
                  OKLIB_THROW("result.first != prefix");
                OKLIB_TEST_EQUAL(result.second,true);
              }
          }

          { // Fill result_set with contents of prefix_container.
            typedef typename APC::const_iterator iterator_t;
            const iterator_t& end(prefix_container.end());
            for(iterator_t begin(prefix_container.begin()); begin!=end; ++begin) 
              {
                result_set.insert(*begin);
              }
          }

          {
            if (expected_result_set != result_set)
              OKLIB_THROW("expected_result_set!=result_set");
          }

          // ####################################################


          typedef std::pair<string_t,bool> checked_string_t;
          typedef std::pair<string_t,checked_string_t> path_checked_string_t;
          typedef std::vector<path_checked_string_t> vector_t;
          vector_t test_vector;

          typedef path_checked_string_t el_t;
          typedef checked_string_t cs_t;

          using namespace boost::assign;

          test_vector +=
            el_t("test001.hpp",cs_t("test001.hpp/dir001/",true)),
            el_t("test002.hpp",cs_t("test002.hpp/dir001/dir002/",false));

          { // Testing of extension functionality.
            typedef vector_t::const_iterator iterator_t;
            const iterator_t& end(test_vector.end());
            for (iterator_t begin(test_vector.begin()); begin!=end; ++begin) {

              string_t test_string((*begin).first);
              checked_string_t((*begin).second);
              const iterator& end(prefix_container.end());
              const string_t& expected_extension(((*begin).second).first);              
              const bool& expected_uniqueness(((*begin).second).second);

              { // Testing of first_extension.
                const iterator& found(prefix_container.first_extension(test_string));
                OKLIB_TEST_NOTEQUAL(found, end);
                OKLIB_TEST_EQUAL(*found, expected_extension);
              }

              { // Testing of first_extension_uniqueness_checked.
                const checked_iterator_type& found_checked(prefix_container.first_extension_uniqueness_checked(test_string));
                const iterator& found_iter(found_checked.first);
                const bool& found_unique(found_checked.second);
                OKLIB_TEST_NOTEQUAL(found_iter, end);
                const string_t& extension(*found_iter);
                OKLIB_TEST_EQUAL(found_unique,expected_uniqueness);
                OKLIB_TEST_EQUAL(extension,expected_extension);
              }
            }
          }
          
        }

        // ######################################################

        {
          // Testing of APC assign function..

          typedef std::string string_t;
          typedef PrefixContainer<string_t> APC;
          typedef typename APC::iterator iterator;
          typedef typename APC::checked_iterator_type checked_iterator_type;
          typedef std::set<string_t> set_t;

          APC prefix_container;
          set_t expected_result_set;
          set_t result_set;

          using namespace boost::assign;
          typedef string_t s_t;

          expected_result_set += // Fill a set with prefix strings.
            s_t("/"),
            s_t("/dir001/"),
            s_t("/dir001/dir002/"),
            s_t("/dir001/dir002/dir003/");

          prefix_container.assign(expected_result_set); // Fill prefix_container with set contents using assign function

          typedef typename APC::const_iterator iterator_t;
          const iterator_t& end(prefix_container.end());
          for(iterator_t begin(prefix_container.begin()); begin!=end; ++begin) // Fill another set with contents of prefix_container.
            {
              result_set.insert(*begin);
            }

          if (expected_result_set != result_set)
            OKLIB_THROW("expected_result_set!=result_set");
        }

        // ###############################################################################################

        { // Older tests.
          typedef PrefixContainer<std::string> APC;
          typedef typename APC::iterator iterator;
          typedef typename APC::checked_iterator_type checked_iterator_type;

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
          {
            const checked_iterator_type& found(prefix_container.first_extension_uniqueness_checked(test_string));
            OKLIB_TEST_EQUAL(found.second, true);
            const iterator& end(prefix_container.end());
            OKLIB_TEST_NOTEQUAL(found.first, end);
            OKLIB_TEST_EQUAL(*found.first, word1);
          }

          const std::string word2("test.hpp/dir1");
          prefix_container.insert(word2);

          {
            const iterator& found(prefix_container.first_extension(test_string));
            const iterator& end(prefix_container.end());
            OKLIB_TEST_NOTEQUAL(found, end);
            OKLIB_TEST_EQUAL(*found, word2);
          }
          {
            const checked_iterator_type& found(prefix_container.first_extension_uniqueness_checked(test_string));
            OKLIB_TEST_EQUAL(found.second, false);
            const iterator& end(prefix_container.end());
            OKLIB_TEST_NOTEQUAL(found.first, end);
            OKLIB_TEST_EQUAL(*found.first, word2);
          }

          
        }
      }
    };

  }

}

#endif
