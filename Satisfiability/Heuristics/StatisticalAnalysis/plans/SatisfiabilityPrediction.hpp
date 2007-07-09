// Oliver Kullmann, 16.6.2007 (Swansea)

/*!
  \file Heuristics/StatisticalAnalysis/plans/SatisfiabilityPrediction.hpp
  \brief Submodule for computing approximated SAT-probabilities


  \todo The simplest method is to compute the (exact) probability
  that a random assignment satisfies the instance (used in the old
  OKsolver).
  <ul>
   <li> A generic algorithm is needed. </li>
   <li> Are there refinements? For example one could impose a special
   distribution on the assignments (for example some variables have a
   higher probability)?! </li>
  </ul>


  \todo Approximating the exact probability via inclusion-exclusion
  <ul>
   <li> The 0-level formula (which assumes all clauses are clashing)
   needs a generic implementation (one can
   use the random-assignment method, since only the coefficients
   are different). </li>
   <li> Then a generic implementation of the level-1 formula (where now
   pairs of clauses are considered, and their overlap is subtracted)
   is needed. </li>
   <li> For constant k a meta-programming implementation of the k-level
   formula would be nice. </li>
   <li> And finally an implementation of arbitrary k (at run-time). </li>
  </ul>


  \todo Experimentation
  <ul>
   <li> Experimentation with the precision of the various methods (first
   on random formulas). </li>
  </ul>


  \todo Counting falsifying assignment
  <ul>
   <li> For counting falsifying assignments, more efficient probabilistic
   methods exists. </li>
   <li> One could use these values for prediction of satisfiability. </li>
  </ul>

*/
