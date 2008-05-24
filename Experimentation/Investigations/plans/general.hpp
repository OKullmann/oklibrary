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


  \todo Experimental investigations on heuristics
  <ul>
   <li> Above we have precise data on families of problem instances. </li>
   <li> Is now this supermodule also the right place for collecting
   data about computational experiments? </li>
   <li> Finally, a database is needed (like SatEx), and this should go
   somewhere else. </li>
   <li> Taking [Hooker, Vinay, 1995, Branching Rules for Satisfiability]
   as a starting point, we should completely update the data, using
   new heuristics and benchmarks (and also various evaluation techniques).
   But we should stay within the lookahead-paradigm, but perhaps generalised
   on the strength of reduction and look-ahead. </li>
  </ul>

*/

