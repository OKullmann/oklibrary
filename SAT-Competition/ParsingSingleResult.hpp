// Oliver Kullmann, 13.5.2005 (Swansea)

#ifndef PARSINGSINGLERESULT_jzRtL77Yq1

#define PARSINGSINGLERESULT_jzRtL77Yq1

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

 //    struct GrammarBase : boost::spirit::grammar<GrammarBase> {};

//     template <class ResultElement>
//     class GrammarResultElement;

//     template <>
//     class GrammarResultElement<SuperSeries> : public GrammarBase {
//       SuperSeries& s;
      
//     public :
//       GrammarResultElement(SuperSeries& s) : s(s) {}
//       template <typename ScannerT>
//       class definition {
//       public :
//         typedef boost::spirit::rule<ScannerT> rule_type;
//       private :
//         rule_type rule;
//       public :
//         definition(const GrammarResultElement&) {

//         }
//         const rule_type& start() const { return rule; }
//       };

//     };

  }

}

#endif
