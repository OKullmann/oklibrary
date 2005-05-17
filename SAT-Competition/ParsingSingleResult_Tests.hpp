// Oliver Kullmann, 13.5.2005 (Swansea)

#ifndef PARSINGSINGLERESULTTESTS_k33wQQl

#define PARSINGSINGLERESULTTESTS_k33wQQl

#include <sstream>
#include <string>
#include <vector>
#include <utility>

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
        {
          typedef std::vector<NaturalNumber> Test_vector;
          Test_vector tv;
          tv.push_back(3); tv.push_back(10);
          for (Test_vector::const_iterator i = tv.begin(); i != tv.end(); ++i) {
            const NaturalNumber k = *i;
            const std::string k_string(boost::lexical_cast<std::string>(k));
            const std::string test = k_string + "SAT";
            OKLIB_TESTTRIVIAL_RETHROW(Test_ParsingString<Parser>(p, test, match_full));
            if(s.name() != test)
            throw ::OKlib::TestSystem::TestException("Resulting name is " + s.name() + ", and not " + test).add(OKLIB_TESTDESCRIPTION);
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
        {
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

    template <class Series>
    class Test_ParserResultElement_Series_positive_cases : public ::OKlib::TestSystem::TestBase {
    public :
      typedef Test_ParserResultElement_Series_positive_cases test_type;
    private :
      void perform_test_trivial() {
        Series s;
        typedef ParserResultElement<Series> Parser;
        Parser p(s);
        {
          const std::string test = "09/y-i/A/xXyY1";
          OKLIB_TESTTRIVIAL_RETHROW(Test_ParsingString<Parser>(p, test, match_full));
          if(s.name() != test)
            throw ::OKlib::TestSystem::TestException("Resulting name is " + s.name() + ", and not " + test).add(OKLIB_TESTDESCRIPTION);
        }
      }
    };
    template <class Series>
    class Test_ParserResultElement_Series_negative_cases : public ::OKlib::TestSystem::TestBase {
    public :
      typedef Test_ParserResultElement_Series_negative_cases test_type;
    private :
      void perform_test_trivial() {
        Series s;
        typedef ParserResultElement<Series> Parser;
        Parser p(s);
        {
          OKLIB_TESTTRIVIAL_RETHROW(Test_ParsingString<Parser>(p, "   A/B", match_not_full));
          OKLIB_TESTTRIVIAL_RETHROW(Test_ParsingString<Parser>(p, "x", match_not_full));
          OKLIB_TESTTRIVIAL_RETHROW(Test_ParsingString<Parser>(p, "x/", match_not_full));
          OKLIB_TESTTRIVIAL_RETHROW(Test_ParsingString<Parser>(p, "", match_not_full));
          OKLIB_TESTTRIVIAL_RETHROW(Test_ParsingString<Parser>(p, "a%/b", match_not_full));
          OKLIB_TESTTRIVIAL_RETHROW(Test_ParsingString<Parser>(p, "a/b ", match_not_full));
        }
       }
    };

    template <class Series>
    class Test_ParserResultElement_Series : public ::OKlib::TestSystem::TestBase {
    public :
      typedef Test_ParserResultElement_Series test_type;
      Test_ParserResultElement_Series() {
        insert(this);
      }
    private :
      void perform_test_trivial() {
        OKLIB_TESTTRIVIAL_RETHROW(Test_ParserResultElement_Series_positive_cases<Series>());
        OKLIB_TESTTRIVIAL_RETHROW(Test_ParserResultElement_Series_negative_cases<Series>());
       }
    };

    // -----------------------------------------------------------------------------------------------------------------------------

    template <class RandomKSat_n>
    class Test_ParserResultElement_RandomKSat_n_positive_cases : public ::OKlib::TestSystem::TestBase {
    public :
      typedef Test_ParserResultElement_RandomKSat_n_positive_cases test_type;
    private :
      void perform_test_trivial() {
        RandomKSat_n s;
        typedef ParserResultElement<RandomKSat_n> Parser;
        Parser p(s);
        {
          typedef std::pair<std::string, NaturalNumber> Pair;
          typedef std::vector<Pair> Vector;
          Vector test_vector;
          test_vector.push_back(Pair("random/MediumSizeBenches/k3-r4.263-v", 300));
          test_vector.push_back(Pair("random/MediumSizeBenches/k3-v7-r4.263-v", 20));
          test_vector.push_back(Pair("random/MediumSizeBenches/k3-v7-r4.263---v", 20));
          for (Vector::const_iterator i = test_vector.begin(); i != test_vector.end(); ++i) {
            const NaturalNumber n = i -> second;
            const std::string test_n =  boost::lexical_cast<std::string>(n);
            const std::string test = i -> first + test_n;
            OKLIB_TESTTRIVIAL_RETHROW(Test_ParsingString<Parser>(p, test, match_full));
            if(s.name() != test)
              throw ::OKlib::TestSystem::TestException("Resulting name is " + s.name() + ", and not " + test).add(OKLIB_TESTDESCRIPTION);
            if (s.count_variables() != n)
              throw ::OKlib::TestSystem::TestException("Variables count is " + boost::lexical_cast<std::string>(s.count_variables()) + ", and not " + test_n).add(OKLIB_TESTDESCRIPTION);
          }
        }
      }
    };
    template <class RandomKSat_n>
    class Test_ParserResultElement_RandomKSat_n_negative_cases : public ::OKlib::TestSystem::TestBase {
    public :
      typedef Test_ParserResultElement_RandomKSat_n_negative_cases test_type;
    private :
      void perform_test_trivial() {
        RandomKSat_n s;
        typedef ParserResultElement<RandomKSat_n> Parser;
        Parser p(s);
        {
          OKLIB_TESTTRIVIAL_RETHROW(Test_ParsingString<Parser>(p, "A/B", match_not_full));
          OKLIB_TESTTRIVIAL_RETHROW(Test_ParsingString<Parser>(p, "A/Bv", match_not_full));
          OKLIB_TESTTRIVIAL_RETHROW(Test_ParsingString<Parser>(p, "X/-7", match_not_full));
          OKLIB_TESTTRIVIAL_RETHROW(Test_ParsingString<Parser>(p, "hh0/-v", match_not_full));
          OKLIB_TESTTRIVIAL_RETHROW(Test_ParsingString<Parser>(p, "a/b-vy", match_not_full));
          OKLIB_TESTTRIVIAL_RETHROW(Test_ParsingString<Parser>(p, "a/b-v100 ", match_not_full));
          OKLIB_TESTTRIVIAL_RETHROW(Test_ParsingString<Parser>(p, "random/MediumSizeBenches/k3-v7-r4.263--v300", match_not_full));
          OKLIB_TESTTRIVIAL_RETHROW(Test_ParsingString<Parser>(p, "random/MediumSizeBenches/-v100-v300", match_not_full));
        }
       }
    };

    template <class RandomKSat_n>
    class Test_ParserResultElement_RandomKSat_n : public ::OKlib::TestSystem::TestBase {
    public :
      typedef Test_ParserResultElement_RandomKSat_n test_type;
      Test_ParserResultElement_RandomKSat_n() {
        insert(this);
      }
    private :
      void perform_test_trivial() {
        OKLIB_TESTTRIVIAL_RETHROW(Test_ParserResultElement_RandomKSat_n_positive_cases<RandomKSat_n>());
        OKLIB_TESTTRIVIAL_RETHROW(Test_ParserResultElement_Series_negative_cases<RandomKSat_n>());
        OKLIB_TESTTRIVIAL_RETHROW(Test_ParserResultElement_RandomKSat_n_negative_cases<RandomKSat_n>());
       }
    };

    // -----------------------------------------------------------------------------------------------------------------------------

    template <class Benchmark>
    class Test_ParserResultElement_Benchmark_positive_cases : public ::OKlib::TestSystem::TestBase {
    public :
      typedef Test_ParserResultElement_Benchmark_positive_cases test_type;
    private :
      void perform_test_trivial() {
        Benchmark s;
        typedef ParserResultElement<Benchmark> Parser;
        Parser p(s);
        {
          const std::string test = "bench123";
          OKLIB_TESTTRIVIAL_RETHROW(Test_ParsingString<Parser>(p, test, match_full));
          if(s.name() != test)
            throw ::OKlib::TestSystem::TestException("Resulting name is " + s.name() + ", and not " + test).add(OKLIB_TESTDESCRIPTION);
        }
      }
    };
    template <class Benchmark>
    class Test_ParserResultElement_Benchmark_negative_cases : public ::OKlib::TestSystem::TestBase {
    public :
      typedef Test_ParserResultElement_Benchmark_negative_cases test_type;
    private :
      void perform_test_trivial() {
        Benchmark s;
        typedef ParserResultElement<Benchmark> Parser;
        Parser p(s);
        {
          OKLIB_TESTTRIVIAL_RETHROW(Test_ParsingString<Parser>(p, "   bench123", match_not_full));
          OKLIB_TESTTRIVIAL_RETHROW(Test_ParsingString<Parser>(p, "bench123   ", match_not_full));
          OKLIB_TESTTRIVIAL_RETHROW(Test_ParsingString<Parser>(p, "", match_not_full));
          OKLIB_TESTTRIVIAL_RETHROW(Test_ParsingString<Parser>(p, "bench123l", match_not_full));
        }
       }
    };

    template <class Benchmark>
    class Test_ParserResultElement_Benchmark : public ::OKlib::TestSystem::TestBase {
    public :
      typedef Test_ParserResultElement_Benchmark test_type;
      Test_ParserResultElement_Benchmark() {
        insert(this);
      }
    private :
      void perform_test_trivial() {
        OKLIB_TESTTRIVIAL_RETHROW(Test_ParserResultElement_Benchmark_positive_cases<Benchmark>());
        OKLIB_TESTTRIVIAL_RETHROW(Test_ParserResultElement_Benchmark_negative_cases<Benchmark>());
       }
    };

    // -----------------------------------------------------------------------------------------------------------------------------

    template <class Solver>
    class Test_ParserResultElement_Solver_positive_cases : public ::OKlib::TestSystem::TestBase {
    public :
      typedef Test_ParserResultElement_Solver_positive_cases test_type;
    private :
      void perform_test_trivial() {
        Solver s;
        typedef ParserResultElement<Solver> Parser;
        Parser p(s);
        {
          const std::string test = "solver123";
          OKLIB_TESTTRIVIAL_RETHROW(Test_ParsingString<Parser>(p, test, match_full));
          if(s.name() != test)
            throw ::OKlib::TestSystem::TestException("Resulting name is " + s.name() + ", and not " + test).add(OKLIB_TESTDESCRIPTION);
        }
      }
    };
    template <class Solver>
    class Test_ParserResultElement_Solver_negative_cases : public ::OKlib::TestSystem::TestBase {
    public :
      typedef Test_ParserResultElement_Solver_negative_cases test_type;
    private :
      void perform_test_trivial() {
        Solver s;
        typedef ParserResultElement<Solver> Parser;
        Parser p(s);
        {
          OKLIB_TESTTRIVIAL_RETHROW(Test_ParsingString<Parser>(p, "   solver123", match_not_full));
          OKLIB_TESTTRIVIAL_RETHROW(Test_ParsingString<Parser>(p, "solver123   ", match_not_full));
          OKLIB_TESTTRIVIAL_RETHROW(Test_ParsingString<Parser>(p, "", match_not_full));
          OKLIB_TESTTRIVIAL_RETHROW(Test_ParsingString<Parser>(p, "solver123l", match_not_full));
        }
       }
    };

    template <class Solver>
    class Test_ParserResultElement_Solver : public ::OKlib::TestSystem::TestBase {
    public :
      typedef Test_ParserResultElement_Solver test_type;
      Test_ParserResultElement_Solver() {
        insert(this);
      }
    private :
      void perform_test_trivial() {
        OKLIB_TESTTRIVIAL_RETHROW(Test_ParserResultElement_Solver_positive_cases<Solver>());
        OKLIB_TESTTRIVIAL_RETHROW(Test_ParserResultElement_Solver_negative_cases<Solver>());
       }
    };

    // -----------------------------------------------------------------------------------------------------------------------------

    template <class SATStatus>
    class Test_ParserResultElement_SATStatus_positive_cases : public ::OKlib::TestSystem::TestBase {
    public :
      typedef Test_ParserResultElement_SATStatus_positive_cases test_type;
    private :
      void perform_test_trivial() {
        SATStatus s;
        typedef ParserResultElement<SATStatus> Parser;
        Parser p(s);
        {
          typedef std::pair<std::string, SolverResult> Pair;
          typedef std::vector<Pair> Vector;
          Vector test_vector;
          test_vector.push_back(Pair("0", unknown));
          test_vector.push_back(Pair("10", sat));
          test_vector.push_back(Pair("20", unsat));
          for (Vector::const_iterator i = test_vector.begin(); i != test_vector.end(); ++i) {
            
            const std::string test = i -> first;
            const SolverResult result = i -> second;
            OKLIB_TESTTRIVIAL_RETHROW(Test_ParsingString<Parser>(p, test, match_full));
            if(s.result() != result)
              throw ::OKlib::TestSystem::TestException("Result is " + boost::lexical_cast<std::string>(s.result()) + ", and not " + boost::lexical_cast<std::string>(result)).add(OKLIB_TESTDESCRIPTION);
          }
        }
      }
    };
    template <class SATStatus>
    class Test_ParserResultElement_SATStatus_negative_cases : public ::OKlib::TestSystem::TestBase {
    public :
      typedef Test_ParserResultElement_SATStatus_negative_cases test_type;
    private :
      void perform_test_trivial() {
        SATStatus s;
        typedef ParserResultElement<SATStatus> Parser;
        Parser p(s);
        {
          OKLIB_TESTTRIVIAL_RETHROW(Test_ParsingString<Parser>(p, "   0", match_not_full));
          OKLIB_TESTTRIVIAL_RETHROW(Test_ParsingString<Parser>(p, "10   ", match_not_full));
          OKLIB_TESTTRIVIAL_RETHROW(Test_ParsingString<Parser>(p, "", match_not_full));
          OKLIB_TESTTRIVIAL_RETHROW(Test_ParsingString<Parser>(p, "1", match_not_full));
        }
       }
    };

    template <class SATStatus>
    class Test_ParserResultElement_SATStatus : public ::OKlib::TestSystem::TestBase {
    public :
      typedef Test_ParserResultElement_SATStatus test_type;
      Test_ParserResultElement_SATStatus() {
        insert(this);
      }
    private :
      void perform_test_trivial() {
        OKLIB_TESTTRIVIAL_RETHROW(Test_ParserResultElement_SATStatus_positive_cases<SATStatus>());
        OKLIB_TESTTRIVIAL_RETHROW(Test_ParserResultElement_SATStatus_negative_cases<SATStatus>());
       }
    };

    // -----------------------------------------------------------------------------------------------------------------------------

    template <class AverageTime>
    class Test_ParserResultElement_AverageTime_positive_cases : public ::OKlib::TestSystem::TestBase {
    public :
      typedef Test_ParserResultElement_AverageTime_positive_cases test_type;
    private :
      void perform_test_trivial() {
        AverageTime s;
        typedef ParserResultElement<AverageTime> Parser;
        Parser p(s);
        {
          typedef std::vector<std::string> Vector;
          Vector test_vector;
          test_vector.push_back("0.0");
          test_vector.push_back("0.1");
          test_vector.push_back("20.2");
          test_vector.push_back("10E3");
          for (Vector::const_iterator i = test_vector.begin(); i != test_vector.end(); ++i) {
            const std::string test = *i;
            const FloatingPoint average = boost::lexical_cast<FloatingPoint>(test);
            OKLIB_TESTTRIVIAL_RETHROW(Test_ParsingString<Parser>(p, test, match_full));
            if(s.average() != average)
              throw ::OKlib::TestSystem::TestException("Average is " + boost::lexical_cast<std::string>(s.average()) + ", and not " + boost::lexical_cast<std::string>(average)).add(OKLIB_TESTDESCRIPTION);
          }
        }
      }
    };
    template <class AverageTime>
    class Test_ParserResultElement_AverageTime_negative_cases : public ::OKlib::TestSystem::TestBase {
    public :
      typedef Test_ParserResultElement_AverageTime_negative_cases test_type;
    private :
      void perform_test_trivial() {
        AverageTime s;
        typedef ParserResultElement<AverageTime> Parser;
        Parser p(s);
        {
          OKLIB_TESTTRIVIAL_RETHROW(Test_ParsingString<Parser>(p, "   0.1", match_not_full));
          OKLIB_TESTTRIVIAL_RETHROW(Test_ParsingString<Parser>(p, "10.0   ", match_not_full));
          OKLIB_TESTTRIVIAL_RETHROW(Test_ParsingString<Parser>(p, "", match_not_full));
          OKLIB_TESTTRIVIAL_RETHROW(Test_ParsingString<Parser>(p, "x", match_not_full));
          OKLIB_TESTTRIVIAL_RETHROW(Test_ParsingString<Parser>(p, "+11.1", match_not_full));
          OKLIB_TESTTRIVIAL_RETHROW(Test_ParsingString<Parser>(p, "100", match_not_full));
        }
       }
    };

    template <class AverageTime>
    class Test_ParserResultElement_AverageTime : public ::OKlib::TestSystem::TestBase {
    public :
      typedef Test_ParserResultElement_AverageTime test_type;
      Test_ParserResultElement_AverageTime() {
        insert(this);
      }
    private :
      void perform_test_trivial() {
        OKLIB_TESTTRIVIAL_RETHROW(Test_ParserResultElement_AverageTime_positive_cases<AverageTime>());
        OKLIB_TESTTRIVIAL_RETHROW(Test_ParserResultElement_AverageTime_negative_cases<AverageTime>());
       }
    };

    // -----------------------------------------------------------------------------------------------------------------------------

    template <class TimeOut>
    class Test_ParserResultElement_TimeOut_positive_cases : public ::OKlib::TestSystem::TestBase {
    public :
      typedef Test_ParserResultElement_TimeOut_positive_cases test_type;
    private :
      void perform_test_trivial() {
        TimeOut s;
        typedef ParserResultElement<TimeOut> Parser;
        Parser p(s);
        {
          typedef std::vector<std::string> Vector;
          Vector test_vector;
          test_vector.push_back("0");
          test_vector.push_back("1");
          test_vector.push_back("1200");
          for (Vector::const_iterator i = test_vector.begin(); i != test_vector.end(); ++i) {
            const std::string test = *i;
            const NaturalNumber time_out = boost::lexical_cast<NaturalNumber>(test);
            OKLIB_TESTTRIVIAL_RETHROW(Test_ParsingString<Parser>(p, test, match_full));
            if(s.time_out() != time_out)
              throw ::OKlib::TestSystem::TestException("Time_Out is " + boost::lexical_cast<std::string>(s.time_out()) + ", and not " + boost::lexical_cast<std::string>(time_out)).add(OKLIB_TESTDESCRIPTION);
          }
        }
      }
    };
    template <class TimeOut>
    class Test_ParserResultElement_TimeOut_negative_cases : public ::OKlib::TestSystem::TestBase {
    public :
      typedef Test_ParserResultElement_TimeOut_negative_cases test_type;
    private :
      void perform_test_trivial() {
        TimeOut s;
        typedef ParserResultElement<TimeOut> Parser;
        Parser p(s);
        {
          OKLIB_TESTTRIVIAL_RETHROW(Test_ParsingString<Parser>(p, "   0", match_not_full));
          OKLIB_TESTTRIVIAL_RETHROW(Test_ParsingString<Parser>(p, "10   ", match_not_full));
          OKLIB_TESTTRIVIAL_RETHROW(Test_ParsingString<Parser>(p, "", match_not_full));
          OKLIB_TESTTRIVIAL_RETHROW(Test_ParsingString<Parser>(p, "x", match_not_full));
          OKLIB_TESTTRIVIAL_RETHROW(Test_ParsingString<Parser>(p, "+11", match_not_full));
          OKLIB_TESTTRIVIAL_RETHROW(Test_ParsingString<Parser>(p, "100.0", match_not_full));
        }
       }
    };

    template <class TimeOut>
    class Test_ParserResultElement_TimeOut : public ::OKlib::TestSystem::TestBase {
    public :
      typedef Test_ParserResultElement_TimeOut test_type;
      Test_ParserResultElement_TimeOut() {
        insert(this);
      }
    private :
      void perform_test_trivial() {
        OKLIB_TESTTRIVIAL_RETHROW(Test_ParserResultElement_TimeOut_positive_cases<TimeOut>());
        OKLIB_TESTTRIVIAL_RETHROW(Test_ParserResultElement_TimeOut_negative_cases<TimeOut>());
       }
    };

  }

}

#endif
