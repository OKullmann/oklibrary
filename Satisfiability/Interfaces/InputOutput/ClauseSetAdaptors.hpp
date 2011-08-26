// Oliver Kullmann, 14.10.2005 (Swansea)
/* Copyright 2005 - 2007, 2009, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file OKlib/Satisfiability/Interfaces/InputOutput/ClauseSetAdaptors.hpp
  \brief Adaptors to transfer clause-sets into some data structure

  A "CLSAdaptor" is a class (according to the concept below) for
  the input of a clause-set via transmission of clauses and meta-data,
  and which delivers some component (according to the purpose of the
  adaptor) filled with this data (e.g., some data-structure, or the data
  might be directly output).

  Main CLS-adaptors delivered here are:
  <ul>
   <li> InputOutput::CLSAdaptorStatistics (computing statistics) </li>
   <li> InputOutput::CLSAdaptorPreciseStatistics </li>
   <li> InputOutput::CLSAdaptorDIMACSOutput (output in DIMACs format) </li>
   <li> InputOutput::CLSAdaptorDIMACSFileOutput (output in DIMACs format to
   files) </li>
   <li> InputOutput::RawDimacsCLSAdaptor (transfer to vector of vectors) </li>
   <li> InputOutput::RawDimacsCLSAdaptorSets (transfer to set of sets) </li>
  </ul>


  \todo Write concept for CLSAdaptor:
  <ul>
   <li> <code> CLSAdaptor::int_type </code> (default = int) </li>
   <li> <code> CLSAdaptor::string_type </code> (default = std::string) </li>
   <li> <code> adaptor.comment(string_type) </code> (input of comment-lines
   (without the leading "c"-part and without end-of-line)) </li>
   <li> <code> adaptor.n(int_type) </code> (input of parameter n) </li>
   <li> <code> adaptor.c(int_type) </code> (input of parameter c) </li>
   <li> <code> adaptor.finish() </code> (signal that input of clause-set is
   finished) </li>
   <li> <code> adaptor.tautological_clause(int_type number_literal_occurrences)
   </code>
   (state that a tautological clause with that-many literal occurrences
   (without contractions) has been found) </li>
   <li>
   \code 
template <class ForwardRange> CLSAdaptor::clause(const ForwardRange& clause,
                   int_type total_original_number_literal_occurrences) 
   \endcode
   (input a non-tautological clause as a range over the literals, where
   multiple occurrences have been removed already, together with the total
   number of original literal occurrences). </li>
  </ul>

*/

#ifndef CLAUSESETADAPTORS_UjXSW3
#define CLAUSESETADAPTORS_UjXSW3

#include <string>
#include <istream>
#include <cstdlib>
#include <cassert>
#include <vector>
#include <set>

#include <boost/lexical_cast.hpp>
#include <boost/range/distance.hpp>
#include <boost/range/value_type.hpp>
#include <boost/range/iterator.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <boost/algorithm/string.hpp>

#include <OKlib/Satisfiability/ProblemInstances/Literals/TrivialLiterals.hpp>

#include <OKlib/Satisfiability/Interfaces/InputOutput/Exceptions.hpp>

namespace OKlib {
  namespace InputOutput {

    /*!
      \class Statistics
      \brief Class for gathering statistics about input/output (especially in DIMACS format).

      Meaning of data members:
      <ul>
       <li> comment_count : number of comment-lines </li>
       <li> parameter_n : value of the first parameter in the parameter line
       </li>
       <li> parameter_c : value of the second parameter in the parameter line
       </li>
       <li> tautological_clauses_count : number of tautological clauses </li>
       <li> non_tautological_clauses_count : number of non-tautological
       clauses </li>
       <li> total_number_literals : number of literal occurrences in
       tautological and non-tautological clauses together </li>
       <li> reduced_number_literals : number of literal occurrences after
       removal of tautological clauses and after contraction of multiple
       literal occurrences </li>
       <li> finished : clause-set has been completely read. </li>
      </ul>

      \todo For the output better a message-class is provided.

      \todo Create a concept
      <ul>
       <li> At least
        <ol>
         <li> Concepts::FullyConstructibleEq </li>
         <li> Concepts::EqualitySubstitutable </li>
         <li> default constructed: null-initialised </li>
         <li> equality holds iff all members are equal </li>
         <li> output-streamable (?) </li>
        </ol>
       </li>
    */

