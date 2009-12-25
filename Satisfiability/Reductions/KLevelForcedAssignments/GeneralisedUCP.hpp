// Oliver Kullmann, 20.12.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file OKlib/Satisfiability/Reductions/KLevelForcedAssignments/GeneralisedUCP.hpp
  \brief Components for r_k-reductions

*/

#ifndef GENERALISEDUCP_HvPi4erD
#define GENERALISEDUCP_HvPi4erD

#include <string>
#include <vector>
#include <list>
#include <sstream>
#include <cassert>

#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <boost/range/size.hpp>
#include <boost/range/const_iterator.hpp>
#include <boost/utility.hpp>

namespace OKlib {
  namespace Satisfiability {
    namespace Reductions {
      namespace KLevelForcedAssignments {

        /*!
          \class CLSAdaptorKUcp
          \brief Transferring a (boolean) clause-set into a clause-list, and then performing r_k-reduction

          For the Maxima-specification see generalised_ucp_cs in
          ComputerAlgebra/Satisfiability/Lisp/Reductions/GeneralisedUCP.mac.

          Requirements:
          <ul>
           <li> the literal type is a signed integral type </li>
          </ul>

          \todo Complete implementation

          \todo Write unit-tests

          \todo Harmonise with UnitClausePropagation::CLSAdaptorUcp
          <ul>
           <li> See
           OKlib/Satisfiability/Reductions/UnitClausePropagation/ClsAdaptorUCP.hpp.
           </li>
          </ul>

        */

        template <class WatchedClauses, class Assignment>
        struct CLSAdaptorKUcp {
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

          CLSAdaptorKUcp() : empty_cl(false), contradicting_ucl(false) {}
          

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

          // return true iff a contradiction was found
          bool perform_kucp(const size_type k) {
            if (empty_cl) return true;
            if (k == 0) return false;
            if (contradicting_ucl) return true;
            if (k == 1) { return perform_ucp(); }
            // XXX to be implemented
            return false;
          }

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
