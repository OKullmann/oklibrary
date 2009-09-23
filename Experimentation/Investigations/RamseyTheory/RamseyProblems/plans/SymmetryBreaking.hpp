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
   <li> MG should include the results from initial experiments he did using
   various solvers (such as picosat, minisat, OKsolver_2002) etc. on this. 
   </li>
   <li> MG should search for or e-mail McKay regarding the solutions for 
   high n values for ramsey_2^2(5) mentioned in [A new upper bound for 
   the %Ramsey number R(5,5)] and check to see whether this conjecture holds 
   in these cases. </li>
   <li> MG has e-mailed McKay regarding this and is awaiting a reply. </li>
   <li> See
   ComputerAlgebra/Satisfiability/Lisp/Generators/RamseyTheory/plans/RamseyProblems.hpp. 
   </li>
  </ul>

*/

