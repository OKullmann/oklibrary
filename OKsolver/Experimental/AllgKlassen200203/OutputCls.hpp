// Oliver Kullmann, 12.4.2003 (Swansea)
/* Copyright 2003 - 2007, 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#ifndef OUTPUTKLMWAECHTER

#define OUTPUTKLMWAECHTER

#include <ostream>
#include <limits>
#include <algorithm>

// #include <iostream> // debugging

#include <OKlib/General/ErrorHandling.hpp>
#include <OKlib/General/StringHandling.hpp>

#include <OKlib/OKsolver/Experimental/AllgKlassen200203/Values.hpp>


namespace OutputCls {

  // -----------------------------------------------------
  // Interface for output of clause-sets as literal stream
  // -----------------------------------------------------

  struct Error_OutputCls : ErrorHandling::Error {
    Error_OutputCls(const std::string& what) : ErrorHandling::Error(what) {}
  };

  template <class Lit>
  class Output_literal_stream {
  public :
    typedef unsigned long int size_type;
    static const size_type max_size;

    Output_literal_stream(std::ostream& out) : out(out), c(0), l(0), min_cl(max_size), max_cl(0), finished(false) {}

    void new_clause() {
      if (++c == 1)
	first_clause();
      else {
	clause_end(); clause_separator();
	min_cl = std::min(clause_size, min_cl);
	max_cl = std::max(clause_size, max_cl);
      }
      clause_size = 0;
      clause_begin();
    }

    Output_literal_stream& add(Lit x) {
      if (++clause_size != 1)
	literal_separator();    
      out << x;
      ++l;
      return *this;
    }
    // using add(x) before the first new_clause() or after finish(),
    // as well as using new_clause() after finish() (without reopen())
    // results in unspecified behaviour
      
    void finish() {
      // meaning: everything has been written (last clause has been completed)
      if (not finished) {
	if (c == 0)
	  empty_clause_set();
	else {
	  clause_end();
	  min_cl = std::min(clause_size, min_cl);
	  max_cl = std::max(clause_size, max_cl);
	  last_clause();
	}
	
	finished = true;
      }
    }
    // Usage of finish() is not required, when the
    // Output_literal_stream-element is destructed.
    // finish() can be invoked several times

    bool is_finished() { return finished; }
    void reopen() { finished = false; }

    void comment_line(const std::string& s) {
      // only before all clauses
      comment_begin();
      if (s.empty())
	out << "\n";
      else
	out << " " << s << "\n";
    }

    size_type clause_number() const {return c;}
    size_type literal_occurrences() const {return l;}
    size_type min_clause_length() const {return min_cl;}
    size_type max_clause_length() const {return max_cl;}

    virtual ~Output_literal_stream() {
      finish();
    }
    
  private :
    size_type c, l, clause_size, min_cl, max_cl;
    bool finished;

  protected :
    std::ostream& out;
    virtual void first_clause() const = 0;
    virtual void last_clause() const = 0;
    virtual void clause_begin() const = 0;
    virtual void clause_end() const = 0;
    virtual void clause_separator() const = 0;
    virtual void literal_separator() const = 0;
    virtual void comment_begin() const = 0;
    virtual void empty_clause_set() const = 0;
  };
  template <class Lit>
  const typename Output_literal_stream<Lit>::size_type Output_literal_stream<Lit>::max_size(std::numeric_limits<size_type>::max());

  // --------------------------------------------
  // Derivation for output in Dimacs format
  // --------------------------------------------

  template <class Lit>
  class Dimacs_output_literal_stream : public Output_literal_stream<Lit> {
  public :
    
    Dimacs_output_literal_stream(
                                 std::ostream& out,
                                 typename Output_literal_stream<Lit>::size_type number_variables,
                                 typename Output_literal_stream<Lit>::size_type number_clauses) :
      Output_literal_stream<Lit>(out), par_n(number_variables), par_c(number_clauses) {}

    typename Output_literal_stream<Lit>::size_type parameter_n() const {
      return par_n;
    }
    typename Output_literal_stream<Lit>::size_type parameter_c() const {
      return par_c;
    }

    ~Dimacs_output_literal_stream() { Output_literal_stream<Lit>::finish(); }
    // ATTENTION: If a class is derived from Dimacs_output_literal_stream,
    // and clause_end() is overridden, then also the destructor must be
    // overriden (otherwise not the overriden version of clause_end()
    // is used).

  protected :
    void first_clause() const {
      parameter_line();
    }
    void last_clause() const { Output_literal_stream<Lit>::out << std::endl; }
    void clause_begin() const {};
    void clause_end() const { Output_literal_stream<Lit>::out << " 0"; }
    void clause_separator() const { Output_literal_stream<Lit>::out << "\n"; }
    void literal_separator() const { Output_literal_stream<Lit>::out << " "; }
    void comment_begin() const { Output_literal_stream<Lit>::out << "c"; }
    void empty_clause_set() const {
      parameter_line();
    }
  private :
    const typename Output_literal_stream<Lit>::size_type par_n;
    const typename Output_literal_stream<Lit>::size_type par_c;
    void parameter_line() const {
      Output_literal_stream<Lit>::out << "p cnf " << par_n << " " << par_c << "\n";
    }
  };

  typedef Dimacs_output_literal_stream<std::string> Dimacs_output_literal_stream_strings;

  // ----------------------------------------------------------------------
  // Adapter to the general interface for construction of a boolean formula
  // ----------------------------------------------------------------------

  class Adapter_Dimacs_Output {
  public :

    typedef Dimacs_output_literal_stream_strings::size_type size_type;

    Adapter_Dimacs_Output(std::ostream& out) : out(out), pout(0), n(0), c(0) {}
    ~Adapter_Dimacs_Output() { delete pout; }

    // before construction (setting capacities)
    void set_number_variables(size_type n_) { n = n_; }
    size_type get_number_variables() { return n; }
    void set_number_clauses(size_type c_) { c = c_; }
    size_type get_number_clauses() { return c; }
    void set_number_literal_occurrences(size_type) const {}
    void set_max_clause_length(size_type) const {}

    void begin_construction() {
      pout = new Dimacs_output_literal_stream_strings(out, n, c);
    }

    void comment_line(const std::string& s) {
      if (pout)
	pout -> comment_line(s);
    }
    void explanation(const std::string&) {}

    void new_clause() {
      if (pout)
	pout -> new_clause();
    }
    Adapter_Dimacs_Output& add(const std::string& name, Values::BooleanValues v) {
      if (pout)
	if (v == Values::pos)
	  pout -> add(name);
	else
	  pout -> add("-" + name);
      return *this;
    }
    void finish() {
      if (pout)
	pout -> finish();
    }
    void reopen() {
      if (pout)
	pout -> reopen();
    }
    bool is_finished() {
      if (pout)
	return pout -> is_finished();
    }

    size_type clause_number() const {
      if (pout)
	return pout -> clause_number();
    }
    size_type literal_occurrences() const {
      if (pout)
	return pout -> literal_occurrences();
    }
    size_type min_clause_length() const {
      if (pout)
	return pout -> min_clause_length();
    }
    size_type max_clause_length() const {
      if (pout)
	return pout -> max_clause_length();
    }

  private :
    std::ostream& out;
    Dimacs_output_literal_stream_strings* pout;
    size_type n, c;
    
  };
}

#endif

