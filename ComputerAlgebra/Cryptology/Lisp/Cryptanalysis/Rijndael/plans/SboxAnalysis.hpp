// Matthew Gwynne, 26.3.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/plans/SboxAnalysis.hpp
  \brief Plans for the Cryptanalysis of the Rijndael Sbox in Maxima/Lisp


  \todo Provide tests for all functions
  <ul>
   <li> generate_full_aes_sbox_cnf_fcs </li>
   <li> test_CNF_aes_sbox </li>
   <li> hitting_cnf_aes_sbox </li>
   <li> generate_full_aes_sbox_dnf_fcs </li>
   <li> binary_list_to_dnf_c </li>
   <li> DONE polynomial2list </li>
  </ul>

  
  \todo Generate good CNF hitting clause-sets for the AES Sbox
  <ul> 
   <li> Different heuristics for generating hitting clause-sets
   \verbatim
length(hitting_cnf_aes_sbox(dll_heuristics_first_formal));
2048
length(hitting_cnf_aes_sbox(dll_heuristics_first_real));
2048
length(hitting_cnf_aes_sbox(dll_heuristics_first_shortest_clause));
2048
statistics_cs(hitting_cnf_aes_sbox(dll_heuristics_max_lit));
[16, 1513, 19546, 16, 6]
length(hitting_cnf_aes_sbox(dll_heuristics_max_var));
2048
statistics_cs(hitting_cnf_aes_sbox(dll_heuristics_max_lit_tb(3,3)));
[16, 1468, 18925, 16, 6]
statistics_cs(hitting_cnf_aes_sbox(dll_heuristics_max_lit_tb(4,4)));
[16, 1460, 18873, 16, 5]
   \endverbatim
   Would be interesting to understand this. </li>
   <li> Also interesting that all the hitting trees produced by the
   SAT solvers are already condensed. </li>
   <li> Use heuristics_lookahead_distances with different lookahead-reductions
   and different distances. (MG: Looking at several combinations of the already
   defined reductions, all seem to lead to 2048. I am still trying different
   combinations) </li>
   <li> Since the AES-DNF is unique, as a correctness test we can
   just check whether we get the input back (using any heuristics):
   \verbatim
test_CNF_aes_sbox(cs_to_fcs(hitting_cnf_aes_sbox(dll_heuristics_max_lit)));
   \endverbatim
   </li>
   <li> We should also use reductions. For that we need the ability
   to translate r_k-splitting trees into hitting clause-sets, which can
   be done in a straightforward way, by just making the forced assignments
   into trees of levelled height 1 (ignoring the actual reduction). </li>
   <li> 
   A general conjecture is: "For computing small dual hitting cause-sets,
   use as splitting literal one which leads to maximal probability of
   satisfiability." So using choose_most_sat_literal_h in
   ComputerAlgebra/Satisfiability/Lisp/Backtracking/DLL_solvers.mac
   should do a good job --- let's test it here! 
   
\verbatim
statistics_cs(hitting_cnf_aes_sbox(choose_most_sat_literal_h(firstorder_sat_approx_t)))$
[16, 1515, 19540, 16, 6]
statistics_cs(hitting_cnf_aes_sbox(choose_most_sat_literal_h(satprob_dll_simplest_trivial1)))$
[16, 1516, 19573, 16, 6]
\endverbatim
   <ul>
    <li> One issue here is that some of the approximations tend to return "inf" 
    when the clauseset is satisfiable or highly satisfiable which is fine for 
    SAT decision, where the only interest is if the branch is satisfiable,
    but completely defeats the point in this case. In the tests above, such 
    problems have been avoided by taking the function definition and temporarily
    defining the function to return the calculated probability. Such functions 
    have been postfixed with "_t" for "true". </li>
    <li> The first result seems very similar to use of 
    "dll_heuristics_max_lit", which it should (in this particular case) given 
    at every stage, under any partial assignment, we always have a full clause 
    set, and each clause falsifies a given total assignment (given the domain of
    only 16 variables). Therefore, branching on the literal with the max 
    occurrences results in most clauses which represent falsifying assignments 
    occurring on that branch. Conversely, given we have a full clause set at 
    each stage, the negation of the maximally occurring literal must be the 
    minimally occurring literal, and given each clause represents a single total
    falsifying assignment, this other branch must represent the choice of 
    literal which leads to maximal probability of satisfiability (and therefore
    min of unsatisfiability). </li>
    <li> IE, in the case of full clause sets, "min_lit" is equivalent in nature
    to choosing the literal which leads to maximal probability of satisfiability
    and "max_lit" is equivalent in nature to choosing the literal which leads to
    maximal probability of unsatisfiability. </li>
    <li> Given this duality with full clause sets, it is arbitrary whether we
    choose to maximise satisfiability or unsatisfiability as you simply 
    generate splitting trees which are mirror images of each other (ignoring
    issues with the ordering imposed when there are ties for maximum and 
    minimum approximations). </li>
    <li> SAT approximations seem to generalise the advantage gained by 
    "max_lit" to clause sets rather than being restricted to full 
    clause sets. </li>
    <li> It is likely that the SAT approximation heuristic would benefit from
    some lookahead, as was presented with regards to max_lit, so as to 
    differentiate when ties occur for the maximal probability of satisfiability.
    We wish the same idea of maximising satisfiability to occur when we make
    the assignment and move 1 level down in the tree, and so the literal 
    which results in the most "bias" when that literal assignment is made 
    (or potentially taking into account both branches) should be chosen. </li>
    <li> The reasoning behind maximising "bias" at each level needs to be made
    clear here (falsifying assignments occurring along the same "path" down the
    tree result in less "branch off"s for the satisfying assignments, where 
    the "branch off"s are then the clauses we take for the dual hitting clause 
    set (after compacting?) - this is likely very imprecise and unclear). </li>
    <li> Most likely, there is a small difference here between "max_lit" and 
    the probability-based heuristic due to issues with ordering and joint
    maximums and minimums not necessarily coinciding. </li>
    <li> Likely this needs tidying or making more concise, as well as certain
    aspects more clearly explained/defined. </li>
   </ul>
   </li>
  </ul>


  \todo Extracting prime implicate representations from the hitting-cls-representations
  <ul>
   <li> As explained in "Hitting clause-sets" in
   ComputerAlgebra/Satisfiability/Lisp/Resolution/plans/PrimeImplicatesImplicants.hpp,
   given a hitting clause-set representation, from it we can obtain a shorter
   representation by prime implicates. This is a better representation. </li>
   <li> This is achieved by "replace_by_prime_implicates_hitting". </li>
   <li>
   \verbatim
