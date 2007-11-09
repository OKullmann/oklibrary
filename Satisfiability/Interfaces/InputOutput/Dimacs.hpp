// Oliver Kullmann, 24.7.2005 (Swansea)
/* Copyright 2005 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Interfaces/InputOutput/Dimacs.hpp
  \brief %Tools for the input and output of cnf's in DIMACS format.


  \todo Consider the code-comments for checking.


  \todo What about writing a generic DIMACS parser, starting with
  ::OKlib::DPv::ParserLiteral and ::OKlib::DPv::DimacsParser
  (located in Input_output.hpp in module DPv) ?


  \todo Write extended Dimacs-parser for clause-sets with non-boolean variables:
  <ul>
   <li> the parameter line now is of the form
   \verbatim
p gcnf n c k
   \endverbatim
   where k specifies the set of possible values {0, ..., k-1} </li>
   <li> literals are of the form
   \verbatim
n,v
   \endverbatim
   where n as usual is the variable (number), while v in {0, ..., k-1}. </li>
   <li> if k=2, then also "+n", "-n" and "n" are literals (so that ordinary
   Dimacs inputs are accepted). </li>
  </ul>


  \todo Write higher level modules like
  ReadClauseCollection<DimacsParser, ClauseCollection> and
  WriteClauseCollection<ClauseCollection, DimacsWriter>
  (likely this should go into a separate file).

*/


#ifndef DIMACS_jahsjdjd010
#define DIMACS_jahsjdjd010

#include <istream>
#include <cassert>
#include <string>
#include <set>
#include <sstream>
#include <cstdlib>

#include <boost/iostreams/filtering_stream.hpp>
#include <boost/ref.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <boost/lexical_cast.hpp>

#include <Transitional/GeneralInputOutput/IOStreamFilters.hpp>

#include <Transitional/Satisfiability/Interfaces/InputOutput/Exceptions.hpp>

namespace OKlib {
  namespace InputOutput {

    /*!
      \class StandardDIMACSInput
      \brief Parsing an input stream containing a cnf formula in DIMACS format and transferring it to a CLS-adaptor.


      \todo Add a policy L which allows to handle extended Dimacs format.
      <ul>
       <li> %Variables here are strings with identifier-syntax. </li>
       <li> The policy L manages a maps from identifiers to natural numbers and
       back. </li>
       <li> L is used exactly once, in member function read_clauses, replacing
       \code in >> literal; \endcode </li>
       <li> The main function is
       \code L::operator()(std::istream& in, int_type& literal); \endcode
       which reads the literal from the stream. </li>
       <li> The "strict policy" emplements this bracket-operator just as
       \code in >> literal; \endcode
       (without anything further). </li>
       <li> The "extended policy" contains
        <ol>
         <li> a map m, which assign to every occurring variable string its
         index; </li>
         <li> a counter n for the number of variables; </li>
         <li> a vector, which for each variable index i contains an iterator
         to the corresponding map entry. </li>
        </ol>
       </li>
      </ul>


      \todo Complete the doxygen-documentation.


      \todo The constructor should take the following optional arguments:
      <ul>
       <li> comments require at least one space after "c" (default: no) </li>
       <li> in the parameter line after "p" either exactly one or at least one
       space is required (default: exactly one) </li>
       <li> non-space characters on the parameter line after the second
       parameter lead to an error (default: yes). </li>
      </ul>


      \todo What happens if the integers from the file are too big?
      <ul>
       <li> We cannot use the stream extractors, since they yield undefined
       behaviour, so it seems necessary to write special
       tools to read and check integers from streams. </li>
       <li> (See my e-mail to Boost from 15/10/2005.) Perhaps the best
       solution here is to wrap int_type into a type with safe reading from
       istreams (setting the stream state accordingly). Or, perhaps better from
       a general design point of view, we should use a BigInteger class here for
       reading. </li>
      </ul>


      \todo It must also be tested, whether the integers can be safely negated.


      \todo The exception safety level must be specified.


      \todo Use Messages for messages.


      \todo For throwing the exceptions a more structured approach should be used
      (so that the exceptions thrown become better testable).

    */

