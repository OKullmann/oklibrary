// Oliver Kullmann, 24.5.2005 (Swansea)

#ifndef PARSERBASETESTS_jfbNb5y

#define PARSERBASETESTS_jfbNb5y

#include "TestBaseClass.hpp"
#include "TestExceptions.hpp"

namespace OKlib {

  namespace Parser {

    enum Matching_possibilities { match_full, match_not_full, };

    template <class Parser>
    class Test_ParsingString : public ::OKlib::TestSystem::TestBase {
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


  }

}
#endif
