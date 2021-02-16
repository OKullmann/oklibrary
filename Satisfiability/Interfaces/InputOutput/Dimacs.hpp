// Oliver Kullmann, 24.7.2005 (Swansea)
/* Copyright 2005 - 2007, 2009, 2011, 2021 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file OKlib/Satisfiability/Interfaces/InputOutput/Dimacs.hpp
  \brief %Tools for input and output of cnf's in DIMACS format (via CLS-adaptors)

  The main components are as follows:
  <ul>
   <li> InputOutput::StandardDIMACSInput transfers a clause-set on a stream
   into a CLS-adaptor. </li>
   <li> InputOutput::ListTransfer transfers a clause-set given by a
   rudimentary data-structure into a CLS-adaptor. </li>
   <li> Instance InputOutput::List2DIMACSOutput directly translates such a
   clause-set into DIMACS output. </li>
  </ul>

*/


#ifndef DIMACS_jahsjdjd010
#define DIMACS_jahsjdjd010

#include <istream>
#include <cassert>
#include <string>
#include <set>
#include <sstream>
#include <cstdlib>
#include <map>
#include <vector>
#include <utility>
#include <ostream>
#include <limits>

#include <boost/iostreams/filtering_stream.hpp>
#include <boost/ref.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <boost/lexical_cast.hpp>

#include <OKlib/Programming/InputOutput/IOStreamFilters.hpp>

#include <OKlib/Satisfiability/ProblemInstances/Literals/TrivialLiterals.hpp>
#include <OKlib/Satisfiability/Interfaces/InputOutput/Exceptions.hpp>

#include <OKlib/Satisfiability/Interfaces/InputOutput/ClauseSetAdaptors.hpp>


namespace OKlib {
  namespace InputOutput {

    // Policies for reading literals *******************************

    /*!
      \class LiteralReadingStrict
      \brief Standard policy for reading a Dimacs-literal.
    */

    template <typename Int>
    struct LiteralReadingStrict {
      typedef Int int_type;
      static void read(std::istream& in, int_type& literal) {
        in >> literal;
      }
    };

    /*!
      \class LiteralReadingExtended
      \brief Extended policy, which allows arbitrary strings as variables.

      The Dimacs-parser uses member-function <code>read(in, literal)</code>,
      where literal is an integer-reference, containing the literal converted
      to an integer.

      The mapping between strings and natural numbers established so far
      is available as follows:
      <ul>
       <li> <code>index(name)</code> returns the index of a variable (runs in
       logarithmic time; return 0 if not found); </li>
       <li> <code>name(index)</code> returns the name belonging to an index
       (runs in constant time, assumes a valid index). </li>
      </ul>

      Throws exceptions in case of syntax errors, according to the following
      restrictions on the strings:
      <ul>
       <li> They must be non-empty. </li>
       <li> They must not contain space-symbols (more precisely, they are read
       in at once when streaming-in std::string). </li>
       <li> A leading '-' is interpreted as negation. </li>
       <li> A string equal "0" is the end-of-clause-marker. </li>
      </ul>

    */

    template <typename Int>
    class LiteralReadingExtended {
    public :
      typedef Int int_type;
    private :
      typedef std::map<std::string, int_type> map_type;
      static map_type indices;
      typedef typename map_type::const_iterator iterator_type;
      typedef std::vector<iterator_type> vector_type;
      static vector_type names;
      static int_type n;

    public :
      //! Used by the Dimacs-parser to read a literal
      static void read(std::istream& in, int_type& literal) {
        assert(in);
        std::string literal_s;
        in >> literal_s;
        if (not in) return;
        assert(not literal_s.empty());
        if (literal_s == "0") {
          literal = 0;
          return;
        }
        const bool neg = (literal_s[0] == '-');
        if (neg) literal_s.erase(0,1);
        if (literal_s.empty())
          throw ClauseInputError("OKlib::InputOutput:LiteralReadingExtended::read\n isolated \"-\" for literal=\"" + literal_s + "\"\n");
        typedef typename map_type::iterator nc_iterator_type;
        const nc_iterator_type literal_i = indices.lower_bound(literal_s);
        if (literal_i == indices.end() or literal_s != literal_i -> first) {
          const typename map_type::value_type value_pair(literal_s, ++n);
          indices.insert(literal_i, value_pair);
          names.push_back(indices.insert(literal_i, value_pair));
          literal = n;
        }
        else
          literal = literal_i -> second;
        if (neg) literal = -literal;
      }

