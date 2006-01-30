// Matthew Henderson, 25.1.2006 (Swansea)

/*!
  \file InputOutput_Plans.hpp
  \brief 
  \todo Satisfiability of total assignment.

  Syntax: 

      assert( apply_total_assignment(formula, assignment) == 1 )

  Semantics:

      This assert is to be interpreted that the result of plugging 
      the assignment into the formula is true.

      apply_total_assignment is a freestanding template function with 
      parameters for the formula type and the assignment type. The 
      function goes through each clause and checks the assignment for
      a satisfying literal.

      template <class Formula, class Assignment>
      bool apply_total_assignment( Formula formula, Assignment assignment ) {
        typedef formula::const_iterator iterator;
        const iterator& end(formula.end());
        for (iterator clause(formula.begin()); clause!=end; ++clause) {
          typedef typename FormulaTraits<Formula>::clause_type::const_iterator cl_iterator;
          const iterator cl_end(clause.end());
            for ( iterator literal(clause.begin()); literal!=cl_end; ++literal ) {
              if ( !  satisfied_by( literal, assignment )  ):
                return false; 
            }
            
        } 
        return true; // made it through, all clauses satisfied.

      Now we have to implement satisfied_by(literal,assignment).
      
      Here the implementation depends very much on the assignment type, in
      our case we simply compare the signs of literal and assignment[literal]
      for equality.

      satisfied_by(int literal, vector<int> assignment) {
        return (literal > 0 && assignment[literal] > 0) | (literal < 0 && assignment[literal] < 0);
      }


   Remarks:

      We need to get the clause_type from the formula if the formula
      is something like vector<set<int>>? For this we need a traits
      class.

      template<typename T> class FormulaTraits;

      template<> class FormulaTraits< vector<set<int> > > {
        public:
          typedef set<int> clause_type;
      }

      Eventually there has to be a way of getting the formula
      and assignment from files (more generally from streams).
      Here there is already the submodule Dimacs.
*/

namespace OKlib {
  namespace InputOutput {
  }
}
