// Matthew Gwynne, 6.1.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/Representations/Minimisation/general.hpp
  \brief On investigating the generation of minimum size CNFs for the AES boxes


  \todo Overview
  <ul>
   <li> We investigate the generation of minimum size CNF representations
   for the AES boxes, including:
   <ul>
    <li> Sbox (see 
    ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/SboxAnalysis.mac).
    </li>
    <li> Field Multiplications (see
    ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/FieldOperationsAnalysis.mac). 
    </li>
    <li> Small scale variations of the Sbox and field multiplications. </li>
    <li> Affine multiplications (see 
    "Affine bijections over ZZ_2" in 
    Experimentation/Investigations/BooleanFunctions/plans/Permutations.hpp). 
    </li>
   </ul>
   </li>
   <li> In some cases, it may seem infeasible to generate the *minimum* size
   representation and so we generate only "small" representations (i.e.
   as small as we can get). </li>
   <li> When generating minimum or small representations, we have the 
   following tools, discussed in todos of the same name:
   <ul>
    <li> Espresso (see
    Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/Representations/Minimisation/Espresso/general.hpp).
    </li>
    <li> Iterative SAT solving (see
    Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/Representations/Minimisation/IterativeSAT/general.hpp).
    </li>
    <li> Minimising using Hypergraph Transversal tools (see
    Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/Representations/Minimisation/HypergraphTransversal/general.hpp). 
    </li>
    <li> Translating to a weighted MaxSAT problem (see
    Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/Representations/Minimisation/WeightedMaxSAT/general.hpp). 
    </li>
    <li> Minimisation at the maxima level (see
    Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/Representations/Minimisation/Maxima/general.hpp). 
    </li>
    <li> Minimisation using the R QCA package (see
    Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/Representations/Minimisation/RQCA/general.hpp). 
    </li>
   </ul>
  </ul>


  \todo Links
  <ul>
   <li> For information on generation of the CNFs for the AES boxes (Sbox, 
   Field multiplications etc), see:
   <ul>
    <li> Sbox and small scale sbox is at
    ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/SboxAnalysis.mac .
    </li>
    <li> Field multiplications and small scale multiplication at
    ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/FieldOperationsAnalysis.mac .
    </li>
   </ul>
   </li>
   <li> See also 
   ComputerAlgebra/Satisfiability/Lisp/Resolution/PrimeImplicatesImplicants.mac
   and "all_minequiv_bvs_cs" for computing minimum representations in Maxima.
   </li>
  </ul>


  \todo Add instructions for using Pseudo-boolean SAT solvers for minimisation
  <ul>
   <li> Pseudoboolean or PB SAT solvers offer the ability to encode linear
   constraints such as cardinality constraints into a specialised CNF format. 
   </li>
   <li> Such solvers might better handle cardinality constraints
   compared to the translations used in "Iterative SAT solving", and so we#
   should investigate their use. </li>
   </ul>


  \todo Move individual investigations to sub-modules
  <ul>
   <li> DONE
   Sub-modules need to be created for each of the investigations topics
   associated with this file. </li>
   <li> DONE
   A good set of sub-modules seems to be based on the methods involved,
   and so we should have the following sub-modules:
   <ul>
    <li> Espresso : DONE </li>
    <li> IterativeSAT (Iterative SAT solving) : DONE. </li>
    <li> HypergraphTransversal (Minimising using Hypergraph Transversal tools)
    : DONE </li>
    <li> WeightedMaxSAT (Translating to a weighted MaxSAT problem) : DONE. 
    </li>
    <li> Maxima (Minimisation at the maxima level) : DONE. </li>
    <li> RQCA (Minimisation using the R QCA package) : DONE. </li>
   </ul>
   </li>
   <li> Each of these modules would then have a file per box using names
   related to the small scale notation, so such as:
   <ul>
    <li> ss_sbox_4.hpp (4-bit Sbox). </li>
    <li> ss_sbox_8.hpp (8-bit Sbox). </li>
    <li> ss_mul_4_2.hpp (4-bit multiplication by 2). </li>
    <li> ss_mul_8_3.hpp (4-bit multiplication by 3). </li>
    <li> etc. </li>
   </ul>
   </li>
  </ul>

*/
