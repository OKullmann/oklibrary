// Oliver Kullmann, 13.5.2005 (Swansea)
/* Copyright 2005 - 2007, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#ifndef PARSINGSINGLERESULTTESTS_k33wQQl
#define PARSINGSINGLERESULTTESTS_k33wQQl

#include <sstream>
#include <string>
#include <vector>
#include <utility>
#include <algorithm>
#include <iterator>
#include <list>
#include <fstream>

#include <boost/spirit/include/classic_core.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/spirit/include/classic_file_iterator.hpp>
#include <boost/spirit/include/classic_position_iterator.hpp>
#include <boost/filesystem/path.hpp>

#include <OKlib/TestSystem/TestBaseClass.hpp>
#include <OKlib/TestSystem/TestExceptions.hpp>

#include <OKlib/Programming/Parsing/ParserBase_Tests.hpp>

#include <OKlib/Experimentation/Competition/SingleResult.hpp>
#include <OKlib/Experimentation/Competition/ParsingSingleResult.hpp>

namespace OKlib {
  namespace SATCompetition {

    template <class SuperSeries>
    class Test_ParserResultElement_SuperSeries_positive_cases : public ::OKlib::TestSystem::Test {
      // ToDo: Also the parser should be a template (template) parameter
    public :
      typedef Test_ParserResultElement_SuperSeries_positive_cases test_type;
    private :
      void perform_test_trivial() {
        SuperSeries s;
        typedef ParserResultElement<SuperSeries> Parser;
        Parser p(s);
        {
          const std::string test = "abc123ABC";
          OKLIB_TESTTRIVIAL_RETHROW(::OKlib::Parser::Test_ParsingString<Parser>(p, test, ::OKlib::Parser::match_full));
          if(s.name() != test)
            OKLIB_THROW("Resulting name is " + s.name() + ", and not " + test);
        }
      }
    };
    template <class SuperSeries>
    class Test_ParserResultElement_SuperSeries_negative_cases : public ::OKlib::TestSystem::Test {
    public :
      typedef Test_ParserResultElement_SuperSeries_negative_cases test_type;
    private :
      void perform_test_trivial() {
        SuperSeries s;
        typedef ParserResultElement<SuperSeries> Parser;
        Parser p(s);
        {
          OKLIB_TESTTRIVIAL_RETHROW(::OKlib::Parser::Test_ParsingString<Parser>(p, "   123ABC456", ::OKlib::Parser::match_not_full));
          OKLIB_TESTTRIVIAL_RETHROW(::OKlib::Parser::Test_ParsingString<Parser>(p, "468xyz1i   ", ::OKlib::Parser::match_not_full));
          OKLIB_TESTTRIVIAL_RETHROW(::OKlib::Parser::Test_ParsingString<Parser>(p, "", ::OKlib::Parser::match_not_full));
          OKLIB_TESTTRIVIAL_RETHROW(::OKlib::Parser::Test_ParsingString<Parser>(p, "a%b", ::OKlib::Parser::match_not_full));
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
    class Test_ParserResultElement_RandomKSat_positive_cases : public ::OKlib::TestSystem::Test {
    public :
      typedef Test_ParserResultElement_RandomKSat_positive_cases test_type;
    private :
      void perform_test_trivial() {
        RandomKSat s;
        typedef ParserResultElement<RandomKSat> Parser;
        Parser p(s);
        {
          typedef typename RandomKSat::natural_number_type natural_number_type;
          typedef std::vector<natural_number_type> Test_vector;
          Test_vector tv;
          tv.push_back(3); tv.push_back(10);
          for (typename Test_vector::const_iterator i = tv.begin(); i != tv.end(); ++i) {
            const natural_number_type k = *i;
            const std::string k_string(boost::lexical_cast<std::string>(k));
            const std::string test = k_string + "SAT";
            OKLIB_TESTTRIVIAL_RETHROW(::OKlib::Parser::Test_ParsingString<Parser>(p, test, ::OKlib::Parser::match_full));
            if(s.name() != test)
              OKLIB_THROW("Resulting name is " + s.name() + ", and not " + test);
            if (s.clause_length() != k)
              OKLIB_THROW("Clause length is " + boost::lexical_cast<std::string>(s.clause_length()) + ", and not " + k_string);
          }
        }
      }
    };
    template <class RandomKSat>
    class Test_ParserResultElement_RandomKSat_negative_cases : public ::OKlib::TestSystem::Test {
    public :
      typedef Test_ParserResultElement_RandomKSat_negative_cases test_type;
    private :
      void perform_test_trivial() {
        RandomKSat s;
        typedef ParserResultElement<RandomKSat> Parser;
        Parser p(s);
        {
          OKLIB_TESTTRIVIAL_RETHROW(::OKlib::Parser::Test_ParsingString<Parser>(p, "SAT", ::OKlib::Parser::match_not_full));
          OKLIB_TESTTRIVIAL_RETHROW(::OKlib::Parser::Test_ParsingString<Parser>(p, "123SAT", ::OKlib::Parser::match_not_full));
          OKLIB_TESTTRIVIAL_RETHROW(::OKlib::Parser::Test_ParsingString<Parser>(p, "12SA", ::OKlib::Parser::match_not_full));
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
    class Test_ParserResultElement_Series_positive_cases : public ::OKlib::TestSystem::Test {
    public :
      typedef Test_ParserResultElement_Series_positive_cases test_type;
    private :
      void perform_test_trivial() {
        Series s;
        typedef ParserResultElement<Series> Parser;
        Parser p(s);
        {
          const std::string test = "09/y-i/A/xXyY1";
          OKLIB_TESTTRIVIAL_RETHROW(::OKlib::Parser::Test_ParsingString<Parser>(p, test, ::OKlib::Parser::match_full));
          if(s.name() != test)
            OKLIB_THROW("Resulting name is " + s.name() + ", and not " + test);
        }
      }
    };
    template <class Series>
    class Test_ParserResultElement_Series_negative_cases : public ::OKlib::TestSystem::Test {
    public :
      typedef Test_ParserResultElement_Series_negative_cases test_type;
    private :
      void perform_test_trivial() {
        Series s;
        typedef ParserResultElement<Series> Parser;
        Parser p(s);
        {
          OKLIB_TESTTRIVIAL_RETHROW(::OKlib::Parser::Test_ParsingString<Parser>(p, "   A/B", ::OKlib::Parser::match_not_full));
          OKLIB_TESTTRIVIAL_RETHROW(::OKlib::Parser::Test_ParsingString<Parser>(p, "x", ::OKlib::Parser::match_not_full));
          OKLIB_TESTTRIVIAL_RETHROW(::OKlib::Parser::Test_ParsingString<Parser>(p, "x/", ::OKlib::Parser::match_not_full));
          OKLIB_TESTTRIVIAL_RETHROW(::OKlib::Parser::Test_ParsingString<Parser>(p, "", ::OKlib::Parser::match_not_full));
          OKLIB_TESTTRIVIAL_RETHROW(::OKlib::Parser::Test_ParsingString<Parser>(p, "a%/b", ::OKlib::Parser::match_not_full));
          OKLIB_TESTTRIVIAL_RETHROW(::OKlib::Parser::Test_ParsingString<Parser>(p, "a/b ", ::OKlib::Parser::match_not_full));
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
    class Test_ParserResultElement_RandomKSat_n_positive_cases : public ::OKlib::TestSystem::Test {
    public :
      typedef Test_ParserResultElement_RandomKSat_n_positive_cases test_type;
    private :
      void perform_test_trivial() {
        RandomKSat_n s;
        typedef ParserResultElement<RandomKSat_n> Parser;
        Parser p(s);
        {
          typedef typename RandomKSat_n::natural_number_type natural_number_type;
          typedef std::pair<std::string, natural_number_type> Pair;
          typedef std::vector<Pair> Vector;
          Vector test_vector;
          test_vector.push_back(Pair("random/MediumSizeBenches/k3-r4.263-v", 300));
          test_vector.push_back(Pair("random/MediumSizeBenches/k3-v7-r4.263-v", 20));
          test_vector.push_back(Pair("random/MediumSizeBenches/k3-v7-r4.263---v", 20));
          for (typename Vector::const_iterator i = test_vector.begin(); i != test_vector.end(); ++i) {
            const natural_number_type n = i -> second;
            const std::string test_n =  boost::lexical_cast<std::string>(n);
            const std::string test = i -> first + test_n;
            OKLIB_TESTTRIVIAL_RETHROW(::OKlib::Parser::Test_ParsingString<Parser>(p, test, ::OKlib::Parser::match_full));
            if(s.name() != test)
              OKLIB_THROW("Resulting name is " + s.name() + ", and not " + test);
            if (s.count_variables() != n)
              OKLIB_THROW("Variables count is " + boost::lexical_cast<std::string>(s.count_variables()) + ", and not " + test_n);
          }
        }
      }
    };
    template <class RandomKSat_n>
    class Test_ParserResultElement_RandomKSat_n_negative_cases : public ::OKlib::TestSystem::Test {
    public :
      typedef Test_ParserResultElement_RandomKSat_n_negative_cases test_type;
    private :
      void perform_test_trivial() {
        RandomKSat_n s;
        typedef ParserResultElement<RandomKSat_n> Parser;
        Parser p(s);
        {
          OKLIB_TESTTRIVIAL_RETHROW(::OKlib::Parser::Test_ParsingString<Parser>(p, "A/B", ::OKlib::Parser::match_not_full));
          OKLIB_TESTTRIVIAL_RETHROW(::OKlib::Parser::Test_ParsingString<Parser>(p, "A/Bv", ::OKlib::Parser::match_not_full));
          OKLIB_TESTTRIVIAL_RETHROW(::OKlib::Parser::Test_ParsingString<Parser>(p, "X/-7", ::OKlib::Parser::match_not_full));
          OKLIB_TESTTRIVIAL_RETHROW(::OKlib::Parser::Test_ParsingString<Parser>(p, "hh0/-v", ::OKlib::Parser::match_not_full));
          OKLIB_TESTTRIVIAL_RETHROW(::OKlib::Parser::Test_ParsingString<Parser>(p, "a/b-vy", ::OKlib::Parser::match_not_full));
          OKLIB_TESTTRIVIAL_RETHROW(::OKlib::Parser::Test_ParsingString<Parser>(p, "a/b-v100 ", ::OKlib::Parser::match_not_full));
          OKLIB_TESTTRIVIAL_RETHROW(::OKlib::Parser::Test_ParsingString<Parser>(p, "random/MediumSizeBenches/k3-v7-r4.263--v300", ::OKlib::Parser::match_not_full));
          OKLIB_TESTTRIVIAL_RETHROW(::OKlib::Parser::Test_ParsingString<Parser>(p, "random/MediumSizeBenches/-v100-v300", ::OKlib::Parser::match_not_full));
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
    class Test_ParserResultElement_Benchmark_positive_cases : public ::OKlib::TestSystem::Test {
    public :
      typedef Test_ParserResultElement_Benchmark_positive_cases test_type;
    private :
      void perform_test_trivial() {
        Benchmark s;
        typedef ParserResultElement<Benchmark> Parser;
        Parser p(s);
        {
          const std::string test = "bench123";
          OKLIB_TESTTRIVIAL_RETHROW(::OKlib::Parser::Test_ParsingString<Parser>(p, test, ::OKlib::Parser::match_full));
          if(s.name() != test)
            OKLIB_THROW("Resulting name is " + s.name() + ", and not " + test);
        }
      }
    };
    template <class Benchmark>
    class Test_ParserResultElement_Benchmark_negative_cases : public ::OKlib::TestSystem::Test {
    public :
      typedef Test_ParserResultElement_Benchmark_negative_cases test_type;
    private :
      void perform_test_trivial() {
        Benchmark s;
        typedef ParserResultElement<Benchmark> Parser;
        Parser p(s);
        {
          OKLIB_TESTTRIVIAL_RETHROW(::OKlib::Parser::Test_ParsingString<Parser>(p, "   bench123", ::OKlib::Parser::match_not_full));
          OKLIB_TESTTRIVIAL_RETHROW(::OKlib::Parser::Test_ParsingString<Parser>(p, "bench123   ", ::OKlib::Parser::match_not_full));
          OKLIB_TESTTRIVIAL_RETHROW(::OKlib::Parser::Test_ParsingString<Parser>(p, "", ::OKlib::Parser::match_not_full));
          OKLIB_TESTTRIVIAL_RETHROW(::OKlib::Parser::Test_ParsingString<Parser>(p, "bench123l", ::OKlib::Parser::match_not_full));
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
    class Test_ParserResultElement_Solver_positive_cases : public ::OKlib::TestSystem::Test {
    public :
      typedef Test_ParserResultElement_Solver_positive_cases test_type;
    private :
      void perform_test_trivial() {
        Solver s;
        typedef ParserResultElement<Solver> Parser;
        Parser p(s);
        {
          const std::string test = "solver123";
          OKLIB_TESTTRIVIAL_RETHROW(::OKlib::Parser::Test_ParsingString<Parser>(p, test, ::OKlib::Parser::match_full));
          if(s.name() != test)
            OKLIB_THROW("Resulting name is " + s.name() + ", and not " + test);
        }
      }
    };
    template <class Solver>
    class Test_ParserResultElement_Solver_negative_cases : public ::OKlib::TestSystem::Test {
    public :
      typedef Test_ParserResultElement_Solver_negative_cases test_type;
    private :
      void perform_test_trivial() {
        Solver s;
        typedef ParserResultElement<Solver> Parser;
        Parser p(s);
        {
          OKLIB_TESTTRIVIAL_RETHROW(::OKlib::Parser::Test_ParsingString<Parser>(p, "   solver123", ::OKlib::Parser::match_not_full));
          OKLIB_TESTTRIVIAL_RETHROW(::OKlib::Parser::Test_ParsingString<Parser>(p, "solver123   ", ::OKlib::Parser::match_not_full));
          OKLIB_TESTTRIVIAL_RETHROW(::OKlib::Parser::Test_ParsingString<Parser>(p, "", ::OKlib::Parser::match_not_full));
          OKLIB_TESTTRIVIAL_RETHROW(::OKlib::Parser::Test_ParsingString<Parser>(p, "solver123l", ::OKlib::Parser::match_not_full));
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
    class Test_ParserResultElement_SATStatus_positive_cases : public ::OKlib::TestSystem::Test {
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
           test_vector.push_back(Pair("1", error));
          for (Vector::const_iterator i = test_vector.begin(); i != test_vector.end(); ++i) {
            
            const std::string test = i -> first;
            const SolverResult result = i -> second;
            OKLIB_TESTTRIVIAL_RETHROW(::OKlib::Parser::Test_ParsingString<Parser>(p, test, ::OKlib::Parser::match_full));
            OKLIB_TEST_EQUAL(s.result(), result);
          }
        }
      }
    };
    template <class SATStatus>
    class Test_ParserResultElement_SATStatus_negative_cases : public ::OKlib::TestSystem::Test {
    public :
      typedef Test_ParserResultElement_SATStatus_negative_cases test_type;
    private :
      void perform_test_trivial() {
        SATStatus s;
        typedef ParserResultElement<SATStatus> Parser;
        Parser p(s);
        {
          OKLIB_TESTTRIVIAL_RETHROW(::OKlib::Parser::Test_ParsingString<Parser>(p, "   0", ::OKlib::Parser::match_not_full));
          OKLIB_TESTTRIVIAL_RETHROW(::OKlib::Parser::Test_ParsingString<Parser>(p, "10   ", ::OKlib::Parser::match_not_full));
          OKLIB_TESTTRIVIAL_RETHROW(::OKlib::Parser::Test_ParsingString<Parser>(p, "", ::OKlib::Parser::match_not_full));
          OKLIB_TESTTRIVIAL_RETHROW(::OKlib::Parser::Test_ParsingString<Parser>(p, "x", ::OKlib::Parser::match_not_full));
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
    class Test_ParserResultElement_AverageTime_positive_cases : public ::OKlib::TestSystem::Test {
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
          test_vector.push_back("100");
          for (Vector::const_iterator i = test_vector.begin(); i != test_vector.end(); ++i) {
            const std::string test = *i;
            typedef typename AverageTime::floating_point_type floating_point_type;
            const floating_point_type average = boost::lexical_cast<floating_point_type>(test);
            OKLIB_TESTTRIVIAL_RETHROW(::OKlib::Parser::Test_ParsingString<Parser>(p, test, ::OKlib::Parser::match_full));
            if(s.average() != average)
              OKLIB_THROW("Average is " + boost::lexical_cast<std::string>(s.average()) + ", and not " + boost::lexical_cast<std::string>(average));
          }
        }
      }
    };
    template <class AverageTime>
    class Test_ParserResultElement_AverageTime_negative_cases : public ::OKlib::TestSystem::Test {
    public :
      typedef Test_ParserResultElement_AverageTime_negative_cases test_type;
    private :
      void perform_test_trivial() {
        AverageTime s;
        typedef ParserResultElement<AverageTime> Parser;
        Parser p(s);
        {
          OKLIB_TESTTRIVIAL_RETHROW(::OKlib::Parser::Test_ParsingString<Parser>(p, "   0.1", ::OKlib::Parser::match_not_full));
          OKLIB_TESTTRIVIAL_RETHROW(::OKlib::Parser::Test_ParsingString<Parser>(p, "10.0   ", ::OKlib::Parser::match_not_full));
          OKLIB_TESTTRIVIAL_RETHROW(::OKlib::Parser::Test_ParsingString<Parser>(p, "", ::OKlib::Parser::match_not_full));
          OKLIB_TESTTRIVIAL_RETHROW(::OKlib::Parser::Test_ParsingString<Parser>(p, "x", ::OKlib::Parser::match_not_full));
          OKLIB_TESTTRIVIAL_RETHROW(::OKlib::Parser::Test_ParsingString<Parser>(p, "+11.1", ::OKlib::Parser::match_not_full));
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
    class Test_ParserResultElement_TimeOut_positive_cases : public ::OKlib::TestSystem::Test {
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
            typedef typename TimeOut::natural_number_type natural_number_type;
            const natural_number_type time_out = boost::lexical_cast<natural_number_type>(test);
            OKLIB_TESTTRIVIAL_RETHROW(::OKlib::Parser::Test_ParsingString<Parser>(p, test, ::OKlib::Parser::match_full));
            if(s.time_out() != time_out)
              OKLIB_THROW("Time_Out is " + boost::lexical_cast<std::string>(s.time_out()) + ", and not " + boost::lexical_cast<std::string>(time_out));
          }
        }
      }
    };
    template <class TimeOut>
    class Test_ParserResultElement_TimeOut_negative_cases : public ::OKlib::TestSystem::Test {
    public :
      typedef Test_ParserResultElement_TimeOut_negative_cases test_type;
    private :
      void perform_test_trivial() {
        TimeOut s;
        typedef ParserResultElement<TimeOut> Parser;
        Parser p(s);
        {
          OKLIB_TESTTRIVIAL_RETHROW(::OKlib::Parser::Test_ParsingString<Parser>(p, "   0", ::OKlib::Parser::match_not_full));
          OKLIB_TESTTRIVIAL_RETHROW(::OKlib::Parser::Test_ParsingString<Parser>(p, "10   ", ::OKlib::Parser::match_not_full));
          OKLIB_TESTTRIVIAL_RETHROW(::OKlib::Parser::Test_ParsingString<Parser>(p, "", ::OKlib::Parser::match_not_full));
          OKLIB_TESTTRIVIAL_RETHROW(::OKlib::Parser::Test_ParsingString<Parser>(p, "x", ::OKlib::Parser::match_not_full));
          OKLIB_TESTTRIVIAL_RETHROW(::OKlib::Parser::Test_ParsingString<Parser>(p, "+11", ::OKlib::Parser::match_not_full));
          OKLIB_TESTTRIVIAL_RETHROW(::OKlib::Parser::Test_ParsingString<Parser>(p, "100.0", ::OKlib::Parser::match_not_full));
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

    // -----------------------------------------------------------------------------------------------------------------------------
    // -----------------------------------------------------------------------------------------------------------------------------

    template <class Container, class TupleType>
    struct Add_positive_result_tuples;

    template <class Container>
    struct Add_positive_result_tuples<Container, TupleResult> {
      void operator() (Container& C) {
        typedef ResultElement::floating_point_type floating_point_type;
        typedef ResultElement::natural_number_type natural_number_type;
        C.push_back(TupleResult(std::string("SAT04"), std::string("crafted/sat04/gomes03"), std::string("bench432"), std::string("solver1"), unknown, floating_point_type(1200), natural_number_type(1200)));
        C.push_back(TupleResult(std::string("zarpas-s"), std::string("industrial/zarpas05/01"), std::string("bench1353"), std::string("solver34"), sat, floating_point_type(409.69), natural_number_type(1200)));
        C.push_back(TupleResult(std::string("3SAT"), std::string("random/MediumSizeBenches/k3-r4.263-v300"), std::string("bench1902"), std::string("solver1"), unsat, floating_point_type(6.65), natural_number_type(1200)));
        C.push_back(TupleResult(std::string("3SAT"), std::string("random/MediumSizeBenches/k3-r4.263-v300"), std::string("bench1903"), std::string("solver5"), unsat, floating_point_type(7), natural_number_type(1300)));
      }
    };

    template <class Container>
    struct Add_positive_result_tuples<Container, TupleResultRandomSat> {
      void operator() (Container& C) {
        typedef ResultElement::floating_point_type floating_point_type;
        typedef ResultElement::natural_number_type natural_number_type;
        C.push_back(TupleResultRandomSat(RandomKSat("3SAT", 3), RandomKSat_n("random/MediumSizeBenches/k3-r4.263-v300", 300), std::string("bench1902"), std::string("solver1"), unsat, floating_point_type(6.65), natural_number_type(1200)));
      C.push_back(TupleResultRandomSat(RandomKSat("7SAT", 7), RandomKSat_n("random/LargeSizeBenches/k7-r85-v160", 160), std::string("bench1603"), std::string("solver16"), unknown, floating_point_type(1196.88), natural_number_type(1200)));
      C.push_back(TupleResultRandomSat(RandomKSat("99SAT", 99), RandomKSat_n("crafted/sat04/gomes03/x-v11", 11), std::string("bench432"), std::string("solver1"), unknown, floating_point_type(1200), natural_number_type(1200)));
      C.push_back(TupleResultRandomSat(RandomKSat("3SAT", 3), RandomKSat_n("random/MediumSizeBenches/k3-r4.263-v300", 300), std::string("bench1902"), std::string("solver1"), unsat, floating_point_type(6.65), natural_number_type(1200)));
      }
    };

    template <class Container>
    inline void add_positive_result_tuples(Container& C) {
      Add_positive_result_tuples<Container, typename Container::value_type>()(C);
    }

    // -----------------------------------------------------------------------------------------------------------------------------
    // -----------------------------------------------------------------------------------------------------------------------------

    template <class Result>
    class Test_ParserResult_Result_positive_cases : public ::OKlib::TestSystem::Test {
    public :
      typedef Test_ParserResult_Result_positive_cases test_type;
    private :
      void perform_test_trivial() {
        Result s;
        typedef ParserResult<Result> Parser;
        Parser p(s);
        {
          typedef std::vector<TupleResult> Vector;
          Vector test_vector;
          add_positive_result_tuples(test_vector);
           for (Vector::const_iterator i = test_vector.begin(); i != test_vector.end(); ++i) {
            std::stringstream test_stream;
            test_stream << *i;
            const std::string test(test_stream.str());
            OKLIB_TESTTRIVIAL_RETHROW(::OKlib::Parser::Test_ParsingString<Parser>(p, test, ::OKlib::Parser::match_full));
            if (s.super_series().name() != i -> get<0>().name()) {
              std::stringstream out;
              out << "Super Series is " << s.super_series() << ", and not " << i -> get<0>();
              OKLIB_THROW(out.str());
            }
            if (s.series().name() != i -> get<1>().name()) {
              std::stringstream out;
              out << "Series is " << s.series() << ", and not " << i -> get<1>();
              OKLIB_THROW(out.str());
            }
            if (s.benchmark().name() != i -> get<2>().name()) {
              std::stringstream out;
              out << "Benchmark is " << s.benchmark() << ", and not " << i -> get<2>();
              OKLIB_THROW(out.str());
            }
            if (s.solver().name() != i -> get<3>().name()) {
              std::stringstream out;
              out << "Solver is " << s.solver() << ", and not " << i -> get<3>();
              OKLIB_THROW(out.str());
            }
            if (s.sat_status().result() != i -> get<4>().result()) {
              std::stringstream out;
              out << "Sat_Status is " << s.sat_status() << ", and not " << i -> get<4>();
              OKLIB_THROW(out.str());
            }
            if (s.average().average() != i -> get<5>().average()) {
              std::stringstream out;
              out << "Average is " << s.average() << ", and not " << i -> get<5>();
              OKLIB_THROW(out.str());
            }
            if (s.time_out().time_out() != i -> get<6>().time_out()) {
              std::stringstream out;
              out << "Time_Out is " << s.time_out() << ", and not " << i -> get<6>();
              OKLIB_THROW(out.str());
            }
            if (s != *i) {
              std::stringstream out;
              out << "Result is \"" << s <<"\", and not \"" << *i << "\"";
              OKLIB_THROW(out.str());
            }
          }
        }
      }
    };
    template <class Result>
    class Test_ParserResult_Result_negative_cases : public ::OKlib::TestSystem::Test {
    public :
      typedef Test_ParserResult_Result_negative_cases test_type;
    private :
      void perform_test_trivial() {
        Result s;
        typedef ParserResult<Result> Parser;
        Parser p(s);
        {
          OKLIB_TESTTRIVIAL_RETHROW(::OKlib::Parser::Test_ParsingString<Parser>(p, " 3SAT random/MediumSizeBenches/k3-r4.263-v300 bench1903 solver5 0 1200 1200", ::OKlib::Parser::match_not_full));
          OKLIB_TESTTRIVIAL_RETHROW(::OKlib::Parser::Test_ParsingString<Parser>(p, "3SAT random/MediumSizeBenches/k3-r4.263-v300 bench1903 solver5 0 1200 1200 ", ::OKlib::Parser::match_not_full));
          OKLIB_TESTTRIVIAL_RETHROW(::OKlib::Parser::Test_ParsingString<Parser>(p, "3SAT random/MediumSizeBenches/k3-r4.263-v300 bench1903 solver5 0 1200", ::OKlib::Parser::match_not_full));
          OKLIB_TESTTRIVIAL_RETHROW(::OKlib::Parser::Test_ParsingString<Parser>(p, "random/MediumSizeBenches/k3-r4.263-v300 bench1903 solver5 0 1200 1200", ::OKlib::Parser::match_not_full));
          OKLIB_TESTTRIVIAL_RETHROW(::OKlib::Parser::Test_ParsingString<Parser>(p, "3SAT random/MediumSizeBenches/k3-r4.263-v300 bench1903 0 1200 1200+11", ::OKlib::Parser::match_not_full));
        }
       }
    };

    template <class Result>
    class Test_ParserResult_Result : public ::OKlib::TestSystem::TestBase {
    public :
      typedef Test_ParserResult_Result test_type;
      Test_ParserResult_Result() {
        insert(this);
      }
    private :
      void perform_test_trivial() {
        OKLIB_TESTTRIVIAL_RETHROW(Test_ParserResult_Result_positive_cases<Result>());
        OKLIB_TESTTRIVIAL_RETHROW(Test_ParserResult_Result_negative_cases<Result>());
       }
    };

    // -----------------------------------------------------------------------------------------------------------------------------

    template <class ResultRandomSat>
    class Test_ParserResult_ResultRandomSat_positive_cases : public ::OKlib::TestSystem::Test {
    public :
      typedef Test_ParserResult_ResultRandomSat_positive_cases test_type;
    private :
      void perform_test_trivial() {
        ResultRandomSat s;
        typedef ParserResult<ResultRandomSat> Parser;
        Parser p(s);
        {
          typedef std::vector<TupleResultRandomSat> Vector;
          Vector test_vector;
          add_positive_result_tuples(test_vector);
          for (Vector::const_iterator i = test_vector.begin(); i != test_vector.end(); ++i) {
            std::stringstream test_stream;
            test_stream << *i;
            const std::string test(test_stream.str());
            OKLIB_TESTTRIVIAL_RETHROW(::OKlib::Parser::Test_ParsingString<Parser>(p, test, ::OKlib::Parser::match_full));
            if (s.super_series().name() != i -> get<0>().name()) {
              std::stringstream out;
              out << "Super Series is " << s.super_series() << ", and not " << i -> get<0>();
              OKLIB_THROW(out.str());
            }
            if (s.super_series_random().clause_length() != i -> get<0>().clause_length()) {
              std::stringstream out;
              out << "Super Series Random is " << s.super_series_random() << " with clause-length " << s.super_series_random().clause_length() << ", and not " << i -> get<0>() << " with clause-length " << i -> get<0>().clause_length();
              OKLIB_THROW(out.str());
            }
            if (s.series().name() != i -> get<1>().name()) {
              std::stringstream out;
              out << "Series is " << s.series() << ", and not " << i -> get<1>();
              OKLIB_THROW(out.str());
            }
            if (s.series_random().count_variables() != i -> get<1>().count_variables()) {
              std::stringstream out;
              out << "Series Random is " << s.series_random() << " with variables count " << s.series_random().count_variables() << ", and not " << i -> get<1>() << " with variables count " << i -> get<1>().count_variables();
              OKLIB_THROW(out.str());
            }
            if (s.benchmark().name() != i -> get<2>().name()) {
              std::stringstream out;
              out << "Benchmark is " << s.benchmark() << ", and not " << i -> get<2>();
              OKLIB_THROW(out.str());
            }
            if (s.solver().name() != i -> get<3>().name()) {
              std::stringstream out;
              out << "Solver is " << s.solver() << ", and not " << i -> get<3>();
              OKLIB_THROW(out.str());
            }
            if (s.sat_status().result() != i -> get<4>().result()) {
              std::stringstream out;
              out << "Sat_Status is " << s.sat_status() << ", and not " << i -> get<4>();
              OKLIB_THROW(out.str());
            }
            if (s.average().average() != i -> get<5>().average()) {
              std::stringstream out;
              out << "Average is " << s.average() << ", and not " << i -> get<5>();
              OKLIB_THROW(out.str());
            }
            if (s.time_out().time_out() != i -> get<6>().time_out()) {
              std::stringstream out;
              out << "Time_Out is " << s.time_out() << ", and not " << i -> get<6>();
              OKLIB_THROW(out.str());
            }
          }
        }
      }
    };

    template <class ResultRandomSat>
    class Test_ParserResult_ResultRandomSat : public ::OKlib::TestSystem::TestBase {
    public :
      typedef Test_ParserResult_ResultRandomSat test_type;
      Test_ParserResult_ResultRandomSat() {
        insert(this);
      }
    private :
      void perform_test_trivial() {
        OKLIB_TESTTRIVIAL_RETHROW(Test_ParserResult_ResultRandomSat_positive_cases<ResultRandomSat>());
        OKLIB_TESTTRIVIAL_RETHROW(Test_ParserResult_Result_negative_cases<ResultRandomSat>());
       }
    };

    // ToDo: All generic and based on concepts!!

  }

}

#endif
