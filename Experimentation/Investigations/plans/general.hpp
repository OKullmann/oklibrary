// Oliver Kullmann, 14.2.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/plans/general.hpp
  \brief Plans for the super-module for collecting data (not tools, but
  actual data)
  
  
  \todo Structure
  <ul>
   <li> Is the name "Investigations" right? </li>
   <li> Is the part Experimentation the right part? </li>
   <li> How shall we present the data? </li>
  </ul>


  \todo Create milestones.


  \todo Pigeonhole formulas
  <ul>
   <li> Collect the known data.
    <ol>
     <li> ComputerAlgebra/Satisfiability/Lisp/Backtracking/plans/OKsolver2002.hpp
     </li>
    </ol>
   </li>
   <li> Likely we should have a dedicated sub-module. </li>
   <li> Conflict-independence number:
    <ol>
     <li> weak_php(1,0) -> 1 </li>
     <li> weak_php(2,1) -> 2 </li>
     <li> weak_php(3,2) -> 6 </li>
     <li> weak_php(4,3) -> 18 </li>
     <li> weak_php(5,4) -> 40 </li>
     <li> weak_php(6,5) -> 75 </li>
     <li> weak_php(7,6) -> 126 </li>
    </ol>
    We should be able to figure this out; see below.
   </li>
   <li> The conflict-partition_number as upper-bounded by the length of
   hitting_decomposition_m_cs(weak_php(n+1,n)[2]) seems to be the
   same as the conflict-independence number. Perhaps the conflict-graph
   is even perfect?! </li>
   <li> In general: Is the hermitean deficiency actually an upper bound
   on the conflict-partition-number ?!? </li>
   <li> Hermitean rank and deficiency:
    <ol>
     <li> weak_php(1,0) -> [0,1] </li>
     <li> weak_php(2,1) -> [1,2] </li>
     <li> weak_php(3,2) -> [3,6] </li>
     <li> weak_php(4,3) -> [4,18] </li>
     <li> weak_php(5,4) -> [5,40] </li>
     <li> weak_php(6,5) -> [6,75] </li>
    </ol>
    We should be able to figure this out:
     <ol>
      <li> hermitean_rank(weak_php(m+1,m)) = m+1 </li>
      <li> hermitean_deficiency and conflict_independence_number follow
      suit. </li>
     </ol>
     Analogously to "eigensharp", we can create a notion for clause-sets where
    the conflict-indepence-number equals the hermitean defect, and show that
    weak_php is an instance (while not being eigensharp).
   </li>
   <li> The number of prime implicates for satisfiable php would be
   interesting. </li>
  </ul>


  \todo Sat-probability-approximations
  <ul>
   <li> Likely we should have a dedicated sub-module. </li>
   <li> Study the sequence of sat-probability-approximations (as computed by
   satprob_seqap in
   ComputerAlgebra/Satisfiability/Lisp/Counting/InclusionExclusion.mac). </li>
   <li> Clearly strict upper and lower bounds alternate, until the final
   entry is correct. </li>
   <li> One could conjecture that the distance to the correct value first
   strictly increases, and then strictly decreases.
    <ol>
     <li> Verified with weak_php(m+1,m) for 0 <= m <= 3. </li>
    </ol>
   </li>
   <li> Perhaps even stronger statements hold?!? </li>
  </ul>


  \todo Low variable-degrees and high clause-lengths
  <ul>
   <li> This should become its own module. </li>
   <li> The general question considers clause-sets F (boolean and also
   non-boolean) with minimal clause-length k and maximal variable-degree
   r. How low can r be so that still unsatisfiable instances are possible?
   </li>
   <li> One can restrict the clause-sets to uniform clause-sets. </li>
   <li> Since elements in MUSAT(1) have relatively the largest number
   of variables possible, one can only consider F in MUSAT(1).
   See
   ComputerAlgebra/Satisfiability/Lisp/MinimalUnsatisfiability/plans/general.hpp.
   </li>
   <li> Or one considers only hitting clause-sets; see
   ComputerAlgebra/Satisfiability/Lisp/ConflictCombinatorics/plans/general.hpp.
   </li>
   <li> One can also demand variable- or literal-regularity (see also the
   considerations for hitting clause-sets). </li>
   <li> Instead variable- or literal-degrees one can consider the maximal
   degrees in the common-variable graph or on the conflict-graph (both
   now are clause-degrees). See again hitting clause-sets. </li>
  </ul>


*/

