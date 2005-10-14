// Oliver Kullmann, 24.7.2005 (Swansea)

/*!
  \namespace InputOutput
  \brief Input and output of clause-sets and related things (but not
  general input/output facilities).
*/

/*!
  \file Dimacs.hpp
  \brief Tools for the input and output of cnf's in DIMACS format.
  \todo Writing generic DIMACS parser, starting with
  ::OKlib::DPv::ParserLiteral and ::OKlib::DPv::DimacsParser
  (located in Input_output.hpp in module DPv).
  \todo Write generic DIMACS writer.
  \todo Write ReadClauseCollection<DimacsParser, ClauseCollection> and WriteClauseCollection<ClauseCollection, DimacsWriter> (likely this should go into a separate file).
*/


#ifndef DIMACS_jahsjdjd010

#define DIMACS_jahsjdjd010

#include <istream>
#include <cassert>
#include <string>
#include <set>
#include <sstream>

#include <boost/iostreams/filtering_stream.hpp>
#include <boost/ref.hpp>
#include <boost/algorithm/string.hpp>

#include "IOStreamFilters.hpp"

#include "Exceptions.hpp"

namespace OKlib {
  
  namespace InputOutput {

    /*!
      \class StandardDIMACSInput
      \brief Parsing an input stream containing a cnf formula in DIMACS format and transferring it to a CLS-adaptor.
      \todo Write concept for CLSAdaptor.
      \todo Write simple CLSAdaptor, which creates only statistics (but does not store the formula).
      \todo Use Message for messages.
    */

    template <class CLSAdaptor, typename Int = int>
    class StandardDIMACSInput {
      boost::iostreams::filtering_istream in;
      CLSAdaptor& out;
      OKlib::FilesystemTools::Counter counter;
      char peek;

      StandardDIMACSInput(const StandardDIMACSInput&);
      ~StandardDIMACSInput();

    public :
      typedef CLSAdaptor cls_adaptor_type;
      typedef Int int_type;

      StandardDIMACSInput(std::istream& in_stream, cls_adaptor_type& out) : out(out) {
        if (not in_stream)
          throw IStreamError("OKlib::InputOutput::StandardDIMACSInput::StandardDIMACSInput(std::istream&, cls_adaptor_type&):\n  can't open the input stream in_stream");
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
            throw DimacsError("OKlib::InputOutput::StandardDIMACSInput::read_comments:\n  end of input before parameter section started\n" + error_location());
          if (peek != 'c') return;
          std::string comment_line;
          std::getline(in, comment_line);
          out.comment(comment_line);
        }
      }

      void read_parameter_line() {
        assert(in);
        if (peek != 'p')
          throw DimacsError("OKlib::InputOutput::StandardDIMACSInput::read_parameter_line:\n  no parameter section\n" + error_location());
        peek = in.get();
        if (not in)
          throw DimacsError("OKlib::InputOutput::StandardDIMACSInput::read_parameter_line:\n  end of input after \"p\" in the parameter line\n" + error_location());
        if (not peek == ' ')
          throw DimacsError("OKlib::InputOutput::StandardDIMACSInput::read_parameter_line:\n  no space as second character in parameter line\n" + error_location());
        std::string label;
        in >> label;
         throw DimacsError("OKlib::InputOutput::StandardDIMACSInput::read_parameter_line:\n  end of input after \"p \" in the parameter line\n" + error_location());
         if (label != "cnf")
           throw DimacsError("OKlib::InputOutput::StandardDIMACSInput::read_parameter_line:\n  instead of the label \"cnf\" the string \"" + label + "\" was found\n" + error_location());
        int_type n;
        in >> n;
        if (not in)
          throw DimacsError("OKlib::InputOutput::StandardDIMACSInput::read_parameter_line:\n  syntax error when reading the maximal index used for variables from the parameter line\n" + error_location());
        out.n(n);
        int_type c;
        in >> c;
        if (not in)
          throw DimacsError("OKlib::InputOutput::StandardDIMACSInput::read_parameter_line:\n  syntax error when reading the number of clauses from the parameter line\n" + error_location());
        out.c(c);
        std::string rest_of_line;
        std::getline(in, rest_of_line);
        if (not in)
          throw DimacsError("OKlib::InputOutput::StandardDIMACSInput::read_parameter_line:\n  unexpected end of file in the parameter line\n" + error_location());
        if (not boost::algorithm::all(rest_of_line, boost::algorithm::is_space()))
          throw DimacsError("OKlib::InputOutput::StandardDIMACSInput::read_parameter_line:\n  a trailing string \"" + rest_of_line + "\" was found in the parameter line\n" + error_location());
      }
        
      void read_clauses() {
        assert(in);
        typedef std::set<int_type> clause_type;
        typedef typename clause_type::size_type size_type;
        typedef typename clause_type::iterator iterator;

        clause_type clause;
        bool tautological = false;
        size_type total_clause_size;
        
        for (int_type literal;;) {
          in >> literal;
          if (not in)
            if (total_clause_size == 0) {
              out.finish(); return;
            }
            else
              throw DimacsError("OKlib::InputOutput::StandardDIMACSInput::read_clauses:\n  syntax error before clause finished\n" + error_location());
          ++total_clause_size;
          if (literal == 0) {
            if (tautological) {
              out.tautological_clause(total_clause_size);
              tautological = false;
            }
            else
              out.clause(clause, total_clause_size); // "clause" is a range here (this should include the size)
            clause.clear();
            total_clause_size = 0;
          }
          else {
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
