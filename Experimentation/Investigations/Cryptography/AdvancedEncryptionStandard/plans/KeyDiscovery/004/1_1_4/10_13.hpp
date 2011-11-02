// Matthew Gwynne, 15.2.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/Cryptography/AdvancedEncryptionStandard/plans/KeyDiscovery/004/1_1_4/10_13.hpp
  \brief Investigations into small-scale AES key discovery with 1 row, 1 column and 4-bit field elements for 10 rounds AES with MixColumns


  \todo Overview
  <ul>
   <li> We investigate the 10 + 1/3 round small-scale AES with
   1 row, 1 column, using the 4-bit field size. </li>
   <li> We denote this AES instance by aes(10,1,1,4). </li>
   <li> aes(10,1,1,4) takes a 4-bit plaintext and 4-bit key and
   outputs a 4-bit ciphertext. </li>
   <li> For the full specification of this AES instance, see
   "Problem specification" in
   Investigations/Cryptography/AdvancedEncryptionStandard/plans/KeyDiscovery/004/1_1_4/general.hpp.
   </li>
   <li> For a full list of the possible translations, see
   "Investigating dimensions" in
   Investigations/Cryptography/AdvancedEncryptionStandard/plans/Experimentation.hpp.
   </li>
   <li> In this file we consider:
    <ul>
     <li> "Using the canonical box translation". </li>
    </ul>
   </li>
  </ul>


  \todo Using the canonical translation
  <ul>
   <li> Translation of aes(10,1,1,4):
    <ul>
     <li> We treat S-boxes and additions as boxes. </li>
     <li> The S-box is considered as an 8x1 boolean function,
     translated using the canonical translation; see dualts_fcl in
     ComputerAlgebra/Satisfiability/Lisp/FiniteFunctions/TseitinTranslation.mac.
     </li>
     <li> Additions of arity k are considered bit-wise as (k+1)-bit to 1-bit
     boolean functions; translated using their prime implicates. </li>
     <li> The MixColumns operation is the identity. </li>
     <li> Due to limitations in the translation, clauses occur in this
     translation representing equivalence of variables in the MixColumns;
     See "Remove hard-coding of multiplication by 01 in small-scale MixColumn"
     in
     ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/plans/Translations.hpp.
     </li>
    </ul>
   </li>
   <li> Generating simplest small-scale AES for 10 rounds:
   \verbatim
num_rounds : 10$
num_rows : 1$
num_columns : 1$
exp : 4$
final_round_b : false$
box_tran : aes_ts_box$
seed : 1$
mc_tran : aes_mc_bidirectional$
output_ss_fcl_std(num_rounds, num_columns, num_rows, exp, final_round_b, box_tran, mc_tran)$

shell> cat ssaes_r10_c1_rw1_e4_f0.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG n
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
572 3436 9688 0 9688 573 1
 length count
1 40
2 2720
3 336
9 320
16 20
   \endverbatim
   </li>
   <li> In this translation, we have:
    <ul>
     <li> 10 full rounds (Key Addition, SubBytes, and MixColumns operation).
     </li>
     <li> 20 Sboxes:
      <ul>
       <li> 10 from SubBytes = 1 byte * 10 rounds; </li>
       <li> 10 from key schedule = 1 row * 1 byte * 10 rounds. </li>
      </ul>
     </li>
     <li> 164 additions:
      <ul>
       <li> 80 additions of arity 1:
        <ul>
         <li> 40 from forward MixColumns = 4 bits * 10 rounds; </li>
         <li> 40 from inverse MixColumns = 4 bits * 10 rounds. </li>
        </ul>
       </li>
       <li> 84 additions of arity 2:
        <ul>
         <li> 40 from key additions = 4 bits * 10 rounds; </li>
         <li> 4 from final key addition = 4 bits; </li>
         <li> 40 from the key schedule = 4 bits * 10 rounds. </li>
        </ul>
       </li>
      </ul>
     </li>
     <li> 40 bits for the constant in the key schedule = 4 bits * 10 rounds.
     </li>
    </ul>
   </li>
   <li> The additions are translated by their prime implicates, containing
   2^a clauses where a is the arity of the addition constraint. </li>
   <li> Sbox canonical-translation statistics:
   \verbatim
> ncl_list_fcl(dualts_fcl(ss_sbox_fulldnf_fcl(2,4,ss_polynomial_2_4)));
[[2,128],[9,16],[16,1]]
   \endverbatim
   </li>
   <li> This instance has the following number of clauses of length:
    <ul>
     <li> 1 : 40 = key schedule constant * 1; </li>
     <li> 2 : 2720 = 20 S-boxes * 128 + 80 "additions" (arity 1) * 2; </li>
     <li> 3 : 336 = 84 additions (arity 2) * 4; </li>
     <li> 9 : 320 = 20 S-boxes * 16; </li>
     <li> 16 : 20 = 20 S-boxes * 1. </li>
    </ul>
   </li>
   <li> We can generate a random assignment with the plaintext and
   ciphertext, leaving the key unknown:
   \verbatim
