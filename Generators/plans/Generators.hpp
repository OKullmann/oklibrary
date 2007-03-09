// Oliver Kullmann, 20.11.2005 (Swansea)

/*!
  \file Generators/plans/Generators.hpp
  \brief Plans for the module Generators (for creating SAT problems)

  \todo Transferring (and updating):
   - OKsolver/Experimental/Transformationen/FaronsPuzzle
   - OKsolver/Experimental/AllgKlassen200203/TransformationsBiclique
  The main generators should produce generalised clause-sets.
  If output and/or boolean formulas is needed, then further
  transformers are to be used.
  The variables are tuples of values.

  \todo Implementing the Tseitin-transformation (with variations):
  <ol>
   <li>
   Given a propositional formula F built from negation, conjunction
   and disjunction (for simplicity we consider only binary operations
   here), by the process specified below, a refinement
   of the Tseitin-translation, we obtain a 3-CNF F' over the variables
   from F plus new auxiliary variable such that

   - every satisfying assignment for F can (easily) be extended
     to a satisfying assignment for F';
   - every satisfying assignment for F' is a satisfying assignment
     for F (when restricted to the variables in F).

   The process F -> F' works, using auxiliary variables v(G) for
   subformulas G of F, by collecting clauses (to avoid renaming
   of existing variables, let v(a) = a for variables a from F):

   For a sub-formula G of F, where G is of the form
     op G_1 ... G_n
   (for us n <= 2), with op one of negation, conjunction or disjunction
   (written for convenience in prefix form), add the clauses

   i) (op v(G_1) ... v(G_n)) -> v(G)
   ii) v(G) -> (op v(G_1) ... v(G_n))

   where in case G occurs positively in F, i) can be dropped,
   while if G occurs negatively in F, ii) can be dropped
   (so the process might be non-deterministic).

   F' is obtained by adding the unit clause {v(F)} to all these clauses.
   </li>
   <li> Is there a general input format for boolean formulas? </li>
   <li> Some obvious variations on the transformation F -> F':
    - handling of arbitrary and-or-arities
    - performing simplifications on the clauses
    - either using always (i)+(ii), or exactly one of them, or using some
      heuristic.
   </li>
  </ol>

  \todo And also we need active clause-sets for biclique-transformations for
  interesting classes of graphs (including complete graphs).

  \todo The HKB-formulas (MUSAT_{delta=2}, where every literal occurs
  at least once) have to be constructed.

  \todo Generator for PHP, and more generally for matching formulas,
  given an arbitrary graph and a set of vertices to be matched.
  Directly, and as active clause-sets.

  \todo Generators for finding incidence structures and designs
  Naturally modelled by boolean variables I(p, b) ("I" like
  "incident", "p" like "point", "b" like "block").
  Searching for example for balanced designs.
  Very simple the search for projective planes of order n:
   - n^2 + n + 1 p's and b's
   - for all p <> p' there is exactly one b with I(p,b) and I(p',b)
   - for all b <> b' there is exactly one p with I(p, b) and I(p, b')
   - for all b there are two p, p' of p1,p2,p3,p4 (fixed variables!)
     with not I(p, b) and not I(p', b).
  Modelled directly or via active clause-sets (especially that every two points
  are on one line, and every two lines intersect in one point!).
  QUESTION OK: Where does this topic belong to? To module LatinSquares?
  Or perhaps we should have a dedicated module on projective plans etc.?
  Sounds reasonable, but only after we really have something to start.

  \todo Very interesting are also the Extended Resolution extensions of PHP
  (and potentially other classes): Seeing the performance of SAT solvers
  on them would be interesting (or perhaps one can tune the SAT solvers
  to make good use of the extension clauses?!).

  \todo Transformers from non-boolean clause-sets to boolean
  clause-sets are needed (the standard translation, with or without
  the at-most-one-clauses, and also the other translations known from
  the literature; see [Prestwich; Local Search on SAT-encoded Colouring
  Problems; SAT 2003] for an overview, and also [Frisch,Peugniez;
  Solving Non-Boolean Satisfiability Problems with Stochastic Local
  Search; 2001]), and [Ansotegui,Manya; Mapping Problems with
  Finite-Domain Variables to Problems with Boolean Variables].

  \todo Implement the general construction of Soeren Riis (especially
  interesting when we know that the formulas have short refutations).

*/

/*!
  \namespace OKlib::Generators
  \brief Generators for (active) clause-sets (which are not yet in their own module).
*/

namespace OKlib {
  namespace Generators {
  }
}

