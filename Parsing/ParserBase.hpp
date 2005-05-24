// Oliver Kullmann, 24.5.2005 (Swansea)

#ifndef PARSERBASE_9Hhn67u

#define PARSERBASE_9Hhn67u

#include <boost/spirit/core.hpp>

namespace OKlib {

  namespace Parser {

    typedef char CharT;
    typedef const CharT* ParseIterator;
    typedef boost::spirit::rule<> Rule;
    
    class ParserBase {
    public :
      const Rule& parser() const { return parser_; }
      virtual ~ParserBase() {}
    protected :
      Rule parser_;
    };

  }

}
#endif
