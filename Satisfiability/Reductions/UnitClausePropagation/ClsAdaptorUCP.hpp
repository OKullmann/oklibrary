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

#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <boost/range/size.hpp>

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
        \brief Transferring a (boolean) clause-set into a vector of vectors, and then performing unit-clause propagation

        Models CLSAdaptor and basic ClauseList. This is the basic linear-time
        algorithm, using the clause-literal graph and considering only
        falsified literals during the reduction process.

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

        void comment(const string_type& com_) { com = com_; }
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
          if (empty_cl) {
            add_com << "\nc Empty clause in input.";
            return falsified;
          }
          if (phi.empty())
            if (F.empty()) {
              add_com << "\nc No non-tautological clause in input.";
              return satisfied;
            }
            else {
              add_com << "\nc No unit-clause in input.";
              return open;
            }
          add_com << "\nc " << phi.size() << " unit-clauses in input.";

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
                if (R.first == falsified) return falsified;
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
          if (F.empty()) return satisfied;
          else return open;
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

      }
    }
  }
}

#endif
