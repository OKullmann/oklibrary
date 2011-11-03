// Matthew Gwynne, 15.2.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/Cryptography/AdvancedEncryptionStandard/plans/KeyDiscovery/032/2_4_4/5_13.hpp
  \brief Investigations into small-scale AES key discovery for 5+1/3 round AES with a 2x4 plaintext matrix and 4-bit field elements


  \todo Problem specification
  <ul>
   <li> We investigate the 5 + 1/3 round small-scale AES with 2 row,
   4 column, using the 4-bit field size. </li>
   <li> We denote this AES instance by aes(5,2,4,4). </li>
   <li> aes(5,2,4,4) takes a 32-bit plaintext and 32-bit key and
   outputs a 32-bit ciphertext. </li>
   <li> For the full specification of this AES instance, see
   "Problem specification" in
   Cryptography/AdvancedEncryptionStandard/plans/KeyDiscovery/032/2_4_4/general.hpp.
   </li>
   <li> Note that we consider the canonical CNF translation, as
   this is an example of the "hardest" representation without
   new variables. See "Hardness of boolean function representations"
   in
   Experimentation/Investigations/BooleanFunctions/plans/general.hpp. </li>
  </ul>


  \todo Using the 1-base box translation
  <ul>
   <li> Translation of aes(5,2,4,4):
    <ul>
     <li> The MixColumns operation is decomposed into its field
     multiplications (02 and 03) and addition operations. </li>
     <li> The MixColumns operation is translated by translating both
     the MixColumns operation and its inverse (it is self-inverse). </li>
     <li> We treat S-boxes, field additions and additions as boxes. </li>
     <li> The S-box and field multiplications are considered as a 8x1
     boolean function, translated using 1-bases; see ss_sbox_rbase_cnfs
     in
     ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/data/SmallScaleSboxCNF.mac.
     </li>
     <li> Additions of arity k are considered bit-wise as (k+1)-bit to 1-bit
     boolean functions; translated using their prime implicates. </li>
    </ul>
   </li>
   <li> Generating small-scale AES for 5 + 1/3 round:
   \verbatim
num_rounds : 5$
num_rows : 2$
num_columns : 4$
exp : 4$
final_round_b : false$
box_tran : aes_rbase_box$
seed : 1$
mc_tran : aes_mc_bidirectional$
output_ss_fcl_std(num_rounds, num_columns, num_rows, exp, final_round_b, box_tran, mc_tran)$

shell> cat ssaes_r5_c4_rw2_e4_f0.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG n
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
1436 6858 21604 0 21604 1437 1
 length count
1 20
2 480
3 4808
4 1550
   \endverbatim
   </li>
   <li> In this translation, we have:
    <ul>
     <li> 5 full rounds (Key Addition, SubBytes, and MixColumns operation).
     </li>
     <li> 50 Sboxes:
      <ul>
       <li> 40 from SubBytes = 8 byte * 5 rounds; </li>
       <li> 10 from key schedule = 2 row * 1 word * 5 rounds. </li>
      </ul>
     </li>
     <li> 80 multiplications by 02: 2 rows * 1 multiplication * 4 columns *
     5 rounds * 2 directions (forward + inverse). </li>
     <li> 80 multiplications by 03: 2 rows * 1 multiplication * 4 columns *
     5 rounds * 2 directions (forward + inverse). </li>
     <li> 652 additions:
      <ul>
       <li> 652 additions of arity 2:
        <ul>
         <li> 160 from key additions = 32 bits * 5 rounds; </li>
         <li> 32 from final key addition = 32 bits; </li>
         <li> 140 from the key schedule = (32 bits - 4 bits) * 5 rounds. </li>
         <li> 160 from forward MixColumns = 2 rows * 4 column * 4 bits *
         5 rounds; </li>
         <li> 160 from inverse MixColumns = 2 rows * 4 column * 4 bits * 5
         rounds. </li>
        </ul>
       </li>
       <li> 20 additions of arity 3:
        <ul>
         <li> 20 from the key schedule = 4 bits * 5 round. </li>
        </ul>
       </li>
      </ul>
     </li>
     <li> 20 bits for the constant in the key schedule = 4 bits * 5 rounds.
     </li>
    </ul>
   </li>
   <li> The number of clauses of each length in the translation, computed by:
   \verbatim
maxima> ncl_list_fcs(ev_hm(ss_sbox_rbase_cnfs,4));
[[3,12],[4,15]]
maxima> ncl_list_fcs(ev_hm(ss_field_rbase_cnfs,[4,2]));
[[2,6],[3,4]]
maxima> ncl_list_fcs(ev_hm(ss_field_rbase_cnfs,[4,3]));
[[3,16],[4,8]]
   \endverbatim
   </li>
   <li> This instance has the following number of clauses of length:
    <ul>
     <li> 1 : 20 = key schedule constant * 1; </li>
     <li> 2 : 480 = 80 multiplications by 02 * 6; </li>
     <li> 3 : 4808 = 50 S-boxes * 12 + 80 multiplications by 02 * 4 +
     80 multiplications by 03 * 16 + 652 additions (arity 2) * 4; </li>
     <li> 4 : 1550 = 50 S-boxes * 15 + 80 multiplications by 03 * 8 +
     20 additions (arity 3) * 8. </li>
    </ul>
   </li>
   <li> Then we can generate a random assignment with the plaintext and
   ciphertext, leaving the key unknown:
   \verbatim
maxima> output_ss_random_pc_pair(seed,num_rounds,num_columns,num_rows,exp,final_round_b);
   \endverbatim
   and the merging the assignment with the translation:
   \verbatim
shell> AppendDimacs-O3-DNDEBUG ssaes_r5_c4_rw2_e4_f0.cnf ssaes_pkpair_r5_c4_rw2_e4_f0_s1.cnf > r5_keyfind.cnf
   \endverbatim
   </li>
   <li> Picosat solves it in ~15 minutes:
   \verbatim
c 0 restarts
c 4596920 conflicts
c 4789770 decisions
c 2809104667 propagations
c 903.5 seconds total run time
   \endverbatim
   </li>
   <li> All other solvers run in the experiment were unable to solve the
   instance within 10 hours (cryptominisat, precosat236, precosat-570.1,
   minisat-2.2.0). </li>
  </ul>

*/
