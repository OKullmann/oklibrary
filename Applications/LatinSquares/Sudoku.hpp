// Oliver Kullmann, 21.11.2006 (Swansea)
/* Copyright 2006 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Applications/LatinSquares/Sudoku.hpp
  \brief Components for solving Sudoku problems
*/

#ifndef SUDOKU_jJJnbfr44ttr
#define SUDOKU_jJJnbfr44ttr

#include <utility>
#include <stack>
#include <tr1/array>

#include <boost/logic/tribool.hpp>

#include <OKlib/Satisfiability/Assignments/PartialAssignments/MultivaluedPartialAssignments.hpp>
#include <OKlib/Satisfiability/ProblemInstances/Variables/TrivialVariables.hpp>

namespace OKlib {
  namespace LatinSquares {

    /*!
      \class SudokuProblem
      \brief First prototype for an alliance of active clause-sets for Sudoku

      \todo Once we have fixed the concepts (a bit more), likely we should rename
      the parameter BijectivityConstraint.

      \todo More doxygen-documentation is needed.
    */

    template <int Box_size, class BijectivityConstraint>
    class SudokuProblem {

    public :

      static const int N = Box_size;
      static const int N2 = N * N;
      static const int n = N2 * N2;

      typedef int box_index; // 1 .. Box_size
      typedef int square_index; // 1 .. N2
      typedef int entry_type; // 1 .. N2

      typedef std::pair<box_index, box_index> box_point_type;
      typedef std::pair<box_point_type, box_point_type> point2_type;
      typedef std::pair<square_index, square_index> point_type;

      typedef BijectivityConstraint constraint_type;

      typedef OKlib::Variables::Variables_unsigned_int variables_type;
      typedef OKlib::PartialAssignments::MultiPASS<entry_type, n, variables_type> partial_assignment_type;
      typedef typename partial_assignment_type::domain_type domain_type;
      typedef typename partial_assignment_type::literal_type literal_type;


      SudokuProblem(partial_assignment_type& phi) : phi(phi) {
        
        // set up constraints XXXXXXXXXXXXXX
      }

      point_type point(const point2_type& p) const {
        assert(p.first.first >= 1);
        assert(p.first.first <= N);
        assert(p.first.second >= 1);
        assert(p.first.second <= N);
        assert(p.second.first >= 1);
        assert(p.second.first <= N);
        assert(p.second.second >= 1);
        assert(p.second.second <= N);
        return point_type((p.first.first - 1) * N + p.second.first, (p.first.second - 1) * N + p.second.second);
      }

      variables_type var(const point_type& p) const {
        assert(p.first >= 1);
        assert(p.second <= N2);
        return (p.first - 1) * N2 + p.second;
      }

      void add_assignment(const literal_type& l) {
        stack.push(phi.get_token());
        phi.set(l.first, l.second);
      }

      void undo() {
        assert(not stack.empty);
        phi.undo(stack.top());
        phi.pop();
      }

    private :

      partial_assignment_type phi;
      typedef typename partial_assignment_type::token_type token_type;
      typedef std::stack<token_type> stack_type;
      stack_type stack;

      std::tr1::array<constraint_type, 3 * N2> constraints;
      typedef int constraint_index; // 0 .. 3*N2-1
      enum constraint_types {row_constraint, column_constraint, box_constraint};

      constraint_index index_any(const square_index i, const constraint_types t) {
        assert(i >= 1);
        assert(i <= N2);
        switch (t) {
        case row_constraint : return i - 1;
        case column_constraint : return N2 - 1 + i;
        case box_constraint : return 2 * N2 - 1 + i;
        }
      }
      constraint_index index_box(const box_point_type& p) {
        assert(p.first >= 1);
        assert(p.first <= N2);
        assert(p.second >= 1);
        assert(p.second <= N2);
        return 2 * N2 - 1 + N * (p.first - 1) + p.second;
      }

    };

    // #################################################

    /*!
      \class Trivial_reduction_Sudoku
      \brief Trivial reduction for a Sudoku problem

      Checks only whether the constraints either all are satisfied or one has been falsified.
      \todo This should be the responsibility of the alliance, or? (Like "generalised UCP" ?!)
    */

    template <class SudokuP>
    class Trivial_reduction_Sudoku {

    public :

      typedef SudokuP sudoku_type;
      typedef typename sudoku_type::literal_type literal_type;

      boost::logic::tribool operator()(const sudoku_type& P) {
        /// Check the constraints XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
      }


    };

  }
}

#endif