h_aes : hitting_cnf_aes_sbox(dll_heuristics_max_lit)$
p_aes : replace_by_prime_implicates_hitting(h_aes)$
statistics_cs(p_aes);
[16, 1359, 9430, 9, 6]
ncl_list_f(cs_to_fcs(p_aes));
[[6, 265], [7, 913], [8, 180], [9, 1]]
irredundant_bydef(cs_to_fcs(p_aes), dll_simplest_trivial1);
false
ip_aes : first_irr_fcs(cs_to_fcs(p_aes), dll_simplest_trivial2)$
statistics_cs(ip_aes[2]);
[16, 581, 4006, 8, 6]
test_CNF_aes_sbox(ip_aes);
true
h2_aes : hitting_cnf_aes_sbox(dll_heuristics_max_lit_tb(4,4))$
test_CNF_aes_sbox(cs_to_fcs(h2_aes));
true
p2_aes : replace_by_prime_implicates_hitting(h2_aes)$
statistics_cs(p2_aes);
[16, 1374, 9524, 9, 5]
ip2_aes : first_irr_fcs(cs_to_fcs(p2_aes), dll_simplest_trivial2)$
statistics_cs(ip2_aes[2]);
[16, 566, 3898, 9, 5]
test_CNF_aes_sbox(ip2_aes);
true
   \endverbatim
   </li>
   <li> This looks very interesting! Should be close to the optimum.
   And looks much smaller than to be expected. </li>
   <li> Analysing p_aes:
   \verbatim
ir_p_aes : all_irr_bydef(cs_to_fcs(p_aes), dll_simplest_trivial2)$
statistics_cs(ir_p_aes);
[16, 447, 3087, 8, 6]
ncl_list_f(cs_to_fcs(ir_p_aes));
[[6, 99], [7, 291], [8, 57]]
oklib_monitor : true;
irrc_p_aes : all_irr_cores_bydef(cs_to_fcs(p_aes), dll_simplest_trivial2)$
   \endverbatim
   </li>
   <li> The computation of all irredundant cores of p_aes was aborted after
   having produced 559 cores, all with 629 clauses, while literal occurrences
   ranged from 4328 to 4330. There could be trillions of such tiny
   variations. Need to try random sampling. </li>
   <li> The question is how small can we get a CNF representation? We need
   also to investigate the size of a CNF obtained from the DNF via Tseitin
   translation (and optimisation); see
   ComputerAlgebra/Satisfiability/Lisp/PropositionalLogic/plans/TseitinTranslation.hpp
   </li>
  </ul>


  \todo Compute *all* prime implicates (for the purpose of analysis):
  <ul>
   <li> For a permutation of GF(2^8) we have 2 * 2^8 * 8 = 2^12 = 4096
   potential prime implicates, given by fixing 8 bit in either the inputor
   the output, and one further bit to the wrong value. As one can see by
   the identity, these are not neccessarily prime, since the identify
   just has 16 prime implicates (of length 2). </li>
   <li> One should test these. </li>
   <li> If the Maxima computation, via
   \verbatim
min_2resolution_closure_cs(generate_full_aes_sbox_cnf_fcs()[2]);
   \endverbatim
   takes too long, then we need a C++ implementation; see
   Transitional/Satisfiability/FiniteFunctions/plans/general.hpp,
   "Prime implicants and implicates". A simple implementation should
   suffice here. </li>
   <li> However, since the S-box has a perfectly regular structure, it
   should also be possible to analytically determine all prime implicates:
    <ol>
     <li> Extract from the above results prime implicates, determine
     their structure, and generalise. </li>
     <li> See the 4096 candidates above. </li>
    </ol>
   </li>
  </ul>


  \todo Related 8-bit bijections
  <ul>
   <li> Investigate random permutations of {0,1}^8, and compare them
   with the AES Sbox. </li>
   <li> Find representations for the multiplication with a constant in
   GF(2^8). </li>
  </ul>
     

  \todo Organisation
  <ul>
   <li> Perhaps we should provide a constant for
   generate_full_aes_sbox_cnf_fcs(). </li>
  </ul>


  \todo DONE Move functions which do not belong to this module.


  \todo DONE Sbox CNF and DNF generation functions
  <ul>
   <li> DONE Two small functions are needed which generate the CNF and DNF clause 
   sets for the AES Sbox. </li>
  </ul>

*/

