// Matthew Gwynne, 27.3.2009 (Swansea)
/* Copyright 2009, 2010, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/Cryptography/AdvancedEncryptionStandard/plans/general.hpp
  \brief On investigations into the Advanced Encryption Standard


  \todo Just an unstructured morass of scripts
  <ul>
   <li> They do not link to the rest of the library. </li>
   <li> They are just random attempts. Obviously a systematic approach is
   needed, collecting all methods currently in use in the *whole* library.
   </li>
   <li> DONE (see "Improve minimisation scripts" in
   Satisfiability/Optimisation/plans/general.hpp)
   Regarding the computation of minimum CNF representations, the scripts
   here are misplaced. </li>
  </ul>


  \todo Connections
  <ul>
   <li> See
   ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/plans/general.hpp
   for the Maxima-level. </li>
   <li> See
   ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/plans/general.hpp
   for an AES translation into SAT. </li>
   <li> See Applications/Cryptanalysis/plans/Rijndael.hpp for the C++ level.
   </li>
   <li> See Experimentation/Investigations/plans/CurrentExperiments.hpp for
   current experiments (todo "Add section on AES experiments to be run" in
   Experimentation/Investigations/plans/general.hpp needs urgent update). </li>
   <li> See "SAT 2011". </li>
  </ul>


  \todo SAT 2011
  <ul>
   <li> We intend to submit a paper investigating the various translations of
   the AES and small scale AES given in [Algebraic Aspects of the Advanced 
   Encryption Standard;Cid,Murphy Robshaw] to the SAT 2011 conference (see
   http://www.lri.fr/SAT2011/). </li>
   <li> For investigations and open problems in this area see
   Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/general.hpp.
   </li>
   <li> This should also link in with 
   Experimentation/Investigations/plans/CurrentExperiments.hpp. </li>
  </ul>


  \bug MiniSAT2 based solvers return incorrect times using experiment script
  <ul>
   <li> minisat2 based solvers (glucose and minisat2) return run times of
   a fraction of a second using when run using "run_all_solvers_2by2" 
   while they clearly take much much longer when run separately. </li>
   <li> See "Using the canonical translation" in
   Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/KeyDiscovery/032/4_2_4/1_13.hpp.
   </li>
   <li> Just use "time"! </li>
  </ul>


  \todo Determining a "good" local search algorithm to use on AES instances
  <ul>
   <li> Question: what is a good algorithm to use, considering the algorithms
   tested by "run_ubcsat" (see 
   Experimentation/ExperimentSystem/ControllingLocalSearch/Evaluation.R) </li>
   <li> Presumably local search will not do well on AES instances compared to 
   DPLL/CDCL solvers, as often local search solvers perform
   poorly on industrial and crafted instances, which the AES translations
   are likely similar to, considering the large number of new variables 
   involved. </li>
   <li> What sort of metrics to use to determine a good algorithm once the
   experiment has been run?
   <ul>
    <li> Sorting first by the average number of falsified clauses and then
    by the number of steps seems reasonable, as we wish to minimise the number
    of falsified clauses as much as possible. </li>
   </ul>
   </li>
  </ul>


  \todo DONE (moved to "Improve minimisation scripts" in
  Satisfiability/Optimisation/plans/general.hpp)
  Update scripts
  <ul>
   <li> The following scripts:
    <ul>
     <li> Satisfiability/Optimisation/minimise_cnf_oksolver. </li>
     <li> Satisfiability/Optimisation/cardinality_bounded_cnf. </li>
     <li> Satisfiability/Optimisation/extend_strict_dimacs_with_extended. </li>
     <li> Satisfiability/Optimisation/all_minimum_cnf. </li>
    </ul>
    need the following changes:
    <ul>
     <li> Tests (URGENT). </li>
     <li> Error checking code. </li>
     <li> Merging with other scripts in other investigations into general tools.
     </li>
     <li> Update documentation in comments. </li>
     <li> Add version numbers. DONE </li>
    </ul>
   </li>
  </ul>


  \todo DONE Replace "merge_cnf.sh" with "AppendDimacs"
  <ul>
   <li> All instances of the "merge_cnf.sh" script at
   /home/csmg/Work/OKlibrary/OKplatform/OKsystem/OKlib/Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/merge_cnf.sh
   should be replaced with "AppendDimacs-O3-DNDEBUG" as
   "AppendDimacs-O3-DNDEBUG" performs the same operation,
   is correct and has tests. </li>
  </ul>

*/
