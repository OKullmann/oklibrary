// Matthew Gwynne, 15.2.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/Cryptography/AdvancedEncryptionStandard/plans/KeyDiscovery/004/1_1_4/1_13.hpp
  \brief Investigations into small-scale AES key discovery with 1 row, 1 column and 4-bit field elements for 1 round AES (1+1/3)


  \todo Overview
  <ul>
   <li> We investigate the 1 + 1/3 round small-scale AES with 1 row,
   1 column, using the 4-bit field size. </li>
   <li> We denote this AES instance by aes(1,1,1,4). </li>
   <li> aes(1,1,1,4) takes a 4-bit plaintext and 4-bit key and
   outputs a 4-bit ciphertext. </li>
   <li> For the full specification of this AES instance, see
   "Problem specification" in
   Cryptography/AdvancedEncryptionStandard/plans/KeyDiscovery/004/1_1_4/general.hpp.
   </li>
   <li> For a full list of the possible translations, see
   "Investigating dimensions" in
   Cryptography/AdvancedEncryptionStandard/plans/Experimentation.hpp.
   </li>
   <li> In this file we consider:
    <ul>
     <li> "Using the canonical box translation". </li>
    </ul>
   </li>
  </ul>


  \todo Using the canonical box translation
  <ul>
   <li> Translation of aes(1,1,1,4):
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
   <li> Generating simplest small-scale AES for 1 round
   \verbatim
num_rounds : 1$
num_rows : 1$
num_columns : 1$
exp : 4$
final_round_b : false$
box_tran : aes_ts_box$
seed : 1$
mc_tran : aes_mc_bidirectional$
output_ss_fcl_std(num_rounds, num_columns, num_rows, exp, final_round_b, box_tran, mc_tran)$

shell> cat ssaes_r1_c1_rw1_e4_f0.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG n
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
68 358 1012 0 1012 69 1
 length count
1 4
2 272
3 48
9 32
16 2
   \endverbatim
   </li>
   <li> In this translation, we have:
    <ul>
     <li> 1 full round (Key Addition, SubBytes, and MixColumns operation).
     </li>
     <li> 2 Sboxes:
      <ul>
       <li> 1 from SubBytes = 1 byte * 1 round; </li>
       <li> 1 from key schedule = 1 row * 1 byte * 1 round. </li>
      </ul>
     </li>
     <li> 20 additions:
      <ul>
       <li> 8 additions of arity 1:
        <ul>
         <li> 4 from forward MixColumns = 4 bits * 1 round; </li>
         <li> 4 from inverse MixColumns = 4 bits * 1 rounds. </li>
        </ul>
       </li>
       <li> 12 additions of arity 2:
        <ul>
         <li> 4 from key additions = 4 bits * 1 round; </li>
         <li> 4 from final key addition = 4 bits; </li>
         <li> 4 from the key schedule = 4 bits * 1 rounds. </li>
        </ul>
       </li>
      </ul>
     </li>
     <li> 4 bits for the constant in the key schedule. </li>
    </ul>
   </li>
   <li> The additions are translated by their prime implicates, containing
   2^a clauses where a is the arity of the addition constraint. </li>
   <li> The Sbox is translated by the canonical-translation:
   \verbatim
> ncl_list_fcl(dualts_fcl(ss_sbox_fulldnf_fcl(2,4,ss_polynomial_2_4)));
[[2,128],[9,16],[16,1]]
   \endverbatim
   </li>
   <li> This instance has the following number of clauses of length:
    <ul>
     <li> 1 : 4 = key schedule constant * 1; </li>
     <li> 2 : 272 = 2 S-boxes * 128 + 8 "additions" (arity 1) * 2; </li>
     <li> 3 : 48 = 12 additions (arity 2) * 4; </li>
     <li> 9 : 32 = 2 S-boxes * 16; </li>
     <li> 16 : 2 = 2 S-boxes * 1. </li>
    </ul>
   </li>
   <li> We can generate a random assignment with the plaintext and
   ciphertext, leaving the key unknown:
   \verbatim
maxima> output_ss_random_pc_pair(seed,num_rounds,num_columns,num_rows,exp,final_round_b);
   \endverbatim
   and the merging the assignment with the translation:
   \verbatim
shell> AppendDimacs-O3-DNDEBUG ssaes_r1_c1_rw1_e4_f0.cnf ssaes_pcpair_r1_c1_rw1_e4_f0_s1.cnf > r1_keyfind.cnf
   \endverbatim
   </li>
   <li> OKsolver solves this with no decisions:
   \verbatim
shell> OKsolver_2002-O3-DNDEBUG r1_keyfind.cnf
c running_time(sec)                     0.0
c number_of_nodes                       1
c number_of_2-reductions                19
c number_of_autarkies                   1
c file_name                             r1_keyfind.cnf
   \endverbatim
   </li>
   <li> However, minisat-2.2.0 and glucose need to branch:
   \verbatim
shell> minisat-2.2.0 r1_keyfind.cnf
<snip>
restarts              : 1
conflicts             : 7              (inf /sec)
decisions             : 14             (0.00 % random) (inf /sec)
propagations          : 198            (inf /sec)
CPU time              : 0 s
shell> minisat2 r1_keyfind.cnf
<snip>
restarts              : 1
conflicts             : 13             (inf /sec)
decisions             : 21             (0.00 % random) (inf /sec)
propagations          : 286            (inf /sec)
CPU time              : 0 s
shell> glucose r1_keyfind.cnf
<snip>
c restarts              : 1
c conflicts             : 12             (inf /sec)
c decisions             : 32             (0.00 % random) (inf /sec)
c propagations          : 215            (inf /sec)
c CPU time              : 0 s
   \endverbatim
   </li>
   <li> It seems OKsolver is propagating purely by r_2. Perhaps the OKsolver
   is able to take advantage of the r_1-basedness of the canonical translation
   due to its use of r_2 reductions? </li>
   <li> We can check we get the right result with:
   \verbatim
shell> OKsolver_2002-O3-DNDEBUG -O r1_keyfind.cnf | grep "^v" | $OKlib/Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/validate_aes_assignment 1 1 1 4 0 && echo "VALID"
VALID
   \endverbatim
   </li>
  </ul>

*/
