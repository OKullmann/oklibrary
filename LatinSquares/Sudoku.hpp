// Oliver Kullmann, 21.11.2006 (Swansea)

/*!
  \file LatinSquares/Sudoku.hpp
  \brief Components for solving Sudoku problems
*/

#ifndef SUDOKU_jJJnbfr44ttr
#define SUDOKU_jJJnbfr44ttr

#include <utility>
#include <stack>

#include <boost/logic/tribool.hpp>

#include <Transitional/PartialAssignments/MultivaluedPartialAssignments.hpp>

namespace OKlib {
  namespace LatinSquares {

    /*!
      \class SudokuProblem
      \brief First prototype for an active clause-set for Sudoku

      \todo Complete implementation:
       - Are all constraints stored together? Perhaps better we use the natural grouping
         into 3 groups?
      \todo Test it.
    */

    template <int Box_size, class AllDifferentConstraint>
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

      typedef AllDifferentConstraint constraint_type;

      typedef OKlib::PartialAssignments::MultiPASS<entry_type, n> partial_assignment_type;

      SudokuProblem(partial_assignment_type& phi) : phi(phi) {
        // set up constraints XXXXXXXXXXXXXX
      }

      typedef typename partial_assignment_type::variables_type variables_type;
      typedef typename partial_assignment_type::domain_type domain_type;
      typedef typename partial_assignment_type::literal_type literal_type;

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

    };

    // #################################################

    /*!
      \class Trivial_reduction
      \brief Trivial reduction for a Sudoku problem

      \todo Complete it.
      \todo Test it.
    */

    template <class SudokoP>
    class Trivial_reduction {

    public :

      typedef SudokoP sudoko_type;
      typedef typename sudoko_type::literal_type literal_type;

      boost::logic::tribool operator()(const sudoko_type& P) {
        /// Check the constraints XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
      }


    };

  }
}

#endif

