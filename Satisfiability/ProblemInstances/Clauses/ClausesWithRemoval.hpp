// Oliver Kullmann, 12.12.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ProblemInstances/Clauses/ClausesWithRemoval.hpp
  \brief Clause-classes which allow removal of literals

  Especially appropriate for (simple) unit-clause propagation.

*/

#ifndef CLAUSESWITHREMOVAL_alakajaFr4
#define CLAUSESWITHREMOVAL_alakajaFr4

#include <vector>
#include <set>
#include <cassert>
#include <utility>

#include <OKlib/Satisfiability/ProblemInstances/Literals/var.hpp>
#include <OKlib/Satisfiability/Values/StatusTypes.hpp>

namespace OKlib {
  namespace Satisfiability {
    namespace ProblemInstances {
      namespace Clauses {

        /*!
          \class RClausesAsVectors
          \brief Clauses with remove-functionality, implemented by vectors
        */

        template <class Lit>
        class RClausesAsVectors {

        public :

          typedef Lit value_type;

          template <class LiteralList> RClausesAsVectors(
            const typename LiteralList::const_iterator begin,
            const typename LiteralList::const_iterator end) :
            C(begin,end), b(C.begin()), e(C.end()), s(C.size()) {
              assert(s >= 2);
            }

          RClausesAsVectors(const RClausesAsVectors& D) : C(D.C), b(C.begin()), e(C.end()), s(D.s) {
            assert(C.size() >= 2);
            assert(s >= 1);
          }

          friend bool operator ==(const RClausesAsVectors& lhs, const RClausesAsVectors& rhs) {
            return lhs.C == rhs.C and lhs.s == rhs.s;
          }

        private :
          typedef std::vector<const Lit> vector_t;
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

          template <class TotalAssignment>
          std::pair<OKlib::Satisfiability::Values::Sat_status,value_type> unit(const TotalAssignment& f) const {
            using namespace OKlib::Satisfiability::Values;
            assert(s == 1);
            for (iterator i = b; i != e; ++i)
              switch (f[OKlib::Literals::var(*i)]) {
              case val0 : continue;
              case val1 : return std::make_pair(satisfied, value_type(0));
              case unassigned : return std::make_pair(open, *i);
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
                case val0 : continue;
                case val1 : return satisfied;
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

          const RClausesAsVectors& operator =(const RClausesAsVectors& rhs); // not provided
          vector_t C;
          iterator b;
          iterator e;
          size_type s;
        };


        /*!
          \class RClausesAsSets
          \brief Clauses with remove-functionality, implemented by sets
        */

        template <class Lit>
        class RClausesAsSets {

        public :

          typedef Lit value_type;

          template <class LiteralList> RClausesAsSets(
            const typename LiteralList::const_iterator begin,
            const typename LiteralList::const_iterator end) :
            C(begin,end), b(C.begin()) {
              assert(C.size() >= 2);
            }

          RClausesAsSets(const RClausesAsSets& D) : C(D.C), b(C.begin()) {
            assert(C.size() >= 2);
          }

          friend bool operator ==(const RClausesAsSets& lhs, const RClausesAsSets& rhs) {
            return lhs.C == rhs.C;
          }

        private :
          typedef std::set<const Lit> set_t;
        public :
          typedef typename set_t::const_iterator const_iterator;
          typedef const_iterator iterator;
          typedef typename set_t::size_type size_type;

          iterator begin() const { return b; }
          iterator end() const {return C.end(); }
          size_type size() const { return C.size(); }

          bool remove(const value_type x) {
            const size_type s = C.size();
            assert(s >= 2);
            C.erase(x);
            return s == 2;
          }

          template <class TotalAssignment>
          std::pair<OKlib::Satisfiability::Values::Sat_status,value_type> unit(const TotalAssignment& f) const {
            using namespace OKlib::Satisfiability::Values;
            assert(C.size() == 1);
            const value_type x = *b;
            switch (f[OKlib::Literals::var(x)]) {
            case val0 : return make_pair(falsified, value_type(0));
            case val1 : return std::make_pair(satisfied, value_type(0));
            case unassigned : return std::make_pair(open, x);
            }
          }

          template <class TotalAssignment>
          OKlib::Satisfiability::Values::Sat_status update(const TotalAssignment& f) {
            using namespace OKlib::Satisfiability::Values;
            assert(C.size() >= 1);
            const iterator end(C.end());
            for (iterator i = b; i != end;) {
              const value_type x = *(i++);
              switch (f[OKlib::Literals::var(x)]) {
                case val0 : C.erase(x); break;
                case val1 : return satisfied;
                case unassigned : continue;
              }
            }
            if (C.empty()) return falsified;
            assert(C.size() >= 2);
            return open;
          }

        private :

          const RClausesAsSets& operator =(const RClausesAsSets& rhs); // not provided
          set_t C;
          const iterator b;
        };

      }
    }
  }
}

#endif
