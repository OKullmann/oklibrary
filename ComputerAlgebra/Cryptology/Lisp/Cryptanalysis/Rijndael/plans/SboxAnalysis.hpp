// Matthew Gwynne, 26.3.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/plans/SboxAnalysis.hpp
  \brief Plans for the Cryptanalysis of the Rijndael Sbox in Maxima/Lisp

  
  \todo Generate "good" CNF hitting clause-sets for the AES Sbox
  <ul>
   <li> Different heuristics:
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
  </ul>


  \todo DONE Sbox CNF and DNF generation functions
  <ul>
   <li> DONE Two small functions are needed which generate the CNF and DNF clause 
   sets for the AES Sbox. </li>
  </ul>

*/

