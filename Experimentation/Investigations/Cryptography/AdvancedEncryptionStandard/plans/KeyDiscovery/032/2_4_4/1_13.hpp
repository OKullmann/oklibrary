// Matthew Gwynne, 15.2.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/Cryptography/AdvancedEncryptionStandard/plans/KeyDiscovery/032/2_4_4/1_13.hpp
  \brief Investigations into small-scale AES key discovery for 1+1/3 round AES with a 2x4 plaintext matrix and 4-bit field elements


  \todo Problem specification
  <ul>
   <li> We investigate the 1 + 1/3 round small-scale AES with 2 row,
   4 column, using the 4-bit field size. </li>
   <li> We denote this AES instance by aes(1,2,4,4). </li>
   <li> aes(1,2,4,4) takes a 32-bit plaintext and 32-bit key and
   outputs a 32-bit ciphertext. </li>
   <li> For the full specification of this AES instance, see
   "Problem specification" in
   Investigations/Cryptography/AdvancedEncryptionStandard/plans/KeyDiscovery/032/2_4_4/general.hpp.
   </li>
   <li> Note that we consider the canonical CNF translation, as
   this is an example of the "hardest" representation without
   new variables. See "Hardness of boolean function representations"
   in
   Experimentation/Investigations/BooleanFunctions/plans/general.hpp. </li>
  </ul>


  \todo Using the canonical box translation
  <ul>
   <li> Translation of aes(1,2,4,4):
    <ul>
     <li> The MixColumns operation is decomposed into its field
     multiplications (02 and 03) and addition operations. </li>
     <li> The MixColumns operation is translated by translating both
     the MixColumns operation and its inverse (it is self-inverse). </li>
     <li> We treat S-boxes, field multiplications and additions as boxes.
     </li>
     <li> The S-box and field multiplications are considered as a 8x1
     boolean functions, translated using the canonical translation;
     see dualts_fcl in
     ComputerAlgebra/Satisfiability/Lisp/FiniteFunctions/TseitinTranslation.mac.
     </li>
     <li> Additions of arity k are considered bit-wise as (k+1)-bit to 1-bit
     boolean functions; translated using their prime implicates. </li>
    </ul>
   </li>
   <li> Generating small-scale AES for 1 + 1/3 round:
   \verbatim
num_rounds : 1$
num_rows : 2$
num_columns : 4$
exp : 4$
final_round_b : false$
box_tran : aes_ts_box$
seed : 1$
mc_tran : aes_mc_bidirectional$
output_ss_fcl_std(num_rounds, num_columns, num_rows, exp, final_round_b, box_tran, mc_tran)$

shell> cat ssaes_r1_c4_rw2_e4_f0.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG n
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
1036 6750 19476 0 19476 1037 1
 length count
1 4
2 5376
3 624
4 32
9 672
16 42
   \endverbatim
   </li>
   <li> In this translation, we have:
    <ul>
     <li> 1 full round (Key Addition, SubBytes, and MixColumns operation).
     </li>
     <li> 10 Sboxes:
      <ul>
       <li> 8 from SubBytes = 8 byte * 1 rounds; </li>
       <li> 2 from key schedule = 2 row * 1 word * 1 rounds. </li>
      </ul>
     </li>
     <li> 16 multiplications by 02: 2 rows * 1 multiplication * 4 columns *
     1 round * 2 directions (forward + inverse). </li>
     <li> 16 multiplications by 03: 2 rows * 1 multiplication * 4 columns *
     1 round * 2 directions (forward + inverse). </li>
     <li> 160 additions:
      <ul>
       <li> 156 additions of arity 2:
        <ul>
         <li> 32 from key additions = 32 bits * 1 round; </li>
         <li> 32 from final key addition = 32 bits; </li>
         <li> 28 from the key schedule = (32 bits - 4 bits) * 1 round. </li>
         <li> 32 from forward MixColumns = 2 rows * 4 column * 4 bits *
         1 round; </li>
         <li> 32 from inverse MixColumns = 2 rows * 4 column * 4 bits * 1
         round. </li>
        </ul>
       </li>
       <li> 4 additions of arity 3:
        <ul>
         <li> 4 from the key schedule = 4 bits * 1 round. </li>
        </ul>
       </li>
      </ul>
     </li>
     <li> 4 bits for the constant in the key schedule = 4 bits * 1 rounds.
     </li>
    </ul>
   </li>
   <li> The number of clauses of each length in the canonical translation:
   \verbatim
maxima> ncl_list_full_dualts(8,16);
[[2,128],[9,16],[16,1]]
   \endverbatim
   </li>
   <li> This instance has 42 boxes = 10 S-boxes + 32 multiplications.
   </li>
   <li> This instance has the following number of clauses of length:
    <ul>
     <li> 1 : 4 = key schedule constant * 1; </li>
     <li> 2 : 5376 = 42 boxes * 128; </li>
     <li> 3 : 624 = 156 additions (arity 2) * 4; </li>
     <li> 4 : 32 = 4 additions (arity 3) * 8; </li>
     <li> 9 : 672 = 42 boxes * 16; </li>
     <li> 16 : 42 = 42 boxes * 1. </li>
    </ul>
   </li>
   <li> Then we can generate a random assignment with the plaintext and
   ciphertext, leaving the key unknown:
   \verbatim
maxima> output_ss_random_pc_pair(seed,num_rounds,num_columns,num_rows,exp,final_round_b);
   \endverbatim
   and the merging the assignment with the translation:
   \verbatim
shell> AppendDimacs-O3-DNDEBUG ssaes_r1_c4_rw2_e4_f0.cnf ssaes_pkpair_r1_c4_rw2_e4_f0_s1.cnf > r1_keyfind.cnf
   \endverbatim
   </li>
   <li> minisat-2.2.0 solves it in 0.03s:
   \verbatim
restarts              : 11
conflicts             : 1654           (55133 /sec)
decisions             : 2581           (0.00 % random) (86033 /sec)
propagations          : 211137         (7037900 /sec)
conflict literals     : 36509          (24.97 % deleted)
Memory used           : 19.00 MB
CPU time              : 0.03 s
   \endverbatim
   </li>
   <li> cryptominisat solves it in 0.03s:
   \verbatim
c restarts                 : 3
c conflicts                : 362         (12066.67  / sec)
c decisions                : 581         (0.00      % random)
c CPU time                 : 0.03        s
   \endverbatim
   ??? what does "Verified 1017 clauses." mean here ???
   </li>
   <li> OKsolver solves it in 0.9s:
   \verbatim
c running_time(sec)                     0.9
c number_of_nodes                       103
c number_of_2-reductions                3705
   \endverbatim
   </li>
  </ul>

*/
