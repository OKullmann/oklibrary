// Oliver Kullmann, 12.12.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file OKlib/Satisfiability/ProblemInstances/Clauses/ClausesWithRemoval.hpp
  \brief Clause-classes which allow removal of literals

  Especially appropriate for (simple) unit-clause propagation.

  The concept of a "ClauseWithRemoval" is as follows:
  <ul>
   <li> Construction with a sequence of literals (given by begin and end).
   </li>
   <li> The sequence must be non-tautological and of lengt at least two. </li>
   <li> Provides then the basic functionality of a sequence of literals. </li>
   <li> Additionally there is the member function remove(x) for a literal x
   occurring in the clause, which possibly "lazily" removes x (see "update"
   below). </li>
   <li> So the sequence can not be retrieved directly after this; however
   its size can be determined (taking only the removals by remove(x) into
   account). </li>
   <li> remove(x) returns a boolean which is true iff the unit-clause has
   been created (ignoring satisfied literals). </li>
   <li> Via unit(f) for a total assignment f (containing the evaluations
   underlying the removals) one obtains the sat-status of the clause plus
   the remaining unit-literal if possible. </li>
   <li> It is assumed that only literals have been removed which are set to
   false by f. </li>
   <li> Finally update(f) updates the clause, returning the sat-status;
   only if the sat-status is open and the clause is not unit can the
   sequence of literals be used after this. </li>
  </ul>

  \todo Make the above concept definition more precise.

  \todo Write concept-check and concept-prototypes.

*/

#ifndef CLAUSESWITHREMOVAL_alakajaFr4
#define CLAUSESWITHREMOVAL_alakajaFr4

#include <vector>
#include <set>
#include <cassert>
#include <utility>

#include <OKlib/Satisfiability/ProblemInstances/Literals/var.hpp>
#include <OKlib/Satisfiability/ProblemInstances/Literals/cond.hpp>
#include <OKlib/Satisfiability/Values/StatusTypes.hpp>

namespace OKlib {
  namespace Satisfiability {
    namespace ProblemInstances {
      namespace Clauses {

        /*!
          \class RClausesAsVectors
          \brief Clauses with remove-functionality, implemented by vectors

          Model of concept ClauseWithRemoval.

          \todo Write unit-tests.
        */

        template <class Lit>
        class RClausesAsVectors {

        public :

          typedef Lit value_type;

          template <class Iterator> RClausesAsVectors(
            const Iterator begin,
            const Iterator end) :
            C(begin,end), b(C.begin()), e(C.end()), s(C.size()) {
              assert(s >= 2);
            }

          RClausesAsVectors(const RClausesAsVectors& D) : C(D.C), b(C.begin()), e(C.end()), s(D.s) {
            assert(C.size() >= 2);
            assert(s >= 1);
          }

          const RClausesAsVectors& operator =(const RClausesAsVectors& rhs) {
            C = rhs.C;
            assert(C.size() >= 2);
            b = C.begin();
            e = C.end();
            s = rhs.s;
            assert(s >= 1);
            return *this;
          }

          friend bool operator ==(const RClausesAsVectors& lhs, const RClausesAsVectors& rhs) {
            return lhs.C == rhs.C and lhs.s == rhs.s;
          }

        private :
          typedef std::vector<value_type> vector_t;
        public :
          typedef typename vector_t::const_iterator const_iterator;
          typedef const_iterator iterator;
          typedef typename vector_t::size_type size_type;

          iterator begin() const { return b; }
          iterator end() const { return e; }
          size_type size() const { return s; }

          bool remove(value_type) {
            assert(s >= 2);
            return --s == 1;
          }

          typedef std::pair<OKlib::Satisfiability::Values::Sat_status,value_type> unit_return_t;

          template <class TotalAssignment>
          unit_return_t unit(const TotalAssignment& f) const {
            using namespace OKlib::Satisfiability::Values;
            assert(s == 1);
            for (iterator i = b; i != e; ++i) {
              const value_type x = *i;
              switch (f[OKlib::Literals::var(x)]) {
              case val0 :
                if (OKlib::Literals::cond(x)) continue; else
                return std::make_pair(satisfied, value_type(0));
              case val1 :
                if (OKlib::Literals::cond(x))
                  return std::make_pair(satisfied, value_type(0));
                else continue;
              case unassigned : return std::make_pair(open, x);
              }
            }
            return std::make_pair(falsified, value_type(0));
          }