maxima> output_ss_random_pc_pair(seed,num_rounds,num_columns,num_rows,exp,final_round_b);
   \endverbatim
   and the merging the assignment with the translation:
   \verbatim
shell> AppendDimacs-O3-DNDEBUG ssaes_r10_c1_rw1_e4_f0.cnf ssaes_pkpair_r10_c1_rw1_e4_f0_s1.cnf > r10_keyfind.cnf
   \endverbatim
   </li>
   <li> OKsolver solves this with no decisions:
   \verbatim
shell> OKsolver_2002-O3-DNDEBUG r10_keyfind.cnf
c running_time(sec)                     0.0
c number_of_nodes                       1
c number_of_2-reductions                148
c number_of_autarkies                   1
   \endverbatim
   </li>
   <li> However, minisat-2.2.0 and glucose need to branch:
   \verbatim
shell> minisat-2.2.0 r10_keyfind.cnf
<snip>
restarts              : 1
conflicts             : 22             (inf /sec)
decisions             : 41             (0.00 % random) (inf /sec)
propagations          : 6095           (inf /sec)
CPU time              : 0 s

shell> minisat2 r10_keyfind.cnf
<snip>
restarts              : 1
conflicts             : 29             (1450 /sec)
decisions             : 67             (0.00 % random) (3350 /sec)
propagations          : 5397           (269850 /sec)
CPU time              : 0.02 s
shell> glucose r10_keyfind.cnf
c restarts              : 1
c conflicts             : 48             (inf /sec)
c decisions             : 168            (2.38 % random) (inf /sec)
c propagations          : 5738           (inf /sec)
c CPU time              : 0 s
   \endverbatim
   </li>
   <li> We can check we get the right result with:
   \verbatim
shell> OKsolver_2002-O3-DNDEBUG -O r10_keyfind.cnf | grep "^v" | $OKlib/Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/validate_aes_assignment 10 1 1 4 0 && echo "VALID"
VALID
   \endverbatim
   </li>
  </ul>


  \todo DONE Explain sizes
  <ul>
   <li> DONE Here and everywhere else we need precise explanations why we have
   so and so many variables and clauses. </li>
   <li> DONE This is needed in the following files:
    <ul>
     <li> DONE KeyDiscovery/004/1_1_4/10_13.hpp. </li>
     <li> DONE KeyDiscovery/004/1_1_4/20_13.hpp. </li>
     <li> DONE KeyDiscovery/008/1_1_8/20_13.hpp. </li>
     <li> DONE KeyDiscovery/016/2_1_8/1_13.hpp. </li>
     <li> DONE KeyDiscovery/016/2_1_8/10_13.hpp. </li>
     <li> DONE KeyDiscovery/016/2_1_8/14_13.hpp. </li>
     <li> DONE KeyDiscovery/016/2_2_4/1_13.hpp. </li>
     <li> DONE KeyDiscovery/016/2_2_4/2_13.hpp. </li>
     <li> DONE KeyDiscovery/016/2_2_4/4_13.hpp. </li>
     <li> DONE KeyDiscovery/016/2_2_4/20_13.hpp. </li>
     <li> DONE KeyDiscovery/016/2_2_4/20_13.hpp. </li>
     <li> DONE KeyDiscovery/032/2_4_4/1_13.hpp. </li>
     <li> DONE KeyDiscovery/032/2_4_4/3_13.hpp. </li>
     <li> DONE KeyDiscovery/032/2_4_4/5_13.hpp. </li>
     <li> DONE KeyDiscovery/032/4_2_4/1_13.hpp. </li>
     <li> DONE KeyDiscovery/032/4_2_4/2_13.hpp. </li>
     <li> DONE KeyDiscovery/064/1_16_4/4_13.hpp. </li>
     <li> DONE KeyDiscovery/064/1_16_4/5_13.hpp. </li>
     <li> DONE KeyDiscovery/064/4_4_4/1_13.hpp. </li>
     <li> DONE KeyDiscovery/128/1_16_8/2_13.hpp. </li>
     <li> DONE KeyDiscovery/128/4_4_8/0_23_13.hpp. </li>
     <li> DONE KeyDiscovery/128/4_4_8/1_13.hpp. </li>
     <li> DONE KeyDiscovery/004/1_1_4/1_13.hpp. </li>
    </ul>
   </li>
  </ul>


  \todo DONE Update description
  <ul>
   <li> Now the key-addition happens at the beginning of each round. </li>
   <li> This update must be performed here and everywhere else. </li>
  </ul>

*/