    template <typename Int = int>
    struct Statistics {

      typedef Int int_type;
      int_type
        comment_count,
        parameter_n,
        parameter_c,
        tautological_clauses_count,
        non_tautological_clauses_count,
        total_number_literals,
        reduced_number_literals;
      bool finished;

      Statistics() : comment_count(0), parameter_n(0), parameter_c(0), tautological_clauses_count(0), non_tautological_clauses_count(0), total_number_literals(0), reduced_number_literals(0), finished(false) {}
      Statistics(int_type cc, int_type pn, int_type pc, int_type tc, int_type ntc, int_type nl, int_type rnl) : comment_count(cc), parameter_n(pn), parameter_c(pc), tautological_clauses_count(tc), non_tautological_clauses_count(ntc), total_number_literals(nl), reduced_number_literals(rnl), finished(true) {}

      friend bool operator ==(const Statistics& lhs, const Statistics& rhs) {
        return lhs.comment_count == rhs.comment_count and lhs.parameter_n == rhs.parameter_n and lhs.parameter_c == rhs.parameter_c and lhs.tautological_clauses_count == rhs.tautological_clauses_count and lhs.non_tautological_clauses_count == rhs.non_tautological_clauses_count and lhs.total_number_literals == rhs.total_number_literals and lhs.reduced_number_literals == rhs.reduced_number_literals and lhs.finished == rhs.finished;
      }

      friend std::ostream& operator <<(std::ostream& out, const Statistics& s) {
        return out << " pn pc l c0 l0 comments finished\n"
          << s.parameter_n << " " << s.non_tautological_clauses_count << " " << s.reduced_number_literals << " " << s.tautological_clauses_count+s.non_tautological_clauses_count << " " << s.total_number_literals << " " << s.comment_count << " " << s.finished;
      }

    };


    enum output_options { full_output=1, no_zeros=2, no_initfinal_zeros=3 };

    /*!
      \class FullStatistics
      \brief Class for gathering "all" statistics about input/output (especially in DIMACS format).
    */

    template <typename Int = int>
    struct FullStatistics {

      typedef Statistics<Int> stat_type;
      typedef typename stat_type::int_type int_type;
      stat_type stat;
      typedef std::vector<int_type> map_type;
      map_type clause_lengths;
      output_options option;
      static const output_options default_option = full_output;

      FullStatistics() : option(default_option) {}
      FullStatistics(int_type cc, int_type pn, int_type pc, int_type tc, int_type ntc, int_type nl, int_type rnl, output_options opt = default_option) : stat(cc,pn,pc,tc,ntc,nl,rnl), option(opt) {}

      friend bool operator ==(const FullStatistics& lhs, const FullStatistics& rhs) {
        return lhs.stat == rhs.stat and lhs.clause_lengths == rhs.clause_lengths;
      }

      friend std::ostream& operator <<(std::ostream& out, const FullStatistics& s) {
        out << s.stat << "\n length count\n";
        typedef typename map_type::const_iterator iterator;
        iterator begin = s.clause_lengths.begin();
        iterator end = s.clause_lengths.end();
        if (s.option == no_initfinal_zeros) {
          while (begin != end and *begin == 0) ++begin;
          while (end != begin and *(end-1) == 0) --end;
        }
        for (iterator i = begin; i != end; ++i)
          if (s.option != no_zeros or *i != 0)
            out << i-s.clause_lengths.begin() << " " << *i << "\n";
        return out;
      }
    };


    // #####################################################

    /*!
      \class CLSAdaptorStatistics
      \brief Adaptor for clause-sets which only gathers (basic) statistics

      The data member stat contains the statistical information.

      \todo CLSAdaptorPreciseStatistics
      <ul>
       <li> Perhaps CLSAdaptorStatistics should be abandoned? </li>
       <li> But keeping it can't hurt? </li>
      </ul>
    */

