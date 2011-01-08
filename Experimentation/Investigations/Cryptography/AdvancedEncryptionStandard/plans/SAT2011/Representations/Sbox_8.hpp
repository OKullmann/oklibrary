// Matthew Gwynne, 6.1.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/Representations/Sbox_8.hpp
  \brief Experiments using the R QCA package to minimise the AES 8-bit Sbox.


  \todo Overview
  <ul>
   <li> We do *not* currently know the minimum CNF size for the 8-bit Sbox.
   </li>
   <li> The smallest CNF we have so far is of size 294, described in
   "Using weighted MaxSAT to compute small CNFs". </li>
   <li> Here we should have an overview of the current state of this 
   investigation and open problems. </li>
  </ul>


  \todo Move individual investigations to sub-modules
  <ul>
   <li> We should have a sub-module "PrimeImplicates" which then has files
   for the Sbox and each Field multiplication etc, where the statistics and
   regularities of each set of prime implicates can be discussed. </li>
  </ul>

  
  \todo Prime implicates
  <ul>
    <li> We can generate the prime implicates for the Sbox by first
    generating the full CNF for the Sbox (in Maxima)
    \verbatim
>  output_rijnsbox_fullcnf_stdname();
    \endverbatim
    and then generating the prime implicates using the QuineMcCluskey 
    procedure
    \verbatim
> QuineMcCluskey-n16-O3-DNDEBUG AES_Sbox_full.cnf > AES_Sbox_pi.cnf
    \endverbatim
    </li>
    <li> Basic statistics
    <ul>
    <li> We have the following basic statistics for the prime implicates
    for the AES Sbox
    \verbatim
> OKsolver_2002-O3-DNDEBUG -P AES_Sbox_pi.cnf
s UNKNOWN
c sat_status                            2
c initial_maximal_clause_length         9
c initial_number_of_variables           16
c initial_number_of_clauses             136253
c initial_number_of_literal_occurrences 999896
    \endverbatim
    and also
    \verbatim
> # Calculate the number of each length clause
> for n in `seq 1 16`; do echo -n $n ": " && C=`echo -n '^' && perl -e "print \"[^ ]+ +\" x $n" && echo '0$'` && cat AES_Sbox_pi.cnf | grep -v "^p" | grep -E "$C" | wc -l; done
1 : 0
2 : 0
3 : 0
4 : 0
5 : 1
6 : 4148
7 : 82659
8 : 48615
9 : 830
10 : 0
11 : 0
12 : 0
13 : 0
14 : 0
15 : 0
16 : 0
    \endverbatim
    </li>
    </ul>
    </li>
    <li> There are no necessary clauses in the AES Sbox prime implicates,
    as can be seen by generating the subsumption hypergraph
    \verbatim
> QuineMcCluskeySubsumptionHypergraph-n16-O3-DNDEBUG AES_Sbox_full.cnf > AES_Sbox_shg.cnf
   \endverbatim
   and observing that there are no unit hyperedges.
   </li>
  </ul>


  \todo Using weighted MaxSAT to compute small CNFs
  <ul>
   <li> Computing the weighted MaxSAT problem:
   \verbatim
maxima> output_rijnsbox_fullcnf_stdname();
   \endverbatim
   and then 
   \verbatim
shell> QuineMcCluskeySubsumptionHypergraph-n16-O3-DNDEBUG AES_Sbox_full.cnf AES_emSbox_pi.cnf > AES_Sbox_shg.cnf
shell> cat AES_Sbox_shg.cnf | awk --file ${OKPLATFORM}/OKsystem/OKlib/Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/shg2partial_maxsat.awk > AES_Sbox_shg.wcnf
   \endverbatim
   <li>
   <li> Running then:
   \verbatim
shell> new-ubcsat-okl  -alg gsat -w -runs 100 -cutoff 40000000 -wtarget 294 -solve 1 -seed 3213901809 -i AES_Sbox_shg.wcnf -r model AES_Sbox_s294.ass; 
shell> cat  AES_Sbox_pi.cnf | FilterDimacs AES_Sbox_s294.ass > AES_Sbox_s294.cnf
   \endverbatim
   yields a CNF of size 294 with the following statistics:
   \verbatim
shell> cat AES_Sbox_s294.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG 
c's = 0, n = 16, c = 294, tc = 0, ntc = 294, tl = 1939, l = 1939, finished = 1
6 : 143
7 : 127
8 : 24
   \endverbatim
   and we can check this is indeed a CNF representation of the Sbox using 
   Maxima:
   \verbatim
> is(rijnsbox_fulldnf_fcs()[2] = all_sat_fcs(Sbox294CNFF));
true
   \endverbatim
   where Sbox294CNFF is the maxima clause-set given by the Dimacs
   output.
   </li>
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


  \todo Small CNFs with espresso
  <ul>
   <li> Computing the minimum CNF:
   \verbatim
maxima> SboxCNF: fcs2fcl(ss_sbox_fullcnf_fcs(2,4,ss_polynomial_2_4))$
maxima> output_tt2pla(fcl2tt( SboxCNF ),"Sbox.pla")$
   \endverbatim
   and then:
   \verbatim
shell> espresso2.3 Sbox.pla
   \endverbatim
   we get a clause-set of size 354.
   </li>
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
