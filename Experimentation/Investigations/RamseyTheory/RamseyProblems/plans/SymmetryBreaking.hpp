// Matthew Gwynne, 28.7.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/RamseyProblems/plans/SymmetryBreaking.hpp
  \brief On performance of symmetry breaking techniques on %Ramsey problems


  \todo Performance of symmetry breaking by fixing monochromatic cliques
  <ul>
   <li> Systematic experiments need to be run examining the performance of
   different classes of solvers on various %Ramsey problems when additional
   symmetry breaking clauses are added, in particular, symmetry breaking 
   clauses derived from the "Fixing monochromatic cliques" method. </li>
   <li> MG ran some experiments using other solvers (picosat, minisat 2.1,
   hybridGM7 etc) which need to be included in the library (both the solvers 
   and the results. </li>
   <li> NOTE, for ramsey_2^2(5) <= 49, functions such as "ramsey2_symbr2_cs" (
   See 
   ComputerAlgebra/Satisfiability/Lisp/Generators/RamseyTheory/RamseyProblems.mac)
   when added to associated clause set, will fix a monochromatic clique
   of size 5 (as 49 is a *known* upper bound for ramsey_2^2(5)), which will 
   make the clause set unsatisfiable (as it should be). </li>
   <li> This is not a problem, but it should be noted that symmetry breaking
   using this method is not making some huge breakthrough here. </li>
   <li> See 
   ComputerAlgebra/Satisfiability/Lisp/Generators/RamseyTheory/RamseyProblems.mac.
   </li>
  </ul>


  \todo Using cardinality constraints
  <ul>
   <li> For ramsey_2^2(4) <= 18, OKsolver seems to perform worse (taking
   considerably longer per %node when monitoring at the same depth) when using
   cardinality constraints (that is, number of variables set to true restricted
   to between floor(153/2) and ceil(153/2)). </li>
   <li> However, this should be looked into further, as the time per node is 
   irrelevant if overall less nodes are used. </li>
   <li> Consider combining the PHP symmetry breaking, which already gives
   ramsey_2^2(4) = 18 in reasonable time, with the cardinality constraints
   (they are perfectly compatible). </li>
   <li> MG should include the results from initial experiments he did using
   various solvers (such as picosat, minisat, OKsolver_2002) etc. on this. 
   </li>
   <li> From the paper "Subgraph Counting Identities and Ramsey Numbers" by 
   McKay and Radziszowski, there are 656 non-isomorphic solutions for 
   "ramsey_2_2(5,5) > 42 ?" (available at 
   http://cs.anu.edu.au/~bdm/data/ramsey.html), where 16 of these have 
   floor(binomial(42,2)/2) = 430 edges of one label. </li>
   <li> However, there there are far more known solutions further from this 
   central number (this can also be seen in the distribution of all 
   ramsey_2^2(4,4) solutions), and with respect to this, Dr McKay offers that
   it seems that the solutions seem to like being slightly away from this 
   central number of edges (number of one label). </li>
   <li> When considering the conjecture, then, it seems reasonable to consider
   that perhaps a solution might only lie in a certain bound, within a certain
   distance of this central area (although there seem to be no available 
   counter-examples to the original conjecture)? </li>
   <li> See
   ComputerAlgebra/Satisfiability/Lisp/Generators/RamseyTheory/plans/RamseyProblems.hpp. 
   </li>
   <li> DONE MG should search for or e-mail McKay regarding the solutions for 
   high n values for ramsey_2^2(5) mentioned in [A new upper bound for 
   the %Ramsey number R(5,5)] and check to see whether this conjecture holds 
   in these cases. </li>
   <li> DONE MG has e-mailed McKay regarding this and is awaiting a reply. </li>
  </ul>

*/

