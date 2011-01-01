// Matthew Gwynne, 30.12.2010 (Swansea)
/* Copyright 2010, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/BoxRepresentations.hpp
  \brief On investigating the representations of the AES and small scale boxes.


  \todo Investigate CNF representations
  <ul>
   <li> One needs to systematically explore CNF representations, with and
   without new variables. </li>
   <li> First to study the set of prime implicates (see
   Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/BoxPrimeImplicates.hpp). </li>
   <li> For minimum and "small" representations, see
   Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/BoxMinimisation.hpp. </li>
   <li> For R-based representations see "R-based representations". </li>
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
  </ul>


  \todo Overview
  <ul>
   <li> Some of the AES and small scale boxes have small sets of prime 
   implicates, and some have sets of prime implicates with a regular
   structure, and therefore it is important to investigate these clause-sets
   to determine whether in some cases one can simply use the prime implicates
   as the best possible box translation. </li>
  </ul>


  \todo R-based representations
  <ul>
   <li> r-bases for r in {r_1,r_2}? (see rand_rbase_cs(F,r) in
   ComputerAlgebra/Satisfiability/Lisp/Reductions/RBases.mac.) </li>
  </ul>

*/
