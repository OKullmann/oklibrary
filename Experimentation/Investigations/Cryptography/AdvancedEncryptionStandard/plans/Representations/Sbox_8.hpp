// Matthew Gwynne, 6.1.2011 (Swansea)
/* Copyright 2011, 2012 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/Cryptography/AdvancedEncryptionStandard/plans/Representations/Sbox_8.hpp
  \brief Investigations into the AES S-box


  \todo Basic data
  <ul>
   <li> Algebraic data:
    <ol>
     <li> Cycle type and order:
     \verbatim
cycletype_pmtf(rijn_sbox_pmtf,256);
  [[2,1],[27,1],[59,1],[81,1],[87,1]]

order_element_pmtf(rijn_sbox_pmtf,256);
  277182
     \endverbatim
     </li>
     <li> How does this compare to a random permutation? </li>
     <li> The cycle-type (i.e., the isomorphism-type, or the conjugacy-type)
     of a permutation is independent of a permutation of the base-set, but
     the boolean functions are not isomorphic (easy example?). Especially,
     what is the easiest permutation from the view-point of prime-implicates
     for a permutation of the above cycle-type? </li>
     <li> It seems however that this can't be exploited for a better
     translation, and thus such considerations should be moved to general
     investigations on boolean functions. </li>
    </ol>
   </li>
   <li> Generating the full CNF representation:
    <ol>
     <li> The CNF-file "AES_Sbox_full.cnf" is created by the Maxima-function
     output_rijnsbox_fullcnf_stdname() in
     ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/SboxAnalysis.mac,
     which is a full clause-set with 16
     variables and 2^16 - 2^8 = 65280 clauses:
     \verbatim
> cat AES_Sbox_full.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG n
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
16 65280 1044480 0 1044480 1 1
 length count
16 65280
     \endverbatim
     </li>
     <li> The underlying clause-set is rijnsbox_fullcnf_fcs(). </li>
     <li> This clause-set is also computed by
     bf2relation_fullcnf_fcs(rijn_sbox_bf,8). </li>
    </ol>
   </li>
   <li> Prime implicates:
    <ol>
     <li> There are 136253 prime implicates, with 999896 literals in total, and
     with clause-length-distribution as follows:
     \verbatim
> QuineMcCluskey-n16-O3-DNDEBUG AES_Sbox_full.cnf > AES_PK.cnf
> cat AES_PK.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
16 136253 999896 0 999896 1 1
 length count
5 1
6 4148
7 82659
8 48615
9 830
     \endverbatim
     </li>
     <li> What is the clause of length 5? What is its meaning?!
     <ul>
      <li> The clause is {2,-5,6,11,-13}. That is three input variables and
      two output. </li>
     </ul>
     </li>
    </ol>
   </li>
   <li> The subsumption-hypergraph of the prime-clauses:
    <ol>
     <li> Creation:
     \verbatim
> QuineMcCluskeySubsumptionHypergraph-n16-O3-DNDEBUG AES_Sbox_full.cnf > AES_S.cnf
> cat AES_S.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG n > AES_S_stat

> head -2 AES_S_stat
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
136253 65280 59122688 0 59122688 1 1
     \endverbatim
     </li>
     <li> All original prime-clauses occur as 136253 variables, and all
     original clauses occur as 65280 clauses (no contractions). </li>
     <li> Curve k -> nr (clause-length to number of occurrences) looks like a
     nice relatively symmetric curve, with maximum around 950, and konvex on
     both sides. </li>
     <li> R-summary:
     \verbatim
> E=read.table("AES_S_stat",skip=2,header=TRUE)
     length           count
 Min.   : 170.0   Min.   :  1.00
 1st Qu.: 555.2   1st Qu.: 11.25
 Median : 846.5   Median : 38.00
 Mean   : 845.8   Mean   : 55.99
 3rd Qu.:1137.8   3rd Qu.:102.00
 Max.   :1517.0   Max.   :173.00
> plot(E)
     \endverbatim
     </li>
     <li> One should try to fit this curve. </li>
     <li> Has the single shortest clause (of length 170) a special meaning?
     What is its original clause? </li>
     </li>
    </ol>
   </li>
   <li> See "Basic data" in
   Cryptography/AdvancedEncryptionStandard/plans/Representations/Inv_8.hpp.
   </li>
   <li> See "First considerations of random permutation" in
   Experimentation/Investigations/BooleanFunctions/plans/Permutations.hpp for
   random permutations. </li>
  </ul>


  \todo Overview
  <ul>
   <li> We do *not* currently know the minimum CNF size for the 8-bit Sbox.
   </li>
   <li> The smallest CNF we have so far is of size 294, described in
   "Using weighted MaxSAT to compute small CNFs". </li>
   <li> Here we should have an overview of the current state of this
   investigation and open problems. </li>
   <li> There are open investigations on finding the smallest
   hitting-clause-set representation, and small representations derived
   from this. See "Generate good CNF hitting clause-sets" and
   "Extracting prime implicate representations from the
   hitting-cls-representations". </li>
   <li> There is also the open problem of investigating the symmetries
   of the DNF, see "Find the symmetries of the AES Sbox DNF". </li>
  </ul>


  \todo Move individual investigations to sub-modules
  <ul>
   <li> We should have a sub-module "PrimeImplicates" which then has files
   for the Sbox and each Field multiplication etc, where the statistics and
   regularities of each set of prime implicates can be discussed. </li>
  </ul>


  \todo Computing a minimum CNF represention : mincl_rinf <= 294
  <ul>
   <li> The current minimum clause-length of an r_infinity-base is 294. </li>
   <li> Currently there are the following approaches:
    <ol>
     <li> see "Using weighted MaxSAT to compute small CNFs" below; apparently
     most powerful </li>
     <li> see "Minimum using exact espresso algorithms"; this doesn't work
     (yet) </li>
     <li> see "Small CNFs with espresso" </li>
     <li> see "Using R QCA package"; this doesn't work (yet). </li>
    </ol>
   </li>
   <li> The hardness of the "minimum" representation for the S-box is 4:
    <ul>
     <li> Consider the small CNF representation of the S-box,
     generated by:
     \verbatim
maxima> oklib_load_all()$
maxima> output_rijnsbox_fullcnf_stdname()$
shell> QuineMcCluskeySubsumptionHypergraph-n16-O3-DNDEBUG AES_Sbox_full.cnf > AES_Sbox_shg.cnf
shell> cat AES_Sbox_shg.cnf | MinOnes2WeightedMaxSAT-O3-DNDEBUG > AES_Sbox_shg.wcnf
shell> ubcsat-okl  -alg gsat -w -runs 100 -cutoff 40000000 -wtarget 294 -solve 1 -seed 3213901809 -i AES_Sbox_shg.wcnf -r model AES_Sbox_s294.ass;
shell> cat  AES_Sbox_full.cnf_primes | FilterDimacs AES_Sbox_s294.ass > AES_Sbox_s294.cnf
     \endverbatim
     we consider the hardness of this representation of the AES S-box. </li>
     <li> See "Hardness of boolean function representations" in
     Experimentation/Investigations/BooleanFunctions/plans/general.hpp
     for a description of the notion of hardness, and method of computation.
     </li>
     <li> Computing the hardness:
     \verbatim
shell> F_Sbox_pi : read_fcl_f("AES_Sbox_full.cnf_primes")$
shell> F_Sbox : read_fcl_f("AES_Sbox_s294.cnf")$
shell> hardness_wpi_cs(setify(F_Sbox[2]), setify(F_Sbox_pi[2]));
4
     \endverbatim
     </li>
     <li> Note we only consider some small representation for the AES S-box
     here, not a minimum representation. We, as yet, do not know the
     minimum size for the S-box. </li>
     <li> Also, we certainly do not yet know if all (or even any) of
     the minimum representations of the 8-bit AES S-box have hardness 4. </li>
    </ul>
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
shell> QuineMcCluskeySubsumptionHypergraph-n16-O3-DNDEBUG AES_Sbox_full.cnf > AES_Sbox_shg.cnf
shell> cat AES_Sbox_shg.cnf | MinOnes2WeightedMaxSAT-O3-DNDEBUG > AES_Sbox_shg.wcnf
   \endverbatim
   </li>
   <li> Running then:
   \verbatim
shell> ubcsat-okl  -alg gsat -w -runs 100 -cutoff 40000000 -wtarget 294 -solve 1 -seed 3213901809 -i AES_Sbox_shg.wcnf -r model AES_Sbox_s294.ass;
shell> cat  AES_Sbox_full.cnf_primes | FilterDimacs AES_Sbox_s294.ass > AES_Sbox_s294.cnf
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
   Cryptography/AdvancedEncryptionStandard/plans/Representations/Methods.hpp.
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
   <li> Computing some small CNF:
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
   Cryptography/AdvancedEncryptionStandard/plans/Representations/Methods.hpp.
   </li>
   <li> Computing a minimum CNF:
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


  \todo r_1-bases : mincl_r1 <= 4398
  <ul>
   <li> Current minimum clause-count of an r_1-base: 4398. </li>
   <li> Starting with a generating set, created from scratch:
   \verbatim
> RUcpGen-O3-DNDEBUG AES_PK.cnf > AES_gen.cnf
> cat AES_gen.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
16 9050 63306 0 63306 1 1
 length count
5 1
6 1373
7 6363
8 1295
9 18
> seed=1; cat AES_gen.cnf | RandomShuffleDimacs-O3-DNDEBUG ${seed} | RUcpBase-O3-DNDEBUG | tee AES_base_${seed}.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
16 4754 32530 0 32530 1 1
 length count
5 1
6 1140
7 3223
8 386
9 4
> seed=2; cat AES_gen.cnf | RandomShuffleDimacs-O3-DNDEBUG ${seed} | RUcpBase-O3-DNDEBUG | tee AES_base_${seed}.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
16 4765 32606 0 32606 1 1
 length count
5 1
6 1135
7 3245
8 380
9 4
> seed=3; cat AES_gen.cnf | RandomShuffleDimacs-O3-DNDEBUG ${seed} | RUcpBase-O3-DNDEBUG | tee AES_base_${seed}.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
16 4739 32404 0 32404 1 1
 length count
5 1
6 1152
7 3205
8 377
9 4
   \endverbatim
   </li>
   <li> Likely first sorting AES_PK.cnf by length (shortest first) should
   yield a shorter (better) generating set (while perhaps once the generating
   set is fixed, there aren't so much choices anymore(?)). </li>
   <li> Another thing to do is to provide RUcpGen-O3-DNDEBUG with a non-empty
   starting set, namely the clauses from a "small" representation. </li>
   <li> The canonical translation has the following statistics:
   \verbatim
maxima> FF_sbox_ts : ss_sbox_ts_gen(2,8,ss_polynomial_2_8)$
maxima> statistics_fcs(F_sbox_ts);
[272,4353,12800,256,2]
maxima> ncl_list_full_dualts(16,256);
[[2,4096],[17,256],[256,1]]
   \endverbatim
   </li>
   <li> Generation from scratch, using a sorted list of prime-clauses:
    <ol>
     <li> Sorting the list of prime-clauses yields a shorter generating
     clause-set (which at least is considerably faster):
     \verbatim
> RUcpGen-O3-DNDEBUG AES_PK_sorted.cnf  | tee sbox_gen_from_sorted.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
16 5883 39850 0 39850 1 1
 length count
5 1
6 1798
7 3619
8 461
9 4
     \endverbatim
     </li>
     <li> A base from that is smaller than what we got above:
     \verbatim
> cat sbox_gen_from_sorted.cnf | RUcpBase-O3-DNDEBUG | tee sbox_base_from_sorted.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
16 4596 31523 0 31523 0 1
 length count
5 1
6 1109
7 3028
8 454
9 4
     \endverbatim
     </li>
     <li> Sorting the generating set in descending order (using
     SortByClauseLength-O3-DNDEBUG, "tac" and then manual editing),
     yields an even smaller r_1 base:
     \verbatim
> cat sbox_gen_from_revsorted.cnf | RUcpBase-O3-DNDEBUG | tee sbox_base_from_revsorted.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
16 4474 30569 0 30569 0 1
 length count
5 1
6 1207
7 2810
8 452
9 4
     \endverbatim
     See 'Need "ReverseDimacs" application and clause-set adaptor' in
     Interfaces/InputOutput/plans/ClauseSetAdaptors.hpp for discussion
     on creating a clause-set "reverse" application" (we could also
     offer another option in SortByClauseLength, although general
     tools such as "Reverse" are also nice).
     </li>
    </ol>
   </li>
   <li> Iterating through the random seeds, while sorting inputs in
   ascending order of clause-length into RUcpGen, and descending into
   RUcpBase yields a new smaller r_1 base:
   \verbatim
shell> RandomRUcpBases AES_Sbox_full.cnf
<snip>
*** Currently trying gs=59,bs=2
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
16 4401 30169 0 30169 0 1
 length count
5 1
6 1155
7 2730
8 511
9 4
------------------------------------------------------------------------------------
CURRENT MINIMUM RBASE: *4401* with gs=59,bs=2
------------------------------------------------------------------------------------
<snip>
*** Currently trying gs=103,bs=1
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
16 4398 30108 0 30108 0 1
 length count
5 1
6 1187
7 2703
8 503
9 4
------------------------------------------------------------------------------------
CURRENT MINIMUM RBASE: *4398* with gs=103,bs=1
------------------------------------------------------------------------------------
<snip>
   \endverbatim
   Experiment run up to gs=104, bs=1 so far. </li>
   <li> It makes sense that sorting the generating set in descending
   order of clause-size, when passing it to RUcpBase, would yield shorter
   r_1 bases as then RUcpBase removes longer clauses first and keeps
   the shorter ones (which cover more). </li>
   <li> Using the smallest known Sbox CNF (mincl_rinf = 294):
    <ol>
     <li> See
     "Using weighted MaxSAT to compute small CNFs") with statistics:
     \verbatim
> cat AES_294.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
16 294 1939 0 1939 0 1
 length count
6 143
7 127
8 24
> cat AES_294.cnf | SortByClauseLength-O3-DNDEBUG > AES_294_sorted.cnf
     \endverbatim
     as a starting point we get the following bases of size 4735, 4744 and
     4658:
     \verbatim
> RUcpGen-O3-DNDEBUG AES_PK_sorted.cnf AES_294_sorted.cnf | tee sbox_gen.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
16 5967 40615 0 40615 1 1
 length count
5 1
6 1638
7 3847
8 476
9 5
> seed=1; cat sbox_gen.cnf | RUcpBase-O3-DNDEBUG | tee AES_base_nosort_${seed}.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
16 4735 32492 0 32492 0 1
 length count
5 1
6 1106
7 3178
8 445
9 5
> seed=1; cat sbox_gen.cnf | SortByClauseLength-O3-DNDEBUG | RUcpBase-O3-DNDEBUG | tee AES_base_sorted_${seed}.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
16 4744 32562 0 32562 0 1
 length count
5 1
6 1099
7 3194
8 445
9 5
> seed=1; cat sbox_294_gen_revsort.cnf | RUcpBase-O3-DNDEBUG | tee AES_base_revsort_${seed}.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
16 4658 31869 0 31869 0 1
 length count
5 1
6 1190
7 3017
8 445
9 5
     \endverbatim
     </li>
     <li> Note that in this case (using an initial input of the 294 clauses
     from the smallest known representation), sorting the clauses of the
     generating set in reverse (sbox_294_gen_revsort.cnf - generated by hand
     from the sorted generating set using "tac" and moving the p line back to
     the top) yields the smallest r_1 base of the three (no sorting, sorting
     and reverse). </li>
    </ol>
   </li>
   <li> The smallest known representation (mincl_rinf=294) doesn't seem to use
   the prime implicate of size 5 (see "Basic Data")? </li>
   <li> Perhaps we can find a small representation using the clause of size 5
   and then try and use that to generate a smaller r_1-base? </li>
   <li> DONE (we have now the approach via first computing a generating set)
   The direct computation (via "cat AES_PK.cnf | RandomShuffleDimacs-O3-DNDEBUG | RUcpBase-O3-DNDEBUG")
   of a base takes too long. </li>
  </ul>


  \todo Generate good CNF hitting clause-sets
  <ul>
   <li> Different heuristics for generating hitting clause-sets
   \verbatim
length(rijnsbox2hittingcnf_fcs(dll_heuristics_first_formal));
2048
length(rijnsbox2hittingcnf_fcs(dll_heuristics_first_real));
2048
length(rijnsbox2hittingcnf_fcs(dll_heuristics_first_shortest_clause));
2048
statistics_cs(rijnsbox2hittingcnf_fcs(dll_heuristics_max_lit));
[16, 1513, 19546, 16, 6]
length(rijnsbox2hittingcnf_fcs(dll_heuristics_max_var));
2048
statistics_cs(rijnsbox2hittingcnf_fcs(dll_heuristics_max_lit_tb(3,3)));
[16, 1468, 18925, 16, 6]
statistics_cs(rijnsbox2hittingcnf_fcs(dll_heuristics_max_lit_tb(4,4)));
[16, 1438, 18536, 16, 6]
   \endverbatim
   Would be interesting to understand this. See below. </li>
   <li> Comparing to the ID3 algorithm, as described in
   http://www.cs.princeton.edu/courses/archive/spr07/cos424/papers/mitchell-dectrees.pdf :
   \verbatim
id3_entropy_fcs_full(FF) := block([p_0, p_1],
  p_0 : length(FF[2])/(2^length(FF[1])),
  p_1 : 1 - p_0,
  if p_0 = 0 or p_1 = 0 then return(0)
  else return(- (p_0 * log(p_0) + p_1 * log(p_1)) / log(2)))$

id3_gain_fcs_full(FF,v) := block([FF_v0, FF_v1, total_space],
  FF_v0 : apply_pa_fcs({-v}, FF),
  FF_v1 : apply_pa_fcs({v}, FF),
  total_space : 2^length(FF[1]),
  id3_entropy_full(FF) -
  ((length(FF_v0[2])/total_space) * id3_entropy_full(FF_v0) +
  (length(FF_v1[2])/total_space) * id3_entropy_full(FF_v1)))$

id3_heuristic_fcs_full(FF) := block([max_v, max_gain : minf],
  for v in FF[1] do block([cur_gain],
  cur_gain : id3_gain_full(FF,v),
  if cur_gain > max_gain then (max_v : v, max_gain : cur_gain)),
  return(max_v))$

F : rijnsbox2hittingcnf_fcs(id3_heuristic_full)$

statistics_cs(F);
 [16,2048,25600,16,9]
   \endverbatim
   We should move this into a separate investigation about the ID3 algorithm.
   </li>
   <li> DONE (explained below: there are no forced assignments, so no
   false-condensation can take place, while true-condensations can't happen
   with clause-sets) Also interesting that all the hitting trees
   produced by the SAT solvers are already condensed. </li>
   <li> Use heuristics_lookahead_distances with different lookahead-reductions
   and different distances. (MG: Looking at several combinations of the already
   defined reductions, all seem to lead to 2048. I am still trying different
   combinations) </li>
   <li> Since the AES-DNF is unique, as a correctness test we can
   just check whether we get the input back (using any heuristics):
   \verbatim
rijnsbox_cnfp(cs_to_fcs(rijnsbox2hittingcnf_fcs(dll_heuristics_max_lit)));
   \endverbatim
   </li>
   <li> We should also use reductions. For that we need the ability
   to translate r_k-splitting trees into hitting clause-sets, which can
   be done in a straightforward way, by just making the forced assignments
   into trees of levelled height 1 (ignoring the actual reduction).
   DONE (as explained below, here there are no forced assignments.
   Nevertheless, in general this approach should be implemented.) </li>
   <li> A general conjecture on computing dual hitting clause-sets from a
   clause-set F.
    <ol>
     <li> "For computing small dual hitting cause-sets, use as splitting
     literal one with leads to maximal probability of satisfiability." </li>
     <li> So using choose_most_sat_literal_h in
     ComputerAlgebra/Satisfiability/Lisp/Backtracking/DLL_solvers.mac
     should do a good job. </li>
     <li> Experimental results:
     \verbatim
statistics_cs(rijnsbox2hittingcnf_fcs(johnson_heuristic));
[16, 1516, 19573, 16, 6]
statistics_cs(rijnsbox2hittingcnf_fcs(choose_most_sat_literal_h(satprob_dll_simplest_trivial1)));
[16, 1516, 19573, 16, 6]
     \endverbatim
     <li> DONE (differences are due to ties)
     The strange thing here are these little differences "1513, 1515, 1516",
     where actually all three trees should be identical! We need to
     investigate this. (Perhaps this comes from tie-braking. But still,
     shouldn't be there.) </li>
     <li> Also the reductions and the look-ahead need to be considered. </li>
     <li> "The strongest approach is to use full elimination of forced
     assignments at each node. The look-ahead then also takes all forced
     assignments into account." </li>
     <li> If the reduction misses forced assignments, then in principle this
     is not a problem for the tree, since condensing the tree will remove
     unnecessary branches at the end. However it affects the look-ahead. </li>
     <li> If F is a hitting clause-set, then computing the sat-probability
     is easy (and all other tasks), and the above heuristics is the Johnson
     heuristics. The heuristics finds also all forced assignments in the
     "combined" look-ahead, since forced assignments are identified as branches
     where the other branch has sat-probability 0; however it is cleaner to
     put this completely into the reduction. </li>
     <li> If more specially F is a full clause-set (and the class of full
     clause-sets is stable under partial assignments(!)), then the Johnson-
     heuristic just becomes the maxlit-occurrences heuristic above, choosing a
     literal occurring most often. </li>
     <li> Moreover, in our case dll_heuristics_max_lit actually implements
     the full elimination of forced assignments and the full look-ahead(!):
      <ol>
       <li> The Sbox-function, as any other permutation, allows only satisfying
       assignments of full length --- and thus (for the CNF-perspective) there
       are no forced assignments at all! </li>
       <li> So reduction and look-ahead are so "strong" because there is simply
       nothing to be reduced (via forced assignments). </li>
      </ol>
     </li>
    </ol>
   </li>
   <li> It is likely that the SAT approximation heuristic would benefit from
   some lookahead:
    <ol>
     <li> As was presented with regards to max_lit, so as to differentiate
     when ties occur for the maximal probability of satisfiability. </li>
     <li> We wish the same idea of maximising satisfiability to occur when we
     make the assignment and move 1 level down in the tree, and so the literal
     which results in the most "bias" when that literal assignment is made
     (or potentially taking into account both branches) should be chosen. </li>
     <li> The reasoning behind maximising "bias" at each level needs to be made
     clear here (falsifying assignments occurring along the same "path" down
     the tree result in less "branch off"s for the satisfying assignments,
     where the "branch off"s are then the clauses we take for the dual hitting
     clause set (after condensing?) - this is likely very imprecise and
     unclear). </li>
    </ol>
   </li>
  </ul>


  \todo Analysing the AES prime implicates
  <ul>
   <li> The AES S-box has 136253 prime implicates:
   \verbatim
> QuineMcCluskey-n16-O3-DNDEBUG AES_Sbox_full.cnf > AES_PK.cnf
> cat AES_PK.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
16 136253 999896 0 999896 1 1
 length count
5 1
6 4148
7 82659
8 48615
9 830
   \endverbatim
   </li>
   <li> Where do these prime implicates come from? </li>
   <li> Can we deduce these prime implicates from first principles?
   </li>
   <li> For a permutation of GF(2^8) we have 2 * 2^8 * 8 = 2^12 = 4096
   potential prime implicates, given by fixing 8 bit in either the input or
   the output, and one further bit to the wrong value. As one can see by
   the identity, these are not necessarily prime, since the identity
   just has 16 prime implicates (of length 2). </li>
   <li> How do the S-box prime implicates relate to these "potential"
   prime implicates? </li>
   <li> See investigations into prime implicates for general boolean functions
   and permutations in
   Experimentation/Investigations/BooleanFunctions/plans/general.hpp. </li>
  </ul>


  \todo Extracting prime implicate representations from the
  hitting-cls-representations
  <ul>
   <li> As explained in "Hitting clause-sets" in
   ComputerAlgebra/Satisfiability/Lisp/Resolution/plans/PrimeImplicatesImplicants.hpp,
   given a hitting clause-set representation, from it we can obtain a shorter
   representation by prime implicates. This is a better representation. </li>
   <li> This is achieved by "replace_by_prime_implicates_hitting". </li>
   <li>
   \verbatim
h_aes : rijnsbox2hittingcnf_fcs(dll_heuristics_max_lit)$
p_aes : replace_by_prime_implicates_hitting(h_aes)$
statistics_cs(p_aes);
[16, 1359, 9430, 9, 6]
ncl_list_cs(p_aes);
[[6, 265], [7, 913], [8, 180], [9, 1]]
irredundant_bydef(cs_to_fcs(p_aes), dll_simplest_trivial1);
false
ip_aes : first_irr_fcs(cs_to_fcs(p_aes), dll_simplest_trivial2)$
statistics_cs(ip_aes[2]);
[16, 581, 4006, 8, 6]
rijnsbox_cnfp(ip_aes);
true
h2_aes : rijnsbox2hittingcnf_fcs(dll_heuristics_max_lit_tb(4,4))$
rijnsbox_cnfp(cs_to_fcs(h2_aes));
true
p2_aes : replace_by_prime_implicates_hitting(h2_aes)$
statistics_cs(p2_aes);
[16, 1303, 9011, 9, 6]
ip2_aes : first_irr_fcs(cs_to_fcs(p2_aes), dll_simplest_trivial2)$
statistics_cs(ip2_aes[2]);
[16, 559, 3838, 8, 6]
rijnsbox_cnfp(ip2_aes);
true
h0_aes : rijnsbox2hittingcnf_fcs(dll_heuristics_first_formal)$
p0_aes : replace_by_prime_implicates_hitting(h0_aes)$
statistics_cs(p0_aes);
[16, 1722, 12118, 9, 5]
ip0_aes : first_irr_fcs(cs_to_fcs(p0_aes), dll_simplest_trivial2)$
statistics_cs(ip0_aes[2]);
[16, 680, 4784, 9, 5]
rijnsbox_cnfp(ip0_aes);
true
   \endverbatim
   </li>
   <li> This looks very interesting! Should be close to the optimum.
   And looks much smaller than to be expected. </li>
   <li> Analysing p_aes:
   \verbatim
ir_p_aes : all_irrcl_bydef(cs_to_fcs(p_aes), dll_simplest_trivial2)$
statistics_cs(ir_p_aes);
[16, 447, 3087, 8, 6]
ncl_list_cs(ir_p_aes);
[[6, 99], [7, 291], [8, 57]]
oklib_monitor : true;
oklib_monitor_level : 1;
irrc_p_aes : all_irr_cores_bydef(cs_to_fcs(p_aes), dll_simplest_trivial2)$
   \endverbatim
   </li>
   <li> We should try random sampling of the irredundant cores. </li>
   <li> Alternatively we compute all maximal non-equivalent sub-clause-sets
  via all_max_noneq_scs_bydef:
    <ol>
     <li> See
     ComputerAlgebra/Satisfiability/Lisp/MinimalUnsatisfiability/Cores.mac
     </li>
     <li> Function rijnsbox_cnfp delivers the required equivalence test.
     </li>
     <li> Then via hypergraph transversals we obtain all irredundant cores.
     </li>
     <li> Likely there are (too) many sub-clause-sets, but we can try. </li>
    </ol>
   </li>
   <li> The question is how small can we get a CNF representation? We need
   also to investigate the size of a CNF obtained from the DNF via Tseitin
   translation (and optimisation); see
   ComputerAlgebra/Satisfiability/Lisp/FiniteFunctions/plans/TseitinTranslation.hpp
   </li>
  </ul>


  \todo Find the symmetries of the AES Sbox DNF
  <ul>
   <li> See
   ComputerAlgebra/Satisfiability/Lisp/Symmetries/plans/general.hpp. </li>
   <li> We don't have full symmetry between the input and output bits, as the
   linear map is not self-inverse. This is further evidenced by the single
   prime implicate of length 5 (see "Basic data"). </li>
   <li> We can run through all 8! * 2^8 = 1,032,1920 permutations with flips
   on the input variables, interpreted as a bijection from input-space to
   output-space, to check whether they constitute an automorphism. </li>
   <li> This would take into account all automorphisms "flipping sides". </li>
   <li> Then there are automorphisms within the input variables resp. within
   the output variables. </li>
   <li> There might be further cases. </li>
   <li> Since the DNF is rather small, Saucy (see "Symmetries" in
   Buildsystem/ExternalSources/SpecialBuilds/plans/SAT.hpp) should be able
   to do the job. </li>
  </ul>

*/
