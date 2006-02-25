// Oliver Kullmann, 20.11.2005 (Swansea)

/*!
  \file Generators_Plans.hpp
  \brief Plans for the module Generators.
  \todo Transferring (and updating):
   - OKsolver/Experimental/Transformationen/FaronsPuzzle
   - OKsolver/Experimental/AllgKlassen200203/TransformationsBiclique
  The main generators should produce generalised clause-sets.
  If output and/or boolean formulas is needed, then further
  transformers are to be used.
  The variables are tuples of values.
  \todo And also we need active clause-sets for biclique-transformations for
  interesting classes of graphs (including complete graphs).
  \todo The HKB-formulas (MUSAT_{delta=2}, where every literal occurs
  at least once) have to be constructed.
  \todo Generator for PHP, and more generally for matching formulas,
  given an arbitrary graph and a set of vertices to be matched.
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
   \todo We should also have generators for factoring a number into
  two factors (at least using the standard multiplication method).
  Challenging (but potentially very interesting) are virtual clause-sets
  for this purpose.
  \todo Challenging (but potentially very interesting) is also the decryption
  of DES (a clause-set translation is in the literature).
*/

/*!
  \namespace OKlib::Generators
  \brief Generators for (active) clause-sets (which are not yet in their own module).
*/

namespace OKlib {
  namespace Generators {
  }
}

