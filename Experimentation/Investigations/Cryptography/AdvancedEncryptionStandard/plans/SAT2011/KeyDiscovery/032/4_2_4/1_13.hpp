// Matthew Gwynne, 15.2.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/KeyDiscovery/032/4_2_4/1_13.hpp
  \brief Investigations into small scale AES key discovery for 1+1/3 round AES with a 4x2 plaintext matrix and 4-bit field elements


  \todo Problem specification
  <ul>
   <li> We investigate the 1 + 1/3 round small scale AES with 4 row,
   2 column, using the 4-bit field size. </li>
   <li> We denote this AES instance by aes(1,4,2,4). </li>
   <li> aes(1,4,2,4) takes a 32-bit plaintext and 32-bit key and
   outputs a 32-bit ciphertext. </li>
   <li> For the full specification of this AES instance, see
   "Problem specification" in
   Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/KeyDiscovery/032/4_2_4/general.hpp.
   </li>
   <li> Note that we consider the canonical CNF translation, as
   this is an example of the "hardest" representation without
   new variables. See "Hardness of boolean function representations"
   in
   Experimentation/Investigations/BooleanFunctions/plans/general.hpp. </li>
  </ul>


  \todo Using the canonical box translation
  <ul>
   <li> Translation of aes(1,4,2,4):
    <ul>
     <li> The MixColumns operation is decomposed into it's field
     multiplications (02 and 03) and addition operations. </li>
     <li> The MixColumns operation is translated by translating both
     the MixColumns operation and it's inverse. </li>
     <li> We treat S-boxes, field multiplications and additions as boxes.
     </li>
     <li> The S-box and field multiplications are considered as a 8-bit to
     1-bit boolean functions, translated using the canonical translation;
     see dualts_fcl in
     ComputerAlgebra/Satisfiability/Lisp/FiniteFunctions/TseitinTranslation.mac.
     </li>
     <li> Additions of arity k are considered bit-wise as (k+1)-bit to 1-bit
     boolean functions; translated using their prime implicates. </li>
    </ul>
   </li>
   <li> Generating small scale AES for 1 + 1/3 round:
   \verbatim
num_rounds : 1$
num_rows : 4$
num_columns : 2$
exp : 4$
final_round_b : false$
box_tran : aes_ts_box$
seed : 1$
mc_tran : aes_mc_bidirectional$
output_ss_fcl_std(num_rounds, num_columns, num_rows, exp, final_round_b, box_tran, mc_tran)$

shell> cat ssaes_r1_c2_rw4_e4_f0.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG n
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
1396 10128 31316 0 31316 1397 1
 length count
1 4
2 7680
3 368
4 32
5 1024
9 960
16 60
   \endverbatim
   </li>
   <li> In this translation, we have:
    <ul>
     <li> One full rounds (Key Addition, SubBytes, and MixColumns operation).
     </li>
     <li> 12 S-boxes:
      <ul>
       <li> 8 from SubBytes = 8 byte * 1 rounds; </li>
       <li> 4 from key schedule = 4 row * 1 word * 1 rounds. </li>
      </ul>
     </li>
     <li> 8 multiplications by 02 = 4 rows * 1 multiplication * 2 columns *
     1 round * 1 direction (forward). </li>
     <li> 8 multiplications by 03 = 4 rows * 1 multiplication * 2 columns *
     1 round * 1 directions (forward). </li>
     <li> 8 multiplications by 09 = 4 rows * 1 multiplication * 2 columns *
     1 round * 1 directions (inverse). </li>
     <li> 8 multiplications by 11 = 4 rows * 1 multiplication * 2 columns *
     1 round * 1 directions (inverse). </li>
     <li> 8 multiplications by 13 = 4 rows * 1 multiplication * 2 columns *
     1 round * 1 directions (inverse). </li>
     <li> 8 multiplications by 14 = 4 rows * 1 multiplication * 2 columns *
     1 round * 1 directions (inverse). </li>
     <li> 160 additions:
      <ul>
       <li> 92 additions of arity 2:
        <ul>
         <li> 32 from key additions = 32 bits * 1 round; </li>
         <li> 32 from final key addition = 32 bits; </li>
         <li> 28 from the key schedule = (32 bits - 4 bits) * 1 round. </li>
        </ul>
       </li>
       <li> 4 additions of arity 3:
        <ul>
         <li> 4 from the key schedule = 4 bits * 1 rounds. </li>
        </ul>
       </li>
       <li> 64 additions of arity 4:
        <ul>
         <li> 32 from forward MixColumns = 4 rows * 2 column * 4 bits *
         1 rounds; </li>
         <li> 32 from inverse MixColumns = 4 rows * 2 column * 4 bits * 1
         rounds. </li>
        </ul>
       </li>
      </ul>
     </li>
     <li> 4 bits for the constant in the key schedule = 4 bits * 1 rounds.
     </li>
    </ul>
   </li>
   <li> The number of clauses of each length in the translation, computed by:
   \verbatim
