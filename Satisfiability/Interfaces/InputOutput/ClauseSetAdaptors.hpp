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
  <ol>
   <li> the input of a clause-set via transmission of clauses and meta-data,
   </li>
   <li> which delivers some component (according to the purpose of the
   adaptor) filled with this data (e.g., some data-structure, or the data
   might be directly output). </li>
  </ol>

  Main CLS-adaptors delivered here are:
  <ul>
   <li> InputOutput::CLSAdaptorStatistics (computing "ncl"-statistics) </li>
   <li> InputOutput::CLSAdaptorPreciseStatistics (extended ncl-statistics)
   </li>
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
   (without contractions, i.e., counting multiple occurrences) has been found)
   </li>
   <li>
   \code
template <class ForwardRange> CLSAdaptor::clause(const ForwardRange& clause,
                   int_type total_original_number_literal_occurrences)
   \endcode
   (input a non-tautological clause as a range over the literals, where
   multiple occurrences have been removed already, together with the total
   number of original literal occurrences (counting multiple occurrences).
   </li>
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
#include <iomanip>

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
       <li> comment_count ("cmts"): number of comment-lines </li>
       <li> parameter_n ("pn"): value of the first parameter in the parameter
       line </li>
       <li> parameter_c ("pc") : value of the second parameter in the
       parameter line </li>
       <li> variables ("n"): the number of actually occurring clauses after
       elementary reductions </li>
       <li> variables_orig ("n0"): the number of actually occurring clauses in
       the original input (taking tautological clauses into account) </li>
       <li> variables_maxindex ("nmi"): the maximal index of (occurring)
       variables after elementary reductions </li>
       <li> variables_maxindex_orig ("n0mi"): the maximal index of (occurring)
       variables in the original input (taking tautological clauses into
       account) </li>
       <li> tautological_clauses_count : number of tautological clauses (in the
       original input); this number is c0 - c </li>
       <li> non_tautological_clauses_count ("c") : number of non-tautological
       clauses </li>
       <li> total_number_literals ("l0") : number of literal occurrences in
       tautological and non-tautological clauses together </li>
       <li> reduced_number_literals ("l") : number of literal occurrences after
       elementary reductions </li>
       <li> finished : clause-set has been completely read </li>
       <li> pne, pce, ne, n0e, nmie, n0mie: whether pn, pc, n, n0, nmi or
       n0mi has been entered or not. </li>
      </ul>
      "Elementary reductions" here is the elimination of tautological clauses
      and elimination of repeated literals in clauses.

      \todo For the output better a message-class is provided.

      \todo Create a concept
      <ul>
       <li> At least
        <ol>
         <li> Concepts::FullyConstructibleEq </li>
         <li> Concepts::EqualitySubstitutable </li>
         <li> default constructed: null-initialised </li>
         <li> equality holds iff all members are equal </li>
         <li> output-streamable </li>
        </ol>
       </li>
    */

    template <typename Int = int>
    class Statistics {
    public :
      typedef Int int_type;
    private :
      int_type
        comment_count,
        parameter_n,
        parameter_c,
        variables,
        variables_orig,
        variables_maxindex,
        variables_maxindex_orig,
        tautological_clauses_count,
        non_tautological_clauses_count,
        total_number_literals,
        reduced_number_literals;
      bool finished_reading;
      bool pne, pce, ne, n0e, nmie, n0mie; // "e" for "entered"
    public :
      Statistics() :
        comment_count(0), tautological_clauses_count(0), non_tautological_clauses_count(0), total_number_literals(0), reduced_number_literals(0), finished_reading(false),
        pne(false), pce(false), ne(false), n0e(false), nmie(false), n0mie(false) {}

      Statistics& pn(const int_type v) { parameter_n = v; pne = true; return *this;}
      int_type pn() const { return parameter_n; }
      bool pn_entered() const { return pne; }

      Statistics& pc(const int_type v) { parameter_c = v; pce = true; return *this;}
      int_type pc() const { return parameter_c; }
      bool pc_entered() const { return pce; }

      Statistics& n(const int_type v) { variables = v; ne = true; return *this;}
      int_type n() const { return variables; }
      bool n_entered() const { return ne; }

      Statistics& n0(const int_type v) { variables_orig = v; n0e = true; return *this;}
      int_type n0() const { return variables_orig; }
      bool n0_entered() const { return n0e; }

      Statistics& nmi(const int_type v) { variables_maxindex = v; nmie = true; return *this;}
      int_type nmi() const { return variables_maxindex; }
      bool nmi_entered() const { return nmie; }

      Statistics& n0mi(const int_type v) { variables_maxindex_orig = v; n0mie = true; return *this;}
      int_type n0mi() const { return variables_maxindex_orig; }
      bool n0mi_entered() const { return n0mie; }

      Statistics& tcadd(const int_type v) { tautological_clauses_count += v; return *this;}
      int_type tc() const { return tautological_clauses_count; }

      Statistics& cadd(const int_type v) { non_tautological_clauses_count += v; return *this;}
      int_type c() const { return non_tautological_clauses_count; }

      Statistics& ladd(const int_type v) { reduced_number_literals += v; return *this; }
      int_type l() const { return reduced_number_literals; }

      Statistics& l0add(const int_type v) { total_number_literals += v; return *this; }
      int_type l0() const { return total_number_literals; }

      Statistics& commentsadd(const int_type v) { comment_count += v; return *this; }
      int_type comments() const { return comment_count; }

      Statistics& finished(const bool v) { finished_reading = v; return *this; }
      bool finished() const { return finished_reading; }


      friend bool operator ==(const Statistics& lhs, const Statistics& rhs) {
        return
          lhs.comment_count == rhs.comment_count and
          (lhs.pne == rhs.pne and (not lhs.pne or lhs.parameter_n == rhs.parameter_n)) and
          (lhs.pce == rhs.pce and (not lhs.pce or lhs.parameter_c == rhs.parameter_c)) and
          (lhs.ne == rhs.ne and (not lhs.ne or lhs.variables == rhs.variables)) and
          (lhs.n0e == rhs.n0e and (not lhs.n0e or lhs.variables_orig == rhs.variables_orig)) and
          (lhs.nmie == rhs.nmie and (not lhs.nmie or lhs.variables_maxindex == rhs.variables_maxindex)) and
          (lhs.n0mie == rhs.n0mie and (not lhs.n0mie or lhs.variables_maxindex_orig == rhs.variables_maxindex_orig)) and
          lhs.tautological_clauses_count == rhs.tautological_clauses_count and
          lhs.non_tautological_clauses_count == rhs.non_tautological_clauses_count and
          lhs.total_number_literals == rhs.total_number_literals and
          lhs.reduced_number_literals == rhs.reduced_number_literals and
          lhs.finished_reading == rhs.finished_reading;
      }

      friend std::ostream& operator <<(std::ostream& out, const Statistics& s) {
        using std::setw;
        const std::streamsize wn = 7, wc = 8, wl = 9, wcmts = 6;
        out << setw(wn) << "pn" << setw(wc) << "pc" << setw(wn) << "n"
          << setw(wn) << "nmi" << setw(wc) << "c" << setw(wl) << "l"
          << setw(wn) << "n0" << setw(wn) << "n0mi" << setw(wc) << "c0"
          << setw(wl) << "l0" << setw(wcmts) << "cmts" << "\n";
        out.width(wn);
        if (s.pne) out << s.parameter_n; else out << "NA";
        out.width(wc);
        if (s.pce) out << s.parameter_c; else out << "NA";
        out.width(wn);
        if (s.ne) out << s.variables; else out << "NA";
        out.width(wn);
        if (s.nmie) out << s.variables_maxindex; else out << "NA";
        out << setw(wc) << s.non_tautological_clauses_count << setw(wl) << s.reduced_number_literals;
        out << setw(wn);
        if (s.n0e) out << s.variables_orig; else out << "NA";
        out << setw(wn);
        if (s.n0mie) out << s.variables_maxindex_orig; else out << "NA";
        out << setw(wc) << s.tautological_clauses_count+s.non_tautological_clauses_count << setw(wl) << s.total_number_literals << setw(wcmts) << s.comment_count;
        return out;
      }

    };


    enum output_options { full_output=1, no_zeros=2, no_initfinal_zeros=3 };

    /*!
      \class FullStatistics
      \brief Class for gathering "all" statistics about input/output (especially in DIMACS format).

      Contains a Statistics object plus for the occurring clause-lengths the
      occurrence counts (after elementary reductions).
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

      friend bool operator ==(const FullStatistics& lhs, const FullStatistics& rhs) {
        return lhs.stat == rhs.stat and lhs.clause_lengths == rhs.clause_lengths;
      }

      friend std::ostream& operator <<(std::ostream& out, const FullStatistics& s) {
        out << s.stat << "\n";
        using std::setw;
        const std::streamsize wn = 7, wc = 8;
        out << setw(wn) << "length" << setw(wc) << "count" << "\n";
        typedef typename map_type::const_iterator iterator;
        iterator begin = s.clause_lengths.begin();
        iterator end = s.clause_lengths.end();
        if (s.option == no_initfinal_zeros) {
          while (begin != end and *begin == 0) ++begin;
          while (end != begin and *(end-1) == 0) --end;
        }
        for (iterator i = begin; i != end; ++i)
          if (s.option != no_zeros or *i != 0)
            out << setw(wn) << i-s.clause_lengths.begin() <<
              setw(wc) << *i << "\n";
        return out;
      }
    };


    // #####################################################

    /*!
      \class CLSAdaptorStatistics
      \brief Adaptor for clause-sets which only gathers the basic ncl-statistics

      <ul>
       <li> The data member stat contains the statistical information. </li>
       <li> Neither actual occurring variables nor their maximal index is
       determined. </li>
       <li> For a more complete computation of ncl-statistics see
       CLSAdaptorPreciseStatistics. </li>
      </ul>

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
      void comment(const string_type&) { stat.commentsadd(1); }
      void n(const int_type pn) { stat.pn(pn); }
      void c(const int_type pc) { stat.pc(pc); }
      void finish() { stat.finished(true); }
      void tautological_clause(const int_type t) {
        assert(t >= 2);
        stat.tcadd(1);
        stat.l0add(t);
      }
      template <class ForwardRange>
      void clause(const ForwardRange& r, const int_type t) {
        stat.cadd(1);
        const int_type width = boost::distance(r);
        assert(width <= t);
        stat.ladd(width);
        stat.l0add(t);
      }
    };

    /*!
      \class CLSAdaptorPreciseStatistics
      \brief Adaptor for clause-sets gathering (basic) statistics in all forms

      <ul>
       <li> The data member stat contains the statistical information. </li>
       <li> The data member var is the set of all occurring variables. </li>
       <li> All members of Statistics are filled except of n0 and n0mi. </li>
      </ul>

      \todo Correction : DONE
      <ul>
       <li> Yet the n-count is tranferred to stat.parameter_n, while this
       is for the parameter-line value. </li>
       <li> So class Statistics needs to be extended, and the new parameter
       needs to be used in finish(). </li>
      </ul>

      \todo Extensions : DONE
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
      int_type max_var_index;
      std::set<int_type> var;
      CLSAdaptorPreciseStatistics() : max_var_index(-1) {}
      void comment(const string_type&) { stat.commentsadd(1); }
      void n(const int_type pn) { stat.pn(pn); }
      void c(const int_type pc) { stat.pc(pc); }
      void finish() {
        stat.nmi(max_var_index);
        stat.n(var.size());
        stat.finished(true);
      }
      void tautological_clause(const int_type t) {
        assert(t >= 2);
        stat.tcadd(1);
        stat.l0add(t);
      }
      template <class ForwardRange>
      void clause(const ForwardRange& r, const int_type t) {
        stat.cadd(1);
        stat.l0add(t);
        typedef typename boost::range_iterator<const ForwardRange>::type iterator;
        const iterator end = boost::const_end(r);
        int_type width = 0;
        for (iterator i = boost::const_begin(r); i != end; ++i, ++width) {
          const int_type underlying_var = std::abs(*i);
          if (underlying_var > max_var_index) max_var_index = underlying_var;
          var.insert(underlying_var);
        }
        assert(width <= t);
        stat.ladd(width);
      }
    };

    /*!
      \class CLSAdaptorFullStatistics
      \brief Adaptor for clause-sets which gathers "all" statistics

      Additional to CLSAdaptorPreciseStatistics, now the clause-lengths of
      non-tautological clauses are determined in the data member stat.
    */

    template <typename Int = int, class String = std::string>
    struct CLSAdaptorFullStatistics {

      typedef String string_type;
      typedef FullStatistics<Int> statistics_type;
      typedef typename statistics_type::int_type int_type;
      typedef typename statistics_type::map_type::size_type size_type;

      statistics_type stat;
      int_type max_var_index;
      std::set<int_type> var;
      CLSAdaptorFullStatistics() : max_var_index(-1) {}

      void comment(const string_type&) { stat.stat.commentsadd(1); }
      void n(const int_type pn) {
        stat.stat.pn(pn);
        stat.clause_lengths.assign((size_type)pn+1,0);
      }
      void c(const int_type pc) { stat.stat.pc(pc); }
      void finish() {
        stat.stat.nmi(max_var_index);
        stat.stat.n(var.size());
        stat.stat.finished(true);
      }
      void tautological_clause(const int_type t) {
        assert(t >= 2);
        stat.stat.tcadd(1);
        stat.stat.l0add(t);
      }
      template <class ForwardRange>
      void clause(const ForwardRange& r, const int_type t) {
        stat.stat.cadd(1);
        stat.stat.l0add(t);
        typedef typename boost::range_iterator<const ForwardRange>::type iterator;
        const iterator end = boost::const_end(r);
        int_type width = 0;
        for (iterator i = boost::const_begin(r); i != end; ++i, ++width) {
          const int_type underlying_var = std::abs(*i);
          if (underlying_var > max_var_index) max_var_index = underlying_var;
          var.insert(underlying_var);
        }
        assert(width <= t);
        stat.stat.ladd(width);
        ++stat.clause_lengths[width];
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
        out << "p cnf " << adaptor_statistics.stat.pn() << " " << adaptor_statistics.stat.pc() << "\n";
      }
      void finish() { adaptor_statistics.finish(); }
      void tautological_clause(const int_type t) {
        adaptor_statistics.tautological_clause(t);
      }
      template <class ForwardRange>
      void clause(const ForwardRange& r, const int_type t) {
        adaptor_statistics.clause(r, t);
        if (adaptor_statistics.stat.c() > adaptor_statistics.stat.pc())
          throw OKlib::InputOutput::ClauseOutputError("OKlib::InputOutput::CLSAdaptorDIMACSOutput::clause:\n  number of non-tautological clauses exceeds specified total number of clauses = " + boost::lexical_cast<std::string>(adaptor_statistics.stat.pc()));
        typedef typename boost::range_const_iterator<ForwardRange>::type const_iterator;
        const const_iterator& end(boost::end(r));
        for (const_iterator i = boost::begin(r); i != end; ++i) {
          typedef typename boost::range_value<ForwardRange>::type value_type;
          const value_type& literal = *i;
          if (std::abs(literal) > adaptor_statistics.stat.pn())
            throw OKlib::InputOutput::ClauseOutputError("OKlib::InputOutput::CLSAdaptorDIMACSOutput::clause:\n  variable index of literal = " + boost::lexical_cast<std::string>(literal) + " exceeds specified maximal index = " + boost::lexical_cast<std::string>(adaptor_statistics.stat.pn()));
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

      <ul>
       <li> Comments, the two parameters and tautological clauses are
       ignored (have no effect) for the output. </li>
       <li> Multiple literal occurrences are not contracted, but show up in
       the clauses. </li>
       <li> The data-member stats contains the (simple) collected statistics
       (no determination of n or nmi). </li>
      </ul>

      Any container for clauses can be used which supports "push_back".

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

      void comment(const string_type&) { stat.commentsadd(1); }
      void n(const int_type pn) { stat.pn(pn); }
      void c(const int_type pc) { stat.pc(pc); }
      void finish() { stat.finished(true); }
      void tautological_clause(const int_type t) {
        assert(t >= 2);
        stat.tcadd(1);
        stat.l0add(t);
      }

      //! all literal occurrences are copied as is
      template <class ForwardRange>
      void clause(const ForwardRange& r, const int_type t) {
        stat.cadd(1);
        const int_type width = boost::distance(r);
        assert(width <= t);
        stat.ladd(width);
        stat.l0add(t);
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
