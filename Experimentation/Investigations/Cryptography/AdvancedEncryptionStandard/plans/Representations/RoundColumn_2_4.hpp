// Matthew Gwynne, 8.3.2011 (Swansea)
/* Copyright 2011, 2012 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/Cryptography/AdvancedEncryptionStandard/plans/Representations/RoundColumn_2_4.hpp
  \brief Studying good representations of the combination of the AES SubBytes and MixColumns operations


  \todo Problem specification
  <ul>
   <li> We consider the operation "round column" which takes two 4-bit words
   of the AES input, applies the Sbox, and then applies the MixColumns matrix
   multiplication.  We look at the properties of this operation on these two
   4-bit words as a single 8-bit permutation. </li>
   <li> The original AES round as part of a (2*4*m)-bit AES cipher (for all
   positive integers m) can be considered as:
   <ol>
    <li> Shifting of row i by i to the left. </li>
    <li> Applying the "round column" operation to each column. </li>
   </ol>
   where the AES (2*4*m)-bit input is considered as a 2xm matrix of 4-bit
   elements.
   </li>
   <li> This combination of the SubBytes and the matrix multiplication
   can be achived because the linear diffusion operation in the AES shifts
   the results of each Sbox in the SubBytes operation as a whole, and doesn't
   mix the output bits of one Sbox with those of another. Therefore we can
   move the Sbox operation through the "ShiftRows". </li>
   <li> The round column operation is an 8-bit permutation, and intuitively
   should mimic a random permutation and so see also
   Cryptography/AdvancedEncryptionStandard/plans/Representations/Sbox_8.hpp
   and
   Cryptography/AdvancedEncryptionStandard/plans/Representations/Inv_8.hpp
   for comparison. </li>
  </ul>


  \todo Naming
  <ul>
   <li> The name "round column" captures that:
   <ul>
    <li> The combination of these operations yields the entire round, and they
    consider disjoint sets of inputs, so this is in some sense an operation
    representative of the entire round on a single column. </li>
   </ul>
   </li>
   <li> However, it has the disadvantages that:
   <ul>
    <li> It might confuse the reader into thinking it acts on each column of
    the original input, not that input after shifting each row. </li>
    <li> It only considers the "core round", not including the key addition,
    and the name suggests otherwise.
    </li>
   </ul>
   </li>
   <li> A better name that makes clear this relationship with the input
   should be devised. </li>
  </ul>


  \todo Basic data
  <ul>
   <li> Generating the CNF in Maxima:
   \verbatim
output_fcs("16-bit AES Round",bf2relation_fullcnf_fcs(lambda([V],ss_round_column_bf(V,2,4)), 8),"round_column_16_full.cnf")$
   \endverbatim
   yields file "round_column_16_full.cnf" in the current directory.
   </li>
   <li> Generating the prime implicate and subsumption hypergraph statistics:
   \verbatim
shell> QuineMcCluskeySubsumptionHypergraphFullStatistics-n16-O3-DNDEBUG round_column_16_full.cnf
   \endverbatim
   </li>
   <li> Prime implicates statistics:
   \verbatim
shell> cat round_column_16_full.cnf_primes_stats
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
16 122228 919954 0 919954 0 1
 length count
4 4
5 144
6 3269
7 52328
8 65039
9 1444
   \endverbatim
   </li>
   <li> Subsumption hypergraph statistics:
   \verbatim
shell> oklib --R
R> E = read.table("round_column_16_full.cnf_shg_stats",header=TRUE,skip=2)
R> summary(E)
     length           count
 Min.   : 255.0   Min.   :  0.00
 1st Qu.: 483.5   1st Qu.:  8.00
 Median : 712.0   Median : 38.00
 Mean   : 712.0   Mean   : 71.34
 3rd Qu.: 940.5   3rd Qu.:131.50
 Max.   :1169.0   Max.   :264.00
R> plot(E)
   \endverbatim
   </li>
   <li> The distribution of the clause-lengths to number of occurrences in the
   subsumption hypergraph looks binomial with the mean around 780 and both
   sides convex. There is a very small bump, up and then down, to the binomial
   curve in the graph near clause count 420. Is this significant? </li>
  </ul>


  \todo Minimum representations using weighted MaxSAT : mincl_rinf <= 396
  <ul>
   <li> Computing the weight MaxSAT instance:
   \verbatim
> oklib --maxima
maxima> output_fcs("16-bit AES Round",bf2relation_fullcnf_fcs(lambda([V],ss_round_column_bf(V,2,4)), 8),"round_column_16_full.cnf")$
> QuineMcCluskeySubsumptionHypergraphWithFullStatistics-n16-O3-DNDEBUG round_column_16_full.cnf > round_column_16_shg.cnf
> MinOnes2WeightedMaxSAT-O3-DNDEBUG < round_column_16_shg.cnf > round_column_16_shg.wcnf
   \endverbatim
   </li>
   <li> Running ubcsat-okl:
   \verbatim
> ubcsat-okl -alg gsat -w -i round_column_16_shg.wcnf -wtarget 396 -seed 57642020 -r model round_column_min396.model
# -rclean -r out stdout run,found,best,beststep,steps,seed -r stats stdout numclauses,numvars,numlits,fps,beststep[mean],steps[mean+max],percentsolve,best[min+max+mean+median] -runs 10 -cutoff 100000 -rflush -alg gsat -w -i round_column_16_shg.wcnf -wtarget 396 -seed 57642020 -r model round_column_min396.model
       sat  min     osteps     msteps       seed
      1 0   396                99656                99656   57642020

> ubcsat-okl -alg gsat -w -i round_column_16_shg.wcnf -wtarget 396 -seed 57642020 -r model round_column_min396.model -solve
   \endverbatim
   </li>
   <li> Generating the instance:
   \verbatim
> cat round_column_16_full.cnf_primes | FilterDimacs-O3-DNDEBUG round_column_min396.model > round_column_min396.cnf
> ExtendedDimacsFullStatistics-O3-DNDEBUG < round_column_min396.cnf
     pn      pc      n    nmi       c        l     n0   n0mi      c0       l0  cmts
     16     396     16     16     396     2744     NA     NA     396     2744     1
 length   count
      4       1
      5      12
      6      74
      7     237
      8      71
      9       1
   \endverbatim
   </li>
  </ul>


  \todo r_1-bases : mincl_r1 <= 2712
  <ul>
   <li> We can compute r_1-base statistics by:
   \verbatim
shell> RandomRUcpBases round_column_16_full.cnf
   \endverbatim
   </li>
   <li> We have that a minimum size r_1-base has less than 2712 clauses:
   \verbatim
shell> RandomRUcpBases round_column_16_full.cnf
*snip*
*** Currently trying gs=36,bs=4
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
16 2712 18667 0 18667 0 1
 length count
4 4
5 129
6 604
7 1420
8 553
9 2
------------------------------------------------------------------------------------
CURRENT MINIMUM RBASE: *2712* with gs=36,bs=4
------------------------------------------------------------------------------------
*snip*
   \endverbatim
   Currently experiments have run up to gs=36, bs=4. </li>
   <li> You can generate this r_1-base by:
   \verbatim
shell> gen_seed=10; base_seed=5;
shell> QuineMcCluskey-n16-O3-DNDEBUG round_column_16_full.cnf > round_column_16_primes.cnf
shell> cat round_column_16_primes.cnf | RandomShuffleDimacs-O3-DNDEBUG ${gen_seed} | SortByClauseLength-O3-DNDEBUG > round_column_16_primes.cnf_sorted
shell> RUcpGen-O3-DNDEBUG round_column_16_primes.cnf_sorted > round_column_16_primes.cnf_gen
shell> cat round_column_16_primes.cnf_gen | RandomShuffleDimacs-O3-DNDEBUG ${base_seed} | SortByClauseLength-O3-DNDEBUG | RUcpBase-O3-DNDEBUG > round_column_16_r1base.cnf
   \endverbatim
   <li> This r_1-base is nearly half the size of the smallest known r_1-base
   for the 8-bit Sbox (mincl_r1 <= 4467). </li>
   <li> We need data on the size of r_1-bases for random 8-bit permutations!
   </li>
  </ul>

*/
