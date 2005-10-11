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

#include <boost/iostreams/filtering_stream.hpp>
#include <boost/ref.hpp>

#include "IOStreamFilters.hpp"

namespace OKlib {
  
  namespace InputOutput {

    /*!
      \class StandardDIMACSInput
      \brief Parsing an input stream containing a cnf formula in DIMACS format and transferring it to a CLS-adaptor.
      \todo To be completed.
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
        // if (not in_stream) throw "can't open" ################
        in.push(boost::ref(counter));
        in.push(in_stream);
        // if (not in) thro "can't create" ######################
        parse();
      }
    private :
      void parse() {
        read_comments();
        read_parameter_line();
        read_clauses();
      }
      void read_comments() {
        assert(in);
        for (;;) {
          peek = in.get();
          // if (not in) throw "end of input before parameter section"
          if (peek != 'c') return;
          std::string comment_line;
          std::getline(in, comment_line);
          out.comment(comment_line);
        }
      }
      void read_parameter_line() {
        assert(in);
        // if (peek != 'p') throw "no parameter section";
        peek = in.get();
        // if (not in) throw "read error"
        // if (not peek == ' ') throw "no space as second character in parameter line"
        std::string label;
        in >> label;
        // if (not in) throw "read error"
        // if (label != "cnf") throw "syntax error"
        int_type n;
        in >> n;
        // if (not in) throw "syntax error"
        out.n(n);
        int_type c;
        in >> c;
        // if (not in) throw "syntax error"
        out.c(c);
        std::string rest_of_line;
        std::getline(in, rest_of_line);
        // if (not in) throw "no clauses; unexpected end of file"
        // if (not rest_of_line only space symbols) throw "other content on parameter line
      }
        
      void read_clauses() {
        assert(in);
        typedef std::set<int_type> clause_type;
        clause_type clause;
        int_type literal;
        bool clause_started = false;
        for (;;) {
          in >> literal;
          // if (not in)
          //   if (not clause_started) { out.finish(); return; }
          //   else throw "unexpected end of file before clause finished"
          if (literal == 0) {
            out.clause(clause.begin(), clause.end(), clause.size());
            clause.clear();
            clause_started = false;
          }
          else {
            clause.insert(literal);
            clause_started = true;
          }
        }
      }
    };

  }

}

#endif
