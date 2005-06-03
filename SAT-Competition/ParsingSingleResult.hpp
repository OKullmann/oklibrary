// Oliver Kullmann, 13.5.2005 (Swansea)

#ifndef PARSINGSINGLERESULT_jzRtL77Yq1

#define PARSINGSINGLERESULT_jzRtL77Yq1

#include <string>
#include <stdexcept>

#include <boost/spirit/core.hpp>
#include <boost/spirit/iterator/file_iterator.hpp>
#include <boost/spirit/iterator/position_iterator.hpp>
#include <boost/filesystem/path.hpp>

#include "ParserBase.hpp"

#include "SingleResult.hpp"

namespace OKlib {

  namespace SATCompetition {

    struct ParserError : std::runtime_error {
      ParserError(const std::string& message) : std::runtime_error(message) {}
    };

    // ---------------------------------------------------------------------------------------------------------

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
      NaturalNumber k;
      struct action1 {
        NaturalNumber& k;
        action1(NaturalNumber& k) : k(k) {}
        void operator() (NaturalNumber k_) const {
          k = k_;
        }
      };
      struct action2 {
        NaturalNumber& k;
        RandomKSat& s;
        action2(NaturalNumber& k, RandomKSat& s) : k(k), s(s) {}
        void operator() (const ParseIterator begin, const ParseIterator end) const {
          s = RandomKSat(std::string(begin, end), k);
        }
      };
    public :
      ParserResultElement(RandomKSat& s) : s(s) {
        this -> parser_ = ((boost::spirit::uint_parser<NaturalNumber, 10, 1, 2>())[action1(k)] >> boost::spirit::str_p("SAT"))[action2(k,s)];
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
      NaturalNumber n;
      struct action1 {
        NaturalNumber& n;
        action1(NaturalNumber& n) : n(n) {}
        void operator() (const NaturalNumber n_) const {
          n = n_;
        }
      };
      struct action2 {
        NaturalNumber& n;
        RandomKSat_n& s;
        action2(NaturalNumber& n, RandomKSat_n& s) : n(n), s(s) {}
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
        void operator() (const ParseIterator begin, const ParseIterator end) const {
          const std::string status(begin, end);
          if (status == "0")
            s = SATStatus(unknown);
          else if (status == "10")
            s = SATStatus(sat);
          else
            s = SATStatus(unsat);
        }
      };
    public :
      ParserResultElement(SATStatus& s) : s(s) {
        this -> parser_ = (boost::spirit::str_p("0") | boost::spirit::str_p("10") | boost::spirit::str_p("20"))[action(s)];
      }
    };

    // ---------------------------------------------------------------------------------------------------------

    template <typename CharT, typename ParseIterator>
    class ParserResultElement<AverageTime, CharT, ParseIterator> : public ::OKlib::Parser::ParserBase<CharT, ParseIterator> {
      AverageTime& s;
      struct action {
        AverageTime& s;
        action(AverageTime& s) : s(s) {}
        void operator() (const FloatingPoint x) const {
          s = AverageTime(x);
        }
      };
    public :
      ParserResultElement(AverageTime& s) : s(s) {
        this -> parser_ = boost::spirit::real_parser<FloatingPoint, boost::spirit::ureal_parser_policies<FloatingPoint> >()[action(s)];
      }
    };

    // ---------------------------------------------------------------------------------------------------------
    
    template <typename CharT, typename ParseIterator>
    class ParserResultElement<TimeOut, CharT, ParseIterator> : public ::OKlib::Parser::ParserBase<CharT, ParseIterator> {
      TimeOut& s;
      struct action {
        TimeOut& s;
        action(TimeOut& s) : s(s) {}
        void operator() (const NaturalNumber x) const {
          s = TimeOut(x);
        }
      };
    public :
      ParserResultElement(TimeOut& s) : s(s) {
        this -> parser_ = boost::spirit::uint_p[action(s)];
      }
    };

    // ---------------------------------------------------------------------------------------------------------
    // ---------------------------------------------------------------------------------------------------------

    template <class Result, typename CharT, typename ParseIterator>
    class ParserResult;

    template <typename CharT, typename ParseIterator>
    class ParserResult<Result, CharT, ParseIterator> : public ::OKlib::Parser::ParserBase<CharT, ParseIterator> {
      Result& r;
      ParserResultElement<SuperSeries, CharT, ParseIterator> p_sup_ser;
      ParserResultElement<Series, CharT, ParseIterator> p_ser;
      ParserResultElement<Benchmark, CharT, ParseIterator> p_bench;
      ParserResultElement<Solver, CharT, ParseIterator> p_solv;
      ParserResultElement<SATStatus, CharT, ParseIterator> p_sat_stat;
      ParserResultElement<AverageTime, CharT, ParseIterator> p_avg;
      ParserResultElement<TimeOut, CharT, ParseIterator> p_tmo;
    public :
      typedef Result result_type;
      ParserResult(Result& r) : r(r), p_sup_ser(*r.sup_ser), p_ser(*r.ser), p_bench(*r.bench), p_solv(*r.solv), p_sat_stat(*r.sat_stat), p_avg(*r.avg), p_tmo(*r.tmo) {
        this -> parser_ = p_sup_ser.parser() >> boost::spirit::ch_p(' ') >> p_ser.parser() >> boost::spirit::ch_p(' ') >> p_bench.parser() >> boost::spirit::ch_p(' ') >> p_solv.parser() >> boost::spirit::ch_p(' ') >> p_sat_stat.parser() >> boost::spirit::ch_p(' ') >> p_avg.parser() >> boost::spirit::ch_p(' ') >> p_tmo.parser();
      }
    };