      /*! The mapping from strings (representing variables) to natural numbers
        (in the order of occurrences in the input-stream).
        Returns 0 if string is not element of the domain of the map.
      */
      static int_type index(const std::string name) {
        const iterator_type index_i = indices.find(name);
        if (index_i == indices.end())
          return 0;
        else {
          assert(index_i -> second >= 1);
          assert(index_i -> second <= n);
          return index_i -> second;
        }
      }
      //! The original name of an index; assumes a valid index.
      static std::string name(const int_type index) {
        assert(index >= 1);
        assert(index <= n);
        return names[index];
      }
      //! The (precise) number of variables found in total.
      static int_type number_variables() { return n; }
    };
    template <typename Int>
    typename LiteralReadingExtended<Int>::map_type LiteralReadingExtended<Int>::indices;
    template <typename Int>
    typename LiteralReadingExtended<Int>::vector_type LiteralReadingExtended<Int>::names;
    template <typename Int>
    typename LiteralReadingExtended<Int>::int_type LiteralReadingExtended<Int>::n(0);


    // Parsing Dimacs input **************************************

    /*!
      \class StandardDIMACSInput
      \brief Parsing an input stream containing a cnf formula in DIMACS format and transferring it to a CLS-adaptor.

      Constructing an object of this type means parsing an input-stream and
      transfer to a clause-set-adaptor.

      Template parameters are:
      <ul>
       <li> CLSAdaptor : the parsed input is handed over to this adaptor. </li>
       <li> LiteralReadingStrict : reads a literal-representation in the file,
       and transfers it into a integer-literal. </li>
       <li> Int : the integer type for representing literals. </li>
       <li> In case of check_par = false, the parameter-line in the input
       stream is just skipped (it is assumed that a line exists), and for its
       values the maximal values of CLSAdaptor::int_type is taken. </li>
      </ul>

    */

    template <class CLSAdaptor, template <typename Int> class LiteralReadingPolicy = LiteralReadingStrict, typename Int = int>
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
      const bool check_par;

      StandardDIMACSInput(std::istream& in_stream, cls_adaptor_type& out, const bool check_par = true) : out(out), n(0), c(0), check_par(check_par) {
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
        if (check_par) read_parameter_line();
        else in.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
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
        if (peek != ' ')
          throw ParameterInputError("OKlib::InputOutput::StandardDIMACSInput::read_parameter_line:\n  no space as second character in parameter line\n" + error_location());
        std::string label;
        in >> label;
        if (not in)
          throw ParameterInputError("OKlib::InputOutput::StandardDIMACSInput::read_parameter_line:\n  end of input after \"p \" in the parameter line\n" + error_location());
        if (label != "cnf")
          throw ParameterInputError("OKlib::InputOutput::StandardDIMACSInput::read_parameter_line:\n  instead of the label \"cnf\" the string \"" + label + "\" was found\n" + error_location());
        {
          in >> n;
          if (not in)
            throw ParameterInputError("OKlib::InputOutput::StandardDIMACSInput::read_parameter_line:\n  syntax error when reading the maximal index used for variables from the parameter line\n" + error_location());
          if (n < 0)
            throw ParameterInputError("OKlib::InputOutput::StandardDIMACSInput::read_parameter_line:\n  the maximal index for variables has the negative value " + boost::lexical_cast<std::string>(n) + "\n" + error_location());
          int_type_target n_target;
          try { n_target = boost::numeric_cast<int_type_target>(n); }
          catch (const boost::bad_numeric_cast& e) {
            throw ParameterInputError(std::string("OKlib::InputOutput::StandardDIMACSInput::read_parameter_line:\n  conversion of maximal variable index to target integer type ") + typeid(int_type_target).name() + " failed with error message\n" + e.what() + "\n" + error_location());
          }
          out.n(n_target);
        }
        {
          in >> c;
          if (not in)
            throw ParameterInputError("OKlib::InputOutput::StandardDIMACSInput::read_parameter_line:\n  syntax error when reading the number of clauses from the parameter line\n" + error_location());
          if (c < 0)
            throw ParameterInputError("OKlib::InputOutput::StandardDIMACSInput::read_parameter_line:\n  the number of clauses has the negative value " + boost::lexical_cast<std::string>(c) + "\n" + error_location());
          int_type_target c_target;
          try { c_target = boost::numeric_cast<int_type_target>(c); }
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

        LiteralReadingPolicy<int_type> lit_handling;
        for (int_type literal;;) {
          lit_handling.read(in, literal);
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
            ++clauses_found;
            if (check_par and clauses_found > c)
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
            else out.clause(clause, total_clause_size_target);
            clause.clear();
            total_clause_size = 0;
          }
          else { // literal added to clause
            ++total_clause_size;
            if (check_par and std::abs(literal) > n)
              throw ClauseInputError("OKlib::InputOutput::StandardDIMACSInput::read_clauses:\n  literal " + boost::lexical_cast<std::string>(literal) + " has variable index larger than the specified upper bound " + boost::lexical_cast<std::string>(n) + "\n" + error_location());
            if (not tautological) {
              const iterator& find_neg = clause.find(-literal);
              if (find_neg == clause.end()) clause.insert(literal);
              else tautological = true;
            }
          }
        }
      }
    };

