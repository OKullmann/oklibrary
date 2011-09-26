// Oliver Kullmann, 3.6.2009 (Swansea)
/* Copyright 2009, 2010, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/plans/CurrentExperiments.hpp
  \brief Overview on currently running experiments

  Just to see what experiments are running at *this* time.


  <h1> Cryptography </h1>

  <h2> Advanced Encryption Standard </h2>

  NEEDS URGEND UPDATE --- one needs to see what's going on!!!

  <ul>
   <li> Key Discovery:
   <ul>
    <li> AdvancedEncryptionStandard/plans/SAT2011/KeyDiscovery/016/2_2_4/1_1.hpp.
    </li>
   </ul>
   </li>
   <li> AdvancedEncryptionStandard/plans/SAT2011/Representations/Sbox_8.hpp.
   </li>
   <li> AdvancedEncryptionStandard/plans/SAT2011/Representations/Inv_8.hpp.
   </li>
  </ul>



  <h2> Data Encryption Standard </h2>

  <ul>
   <li> Key Discovery:
    <ul>
     <li> 5-round DES :
      <ul>
       <li> See "Using the Massacci DES translator" in
       Investigations/Cryptography/DataEncryptionStandard/plans/KeyDiscovery/5.hpp.
       </li>
       <li> See "Using the 1-base translation for the S-boxes (6-to-4)" in
       Investigations/Cryptography/DataEncryptionStandard/plans/KeyDiscovery/5.hpp.
       </li>
       <li> See "Using the canonical translation for the S-boxes (6-to-4)" in
       Investigations/Cryptography/DataEncryptionStandard/plans/KeyDiscovery/5.hpp.
       </li>
       <li> See 'Using the minimum translation for the S-boxes (6-to-4)' in
       Investigations/Cryptography/DataEncryptionStandard/plans/KeyDiscovery/5.hpp.
      </ul>
     </li>
    </ul>
     <li> 6-round DES :
      <ul>
       <li> See "Using the 1-base translation for the S-boxes (6-to-4)" in
       Investigations/Cryptography/DataEncryptionStandard/plans/KeyDiscovery/6.hpp.
       </li>
       <li> See "Using the canonical translation for the S-boxes (6-to-4)" in
       Investigations/Cryptography/DataEncryptionStandard/plans/KeyDiscovery/6.hpp.
       </li>
       <li> See 'Using the minimum translation for the S-boxes (6-to-4)' in
       Investigations/Cryptography/DataEncryptionStandard/plans/KeyDiscovery/6.hpp.
      </ul>
     </li>
   </li>
  </ul>



  <h1> Ramsey theory </h1>

  <h2> Green-Tao problems </h2>

  <ul>
   <li> RamseyTheory/GreenTaoProblems/plans/GreenTao_2-3-7.hpp </li>
   <li> RamseyTheory/GreenTaoProblems/plans/GreenTao_3-3-3-k.hpp
    <ol>
     <li> greentao_3(3,3,5) : COMPLETED for now </li>
    </ol>
   </li>
   <li> RamseyTheory/GreenTaoProblems/plans/GreenTao_3-3-4-k.hpp
    <ol>
     <li> greentao_3(3,4,4) : COMPLETED for now </li>
     <li> greentao_3(3,4,5) </li>
    </ol>
   </li>
   <li> RamseyTheory/GreenTaoProblems/plans/GreenTao_4-3.hpp : COMPLETED for
   now </li>
   <li> RamseyTheory/GreenTaoProblems/plans/GreenTao_te_m-3-4.hpp </li>
   <li> RamseyTheory/GreenTaoProblems/plans/GreenTao_te_m-3-5.hpp </li>
   <li> RamseyTheory/GreenTaoProblems/plans/GreenTao_te_m-4-4.hpp </li>
   <li> RamseyTheory/GreenTaoProblems/plans/GreenTao_te_m-3-3-3.hpp </li>
   <li> RamseyTheory/GreenTaoProblems/plans/GreenTao_te_m-3-3-4.hpp :
   COMPLETED for now </li>
   <li> RamseyTheory/GreenTaoProblems/plans/CountingProgressions.hpp
    <ol>
     <li> Computing numerical values for C_k. </li>
    </ol>
   </li>
  </ul>


  <h2> Hindman problems </h2>

  See (currently)
  RamseyTheory/SchurProblems/plans/general.hpp.
  <ul>
   <li> hindman_3(2) </li>
   <li> hindmani_2^6(2) </li>
   <li> hindmani_2^7(2) </li>
   <li> hindmani_2^8(2) </li>
   <li> hindmani_2^9(2) </li>
   <li> hindmani_2^10(2) </li>
   <li> hindmani_2^11(2) </li>
   <li> hindmani_2^12(2) </li>
  </ul>


  <h2> %Ramsey problems </h2>

  <ul>
   <li> RamseyTheory/RamseyProblems/plans/Ramsey_2_5_5.hpp
    <ul> 
     <li> Using DPLL and Conflict driven solvers </li>
    </ul>
   </li>
   <li> RamseyTheory/RamseyProblems/plans/Ramsey_2_4_4.hpp
    <ul> 
     <li> Using DPLL and Conflict driven solvers </li>
    </ul>
   </li>
   <li> RamseyTheory/RamseyProblems/plans/Ramsey_2_4_6.hpp
    <ul> 
     <li> Using DPLL and Conflict driven solvers </li>
    </ul>
   </li>
   <li> RamseyTheory/RamseyProblems/plans/Ramsey_2_3_10.hpp
    <ul> 
     <li> Using DPLL and Conflict driven solvers </li>
    </ul>
   </li>
  </ul>


  <h2> Van der Waerden problems </h2>

  <ul>
   <li> RamseyTheory/VanderWaerdenProblems/plans/Parallelisation/general.hpp
   </li>
   <li> RamseyTheory/VanderWaerdenProblems/plans/3-k/general.hpp </li>
   <li> RamseyTheory/VanderWaerdenProblems/plans/4-k/general.hpp </li>
   <li> RamseyTheory/VanderWaerdenProblems/plans/5-k/general.hpp </li>
   <li> RamseyTheory/VanderWaerdenProblems/plans/6-k/general.hpp </li>
   <li> RamseyTheory/VanderWaerdenProblems/plans/7-k/general.hpp </li>
   <li> (Postponed for now until the full Ramsey-theory-project starts)
   RamseyTheory/VanderWaerdenProblems/plans/VanderWaerden_3-3-3-k.hpp </li>
   <li> (Postponed for now until the full Ramsey-theory-project starts)
   RamseyTheory/VanderWaerdenProblems/plans/VanderWaerden_3-3-4-k.hpp </li>
   <li> (Postponed for now until the full Ramsey-theory-project starts)
   RamseyTheory/VanderWaerdenProblems/plans/VanderWaerden_3-3-3-k.hpp </li>
  </ul>


  <h2> Gasarch problems </h2>

  <ul>
   <li> RamseyTheory/GasarchProblems/plans/general.hpp </li>
  </ul>



  <h1> Unsatisfiable hitting clause-sets </h1>

  See
  Experimentation/Investigations/UHittingCatalogue/plans/general.hpp
  <ul>
   <li> Deficiency = 3, n = 5 </li>
  </ul>



  <h1> Groupoids </h1>

  <ul>
   <li> Investigations/Groupoids/plans/Transformations.hpp
    <ol>
     <li> "Hypergraph transformations I" </li>
     <li> "Hypergraph transformations II" </li>
    </ol>
   </li>
  </ul>


  <h1> Sudoku </h1>

  <ul>
   <li> Investigations/LatinSquares/Sudoku/plans/EmptyPuzzle.hpp </li>
  </ul>
  
*/

