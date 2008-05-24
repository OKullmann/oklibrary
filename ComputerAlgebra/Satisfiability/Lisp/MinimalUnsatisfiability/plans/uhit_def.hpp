// Oliver Kullmann, 17.5.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/MinimalUnsatisfiability/plans/uhit_def.hpp
  \brief Plans regarding the catalogue of unsatisfiable non-singular hitting clause-sets


  \todo Connections to other modules
  <ul>
   <li> See ComputerAlgebra/Satisfiability/Lisp/ConflictCombinatorics/plans/HittingClauseSets.hpp
   </li>
  </ul>


  \todo Tests
  <ul>
   <li> Write tests for all functions.
    <ol>
     <li> uhit_def : DONE </li>
     <li> uhit_n : started </li>
     <li> apply_uhit </li>
     <li> collect_uhit_n </li>
     <li> classify_candidates_uhit_def </li>
     <li> isotype_uhit_def </li>
     <li> closure_uhit_def_pass </li>
    </ol>
   </li>
   <li> Testing functions which evaluate uhit_def:
    <ol>
     <li> Create mock-variations of uhit_def using dynamic binding. </li>
     <li> Perhaps we create a list of such mock-variations. </li>
    </ol>
   </li>
   <li> At level basic, the test for uhit_def takes too long. </li>
  </ul>


  \todo uhit_n
  <ul>
   <li> info_data should be computed. </li>
   <li> def_data should be computed. </li>
   <li> Run through all deficiencies in uhit_def and check whether
   the given n is contained. </li>
   <li> To determine whether the classification is complete, use
   two functions which compute lower and upper bounds on possible
   deficiencies for a given n, and then check whether for all these
   deficiencies the corresponding entries are complete. </li>
  </ul>


  \todo New entries
  <ul>
   <li> For n=5 add deficiencies delta = 14, ..., 27. </li>
   <li> It seems unlikely that we can get all for n=5. Nevertheless
   we should get a good variety, and for that we need a version of
   all_unsinghitting which starts from a different (random) path. </li>
   <li> The general policy about new entries should be that in case
   there is a large number of entries, and so we cannot have them all,
   then we should enter "interesting" examples:
    <ol>
     <li> Since we are investigating the maximal min-var-degree for a given
     deficiency (see "Maximal min-var-degrees" in
     ComputerAlgebra/Satisfiability/Lisp/MinimalUnsatisfiability/plans/general.hpp),
     examples where the bound is attained are interesting. </li>
     <li> Also of interest are ABDs. </li>
     <li> And examples for minimal and maximal n. </li>
    </ol>
   </li>
  </ul>


  \todo ExternalSources for larger data-sets
  <ul>
   <li> Larger data-sets should be made available as uhit_def*.mac
   data-sets under ExternalSources, loadable on demand. </li>
   <li> Process n=5,6,7. </li>
   <li> For n=5 add deficiencies delta = 5,6,7,8,9,10,11. </li>
  </ul>


  \todo Extending the catalogue with other forms of clause-sets
  <ul>
   <li> A second catalogue should be created, which contains only
   those unsatisfiable hitting clause-sets for which
   no 2-subsumption resolution is possible:
    <ol>
     <li> Call it uhit_def_n2s. </li>
     <li> It seems not sensible to ask also for non-singularity. </li>
     <li> From this catalogue all of uhit_def can be created by inverse
     2-subsumption resolution. </li>
     <li> We should get much smaller numbers.
      <ol>
       <li> Though the process of reduction by 2-subsumption-resolution
       is not confluent, and for a given (non-singular) hitting clause-set
       F there can exist 2 non-isomorphic hitting clause-sets reduces w.r.t.
       2-subsumption-resolution which can be expanded to F. </li>
       <li> Obvious examples are given by reducing full_fcs(n). </li>
       <li> So for full_fcs(n) there exist many representations, the optimal
       representing full_fcs(n) as the expansion of {{}} by setn(n). </li>
      </ol>
     </li>
     <li> The entries in this new catalogue shall form a subset of uhit_def.
     </li>
     <li> The structure of this catalogue is the same as of uhit_def. </li>
     <li> We need a function which runs through the elements of uhit_def,
     and returns those not in uhit_def_n2s. </li>
    </ol>
   </li>
  </ul>

*/

