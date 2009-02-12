// Oliver Kullmann, 13.5.2005 (Swansea)
/* Copyright 2005 - 2007, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Competition/ParsingSingleResult.hpp
  \brief Parser classes for parsing parts of some result ("result elements"), and the parser
  classe template ParserResult for parsing complete results (respectively "random-results").
*/

#ifndef PARSINGSINGLERESULT_jzRtL77Yq1
#define PARSINGSINGLERESULT_jzRtL77Yq1

#include <string>
#include <stdexcept>

#include <boost/spirit/include/classic_core.hpp>
#include <boost/spirit/include/classic_file_iterator.hpp>
#include <boost/spirit/include/classic_position_iterator.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/lexical_cast.hpp>

#include <OKlib/Programming/Parsing/ParserBase.hpp>

#include <OKlib/Experimentation/Competition/SingleResult.hpp>

namespace OKlib {
  namespace SATCompetition {

    struct ParserError : std::runtime_error {
      ParserError(const std::string& message) : std::runtime_error(message) {}
    };

    // #######################################################

    template <class ResultElement, typename CharT = char, typename ParseIterator = const CharT*>
    class ParserResultElement;

    // ---------------------------------------------------------------------------------------------------------

    template <typename CharT, typename ParseIterator>
    class ParserResultElement<SuperSeries, CharT, ParseIterator> : public ::OKlib::Parser::ParserBase<CharT, ParseIterator> {
      SuperSeries& s;
      struct action {
        SuperSeries& s;
        action(SuperSeries& s) : s(s) {}
        void operator() (const ParseIterator begin, const ParseIterator end) const {
          s = SuperSeries(std::string(begin, end));
        }
      };
    public :
      ParserResultElement(SuperSeries& s) : s(s) {
        this -> parser_ = (+(boost::spirit::alnum_p | boost::spirit::ch_p('-')))[action(s)];
      }
    };

    template <typename CharT, typename ParseIterator>
    class ParserResultElement<RandomKSat, CharT, ParseIterator> : public ::OKlib::Parser::ParserBase<CharT, ParseIterator> {
      RandomKSat& s;
      typedef RandomKSat::natural_number_type natural_number_type;
      natural_number_type k;
      struct action1 {
        natural_number_type& k;
        action1(natural_number_type& k) : k(k) {}
        void operator() (natural_number_type k_) const {
          k = k_;
        }
      };
      struct action2 {
        natural_number_type& k;
        RandomKSat& s;
        action2(natural_number_type& k, RandomKSat& s) : k(k), s(s) {}
        void operator() (const ParseIterator begin, const ParseIterator end) const {
          s = RandomKSat(std::string(begin, end), k);
        }
      };
    public :
      ParserResultElement(RandomKSat& s) : s(s) {
        this -> parser_ = ((boost::spirit::uint_parser<natural_number_type, 10, 1, 2>())[action1(k)] >> boost::spirit::str_p("SAT"))[action2(k,s)];
      }
    };

    // ---------------------------------------------------------------------------------------------------------

    template <typename CharT, typename ParseIterator>
    class ParserResultElement<Series, CharT, ParseIterator> : public ::OKlib::Parser::ParserBase<CharT, ParseIterator> {
      Series& s;
      struct action {
        Series& s;
        action(Series& s) : s(s) {}
        void operator() (const ParseIterator begin, const ParseIterator end) const {
          s = Series(std::string(begin, end));
        }
      };
      typedef typename ::OKlib::Parser::ParserBase<CharT, ParseIterator>::Rule Rule;
      Rule filename;
    public :
      ParserResultElement(Series& s) : s(s) {
        filename = +(boost::spirit::alnum_p | boost::spirit::ch_p('-') | boost::spirit::ch_p('.') | boost::spirit::ch_p('_'));
        this -> parser_ = (+(filename >> boost::spirit::ch_p('/')) >> filename)[action(s)];
      }
    };

