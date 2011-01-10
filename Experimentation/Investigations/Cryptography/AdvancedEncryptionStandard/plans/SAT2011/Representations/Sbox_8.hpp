// Matthew Gwynne, 6.1.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/Representations/Sbox_8.hpp
  \brief Investigations into the AES S-box


  \todo Basic data
  <ul>
   <li> The CNF is created by the Maxima-function
   output_rijnsbox_fullcnf_stdname(), which is a full clause-set with 16
   variables and 2^16 - 2^8 = 65280 clauses. </li>
   <li> Prime implicates:
    <ol>
     <li> There are 136253 prime implicates, with 999896 literals in total, and
     with clause-length-distribution as follows:
     \verbatim
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
     <li> What is the clause of length 5? What is its meaning?! </li>
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
   Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/Representations/Minimisation/RQCA/general.hpp .
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
   <li> We have at least the symmetry exchanging input and output variables.
   (since x^(-1) in the field is self-inverse). </li>
   <li> Then we could simply run through all 8! * 2^8 1,032,1920
   literal substitutions on the input variables, while copying the
    values for the output variables accordingly. </li>
  </ul>

*/
