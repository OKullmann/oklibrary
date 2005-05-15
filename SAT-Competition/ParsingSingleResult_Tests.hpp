// Oliver Kullmann, 13.5.2005 (Swansea)

#ifndef PARSINGSINGLERESULTTESTS_k33wQQl

#define PARSINGSINGLERESULTTESTS_k33wQQl

#include <sstream>
#include <string>
#include <vector>

#include <boost/spirit/core.hpp>
#include <boost/lexical_cast.hpp>

#include "TestBaseClass.hpp"
#include "TestExceptions.hpp"

#include "SingleResult.hpp"
#include "ParsingSingleResult.hpp"

namespace OKlib {

  namespace SATCompetition {

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
            throw ::OKlib::TestSystem::TestException("String " + test_string + " was not accepted.").add(OKLIB_TESTDESCRIPTION);
            // ToDo: Using Messages
          break;
        case (match_not_full) :
          if (boost::spirit::parse(test_string.c_str(), parser.parser()).full) {
            throw ::OKlib::TestSystem::TestException("String " + test_string + " was accepted.").add(OKLIB_TESTDESCRIPTION);
            // ToDo: Using Messages
            break;
          }
        }
      }
    };

    // -----------------------------------------------------------------------------------------------------------------------------

    template <class SuperSeries>
    class Test_ParserResultElement_SuperSeries_positive_cases : public ::OKlib::TestSystem::TestBase {
    public :
      typedef Test_ParserResultElement_SuperSeries_positive_cases test_type;
    private :
      void perform_test_trivial() {
        SuperSeries s;
        typedef ParserResultElement<SuperSeries> Parser;
        Parser p(s);
        {
          const std::string test = "abc123ABC";
          OKLIB_TESTTRIVIAL_RETHROW(Test_ParsingString<Parser>(p, test, match_full));
          if(s.name() != test)
            throw ::OKlib::TestSystem::TestException("Resulting name is " + s.name() + ", and not " + test).add(OKLIB_TESTDESCRIPTION);
        }
      }
    };
    template <class SuperSeries>
    class Test_ParserResultElement_SuperSeries_negative_cases : public ::OKlib::TestSystem::TestBase {
    public :
      typedef Test_ParserResultElement_SuperSeries_negative_cases test_type;
    private :
      void perform_test_trivial() {
        SuperSeries s;
        typedef ParserResultElement<SuperSeries> Parser;
        Parser p(s);
        {
          OKLIB_TESTTRIVIAL_RETHROW(Test_ParsingString<Parser>(p, "   123ABC456", match_not_full));
          OKLIB_TESTTRIVIAL_RETHROW(Test_ParsingString<Parser>(p, "468xyz1i   ", match_not_full));
          OKLIB_TESTTRIVIAL_RETHROW(Test_ParsingString<Parser>(p, "", match_not_full));
          OKLIB_TESTTRIVIAL_RETHROW(Test_ParsingString<Parser>(p, "a%b", match_not_full));
        }
       }
    };


    template <class SuperSeries>
    class Test_ParserResultElement_SuperSeries : public ::OKlib::TestSystem::TestBase {
    public :
      typedef Test_ParserResultElement_SuperSeries test_type;
      Test_ParserResultElement_SuperSeries() {
        insert(this);
      }
    private :
      void perform_test_trivial() {
        OKLIB_TESTTRIVIAL_RETHROW(Test_ParserResultElement_SuperSeries_positive_cases<SuperSeries>());
        OKLIB_TESTTRIVIAL_RETHROW(Test_ParserResultElement_SuperSeries_negative_cases<SuperSeries>());
       }
    };

    // -----------------------------------------------------------------------------------------------------------------------------

    template <class RandomKSat>
    class Test_ParserResultElement_RandomKSat_positive_cases : public ::OKlib::TestSystem::TestBase {
    public :
      typedef Test_ParserResultElement_RandomKSat_positive_cases test_type;
    private :
      void perform_test_trivial() {
        RandomKSat s;
        typedef ParserResultElement<RandomKSat> Parser;
        Parser p(s);
        { // positive cases
          typedef std::vector<NaturalNumber> Test_vector;
          Test_vector tv;
          tv.push_back(3); tv.push_back(10);
          for (Test_vector::const_iterator i = tv.begin(); i != tv.end(); ++i) {
            const NaturalNumber k = *i;
            const std::string k_string(boost::lexical_cast<std::string>(k));
            const std::string test = k_string + "SAT";
            OKLIB_TESTTRIVIAL_RETHROW(Test_ParsingString<Parser>(p, test, match_full));
            if (s.clause_length() != k)
              throw ::OKlib::TestSystem::TestException("Clause length is " + boost::lexical_cast<std::string>(s.clause_length()) + ", and not " + k_string).add(OKLIB_TESTDESCRIPTION);
          }
        }
      }
    };
    template <class RandomKSat>
    class Test_ParserResultElement_RandomKSat_negative_cases : public ::OKlib::TestSystem::TestBase {
    public :
      typedef Test_ParserResultElement_RandomKSat_negative_cases test_type;
    private :
      void perform_test_trivial() {
        RandomKSat s;
        typedef ParserResultElement<RandomKSat> Parser;
        Parser p(s);
        { // negative cases
          OKLIB_TESTTRIVIAL_RETHROW(Test_ParsingString<Parser>(p, "SAT", match_not_full));
          OKLIB_TESTTRIVIAL_RETHROW(Test_ParsingString<Parser>(p, "123SAT", match_not_full));
          OKLIB_TESTTRIVIAL_RETHROW(Test_ParsingString<Parser>(p, "12SA", match_not_full));
        }
      }
    };

    template <class RandomKSat>
    class Test_ParserResultElement_RandomKSat : public ::OKlib::TestSystem::TestBase {
    public :
      typedef Test_ParserResultElement_RandomKSat test_type;
      Test_ParserResultElement_RandomKSat() {
	insert(this);
      }
    private :
      void perform_test_trivial() {
        OKLIB_TESTTRIVIAL_RETHROW(Test_ParserResultElement_SuperSeries_negative_cases<RandomKSat>());
        OKLIB_TESTTRIVIAL_RETHROW(Test_ParserResultElement_RandomKSat_positive_cases<RandomKSat>());
        OKLIB_TESTTRIVIAL_RETHROW(Test_ParserResultElement_RandomKSat_negative_cases<RandomKSat>());
      }
    };

    // -----------------------------------------------------------------------------------------------------------------------------


  }

}

#endif