    template <typename CharT, typename ParseIterator>
    class ParserResultElement<RandomKSat_n, CharT, ParseIterator> : public ::OKlib::Parser::ParserBase<CharT, ParseIterator> {
      RandomKSat_n& s;
      typedef RandomKSat::natural_number_type natural_number_type;
      natural_number_type n;
      struct action1 {
        natural_number_type& n;
        action1(natural_number_type& n) : n(n) {}
        void operator() (const natural_number_type n_) const {
          n = n_;
        }
      };
      struct action2 {
        natural_number_type& n;
        RandomKSat_n& s;
        action2(natural_number_type& n, RandomKSat_n& s) : n(n), s(s) {}
        void operator() (const ParseIterator begin, const ParseIterator end) const {
          s = RandomKSat_n(std::string(begin, end), n);
        }
      };
      typedef typename ::OKlib::Parser::ParserBase<CharT, ParseIterator>::Rule Rule;
      Rule filename;
      Rule basename;
    public :
      ParserResultElement(RandomKSat_n& s) : s(s) {
        filename = +(boost::spirit::alnum_p | boost::spirit::ch_p('-'));
        basename = +(
                     boost::spirit::alnum_p  |
                     boost::spirit::ch_p('.') |
                     (boost::spirit::ch_p('-') >> ((boost::spirit::alnum_p - boost::spirit::ch_p('v')) | boost::spirit::ch_p('.') | boost::spirit::ch_p('-'))) |
                     (boost::spirit::str_p("-v") >> (boost::spirit::alpha_p | boost::spirit::ch_p('.') | boost::spirit::ch_p('-')) |
                     (boost::spirit::str_p("-v") >> boost::spirit::uint_p >>
                      (boost::spirit::alpha_p | boost::spirit::ch_p('.') | boost::spirit::ch_p('-'))))
                     )
          >> boost::spirit::str_p("-v") >> boost::spirit::uint_p[action1(n)];
        this -> parser_ = (+(filename >> boost::spirit::ch_p('/')) >> basename)[action2(n,s)];
        //ToDo: The following comment into the parsing documentation:
        // parser_ = *boost::spirit::ch_p('a') >> boost::spirit::str_p("ab"); //CAUTION: does not accept "ab"
      }
    };

    // ---------------------------------------------------------------------------------------------------------

    template <typename CharT, typename ParseIterator>
    class ParserResultElement<Benchmark, CharT, ParseIterator> : public ::OKlib::Parser::ParserBase<CharT, ParseIterator> {
      Benchmark& s;
      struct action {
        Benchmark& s;
        action(Benchmark& s) : s(s) {}
        void operator() (const ParseIterator begin, const ParseIterator end) const {
          s = Benchmark(std::string(begin, end));
        }
      };
    public :
      ParserResultElement(Benchmark& s) : s(s) {
        this -> parser_ = (boost::spirit::str_p("bench") >> boost::spirit::uint_p)[action(s)];
      }
    };

    // ---------------------------------------------------------------------------------------------------------

    template <typename CharT, typename ParseIterator>
    class ParserResultElement<Solver, CharT, ParseIterator> : public ::OKlib::Parser::ParserBase<CharT, ParseIterator> {
      Solver& s;
      struct action {
        Solver& s;
        action(Solver& s) : s(s) {}
        void operator() (const ParseIterator begin, const ParseIterator end) const {
          s = Solver(std::string(begin, end));
        }
      };
    public :
      ParserResultElement(Solver& s) : s(s) {
        this -> parser_ = (boost::spirit::str_p("solver") >> boost::spirit::uint_p)[action(s)];
      }
    };

    // ---------------------------------------------------------------------------------------------------------

    template <typename CharT, typename ParseIterator>
    class ParserResultElement<SATStatus, CharT, ParseIterator> : public ::OKlib::Parser::ParserBase<CharT, ParseIterator> {
      SATStatus& s;
      struct action {
        SATStatus& s;
        action(SATStatus& s) : s(s) {}
        void operator() (const unsigned int& code) const {
          switch (code) {
          case unknown :
            s = SATStatus(unknown); break;
          case sat :
            s = SATStatus(sat); break;
          case unsat :
            s = SATStatus(unsat); break;
          default :
            s = SATStatus(error);
          }
        }
      };
    public :
      ParserResultElement(SATStatus& s) : s(s) {
        this -> parser_ = boost::spirit::uint_p[action(s)];
      }
    };


    // ---------------------------------------------------------------------------------------------------------

    template <typename CharT, typename ParseIterator>
    class ParserResultElement<AverageTime, CharT, ParseIterator> : public ::OKlib::Parser::ParserBase<CharT, ParseIterator> {
      AverageTime& s;
      typedef AverageTime::floating_point_type floating_point_type;
      struct action {
        AverageTime& s;
        action(AverageTime& s) : s(s) {}
        void operator() (const floating_point_type x) const {
          s = AverageTime(x);
        }
      };
    public :
      ParserResultElement(AverageTime& s) : s(s) {
        this -> parser_ = boost::spirit::real_parser<floating_point_type, boost::spirit::ureal_parser_policies<floating_point_type> >()[action(s)];
      }
    };

    // ---------------------------------------------------------------------------------------------------------
    
    template <typename CharT, typename ParseIterator>
    class ParserResultElement<TimeOut, CharT, ParseIterator> : public ::OKlib::Parser::ParserBase<CharT, ParseIterator> {
      TimeOut& s;
      struct action {
        TimeOut& s;
        action(TimeOut& s) : s(s) {}
        typedef TimeOut::natural_number_type natural_number_type;
        void operator() (const natural_number_type x) const {
          s = TimeOut(x);
        }
      };
    public :
      ParserResultElement(TimeOut& s) : s(s) {
        this -> parser_ = boost::spirit::uint_p[action(s)];
      }
    };

