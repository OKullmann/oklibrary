// Oliver Kullmann, 21.11.2006 (Swansea)
/* Copyright 2006 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/Algorithms/Backtracking/GenericBacktracking.hpp
  \brief Generic backtracking (DPLL-like) solvers
*/

#ifndef GENERICBACKTRACKING_JJhb171Yh
#define GENERICBACKTRACKING_JJhb171Yh

#include <boost/logic/tribool.hpp>

namespace OKlib {
  namespace SATAlgorithms {

    /*!
      \class Backtracking
      \brief Generic backtracking solver (first prototype)

      Backtracking<ExtProblem, Red, Heur, Vis> has the following template parameters:
       - ExtProblem ist a class with the problem and the current partial assignment
       - Red is the reduction for every node
       - Heur is the heuristics (the branching literal)
       - Vis is the (decision-)visitor

       \todo Update

    */

    template <class ExtProblem, class Red, class Heur, class Vis>
    struct Backtracking {

      typedef ExtProblem extended_problem_type;
      typedef Red reduction_type;
      typedef Heur heuristics_type;
      typedef Vis visitor_type;

      extended_problem_type& P;
      reduction_type r;
      heuristics_type h;
      visitor_type& vis;
      const bool all;

      Backtracking(extended_problem_type& problem, reduction_type reduction, heuristics_type heuristics, visitor_type& visitor, const bool all_solutions = false) :
        P(problem), r(reduction), h(heuristics), vis(visitor), all(all_solutions) {}

      boost::logic::tribool operator()() {
        {
          const boost::logic::tribool initial_decision = r(P);
          if (initial_decision) { vis.satisfied(P); P.undo(); return true; }
          else if (not initial_decision) { vis.falsified(P); P.undo(); return false; }
        }
        typedef typename heuristics_type::literal_type literal_type;
        const literal_type& l(h(P));
        P.add_assignment(l); // do we have failed-literal reduction?
        {
          const boost::logic::tribool first_branch = Backtracking::operator()();
          P.undo(); if (first_branch and not all) return true;
        }
        P.add_negated_assignment(l);
        {
          const boost::logic::tribool second_branch = Backtracking::operator()();
          P.undo(); return second_branch;
        }
      }

    };

  }
}

#endif

