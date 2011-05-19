// Oliver Kullmann, 2.5.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file OKlib/Satisfiability/Assignments/PartialAssignments/Boolean.hpp
  \brief Partial assignments for boolean literals
*/

#ifndef BOOLEAN_gB5eSpoao7Y
#define BOOLEAN_gB5eSpoao7Y

#include <map>
#include <utility>
#include <cassert>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <vector>

#include <OKlib/Satisfiability/ProblemInstances/Literals/var.hpp>
#include <OKlib/Satisfiability/ProblemInstances/Literals/cond.hpp>
#include <OKlib/Satisfiability/ProblemInstances/Literals/Basics.hpp>
#include <OKlib/Satisfiability/Values/StatusTypes.hpp>

namespace OKlib {
  namespace Satisfiability {
    namespace Assignments {
      namespace PartialAssignments {

        /*!
          \class BPass0
          \brief An immutable boolean partial assignment in the simplest form
        */

        template <typename Lit>
        class BPass0 {
        public :
          typedef Lit literal_type;
        private :
          typedef OKlib::Satisfiability::ProblemInstances::Literals::Literal2pair<Lit> lit2pair_functor;
        public :
          typedef typename lit2pair_functor::litaspair_type litaspair_type;
          typedef typename lit2pair_functor::var_type var_type;
          typedef typename lit2pair_functor::cond_type cond_type;
        private :
          typedef std::map<var_type,bool> map_type;
          typedef typename map_type::const_iterator iterator;
          const map_type map;
        public :
          typedef typename map_type::size_type size_type;
          typedef OKlib::Satisfiability::Values::Assignment_status status_type;

          BPass0() {}
          template <class LitIterator>
          BPass0(const LitIterator begin, const LitIterator end) : map(fill_map(begin,end)) {}
          BPass0(const literal_type x) : map(map_type()) {
            if (not x) return;
            map[OKlib::Literals::var(x)] = OKlib::Literals::cond(x);
          }

          size_type size() const { return map.size(); }
          bool empty() const { return map.empty(); }

          friend bool operator ==(const BPass0& lhs, const BPass0& rhs) {
            return lhs.map == rhs.map;
          }

          status_type operator()(const literal_type x) const {
            using namespace OKlib::Literals;
            const var_type v = var(x);
            const iterator it = map.find(v);
            if (it == map.end()) return OKlib::Satisfiability::Values::unassigned;
            assert(v);
            return (status_type) ((cond(x)) ? it->second : not it->second);
          }

        private :
          template <class LitIterator>
          map_type fill_map(const LitIterator begin, const LitIterator end) {
            const lit2pair_functor l2p;
            map_type result;
            for (LitIterator i = begin; i != end; ++i) {
              const literal_type x = *i;
              if (not x) return result;
              const litaspair_type p = l2p(x);
              result.insert(p);
            }
            return result;
          }
        };


        /*!
          \class ApplyPassAdaptor
          \brief CLSAdaptor, which applies a partial assignments, and transfers the result to another CLSAdaptor

           - Inserts a comment showing the partial assignment directly before
             the parameter line.
           - Keeps the parameter values as given.
           - Maintains also the variables (i.e., no renaming), the order of
             clauses and the order of literals.
           - Via the member function new_c() one gets the current number
             of clauses (clauses transferred minus clauses satisfied).
           - And via new_n() one gets the current maximal variable index
             (which occurs in the result of the application).
           - Construction with an object as produced by InputOutput::ReadPass,
             and the adaptor-object for transfer.
        */

        template <typename Int, class RPass, class CLSAdaptor>
        class ApplyPassAdaptor {
        public :
          typedef Int int_type;
          typedef std::string string_type;

          typedef RPass rpass_type;
          typedef typename rpass_type::pass_type pass_type;
          typedef int_type literal_type;
          typedef CLSAdaptor cls_adaptor_t;

          const rpass_type& rpa;
          cls_adaptor_t& ad;

          ApplyPassAdaptor(const rpass_type& rpa, cls_adaptor_t& ad) : rpa(rpa), ad(ad),  numcl(0), maxvar(0) {}

          void comment(const string_type& com) { ad.comment(com); }
          void n(const int_type nvar) {
            ad.comment("Applied the following partial assignment:");
            std::stringstream new_line;
            typedef std::ostream_iterator<literal_type> oiterator;
            std::copy(rpa.litset.begin(), rpa.litset.end(), oiterator(new_line, " "));
            ad.comment(new_line.str());
            ad.n(nvar);
          }
          void c(const int_type nclauses) { ad.c(nclauses); }
          void finish() { ad.finish(); }
          void tautological_clause(int_type) {}
          template <class ForwardRange>
          void clause(const ForwardRange& clause, const int_type) {
            typedef typename ForwardRange::const_iterator iterator;
            const iterator clauseend = clause.end();
            typedef std::vector<literal_type> cl_t;
            cl_t C;
            int_type maxv = 0;
            for (iterator i = clause.begin(); i != clauseend; ++i) {
              typedef typename pass_type::status_type status_type;
              const literal_type x = *i;
              const status_type s = rpa.pa(x);
              using namespace OKlib::Satisfiability::Values;
              if (s == val0) continue;
              if (s == val1) return;
              C.push_back(x);
              const int_type v = OKlib::Literals::var(x);
              if (v > maxv) maxv = v;
            }
            ad.clause(C, C.size());
            ++numcl;
            if (maxv > maxvar) maxvar = maxv;
          }

          int_type new_c() const { return numcl; }
          int_type new_n() const { return maxvar; }

        private :
          int_type numcl;
          int_type maxvar;
        };

      }
    }
  }
}

#endif
