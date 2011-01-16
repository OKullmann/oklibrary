// Oliver Kullmann, 25.12.2009 (Swansea)
/* Copyright 2009, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file OKlib/Satisfiability/Assignments/TotalAssignments/AssignmentsWithBuffer.hpp
  \brief Components for assignments with buffers (for processing of single assignments)

*/

#ifndef ASSIGNMENTSWITHBUFFER_nBaLk810
#define ASSIGNMENTSWITHBUFFER_nBaLk810

#include <vector>
#include <stack>
#include <cassert>

#include <OKlib/Satisfiability/ProblemInstances/Variables/traits/index_type.hpp>
#include <OKlib/Satisfiability/ProblemInstances/Literals/traits/var_type.hpp>
#include <OKlib/Satisfiability/ProblemInstances/Literals/traits/cond_type.hpp>
#include <OKlib/Satisfiability/ProblemInstances/Literals/var.hpp>
#include <OKlib/Satisfiability/ProblemInstances/Literals/cond.hpp>
#include <OKlib/Satisfiability/Values/StatusTypes.hpp>

namespace OKlib {
 namespace Satisfiability {
  namespace Assignments {
   namespace TotalAssignments {

     /*!
       \class BAssignmentWithQueue
       \brief Boolean assignments with queue for (elementary) assignments to be processed

       \todo Provide specification

       \todo Write unit-tests

       \todo Improve implementation
       <ul>
        <li> Regarding member functions push, [] and () it seems that the
        code needs clean-up w.r.t. the various auxiliary values computed. </li>
       </ul>

     */

     template <typename Lit>
     class BAssignmentWithQueue {
     public :

       typedef Lit literal_type;
       typedef typename OKlib::Literals::traits::var_type<literal_type>::type variable_type;
       typedef typename OKlib::Variables::traits::index_type<variable_type>::type index_type;

       typedef OKlib::Satisfiability::Values::Assignment_status value_type;

       //! n is the maximal variable index
       BAssignmentWithQueue() : n(0), next_lit(phi.begin()) {}
       BAssignmentWithQueue(const index_type n_) : n(n_), V(n+1,OKlib::Satisfiability::Values::unassigned) {
         assert(n >= 0);
         phi.reserve(n);
         next_lit = phi.begin();
       }
       BAssignmentWithQueue(const BAssignmentWithQueue& other) :
         n(other.n), V(other.V), phi(other.phi), next_lit(phi.end() - other.size()) {
         phi.reserve(n);
         assert(size() == other.size());
       }
       BAssignmentWithQueue& operator =(const BAssignmentWithQueue& rhs) {
         n = rhs.n;
         V = rhs.V;
         phi = rhs.phi;
         phi.reserve(n);
         next_lit = phi.end() - rhs.size();
         assert(size() == rhs.size());
         return *this;
       }

       //! enlarging the capacity
       void resize(const index_type n_) {
         assert(n_ >= 0);
         n = n_;
         V.resize(n+1,OKlib::Satisfiability::Values::unassigned);
         phi.reserve(n);
         next_lit = phi.begin();
       }

       //! the value of the partial assignment for variable v
       value_type operator[] (const variable_type v) const {
         assert(index_type(v) <= n);
         return V[index_type(v)];
       }
       //! the value of the partial assignment for literal x
       value_type operator() (const literal_type x) const {
         assert(index_type(OKlib::Literals::var(x)) <= n);
         if (OKlib::Literals::cond(x))
           return V[index_type(OKlib::Literals::var(x))];
         else
           return -V[index_type(OKlib::Literals::var(x))];
       }

       /*!
         \brief push x -> 1 on the buffer and enter into the assignment, in
         both cases only if not already present, checking with the current
         assignment; returns false iff inconsistent with current assignment
         (and thus not pushed)
       */
       bool push(const literal_type x) {
         assert(index_type(OKlib::Literals::var(x)) <= n);
         switch (operator()(x)) {
         case OKlib::Satisfiability::Values::val0 :
           return false;
         case OKlib::Satisfiability::Values::val1 :
           return true;
         default :
           phi.push_back(x);
           V[index_type(OKlib::Literals::var(x))] = value_type(OKlib::Literals::cond(x));
           return true;
         }
       }

       //! return the next literal (assigned to true) to be processed
       literal_type top() const {
         assert(not empty());
         return *next_lit;
       }
       //! remove the next literal to be processed
       void pop() {
        assert(next_lit != phi.end());
         ++next_lit;
       }
       //! the size of the buffer
       index_type size() const { return phi.end() - next_lit; }
       //! whether the buffer is empty
       bool empty() const { return next_lit == phi.end(); }

     private :

       index_type n;
       typedef std::vector<value_type> vector_t;
       vector_t V;
       typedef std::vector<literal_type> pass_t;
       pass_t phi;
       typedef typename pass_t::const_iterator iterator_t;
       iterator_t next_lit;
     };


     /*!
       \class BAssignmentWithRQueue
       \brief Boolean assignments with resettable queue for (elementary) assignments to be processed

       \todo Complete.
     */

     template <typename Lit>
     class BAssignmentWithRQueue : public BAssignmentWithQueue<Lit> {
       typedef BAssignmentWithQueue<Lit> base_class;
     public :

       BAssignmentWithRQueue(const typename base_class::index_type n) : base_class(n) {}

       bool push_forced(const typename base_class::literal_type x) {
         return this -> push(x);
       }
       bool push_free(const typename base_class::literal_type x) {
         if (not this -> push_forced(x)) return false;
         S.push(-- (this -> phi.end()));
         return true;
       }

       void reset() {
         // XXX
       }

     private :

       using typename base_class::iterator_t;
       typedef std::stack<typename base_class::iterator_t> stack_t;
       stack_t S;
     };

    

   } 
  }
 }
}

#endif
