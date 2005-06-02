// Oliver Kullmann, 24.5.2005 (Swansea)

#ifndef PARSERBASE_9Hhn67u

#define PARSERBASE_9Hhn67u

#include <boost/spirit/core.hpp>

namespace OKlib {

  namespace Parser {

    template <typename CharT = char, typename ParseIterator_ = const CharT*>
    class ParserBase {
    public :
      typedef CharT char_type;
      typedef ParseIterator_ ParseIterator; // ToDo: ParseIterator -> parse_iterator;
      typedef boost::spirit::scanner<ParseIterator> scanner_type;
      typedef boost::spirit::rule<scanner_type> Rule; // ToDo: ->  rule_type;

      const Rule& parser() const { return parser_; }
      virtual ~ParserBase() {}
    protected :
      Rule parser_;
    };

  }

}
#endif