          template <class TotalAssignment>
          OKlib::Satisfiability::Values::Sat_status update(const TotalAssignment& f) {
            using namespace OKlib::Satisfiability::Values;
            assert(s >= 1);
            vector_t D;
            const iterator end(e);
            for (iterator i = b; i != end; ++i) {
              const value_type x = *i;
              switch (f[OKlib::Literals::var(x)]) {
                case val0 : if (OKlib::Literals::cond(x)) continue;
                else return satisfied;
                case val1 : if (OKlib::Literals::cond(x)) return satisfied;
                else continue;
                case unassigned : D.push_back(x);
              }
            }
            if (D.empty()) return falsified;
            C.swap(D);
            b = C.begin();
            e = C.end();
            s = C.size();
            assert(s >= 2);
            return open;
          }

        private :

          vector_t C;
          iterator b;
          iterator e;
          size_type s;
        };


        /*!
          \class RClausesAsSets
          \brief Clauses with remove-functionality, implemented by sets

          Model of concept ClauseWithRemoval.

          \todo Write unit-tests.
        */

        template <class Lit>
        class RClausesAsSets {

        public :

          typedef Lit value_type;

          template <class Iterator>
          RClausesAsSets(
            const Iterator begin,
            const Iterator end) :
            C(begin,end), b(C.begin()) {
              assert(C.size() >= 2);
            }

          RClausesAsSets(const RClausesAsSets& D) : C(D.C), b(C.begin()) {
            assert(C.size() >= 2);
          }

          const RClausesAsSets& operator =(const RClausesAsSets& rhs) {
            C = rhs.C;
            assert(C.size() >= 2);
            b = C.begin();
            return *this;
          }

         friend bool operator ==(const RClausesAsSets& lhs, const RClausesAsSets& rhs) {
            return lhs.C == rhs.C;
          }

        private :
          typedef std::set<value_type> set_t;
        public :
          typedef typename set_t::const_iterator const_iterator;
          typedef const_iterator iterator;
          typedef typename set_t::size_type size_type;

          const_iterator begin() const { return b; }
          const_iterator end() const {return C.end(); }
          size_type size() const { return C.size(); }

          bool remove(const value_type x) {
            const size_type s = C.size();
            assert(s >= 2);
            C.erase(x);
            assert(C.size() == s-1);
            assert(C.begin() == b);
            return s == 2;
          }

          typedef std::pair<OKlib::Satisfiability::Values::Sat_status,value_type> unit_return_t;

          template <class TotalAssignment>
          std::pair<OKlib::Satisfiability::Values::Sat_status,value_type> unit(const TotalAssignment& f) const {
            using namespace OKlib::Satisfiability::Values;
            assert(C.size() == 1);
            const value_type x = *b;
            assert(x != 0);
            switch (f[OKlib::Literals::var(x)]) {
            case val0 :
              if (OKlib::Literals::cond(x))
                return std::make_pair(falsified, value_type(0));
              else return std::make_pair(satisfied, value_type(0));
            case val1 :
              if (OKlib::Literals::cond(x))
                return std::make_pair(satisfied, value_type(0));
              else return std::make_pair(falsified, value_type(0));
            default : return std::make_pair(open, x);
            }
          }

          template <class TotalAssignment>
          OKlib::Satisfiability::Values::Sat_status update(const TotalAssignment& f) {
            using namespace OKlib::Satisfiability::Values;
            assert(C.size() >= 1);
            const iterator end(C.end());
            for (iterator i = C.begin(); i != end;) {
              const value_type x = *(i++);
              assert(x != 0);
              switch (f[OKlib::Literals::var(x)]) {
                case val0 :
                  if (OKlib::Literals::cond(x)) {
                    C.erase(i); break;
                  } else return satisfied;
                case val1 :
                  if (OKlib::Literals::cond(x)) return satisfied;
                  else {
                    C.erase(i); break;
                  }
                case unassigned : ;
              }
            }
            if (C.empty()) return falsified;
            assert(C.size() >= 2);
            return open;
          }

        private :

          set_t C;
          iterator b;
        };

      }
    }
  }
}

#endif
