// Matthew Gwynne, 15.2.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/KeyDiscovery/128/4_4_8/1_13.hpp
  \brief Investigations into AES key discovery for one round AES (1+1/3)


  \todo Problem specification
  <ul>
   <li> We investigate the 1 + 1/3 round small scale AES with 4 row,
   4 column, using the 8-bit field size. </li>
   <li> We denote this AES instance by aes(1,4,4,8). </li>
   <li> aes(1,4,4,8) takes a 128-bit plaintext and 128-bit key and
   outputs a 128-bit ciphertext. </li>
   <li> For the full specification of this AES instance, see
   "Problem specification" in
   Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/KeyDiscovery/128/4_4_8/general.hpp.
   </li>
   <li> Note that we consider the canonical CNF translation, as
   this is an example of the "hardest" representation without
   new variables. See "Hardness of boolean function representations"
   in
   Experimentation/Investigations/BooleanFunctions/plans/general.hpp. </li>
  </ul>


  \todo Using the canonical box translation
  <ul>
   <li> Translation of aes(1,4,4,8):
    <ul>
     <li> The MixColumns operation is decomposed into it's field
     multiplications (02 and 03) and addition operations. </li>
     <li> The MixColumns operation is translated by translating both
     the MixColumns operation and it's inverse (it is self-inverse). </li>
     <li> We treat S-boxes, field multiplications and additions as boxes.
     </li>
     <li> The S-box and field multiplications are considered as a 16-bit to
     1-bit boolean functions, translated using the canonical translation;
     see dualts_fcl in
     ComputerAlgebra/Satisfiability/Lisp/FiniteFunctions/TseitinTranslation.mac.
     </li>
     <li> Additions of arity k are considered bit-wise as (k+1)-bit to 1-bit
     boolean functions; translated using their prime implicates. </li>
    </ul>
   </li>
   <li> Generating AES for 1 + 1/3 round:
   \verbatim
num_rounds : 1$
num_rows : 4$
num_columns : 4$
exp : 8$
final_round_b : false$
box_tran : aes_ts_box$
seed : 1$
mc_tran : aes_mc_bidirectional$
output_ss_fcl_std(num_rounds, num_columns, num_rows, exp, final_round_b, box_tran, mc_tran)$

???
Maxima encountered a Lisp error:

 Memory limit reached. Please jump to an outer pointer, quit program and enlarge the
memory limits before executing the program again.
???

shell> cat ssaes_r1_c4_rw4_e8_f0.cnf | ExtendedDimacsFullStatistics n
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
31400 510620 1510056 0 1510056 31401 1
 length count
1 8
2 475136
3 1504
4 64
5 4096
17 29696
256 116
   \endverbatim
   </li>
   <li> In this translation, we have:
    <ul>
     <li> 1 full round (Key addition, SubBytes and MixColumns). </li>
     <li> 20 Sboxes:
      <ul>
       <li> 16 from SubBytes = 16 bytes * 1 round. </li>
       <li> 4 from key schedule = 1 column * 4 bytes * 1 round. </li>
      </ul>
     <li> 640 additions:
      <ol>
       <li> 376 arity two additions:
        <ul>
         <li> 128 from key additions = 128 bits * 1 round. </li>
         <li> 128 from final key addition = 128 bits. </li>
         <li> 120 from key schedule = (128 - 8) bits *  1 round. </li>
        </ul>
       </li>
       <li> 8 arity three additions: 8 from key schedule = 8 bits * 1 round.
       </li>
       <li> 256 arity four additions: 256 from summation in
       MixColumns matrix multiplication = 4 additions * 4 columns * 16 bits *
       1 round. </li>
      </ul>
     </li>
     <li> 16 multiplications by 02: 4 rows * 1 multiplication * 4 columns
     * 1 round. </li>
     <li> 16 multiplications by 03: 4 rows * 1 multiplication * 4 columns
     * 1 round. </li>
     <li> 16 multiplications by 09: 4 rows * 1 multiplication * 4 columns
     * 1 round. </li>
     <li> 16 multiplications by 11: 4 rows * 1 multiplication * 4 columns
     * 1 round. </li>
     <li> 16 multiplications by 13: 4 rows * 1 multiplication * 4 columns
     * 1 round. </li>
     <li> 16 multiplications by 14: 4 rows * 1 multiplication * 4 columns
     * 1 round. </li>
     <li> 8 bits set for the constant in the key schedule. </li>
    </ul>
   </li>
   <li> The number of clauses of each length in the canonical box
   translations:
   \verbatim
maxima> ncl_list_full_dualts(16,256);
[[2,4096],[17,256],[256,1]]
   \endverbatim
   </li>
   <li> The instance has 20 S-boxes + 16 * 6 multiplications = 116 boxes
   (excluding additions). </li>
   <li> The instance has the following number of clauses of the
   following size:
    <ul>
     <li> 1 : 8 = key schedule constant * 1; </li>
     <li> 2 : 475136 = 116 boxes * 4096; </li>
     <li> 3 : 1504 = 376 additions (arity 2) * 4; </li>
     <li> 4 : 64 = 8 additions (arity 3) * 8; </li>
     <li> 5 : 4096 = 256 additions (arity 4) * 16; </li>
     <li> 16 : 29696 = 116 boxes * 256 clauses; </li>
     <li> 256 : 116 = 116 boxes * 1. </li>
    </ul>
   </li>
   <li> Then we can generate a random assignment with the plaintext and
   ciphertext, leaving the key unknown:
   \verbatim
maxima> output_ss_random_pc_pair(seed,num_rounds,num_columns,num_rows,exp,final_round_b);
   \endverbatim
   and the merging the assignment with the translation:
   \verbatim
shell> AppendDimacs-O3-DNDEBUG ssaes_r1_c4_rw4_e8_f0.cnf ssaes_pkpair_r1_c4_rw4_e8_f0_s1.cnf > ssaes_r1_c4_rw4_e8_f0_keyfind.cnf
   \endverbatim
   </li>
   <li> MG is running experiments with various solvers, but all are ongoing
   (after a day). </li>
   <li> The next thing to try is replacing the boxes with the r_1 bases we
   have. </li>
  </ul>

*/
