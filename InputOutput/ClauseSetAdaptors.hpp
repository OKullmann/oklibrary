// Oliver Kullmann, 14.10.2005 (Swansea)

/*!
  \file ClauseSetAdaptors.hpp
  \brief Adaptors to transfer clause-sets into some data structure.
  \todo Write concept for CLSAdaptor:
   - CLSAdaptor::int_type (default = int)
   - CLSAdaptor::string_type (default = std::string)
   - adaptor.comment(string_type)
   - adaptor.n(int_type)
   - adaptor.c(int_type)
   - adaptor.finish()
   - adaptor.tautological_clause(int_type number_literal_occurrences)
   - template <class ForwardRange> CLSAdaptor::clause(const Range& clause, int_type total_original_number_literal_occurrences).
  \todo Write concepts for Statistics class.
  \todo Write RawDimacsCLSAdaptor.
*/

#ifndef CLAUSESETADAPTORS_UjXSW3

#define CLAUSESETADAPTORS_UjXSW3

#include <string>
#include <ostream>
#include <cstdlib>
#include <cassert>
#include <vector>

#include <boost/range/size.hpp>
#include <boost/range/value_type.hpp>
#include <boost/range/const_iterator.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <boost/algorithm/string.hpp>

#include <Transitional/InputOutput/Exceptions.hpp>

namespace OKlib {

  namespace InputOutput {

    /*!
      \class Statistics
      \brief Class for gathering statistics about input/output (especially in DIMCAS format).
    */

    template <typename Int = int>
    struct Statistics {
      typedef Int int_type;
      int_type comment_count, parameter_n, parameter_c, tautological_clauses_count, non_tautological_clauses_count, total_number_literals, reduced_number_literals;
      bool finished;
      Statistics() : comment_count(0), parameter_n(0), parameter_c(0), tautological_clauses_count(0), non_tautological_clauses_count(0), total_number_literals(0), reduced_number_literals(0), finished(false) {}
      Statistics(int_type cc, int_type pn, int_type pc, int_type tc, int_type ntc, int_type nl, int_type rnl) : comment_count(cc), parameter_n(pn), parameter_c(pc), tautological_clauses_count(tc), non_tautological_clauses_count(ntc), total_number_literals(nl), reduced_number_literals(rnl), finished(true) {}

      friend bool operator ==(const Statistics& lhs, const Statistics& rhs) {
        return lhs.comment_count == rhs.comment_count and lhs.parameter_n == rhs.parameter_n and lhs.parameter_c == rhs.parameter_c and lhs.tautological_clauses_count == rhs.tautological_clauses_count and lhs.non_tautological_clauses_count == rhs.non_tautological_clauses_count and lhs.total_number_literals == rhs.total_number_literals and lhs.reduced_number_literals == rhs.reduced_number_literals and lhs.finished == rhs.finished;
      }

      friend std::ostream& operator <<(std::ostream& out, const Statistics& s) {
        return out << "c's = " << s.comment_count << ", n = " << s.parameter_n << ", c = " << s.parameter_c << ", tc = " << s.tautological_clauses_count << ", ntc = " << s.non_tautological_clauses_count << ", tl = " << s.total_number_literals << ", l = " << s.reduced_number_literals << ", finished = " << s.finished;
      }

    };

    // #####################################################

    /*!
      \class CLSAdaptorStatistics
      \brief Adaptor for clause-sets which only gathers statistics.
    */

    template <typename Int = int, class String = std::string>
    struct CLSAdaptorStatistics {

      typedef Int int_type;
      typedef String string_type;
      typedef Statistics<int_type> statistics_type;

      statistics_type stat;

      void comment(const string_type&) { ++ stat.comment_count; }
      void n(const int_type pn) { stat.parameter_n = pn; }
      void c(const int_type pc) { stat.parameter_c = pc; }
      void finish() { stat.finished = true; }
      void tautological_clause(const int_type t) {
        ++stat.tautological_clauses_count; stat.total_number_literals += t;
      }
      template <class ForwardRange>
      void clause(const ForwardRange& r, const int_type t) {
        ++stat.non_tautological_clauses_count; stat.total_number_literals += t;
        stat.reduced_number_literals += boost::size(r);
      }
      
    };

    // #####################################################

