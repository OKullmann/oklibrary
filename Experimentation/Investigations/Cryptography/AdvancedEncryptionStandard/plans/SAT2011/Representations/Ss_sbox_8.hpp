// Matthew Gwynne, 6.1.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/Representations/Ss_sbox_8.hpp
  \brief Experiments using the R QCA package to minimise the AES 8-bit Sbox.


  \todo Overview
  <ul>
   <li> Here we should have an overview of the current state of this 
   investigation and open problems. </li>
  </ul>


  \todo Minimum using exact espresso algorithms
  <ul>
   <li> See    
   Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/Representations/Minimisation/Espresso/Ss_sbox_8.hpp .
   </li>
   <li> Computing the minimum CNF:
   \verbatim
maxima> SboxCNF: fcs2fcl(ss_sbox_fullcnf_fcs(2,4,ss_polynomial_2_4))$
maxima> output_tt2pla(fcl2tt( SboxCNF ),"Sbox.pla")$
   \endverbatim
   and then:
   \verbatim
shell> espresso2.3 -Dexact Sbox.pla
   \endverbatim
   causes espresso to run out of memory on an 8GB machine.
   </li>
   <li> The same occurs using the espresso "signature" algorithm. 
   </li>
   <li> Machines with more memory should be found. </li>
  </ul>

  
  \todo Using R QCA package
  <ul>
   <li> See    
   Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/Representations/Minimisation/RQCA/general.hpp .
   </li>
   <li> Computing the minimum CNF:
   \verbatim
maxima> SboxCNF: fcs2fcl(ss_sbox_fullcnf_fcs(2,4,ss_polynomial_2_4))$
maxima> output_tt2table(fcl2tt(SboxCNF),"Sbox.dat")$
   \endverbatim
   and then in R:
   \verbatim
R> oklib_load_all()
R> library(QCA)
R> sbox_tt = read.table("Sbox.dat",header=TRUE)
R> eqmcc(sbox_tt, outcome="O", expl.0=TRUE)
   \endverbatim
   yields:
   \verbatim
Error: Impossible to solve the PI chart (too many possible combinations).
   \endverbatim
   </li>
   <li> This suggests that it at may at least be computing the prime 
   implicates. </li>
   <li> We should look into the options for this package, or perhaps
   e-mail the developers. </li>
  </ul>

*/