maxima> ncl_list_full_dualts(8,16);
[[2,128],[9,16],[16,1]]
   \endverbatim
   </li>
   <li> This instance has 60 boxes = 12 S-boxes + 48 multiplications.
   </li>
   <li> This instance has the following number of clauses of length:
    <ul>
     <li> 1 : 4 = key schedule constant * 1; </li>
     <li> 2 : 7680 = 60 boxes * 128; </li>
     <li> 3 : 368 = 92 additions (arity 2) * 4; </li>
     <li> 4 : 32 = 4 additions (arity 3) * 8; </li>
     <li> 5 : 1024 = 64 additions (arity 4) * 16; </li>
     <li> 9 : 960 = 60 boxes * 16; </li>
     <li> 16 : 60 = 60 boxes * 1. </li>
    </ul>
   </li>
   <li> Then we can generate a random assignment with the plaintext and
   ciphertext, leaving the key unknown:
   \verbatim
maxima> output_ss_random_pc_pair(seed,num_rounds,num_columns,num_rows,exp,final_round_b);
   \endverbatim
   and the merging the assignment with the translation:
   \verbatim
shell> AppendDimacs-O3-DNDEBUG ssaes_r1_c2_rw4_e4_f0.cnf ssaes_pkpair_r1_c2_rw4_e4_f0_s1.cnf > r1_keyfind.cnf
   \endverbatim
   </li>
   <li> OKsolver_2002:
   \verbatim
shell> OKsolver_2002-O3-DNDEBUG r1_keyfind.cnf
c running_time(sec)                     42.8
c number_of_nodes                       2915
c number_of_2-reductions                25478
   \endverbatim
   </li>
   <li> minisat-2.2.0 and glucose:
   \verbatim
shell> minisat-2.2.0 r1_keyfind.cnf
restarts              : 126
conflicts             : 38174          (11967 /sec)
decisions             : 41318          (0.00 % random) (12952 /sec)
propagations          : 15523483       (4866296 /sec)
CPU time              : 3.19 s

shell> minisat2 r1_keyfind.cnf
<snip>
restarts              : 14
conflicts             : 43604          (1401 /sec)
decisions             : 47364          (1.34 % random) (1522 /sec)
propagations          : 17037471       (547477 /sec)
CPU time              : 31.12 s
shell> glucose r1_keyfind.cnf
<snip>
c restarts              : 8
c conflicts             : 16554          (13035 /sec)
c decisions             : 21834          (1.56 % random) (17192 /sec)
c propagations          : 3407020        (2682693 /sec)
c CPU time              : 1.27 s
   \endverbatim
   </li>
   <li> We can check we get the right result with:
   \verbatim
shell> OKsolver_2002-O3-DNDEBUG -O r1_keyfind.cnf | grep "^v" | $OKlib/Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/validate_aes_assignment 1 2 4 4 0 && echo "VALID"
VALID
   \endverbatim
   </li>
  </ul>


  \bug DONE (Corrected dimensions and specification for each file; added
  todo on updating translation functions)
  False specification of sizes
  <ul>
   <li> The directory is "2_4_4", and a "2x4 block" is mentioned, while
   below it says "two columns, four rows". </li>
   <li> What is a "block"? This likely should be a matrix. </li>
   <li> The dimensions of a matrix is specified as first the number of rows,
   then the number of columns. So we have an inconsistency. </li>
   <li> See "Order of small scale matrix dimensions" in
   ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/plans/Translations.hpp
   for a todo on updating the translation function parameter order to
   correctly reflect the standard ordering for matrix dimensions.</li>
  </ul>

*/
