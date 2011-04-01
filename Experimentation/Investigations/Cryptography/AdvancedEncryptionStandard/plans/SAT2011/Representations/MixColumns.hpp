// Oliver Kullmann, 11.2.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/Representations/MixColumns.hpp
  \brief Studying good representations of the AES (small-scale) MixColumns operations


  \todo Sizes (and structure)
  <ul>
   <li> To start, we need precise and explained data on the sizes of the
   MixColumns-permutations for AES and variations. </li>
  </ul>


  \todo Direct linear representation
  <ul>
   <li> Every linear map over ZZ_2 from {0,1}^n to {0,1}^m can be described by
   m sums over the input variables. </li>
   <li> We need to determine this representation, and we need to find out the
   relation to our sum-of-boxes representation. </li>
   <li> For the four by four MixColumns operation at the 4-bit level, we have
   the following boolean matrix:
   \verbatim
maxima> ss_mixcolumn_boolm(2,4,4);
matrix([0,1,0,0,1,1,0,0,1,0,0,0,1,0,0,0],
       [0,0,1,0,0,1,1,0,0,1,0,0,0,1,0,0],
       [1,0,0,1,1,0,1,1,0,0,1,0,0,0,1,0],
       [1,0,0,0,1,0,0,1,0,0,0,1,0,0,0,1],
       [1,0,0,0,0,1,0,0,1,1,0,0,1,0,0,0],
       [0,1,0,0,0,0,1,0,0,1,1,0,0,1,0,0],
       [0,0,1,0,1,0,0,1,1,0,1,1,0,0,1,0],
       [0,0,0,1,1,0,0,0,1,0,0,1,0,0,0,1],
       [1,0,0,0,1,0,0,0,0,1,0,0,1,1,0,0],
       [0,1,0,0,0,1,0,0,0,0,1,0,0,1,1,0],
       [0,0,1,0,0,0,1,0,1,0,0,1,1,0,1,1],
       [0,0,0,1,0,0,0,1,1,0,0,0,1,0,0,1],
       [1,1,0,0,1,0,0,0,1,0,0,0,0,1,0,0],
       [0,1,1,0,0,1,0,0,0,1,0,0,0,0,1,0],
       [1,0,1,1,0,0,1,0,0,0,1,0,1,0,0,1],
       [1,0,0,1,0,0,0,1,0,0,0,1,1,0,0,0])
   \endverbatim
   </li>
   <li> The default matrix for the four by four 4-bit AES has no more than 7 
   ones, out of 16, in any row in it's matrix:
   \verbatim
maxima> num_ones_in_list(L) := length(sublist(L,lambda([e],is(e=1))))$
maxima> map(num_ones_in_list,args(ss_mixcolumn_boolm(2,4,4)));
[5,5,7,5,5,5,7,5,5,5,7,5,5,5,7,5]
   \endverbatim
   </li>
   <li> For the four by four MixColumns operation at the 8-bit level,
   we have the following boolean matrix:
   \verbatim
maxima> ss_mixcolumn_boolm(2,8,4);
matrix([0,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0],
       [0,0,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0],
       [0,0,0,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0],
       [1,0,0,0,1,0,0,0,1,0,0,1,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0],
       [1,0,0,0,0,1,0,0,1,0,0,0,1,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0],
       [0,0,0,0,0,0,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0],
       [1,0,0,0,0,0,0,1,1,0,0,0,0,0,1,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0],
       [1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1],
       [1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0],
       [0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,0,0,0,0,0,0],
       [0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,0,0,0,0,0],
       [0,0,0,1,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,1,1,0,0,0,0,0,0,1,0,0,0,0],
       [0,0,0,0,1,0,0,0,1,0,0,0,0,1,0,0,1,0,0,0,1,1,0,0,0,0,0,0,1,0,0,0],
       [0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,0,0],
       [0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,1,1,0,0,0,0,0,1,1,0,0,0,0,0,0,1,0],
       [0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1],
       [1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0],
       [0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,1,0,0,0,0,0],
       [0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,1,0,0,0,0],
       [0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,1,1,0,0,0],
       [0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,1,0,0,1,0,0,0,1,1,0,0],
       [0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,1,0],
       [0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,1,1,0,0,0,0,0,1,1],
       [0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1],
       [1,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0],
       [0,1,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0],
       [0,0,1,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0],
       [1,0,0,1,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,1,0,0,0],
       [1,0,0,0,1,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,1,0,0],
       [0,0,0,0,0,1,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0],
       [1,0,0,0,0,0,1,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,1],
       [1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0])
   \endverbatim
   </li>
   <li> The default matrix for the four by four 8-bit AES seems 
   sparse, having no more than 7 ones, out of 32, in any row in it's
   matrix:
   \verbatim