    template <typename Int = int, class String = std::string>
    struct CLSAdaptorStatistics {
      typedef Int int_type;
      typedef String string_type;
      typedef Statistics<int_type> statistics_type;
      statistics_type stat;
      void comment(const string_type&) { ++stat.comment_count; }
      void n(const int_type pn) { stat.parameter_n = pn; }
      void c(const int_type pc) { stat.parameter_c = pc; }
      void finish() { stat.finished = true; }
      void tautological_clause(const int_type t) {
        ++stat.tautological_clauses_count;
        stat.total_number_literals += t;
      }
      template <class ForwardRange>
      void clause(const ForwardRange& r, const int_type t) {
        ++stat.non_tautological_clauses_count;
        stat.total_number_literals += t;
        stat.reduced_number_literals += boost::distance(r);
      }
    };

    /*!
      \class CLSAdaptorPreciseStatistics
      \brief Adaptor for clause-sets gathering (basic) statistics in all forms

      The data member stat contains the statistical information.

      \todo Correction
      <ul>
       <li> Yet the n-count is tranferred to stat.parameter_n, while this
       is for the parameter-line value. </li>
       <li> So class Statistics needs to be extended, and the new parameter
       needs to be used in finish(). </li>
      </ul>

      \todo Extensions
      <ul>
       <li> Also the maximal variable-index is needed. </li>
      </ul>
    */

    template <typename Int = int, class String = std::string>
    struct CLSAdaptorPreciseStatistics {
      typedef Int int_type;
      typedef String string_type;
      typedef Statistics<int_type> statistics_type;
      statistics_type stat;
      void comment(const string_type&) { ++stat.comment_count; }
      void n(const int_type pn) { stat.parameter_n = pn; }
      void c(const int_type pc) { stat.parameter_c = pc; }
      void finish() {
        stat.parameter_n = var.size();
        stat.finished = true;
      }
      void tautological_clause(const int_type t) {
        ++stat.tautological_clauses_count;
        stat.total_number_literals += t;
      }
      std::set<int_type> var;
      template <class ForwardRange>
      void clause(const ForwardRange& r, const int_type t) {
        ++stat.non_tautological_clauses_count;
        stat.total_number_literals += t;
        stat.reduced_number_literals += boost::distance(r);
        typedef typename boost::range_iterator<const ForwardRange>::type iterator;
        const iterator end = boost::const_end(r);
        for (iterator i = boost::const_begin(r); i != end; ++i)
          var.insert(std::abs(*i));
      }
    };

    /*!
      \class CLSAdaptorFullStatistics
      \brief Adaptor for clause-sets which gathers "all" statistics

      Additional to CLSAdaptorStatistics, now the clause-lengths of
      non-tautological clauses are determined in the data member stat.
    */

    template <typename Int = int, class String = std::string>
    struct CLSAdaptorFullStatistics {

      typedef String string_type;
      typedef FullStatistics<Int> statistics_type;
      typedef typename statistics_type::int_type int_type;
      typedef typename statistics_type::map_type::size_type size_type;

      statistics_type stat;

      void comment(const string_type&) { ++stat.stat.comment_count; }
      void n(const int_type pn) {
        stat.stat.parameter_n = pn;
        stat.clause_lengths.assign((size_type)pn+1,0);
      }
      void c(const int_type pc) { stat.stat.parameter_c = pc; }
      void finish() { stat.stat.finished = true; }
      void tautological_clause(const int_type t) {
        ++stat.stat.tautological_clauses_count;
        stat.stat.total_number_literals += t;
      }
      template <class ForwardRange>
      void clause(const ForwardRange& r, const int_type t) {
        ++stat.stat.non_tautological_clauses_count;
        stat.stat.total_number_literals += t;
        const size_type clause_length = boost::distance(r);
        stat.stat.reduced_number_literals += clause_length;
        ++stat.clause_lengths[clause_length];
      }
      
    };

    // #####################################################

