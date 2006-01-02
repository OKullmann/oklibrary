// Oliver Kullmann, 10.10.2005 (Swansea)

/*!
  \file IOStreamFilters_Tests.hpp
  \brief Tests for iostream-filters.
*/

#ifndef IOSTREAMFILTERSTESTS_6tgBB

#define IOSTREAMFILTERSTESTS_6tgBB

#include <string>
#include <sstream>
#include <cassert>
#include <vector>
#include <algorithm>
#include <functional>

#include <boost/iostreams/filtering_stream.hpp>
#include <boost/ref.hpp>

#include "TestBaseClass.hpp"
#include "TestExceptions.hpp"

namespace OKlib {

  namespace GeneralInputOutput {

    /*!
      \class Test_BasicCounter
      \brief Testing a filter for counting characters and lines
    */

    template <template <typename Ch> class BasicCounter>
    class Test_BasicCounter : public ::OKlib::TestSystem::TestBase {
    public :
      typedef Test_BasicCounter test_type;
      Test_BasicCounter() {
        insert(this);
      }
    private :
      void perform_test_trivial() {
        {
          typedef std::vector<std::string> test_vector_type;
          test_vector_type test_vector;
          test_vector.push_back(""); test_vector.push_back("x"); test_vector.push_back("\n"); test_vector.push_back("abc\nxxx\n"); test_vector.push_back("\n\nxyz\nabc");
          typedef test_vector_type::const_iterator iterator;
          const iterator& end(test_vector.end());
          for (iterator i = test_vector.begin(); i != end; ++i)
            test_string(*i);
        }
      }
      void test_string(const std::string& s) const {
        typedef std::string::value_type char_type;
        typedef BasicCounter<char_type> basic_counter_type;
        basic_counter_type counter;
        std::stringstream ss(s);
        boost::iostreams::filtering_istream in;
        in.push(boost::ref(counter));
        in.push(ss);
        assert(in);
        char_type character;
        int newlines = 0, total_characters = 0, current_characters = 0;
        for (;;) {
          OKLIB_TEST_EQUAL(counter.lines(), newlines);
          OKLIB_TEST_EQUAL(counter.characters(), total_characters);
          OKLIB_TEST_EQUAL(counter.current_characters(), current_characters);
          character = in.get();
          if (not in)
            break;
          ++total_characters;
          if (character == '\n') {
            ++newlines; current_characters = 0;
          }
          else
            ++current_characters;
        }
        assert(total_characters >= 0);
        assert((unsigned int)total_characters == s.size());
      }
    };

  }

}

#endif
