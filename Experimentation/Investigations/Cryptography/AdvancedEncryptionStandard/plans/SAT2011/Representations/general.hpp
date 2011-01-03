// Matthew Gwynne, 2.1.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/Representations/general.hpp
  \brief Investigations into representations for components of the AES related to the SAT 2011 paper


  \todo Add links and update module structure
  <ul>
   <li> Todos and files in this module have been moved from the level
   above, links must be added here and the module updated in general
   to make each of the todos accessible. </li>
   <li> The general structure of the investigations (from arbitrary boolean
   functions to permutations and linear maps) needs to be expressed. </li>
   <li> The tasks are (at least):
   <ul>
    <li> Add links in this file to each of the individual files, and links
    from the level above to this level. </li>
    <li> Add links to external modules for each file for relevant information.
    </li>
    <li> For each of the following files, add information on how to generate
    each of the experiments, as well as a todo on creating submodules for
    each type of experiment (including separately methods of experimentation):
    <ul>
     <li> BoxMinimisation. </li>
     <li> BoxPrimeImplicates : DONE </li>
    </ul>
    </li>
   </ul>
   </li>
   <li> DONE
   Move todos from BoxRepresentations into this file (as this module is
   precisely about box representations). </li>
  </ul>


  \todo Overview
  <ul>
   <li> One needs to systematically explore CNF representations, with and
   without new variables. </li>
   <li> First to study the set of prime implicates (see
   Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/Representations/BoxPrimeImplicates.hpp). </li>
   <li> For minimum and "small" representations, see
   Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/Representations/BoxMinimisation.hpp. </li>
   <li> For R-based representations see "R-based representations". </li>
  </ul>


  \todo R-based representations
  <ul>
   <li> r-bases for r in {r_1,r_2}? (see rand_rbase_cs(F,r) in
   ComputerAlgebra/Satisfiability/Lisp/Reductions/RBases.mac.) </li>
   <li> One could consider certain prime implicates more important than others;
   for example ensuring that at least given a full input and/or a full output
   to one permutation the output resp. input can be inferred.
    <ol>
     <li> Can one formulate (relatively efficiently) the minimisation target
     that these inferences are "easily" available while otherwise using the
     smallest representation? </li>
     <li> We could generalise the notion of r-base w.r.t. specific clauses
     which have to be deducible via r, while all (other) removed clauses just
     need to follow logically, or perhaps using some stronger reduction. </li>
    </ol>
   </li>
   <li> When investigations begin fully in this area, this todo should be
   moved to a new file, and most likely a new sub-module. </li>
  </ul>

*/
