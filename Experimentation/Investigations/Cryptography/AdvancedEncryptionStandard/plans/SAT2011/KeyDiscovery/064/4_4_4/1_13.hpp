// Matthew Gwynne, 15.2.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file AdvancedEncryptionStandard/plans/SAT2011/KeyDiscovery/064/4_4_4/1_13.hpp
  \brief Investigations into small scale AES key discovery for 1 round AES with a 4x4 plaintext matrix and 4-bit field elements (1+1/3)


  \todo Problem specification
  <ul>
   <li> We investigate the 1 + 1/3 round small scale AES with 4 row,
   4 column, using the 4-bit field size. </li>
   <li> We denote this AES instance by aes(1,4,4,4). </li>
   <li> aes(1,4,4,4) takes a 64-bit plaintext and 64-bit key and
   outputs a 64-bit ciphertext. </li>
   <li> For the full specification of this AES instance, see
   "Problem specification" in
   Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/KeyDiscovery/032/2_4_4/general.hpp.
   </li>
   <li> Note that we consider the canonical CNF translation, as
   this is an example of the "hardest" representation without
   new variables. See "Hardness of boolean function representations"
   in
   Experimentation/Investigations/BooleanFunctions/plans/general.hpp. </li>
  </ul>


  \todo Using the canonical box translation
  <ul>
   <li> Translation of aes(1,4,4,4):
    <ul>
     <li> The MixColumns operation is decomposed into it's field
     multiplications (02 and 03) and addition operations. </li>
     <li> The MixColumns operation is translated by translating both
     the MixColumns operation and it's inverse (it is self-inverse). </li>
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
   <li> Generating small scale AES for 1 round:
   \verbatim
num_rounds : 1$
num_rows : 4$
num_columns : 4$
exp : 4$
final_round_b : false$
box_tran : aes_ts_box$
seed : 1$
mc_tran : aes_mc_bidirectional$
output_ss_fcl_std(num_rounds, num_columns, num_rows, exp, final_round_b, box_tran, mc_tran)$

shell> cat ssaes_r1_c4_rw4_e4_f0.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG n
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
2708 19656 60884 0 60884 2709 1
 length count
1 4
2 14848
3 752
4 32
5 2048
9 1856
16 116
   \endverbatim
   </li>
   <li> In this translation, we have:
    <ul>
     <li> 1 full round (Key Addition, SubBytes, and MixColumns operation).
     </li>
     <li> 20 S-boxes:
      <ul>
       <li> 16 from SubBytes = 16 byte * 1 rounds; </li>
       <li> 4 from key schedule = 1 row * 1 byte * 1 rounds. </li>
      </ul>
     </li>
     <li> 16 multiplications by 02 = 4 rows * 1 multiplication * 4 columns *
     1 round * 1 direction (forward). </li>
     <li> 16 multiplications by 03 = 4 rows * 1 multiplication * 4 columns *
     1 round * 1 directions (forward). </li>
     <li> 16 multiplications by 09 = 4 rows * 1 multiplication * 4 columns *
     1 round * 1 directions (inverse). </li>
     <li> 16 multiplications by 11 = 4 rows * 1 multiplication * 4 columns *
     1 round * 1 directions (inverse). </li>
     <li> 16 multiplications by 13 = 4 rows * 1 multiplication * 4 columns *
     1 round * 1 directions (inverse). </li>
     <li> 16 multiplications by 14 = 4 rows * 1 multiplication * 4 columns *
     1 round * 1 directions (inverse). </li>
     <li> 1088 additions:
      <ul>
       <li> 188 additions of arity 2:
        <ul>
         <li> 64 from key additions = 64 bits * 1 round; </li>
         <li> 64 from final key addition = 64 bits; </li>
         <li> 60 from the key schedule = (64 bits - 4 bits) * 1 round. </li>
        </ul>
       </li>
       <li> 4 additions of arity 3:
        <ul>
         <li> 4 from the key schedule = 4 bits * 1 rounds. </li>
        </ul>
       </li>
       <li> 128 additions of arity 4:
        <ul>
         <li> 64 from forward MixColumns = 4 rows * 4 columns * 4 bits *
         1 round; </li>
         <li> 64 from inverse MixColumns = 4 rows * 4 columns * 4 bits * 1
         round. </li>
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
   <li> There are 116 boxes = 20 S-boxes + 96 multiplications. </li>
   <li> This instance has the following number of clauses of length:
    <ul>
     <li> 1 : 4 = key schedule constant * 1; </li>
     <li> 2 : 14848 = 116 boxes * 128; </li>
     <li> 3 : 752 = 188 additions (arity 2) * 4; </li>
     <li> 4 : 32 = 4 additions (arity 3) * 8; </li>
     <li> 5 : 2048 = 128 additions (arity 4) * 16; </li>
     <li> 9 : 1856 = 116 boxes * 16; </li>
     <li> 16 : 116 = 116 boxes * 1. </li>
    </ul>
   </li>
   <li> Then we can generate a random assignment with the plaintext and
   ciphertext, leaving the key unknown:
   \verbatim
