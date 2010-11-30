// Oliver Kullmann, 27.11.2010 (Swansea)
/* Copyright 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/Tools.hpp
  \brief On investigations into tools helping with solving van-der-Waerden problems


  \todo Script for searching for a vdW-number
  <ul>
   <li> Input is the vdW-parameter-tuple, and the start value for n. </li>
   <li> Then the script runs through n, n+1, ..., until the instance
   is unsatisfiable. </li>
   <li> See
   ExperimentSystem/ControllingLocalSearch/plans/PointOfUnsatisfiability.hpp.
   </li>
   <li> For problems with two parts only the SAT solver is needed as
   parameter, while for more parts also the translation method is needed. </li>
   <li> A complication arises when using incomplete algorithms for finding a
   satisfying assignment.
    <ol>
     <li> The main problem then is the number of runs and the cutoff. </li>
     <li> And at a certain point one has to try a complete solver. </li>
     <li> Perhaps all this needs a second script; so we have one script for
     running one complete solver, and one script for running one incomplete
     solver. </li>
    </ol>
   </li>
   <li> The script RunVdW3k is a first very simple tool, just using a fixed
   number of runs and a fixed cutoff. </li>
  </ul>


  \todo Version of RunVdW3k for palindromic problems
    <ol>
     <li> Use of solution for n-2:
      <ol>
       <li> Likely this solution should be translated, that is, interpreted
       as concerning now the vertices 2, ..., n-1, while vertices 1 and n
       are left open. </li>
      </ol>
     </li>
     <li> Later, one could increase cutoff or rounds, however in the first
     version we just stop. </li>
     <li> DONE
     Let's call it "RunPdVdW33k". </li>
     <li> DONE (we don't impose such restrictions, since we need to explore
     the whole range)
     An upper bound on the unsat-gap is given; say unsat_gap is 2 or 3.
     </li>
     <li> DONE (there are no sat- or unsat-gaps)
     That is, if unsat_gap consecutive "unsatisfiable" problems
     (according to the current settings) are found, then the first
     unsatisfiable problem is revisited. </li>
     <li> DONE (different from RunVdW33k, here solution-inheritance happens
     for n -> n+2)
     So RunPdVdW33k is very similar to RunVdW33k, except that it only
     stops after unsat-gap many unsatisfiable problems in a row, and that
     a different generator is used. </li>
    </ol>
   </li>

*/
