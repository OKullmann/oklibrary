// Matthew Gwynne, 26.3.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/plans/SboxAnalysis.hpp
  \brief Plans for the Cryptanalysis of the Rijndael Sbox in Maxima/Lisp

  
  \todo Generate good CNF clause-sets for the AES Sbox
   <li> Different heuristics for generating hitting clause-sets
   \verbatim
length(hitting_cnf_aes_sbox(dll_heuristics_first_formal));
2048
length(hitting_cnf_aes_sbox(dll_heuristics_first_real));
2048
length(hitting_cnf_aes_sbox(dll_heuristics_first_shortest_clause));
2048
length(hitting_cnf_aes_sbox(dll_heuristics_max_lit));
1513
length(hitting_cnf_aes_sbox(dll_heuristics_max_var));
2048
   \endverbatim
   Would be interesting to understand this. </li>
   <li> Use heuristics_lookahead_distances with different lookahead-reductions
   and different distances. </li>
   <li> Since the AES-DNF is unique, as a correctness test we can
   just check whether we get the input back (using any heuristics):
   \verbatim
is(cs_to_fcs(dualtreehittingcls_fcs(cs_to_fcs(hitting_cnf_aes_sbox(dll_heuristics_max_lit)),dll_heuristics_first_formal)) =
   generate_full_aes_sbox_dnf_fcs());
   \endverbatim
   </li>
   <li> We should also use reductions. For that we need the ability
   to translate r_k-splitting trees into hitting clause-sets, which can
   be done in a straightforward way, by just making the forced assignments
   into trees of levelled height 1 (ignoring the actual reduction). </li>
   <li> As explained in "Hitting clause-sets" in
   ComputerAlgebra/Satisfiability/Lisp/Resolution/plans/PrimeImplicatesImplicants.hpp,
   given a hitting clause-set representation, from it we can obtain a shorter
   representation by prime implicates. This is a better representation. </li>
   <li> The question is how small can we get a CNF representation? We need
   also to investigate the size of a CNF obtained from the DNF via Tseitin
   translation (and optimisation); see
   ComputerAlgebra/Satisfiability/Lisp/PropositionalLogic/plans/TseitinTranslation.hpp
   </li>
   <li> Compute *all* prime implicates (for the purpose of analysis):
    <ol>
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
    </ol>
   </li>
  </ul>



  \todo DONE Sbox CNF and DNF generation functions
  <ul>
   <li> DONE Two small functions are needed which generate the CNF and DNF clause 
   sets for the AES Sbox. </li>
  </ul>

*/