maxima> output_ss_random_pc_pair(seed,num_rounds,num_columns,num_rows,exp,final_round_b);
   \endverbatim
   and the merging the assignment with the translation:
   \verbatim
shell> AppendDimacs-O3-DNDEBUG ssaes_r1_c4_rw4_e4_f0.cnf ssaes_pkpair_r1_c4_rw4_e4_f0_s1.cnf > experiment_r1_k1.cnf
   \endverbatim
   </li>
??? This todo is obviously too long --- the solvers etc. must go into another
todo ???
   <li> minisat-2.2.0 takes only 20s:
   \verbatim
restarts              : 435
conflicts             : 159689         (7684 /sec)
decisions             : 191056         (0.00 % random) (9193 /sec)
propagations          : 80837387       (3889809 /sec)
conflict literals     : 7597323        (51.64 % deleted)
Memory used           : 18.00 MB
CPU time              : 20.7818 s
   \endverbatim
   This has been verified by hand.
   </li>
   <li> glucose (apparently) solves this in a fraction of a second:
   \verbatim
shell> glucose experiment_r1_k1.cnf
c restarts              : 566
c conflicts             : 12127207       (92588235 /sec)
c decisions             : 12923545       (1.40 % random) (98668079 /sec)
c propagations          : 5248749924     (40072911315 /sec)
c CPU time              : 0.13098 s
   \endverbatim
   however, re-running glucose does not yield the same behaviour, and there
   are a very large number of restarts in just 0.13s. Does glucose have a bug
   in it's timing code? Perhaps there is a bug in the experiment run script?
   See "MiniSAT2 based solvers return incorrect times using experiment script"
   in
   Investigations/Cryptography/AdvancedEncryptionStandard/plans/general.hpp.
??? This is trivial to fix --- just apply "time" ???
   </li>
   <li> Other solvers such as cryptominisat take longer (but still only a few
   minutes):
   \verbatim
shell> cryptominisat experiment_r1_k1.cnf
c restarts                 : 569
c conflicts                : 22042524    (350318.21 / sec)
c decisions                : 24091000    (0.14      % random)
c CPU time                 : 62.92       s
   \endverbatim
   </li>
   <li> precosat236 takes a very long time (comparatively):
   \verbatim
shell> precosat236 experiment_r1_k1.cnf
c 12017467 conflicts, 13372168 decisions, 1 random
c 0 iterations, 2 restarts, 17488 skipped
c 95 simplifications, 10 reductions
c prps: 3155560431 propagations, 0.70 megaprops
c 4505.6 seconds, 56 MB max, 1732 MB recycled
   \endverbatim
   </li>
??? where are 2 rounds etc.? where is the general plans-file ???
  </ul>

*/
