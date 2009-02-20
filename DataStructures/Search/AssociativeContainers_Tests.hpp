// Matthew Henderson, 6.9.2005 (Swansea)
/* Copyright 2005 - 2007, 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file DataStructures/Search/AssociativeContainers_Tests.hpp
  \brief Tests for search data structures.

  \bug The tests succeed though the inverted path for Autarkies/Search
  is wrong. It seems these tests are not performed.

*/

#ifndef ASSOCIATIVECONTAINERSTESTS_okMMnh
#define ASSOCIATIVECONTAINERSTESTS_okMMnh

#include <string>
#include <set>

#include <boost/assign/std/set.hpp>
#include <boost/assign/std/vector.hpp>
#include <boost/tuple/tuple.hpp>

#include <OKlib/TestSystem/TestBaseClass.hpp>
#include <OKlib/TestSystem/TestExceptions.hpp>

namespace OKlib {
  namespace SearchDataStructures {

    // ##############################################################

    /*!
      \class BaseTestData
      \brief Base class for testing data.
      \todo More test data!
      \todo Test cases are needed where the elements of the associate prefix containers are more
      complex: Not just sequences of characters, but sequences of containers.
      \todo The test data is quite obscure; the reversal of order is very unnatural. The test cases here have nothing to do with the module Refactoring !!
      \todo No hierarchy; template parameter instead.
    */

    class BaseTestData {
    public:
      typedef std::string string_type;
    };

    /*!
      \class PrefixTestData
      \brief Provides a vector of strings, each of which is a string 
      representation of the full path of some header.
      \todo What is the meaning of the *two* vectors?
      \todo Static data member should need no constructor!!
    */

    class PrefixTestData : BaseTestData {
      
      typedef std::vector<string_type> prefix_vector_type;
      
    public:

      typedef prefix_vector_type::const_iterator const_iterator;
      static prefix_vector_type prefix_vector;
      static prefix_vector_type prefix_vector_2;


      PrefixTestData() {

        using namespace boost::assign;
        typedef string_type el_t;

        prefix_vector_2 +=
          el_t("AnalyseTotalAssignment.hpp/Autarkies/Search/OKsystem");

        prefix_vector +=
          el_t("test001.hpp/dir001/"),
          el_t("test001.hpp/dir001/"),
          el_t("test002.hpp/dir001/dir002/"), 
          el_t("test002.hpp/dir001/dir002/dir003/");
      }
      
    };

    PrefixTestData::prefix_vector_type PrefixTestData::prefix_vector;
    PrefixTestData::prefix_vector_type PrefixTestData::prefix_vector_2;

    // ##############################################################

    /*!
      \class ExtensionTestData
      \brief Provides a vector of tuples. The first element of each 
      tuple is a header, the second is the first extension (according
      to the data in PrefixTestData) and the final is a boolean which 
      is true if and only if the extension is unique, and false
      otherwise.
      \todo Again the problem with the static data members!
    */

    class ExtensionTestData : BaseTestData {
     
      typedef boost::tuple<string_type, bool, string_type, bool> tuple_type;
      typedef std::vector<tuple_type> extension_vector_type;

    public:

      typedef extension_vector_type::const_iterator const_iterator;
      static extension_vector_type extension_vector;
      static extension_vector_type extension_vector_2;

      ExtensionTestData() {

        using namespace boost::assign;
        typedef tuple_type el_t;

        extension_vector_2 +=
          el_t("AnalyseTotalAssignment.hpp",true,"AnalyseTotalAssignment.hpp/Autarkies/Search/OKsystem",true);

        extension_vector +=
          el_t("test001.hpp",true,"test001.hpp/dir001/",true),
          el_t("test002.hpp",true,"test002.hpp/dir001/dir002/",false),
          el_t("test003.hpp",false,"",true);

      }

      string_type header(const_iterator i) {
        return i -> get<0>();
      }

      bool has_extension(const_iterator i) {
        return i -> get<1>();
      }

      string_type extension_value(const_iterator i) {
        return i -> get<2>();
      }

