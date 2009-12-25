// Oliver Kullmann, 12.12.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file OKlib/Satisfiability/Reductions/UnitClausePropagation/ClsAdaptorUCP.hpp
  \brief CLS-adaptors which perform unit-clause propagation
*/

#ifndef CLSADAPTORUCP_JHjjHEw2
#define CLSADAPTORUCP_JHjjHEw2

#include <list>
#include <vector>
#include <string>
#include <sstream>
#include <cassert>

#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <boost/range/size.hpp>
#include <boost/range/const_iterator.hpp>
#include <boost/utility.hpp>

#include <OKlib/Satisfiability/ProblemInstances/Literals/TrivialLiterals.hpp>
#include <OKlib/Satisfiability/ProblemInstances/Literals/var.hpp>
#include <OKlib/Satisfiability/ProblemInstances/Literals/cond.hpp>
#include <OKlib/Satisfiability/ProblemInstances/Clauses/ClausesWithRemoval.hpp>

namespace OKlib {
  namespace Satisfiability {
    namespace Reductions {
      namespace UnitClausePropagation {

      /*!
        \class CLSAdaptorUcp
        \brief Transferring a (boolean) clause-set into a clause-list, and then performing unit-clause propagation

        Specification:
        <ul>
         <li> Models CLSAdaptor and basic ClauseList. </li>
         <li> This is the basic linear-time algorithm, using the
         clause-literal graph and considering only falsified literals during
         the reduction process. </li>
         <li> The reduction process is performed when calling perform_ucp()
         (after having completed the transfer). </li>
         <li> The clause-list is usable thereafter iff the return-value is
         "open". </li>
         <li> Additional comments reflecting the UCP-process are available
         via add_comment(). </li>
        </ul>

        Assumptions:
        <ul>
         <li> Template-parameter Lit needs to be a (signed) integral type.
         </li>
         <li> Template-parameter Clauses needs to fulfil concept "Clauses with
         removal". </li>
        </ul>

        \todo Complete container-functionality
        <ul>
         <li> Yet not all required members for a sequence are there. </li>
        </ul>

        \todo Write unit-tests.

        \todo Improve implementation
        <ul>
         <li> Should we store the clause-list via a vector? Likely then
         data-locality is improved; on the other hand then the final
         removal of satisfied clauses becomes more complicated. </li>
         <li> Likely using vectors instead of lists for the occurrence-lists
         (per literal) is faster: construction is slower, but then usage
         is faster. </li>
        </ul>

        \todo Use messages.

        \todo Separate the transfer from the unit-clause propagation
        <ul>
         <li> Then for the UCP general (boolean) literals can be used. </li>
        </ul>

        \todo Harmonise with KLevelForcedAssignments::CLSAdaptorKUcp
        <ul>
         <li> See
         OKlib/Satisfiability/Reductions/KLevelForcedAssignments/GeneralisedUCP.hpp.
         </li>
        </ul>

      */

      template <typename Lit = OKlib::Literals::Literals_int, class Clauses = OKlib::Satisfiability::ProblemInstances::Clauses::RClausesAsVectors<Lit> >
      class CLSAdaptorUcp {
      public :
        typedef Lit int_type;
        typedef std::string string_type;

        typedef Clauses value_type;
        typedef std::list<value_type> clause_set_type;
        typedef typename clause_set_type::const_iterator const_iterator;
        typedef typename clause_set_type::iterator iterator;
        typedef typename clause_set_type::size_type size_type;

        typedef std::vector<OKlib::Satisfiability::Values::Assignment_status> assignment_type;
        typedef std::vector<int_type> partial_assignment_type;

        CLSAdaptorUcp() : empty_cl(false) {}

        const_iterator begin() const { return F.begin(); }
        const_iterator end() const { return F.end(); }
        size_type size() const { return F.size(); }
        const string_type& orig_comment() const { return com; }
        string_type add_comment() const { return add_com.str(); }
        const assignment_type& ucp_total() const { return f; }
        const partial_assignment_type& ucp_partial() const { return phi; }

