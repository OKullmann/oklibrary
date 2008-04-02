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
   <li> generate_full_aes_sbox_dnf_fcs </li>
   <li> generate_full_aes_sbox_cnf_fcs </li>
   <li> test_CNF_aes_sbox </li>
   <li> hitting_cnf_aes_sbox </li>
   <li> polynomial2list </li>
   <li> binary_list_to_dnf_c </li>
  </ul>


  \todo Move functions which do not belong to this module.

  
  \todo Generate good CNF clause-sets for the AES Sbox
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
   <li> As explained in "Hitting clause-sets" in
   ComputerAlgebra/Satisfiability/Lisp/Resolution/plans/PrimeImplicatesImplicants.hpp,
   given a hitting clause-set representation, from it we can obtain a shorter
   representation by prime implicates. This is a better representation.
    <ol>
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


  \todo DONE Sbox CNF and DNF generation functions
  <ul>
   <li> DONE Two small functions are needed which generate the CNF and DNF clause 
   sets for the AES Sbox. </li>
  </ul>

*/