      bool uniqueness(const_iterator i) {
        return i -> get<3>();
      }

    };

    ExtensionTestData::extension_vector_type ExtensionTestData::extension_vector;
    ExtensionTestData::extension_vector_type ExtensionTestData::extension_vector_2;

    // ##############################################################

    /*!
      \class Test_AssociativePrefixContainer
      \brief Test for associative prefix container
      \todo Replace old tests.
      \todo Test for extensions, where no extension is possible.
      \todo More test cases.
      \todo Design an informal concept.
      \todo Once a concept has been created, test the syntax.
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

      typedef typename BaseTestData::string_type string_type;

      /*!
        \fn test_assign_function() 
        \brief Tests for AssociativePrefixContainer assign member function.

        An AssociativePrefixContainer is initialized with the a vector of
        prefixes. Then one set is initialized with the same vector of 
        prefixes and another set is intialized from the prefix container.
        The equality of both sets is then tested.
      */

      void test_assign_function() {
        typedef PrefixTestData::prefix_vector_type vector_type;
        typedef AssociativePrefixContainer<string_type> prefix_container_type;

        const vector_type& prefix_vector(prefix_test_data.prefix_vector);      
        prefix_container_type prefix_container;
        prefix_container.assign(prefix_vector);

        std::set<string_type> expected_prefix_set(prefix_vector.begin(),prefix_vector.end());
        std::set<string_type> actual_prefix_set(prefix_container.begin(),prefix_container.end());

        if (not (expected_prefix_set == actual_prefix_set))
          OKLIB_THROW("not (expected_prefix_set == actual_prefix_set)");
      }

      // #############################

      /*!
        \fn test_begin_end() 
        \brief Tests for AssociativePrefixContainer begin and end member 
        functions.

        First the equality of begin and end of an empty prefix container 
        is tested. Then a prefix container is initalized from a vector 
        of prefixes and the inequality of begin and end is tested.
      */

      void test_begin_end() {
        typedef AssociativePrefixContainer<string_type> prefix_container_type;
        typedef PrefixTestData::prefix_vector_type vector_type;
        prefix_container_type prefix_container;
        const vector_type& prefix_vector(prefix_test_data.prefix_vector);

        if (not (prefix_container.begin() == prefix_container.end()))
          OKLIB_THROW("not (prefix_container.begin() == prefix_container.end())");
        prefix_container.assign(prefix_vector);
        if (prefix_container.begin() == prefix_container.end())
          OKLIB_THROW("(prefix_container.begin() == prefix_container.end())");

      }

      // #############################

      /*!
        \fn test_insert() 
        \brief Tests for AssociativePrefixContainer insert member
        function.

        One prefix container is initialized with a vector of prefixes,
        and another prefix container is intialized by iterating over
        the same vector of prefixes and calling the insert member on
        each element. Then two sets are initialized with the contents
        of the two prefix containers and the equality of both sets is
        checked.
      */

      void test_insert() {
        typedef AssociativePrefixContainer<string_type> container_type;
        typedef PrefixTestData::prefix_vector_type vector_type;
        typedef vector_type::const_iterator const_iterator;

        const vector_type& prefix_vector(prefix_test_data.prefix_vector);
        container_type prefix_container_1;
        container_type prefix_container_2;

        prefix_container_1.assign(prefix_vector);
        const const_iterator& end(prefix_vector.end());

        for(const_iterator begin(prefix_vector.begin()); begin != end; ++begin) {
          prefix_container_2.insert(*begin);
        }
        std::set<string_type> prefix_set_1(prefix_container_1.begin(),prefix_container_1.end());
        std::set<string_type> prefix_set_2(prefix_container_2.begin(),prefix_container_2.end());
        if (not (prefix_set_1 == prefix_set_2))
          OKLIB_THROW("not (prefix_set_1 == prefix_set_2)");

      }

      // #############################