        void comment(const string_type& com_) {
          if (com.empty()) com = com_;
          else
            com += "\nc" + com_;
        }
        void n(const int_type n_) {
          num_var = n_;
          f.assign(num_var+1, OKlib::Satisfiability::Values::unassigned);
          phi.reserve(num_var);
        }
        void c(const int_type c_) {
          num_cl = c_;
        }
        void tautological_clause(int_type) const {}
        template <class Range>
        void clause(const Range& clause, int_type) {
          const size_type s = boost::size(clause);
          if (s == 0) { empty_cl = true; return; }
          if (s == 1) {
            const int_type x = *boost::const_begin(clause);
            assert(x != 0);
            const int_type v = OKlib::Literals::var(x);
            using namespace OKlib::Satisfiability::Values;
            const Assignment_status new_val = Assignment_status(OKlib::Literals::cond(x));
            const Assignment_status old_val = f[v];
            if (old_val == unassigned) {
              phi.push_back(x);            
              f[v] = new_val;
            }
            else if (old_val != new_val) empty_cl = true;
            return;
          }
          F.push_back(value_type(boost::const_begin(clause), boost::const_end(clause)));
        }
        void finish() const {}
        bool empty_clause() const { return empty_cl; }

        // Perform unit-clause propagation; the clause-sequence is usable
        // afterwords iff the return-value is "open".
        OKlib::Satisfiability::Values::Sat_status perform_ucp() {
          using namespace OKlib::Satisfiability::Values;
          add_com << "\nc Additional comments regarding the unit-clause propagation:";
          add_com << "\nc The original parameter were: n = " << num_var << ", c = " << num_cl << ".";
          add_com << "\nc After elimination of empty clauses, unit-clauses and tautological clauses the number of clauses is " << F.size() << ".";
          if (empty_cl) {
            add_com << "\nc Empty clause directly entailed by input (thus the problem is unsatisfiable).";
            return falsified;
          }
          if (phi.empty())
            if (F.empty()) {
              add_com << "\nc No non-tautological clause in input (thus the problem is satisfiable).";
              return satisfied;
            }
            else {
              add_com << "\nc No unit-clause in input.";
              return open;
            }
          add_com << "\nc Number of unit-clauses in input = " << phi.size() << ".";

          // create the clause-literal graph:
          typedef std::list<iterator> occurrence_list_t;
          typedef std::vector<occurrence_list_t> clause_literal_t;
          clause_literal_t cl_graph(2*num_var+1);
          const iterator endF(F.end());
          for (iterator i = F.begin(); i != endF; ++i) {
            typedef typename value_type::const_iterator l_iterator;
            const l_iterator endC(i -> end());
            for (l_iterator j = i -> begin(); j != endC; ++j)
              cl_graph[index(*j)].push_back(i);
          }

          // perform ucp:
          typedef typename partial_assignment_type::const_iterator u_iterator;
          for (u_iterator next = phi.begin(); next != phi.end(); ++next) {
            const int_type x = - *next;
            assert(f[OKlib::Literals::var(x)] == (x>0) ? val0 : val1);
            const int_type i = index(x);
            typedef typename occurrence_list_t::const_iterator o_iterator;
            const o_iterator endO(cl_graph[i].end());
            for (o_iterator C = cl_graph[i].begin(); C != endO; ++C)
              if ((*C) -> remove(x)) {
                const typename value_type::unit_return_t R = (*C) -> unit(f);
                if (R.first == falsified) {
                  add_com << "\nc UCP determines unsatisfiability after processing " << (next - phi.begin()) + 1 << " assignments.";
                  return falsified;
                }
                if (R.first == satisfied) continue;
                const int_type y = R.second;
                assert(y != 0);
                phi.push_back(y);
                const int_type v = OKlib::Literals::var(y);
                assert(f[v] == unassigned);
                f[v] = Assignment_status(OKlib::Literals::cond(y));
              }
          }

          // final update of the clause-list:
          const iterator end(F.end());
          for (iterator i = F.begin(); i != end;)
            switch (i -> update(f)) {
            case falsified :
              assert(0); // this must has been detected before
              return falsified;
            case satisfied : F.erase(i++); break;
            case open : ++i;
            }
          if (F.empty()) {
            add_com << "\nc UCP determines satisfiability after processing " << phi.size() << " assignments.";
            return satisfied;
          }
          else {
            add_com << "\nc UCP performed " << phi.size() << " assignments.";
            return open;
          }
        }

      private :
        clause_set_type F;
        assignment_type f;
        partial_assignment_type phi;
        int_type num_var;
        int_type num_cl;
        string_type com;
        bool empty_cl;
        std::stringstream add_com;

        int_type index(const int_type lit) const {
          assert(lit >= - num_var);
          assert(lit != 0);
          assert(lit <= num_var);
          return lit + num_var;
        }
      };


