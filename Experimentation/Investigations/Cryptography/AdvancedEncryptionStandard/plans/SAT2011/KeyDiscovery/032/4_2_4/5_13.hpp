// Matthew Gwynne, 15.2.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/KeyDiscovery/032/4_2_4/5_13.hpp
  \brief Investigations into small scale AES key discovery for 5+1/3 round AES with a 4x2 block and 4-bit field elements


  \todo Problem specification
  <ul>
   <li> In this file, we collect the investigations into translations of
   5 + 1/3 round small scale AES with four columns, two rows,
   using the 4-bit field size. </li>
   <li> The AES encryption scheme we model takes a 32-bit plaintext,
   32-bit key and applies the following operations:
   <ol>
    <li> The round function iterated five times, consisting of (for 
    round 0 <= i < 5):
    <ol>
     <li> Addition of round key i-1 to plaintext. </li>
     <li> Application of SubBytes (Sbox to each byte) operation. </li>
     <li> Application of MixColumns' operation. </li>
    </ol>
    <li> Addition of round key 5, resulting in the ciphertext. </li>
   </ol>
   </li>
  </ul>

  
  \todo Best result
  <ul>
   <li> Solvable in 903.5 by picosat. See "Using the rbase box
   translation". </li>
  </ul>


  \todo Using the rbase box translation
  <ul>
   <li> Generating small scale AES for 5 + 1/3 round:
   \verbatim
num_rounds : 5$
num_columns : 4$
num_rows : 2$
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
   <li> Note we have the following numbers of each type of box in this 
   translation:
   \verbatim
maxima> component_statistics_ss(5,4,2,4,false,aes_mc_bidirectional);
[5,0,40,512,[[x,80],[x+1,80]],10,160,20]
   \endverbatim
   That is, we have:
   <ul>
    <li> Five full rounds (Key Addition, SubBytes, and MixColumns').
    </li>
    <li> No special rounds (Key Addition, SubBytes and ShiftRows). </li>
    <li> 40 Sboxes in the AES round components. This comes from the four 
    columns and two rows of the block with five rounds. </li>
    <li> 512 additions within the round and key additions, coming from:
     <ul>
      <li> Six 32-bit key additions (adding two bits), yielding 
      64 additions of arity two in total. </li>
      <li> Two additions for the MixColumn operation over four columns, 
      applied twice (forward and backward) over five rounds, yielding 512 
      additions of arity two in total. </li>
     </ul>
    </li>
    <li> 80 multiplications each by 02 and 03 across the MixColumns 
    operations. Two 02 multiplications and two 03 from each MixColumn
    matrix mulitiplication, across four columns, applied twice (once
    forward and once in for the inverse MixColumn) across five rounds, giving 
    2 * 4 * 2 * 5 = 80 instances of each multiplication. </li>
    <li> 2 Sboxes in the AES key schedule per round, yielding 10 overall. 
    </li>
    <li> 32 additions in the key schedule per round, yielding 5 * 32 = 160 
    overall. One addition of arity three for each bit in one element in the 
    AES key, and one addition of arity two for all remaining bits in the key 
    schedule (160 - (5*4) = 140). </li>
    <li> 4 bits for the constant in the key schedule in each round, yielding
    5 * 4 = 20 bits overall. </li>
   </ul>
   </li>
   <li> The number of clauses of each length in the translation, computed by:
   \verbatim
maxima> ncl_list_ss(1,4,2,4,false,aes_ts_box,aes_mc_bidirectional);
[[1,20],[2,480],[3,4808],[4,1550]]
maxima> ncl_list_ss_gen(5,4,2,4,ss_mixcolumns_matrix(2,4,2),[[2,'s2],[3,'s3],[4,'s4]],[[x,[[2,'m2_2],[3,'m2_3],[4,'m2_4]]],[x+1,[[2,'m3_2],[3,'m3_4],[4,'m3_4]]]],false,aes_mc_bidirectional);
[[1,20],[2,50*s2+80*m3_2+80*m2_2],[3,50*s3+80*m3_4+80*m2_3+2608],
        [4,50*s4+80*m3_4+80*m2_4+160]]
maxima> ncl_list_fcs(ev_hm(ss_sbox_rbase_cnfs,4));
[[3,12],[4,15]]
maxima> ncl_list_fcs(ev_hm(ss_field_rbase_cnfs,[4,2]));
[[2,6],[3,4]]
maxima> ncl_list_fcs(ev_hm(ss_field_rbase_cnfs,[4,3]));
[[3,16],[4,8]]
   \endverbatim
   are comprised of:
   <ul>
    <li> 20 unit clauses for the 4-bit constants in the Key schedule. </li>
    <li> 480 binary clauses, coming from 50 Sboxes and 80 of each of the two 
    multiplications ((50 * 0) + (80 * 6) + (80 * 0) = 480). </li>
    <li> 4808 ternary clauses, coming from 652 additions of arity two,
    50 Sboxes and 80 of each multiplication
    ((652 * 4) + (50*12) + (80*4) + (80*16) = 4808). </li>
    <li> 1550 clauses of length four, coming from 20 additions of arity three,
    50 Sboxes and 80 multiplications by 03
    ((20 * 8) + (50 * 15) + (80 * 8) = 1550). </li>
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
c 0 iterations
c 0 restarts
c 0 failed literals
c 4596920 conflicts
c 4789770 decisions
c 84 fixed variables
c 177868241 learned literals
c 40.4% deleted literals
c 2809104667 propagations
c 100.0% variables used
c 903.5 seconds in library
c 3.1 megaprops/second
c 1 simplifications
c 371 reductions
c 761.7 MB recycled
c 5.6 MB maximally allocated
c 903.5 seconds total run time
   \endverbatim
   </li>
   <li> All other solvers run in the experiment were unable to solve the 
   instance within 10 hours (cryptominisat, precosat236, precosat-570.1, 
   minisat-2.2.0). </li>
  </ul>

*/
