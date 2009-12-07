// Matthew Gwynne, 26.3.2008 (Swansea)
/* Copyright 2008, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/plans/SboxAnalysis.hpp
  \brief Plans for the Cryptanalysis of the Rijndael Sbox in Maxima/Lisp


  \todo Connections
  <ul>
   <li> See Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/plans/SboxInvestigations.hpp
   for experimental investigations. </li>
  </ul>

  
  \todo Generate good CNF hitting clause-sets for the AES Sbox
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
test_CNF_aes_sbox(cs_to_fcs(rijnsbox2hittingcnf_fcs(dll_heuristics_max_lit)));
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
test_CNF_aes_sbox(ip_aes);
true
h2_aes : rijnsbox2hittingcnf_fcs(dll_heuristics_max_lit_tb(4,4))$
test_CNF_aes_sbox(cs_to_fcs(h2_aes));
true
p2_aes : replace_by_prime_implicates_hitting(h2_aes)$
statistics_cs(p2_aes);
[16, 1303, 9011, 9, 6]
ip2_aes : first_irr_fcs(cs_to_fcs(p2_aes), dll_simplest_trivial2)$
statistics_cs(ip2_aes[2]);
[16, 559, 3838, 8, 6]
test_CNF_aes_sbox(ip2_aes);
true
h0_aes : rijnsbox2hittingcnf_fcs(dll_heuristics_first_formal)$
p0_aes : replace_by_prime_implicates_hitting(h0_aes)$
statistics_cs(p0_aes);
[16, 1722, 12118, 9, 5]
ip0_aes : first_irr_fcs(cs_to_fcs(p0_aes), dll_simplest_trivial2)$
statistics_cs(ip0_aes[2]);
[16, 680, 4784, 9, 5]
test_CNF_aes_sbox(ip0_aes);
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
     <li> Function test_CNF_aes_sbox delivers the required equivalence test.
     </li>
     <li> Then via hypergraph transversals we obtain all irredundant cores. 
     </li>
     <li> Likely there are (too) many sub-clause-sets, but we can try. </li>
    </ol>
   </li>
   <li> The question is how small can we get a CNF representation? We need
   also to investigate the size of a CNF obtained from the DNF via Tseitin
   translation (and optimisation); see
   ComputerAlgebra/Satisfiability/Lisp/PropositionalLogic/plans/TseitinTranslation.hpp
   </li>
  </ul>


  \todo Compute *all* prime implicates (for the purpose of analysis):
  <ul>
   <li> See
   ComputerAlgebra/Satisfiability/Lisp/Resolution/plans/PrimeImplicatesImplicants.hpp
   </li>
   <li> For a permutation of GF(2^8) we have 2 * 2^8 * 8 = 2^12 = 4096
   potential prime implicates, given by fixing 8 bit in either the input or
   the output, and one further bit to the wrong value. As one can see by
   the identity, these are not necessarily prime, since the identify
   just has 16 prime implicates (of length 2). </li>
   <li> One should test these. </li>
   <li> If the Maxima computation, via
   \verbatim
min_2resolution_closure_cs(rijnsbox_fullcnf_fcs()[2]);
   \endverbatim
   takes too long, then we need a C++ implementation; see
   OKlib/Satisfiability/FiniteFunctions/plans/general.hpp,
   "Prime implicants and implicates". A simple implementation should
   suffice here. </li>
   <li> Brute-force approaches:
    <ol>
     <li> There are 3^16 = 43,046,721 clauses with 16 variables altogether,
     so with a C++ implementation it is even no problem to run through
     all clauses and determine in this brute-force way the prime implicates. 
     </li>
     <li> We should write a little Maxima function which searches for prime 
     implicates (of length up to k) of a clause-set via this brute-force 
     approach (using some input SAT solver). This would need as input one of 
     the CNF-representations; since they are hitting clause-sets, we can then
     use a specialised SAT solver. </li>
     <li> We should also write a Maxima function for brute-force determination
     of all dual prime implicates (up to a given length), that is, the minimal 
     satisfying (partial) assignments. This is even simpler and doesn't need a 
     SAT solver. </li>
     <li> Perhaps we should write this function then also in C++ --- it's the 
     most simple function imaginable here, just run through all partial 
     assignments and check whether they satisfy the given clause-set. </li>
     <li> The only thing to think about is subsumption-elimination. But the
     simple implementation just does it at the end (since we have already a
     function for that!). As a side result, we obtain here the number of all
     satisfying partial assignments (which otherwise is actually not so easy to
     compute?!?). </li>
     <li> This would belong to module Satisfiability/Algorithms/AllSolutions;
     see Algorithms/AllSolutions/plans/MinimalAssignments.hpp. </li>
    </ol>
   </li>
   <li> However, since the S-box has a perfectly regular structure, it
   should also be possible to analytically determine all prime implicates:
    <ol>
     <li> Extract from the above results prime implicates, determine
     their structure, and generalise. </li>
     <li> See the 4096 candidates above. </li>
    </ol>
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


  \todo Related 8-bit bijections
  <ul>
   <li> Investigate random permutations of {0,1}^8, and compare them
   with the AES Sbox. </li>
   <li> 
   \verbatim
set_random_state(make_random_state(1))$
random_perm : random_permutation(makelist(i,i,0,255))$
random_dnf : generate_full_int_perm_dnf_fcs(random_perm)$
random_hcs : dualtreehittingcls_condensed_fcs(random_dnf,dll_heuristics_max_lit)$
statistics_cs(random_hcs);
[16,1536,19764,16,5]
random_pi : replace_by_prime_implicates_hitting(random_hcs)$
statistics_cs(random_pi);
[16,1363,9379,9,5]
ncl_list_cs(random_pi);
[[5,1],[6,298],[7,929],[8,132],[9,3]]
irredundant_bydef(cs_to_fcs(random_pi),dll_simplest_trivial1);
false
random_ip : first_irr_fcs(cs_to_fcs(random_pi),dll_simplest_trivial2)$
test_CNF_int_perm(random_ip,random_perm, dll_simplest_trivial1);
true
statistics_cs(random_ip[2]);
[16,565,3869,9,5]
ncl_list_fcs(random_ip);
[[5,1],[6,149],[7,351],[8,63],[9,1]]

set_random_state(make_random_state(25012))$ 
random_perm2 : random_permutation(makelist(i,i,0,255))$
random_dnf2 : generate_full_int_perm_dnf_fcs(random_perm2)$
random_hcs2 : dualtreehittingcls_condensed_fcs(random_dnf2,dll_heuristics_max_lit)$
statistics_cs(random_hcs2);
[16,1510,19456,16,6]
random_pi2 : replace_by_prime_implicates_hitting(random_hcs2)$
statistics_cs(random_pi2);
[16,1324,9102,9,5]
ncl_list_cs(random_pi2);
[[5,6],[6,306],[7,861],[8,150],[9,1]]
irredundant_bydef(cs_to_fcs(random_pi2),dll_simplest_trivial1);
random_ip2 : first_irr_fcs(cs_to_fcs(random_pi2),dll_simplest_trivial2)$
test_CNF_int_perm(random_ip2,random_perm2, dll_simplest_trivial1);
true
statistics_cs(random_ip2[2]);
[16,545,3731,9,5]
ncl_list_fcs(random_ip2);
[[5,3],[6,134],[7,353],[8,54],[9,1]]

set_random_state(make_random_state(3093215))$
random_perm3 : random_permutation(makelist(i,i,0,255))$
random_dnf3 : generate_full_int_perm_dnf_fcs(random_perm3)$
random_hcs3 : dualtreehittingcls_condensed_fcs(random_dnf3,dll_heuristics_max_lit)$
statistics_cs(random_hcs3);
[16,1576,20254,16,6]
random_pi3 : replace_by_prime_implicates_hitting(random_hcs3)$
statistics_cs(random_pi3);
[16,1383,9558,9,5]
ncl_list_cs(random_pi3);
[[5,1],[6,268],[7,968],[8,145],[9,1]]
irredundant_bydef(cs_to_fcs(random_pi3),dll_simplest_trivial1);
false
random_ip3 : first_irr_fcs(cs_to_fcs(random_pi3),dll_simplest_trivial2)$
test_CNF_int_perm(random_ip3,random_perm3, dll_simplest_trivial1);
true
statistics_cs(random_ip3[2]);
[16,563,3877,9,5]
ncl_list_fcs(random_ip3);
[[5,1],[6,124],[7,377],[8,60],[9,1]]
   \endverbatim
   </li>
   <li> Is there some kind of bound here on the number of prime implicates for
   permutations of this size, or is it simply that the permutations above are 
   not "random" enough? </li>
   <li> Would it be better to define certain measures for permutations, such as
    <ul>
     <li> Number of identity mappings (i.e |{x | f(x) = x}|) </li>
     <li> Hamming distance given binary representations for the elements </li>
    </ul>
   and then compare various permutations, with certain well defined properties 
   to see what properties cause larger CNF representations. This might also 
   might help us to better understand the AES Sbox. </li>
   <li> A simple function to compute a random bijection given an input set 
   seems appropriate here.
    <ol>
     <li> Which is the most appropriate module to put such
     a generic function within ComputerAlgebra? </li>
     <li> No such function is needed: Use random_permutation together with
     make_random_state and set_random_state. </li>
     <li> See example usage in this library. </li>
    </ol>
   </li>
   <li> (DONE See Cryptanalysis/Rijndael/plans/FieldOperationsAnalysis.hpp )
   Find representations for the multiplication with a constant in 
   GF(2^8). </li>
  </ul>
     

  \todo Organisation
  <ul>
   <li> Perhaps we should provide a constant for
   rijnsbox_fullcnf_fcs(). </li>
  </ul>


  \todo Representations of the Sbox using additional variables
  <ul>
   <li> Compare with "Investigating conditions and their representations"
   in 
   Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/plans/general.hpp. 
   </li>
   <li> When using (only) CNF, likely the use of additional variables is
   crucial. </li>
   <li> One should also consider the various arithmetical representations
   of the Sbox. </li>
   <li> DONE Most natural is to consider the Tseitin translation of the
   DNF-representation.
    <ol>
     <li> See
    ComputerAlgebra/Satisfiability/Lisp/PropositionalLogic/plans/TseitinTranslation.hpp
     </li>
     <li> Most natural should be to use dualts_fcl in
     ComputerAlgebra/Satisfiability/Lisp/ClauseSets/Constructions.mac. </li>
    </ol>
   </li>
  </ul>

*/