    template <class CLSAdaptor, typename Int = int>
    class StandardDIMACSInput {
      boost::iostreams::filtering_istream in;
      CLSAdaptor& out;
      OKlib::GeneralInputOutput::Counter counter;
      char peek;
      Int n, c;

      StandardDIMACSInput(const StandardDIMACSInput&);
      StandardDIMACSInput& operator =(const StandardDIMACSInput&);

    public :
      typedef CLSAdaptor cls_adaptor_type;
      typedef Int int_type;
      typedef typename cls_adaptor_type::int_type int_type_target;

      StandardDIMACSInput(std::istream& in_stream, cls_adaptor_type& out) : out(out) {
        if (not in_stream)
          throw IStreamError("OKlib::InputOutput::StandardDIMACSInput::StandardDIMACSInput(std::istream&, cls_adaptor_type&):\n  cannot open the input stream in_stream");
        in.push(boost::ref(counter));
        in.push(in_stream);
        if (not in) throw IStreamError("OKlib::InputOutput::StandardDIMACSInput::StandardDIMACSInput(std::istream&, cls_adaptor_type&):\n  boost::iostreams::filtering_istream is defective");
        parse();
      }

    private :

      std::string error_location() const {
        std::stringstream s;
        s << "line " << counter.lines() << ", column " << counter.current_characters() << ", total characters read " << counter.characters();
        return s.str();
      }

      void parse() {
        read_comments();
        read_parameter_line();
        read_clauses();
      }

      void read_comments() {
        assert(in);
        for (;;) {
          peek = in.get();
          if (not in)
            throw CommentInputError("OKlib::InputOutput::StandardDIMACSInput::read_comments:\n  end of input before parameter section started\n" + error_location());
          if (peek != 'c') return;
          std::string comment_line;
          std::getline(in, comment_line);
          out.comment(comment_line);
        }
      }

      void read_parameter_line() {
        assert(in);
        if (peek != 'p')
          throw ParameterInputError("OKlib::InputOutput::StandardDIMACSInput::read_parameter_line:\n  no parameter section\n" + error_location());
        peek = in.get();
        if (not in)
          throw ParameterInputError("OKlib::InputOutput::StandardDIMACSInput::read_parameter_line:\n  end of input after \"p\" in the parameter line\n" + error_location());
        if (not peek == ' ')
          throw ParameterInputError("OKlib::InputOutput::StandardDIMACSInput::read_parameter_line:\n  no space as second character in parameter line\n" + error_location());
        std::string label;
        in >> label;
        if (not in)
          throw ParameterInputError("OKlib::InputOutput::StandardDIMACSInput::read_parameter_line:\n  end of input after \"p \" in the parameter line\n" + error_location());
        if (label != "cnf")
          throw ParameterInputError("OKlib::InputOutput::StandardDIMACSInput::read_parameter_line:\n  instead of the label \"cnf\" the string \"" + label + "\" was found\n" + error_location());
        {
          int_type_target n_target;
          in >> n; // needs to be checked ########################
          if (not in)
            throw ParameterInputError("OKlib::InputOutput::StandardDIMACSInput::read_parameter_line:\n  syntax error when reading the maximal index used for variables from the parameter line\n" + error_location());
          if (n < 0)
            throw ParameterInputError("OKlib::InputOutput::StandardDIMACSInput::read_parameter_line:\n  the maximal index for variables has the negative value " + boost::lexical_cast<std::string>(n) + "\n" + error_location());
          try {
            n_target = boost::numeric_cast<int_type_target>(n);
          }
          catch (const boost::bad_numeric_cast& e) {
            throw ParameterInputError(std::string("OKlib::InputOutput::StandardDIMACSInput::read_parameter_line:\n  conversion of maximal variable index to target integer type ") + typeid(int_type_target).name() + " failed with error message\n" + e.what() + "\n" + error_location());
          }
          out.n(n_target);
        }
        {
          int_type_target c_target;
          in >> c; // needs to be checked ########################
          if (not in)
            throw ParameterInputError("OKlib::InputOutput::StandardDIMACSInput::read_parameter_line:\n  syntax error when reading the number of clauses from the parameter line\n" + error_location());
          if (c < 0)
            throw ParameterInputError("OKlib::InputOutput::StandardDIMACSInput::read_parameter_line:\n  the number of clauses has the negative value " + boost::lexical_cast<std::string>(c) + "\n" + error_location());
          try {
            c_target = boost::numeric_cast<int_type_target>(c);
          }
          catch (const boost::bad_numeric_cast& e) {
            throw ParameterInputError(std::string("OKlib::InputOutput::StandardDIMACSInput::read_parameter_line:\n  conversion of number of clauses to target integer type ") + typeid(int_type_target).name() + " failed with error message\n" + e.what() + "\n" + error_location());
          }
          out.c(c_target);
        }
        std::string rest_of_line;
        std::getline(in, rest_of_line);
        if (not in)
          throw ParameterInputError("OKlib::InputOutput::StandardDIMACSInput::read_parameter_line:\n  unexpected end of file in the parameter line\n" + error_location());
        if (not boost::algorithm::all(rest_of_line, boost::algorithm::is_space()))
          throw ParameterInputError("OKlib::InputOutput::StandardDIMACSInput::read_parameter_line:\n  a trailing string \"" + rest_of_line + "\" was found in the parameter line\n" + error_location());
      }
      