    /*!
      \class CLSAdaptorDIMACSOutput
      \brief Adaptor for clause-sets for output in DIMACS format
    */

    template <typename Int = int, class String = std::string, class AdaptorStatistics = CLSAdaptorStatistics<Int, String> >
    class CLSAdaptorDIMACSOutput {

      typedef AdaptorStatistics adaptor_statistics_type;

      std::ostream& out;
      adaptor_statistics_type adaptor_statistics;

    public :

      typedef Int int_type;
      typedef String string_type;
      typedef typename adaptor_statistics_type::statistics_type statistics_type;

      CLSAdaptorDIMACSOutput(std::ostream& out) : out(out) {
        if (not out)
          throw OStreamError("OKlib::InputOutput::CLSAdaptorDIMACSOutput::CLSAdaptorDIMACSOutput(std::ostream&):\n  cannot open the output stream");
      }

      void comment(const string_type& s) {
        adaptor_statistics.comment(s);
        if (s.empty())
          out << "c";
        else
          if (boost::algorithm::is_space()(s[0]))
            out << "c" << s;
          else
            out << "c " << s;
        out << "\n";
      }
      void n(const int_type pn) {
        adaptor_statistics.n(pn);
        if (pn < 0)
          throw ParameterOutputError("OKlib::InputOutput::CLSAdaptorDIMACSOutput::n:\n  maximal variable index is a negative quantity = " + boost::lexical_cast<std::string>(pn));
      }
      void c(const int_type pc) {
        adaptor_statistics.c(pc);
        if (pc < 0)
          throw ParameterOutputError("OKlib::InputOutput::CLSAdaptorDIMACSOutput::n:\n  number of clauses is a negative quantity = " + boost::lexical_cast<std::string>(pc));
        out << "p cnf " << adaptor_statistics.stat.parameter_n << " " << adaptor_statistics.stat.parameter_c << "\n";
      }
      void finish() {
        adaptor_statistics.finish();
      }
      void tautological_clause(const int_type t) {
        adaptor_statistics.tautological_clause(t);
      }
      template <class ForwardRange>
      void clause(const ForwardRange& r, const int_type t) {
        adaptor_statistics.clause(r, t);
        if (adaptor_statistics.stat.non_tautological_clauses_count > adaptor_statistics.stat.parameter_c)
          throw ClauseOutputError("OKlib::InputOutput::CLSAdaptorDIMACSOutput::clause:\n  number of non-tautological clauses exceeds specified total number of clauses = " + boost::lexical_cast<std::string>(adaptor_statistics.stat.parameter_c));
        typedef typename boost::range_const_iterator<ForwardRange>::type const_iterator;
        const const_iterator& end(boost::end(r));
        for (const_iterator i = boost::begin(r); i != end; ++i) {
          typedef typename boost::range_value<ForwardRange>::type value_type;
          const value_type& literal = *i;
          if (std::abs(literal) > adaptor_statistics.stat.parameter_n)
            throw ClauseOutputError("OKlib::InputOutput::CLSAdaptorDIMACSOutput::clause:\n  variable index of literal = " + boost::lexical_cast<std::string>(literal) + " exceeds specified maximal index = " + boost::lexical_cast<std::string>(adaptor_statistics.stat.parameter_n));
          out << literal << " ";
        }
        out << 0 << "\n";
      }

      const adaptor_statistics_type& stat() const {
        return adaptor_statistics.stat;
      }

    };


    // #####################################################

    /*!
      \class RawDimacsCLSAdaptor
      \brief Adaptor which turns Dimacs input into a vector<vector<int>>
    */

    template <typename Int = int, class String = std::string >
    class RawDimacsCLSAdaptor {

    public :

      typedef Int int_type;
      typedef String string_type;

      typedef std::vector<int_type> clause_type;
      typedef std::vector<clause_type> clause_set_type;

      clause_set_type clause_set;

      RawDimacsCLSAdaptor() {}

      void comment(const string_type& s) {}
      void n(const int_type pn) {} 
      void c(const int_type pc) {}
      void finish() {}
      void tautological_clause(const int_type t) {}

      template <class ForwardRange>
      void clause(const ForwardRange& r, const int_type t) {
        // Add a clause based on r to the clause_set
      }

    };
      
  }

}

#endif