      /*!
        \fn test_first_extension() 
        \brief Tests for AssociativePrefixContainer first_extension 
        member function.

        A vector of extension test data is iterated over. Each element
        consists of a header and an expected extended header. For each
        header the first_extension member function is called and the
        obtained extension is checked against the expected extended
        header.

        \todo Test iterators, not just values.
        \todo Test for extensions, where no extension is possible.
      */

      void test_first_extension() {
        typedef AssociativePrefixContainer<string_type> prefix_container_type;
        typedef ExtensionTestData::const_iterator iterator;
        typedef prefix_container_type::const_iterator prefix_iterator_type;

        prefix_container_type prefix_container(prefix_test_data.prefix_vector);
        const iterator& end(extension_test_data.extension_vector.end());

        for (iterator begin(extension_test_data.extension_vector.begin()); begin != end; ++begin) {
          string_type header(extension_test_data.header(begin));
          if (extension_test_data.has_extension(begin)) {
            string_type expected_extension(extension_test_data.extension_value(begin));
            prefix_iterator_type first_extension(prefix_container.first_extension(header));
            prefix_iterator_type end(prefix_container.end());
            if (first_extension == end) 
              OKLIB_THROW("first_extension == end"); 
            OKLIB_TEST_EQUAL(*first_extension,expected_extension);
          }
          else {
            if (not (prefix_container.first_extension(header) == prefix_container.end()))
              OKLIB_THROW("not (prefix_container.first_extension(header) == prefix_container.end())");
          }
        }
      }

      // #############################

      /*!
        \fn test_first_extension_uniqueness_checked() 
        \brief Tests for AssociativePrefixContainer first_extension_
        uniqueness_checked member function.

        A vector of extension test data is iterated over. Each element
        of the vector is a tuple <string_type,bool,string_type,bool>
        where the first string_type element is a header, the second is 
        true iff the header has an extension, the third element is
        an expected extended header and the fourth element is a bool
        which is true if and only if the expected extension is unique.
        For each header, if it has an extension, then the first_extension
        member function is called and the obtained extension is checked 
        against the expected extended header and the uniqueness is checked 
        against the expected uniqueness. Otherwise the first_extension
        function is called and the result is checked to see if the first
        element is a past_the_end iterator, and the second is true.

        \todo Test iterators, not just values.
        \todo Test for extensions, where no extension is possible.
        \todo Test for ambiguous extensions.
      */

      void test_first_extension_uniqueness_checked() {
        typedef AssociativePrefixContainer<string_type> prefix_container_type;
        typedef ExtensionTestData::const_iterator iterator;
        typedef prefix_container_type::const_iterator prefix_iterator_type;
        typedef prefix_container_type::checked_iterator_type checked_iterator_type;
        prefix_container_type prefix_container(prefix_test_data.prefix_vector);
        const iterator& end(extension_test_data.extension_vector.end());
        for (iterator begin(extension_test_data.extension_vector.begin()); begin != end; ++begin) {
          string_type header(extension_test_data.header(begin));
          if (extension_test_data.has_extension(begin)) {
            string_type expected_extension(extension_test_data.extension_value(begin));
            checked_iterator_type first_extension(prefix_container.first_extension_uniqueness_checked(header));
            prefix_iterator_type end(prefix_container.end());
            if (first_extension.first == end)
              OKLIB_THROW("first_extension.first == end");
            OKLIB_TEST_EQUAL(*(first_extension.first),expected_extension);
            OKLIB_TEST_EQUAL(first_extension.second,extension_test_data.uniqueness(begin));
          }
          else {
            checked_iterator_type first_extension(prefix_container.first_extension_uniqueness_checked(header));
            if (not (first_extension.first == prefix_container.end()))
              OKLIB_THROW("not (first_extension == prefix_container.end())");
            OKLIB_TEST_EQUAL(first_extension.second,extension_test_data.uniqueness(begin));
          }
        }
        
      }

      // ##############################################################

      void perform_test_trivial() {
        test_assign_function();
        test_begin_end();
        test_insert();
        test_first_extension();
        test_first_extension_uniqueness_checked();
      }
     
    };   
  }  
}

#endif
