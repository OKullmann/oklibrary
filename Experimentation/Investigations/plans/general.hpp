// Oliver Kullmann, 14.2.2008 (Swansea)
/* Copyright 2008, 2011 Oliver Kullmann
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


  \todo Add section on AES experiments to be run NEEDS UPDATE
  <ul>
   <li> Experiments need to be run on AES instances with different types
   of SAT algorithm (local search, dpll etc) to see how things stand
   with the AES translation (See 
   ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/plans/Translations.hpp).
   </li>
   <li> Also experiments with the following need to be run :
    <ul>
     <li> Reduced round variants </li>
     <li> AES variants with different Sboxes. </li>
     <li> Analysis of good heuristics for AES (See "Experimental 
     investigations on heuristics") . </li>
    </ul>
   </li>
  </ul>


  \todo Translations from CSP to SAT
  <ul>
   <li> For the order-encoding and variations, especially for arithmetical
   constraints, see [Tamura, Taga, Kitagawa, Banbara, Compiling finite linear
   {CSP} into {SAT}, Constraints, 2009, 14(2):254-272, doi =
   10.1007/s10601-008-9061-0Open Access]. </li>
   <li> There is also the "compact order encoding" by the same people. </li>
   <li> See also "Sugar" in
   Buildsystem/ExternalSources/SpecialBuilds/plans/CSP.hpp. </li>
  </ul>

*/