    // Transferring clause-sets as sequences of sequences, assuming DIMACS literals

    /*!
      \class ListTransfer
      \brief Transfers a sequence of sequences of literals to a CLSAdaptor

      <ul>
       <li> It is assumed that the clauses do not contain clashing or repeated
       literals. </li>
       <li> The literals are assumed to be integral, and are transferred
       unchanged. </li>
       <li> The p-values for the output are computed as maximal variable-index
       and as the clause-count. </li>
      </ol>
    */

    template <class CLSAdaptor>
    class ListTransfer {

    public :
      typedef CLSAdaptor cls_adaptor_type;
      typedef typename cls_adaptor_type::int_type int_type;
      typedef typename cls_adaptor_type::string_type string_type;

      template <class Cls>
      ListTransfer(const Cls& F, cls_adaptor_type& out, const string_type& comment = "") {
        if (not comment.empty()) out.comment(comment);
        typedef typename Cls::const_iterator clause_iterator;
        typedef typename Cls::value_type clause_type;
        typedef typename clause_type::const_iterator literal_iterator;
        {
          int_type n = 0, c = 0;
          for (clause_iterator Ci = F.begin(); Ci != F.end(); ++Ci, ++c) {
            const clause_type& C(*Ci);
            for (literal_iterator li = C.begin(); li != C.end(); ++li) {
              const int_type v(OKlib::Literals::var(*li));
              if (v > n) n = v;
            }
          }
          out.n(n); out.c(c);
        }
        for (clause_iterator Ci = F.begin(); Ci != F.end(); ++Ci)
          out.clause(*Ci, Ci -> size());
        out.finish();
      }

      template <class Cls>
      ListTransfer(const Cls& F, cls_adaptor_type& out, const int_type n, const string_type& comment = "") {
        if (not comment.empty()) out.comment(comment);
        typedef typename Cls::const_iterator clause_iterator;
        out.n(n); out.c(F.size());
        for (clause_iterator Ci = F.begin(); Ci != F.end(); ++Ci)
          out.clause(*Ci, Ci -> size());
        out.finish();
      }
    };

    // Convenience wrappers

    /*!
      \class List2DIMACSOutput
      \brief Transfers a clause-set as a sequence of sequences to an output-stream in Dimacs format
    */

    struct List2DIMACSOutput {
      typedef InputOutput::CLSAdaptorDIMACSOutput<> cls_adaptor_type;
      typedef ListTransfer<cls_adaptor_type> list_transfer_type;
      typedef list_transfer_type::string_type string_type;
      template <class Cls>
      List2DIMACSOutput(const Cls& F, std::ostream& out, const string_type& comment = "") {
        cls_adaptor_type A(out);
        list_transfer_type(F, A, comment);
      }
    };

  }

}

#endif