      void read_clauses() {
        assert(in);
        typedef std::set<int_type> clause_type;
        typedef typename clause_type::size_type size_type;
        typedef typename clause_type::iterator iterator;

        clause_type clause;
        bool tautological = false;
        size_type total_clause_size = 0;
        int_type clauses_found = 0;
        
        for (int_type literal;;) {
          in >> literal; // needs to be checked #########################
          if (not in) {
            if (not in.eof())
              throw ClauseInputError("OKlib::InputOutput::StandardDIMACSInput::read_clauses:\n  syntax error before clause finished\n" + error_location());
            else if (total_clause_size == 0) {
              out.finish(); return;
            }
            else
              throw ClauseInputError("OKlib::InputOutput::StandardDIMACSInput::read_clauses:\n  file end before clause finished\n" + error_location());
          }
          if (literal == 0) { // end of clause
            if (++clauses_found > c)
              throw ClauseInputError("OKlib::InputOutput::StandardDIMACSInput::read_clauses:\n  more clauses found than the specified upper bound " + boost::lexical_cast<std::string>(c) + "\n" + error_location());
            int_type_target total_clause_size_target;
            try {
              total_clause_size_target = boost::numeric_cast<int_type_target>(total_clause_size);
            }
            catch (const boost::bad_numeric_cast& e) {
              throw ClauseInputError(std::string("OKlib::InputOutput::StandardDIMACSInput::read_clauses:\n  conversion of total number of literals read to target integer type ") + typeid(int_type_target).name() + " failed with error message\n" + e.what() + "\n" + error_location());
            }
            if (tautological) { 
              out.tautological_clause(total_clause_size_target);
              tautological = false;
            }
            else
              out.clause(clause, total_clause_size_target);
            clause.clear();
            total_clause_size = 0;
          }
          else { // literal added to clause
            ++total_clause_size;
            if (std::abs(literal) > n)
              throw ClauseInputError("OKlib::InputOutput::StandardDIMACSInput::read_clauses:\n  literal " + boost::lexical_cast<std::string>(literal) + " has variable index larger than the specified upper bound " + boost::lexical_cast<std::string>(n) + "\n" + error_location());
            if (not tautological) {
              const iterator& find_neg = clause.find(-literal);
              if (find_neg == clause.end())
                clause.insert(literal);
              else
                tautological = true;
            }
          }
        }
      }
    };

  }

}

#endif
