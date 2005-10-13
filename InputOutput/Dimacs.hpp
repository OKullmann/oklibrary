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
      \todo To be completed:
       - Create exception class hierarchy.
       - Create helper function for writing error messages (with stream positions).
       - Replace the #-parts below.
      \todo Write concept for CLSAdaptor.
      \todo Write simple CLSAdaptor, which creates only statistics (but does not store the formula).
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
        // if (not in) throw "can't create" ######################
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
          // if (not in) throw "end of input before parameter section" ########################
          if (peek != 'c') return;
          std::string comment_line;
          std::getline(in, comment_line);
          out.comment(comment_line);
        }
      }
      void read_parameter_line() {
        assert(in);
        // if (peek != 'p') throw "no parameter section"; ########################
        peek = in.get();
        // if (not in) throw "read error" ###########################
        // if (not peek == ' ') throw "no space as second character in parameter line" ###################
        std::string label;
        in >> label;
        // if (not in) throw "read error" #######################
        // if (label != "cnf") throw "syntax error parameter line" ########################
        int_type n;
        in >> n;
        // if (not in) throw "syntax error parameter line" #########################
        out.n(n);
        int_type c;
        in >> c;
        // if (not in) throw "syntax error parameter line" #########################
        out.c(c);
        std::string rest_of_line;
        std::getline(in, rest_of_line);
        // if (not in) throw "no clauses; unexpected end of file" #####################
        // if (not rest_of_line only space symbols) throw "other content at end of parameter line #################
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
          // if (not in)
          //   if (total_clause_size == 0) { out.finish(); return; }
          //   else throw "unexpected end of file before clause finished (or perhaps syntax error)" #######################
          ++total_clause_size;
          if (literal == 0) {
            if (tautological) {
              out.tautological_clause(total_clause_size);
              tautological = false;
            }
            else
              out.clause(clause.begin(), clause.end(), clause.size(), total_clause_size); // perhaps better as range?
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
