// Matthew Gwynne, 30.12.2010 (Swansea)
/* Copyright 2010, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/Representations/BoxMinimisation.hpp
  \brief On investigating the generation of minimum size CNFs for the AES boxes


  \todo Overview
  <ul>
   <li> In current investigations we consider several different
   representations of the various boxes (Sbox and field multiplications)
   in the AES and small scale AES translations, namely:
   <ul>
    <li> Minimum size CNF representations. </li>
    <li> Small size CNF representations (when minimum size is infeasible). 
    </li>
    <li> Tseitin translations of the DNF representations (offering
    r1-based properties and reasonable size translations). </li>
    <li> Prime implicate representations. </li>
   </ul>
   </li>
   <li> Investigations into minimal size Sbox representations can be found in
    Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/Representations/SboxMinimisation.hpp.
    </li>
   <li> Investigations into minimal size field multiplication representations can be found in
   Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/Representations/FieldMulMinimisation.hpp.
   </li>
   <li> Futher information on generating CNFs and DNFs for the Sbox and field
   multiplications can be found at
   ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/SboxAnalysis.mac and
   ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/FieldOperationsAnalysis.mac .
   </li>
  </ul>

*/