    /*!
      \class CLSAdaptorDIMACSOutput
      \brief Adaptor for clause-sets for output in DIMACS format

      Parameter n is considered as maximal possible variable index,
      while parameter c is considered as upper bound on the number of clauses.

      The role of the template parameters are as follows:
      <ol>
       <li> Int is the integral type for the Dimacs literals. </li>
       <li> String is the string type for comments. </li>
       <li> AdaptorStatistics is a CLSAdaptor which handles statistics. </li>
      </ol>


      \todo For the output-jobs message-classes should be employed.


      \todo Handling of extended Dimacs-format (with real variable-names)
      is needed:
      <ul>
       <li> Instead of using the indices, the output might use the
       names via a supplied map from indices to strings. </li>
       <li> Or the mapping from indices to names is added to the
       comment section. </li>
      </ul>

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
          throw OKlib::InputOutput::OStreamError("OKlib::InputOutput::CLSAdaptorDIMACSOutput::CLSAdaptorDIMACSOutput(std::ostream&):\n  cannot open the output stream");
      }

      void comment(const string_type& s) {
        adaptor_statistics.comment(s);
        if (s.empty()) out << "c";
        else
          if (boost::algorithm::is_space()(s[0])) out << "c" << s;
          else out << "c " << s;
        out << "\n";
      }
      void n(const int_type pn) {
        adaptor_statistics.n(pn);
        if (pn < 0)
          throw OKlib::InputOutput::ParameterOutputError("OKlib::InputOutput::CLSAdaptorDIMACSOutput::n:\n  maximal variable index is a negative quantity = " + boost::lexical_cast<std::string>(pn));
      }
      void c(const int_type pc) {
        adaptor_statistics.c(pc);
        if (pc < 0)
          throw OKlib::InputOutput::ParameterOutputError("OKlib::InputOutput::CLSAdaptorDIMACSOutput::n:\n  number of clauses is a negative quantity = " + boost::lexical_cast<std::string>(pc));
        out << "p cnf " << adaptor_statistics.stat.parameter_n << " " << adaptor_statistics.stat.parameter_c << "\n";
      }
      void finish() { adaptor_statistics.finish(); }
      void tautological_clause(const int_type t) {
        adaptor_statistics.tautological_clause(t);
      }
      template <class ForwardRange>
      void clause(const ForwardRange& r, const int_type t) {
        adaptor_statistics.clause(r, t);
        if (adaptor_statistics.stat.non_tautological_clauses_count > adaptor_statistics.stat.parameter_c)
          throw OKlib::InputOutput::ClauseOutputError("OKlib::InputOutput::CLSAdaptorDIMACSOutput::clause:\n  number of non-tautological clauses exceeds specified total number of clauses = " + boost::lexical_cast<std::string>(adaptor_statistics.stat.parameter_c));
        typedef typename boost::range_const_iterator<ForwardRange>::type const_iterator;
        const const_iterator& end(boost::end(r));
        for (const_iterator i = boost::begin(r); i != end; ++i) {
          typedef typename boost::range_value<ForwardRange>::type value_type;
          const value_type& literal = *i;
          if (std::abs(literal) > adaptor_statistics.stat.parameter_n)
            throw OKlib::InputOutput::ClauseOutputError("OKlib::InputOutput::CLSAdaptorDIMACSOutput::clause:\n  variable index of literal = " + boost::lexical_cast<std::string>(literal) + " exceeds specified maximal index = " + boost::lexical_cast<std::string>(adaptor_statistics.stat.parameter_n));
          out << literal << " ";
        }
        out << 0 << "\n";
      }

      const adaptor_statistics_type& stat() const {
        return adaptor_statistics.stat;
      }

    };


    /*!
      \class CLSAdaptorDIMACSFileOutput
      \brief Adaptor for clause-sets for file-output in DIMACS format

      As CLSAdaptorDIMACSOutput, but with two additional member functions:
       - pline_position() returns the file-position of the parameter-line
       - pline_length() returns the length of the parameter-line (without
         the line-end).
    */

    template <typename Int = int, class String = std::string, class AdaptorStatistics = CLSAdaptorStatistics<Int, String> >
    class CLSAdaptorDIMACSFileOutput {
      typedef CLSAdaptorDIMACSOutput<Int, String, AdaptorStatistics> output_adaptor;
      std::iostream& file;
      output_adaptor out;
    public :
      typedef typename output_adaptor::int_type int_type;
      typedef typename output_adaptor::string_type string_type;
      typedef typename output_adaptor::statistics_type adaptor_statistics_type;
      typedef std::ios::pos_type pos_type;
      typedef std::string::size_type size_type;

      pos_type pline_position() { return ppos; }
      size_type pline_length() { return plength; }

