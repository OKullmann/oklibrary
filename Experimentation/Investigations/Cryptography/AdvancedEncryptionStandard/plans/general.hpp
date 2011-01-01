// Matthew Gwynne, 27.3.2009 (Swansea)
/* Copyright 2009, 2010, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/Cryptography/AdvancedEncryptionStandard/plans/general.hpp
  \brief On investigations into the Advanced Encryption Standard


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
  </ul>


  \todo General organisation
  <ul>
   <li> All the experiments must be fully reproducible. Thus *only* tools
   as provided by the OKlibrary are to be used, *NO* private code or private
   schemes in any form. </li>
   <li> Besides reproducibility, the point is to strengthen the OKlibrary,
   by continuous improvement, not to weaken it by hiding. </li>
   <li> DONE Directory structure:
    <ol>
     <li> DONE
     The main topic here is not investigation of certain solvers, but to
     investigate the interaction of cryptography and (generalised) SAT
     solving. </li>
     <li> DONE Thus solver-names as file-names should only occur, if at all, in
     sub-directories associated with specific investigations. </li>
     <li> The general standards for naming in the OKlibrary have to be
     observed:
      <ul>
       <li> DONE Non-generic filenames start always with a capital letter.
       </li>
       <li> No file- (or directory-)name consists only of capital letters.
       </li>
      </ul>
     </li>
    </ol>
   </li>
  </ul>


  \todo DONE Improve formulations
  <ul>
   <li> The general naming conventions must be followed:
    <ol>
     <li> DONE No camel-cases here (like "removeTopNVars", "AESHexToPA", 
     "NewCL", "mergeSameVarCNFs". </li>
     <li> DONE (renamed relevant functions) 
     Grammatical case indicates verb or noun! </li>
    </ol>
   </li>
   <li> DONE Clean-up minisat output (of course, no intermediate results).
   <ul>
    <li> What is meant by intermediate results here? </li>
    <li> Solver output should be concise, and individual restarts of solvers
    such as minisat should not be included in plans/docus unless there is 
    specifically something interesting to present. </li>
   </ul>
   </li>
   <li> (DONE see for example "Generating experiments" in 
   Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/plans/BreakingAES.hpp)
   Explanations needed! Experiments need full specifications (to be
   reproducible). </li>
   <li> DONE More care! (Of course, the doxygen-output needs to be inspected) 
   </li>
   <li> (DONE see "Experiments") And discussions! What are the underlying 
   problems to be investigated?
   </li>
   <li> DONE No names consisting only of upper-case letters! See
   "Weak state of the whole module" in
   Experimentation/Investigations/Cryptography/plans/general.hpp. </li>
   <li> DONE No "is useful". </li>
   <li> DONE No "may be used". </li>
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

*/
