// Oliver Kullmann, 21.11.2006 (Swansea)

/*!
  \file SATAlgorithms/GenericBacktracking.hpp
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
       - Heur is the heuristics
       - Vis is the visitor

      \todo Test it.
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

      Backtracking(extended_problem_type& Problem, reduction_type reduction, heuristics_type heuristics, visitor_type& visitor, const bool all_solutions = false) :
        P(Problem), r(reduction), h(heuristics), vis(visitor), all(all_solutions) {}

      boost::logic::tribool operator()() {
        {
          const boost::logic::tribool initial_decision = r(P);
          if (initial_decision) { vis.satisfied(P); P.undo(); return true; }
          else if (not initial_decision) { vis.falsified(P); P.undo(); return false; }
        }
        typedef typename heuristics_type::literal_type literal_type;
        const literal_type& l(h(P));
        P.add_assignment(l);
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

