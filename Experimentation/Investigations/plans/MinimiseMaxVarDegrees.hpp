// Oliver Kullmann, 2.6.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/plans/MinimiseMaxVarDegrees.hpp
  \brief Plans on investigations into the minimum of the max-var-degree of parameterised classes of unsatisfiable clause-sets


  \todo Low variable-degrees and high clause-lengths
  <ul>
   <li> The general question considers clause-sets F (boolean and also
   non-boolean) with minimal clause-length k and maximal variable-degree
   r. How low can r be so that still unsatisfiable instances are possible?
   </li>
   <li> One can restrict the clause-sets to uniform clause-sets. </li>
   <li> See plans in
   ComputerAlgebra/Satisfiability/Lisp/MinimalUnsatisfiability/plans/SmallVariableDegrees.hpp
   </li>
   <li> Or one considers only hitting clause-sets; see
   ComputerAlgebra/Satisfiability/Lisp/ConflictCombinatorics/plans/general.hpp.
   </li>
   <li> One can also demand variable- or literal-regularity (see also the
   considerations for hitting clause-sets). </li>
   <li> Instead variable- or literal-degrees one can consider the maximal
   degrees in the common-variable graph or on the conflict-graph (both
   now are clause-degrees). See again hitting clause-sets. </li>
   <li> Instead of minimal clause-length one can consider the deficiency. </li>
   <li> DONE This should become its own module. </li>
  </ul>


  \todo Unsatisfiable non-singular hitting clause-sets
  <ul>
   <li> Via lmin(map(max_variable_degree_cs,all_uhit_def(k)))
   we obtain the mininum of the max-var-degrees for a given deficiency
   of examples in the catalogue. </li>
   <li> Let minmaxvd(k) be the minimum of max_variable_degree_cs(F)
   for all non-singular unsatisfiable hitting clause-sets of deficiency k.
   </li>
   <li> The most basic question is whether minmaxvd(k) must grow with k. </li>
   <li> We know minmaxvd(2) = 4. </li>
   <li> We have examples showing minmaxvd(3) <= 6. </li>
   <li> And also minmaxvd(4) <= 6. </li>
   <li> And minmaxvd(5) <= 8. </li>
   <li> minmaxvd(6) <= 9. </li>
   <li> minmaxvd(7) <= 10. </li>
   <li> minmaxvd(8) <= 11. </li>
   <li> minmaxvd(8) <= 13. </li>
  </ul>

*/

