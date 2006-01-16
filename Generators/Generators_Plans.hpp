// Oliver Kullmann, 20.11.2005 (Swansea)

/*!
  \file Generators_Plans.hpp
  \brief Plans for the module Generators.
  \todo Transferring (and updating):
   - OKsolver/Experimental/Transformationen/LinInequal
   - OKsolver/Experimental/Transformationen/FaronsPuzzle
   - OKsolver/Experimental/AllgKlassen200203/TransformationsBiclique
  The main generators should produce generalised clause-sets.
  If output and/or boolean formulas is needed, then further
  transformers are to be used.
  The variables are tuples of values.
  \todo Especially for Ramsey- and Van der Waerden- formulas
  the generators should be active clause-sets usable by some SAT solver.
  And also we need active clause-sets for biclique-transformations for
  interesting classes of graphs (including complete graphs).
  \todo Transformers from non-boolean clause-sets to boolean
  clause-sets are needed (the standard translation, with or without
  the at-most-one-clauses, and also the other translations known from
  the literature; see [Prestwich; Local Search on SAT-encoded Colouring
  Problems; SAT 2003] for an overview, and also [Frisch,Peugniez;
  Solving Non-Boolean Satisfiability Problems with Stochastic Local
  Search; 2001]), and [Ansotegui,Manya; Mapping Problems with
  Finite-Domain Variables to Problems with Boolean Variables].
  \todo The CSP formats
   - http://cpai.ucc.ie/05/xml.html
   - http://cpai.ucc.ie/05/table.html
  are to be transformed into aliances of P-clause-sets (or specialised
  versions; perhaps just one P-clause-set is also alright; "mono-signed"
  full clauses per constraint are exactly enough).
  The benchmarks from http://cpai.ucc.ie/05/CallForSolvers.html should
  all be tried!
  \todo We should also have generators for factoring a number into
  two factors (at least using the standard multiplication method).
  Challenging (but potentially very interesting) are virtual clause-sets
  for this purpose.
  \todo Challenging (but potentially very interesting) is also the decryption
  of DES (a clause-set translation is in the literature).
*/

