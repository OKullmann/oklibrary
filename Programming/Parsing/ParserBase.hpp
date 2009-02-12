// Oliver Kullmann, 24.5.2005 (Swansea)
/* Copyright 2005 - 2007, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Programming/Parsing/ParserBase.hpp
  \brief Base class for parsers in the Boost::Spirit framework
*/


#ifndef PARSERBASE_9Hhn67u
#define PARSERBASE_9Hhn67u

#include <boost/spirit/include/classic_core.hpp>

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
