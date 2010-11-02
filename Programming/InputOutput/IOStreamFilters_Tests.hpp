// Oliver Kullmann, 10.10.2005 (Swansea)
/* Copyright 2005 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Programming/InputOutput/IOStreamFilters_Tests.hpp
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
#include <iterator>

#include <boost/iostreams/filtering_stream.hpp>
#include <boost/ref.hpp>
#include <boost/assign/std/vector.hpp>
#include <boost/assign/list_of.hpp>

#include <OKlib/TestSystem/TestBaseClass.hpp>
#include <OKlib/TestSystem/TestExceptions.hpp>

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

    // #######################################################

    /*!
      \class Test_BasicIndentLines
      \brief Testing a filter for indenting lines
    */

    template <template<typename Ch, typename Alloc = std::allocator<Ch> > class BasicIndentLines>
    class Test_BasicIndentLines : public ::OKlib::TestSystem::TestBase {
    public :
      typedef Test_BasicIndentLines test_type;
      Test_BasicIndentLines() {
        insert(this);
      }
    private :
      void perform_test_trivial() {
        {
          typedef std::string::value_type char_type;
          typedef BasicIndentLines<char_type> basic_indent_lines_type;
          typedef std::vector<std::string> line_collection_type;
          typedef std::vector<line_collection_type> test_vector_type;
          test_vector_type test_vector;
          using namespace boost::assign;
          test_vector +=
            list_of("a"),
            list_of("")("")(""),
            list_of(" ")(" ")(" "),
            list_of("abc")("xy z")(" ")("t y v");
          test_vector.push_back(line_collection_type());
          typedef typename basic_indent_lines_type::size_type size_type;
          const size_type max_indentation = 4;
          basic_indent_lines_type indent_lines;
          const char_type ch(indent_lines.character());
          for (size_type indentation = 0; indentation <= max_indentation;) {
            typedef test_vector_type::const_iterator iterator;
            const iterator& end(test_vector.end());
            for (iterator i(test_vector.begin()); i != end; ++i) {
              std::ostringstream ss;
              boost::iostreams::filtering_ostream out;
              out.push(indent_lines);
              out.push(ss);
              assert(out);
              typedef line_collection_type::const_iterator line_iterator;
              const line_iterator& end(i -> end());
              std::copy(i -> begin(), end, std::ostream_iterator<std::string>(out, "\n"));
              out.strict_sync();
              std::ostringstream ref;
              for (line_iterator j(i -> begin()); j != end; ++j) {
                ref << std::string(indentation, ch) + *j + "\n";
              }
              out.flush();
              OKLIB_TEST_EQUAL(ss.str(), ref.str());
            }
            indent_lines.indentation() = ++indentation;
          }
        }
      }
    };

  }
}

#endif
