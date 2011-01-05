// Matthew Gwynne, 30.12.2010 (Swansea)
/* Copyright 2010, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/Representations/BoxPrimeImplicates.hpp
  \brief On investigating the prime implicates of the AES and small scale boxes


  \todo Overview
  <ul>
   <li> Some of the AES and small scale boxes have small sets of prime 
   implicates, and some have sets of prime implicates with a regular
   structure, and therefore it is important to investigate these clause-sets
   to determine whether in some cases one can simply use the prime implicates
   as the best possible box translation. </li>
   <li> To generate the prime implicates for a given box, there are two 
   options, depending on the predicted size of the set.
   <ol>
    <li> If there are likely to be very few prime implicates, then we can 
    generate them in the maxima system like so:
    \verbatim
> ss_box_pi : min_2resolution_closure_cs( 
                 fcs2cs(ss_sbox_fullcnf_fcs(2,4,ss_polynomial_2_4)) )$
    \endverbatim
    where "ss_sbox_fullcnf_fcs(2,4,ss_polynomial_2_4)" generates the *full* 
    CNF (note min_2resolution_closure_cs should only be given full CNFs) for
    the small scale 4-bit Sbox (see 
    ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/SboxAnalysis.mac)
    and then from this min_2resolution_closure_cs computes the prime 
    implicates. The same can be done for the field operations using similar
    CNF generation functions (see
    ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/FieldOperationsAnalysis.mac).
    </li>
    <li> Another method is to use the C++ QuineMcCluskey implementation, by
    generating the full CNF as a DIMACS file:
    \verbatim
maxima> output_rijnsbox_fullcnf_stdname();
    \endverbatim
    which generates the file "AES_Sbox_full.cnf" and then using 
    "QuineMcCluskey-n16-O3-DNDEBUG" to generate the prime implicates:
    \verbatim
shell> QuineMcCluskey-n16-O3-DNDEBUG AES_Sbox_full.cnf > sbox_pi.cnf
    \endverbatim
    Again, see 
    ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/SboxAnalysis.mac
    and
    ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/FieldOperationsAnalysis.mac 
    for information on how to generate these Dimacs files.
    </li>
   </ol>
   </li>
  </ul>


  \todo Links
  <ul>
   <li> See 
   ComputerAlgebra/Satisfiability/Lisp/Resolution/PrimeImplicatesImplicants.mac
   and "min_2resolution_closure_cs" for computing the prime implicates at the 
   Maxima level. </li>
   <li> See Satisfiability/FiniteFunctions/QuineMcCluskey.cpp for information
   on generating prime implicates at the C++ level. </li>
  </ul>

  
  \todo Move individual investigations to sub-modules
  <ul>
   <li> We should have a sub-module "PrimeImplicates" which then has files
   for the Sbox and each Field multiplication etc, where the statistics and
   regularities of each set of prime implicates can be discussed. </li>
  </ul>

  
  \todo Sbox prime implicates
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


  \todo Number of prime implicates for field multiplications
  <ul>
   <li> Currently, within the SAT translation, a powerful representation
   of the field operations used within the AES SAT translation (such as 
   multiplication by 02, 03 etc within Rijndael's byte field, see 
   ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/plans/FieldOperationsAnalysis.hpp)
   is the canonical translation using new variables (see "dualts_fcl" in 
   ComputerAlgebra/Satisfiability/Lisp/FiniteFunctions/TseitinTranslation.mac).
   </li>
   <li> However, the most powerful representation of any boolean function, not 
   considering size of the translation is always the set of prime implicates. 
   </li>
   <li> In general, the set of prime implicates for a given boolean function is
   prohibitively large, such as is the case with the Sbox with > 100000
   clauses. </li>
   <li> This is not always the case though, as the size and structure of the 
   set of prime implicates for each of the field multiplication functions must
   be checked. </li>
   <li> Small set of prime implicates would provide possibly better
   representations than "dualts_fcl" produces, since we save the 256 new
   variables introduced with every single box-representation. </li>
   <li> And if the set of prime implicates is small enough, it might
   even be smaller than the "dualts_fcl" translation. </li>
   <li> The full CNF representation of the boolean function for multiplication
   by a constant factor n (where n is the integer representation for
   a polynomial in base 2) in Rijndael's byte-field, where n is an integer 
   (see 
   ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/FieldOperationsAnalysis.mac), 
   can be generated by:
   \verbatim
output_rijnmult_fullcnf_stdname(n);
   \endverbatim
   and the prime implicates can then be generated using:
   \verbatim
n=2; QuineMcCluskey-n16-O3-DNDEBUG AES_byte_field_mul_full_${n}.cnf > AES_byte_field_mul_full_${n}.pi
   \endverbatim
   and getting a histogram of clause lengths
   \verbatim
> # Calculate the number of each length clause
> n=2; for m in `seq 1 16`; do echo -n $m ": " && C=`echo -n '^' && perl -e "print \"[^ ]+ +\" x $m" && echo '0$'` && cat AES_byte_field_mul_full_${n}.pi | grep -v "^p" | grep -E "$C" | wc -l; done
   \endverbatim
   </li>
   <li> Multiplication by 1: </li>
   <li> Multiplication by 2:
   <ul>
    <li> There are 58 prime implicates for multiplication by 2. </li>
    <li> The minimal size of a prime clause is 2. </li>
    <li> The maximal size of a prime clause is 4. </li>
    <li> Multiplication by 2 is a very simple operation, which is essentially
    a shift operation, hence the small number of prime implicates. </li>
    <li> In this case, representing multiplication by 02 by all it's prime
    implicates seems the best solution, given such a small number. </li>
   </ul>
   </li>
   <li> Multiplication by 3:
   <ul>
    <li> There are 5048 prime implicates for multiplication by 3. </li>
    <li> The minimal size of a prime clause is 3. </li>
    <li> The maximal size of a prime clause is 9. </li>
   </ul>
   </li>
   <li> Multiplication by 9:
   <ul>
    <li> There are 7840 prime implicates for multiplication by 9. </li>
    <li> The minimal size of a prime clause is 3. </li>
    <li> The maximal size of a prime clause is 9. </li>
   </ul>
   </li>
   <li> Multiplication by 11:
   <ul>
    <li> There are 15008 prime implicates for multiplication by 11. </li>
    <li> The minimal size of a prime clause is 4. </li>
    <li> The maximal size of a prime clause is 9. </li>
   </ul>
   </li>
   <li> Multiplication by 13:
   <ul>
    <li> There are 15312 prime implicates for multiplication by 13. </li>
    <li> The minimal size of a prime clause is 4. </li>
    <li> The maximal size of a prime clause is 9. </li>
   </ul>
   </li>
   <li> Multiplication by 14:
   <ul>
    <li> There are 14300 prime implicates for multiplication by 14. </li>
    <li> The minimal size of a prime clause is 3. </li>
    <li> The maximal size of a prime clause is 9. </li>
   </ul>
   </li>
   <li> We need to handle all multiplications; and we need the size (and
   number) of minimum CNF representations. </li>
   <li> It would be nice here to be able to read the clause-sets into the
   Maxima system, where we have statistics_fcl etc. (see "Input and output" in 
   ComputerAlgebra/Satisfiability/Lisp/ClauseSets/plans/general.hpp). </li>
  </ul>  \todo Number of prime implicates for field multiplications
  <ul>
   <li> Currently, within the SAT translation, a powerful representation
   of the field operations used within the AES SAT translation (such as 
   multiplication by 02, 03 etc within Rijndael's byte field, see 
   ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/plans/FieldOperationsAnalysis.hpp)
   is the canonical translation using new variables (see "dualts_fcl" in 
   ComputerAlgebra/Satisfiability/Lisp/FiniteFunctions/TseitinTranslation.mac).
   </li>
   <li> However, the most powerful representation of any boolean function, not 
   considering size of the translation is always the set of prime implicates. 
   </li>
   <li> In general, the set of prime implicates for a given boolean function is
   prohibitively large, such as is the case with the Sbox with > 100000
   clauses. </li>
   <li> This is not always the case though, as the size and structure of the 
   set of prime implicates for each of the field multiplication functions must
   be checked. </li>
   <li> Small set of prime implicates would provide possibly better
   representations than "dualts_fcl" produces, since we save the 256 new
   variables introduced with every single box-representation. </li>
   <li> And if the set of prime implicates is small enough, it might
   even be smaller than the "dualts_fcl" translation. </li>
   <li> The full CNF representation of the boolean function for multiplication
   by a constant factor n (where n is the integer representation for
   a polynomial in base 2) in Rijndael's byte-field, where n is an integer 
   (see 
   ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/FieldOperationsAnalysis.mac), 
   can be generated by:
   \verbatim
output_rijnmult_fullcnf_stdname(n);
   \endverbatim
   and the prime implicates can then be generated using:
   \verbatim
n=2; QuineMcCluskey-n16-O3-DNDEBUG AES_byte_field_mul_full_${n}.cnf > AES_byte_field_mul_full_${n}.pi
   \endverbatim
   and getting a histogram of clause lengths
   \verbatim
> # Calculate the number of each length clause
> n=2; for m in `seq 1 16`; do echo -n $m ": " && C=`echo -n '^' && perl -e "print \"[^ ]+ +\" x $m" && echo '0$'` && cat AES_byte_field_mul_full_${n}.pi | grep -v "^p" | grep -E "$C" | wc -l; done
   \endverbatim
   </li>
   <li> Multiplication by 1: </li>
   <li> Multiplication by 2:
   <ul>
    <li> There are 58 prime implicates for multiplication by 2. </li>
    <li> The minimal size of a prime clause is 2. </li>
    <li> The maximal size of a prime clause is 4. </li>
    <li> Multiplication by 2 is a very simple operation, which is essentially
    a shift operation, hence the small number of prime implicates. </li>
    <li> In this case, representing multiplication by 02 by all it's prime
    implicates seems the best solution, given such a small number. </li>
   </ul>
   </li>
   <li> Multiplication by 3:
   <ul>
    <li> There are 5048 prime implicates for multiplication by 3. </li>
    <li> The minimal size of a prime clause is 3. </li>
    <li> The maximal size of a prime clause is 9. </li>
   </ul>
   </li>
   <li> Multiplication by 9:
   <ul>
    <li> There are 7840 prime implicates for multiplication by 9. </li>
    <li> The minimal size of a prime clause is 3. </li>
    <li> The maximal size of a prime clause is 9. </li>
   </ul>
   </li>
   <li> Multiplication by 11:
   <ul>
    <li> There are 15008 prime implicates for multiplication by 11. </li>
    <li> The minimal size of a prime clause is 4. </li>
    <li> The maximal size of a prime clause is 9. </li>
   </ul>
   </li>
   <li> Multiplication by 13:
   <ul>
    <li> There are 15312 prime implicates for multiplication by 13. </li>
    <li> The minimal size of a prime clause is 4. </li>
    <li> The maximal size of a prime clause is 9. </li>
   </ul>
   </li>
   <li> Multiplication by 14:
   <ul>
    <li> There are 14300 prime implicates for multiplication by 14. </li>
    <li> The minimal size of a prime clause is 3. </li>
    <li> The maximal size of a prime clause is 9. </li>
   </ul>
   </li>
   <li> We need to handle all multiplications; and we need the size (and
   number) of minimum CNF representations. </li>
   <li> It would be nice here to be able to read the clause-sets into the
   Maxima system, where we have statistics_fcl etc. (see "Input and output" in 
   ComputerAlgebra/Satisfiability/Lisp/ClauseSets/plans/general.hpp). </li>
  </ul>


  \todo Understanding prime implicates after any partial assignment
  <ul>
   <li> To consider the AES Sbox as an "active clause", we want to first be 
   able, given a partial assignment, to infer as many forced assignments
   as possible. This can be done simply with the DNF representation. </li>
   <li> However, secondly one needs, given a partial assignment, to be able to
   determine various measures for heuristics. </li>
   <li> Therefore, investigating several statistics (most notably the number of
   clauses for a given variable) of the prime implicates of the clause-set, 
   formed after taking the Sbox and applying each partial assignment, is 
   necessary to try and discern a pattern. </li>
   <li> If such patterns can be deduced for particular clause-set measures,
   then the active clause can use this pattern, given a partial assignment, 
   to return reasonable values for these measures which can be used for 
   statistics. </li>
   <li> A C++ implementation of such a system whereby the set of prime 
   implicates is taken as input, and each partial assignment along with
   the relevant statistics is returned is necessary. </li>
   <li> Such a C++ implementation would need to be able to apply a partial
   assignment to a clause-set and then compute various statistics on the 
   result. This would need to be done for every partial assignment. </li>
   <li> After applying the partial assignment, to gain the prime implicates
   of the new boolean function, one must simply apply subsumption elimination
   to the new clause-set (which is just result of applying a partial assignment
   to the prime implicates of the original function). This can be done using 
   functionality already in the library (MG: Where?). </li>
  </ul>

*/
