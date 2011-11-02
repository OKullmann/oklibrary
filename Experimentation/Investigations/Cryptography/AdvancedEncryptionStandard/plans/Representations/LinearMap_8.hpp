// Matthew Gwynne, 13.1.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/Representations/LinearMap_8.hpp
  \brief Investigations into the AES Sbox linear map (8-bit)


  \todo Basic data
  <ul>
   <li> The CNF file 
   "AES_sbox_lin_2_8_1_1_1_1_1_0_0_0_0_1_1_1_1_1_0_0_0_0_1_1_1_1_1_0_0_0_0_1_1_1_1_1_1_0_0_0_1_1_1_1_1_1_0_0_0_1_1_1_1_1_1_0_0_0_1_1_1_1_1_1_0_0_0_1_full.cnf" 
   is created by the Maxima-function 
   output_ss_sbox_linmap_fullcnf_stdname(2,8), in
   ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/SboxAnalysis.mac 
   which is a full clause-set with 16 variables and 2^16 - 2^8 = 65280 
   clauses:
   \verbatim
> cat AES_sbox_lin_2_8_1_1_1_1_1_0_0_0_0_1_1_1_1_1_0_0_0_0_1_1_1_1_1_0_0_0_0_1_1_1_1_1_1_0_0_0_1_1_1_1_1_1_0_0_0_1_1_1_1_1_1_0_0_0_1_1_1_1_1_1_0_0_0_1_full.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG n
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
16 65280 1044480 0 1044480 1 1
 length count
16 65280
   \endverbatim
   </li>
   <li> Prime implicates:
    <ol>
     <li> There are 12672 prime implicates, with 97792 literals in total, 
     and with clause-length-distribution as follows:
     \verbatim
> cat AES_sbox_lin_2_8_1_1_1_1_1_0_0_0_0_1_1_1_1_1_0_0_0_0_1_1_1_1_1_0_0_0_0_1_1_1_1_1_1_0_0_0_1_1_1_1_1_1_0_0_0_1_1_1_1_1_1_0_0_0_1_1_1_1_1_1_0_0_0_1_full.cnf_primes_stats 
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
16 12672 97792 0 97792 0 1
 length count
4 128
5 0
6 1536
7 0
8 11008
     \endverbatim
     </li>
     <li> Why are there no prime implicates for 5 and 7? </li>
    </ol>
   </li>
   <li> The subsumption-hypergraph of the prime-clauses:
    <ol>
     <li> Full statistics:
     \verbatim
> cat AES_sbox_lin_2_8_1_1_1_1_1_0_0_0_0_1_1_1_1_1_0_0_0_0_1_1_1_1_1_0_0_0_0_1_1_1_1_1_1_0_0_0_1_1_1_1_1_1_0_0_0_1_1_1_1_1_1_0_0_0_1_1_1_1_1_1_0_0_0_1_full.cnf_shg_stats 
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
12672 65280 4915200 0 4915200 0 1
 length count
59 2048
71 2048
72 6400
73 2048
74 7168
75 12288
76 11264
77 10240
78 5120
79 0
80 0
81 4096
82 2048
83 0
84 512
     \endverbatim
     </li>
     <li> Why the big jump from 59 to 71 (with all zeroes inbetween)?
     </li>
     <li> One should try to fit this curve. </li>
    </ol>
   </li>
   <li> So far, we have the following smallest representation:
   \verbatim
R> subset(E, min==48)[1:5,]
  sat min osteps  msteps       seed
1   0  48 344293 1000000 1999482689
2   0  48 175413 1000000 3807176898
3   0  48 159985 1000000 4020856009
4   0  48 164319 1000000 2022796238
5   0  48 417957 1000000  324341182
   \endverbatim
   </li>
   <li> See "Basic data" in 
   Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/Representations/Sbox_8.hpp.
   </li>
   <li> See "First considerations of random permutation" in
   Experimentation/Investigations/BooleanFunctions/plans/Permutations.hpp for
   random permutations. </li>
  </ul>


  \todo Using weighted MaxSAT to compute small CNFs : mincl_rinf <= 48
  <ul>
   <li> Computing the weighted MaxSAT problem:
   \verbatim
maxima> output_ss_sbox_linmap_fullcnf_stdname(2,8);
   \endverbatim
   and then 
   \verbatim
shell> QuineMcCluskeySubsumptionHypergraphWithFullStatistics-n16-O3-DNDEBUG AES_sbox_lin_2_8_1_1_1_1_1_0_0_0_0_1_1_1_1_1_0_0_0_0_1_1_1_1_1_0_0_0_0_1_1_1_1_1_1_0_0_0_1_1_1_1_1_1_0_0_0_1_1_1_1_1_1_0_0_0_1_1_1_1_1_1_0_0_0_1_full.cnf > shg.cnf
shell> cat shg.cnf | MinOnes2WeightedMaxSAT-O3-DNDEBUG > shg.wcnf
   \endverbatim
   <li>
   <li> Running then:
   \verbatim
shell> ubcsat-okl -alg gsat -w -runs 100 -cutoff 1000000 -i shg.wcnf > ubcsat_agsat_r100_c1000000.runs
   \endverbatim
   yields:
   \verbatim
R> E = read.table("ubcsat_agsat_r100_c1000000.runs",header=TRUE)
R> summary(E)
     sat         min            osteps           msteps     
 Min.   :0   Min.   :48.00   Min.   :104441   Min.   :1e+06  
 1st Qu.:0   1st Qu.:48.00   1st Qu.:148122   1st Qu.:1e+06  
 Median :0   Median :48.00   Median :176470   Median :1e+06  
 Mean   :0   Mean   :48.44   Mean   :201289   Mean   :1e+06  
 3rd Qu.:0   3rd Qu.:48.00   3rd Qu.:220279   3rd Qu.:1e+06  
 Max.   :0   Max.   :52.00   Max.   :483843   Max.   :1e+06  
      seed          
 Min.   :2.721e+07  
 1st Qu.:1.430e+09  
 Median :2.226e+09  
 Mean   :2.355e+09  
 3rd Qu.:3.435e+09  
 Max.   :4.282e+09 
   \endverbatim
   </li>
   <li> Is 48 the minimum? </li>
  </ul>

*/
