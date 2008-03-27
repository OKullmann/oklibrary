// Oliver Kullmann, 28.1.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/Resolution/plans/general.hpp
  \brief Plans for Maxima-components regarding the resolution calculus


  \todo Create milestones


  \todo Write tests


  \todo Write docus


  \todo See ProofSystems/Resolution/plans/Search.hpp.


  \todo Resolution proofs
  <ul>
   <li> The natural format for a resolution proof is a non-empty list,
   where each entry is either a clause (an "axiom") or a pair consisting
   of a clause (the "resolvent") and a pair of (smaller) indices (the
   indices of the "parent clauses"). </li>
   <li> We need a correctness-checker. </li>
   <li> We should also investigate the existing file-formats for resolution
   proofs, and we should provide input- and output-facilities. </li>
   <li> This linear format is in 1-1 correspondence to the representation
   via labelled dag's; we need a representation of labelled graphs,
   digraphs and labelled digraphs. </li>
   <li> The above can easily be generalised to non-boolean clause-sets.
   </li>
  </ul>


  \todo Bounded resolution
  <ul>
   <li> Implement the different forms of bounded resolution. </li>
   <li> In this way we can determine the width of a clause-set. </li>
  </ul>


  \todo Read-once resolution proofs
  <ul>
   <li> Write a checker whether a resolution proof is read-once. </li>
   <li> Implement the translation of "has read-once refutation" into SAT. </li>
  </ul>

*/

