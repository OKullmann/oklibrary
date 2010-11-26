// Tony Bao, 20.5.2005 (Swansea)
/* Copyright 2005 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file OKlib/Satisfiability/ProofSystems/DPv/Input_output.hpp
  \brief Deprecated. Input and output of cnf formulas in DIMACS format.
  \deprecated
  \todo The new implementations (see below) could be the starting point
  for a generic Dimacs input library (belongs to the module InputOutput)
*/

#ifndef INPUTOUTPUT_ah9s8ty332
#define INPUTOUTPUT_ah9s8ty332

#define BOOST_SPIRIT_USE_OLD_NAMESPACE

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <istream>
#include <iostream>
#include <cassert>
#include <utility>
#include <limits>

#include <boost/lexical_cast.hpp>
#include <boost/spirit/include/classic_core.hpp>

#include <OKlib/Programming/Parsing/ParserBase.hpp>

#include <OKlib/Satisfiability/ProofSystems/DPv/DPv.hpp>
#include <OKlib/Satisfiability/ProofSystems/DPv/BasicDataStructure.hpp>

namespace OKlib {
  namespace DPv {

    // Deprecated ****************************************************************************************
    
    namespace Input {

      struct FStream {

	FStream(const std::string& fname) : fname(fname) {
	  fopen();
	}  
	~FStream() {
	  fclose();
	}
	
	std::fstream filestream;
	
      private :
	
	std::string fname;
	void fopen() {
	  if (not filestream.is_open()) filestream.open(fname.c_str(), std::ios_base::in);
	  assert(filestream.is_open() == true);
	}

	void fclose() {
	  if (filestream.is_open()) filestream.close();
	  assert(filestream.is_open() == false);
	}
      }; 
    }

    // -------------------------------------------------------------------------------------------

    struct Dimacs {
      
      typedef OKlib::DPv::Clause_set::const_iterator const_iterator;
      OKlib::DPv::Clause_set formula;
      
      template <typename charT, typename traits> friend
      Dimacs& operator >>(std::basic_istream<charT, traits>& is, Dimacs& obj);
      
      friend Dimacs& operator >>(std::fstream& is, Dimacs& obj);
    };
    
    
	
    template <typename charT, typename traits>
    Dimacs& operator >>(std::basic_istream<charT, traits>& is, Dimacs& obj){
      return obj;
    }
    
    Dimacs& operator >>(std::fstream& is, Dimacs& obj){

      char x = is.peek();
      while (x == 'c' or x == 'p') {
        is.ignore(std::numeric_limits<int>::max(), '\n');
        x = is.peek();
      }
      
      OKlib::DPv::int_type j; 
      while (not is.eof()){
        is >> j;
        Clause cl;
        while (j != 0){
          Literal l(j);
          cl_insert(l,cl); 
	    is >> j;
        }
        //for (Clause::const_iterator i = cl.c.begin(); i!=cl.c.end();++i) {
        //std::cout << (*i).l << '\t';
        //}
        //std::cout << '\n';
        if (j == 0) is.ignore(1,'\n');
        if (not cl.c.size() == 0) cls_insert(cl, obj.formula);
      }
      return obj;
    }

    // -----------------------------------------------------------------------------------------------

    // New:

    // ToDo:
    // Error messages with error locations (needs then a generic ParseIterator in order to use spirit FileIterator as MultiPassIterator)
    // Generic components

    typedef char CharT;
    typedef const CharT* ParseIterator;
    typedef boost::spirit::rule<> Rule;

    class ParserLiteral : public ::OKlib::Parser::ParserBase<> {
      Literal& l;
      bool negative;
      struct action1 {
        bool& negative;
        action1(bool& negative) : negative(negative) {}
        void operator() (CharT) const {
          negative = true;
        }
      };
      struct action2 {
        Literal& l;
        bool& negative;
        action2(Literal& l, bool& negative) : l(l), negative(negative) {}
        void operator() (const int_type v) const {
          if (negative)
            l.l = - v;
          else
            l.l = v;
        }
      };
    public :
      ParserLiteral(Literal& l) : l(l), negative(false) {
        parser_ = !(boost::spirit::ch_p('-')[action1(negative)]) >> boost::spirit::uint_parser<int_type>()[action2(l, negative)] - (boost::spirit::ch_p('0') >> *boost::spirit::digit_p); //[action(l)];
      }
    };
    // ToDo: Check for overflow

    // --------------------------------------------------------------------
    template <class ClauseSet>
    class DimacsParser : public ::OKlib::Parser::ParserBase<> {
      ClauseSet& F;
      Literal& l;
      struct action1 {
        ClauseSet& F;
        action1(ClauseSet& F) : F(F) {}
        void operator() (const ParseIterator begin, const ParseIterator end) {
          F.comment(begin, end);
        }
      };
      struct action2 {
        ClauseSet& F;
        action2(ClauseSet& F) : F(F) {}
        void operator() (const int_type x) {
          F.maximal_index_variables(x);
        }
      };
      struct action3 {
        ClauseSet& F;
        action3(ClauseSet& F) : F(F) {}
        void operator() (const int_type x) {
          F.number_clauses(x);
        }
      };
      struct action4 {
        ClauseSet& F;
        const Literal& l;
        action4(ClauseSet& F, const Literal& l) : F(F), l(l) {}
        void operator() (const ParseIterator, const ParseIterator) {
          F.literal(l);
        }
      };
      struct action5 {
        ClauseSet& F;
        action5(ClauseSet& F) : F(F) {}
        void operator() (const ParseIterator, const ParseIterator) {
          F.end_of_clause();
        }
      };
    public :
      DimacsParser(ClauseSet& F) : F(F) {
        Rule comment = boost::spirit::str_p("c ") >> (*(boost::spirit::print_p - boost::spirit::eol_p))[action1(F)] >> boost::spirit::eol_p;
        Rule parameter = boost::spirit::str_p("p cnf ") >> *boost::spirit::blank_p >> boost::spirit::uint_parser<int_type>()[action2(F)] >> +boost::spirit::blank_p >> boost::spirit::uint_parser<int_type>()[action3(F)] >> boost::spirit::eol_p;
        Rule clause = (+(ParserLiteral(l).parser()[action4(F, l)] >> +boost::spirit::blank_p) >> boost::spirit::ch_p('0') >> +boost::spirit::space_p)[action5(F)]; // ToDo: clause on its own with parameter F like DimacsParser.
        // ToDo: comment, parameter, clause on its own with parameter F like DimacsParser.
        parser = *comment >> parameter >> +(boost::spirit::blank_p >> clause);
      }
    };

    template <typename T>
    T& operator <<(T& os, Dimacs& obj) {
      os << obj.formula;
      return os;
    }

    namespace Output {
      struct FStream {

	FStream(const std::string& fname) : fname(fname) {
	  fopen();
	}  
	~FStream() {
	  fclose();
	}
	
	std::fstream filestream;
	
      private :
	
	std::string fname;
	void fopen() {
	  if (not filestream.is_open()) filestream.open(fname.c_str(), std::ios_base::app);
	  assert(filestream.is_open() == true);
	}

	void fclose() {
	  if (filestream.is_open()) filestream.close();
	  assert(filestream.is_open() == false);
	}
      }; 

    }
  }
}
#endif