      CLSAdaptorDIMACSFileOutput(std::iostream& f) : file(f), out(file) {}
      void comment(const string_type& s) { out.comment(s); }
      void n(const int_type pn) { out.n(pn); }
      void c(const int_type pc) {
        ppos = file.tellg();
        out.c(pc);
        plength = (file.tellg() - ppos) - 1;
        assert(plength >= 1 + 1 + 3 + 1 + 1 + 1);
      }
      void finish() { out.finish(); }
      void tautological_clause(const int_type t) {out.tautological_clause(t);}
      template <class ForwardRange>
      void clause(const ForwardRange& r, const int_type t) {out.clause(r,t);}
      const adaptor_statistics_type& stat() const { out.stat(); }
    private :
      pos_type ppos;
      size_type plength;
    };


    // #####################################################

    /*!
      \class RawDimacsCLSAdaptor
      \brief Adaptor which turns Dimacs input into a
      <code> std::vector<std::vector<int> > </code>.

      Comments, the two parameters and tautological clauses are
      ignored (have no effect). Multiple literal occurrences
      are not contracted, but show up in the clauses.

      More generally, any container for clauses can be used
      which supports "push_back".

    */

    template <
      typename Lit = OKlib::Literals::Literals_int,
      class ClauseContainer = std::vector<std::vector<Lit> >,
      typename Int = typename OKlib::Variables::traits::index_type<
        typename OKlib::Literals::traits::var_type<Lit>::type>::type,
      class String = std::string
      >
    class RawDimacsCLSAdaptor {

    public :

      typedef Lit literal_type;
      typedef ClauseContainer clause_set_type;
      typedef typename clause_set_type::value_type clause_type;
      typedef Int int_type;
      typedef String string_type;

      clause_set_type clause_set;

      typedef Statistics<int_type> statistics_type;
      statistics_type stat;

      RawDimacsCLSAdaptor() {}

      void comment(const string_type&) { ++stat.comment_count; }
      void n(const int_type pn) { stat.parameter_n = pn; } 
      void c(const int_type pc) { stat.parameter_c = pc; }
      void finish() { stat.finished = true; }
      void tautological_clause(const int_type t) {
        ++stat.tautological_clauses_count;
        stat.total_number_literals += t;
      }

      //! all literal occurrences are copied as is
      template <class ForwardRange>
      void clause(const ForwardRange& r, const int_type t) {
        ++stat.non_tautological_clauses_count;
        stat.total_number_literals += t;
        stat.reduced_number_literals += boost::distance(r);
        clause_set.push_back(clause_type(boost::begin(r), boost::end(r)));
      }

    };


    /*!
      \class RawDimacsCLSAdaptorSets
      \brief Adaptor which turns Dimacs input into a
      <code> std::set<std::set<int> > </code>.

      Comments, the two parameters and tautological clauses are
      ignored (have no effect). Multiple literal occurrences
      are (obviously) contracted, and so are multiple clauses.

      More generally, any container for clauses can be used
      which supports "insert". Here the container for literals should
      contract multiple literal occurrences (if not then the above
      specification about contraction has to be cancelled), and also
      the container for clauses should contract multiple clauses.

    */

    template <
      typename Lit = OKlib::Literals::Literals_int,
      class ClauseContainer = std::set<std::set<Lit> >,
      typename Int = typename OKlib::Variables::traits::index_type<
        typename OKlib::Literals::traits::var_type<Lit>::type>::type,
      class String = std::string
      >
    class RawDimacsCLSAdaptorSets {

    public :

      typedef Lit literal_type;
      typedef ClauseContainer clause_set_type;
      typedef typename clause_set_type::value_type clause_type;
      typedef Int int_type;
      typedef String string_type;

      clause_set_type clause_set;

      RawDimacsCLSAdaptorSets() {}
      void comment(const string_type&) const {}
      void n(const int_type) const {} 
      void c(const int_type) const {}
      void finish() const {}
      void tautological_clause(const int_type) const {}

      template <class ForwardRange>
      void clause(const ForwardRange& r, const int_type) {
        clause_set.insert(clause_type(boost::begin(r), boost::end(r)));
      }

    };
      
  }

}

#endif
