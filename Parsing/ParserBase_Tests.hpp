// Oliver Kullmann, 24.5.2005 (Swansea)

#ifndef PARSERBASETESTS_jfbNb5y

#define PARSERBASETESTS_jfbNb5y

#include <boost/spirit/core.hpp>
#include <boost/spirit/iterator/position_iterator.hpp>
#include <boost/lexical_cast.hpp>

#include "TestBaseClass.hpp"
#include "TestExceptions.hpp"

namespace OKlib {

  namespace Parser {

    enum Matching_possibilities { match_full, match_not_full, };

    // -----------------------------------------------------------------------------------------------------------------------------------

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
          typedef boost::spirit::file_position position_type;
          const ParseIterator it(info.stop);
          position_type pos(it.get_position());
          OKLIB_THROW("Parse error in file " + pos.file + " at line " + boost::lexical_cast<std::string>(pos.line) + " and column " +  boost::lexical_cast<std::string>(pos.column));
        }
      }
    };

  }

}
#endif
