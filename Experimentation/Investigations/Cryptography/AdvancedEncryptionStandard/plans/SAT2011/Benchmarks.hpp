// Matthew Gwynne, 10.2.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/Benchmarks.hpp
  \brief Benchmarks for submission to the 2011 SAT competition.


  \todo Links
  <ul>
   <li> See "Benchmarks for SAT 2011" in
   Experimentation/Investigations/plans/SATCompetition.hpp. </li>
   <li> The SAT competition website is at http://www.satcompetition.org/2011/.
   </li>
   <li> DES benchmarks were available in the SAT 2009 competition in the
   industrial track, available at 
   http://www.cril.univ-artois.fr/SAT09/bench/appli.7z under
   SAT09/APPLICATIONS/crypto/desgen/. </li>
  </ul>

 
  \todo Prepare benchmarks for SAT 2011
  <ul>
   <li> Other than the standard AES and small scale translations, the 
   minimisation problems for the AES boxes (Sbox and field multiplications) 
   should yield good benchmarks for weighted MaxSAT solvers. </li>
   <li> Our benchmarks should come under the "Applications"
   category (see "Links"). </li>
   <li> We consider the following benchmarks for the SAT competition:
    <ul> 
     <li> Full AES encryption and decryption for one round up to ten rounds
     using the "minimum" and "canonical" translations.
     </li>
     <li> Full AES key discovery for one round using the "minimum" and 
     "canonical" translations. </li>
     <li> AES key discovery for one round up to three rounds for the
     64-bit 4x4 with 4-bit field elements AES variant using "minimum"
     and "canonical" translations (one round here is currently the
     limit of what we are able to solve). </li>
    </ul>
   </li>
   <li> We consider the following benchmarks for the pseudo boolean
   competition:
    <ul>
     <li> minimising the 8-bit Sbox using a translation of the
     optimisation problem for the minimum hypergraph transversal problem,
     using the subsumption hypergraph. </li>
     <li> minimising the 8-bit multiplications (03,09,11,13,14) using a 
     translation of the optimisation problem for the minimum hypergraph 
     transversal problem, using the subsumption hypergraph. </li>
     <li> minimising the 8-bit field inversion using a translation of the
     optimisation problem for the minimum hypergraph transversal problem,
     using the subsumption hypergraph. </li>
     <li> minimising the 8-bit Sbox linear multiplication using a translation
     of the optimisation problem for the minimum hypergraph transversal 
     problem, using the subsumption hypergraph. </li>
    </ul>
   </li>
   <li> For the MaxSAT competition, we should consider the same problems
   as with the PseudoBoolean, but simply using a translation of the
   minimum transversal problems into MaxSAT problems, rather than
   pseudo boolean. </li>
   <li> We should further specify each of the above benchmarks and move
   them into separate todos with instructions on how to generate each
   instance, their statistics and so on. This information can then be
   moved into their respective accompanying documents (see "Supporting 
   documents" in Experimentation/Investigations/plans/SATCompetition.hpp .
   </li>
   <li> We need proper translations of the minimum hypergraph transversal
   problem into a weighted MaxSAT problem. See "Add MinOnes2WCNF output 
   clause-set adaptor" in Interfaces/InputOutput/plans/ClauseSetAdaptors.hpp.
   </li>
   <li> We also need proper translations of the minimum hypergraph transversal
   problem into a pseudo-boolean problem. </li>
  </ul>

*/
