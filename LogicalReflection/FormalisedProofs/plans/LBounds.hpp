// Oliver Kullmann, 9.12.2007 (Swansea)
/* Copyright 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file LogicalReflection/FormalisedProofs/plans/LBounds.hpp
  \brief Plans for the verification of L-bounds (number of literal occurrences) for SAT-decision


  \todo Mathematical precise proofs
  <ul>
   <li> The 2^{l/9} bound from the [Kullmann, Luckhardt]-preprints. </li>
   <li> And the tau(9,10)-bound as well. </li>
   <li> If possible with accompanying algorithms and implementations. </li>
   <li> Only the "combinatorial core" should be treated. </li>
   <li> Investigate how these "combinatorial cores" could be handled
   semi-automatically.
    <ol>
     <li> One needs a way to supply "patterns", which are systematically
     investigated. </li>
    </ol>
   </li>
   <li> If the combinatorial cores could be formulated in first-order, then
   one could apply ATP. </li>
   <li> Or at least a library of proof-components is established, which
   eases the proof-burden. </li>
   <li> At least one should implement the algorithm, run un on example,
   and search for worst-case examples.
    <ol>
     <li> Some kind of "unit-test" scheme could be applied to test
     the assertion (together with its sub-assertions). </li>
    </ol>
   </li>
  </ul>

*/