maxima> num_ones_in_list(L) := length(sublist(L,lambda([e],is(e=1))))$
maxima> map(num_ones_in_list,args(ss_mixcolumn_boolm(2,8,4)));
[5,5,5,7,7,5,7,5,5,5,5,7,7,5,7,5,5,5,5,7,7,5,7,5,5,5,5,7,7,5,7,5]
   \endverbatim
   </li>
  </ul>


  \todo Computation of r_k-bases
  <ul>
   <li> See Experimentation/Investigations/BooleanFunctions/plans/Permutations/Linear.hpp
   for general investigations. </li>
   <li> For very small sizes we can directly compute r_0- and r_1-bases. </li>
   <li> Can we learn something from that for the bigger cases? </li>
  </ul>


  \todo 16-bit MixColumn operation (2x2 matrix)
  <ul>
   <li> Consider the small scale MixColumns matrix with
   two columns and two rows, acting on a single 8-bit column 
   (two rows of 4-bits). </li>
   <li> The function takes an 8-bit column and returns
   the result of left multiplying the default small scale 2 by 2 MixColumns 
   matrix by the column. This function is given by "ss_mixcolumn_bf" in
   ComputerAlgebra/Cryptology/Lisp/CryptoSystems/Rijndael/SmallScaleAdvancedEncryptionStandard.mac
   and is a 16-bit boolean function. </li>
   <li> The boolean function has the following statistics for
   it's prime implicates:
   \verbatim
maxima> output_fcs("32-bit MixColumns",bf2relation_fullcnf_fcs(lambda([V],ss_mixcolumn_bf(V,2,4)), 8),"mixcolumn_32.cnf");
shell> QuineMcCluskey-n16-O3-DNDEBUG mixcolumn_32.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG n
n non_taut_c red_l taut_c orig_l comment_count finished_bool
16 11424 88704 0 88704 1 1
 length count
4 160
6 1024
8 10240
   \endverbatim
   </li>
   <li> We have an r1-base with the following statistics:
   \verbatim
shell> RandomRUcpBases mixcolumn_32.cnf
<snip>
------------------------------------------------------------------------------------
CURRENT MINIMUM RBASE: *2208* with gs=1,bs=1
------------------------------------------------------------------------------------
*** Currently trying gs=1,bs=1
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
16 2208 17024 0 17024 0 1
 length count
4 160
5 0
6 0
7 0
8 2048
<snip>
   \endverbatim
   </li>
   <li> For 50 different random r1-bases, with seeds for the RUcpGen
   ranging from 1-10 and seeds for the RUcpBase ranging from 1 to 5,
   ALL r1_bases had the same statistics. </li>
   <li> Are these r1-bases actually the same? </li>
   <li> All r_1-bases we generated for the multiplication by 03 were also the
   same. Is the inclusion of the 03 in the MixColumns matrix the reason we
   seem to have a unique r_1-base? </li>
   <li> Not all linear maps have unique r_1-bases, as can be seen by 
   considering the multiplication by 02 in "r_1-bases" in
   Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/Representations/Mul_2_8.hpp .
   </li>
   <li> See also "r_1-bases" in
   Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/Representations/Mul_3_8.hpp.
   </li>
  </ul>

*/
