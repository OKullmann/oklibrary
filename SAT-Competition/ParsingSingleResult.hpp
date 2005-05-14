// Oliver Kullmann, 13.5.2005 (Swansea)

#ifndef PARSINGSINGLERESULT_jzRtL77Yq1

#define PARSINGSINGLERESULT_jzRtL77Yq1

#include <string>

#include <boost/spirit/core.hpp>

#include "SingleResult.hpp"

namespace OKlib {

  namespace SATCompetition {

    typedef const char* ParseIterator;
    typedef boost::spirit::rule<> Rule;

    class ParserBase {
    public :
      const Rule& parser() const { return parser_; }
      virtual ~ParserBase() {}
    protected :
      Rule parser_;
    };

    template <class ResultElement>
    class ParserResultElement;

    template <>
    class ParserResultElement<SuperSeries> : public ParserBase {
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
        parser_ = (+boost::spirit::alnum_p)[action(s)];
      }
    };
   template <>
    class ParserResultElement<RandomKSat> : public ParserBase {
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
        parser_ = boost::spirit::uint_parser<NaturalNumber, 10, 1, 2>()[action1(k)] >> boost::spirit::str_p("SAT")[action2(k,s)];
      }
    };



  }

}

#endif
