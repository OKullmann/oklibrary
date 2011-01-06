// Matthew Gwynne, 6.1.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/Representations/Minimisation/IterativeSAT/general.hpp
  \brief Experimental results relating to use of iterated SAT techniques for minimising AES boxes.


  \todo Overview
  <ul>
   <li> Optimising the size of CNF representations for a given clause-set
   using standard SAT solvers can be approached by generating the 
   subsumption hypergraph for the original (full) CNF and it's prime 
   implicates and then adding additional cardinality clauses to the 
   subsumption hypergraph restricting the number of variables set to true
   and hence the number of clauses in our representation. </li>
   <li> By continuing to reduce the cardinality of the result until
   the subsumption hypergraph clause-set is unsatisfiable (i.e., there
   is no CNF representation with a number of clauses less than the 
   cardinality), we can iteratively search for the solution. </li>
   <li> So, for example, we generate the subsumption hypergraph
   for the 4-bit small scale Sbox like so:
   \verbatim
maxima> output_ss_sbox_fullcnf_stdname(2);
   \endverbatim
   and then 
   \verbatim
shell> ${OKPLATFORM}/OKsystem/OKlib/Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/minimise_cnf_oksolver ss_byte2_4_field_mul_full_2.cnf
c Starting level  14
c Starting level  13
c Starting level  12
c Starting level  11
c Starting level  10
c Starting level  9
c Starting level  8
c Found (a) minimum size representation for " ss_byte2_4_field_mul_full_2.cnf " of size 9
c Subsumption hypergraph for the minimisation problem for ss_byte2_4_field_mul_full_2.cnf
p cnf 8 9
-5 2 0
-2 5 0
-6 3 0
-3 6 0
-7 1 4 0
-4 1 7 0
-8 -7 -4 0
-8 4 7 0
-1 8 0
   \endverbatim
   </li>
   <li> Note the above uses the OKsolver to check for satisfiability of each
   clause-set. Further scripts should be written to check using other solvers.
   See "Solvers to be used for experimentation" in 
   Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/Experimentation.hpp . </li>
  </ul>


  \todo Add todos.

*/