    template <typename CharT, typename ParseIterator>
    class ParserResult<ResultRandomSat, CharT, ParseIterator> : public ::OKlib::Parser::ParserBase<CharT, ParseIterator> {
      ResultRandomSat& r;
      ParserResultElement<RandomKSat, CharT, ParseIterator> p_sup_ser;
      ParserResultElement<RandomKSat_n, CharT, ParseIterator> p_ser;
      ParserResultElement<Benchmark, CharT, ParseIterator> p_bench;
      ParserResultElement<Solver, CharT, ParseIterator> p_solv;
      ParserResultElement<SATStatus, CharT, ParseIterator> p_sat_stat;
      ParserResultElement<AverageTime, CharT, ParseIterator> p_avg;
      ParserResultElement<TimeOut, CharT, ParseIterator> p_tmo;
    public :
      typedef ResultRandomSat result_type;
      ParserResult(ResultRandomSat& r) : r(r), p_sup_ser(*r.sup_ser), p_ser(*r.ser), p_bench(*r.bench), p_solv(*r.solv), p_sat_stat(*r.sat_stat), p_avg(*r.avg), p_tmo(*r.tmo) {
        this -> parser_ = p_sup_ser.parser() >> boost::spirit::ch_p(' ') >> p_ser.parser() >> boost::spirit::ch_p(' ') >> p_bench.parser() >> boost::spirit::ch_p(' ') >> p_solv.parser() >> boost::spirit::ch_p(' ') >> p_sat_stat.parser() >> boost::spirit::ch_p(' ') >> p_avg.parser() >> boost::spirit::ch_p(' ') >> p_tmo.parser();
      }
    };

    // ---------------------------------------------------------------------------------------------------------
    // ---------------------------------------------------------------------------------------------------------

    template <class ParserResult, class OutputIterator>
    class ParserResultSequence : public ::OKlib::Parser::ParserBase<typename ParserResult::char_type, typename ParserResult::ParseIterator> {
      typedef typename ParserResult::ParseIterator ParseIterator;
      OutputIterator& begin;
      typedef typename ParserResult::result_type Result;
      Result r;
      ParserResult p;
      struct action {
        OutputIterator& begin;
        Result& r;
        action(OutputIterator& begin, Result& r) : begin(begin), r(r) {}
        void operator() (ParseIterator, ParseIterator) const {
          *begin++ = r;
        }
      };
    public :
      ParserResultSequence(OutputIterator& begin) : begin(begin), p(r) {
        this -> parser_ = +((p.parser() >> boost::spirit::eol_p)[action(begin, r)]);
      }
    };

    template <class ParserResult, class OutputIterator>
    // ToDo: Conceptualisation
    // ParserResult is a parser for results
    // OutputIterator::value_type is Result
    // ToDo: It seems better to have the two operators in different specialisations.
    // ToDo: Making functor classes (with corresponding type traits) out of it.
    struct Copy_results {

      typedef typename ParserResult::ParseIterator ParseIterator;
      typedef boost::spirit::parse_info<ParseIterator> parse_info_it;
      typedef typename ParserResult::char_type char_type;
      typedef boost::spirit::parse_info<const char_type*> parse_info_c;

      parse_info_it operator() (const ParseIterator begin_in, const ParseIterator end_in, OutputIterator begin_out) {
        ParserResultSequence<ParserResult, OutputIterator> p(begin_out);
        return boost::spirit::parse(begin_in, end_in, p.parser());
      }
      template <typename PIterator>
      parse_info_c operator() (const PIterator begin_in, OutputIterator begin_out) {
        ParserResultSequence<ParserResult, OutputIterator> p(begin_out);
        return boost::spirit::parse(begin_in, p.parser());
      }
      
    };

    template <class ParserResult, class OutputIterator>
    inline typename Copy_results<ParserResult, OutputIterator>::parse_info_it copy_results(const typename ParserResult::ParseIterator begin_in, const typename ParserResult::ParseIterator end_in, const OutputIterator begin_out) {
      return Copy_results<ParserResult, OutputIterator>()(begin_in, end_in, begin_out);
    }
    template <class ParserResult, typename PIterator, class OutputIterator>
    inline typename Copy_results<ParserResult, OutputIterator>::parse_info_c copy_results(const PIterator begin_in, const OutputIterator begin_out) {
      return Copy_results<ParserResult, OutputIterator>()(begin_in, begin_out);
    }

    // ----------------------------------------------

    template <template <typename Result, typename CharT, typename ParseIterator> class ParserResult, class OutputIterator>
    struct Copy_results_from_file {

      typedef char char_type; // ToDo: to generalise
      typedef boost::spirit::file_iterator<char_type> file_iterator;
      typedef boost::spirit::position_iterator<file_iterator> ParseIterator;
      typedef boost::spirit::parse_info<ParseIterator> parse_info_f;
      typedef ParserResult<Result, char_type, ParseIterator> Parser;

      parse_info_f operator() (const boost::filesystem::path& filename, OutputIterator begin_out) {
        const std::string native_filename(filename.native_file_string());
        file_iterator file_begin(native_filename.c_str());
        if (not file_begin)
          throw ParserError("Error when opening " + native_filename);
        const file_iterator file_end(file_begin.make_end());
        const ParseIterator parse_begin(file_begin, file_end, native_filename);
        const ParseIterator parse_end;
        return copy_results<Parser>(parse_begin, parse_end, begin_out);
      }

    };

    template <template <typename Result, typename CharT, typename ParseIterator> class ParserResult, class OutputIterator>
    inline typename Copy_results_from_file<ParserResult, OutputIterator>::parse_info_f copy_results_from_file(const boost::filesystem::path& filename, OutputIterator begin_out) {
      return Copy_results_from_file<ParserResult, OutputIterator>()(filename, begin_out);
    }

  }

}

#endif
