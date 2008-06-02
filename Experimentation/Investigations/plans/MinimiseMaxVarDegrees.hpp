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
   <li> DONE This should become its own module. </li>
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
  </ul>

*/

