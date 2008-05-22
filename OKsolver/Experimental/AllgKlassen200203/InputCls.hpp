// Oliver Kullmann, 1.5.2003 (Swansea)
/* Copyright 2003 - 2007, 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#ifndef INPUTKLMWAECHTER

#define INPUTKLMWAECHTER

#include <string>
#include <vector>
#include <ios>
#include <algorithm>
#include <functional>
#include <istream>

#include <Transitional/General/ErrorHandling.hpp>
#include <Transitional/General/StringHandling.hpp>

#include <Transitional/OKsolver/Experimental/AllgKlassen200203/Values.hpp>

namespace InputCls {

  struct Error_InputCls : ErrorHandling::Error {
    Error_InputCls(const std::string& what) : ErrorHandling::Error(what) {}
  };
  struct ReadError : Error_InputCls {
    ReadError(const std::string& what) : Error_InputCls(what) {}
  };
  struct SyntaxError : Error_InputCls {
    SyntaxError(const std::string& what) : Error_InputCls(what) {}
  };
  struct MissingCnfError: Error_InputCls {
    MissingCnfError(const std::string& what) : Error_InputCls(what) {}
  };
  struct ParameterError : Error_InputCls {
    ParameterError(const std::string& what) : Error_InputCls(what) {}
  };
  struct AfterParameterError : Error_InputCls {
    AfterParameterError(const std::string& what) : Error_InputCls(what) {}
  };
  struct EmptyClauseError : Error_InputCls {
    EmptyClauseError(const std::string& what) : Error_InputCls(what) {}
  };
   struct IsolatedNegationError : Error_InputCls {
    IsolatedNegationError(const std::string& what) : Error_InputCls(what) {}
  };

  template <class Formula>
  void DIMACS_Input(std::istream& in, Formula& F) {

    // NOCH ZU TUN:
    // Verbesserte Fehlermeldungen:
    //  - mehrsprachig
    //  - Position im Eingabe-Strom (Zeilennummer, absolute Position)
    // Klaerung der DIMACS-Syntax

    // Formula : Concept Formula-Constructor
    // Assumption: Formula::Lit::Var can be constructed via strings as names

    // Syntax:
    // First lines with comments, starting with "c";
    // then a line "p cnf n c";
    // then the clauses, where variables are denoted by names, literals are separated by spaces, completed by "0";
    // names: non-empty sequence of characters without space symbols and not starting with "-" and not equal to "0"
    // literals: either a name or "-" immediately followed by a name

    typedef typename Formula::size_type size_type;
    typedef typename Formula::Lit Lit;
    typedef typename Lit::Var Var;

    using namespace std;

    streamsize nr_lines = 1;

    {
      vector<string> comment_lines;
      string token;
      // reading comments, until the parameter line is found
      for (;; ++nr_lines) {
	in >> token;
	if (not in)
	  throw ReadError("DIMACS_Input(std::istream& in, Formula& F) : line " + StringHandling::toString(nr_lines));
	if (token == "p") {
	  in >> token;
	  if (token == "cnf")
	    break;
	  else
	    throw MissingCnfError("DIMACS_Input(std::istream& in, Formula& F) : line " + StringHandling::toString(nr_lines) + ", parameter line has token \"" + token + "\" instead of \"cnf\"");
	}
	if (token[0] != 'c')
	  throw SyntaxError("DIMACS_Input(std::istream& in, Formula& F) : Non-comment line before parameter line; line " + StringHandling::toString(nr_lines) + ", first token \"" + token + "\"");
	string rest_of_line;
	getline(in, rest_of_line);
	comment_lines.push_back(token + rest_of_line);
      }
      
      {
	size_type n, c;
	in >> n >> c;
	if (not in)
	  throw ParameterError("DIMACS_Input(std::istream& in, Formula& F) : invalid parameter values; line " + StringHandling::toString(nr_lines));
	{
	  string rest_of_line;
	  getline(in, rest_of_line);
	  if (not rest_of_line.empty())
	    throw AfterParameterError("DIMACS_Input(std::istream& in, Formula& F) : line " + StringHandling::toString(nr_lines) + ", rest of line after parameter values is not empty, but is \"" + rest_of_line + "\"");
	}
	
	F.set_number_variables(n);
	F.set_number_clauses(c);
	F.begin_construction();
      }
      
      for (vector<string>::const_iterator i = comment_lines.begin(); i != comment_lines.end(); ++i)
	F.comment_line(*i);
    }

    {
      string literal;
      for (;;) { // clause loop

	in >> literal;
	if (in.eof()) goto finish;
	if (not in)
	  throw ReadError("Unexpected end of file at the beginning of a clause.");
	F.new_clause();
	if (literal == "0")
	  throw EmptyClauseError("");
	
	do { // literal loop
	  Values::BooleanValues sign = Values::pos;
	  if (literal[0] == '-') {
	    literal.erase(0,1);
	    if (literal.empty())
	      throw IsolatedNegationError("");
	    sign = Values::neg;
	  }
	  F.add(Lit(Var(literal), sign));
	  in >> literal;
	  if (not in)
	    throw ReadError("Unexpected end of file in the middle of a clause.");
	} while (literal != "0");
      }
    }

    finish: F.finish();
  }

  

}

namespace InputCls {

  // ----------------------------------------------------------------------
  // Adapter to the general interface for construction of a boolean formula
  // ----------------------------------------------------------------------

  template <class Clauseset>
  class Construct_Cls_dynamic {

  // Clauseset : Concept clause-set
  // Construct_Cls_dynamic : Concept Formula-Constructor

    Clauseset& F;
    typename Clauseset::Cl C;
    bool open_clause;
  public :
    typedef Clauseset Cls;
    typedef typename Cls::size_type size_type;
    typedef typename Cls::Cl Cl;
    typedef typename Cls::Lit Lit;
    typedef typename Cls::Var Var;

    Construct_Cls_dynamic(Cls& F) : F(F), open_clause(false) {}

    void set_number_variables(size_type) {}
    void set_number_clauses(size_type) {}
    void set_number_literal_occurrences(size_type) const {}
    void set_max_clause_length(size_type) const {}

    void begin_construction() {}
    void comment_line(const std::string&) {}
    void explanation(const std::string&) {}

    void new_clause() {
      if (open_clause) {
	F.insert(C);
	C.clear();
      }
      else
	open_clause = true;
    }
    void finish() {
      if (open_clause) {
	F.insert(C);
	C.clear();
	open_clause = false;
      }
    }
    bool is_finished() { return not open_clause; }
    ~Construct_Cls_dynamic() {
      finish();
    }

    Construct_Cls_dynamic& add(typename Cls::Lit x) {
      C.insert(x);
    }
  };

}

#endif