    // #######################################################

    template <typename CharT, typename ParseIterator>
    struct ParserEmpty : public ::OKlib::Parser::ParserBase<CharT, ParseIterator> {
      ParserEmpty() {
        this -> parser_ = ! boost::spirit::nothing_p;
        // ToDo: Is this the best implementation of the "neutral parser" ?
      }
    };

     template <typename CharT, typename ParseIterator>
    struct ParserThreeElements : public ::OKlib::Parser::ParserBase<CharT, ParseIterator> {
      ParserThreeElements() {
        this -> parser_ = boost::spirit::ch_p(' ') >> (+boost::spirit::alpha_p) >> boost::spirit::uint_p >> boost::spirit::ch_p(' ') >> boost::spirit::uint_p >> boost::spirit::ch_p(' ') >> boost::spirit::uint_p;
      }
    };
   
    // #######################################################
    // #######################################################

    template <class Result, typename CharT, typename ParseIterator, class ParserExtension>
    class ParserResult;

    template <typename CharT, typename ParseIterator, class ParserExtension>
    class ParserResult<Result, CharT, ParseIterator, ParserExtension> : public ::OKlib::Parser::ParserBase<CharT, ParseIterator> {
      Result& r;
      ParserResultElement<SuperSeries, CharT, ParseIterator> p_sup_ser;
      ParserResultElement<Series, CharT, ParseIterator> p_ser;
      ParserResultElement<Benchmark, CharT, ParseIterator> p_bench;
      ParserResultElement<Solver, CharT, ParseIterator> p_solv;
      ParserResultElement<SATStatus, CharT, ParseIterator> p_sat_stat;
      ParserResultElement<AverageTime, CharT, ParseIterator> p_avg;
      ParserResultElement<TimeOut, CharT, ParseIterator> p_tmo;
      ParserExtension p_ext;
    public :
      typedef Result result_type;
      ParserResult(Result& r) : r(r), p_sup_ser(*r.sup_ser), p_ser(*r.ser), p_bench(*r.bench), p_solv(*r.solv), p_sat_stat(*r.sat_stat), p_avg(*r.avg), p_tmo(*r.tmo) {
        this -> parser_ = p_sup_ser.parser() >> boost::spirit::ch_p(' ') >> p_ser.parser() >> boost::spirit::ch_p(' ') >> p_bench.parser() >> boost::spirit::ch_p(' ') >> p_solv.parser() >> boost::spirit::ch_p(' ') >> p_sat_stat.parser() >> boost::spirit::ch_p(' ') >> p_avg.parser() >> boost::spirit::ch_p(' ') >> p_tmo.parser() >> p_ext.parser();
      }
    };

    template <typename CharT, typename ParseIterator, class ParserExtension>
    class ParserResult<ResultRandomSat, CharT, ParseIterator, ParserExtension> : public ::OKlib::Parser::ParserBase<CharT, ParseIterator> {
      ResultRandomSat& r;
      ParserResultElement<RandomKSat, CharT, ParseIterator> p_sup_ser;
      ParserResultElement<RandomKSat_n, CharT, ParseIterator> p_ser;
      ParserResultElement<Benchmark, CharT, ParseIterator> p_bench;
      ParserResultElement<Solver, CharT, ParseIterator> p_solv;
      ParserResultElement<SATStatus, CharT, ParseIterator> p_sat_stat;
      ParserResultElement<AverageTime, CharT, ParseIterator> p_avg;
      ParserResultElement<TimeOut, CharT, ParseIterator> p_tmo;
      ParserExtension p_ext;
    public :
      typedef ResultRandomSat result_type;
      ParserResult(ResultRandomSat& r) : r(r), p_sup_ser(*r.sup_ser), p_ser(*r.ser), p_bench(*r.bench), p_solv(*r.solv), p_sat_stat(*r.sat_stat), p_avg(*r.avg), p_tmo(*r.tmo) {
        this -> parser_ = p_sup_ser.parser() >> boost::spirit::ch_p(' ') >> p_ser.parser() >> boost::spirit::ch_p(' ') >> p_bench.parser() >> boost::spirit::ch_p(' ') >> p_solv.parser() >> boost::spirit::ch_p(' ') >> p_sat_stat.parser() >> boost::spirit::ch_p(' ') >> p_avg.parser() >> boost::spirit::ch_p(' ') >> p_tmo.parser() >> p_ext.parser();
      }
    };

  }

}

#endif
