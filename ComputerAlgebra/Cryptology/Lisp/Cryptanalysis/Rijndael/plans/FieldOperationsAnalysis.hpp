// Matthew Gwynne, 12.4.2008 (Swansea)
/* Copyright 2008, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/plans/FieldOperationsAnalysis.hpp
  \brief Plans for the Cryptanalysis of the Rijndael field operations in Maxima/Lisp


  \todo Add XOR translations for field multiplications
  <ul>
   <li> The AES field multiplications are simply multiplications by an 8x8 bit
   matrix (this is the same even for the small scale, it simply becomes a 4x4
   or e * e bit matrix), and therefore, we could simply translate this 
   function as a series of XOR (+ possibly negating variables) constraints. 
   </li>
   <li> Such translations might benefit solvers which make use of XOR 
   constraints. </li>
   <li> This should also be useful when considering rearranging linear 
   components of the AES (see "Rearranging linear components of Sbox and 
   MixColumns" in 
   ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/plans/Translations.hpp) . </li>
  </ul>

  
  \todo Generate good CNF hitting clause-sets for the AES Field Operations
  <ul>
   <li> Multiplication by 2 in Rijndael's byte field (GF(2^8)).
   \verbatim
statistics_cs(rijnmult2hittingcnf_fcs(intToGF2t8Poly(2),dll_heuristics_max_lit));
[16, 522, 7244, 16, 3]
   \endverbatim
   </li>
   <li> Multiplication by 3 in Rijndael's byte field (GF(2^8)).
   \verbatim
statistics_cs(rijnmult2hittingcnf_fcs(intToGF2t8Poly(3),dll_heuristics_max_lit));
[16, 842, 11592, 16, 3]
   \endverbatim
   </li>
   <li> Multiplication by 9 in Rijndael's byte field (GF(2^8)).
   \verbatim
statistics_cs(rijnmult2hittingcnf_fcs(intToGF2t8Poly(9),dll_heuristics_max_lit));
[16, 1192, 15742, 16, 4]
   \endverbatim
   </li>
   <li> Multiplication by 11 in Rijndael's byte field (GF(2^8)).
   \verbatim
statistics_cs(rijnmult2hittingcnf_fcs(intToGF2t8Poly(11),dll_heuristics_max_lit));
[16, 1270, 17012, 16, 5]
   \endverbatim
   </li>
   <li> Multiplication by 13 in Rijndael's byte field (GF(2^8)).
   \verbatim
statistics_cs(rijnmult2hittingcnf_fcs(intToGF2t8Poly(13),dll_heuristics_max_lit));
[16, 1230, 16180, 16, 6]
   \endverbatim
   </li>
   <li> Multiplication by 14 in Rijndael's byte field (GF(2^8)).
   \verbatim
statistics_cs(rijnmult2hittingcnf_fcs(intToGF2t8Poly(14),dll_heuristics_max_lit));
[16, 1312, 17406, 16, 4]
   \endverbatim
   </li>
  </ul>
  
  
  \todo Extracting prime implicate representations from the hitting-cls-representations
  <ul>
   <li> As explained in "Hitting clause-sets" in
   ComputerAlgebra/Satisfiability/Lisp/Resolution/plans/PrimeImplicatesImplicants.hpp,
   given a hitting clause-set representation, from it we can obtain a shorter
   representation by prime implicates. This is a better representation. </li>
   <li> This is achieved by "replace_by_prime_implicates_hitting". </li>
   <li> Multiplication by 2 
   \verbatim
h_aes : rijnmult2hittingcnf_fcs(intToGF2t8Poly(2),dll_heuristics_max_lit)$
p_aes : replace_by_prime_implicates_hitting(h_aes)$
statistics_cs(p_aes);
[16, 27, 71, 3, 2]
ncl_list_cs(p_aes);
[[2, 10], [3, 17]]
irredundant_bydef(cs_to_fcs(p_aes), dll_simplest_trivial1);
false
ip_aes : first_irr_fcs(cs_to_fcs(p_aes), dll_simplest_trivial2)$
statistics_cs(ip_aes[2]);
[16, 21, 54, 3, 2]
ncl_list_fcs(ip_aes);
[[2, 9], [3, 12]]
rijnmult_cnfp(intToGF2t8Poly(2),ip_aes);
true
   \endverbatim
   </li>
   <li> Multiplication by 3 
   \verbatim
h_aes : rijnmult2hittingcnf_fcs(intToGF2t8Poly(3),dll_heuristics_max_lit)$
p_aes : replace_by_prime_implicates_hitting(h_aes)$
statistics_cs(p_aes);
[16, 171, 888, 9, 3]
ncl_list_cs(p_aes);
[[3, 23], [4, 34], [5, 51], [6, 34], [7, 14], [8, 9], [9, 6]]
irredundant_bydef(cs_to_fcs(p_aes), dll_simplest_trivial1);
false
ip_aes : first_irr_fcs(cs_to_fcs(p_aes), dll_simplest_trivial2)$
statistics_cs(ip_aes[2]);
[16, 61, 271, 9, 3]
ncl_list_fcs(ip_aes);
[[3, 19], [4, 19], [5, 11], [6, 6], [7, 2], [8, 3], [9, 1]]
rijnmult_cnfp(intToGF2t8Poly(3),ip_aes);
true
   \endverbatim
   </li>
   <li> Multiplication by 9
   \verbatim
h_aes : rijnmult2hittingcnf_fcs(intToGF2t8Poly(9),dll_heuristics_max_lit)$
p_aes : replace_by_prime_implicates_hitting(h_aes)$
statistics_cs(p_aes);
[16, 367, 2110, 9, 3]
ncl_list_cs(p_aes);
[[3, 16], [4, 49], [5, 94], [6, 97], [7, 77], [8, 31], [9, 3]]
irredundant_bydef(cs_to_fcs(p_aes), dll_simplest_trivial1);
false
ip_aes : first_irr_fcs(cs_to_fcs(p_aes), dll_simplest_trivial2)$
statistics_cs(ip_aes[2]);
[16, 86, 402, 8, 3]
ncl_list_fcs(ip_aes);
[[3, 12], [4, 32], [5, 24], [6, 10], [7, 6], [8, 2]]
rijnmult_cnfp(intToGF2t8Poly(3),ip_aes);
true
   \endverbatim
   </li>
   <li> Multiplication by 3 
   \verbatim
h_aes : rijnmult2hittingcnf_fcs(intToGF2t8Poly(3),dll_heuristics_max_lit)$
p_aes : replace_by_prime_implicates_hitting(h_aes)$
statistics_cs(p_aes);
[16, 171, 888, 9, 3]
ncl_list_cs(p_aes);
[[3, 23], [4, 34], [5, 51], [6, 34], [7, 14], [8, 9], [9, 6]]
irredundant_bydef(cs_to_fcs(p_aes), dll_simplest_trivial1);
false
ip_aes : first_irr_fcs(cs_to_fcs(p_aes), dll_simplest_trivial2)$
statistics_cs(ip_aes[2]);
[16, 61, 271, 9, 3]
ncl_list_fcs(ip_aes);
[[3, 19], [4, 19], [5, 11], [6, 6], [7, 2], [8, 3], [9, 1]]
rijnmult_cnfp(intToGF2t8Poly(3),ip_aes);
true
   \endverbatim
   </li>
   <li> Multiplication by 9
   \verbatim
h_aes : rijnmult2hittingcnf_fcs(intToGF2t8Poly(9),dll_heuristics_max_lit)$
p_aes : replace_by_prime_implicates_hitting(h_aes)$
statistics_cs(p_aes);
[16, 367, 2110, 9, 3]
ncl_list_cs(p_aes);
[[3, 16], [4, 49], [5, 94], [6, 97], [7, 77], [8, 31], [9, 3]]
irredundant_bydef(cs_to_fcs(p_aes), dll_simplest_trivial1);
false
ip_aes : first_irr_fcs(cs_to_fcs(p_aes), dll_simplest_trivial2)$
statistics_cs(ip_aes[2]);
[16, 86, 402, 8, 3]
ncl_list_fcs(ip_aes);
[[3, 12], [4, 32], [5, 24], [6, 10], [7, 6], [8, 2]]
rijnmult_cnfp(intToGF2t8Poly(3),ip_aes);
true
   \endverbatim
   </li>
   <li> Multiplication by 11
   \verbatim
h_aes : rijnmult2hittingcnf_fcs(intToGF2t8Poly(11),dll_heuristics_max_lit)$
p_aes : replace_by_prime_implicates_hitting(h_aes)$
statistics_cs(p_aes);
[16, 492, 2896, 9, 4]
ncl_list_cs(p_aes);
[[4, 40], [5, 157], [6, 155], [7, 103], [8, 33], [9, 4]]
irredundant_bydef(cs_to_fcs(p_aes), dll_simplest_trivial1);
false
ip_aes : first_irr_fcs(cs_to_fcs(p_aes), dll_simplest_trivial2)$
statistics_cs(ip_aes[2]);
[16, 148, 767, 7, 4]
ncl_list_fcs(ip_aes);
[[4, 28], [5, 75], [6, 35], [7, 10]]
rijnmult_cnfp(intToGF2t8Poly(11),ip_aes);
true
   \endverbatim
   </li>
   <li> Multiplication by 13
   \verbatim
h_aes : rijnmult2hittingcnf_fcs(intToGF2t8Poly(13),dll_heuristics_max_lit)$
p_aes : replace_by_prime_implicates_hitting(h_aes)$
statistics_cs(p_aes);
[16, 489, 2885, 9, 4]
ncl_list_cs(p_aes);
[[4, 57], [5, 118], [6, 171], [7, 106], [8, 34], [9, 3]]
irredundant_bydef(cs_to_fcs(p_aes), dll_simplest_trivial1);
false
ip_aes : first_irr_fcs(cs_to_fcs(p_aes), dll_simplest_trivial2)$
statistics_cs(ip_aes[2]);
[16, 139, 727, 9, 4]
ncl_list_fcs(ip_aes);
[[4, 30], [5, 63], [6, 32], [7, 13], [9, 1]]
rijnmult_cnfp(intToGF2t8Poly(3),ip_aes);
true
   \endverbatim
   </li>
   <li> Multiplication by 14
   \verbatim
h_aes : rijnmult2hittingcnf_fcs(intToGF2t8Poly(14),dll_heuristics_max_lit)$
p_aes : replace_by_prime_implicates_hitting(h_aes)$
statistics_cs(p_aes);
[16, 456, 2689, 9, 3]
ncl_list_cs(p_aes);
[[3, 4], [4, 47], [5, 118], [6, 153], [7, 96], [8, 33], [9, 5]]
irredundant_bydef(cs_to_fcs(p_aes), dll_simplest_trivial1);
false
ip_aes : first_irr_fcs(cs_to_fcs(p_aes), dll_simplest_trivial2)$
statistics_cs(ip_aes[2]);
[16, 119, 612, 8, 3]
ncl_list_fcs(ip_aes);
[[3, 3], [4, 33], [5, 42], [6, 28], [7, 11], [8, 2]]
rijnmult_cnfp(intToGF2t8Poly(3),ip_aes);
true
   \endverbatim
   </li>
  </ul>
  

  \todo Compute *all* prime implicates (for the purpose of analysis):
  
 
  \todo Find the symmetries of the AES Field Operations


  \bug DONE What's the purpose of rijnmult_cnfp 
  <ul>
   <li> There is exactly one "full" CNF clause-set for a field multiplication,
   so what is the point of this function? </li>
   <li> rijnmult_cnfp tests whether any CNF (not necessarily full) represents
   multiplication by the given AES field element. </li>
  </ul>

  
  \todo DONE Provide tests for all functions


  \bug DONE rijnmult_fulldnf_fcs only works for trivial arguments
  <ul>
   <li> "rijnmult_fulldnf_fcs(n)" for n >= 10 produces
   various errors. </li>
   <li> This occurred as "aes_field_mul_data", which is an
   lookup array for the field operations which are actually
   considered in the AES, was used to lookup the values
   for translation, and for any field elements not considered
   in the AES, the array was not defined, leading to errors.
   This has been fixed by using a simple if statements which
   uses the gf-package multiplication routines if the data
   is not available from the array. </li>
  </ul>



*/
