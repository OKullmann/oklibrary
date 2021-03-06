// Oliver Kullmann, 25.12.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file OKlib/Satisfiability/ProblemInstances/Clauses/WatchedLiterals.hpp
  \brief Clause-classes supporting watched literals
*/

#ifndef WATCHEDLITERALS_nNHpet5t5
#define WATCHEDLITERALS_nNHpet5t5

#include <vector>
#include <cassert>

#include <OKlib/Satisfiability/ProblemInstances/Literals/traits/var_type.hpp>
#include <OKlib/Satisfiability/Values/StatusTypes.hpp>

namespace OKlib {
  namespace Satisfiability {
    namespace ProblemInstances {
      namespace Clauses {

        /*!
          \class WatchedLiterals_mono
          \brief Clause-type with two watched literals, where the user can't distinguish between them

          See ProblemInstances/Clauses/concepts/WatchedLiterals.hpp
          for the concept.

          \todo Complete the concept

          \todo Provide unit-tests

          \todo Complete sequence-member-functions

          \todo Improve implementation
          <ul>
           <li> Likely we should provide a specialisation for size 3. </li>
           <li> Are members b, e really helpful (w.r.t. speed)? </li<
          </ul>
        */

        template <class Lit>
        class WatchedLiterals_mono {
        public :

          typedef Lit value_type;
          typedef typename OKlib::Literals::traits::var_type<value_type>::type var_type;

        private :

          typedef std::vector<value_type> clause_type;

        public :

          typedef typename clause_type::const_iterator const_iterator;
          typedef const_iterator iterator;
          typedef typename clause_type::size_type size_type;

          const_iterator begin() const { return b; }
          const_iterator end() const { return e; }
          size_type size() const { return C.size(); }
          bool empty() const { return false; }

          template <typename InputIterator>
          WatchedLiterals_mono(const InputIterator b_, const InputIterator e_)
            : C(b_,e_), b(C.begin()), e(C.end()), i1(b), i2(e-1), w1(*i1), w2(*i2) {
            assert(C.size() >= 3);
            assert(*i1 == w1);
            assert(*i2 == w2);
          }
          template <typename InputIterator>
          void assign(const InputIterator b_, const InputIterator e_) {
            C.assign(b_,e_);
            assert(C.size() >= 3);
            b = C.begin(); e = C.end();
            i1 = b; i2 = e-1;
            w1 = *i1; w2 = *i2;
          }

          WatchedLiterals_mono(const WatchedLiterals_mono& w)
            : C(w.C), b(C.begin()), e(C.end()), i1(b), i2(e-1), w1(w.w1), w2(w.w2) {
            assert(*i1 == w1);
            assert(*i2 == w2);
          }

          WatchedLiterals_mono& operator= (const WatchedLiterals_mono& rhs) {
            C = rhs.C;
            b = C.begin(); e = C.end();
            i1 = b; i2 = e-1;
            w1 = *i1; w2 = *i2;
            return *this;
          }

          value_type first() const { assert(*i1==w1); return w1; }
          value_type second() const { assert(*i2==w2); return w2; }

          template <class Assignment>
          value_type remove(const value_type x, Assignment& f) {
            using namespace OKlib::Satisfiability::Values;
            if (x == w1) {
              for (const_iterator i = i1+1; i != e; ++i) {
                if (i == i2) continue;
                const Assignment_status val = f(*i);
                switch (val) {
                case val1 : return x;
                case unassigned : i1 = i; w1 = *i; return w1;
                default : continue;
                }
              }
              for (const_iterator i = b; i != i1; ++i) {
                if (i == i2) continue;
                const Assignment_status val = f(*i);
                switch (val) {
                case val1 : return x;
                case unassigned : i1 = i; w1 = *i; return w1;
                default : continue;
                }
              }
              if (f.push(w2)) return x;
              else return 0;
            }
            else {
              assert(x == w2);
              if (i2 != b)
                for (const_iterator i = i2-1; i != b; --i) {
                  if (i == i1) continue;
                  const Assignment_status val = f(*i);
                  switch (val) {
                  case val1 : return x;
                  case unassigned : i2 = i; w2 = *i; return w2;
                  default : continue;
                  }
                }
              if (i1 != b) {
               const Assignment_status val = f(*b);
               switch (val) {
               case val1 : return x;
               case unassigned : i2 = b; w2 = *b; return w2;
               default : ;
               }
              }
              for (const_iterator i = e-1; i != i2; --i) {
                if (i == i1) continue;
                const Assignment_status e = f(*i);
                switch (e) {
                case val1 : return x;
                case unassigned : i2 = i; w2 = *i; return w2;
                default : continue;
                }
              }
              if (f.push(w1)) return x;
              else return 0;
            }
          }

        private :

          clause_type C;
          const_iterator b, e;
          const_iterator i1, i2;
          value_type w1, w2;
        };

      }
    }
  }
}

#endif
