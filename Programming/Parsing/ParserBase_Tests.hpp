// Oliver Kullmann, 24.5.2005 (Swansea)
/* Copyright 2005 - 2007, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Programming/Parsing/ParserBase_Tests.hpp
  \brief Tools for testing parsers
*/

#ifndef PARSERBASETESTS_jfbNb5y
#define PARSERBASETESTS_jfbNb5y

#include <boost/spirit/include/classic_core.hpp>
#include <boost/spirit/include/classic_position_iterator.hpp>
#include <boost/lexical_cast.hpp>

#include <OKlib/TestSystem/TestBaseClass.hpp>
#include <OKlib/TestSystem/TestExceptions.hpp>

namespace OKlib {
  namespace Parser {

    enum Matching_possibilities { match_full, match_not_full };

    // -----------------------------------------------------------------------------------------------------------------------------------

    /*!
      class Test_ParsingString
      \brief Subtest for testing whether a parser derived from OKlib::Parser::ParserBase parses successfully a string or not.
      \todo What is the future of this class?!
    */

    template <class Parser>
    class Test_ParsingString : public ::OKlib::TestSystem::Test {
      Parser& parser;
      const std::string test_string;
      const Matching_possibilities match;
    public :
      typedef Test_ParsingString test_type;
      Test_ParsingString(Parser& parser, const std::string& test_string, const Matching_possibilities match) : parser(parser), test_string(test_string), match(match) {}
    private :
      void perform_test_trivial() {
        switch (match) {
        case match_full:
          if (not boost::spirit::parse(test_string.c_str(), parser.parser()).full)
            OKLIB_THROW("String " + test_string + " was not accepted.");
            // ToDo: Using Messages
          break;
        case (match_not_full) :
          if (boost::spirit::parse(test_string.c_str(), parser.parser()).full) {
            OKLIB_THROW("String " + test_string + " was accepted.");
            // ToDo: Using Messages
            break;
          }
        }
      }
    };

    // -----------------------------------------------------------------------------------------------------------------------------------

    template <typename ParseIterator>
    class Test_ParsingResult_Positional : public ::OKlib::TestSystem::Test {
      // ToDo: ParseIterator must be a PositionIterator (in the boost::spirit sense).
      typedef boost::spirit::parse_info<ParseIterator> info_type;
      const info_type& info;
    public :
      typedef Test_ParsingResult_Positional test_type;
      Test_ParsingResult_Positional(const info_type& info) : info(info) {}
    private :
      void perform_test_trivial() {
        if (not info.full) {
          const ParseIterator it(info.stop);
          typedef boost::spirit::file_position position_type;
          const position_type pos(it.get_position());
          OKLIB_THROW("Parse error in file " + pos.file + " at line " + boost::lexical_cast<std::string>(pos.line) + " and column " +  boost::lexical_cast<std::string>(pos.column));
        }
      }
    };

  }

}
#endif