        /*!
          \class CLSAdaptorUcpW
          \brief Transferring a (boolean) clause-set into a clause-list, and then performing UCP using watched literals

          Requirements:
          <ul>
           <li> the literal type is a signed integral type </li>
          </ul>

          \todo Complete implementation

          \todo Write unit-tests

          \todo Harmonise with UnitClausePropagation::CLSAdaptorUcp
          <ul>
           <li> See above.
           </li>
          </ul>

        */

        template <class WatchedClauses, class Assignment>
        struct CLSAdaptorUcpW {
          typedef WatchedClauses clause_type;
          typedef typename clause_type::value_type literal_type;
          typedef Assignment assignment_type;

          typedef std::vector<clause_type> clause_set_type;

          typedef std::vector<literal_type> bclause_list_type;
          typedef std::vector<bclause_list_type> bclause_set_type;
          typedef typename bclause_list_type::const_iterator iterator_bclauses;

          typedef typename clause_set_type::iterator iterator_clauses;
          typedef std::list<iterator_clauses> wclause_list_type;
          typedef std::vector<wclause_list_type> wclause_set_type;
          typedef typename wclause_list_type::iterator iterator_wclauses;

          typedef typename clause_set_type::size_type size_type;

          CLSAdaptorUcpW() : empty_cl(false), contradicting_ucl(false) {}
          

          // Functionality for CLSAdaptor
          typedef literal_type int_type;
          typedef std::string string_type;
          void comment(const string_type& com_) {
            if (com.empty()) com = com_;
            else com += "\nc" + com_;
          }
          void n(const int_type n_) {
            num_var = n_;
            max_lit_index = 2*num_var;
            F2.reserve(max_lit_index+1);
            FW.reserve(max_lit_index+1);
          }
          void c(const int_type c_) {
            num_cl = c_;
            F.reserve(num_cl);
          }
          void tautological_clause(int_type) const {}
          template <class Range>
          void clause(const Range& clause, int_type) {
            const size_type s = boost::size(clause);
            if (s == 0) { empty_cl = true; return; }
            typedef typename boost::range_const_iterator<Range>::type range_iterator;
            const range_iterator bc = boost::const_begin(clause);
            if (s == 1) {
              const literal_type x = *bc;
              if (not f.add_forced(x))
                contradicting_ucl : true;
              return;
            }
            if (s == 2) {
              const literal_type x = *bc;
              const literal_type y = *boost::next(bc);
              F2[index(x)].push_back(y);
              F2[index(y)].push_back(x);
              return;
            }
            F.push_back(clause_type(bc, boost::const_end(clause)));
            const iterator_clauses C = F.back();
            const literal_type x = C -> first();
            const literal_type y = C -> second();
            FW[index(x)].push_back(C);
            FW[index(y)].push_back(C);
          }
          void finish() const {}
          bool empty_clause() const { return empty_cl; }

        private :
          clause_set_type F;
          bclause_set_type F2;
          wclause_set_type FW;
          assignment_type f;
          int_type num_var;
          int_type num_cl;
          int_type max_lit_index;
          string_type com;
          bool empty_cl;
          bool contradicting_ucl;
          std::stringstream add_com;

          // translate a literal lit into an index from 0 to max_lit_index
          // (inclusive; index num_var is not used):
          int_type index(const int_type lit) const {
            assert(lit >= - num_var);
            assert(lit != 0);
            assert(lit <= num_var);
            return lit + num_var;
          }

          // return true iff a contradiction was found
          bool perform_ucp() {
            while (not f.empty()) {
              const int_type x = - f.top(); f.pop();
              const int_type x_i = index(x);
              {
                const iterator_bclauses end = F2[x_i].end();
                for (iterator_bclauses i = F2[x_i].begin(); i != end; ++i)
                  if (not f.push_forced(*i)) return true;
              }
              const iterator_wclauses end = FW[x_i].end();
              for (iterator_wclauses i = FW[x_i].begin(); i != end;) {
                const iterator_clauses j = *(i++);
                const int_type y = *j -> remove(x, f);
                if (y == 0) return true;
                if (y == x) continue;
                if (not f.push_forced(y)) return true;
                FW[x_i].remove(i);
                FW[index(y)].push_back(j);
              }
            }
            return false;
          }
        };

      }
    }
  }
}

#endif
