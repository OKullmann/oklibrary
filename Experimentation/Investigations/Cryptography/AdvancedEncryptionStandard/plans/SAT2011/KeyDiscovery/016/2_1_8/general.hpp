// Matthew Gwynne, 20.5.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/KeyDiscovery/016/2_1_8/general.hpp
  \brief Investigations into small-scale AES key discovery for AES with a 2x1 plaintext matrix and 8-bit field elements


  \todo Problem specification
  <ul>
  <li> We consider the small-scale AES with 2 row, 1 column, using the 8-bit
   field size for rounds 1 to 20. </li>
   <li> We denote this AES instance by aes(r,2,1,8) for r in 1,...,20. </li>
   <li> We investigate translations of the key discovery problem for
   aes(r,2,1,8) into SAT. </li>
   <li> aes(r,2,1,8) takes a 16-bit plaintext and 16-bit key and outputs a
   16-bit ciphertext. </li>
   <li> aes(r,2,1,8) applies the following operations:
    <ol>
     <li> Key schedule which takes the key and generates r+1 16-bit round
     keys. </li>
     <li> Application of the following operation (the "round") r times:
      <ol>
       <li> Addition of round key n-1. </li>
       <li> Application of Sbox operation to each byte. </li>
       <li> Application of the MixColumns operation. </li>
      </ol>
     </li>
     <li> Addition of round key r+1. </li>
     <li> The result of the last round key addition is the ciphertext. </li>
    </ol>
   </li>
   <li> Round key 0 is the input key. </li>
   <li> The key schedule computes the round key i, K_(i,j), from round key
   i-1, K_(i-1), by:
   \verbatim
K_(i,j) := S-box(K_(i-1,j)) + C_i
   \endverbatim
   where
    <ul>
     <li> C_i is the round constant for round i; </li>
     <li> K_(i,j) is the j-th 4-bit word of the i-th round-key. </li>
    </ul>
   </li>
   <li> The S-box is a permutation from {0,1}^8 to {0,1}^8 which we consider
   as either:
    <ul>
     <li> a 16x1 boolean function; see ss_sbox_bf in
     ComputerAlgebra/Cryptology/Lisp/CryptoSystems/Rijndael/AdvancedEncryptionStandard.mac;
     </li>
     <li> 4 8x1 boolean functions. </li>
    </ul>
   </li>
   <li> The MixColumns operation is a permutation from ({0,1}^8)^2 to
   ({0,1}^8)^2, which we consider to be defined as:
   \verbatim
MixColumns(I_1) := Mul02(I_1) + Mul03(I_2)
MixColumns(I_2) := Mul03(I_1) + Mul02(I_2)
   \endverbatim
   where
    <ul>
     <li> I_i is the i-th 8-bit word in the input; </li>
     <li> Mul02 is a permutation over {0,1}^8 representing multiplication
     by 02 in the Rijndael byte field; </li>
     <li> Mul03 is a permutation over {0,1}^8 representing multiplication
     by 03 in the Rijndael byte field. </li>
    </ul>
   </li>
   <li> The decompositions and translations are listed in "Investigating
   dimensions" in
   Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/Experimentation.hpp.
   </li>
   <li> The plaintext and ciphertext variables are then set, and the SAT
   SAT solver is run on this instance to deduce the key variables. </li>
  </ul>


  \todo Comparisons
  <ul>
   <li> Comparing the different translations. </li>
   <li> For 10 round instances, the canonical translation
   seems to perform best by a factor of 20 or more. </li>
   <li> See:
    <ul>
     <li> 'The canonical box translation'. </li>
     <li> 'The "minimum" box translation'. </li>
    </ul>
   </li>
  </ul>


  \todo Translations
  <ul>
   <li> Standard box translations:
    <ul>
     <li> Translating aes(r,2,1,8):
      <ul>
       <li> The MixColumns operation is decomposed into its field
       multiplications (02 and 03) and addition operations. </li>
       <li> The MixColumns operation is translated by translating both
       the MixColumns operation and its inverse (it is self-inverse). </li>
       <li> We treat S-boxes, field multiplications and additions as boxes.
       </li>
       <li> The S-box and field multiplications are considered as a 16x1
       boolean functions. </li>
       <li> Additions of arity k are considered bit-wise as (k+1)-bit to 1-bit
       boolean functions; translated using their prime implicates. </li>
      </ul>
     </li>
     <li> In this translation, for r rounds, we have:
      <ul>
       <li> r full rounds (Key Addition, SubBytes, and MixColumns operation).
       </li>
       <li> 4*r Sboxes:
        <ul>
         <li> 2*r from SubBytes = 2 byte * r rounds; </li>
         <li> 2*r from key schedule = 2 row * 1 byte * r rounds. </li>
        </ul>
       </li>
       <li> 4*r multiplications by 02: 2 rows * 1 multiplication * 1 columns *
       r rounds * 2 directions (forward + inverse). </li>
       <li> 4*r multiplications by 03: 2 rows * 1 multiplication * 1 columns *
       r rounds * 2 directions (forward + inverse). </li>
       <li> 56*r + 16 additions of arity 2:
        <ul>
         <li> 16*r from key additions = 16 bits * r round; </li>
         <li> 16 from final key addition = 16 bits; </li>
         <li> 8*r from the key schedule = 1 rows * 8 bits * r round. </li>
         <li> 16*r from forward MixColumns = 2 rows * 1 column * 8 bits *
         r rounds; </li>
         <li> 16*r from inverse MixColumns = 2 rows * 1 column * 8 bits * r
         rounds. </li>
        </ul>
       </li>
       <li> 8*r bits for the constant in the key schedule = 8 bits * r rounds.
       </li>
      </ul>
     </li>
     <li> Note that as this variant has only one column, the key schedule
     applies Sbox(K_i) + C rather than Sbox(K_i) + K_j + C where K_i and
     K_j are key words from the previous round key. </li>
     <li> The canonical box translation:
      <ul>
       <li> See "The canonical box translation". </li>
       <li> The Sboxes and multiplications boxes are translated using the
       canonical translation, which has the following number of clauses of
       each length:
       \verbatim
maxima> ncl_list_full_dualts(16,256);
[[2,4096],[17,256],[256,1]]
       \endverbatim
       </li>
       <li> This instances has 120 boxes = 40 S-boxes + 80 multiplications.
       </li>
       <li> This instance has the following number of clauses of length:
        <ul>
         <li> 1 : 8*r = key schedule constant * 1; </li>
         <li> 2 : 49152*r = 12*r boxes * 4096; </li>
         <li> 3 : 224*r + 64 = 56*r+16 additions (arity 2) * 4; </li>
         <li> 17 : 3072*r = 120 boxes * 256; </li>
         <li> 256 : 12*r = 120 boxes * 1. </li>
        </ul>
       </li>
      </ul>
     </li>
     <li> The 1-base box translation:
      <ul>
       <li> See "The 1-base box translation". </li>
       <li> Generating a 1-base for the S-box from
       Cryptography/AdvancedEncryptionStandard/plans/SAT2011/Representations/Sbox_8.hpp:
       \verbatim
maxima> output_rijnsbox_fullcnf_stdname();
shell> QuineMcCluskey-n16-O3-DNDEBUG AES_Sbox_full.cnf > AES_Sbox_pi.cnf
shell> RandomShuffleDimacs-O3-DNDEBUG 103 < AES_Sbox_pi.cnf | SortByClauseLength-O3-DNDEBUG > AES_Sbox_sortedpi.cnf
shell> RUcpGen-O3-DNDEBUG AES_Sbox_sortedpi.cnf > AES_Sbox_gen.cnf
shell> RandomShuffleDimacs-O3-DNDEBUG 1 < AES_Sbox_gen.cnf | SortByClauseLengthDescending-O3-DNDEBUG | RUcpBase-O3-DNDEBUG > AES_Sbox_base.cnf
shell> cat AES_Sbox_base.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG n
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
16 4398 30108 0 30108 0 1
 length count
5 1
6 1187
7 2703
8 503
9 4
       \endverbatim
       </li>
       <li> Generating a 1-base for the multiplication by 02 and 03 from
       Cryptography/AdvancedEncryptionStandard/plans/SAT2011/Representations/Mul_8_2.hpp
       and
       Cryptography/AdvancedEncryptionStandard/plans/SAT2011/Representations/Mul_8_3.hpp:
       \verbatim
maxima> output_rijn_mult_fullcnf_stdname(2);
shell> QuineMcCluskey-n16-O3-DNDEBUG AES_byte_field_mul_full_2.cnf > AES_byte_field_mul_2_pi.cnf
shell> RandomShuffleDimacs-O3-DNDEBUG 1 < AES_byte_field_mul_2_pi.cnf | SortByClauseLength-O3-DNDEBUG > AES_byte_field_mul_2_sortedpi.cnf
shell> RUcpGen-O3-DNDEBUG AES_byte_field_mul_2_sortedpi.cnf > AES_byte_field_mul_2_gen.cnf
shell> RandomShuffleDimacs-O3-DNDEBUG 1 < AES_byte_field_mul_2_gen.cnf | SortByClauseLengthDescending-O3-DNDEBUG | RUcpBase-O3-DNDEBUG > AES_byte_field_mul_2_base.cnf
shell> cat AES_byte_field_mul_2_base.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG n
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
16 22 56 0 56 0 1
 length count
2 10
3 12
maxima> output_rijn_mult_fullcnf_stdname(3);
shell> QuineMcCluskey-n16-O3-DNDEBUG AES_byte_field_mul_full_3.cnf > AES_byte_field_mul_3_pi.cnf
shell> RandomShuffleDimacs-O3-DNDEBUG 1 < AES_byte_field_mul_3_pi.cnf | SortByClauseLength-O3-DNDEBUG > AES_byte_field_mul_3_sortedpi.cnf
shell> RUcpGen-O3-DNDEBUG AES_byte_field_mul_3_sortedpi.cnf > AES_byte_field_mul_3_gen.cnf
shell> RandomShuffleDimacs-O3-DNDEBUG 1 < AES_byte_field_mul_3_gen.cnf | SortByClauseLengthDescending-O3-DNDEBUG | RUcpBase-O3-DNDEBUG > AES_byte_field_mul_3_base.cnf
shell> cat AES_byte_field_mul_3_base.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG n
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
16 80 328 0 328 0 1
 length count
3 24
4 24
5 32
       \endverbatim
       </li>
       <li> The Sboxes and multiplications use 1-base translations,
       which have the following number of clauses of each length:
       \verbatim
maxima> ncl_list_fcs(ev_hm(ss_sbox_rbase_cnfs,8));
[[5,1],[6,1187],[7,2703],[8,503],[9,4]]
maxima> ncl_list_fcs(ev_hm(ss_field_rbase_cnfs,[8,2]))
[[2,10],[3,12]]
maxima> ncl_list_fcs(ev_hm(ss_field_rbase_cnfs,[8,3]))
[[3,24],[4,24],[5,32]]
       \endverbatim
       </li>
       <li> This instance has the following number of clauses of length:
        <ul>
         <li> 1 : 8*r = key schedule constant * 1; </li>
         <li> 2 : 4*r = 4*r multiplications by 02 * 10; </li>
         <li> 3 : 368*r + 64 = 4*r multiplications by 02 * 12 + 4*r
         multiplications by 03 * 24 + 56*r+16 additions (arity 2) * 4; </li>
         <li> 4 : 96*r = 4*r multiplications by 03 * 24; </li>
         <li> 5 : 132 = 4*r S-boxes * 1 + 4 multiplications by 03 * 32; </li>
         <li> 6 : 4748*r = 4*r S-boxes * 1187; </li>
         <li> 7 : 10812*r = 4*r S-boxes * 2703; </li>
         <li> 8 : 2012*r = 4*r S-boxes * 503; </li>
         <li> 9 : 16*r = 4*r S-boxes * 4. </li>
        </ul>
       </li>
      </ul>
     </li>
     <li> The "minimum" box translation:
      <ul>
       <li> See 'The "minimum"  box translation. </li>
       <li> The "minimum" representations for the S-box and multiplications
       by 02 and 03 from
       Cryptography/AdvancedEncryptionStandard/plans/SAT2011/Representations/Sbox_8.hpp,
       Cryptography/AdvancedEncryptionStandard/plans/SAT2011/Representations/Mul_8_2.hpp
       and
       Cryptography/AdvancedEncryptionStandard/plans/SAT2011/Representations/Mul_8_3.hpp :
       \verbatim
/* Multiplication by 02: */
maxima> FieldMul2CNF : [{1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16},{{-9,2},{-2,9},{-10,3},{-3,10},{-11,4},{-4,11},{-12,-5,-1},{-12,1,5},{-5,1,12},{-1,5,12},{-13,-6,-1},{-1,6,13},{-14,7},{-7,14},{-15,1,8},{-8,1,15},{-16,-15,-8},{-16,8,15},{-13,6,16},{-6,13,16}}]$
set_hm(ss_field_cnfs,[8,2], FieldMul2CNF));
/* Multiplication by 03: */
maxima> FieldMul3CNF :
 [[1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16],
  [{-9,-2,-1},{-2,1,9},{-10,2,3},{-10,-9,-3,1},{-10,-3,-1,9},{-3,2,10},{-9,1,3,10},{-1,3,9,10},{-11,-4,-3},{-11,3,4},{-4,3,11},{-3,4,11},{-12,-5,-4,1},{-12,-4,-1,5},{-5,1,4,12},{-1,4,5,12},{-13,-5,-1,6},{-13,1,5,6},{-13,-12,-6,4},{-13,-6,-4,12},{-6,-5,-1,13},{-6,1,5,13},
   {-12,4,6,13},{-4,6,12,13},{-14,-7,-6},{-14,6,7},{-7,6,14},{-6,7,14},{-16,-8,-1},{-16,1,8},{-16,-15,-7},{-16,7,15},{-8,1,16},{-1,8,16},{-15,7,16},{-7,15,16}]]$
set_hm(ss_field_cnfs,[8,2], FieldMul3CNF));
/* Sbox: */
maxima> output_rijnsbox_fullcnf_stdname();
shell> QuineMcCluskeySubsumptionHypergraph-n16-O3-DNDEBUG AES_Sbox_full.cnf > AES_Sbox_shg.cnf
shell> cat AES_Sbox_shg.cnf | MinOnes2WeightedMaxSAT-O3-DNDEBUG > AES_Sbox_shg.wcnf
shell> ubcsat-okl  -alg gsat -w -runs 100 -cutoff 40000000 -wtarget 294 -solve 1 -seed 3213901809 -i AES_Sbox_shg.wcnf -r model AES_Sbox_s294.ass;
shell> cat  AES_Sbox_full.cnf_primes | FilterDimacs AES_Sbox_s294.ass > AES_Sbox_s294.cnf
maxima> SboxMinCNF : read_fcl_f("AES_Sbox_s294.cnf");
maxima> set_hm(ss_sbox_cnfs,8, SboxMinCNF));
       \endverbatim
       </li>
       <li> The Sboxes and multiplications use the "minimum" translations,
       which have the following number of clauses of each length:
       \verbatim
maxima> ncl_list_fcs(ev_hm(ss_sbox_cnfs,8));
[[6,143],[7,127],[8,24]]
maxima> ncl_list_fcs(ev_hm(ss_field_cnfs,[8,2]))
[[2,8],[3,12]]
maxima> ncl_list_fcs(ev_hm(ss_field_cnfs,[8,3]))
[[3,20],[4,16]]
       \endverbatim
       </li>
       <li> This instance has the following number of clauses of length:
        <ul>
         <li> 1 : 8*r = key schedule constant * 1; </li>
         <li> 2 : 32*r = 4*r multiplications by 02 * 8; </li>
         <li> 3 : 352*r + 64 = 4*r multiplications by 02 * 12 + 4*r
         multiplications by 03 * 20 + 56*r+16 additions (arity 2) * 4; </li>
         <li> 4 : 64*r = 4*r multiplications by 04 * 16; </li>
         <li> 6 : 572*r = 4*r S-boxes * 143; </li>
         <li> 7 : 508*r = 4*r S-boxes * 127; </li>
         <li> 8 : 96*r = 4*r S-boxes * 24. </li>
        </ul>
       </li>
      </ul>
     </li>
    </ul>
   </li>
  </ul>


  \todo The canonical box translation
  <ul>
   <li> Over 20 rounds, with 20 random plaintext-ciphertext pairs per round:
    <ul>
     <li> Generating the instances:
     \verbatim
shell> oklib --maxima
oklib_load_all()$
num_rows : 2$
num_columns : 1$
exp : 8$
final_round_b : false$
box_tran : aes_ts_box$
mc_tran : aes_mc_bidirectional$
oklib_monitor : true$
for rounds : 1 thru 20 do
  output_ss_fcl_std(rounds, num_columns, num_rows, exp, final_round_b, box_tran, mc_tran)$
     \endverbatim
     </li>
     <li> Generating 20 random assignments for the plaintext and ciphertext, leaving
     the key unknown:
     \verbatim
maxima> for seed : 1 thru 20 do output_ss_random_pc_pair(seed,rounds,num_columns,num_rows,exp,final_round_b);
     \endverbatim
     </li>
     <li> Running minisat-2.2.0:
     \verbatim
shell> col=1; row=2; e=8; r=1;
for k in $(seq 1 20); do
    echo "Key ${k} Round ${r}";
    AppendDimacs-O3-DNDEBUG ssaes_r${r}_c${col}_rw${row}_e${e}_f0.cnf ssaes_pcpair_r${r}_c${col}_rw${row}_e${e}_f0_s${k}.cnf > r${r}_k${k}.cnf;
    (time minisat-2.2.0 r${r}_k${k}.cnf) > minisat_r${r}_k${k}.result 2>&1;
done;
shell> echo "n  c  t  sat  cfs dec rts r1 mem ptime stime cfl r k" > minisat_results;
for r in $(seq 1 20); do
  for k in $(seq 1 20); do
    cat minisat_r${r}_k${k}.result | awk -f $OKlib/Experimentation/ExperimentSystem/SolverMonitoring/ExtractMinisat.awk | awk " { print \$0 \"  $r  $k\" }";
  done;
done >> minisat_results;
     \endverbatim
     yields:
     \verbatim
shell> oklib --R
E = read.table("minisat_results", header=TRUE)
EM = aggregate(E, by=list(r=E$r), FUN=mean)
options(width=1000)
EM
 r     n       c          t   bash_t      cfs      dec    rts           r1    mem  ptime  stime        cfl
 1  3264   52508  0.1249306   0.0650   458.95   6076.0   4.00     423525.7  11.00 0.0215 0.0400   124822.4
 2  6480  104952 33.0200854  75.7310 67356.80 242387.5 192.40   97916518.9 139.60 0.0410 0.0800 21278854.4
 3  9696  157396 26.9537606  41.2970 53390.60 248458.1 157.80  109114823.9 106.95 0.0660 0.1200 17142892.6
 4 12912  209840 38.6374940  73.2580 64889.50 239838.5 186.90  235101310.2 130.65 0.0870 0.1600 18468994.6
 5 16128  262284 45.8912920 102.0715 72181.40 278187.7 208.95  345243337.9 159.45 0.1095 0.2000 21840963.2
 6 19344  314728 47.9305295  90.4860 65694.70 255653.8 191.80  391090908.6 134.80 0.1345 0.2400 18871607.4
 7 22560  367172 35.7736675  69.3640 43421.75 181484.0 135.20  308959433.1 106.95 0.1585 0.2800 13469693.6
 8 25776  419616 44.3857652  82.4390 52619.20 239547.9 155.60  415826427.1 127.40 0.1790 0.3200 14776510.8
 9 28992  472060 49.4745025 102.6980 57760.20 256061.6 170.55  478385590.6 146.30 0.2000 0.3600 18764325.2
10 32208  524504 55.7512070 145.2080 74473.55 328906.0 207.35  692318870.9 176.85 0.2260 0.4015 23922493.2
11 35424  576948 59.6738650 219.1985 88613.30 325327.2 250.20  988175081.3 203.60 0.2430 0.4440 27092520.0
12 38640  629392 54.0172515 134.8720 58642.20 239197.1 175.30  712113529.7 137.00 0.2725 0.4810 15833605.6
13 41856  681836 58.7378500 240.9780 84993.45 334794.7 238.25 1178269114.9 201.85 0.2940 0.5240 24913310.5
14 45072  734280 56.2418225 216.8590 73503.25 299043.2 215.90 1022098704.8 194.25 0.3150 0.5675 22868749.3
15 48288  786724 55.0052705 196.6845 65558.30 308100.1 189.20 1012224859.0 182.30 0.3385 0.6085 21748863.7
16 51504  839168 62.2242725 237.2150 75545.25 286732.1 219.90 1334688295.2 181.20 0.3640 0.6480 21172958.2
17 54720  891612 58.9882610 226.4960 71584.25 326660.5 198.10 1145534955.9 201.45 0.3875 0.6900 22636613.2
18 57936  944056 61.3998400 235.7845 73793.80 311720.4 209.20 1297866385.2 193.85 0.4065 0.7295 22467222.5
19 61152  996500 63.3142550 187.8575 56659.20 263318.7 165.95 1129640755.5 172.10 0.4295 0.7700 16518592.8
20 64368 1048944 56.0893500 268.0105 74141.35 349173.7 206.25 1358448327.8 225.15 0.4555 0.8105 22652719.3
     \endverbatim
     where bash_t (time from bash time command) was extracted by:
     \verbatim
shell> echo "r bash_t" > minisat_bash_times;
for r in $(seq 1 20); do
  for k in $(seq 1 20); do
    cat minisat_r${r}_k${k}.result | awk "/^real/  { split(\$2,a,\"m\"); split(a[2],b,\"s\"); print \"${r} \" (a[1] * 60) + b[1]; }" >> minisat_bash_times;
  done;
done

shell> oklib --R
E = read.table("minisat_bash_times", header=TRUE)
EM = aggregate(E, by=list(r=E$r), FUN=mean)
EM
     \endverbatim
     </li>
     <li> Consider:
     \verbatim
R> plot(EM$cfs)
R> plot(EM$bash_t)
     \endverbatim
     The minisat-2.2.0 times and number of conflicts appear to be linear
     in the number of rounds!
     </li>
     <li> Running OKsolver_2002:
     \verbatim
shell> col=1; row=2; e=8; r=1;
for k in $(seq 1 20); do
    echo "Key ${k} Round ${r}";
    AppendDimacs-O3-DNDEBUG ssaes_r${r}_c${col}_rw${row}_e${e}_f0.cnf ssaes_pcpair_r${r}_c${col}_rw${row}_e${e}_f0_s${k}.cnf > r${r}_k${k}.cnf;
    (time OKsolver_2002-O3-DNDEBUG r${r}_k${k}.cnf) > minisat_r${r}_k${k}.result 2>&1;
done;
     \endverbatim
     is still running after 5 hours on a single instance.
     </li>
    </ul>
   </li>
   <li> See:
    <ul>
     <li> Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/KeyDiscovery/016/2_1_8/1_13.hpp. </li>
     <li> Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/KeyDiscovery/016/2_1_8/10_13.hpp. </li>
     <li> Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/KeyDiscovery/016/2_1_8/14_13.hpp. </li>
    </ul>
   </li>
  </ul>


  \todo The 1-base box translation
  <ul>
   <li> Over 20 rounds, with 20 random plaintext-ciphertext pairs per round:
    <ul>
     <li> Generating the instances:
     \verbatim
shell> oklib --maxima
oklib_load_all()$
num_rows : 2$
num_columns : 1$
exp : 8$
final_round_b : false$
box_tran : aes_rbase_box$
mc_tran : aes_mc_bidirectional$
oklib_monitor : true$
set_hm(ss_sbox_rbase_cnfs,8,read_fcl_f("AES_Sbox_base.cnf"))$
set_hm(ss_field_rbase_cnfs,[8,2],read_fcl_f("AES_byte_field_mul_2_base.cnf"))$
set_hm(ss_field_rbase_cnfs,[8,3],read_fcl_f("AES_byte_field_mul_3_base.cnf"))$
for rounds : 1 thru 20 do
  output_ss_fcl_std(rounds, num_columns, num_rows, exp, final_round_b, box_tran, mc_tran)$
     \endverbatim
     </li>
     <li> Generating 20 random assignments for the plaintext and ciphertext, leaving
     the key unknown:
     \verbatim
maxima> for seed : 1 thru 20 do output_ss_random_pc_pair(seed,rounds,num_columns,num_rows,exp,final_round_b);
     \endverbatim
     </li>
     <li> Running minisat-2.2.0:
     \verbatim
shell> col=1; row=2; e=8; r=1;
for k in $(seq 1 20); do
    echo "Key ${k} Round ${r}";
    AppendDimacs-O3-DNDEBUG ssaes_r${r}_c${col}_rw${row}_e${e}_f0.cnf ssaes_pcpair_r${r}_c${col}_rw${row}_e${e}_f0_s${k}.cnf > r${r}_k${k}.cnf;
    (time minisat-2.2.0 r${r}_k${k}.cnf) > minisat_r${r}_k${k}.result 2>&1;
done;
shell> echo "n  c  t  sat  cfs dec rts r1 mem ptime stime cfl r k" > minisat_results;
for r in $(seq 1 20); do
  for k in $(seq 1 20); do
    cat minisat_r${r}_k${k}.result | awk -f $OKlib/Experimentation/ExperimentSystem/SolverMonitoring/ExtractMinisat.awk | awk " { print \$0 \"  $r  $k\" }";
  done;
done >> minisat_results;
     \endverbatim
     yields:
     \verbatim
shell> oklib --R
E = read.table("minisat_results", header=TRUE)
EM = aggregate(E, by=list(r=E$r), FUN=mean)
options(width=1000)
EM
 r    n      c          t   bash_t       cfs       dec    rts          r1  ptime  stime       cfl
 1  192  18272  0.5593143   0.5285    378.40    407.10   3.55    17333.15 0.0110 0.5115    2898.3
 2  336  36480  9.6829740   9.6240  38637.05  41081.25 125.30  3096961.95 0.0205 0.9600  635664.8
 3  480  54688 15.3220155  15.2920  37360.50  39724.25 118.35  5821360.20 0.0350 1.4000  569780.4
 4  624  72896 24.4987730  24.8370  45409.70  49792.90 138.35  8149119.90 0.0445 1.8700  817007.7
 5  768  91104 28.8751670  30.2230  40354.70  44488.95 130.10  9966599.80 0.0570 2.3100  644105.6
 6  912 109312 41.2160245  46.9385  56145.40  62684.00 170.25 14110385.50 0.0640 2.7595  888331.9
 7 1056 127520 40.4946880  46.7475  53666.90  61842.45 166.15 13175222.50 0.0785 3.1975  853229.2
 8 1200 145728 50.8320355 102.3470  84242.95  96100.55 239.45 24387576.30 0.0915 3.6640 1316421.4
 9 1344 163936 53.8861850 117.9040  87504.90 101635.25 251.80 27133294.20 0.1015 4.1090 1399346.6
10 1488 182144 59.9203645 143.2785  97662.45 114138.40 273.80 30732365.70 0.1130 4.5550 1627517.4
11 1632 200352 55.4241445 100.5410  82608.05 100458.20 239.20 21793714.35 0.1245 4.9960 1477655.9
12 1776 218560 62.1145400 162.2785 112890.40 135246.05 309.75 33442510.10 0.1345 5.4655 2002968.6
13 1920 236768 63.5071750 219.8780 128707.95 154327.85 350.65 44485694.45 0.1510 5.9085 2238872.5
14 2064 254976 65.4362100 277.6545 149758.45 179119.00 397.55 54358522.10 0.1570 6.3560 2664466.5
15 2208 273184 64.8558750 278.9755 154428.90 186527.30 408.50 54256192.65 0.1710 6.7920 2824605.1
16 2352 291392 62.8937150 244.3250 144208.05 177472.55 386.45 48356482.55 0.1835 7.2620 2758481.2
17 2496 309600 65.3945950 283.3340 163903.60 202336.40 422.85 54694309.85 0.1970 7.7045 3221092.9
18 2640 327808 65.4254900 350.5680 180847.40 222289.35 461.60 66776355.60 0.2075 8.1530 3461162.0
19 2784 346016 62.8704800 350.7520 182180.55 226259.50 453.00 66744064.25 0.2190 8.5940 3528544.8
20 2928 364224 65.4332800 365.6620 194371.05 242715.75 491.40 69723963.15 0.2275 9.0660 3904121.5
     \endverbatim
     where bash_t (time from bash time command) was extracted by:
     \verbatim
shell> echo "r bash_t" > minisat_bash_times;
for r in $(seq 1 20); do
  for k in $(seq 1 20); do
    cat minisat_r${r}_k${k}.result | awk "/^real/  { split(\$2,a,\"m\"); split(a[2],b,\"s\"); print \"${r} \" (a[1] * 60) + b[1]; }" >> minisat_bash_times;
  done;
done
shell> oklib --R
E = read.table("minisat_bash_times", header=TRUE)
EM = aggregate(E, by=list(r=E$r), FUN=mean)
EM
     \endverbatim
     </li>
     <li> Consider:
     \verbatim
R> plot(E$cfs)
R> plot(E$bash_t)
     \endverbatim
     The minisat-2.2.0 times and number of conflicts appear to be linear
     in the number of rounds!
     </li>
     <li> Running OKsolver_2002:
     \verbatim
shell> col=1; row=2; e=8; r=1;
for k in $(seq 1 20); do
    echo "Key ${k} Round ${r}";
    AppendDimacs-O3-DNDEBUG ssaes_r${r}_c${col}_rw${row}_e${e}_f0.cnf ssaes_pcpair_r${r}_c${col}_rw${row}_e${e}_f0_s${k}.cnf > r${r}_k${k}.cnf;
    (time OKsolver_2002-O3-DNDEBUG r${r}_k${k}.cnf) > minisat_r${r}_k${k}.result 2>&1;
done;
     \endverbatim
     is still running after 5 hours on a single instance.
     </li>
    </ul>
   </li>
   <li> See:
    <ul>
     <li> Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/KeyDiscovery/016/2_1_8/1_13.hpp. </li>
     <li> Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/KeyDiscovery/016/2_1_8/10_13.hpp. </li>
     <li> Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/KeyDiscovery/016/2_1_8/14_13.hpp. </li>
    </ul>
   </li>
  </ul>


  \todo The "minimum" box translation
  <ul>
   <li> Over 7 rounds, with 20 random plaintext-ciphertext pairs per round:
    <ul>
     <li> Generating the instances:
     \verbatim
shell> oklib --maxima
oklib_load_all()$
num_rows : 2$
num_columns : 1$
exp : 8$
final_round_b : false$
box_tran : aes_small_box$
mc_tran : aes_mc_bidirectional$
oklib_monitor : true$
for rounds : 1 thru 20 do
  output_ss_fcl_std(rounds, num_columns, num_rows, exp, final_round_b, box_tran, mc_tran)$
     \endverbatim
     </li>
     <li> Generating 20 random assignments for the plaintext and ciphertext, leaving
     the key unknown:
     \verbatim
maxima> for seed : 1 thru 20 do output_ss_random_pc_pair(seed,rounds,num_columns,num_rows,exp,final_round_b);
     \endverbatim
     </li>
     <li> Running minisat-2.2.0:
     \verbatim
shell> col=1; row=2; e=8; r=1;
for k in $(seq 1 20); do
    echo "Key ${k} Round ${r}";
    AppendDimacs-O3-DNDEBUG ssaes_r${r}_c${col}_rw${row}_e${e}_f0.cnf ssaes_pcpair_r${r}_c${col}_rw${row}_e${e}_f0_s${k}.cnf > r${r}_k${k}.cnf;
    (time minisat-2.2.0 r${r}_k${k}.cnf) > minisat_r${r}_k${k}.result 2>&1;
done;
shell> echo "n  c  t  sat  cfs dec rts r1 mem ptime stime cfl r k" > minisat_results;
for r in $(seq 1 6); do
  for k in $(seq 1 20); do
    cat minisat_r${r}_k${k}.result | awk -f $OKlib/Experimentation/ExperimentSystem/SolverMonitoring/ExtractMinisat.awk | awk " { print \$0 \"  $r  $k\" }";
  done;
done >> minisat_results;
     \endverbatim
     yields:
     \verbatim
shell> oklib --R
E = read.table("minisat_results", header=TRUE)
EM = aggregate(E, by=list(r=E$r), FUN=mean)
options(width=1000)
EM
r   n    c           t    bash_t        cfs         dec      rts         r1   mem ptime  stime         cfl
1 192 1672  0.02589555    0.0000     2172.9     2519.45    12.85      30390  8.00 0.000 0.0100     20832.6
2 336 3280  6.64343650    6.5950   409431.4   500513.75   890.90   10805115  8.00 0.000 0.0100   5935048.8
3 480 4888 39.33021650   41.2035  2037583.0  2750387.90  3644.10   57608122 10.30 0.003 0.0110  31753123.5
4 624 6496 62.84777000  175.4085  7296806.1  9758815.50 11511.35  211049076 19.80 0.007 0.0200 121938539.2
5 768 8104 63.71939500 1283.3405 34959474.8 45516313.85 47351.05 1298488466 46.30 0.010 0.0200 653104802.5
6 912 9712 65.03159500 2271.9195 49611107.0 64456781.95 64882.05 2092377044 62.45 0.010 0.0215 995531156.5
     \endverbatim
     where bash_t (time from bash time command) was extracted by:
     \verbatim
shell> echo "r bash_t" > minisat_bash_times;
for r in $(seq 1 6); do
  for k in $(seq 1 20); do
    cat minisat_r${r}_k${k}.result | awk "/^real/  { split(\$2,a,\"m\"); split(a[2],b,\"s\"); print \"${r} \" (a[1] * 60) + b[1]; }" >> minisat_bash_times;
  done;
done
shell> oklib --R
E = read.table("minisat_bash_times", header=TRUE)
EM = aggregate(E, by=list(r=E$r), FUN=mean)
EM
     \endverbatim
     </li>
     <li> Consider:
     \verbatim
R> plot(E$cfs)
R> plot(E$bash_t)
     \endverbatim
     The minisat-2.2.0 times and number of conflicts grow much more quickly in
     the number of rounds than the 1-base and canonical translations. Is this
     exponential growth? </li>
     <li> We need data for more rounds. </li>
     <li> minisat-2.2.0 on 7 rounds is still running for 20 keys;
     With seed=1, minisat-2.2.0 uses 46085781 conflicts and takes
     47 minutes. </li>
     <li> Running OKsolver_2002:
     \verbatim
shell> col=1; row=2; e=8; r=1;
for k in $(seq 1 20); do
    echo "Key ${k} Round ${r}";
    AppendDimacs-O3-DNDEBUG ssaes_r${r}_c${col}_rw${row}_e${e}_f0.cnf ssaes_pcpair_r${r}_c${col}_rw${row}_e${e}_f0_s${k}.cnf > r${r}_k${k}.cnf;
    (time OKsolver_2002-O3-DNDEBUG r${r}_k${k}.cnf) > minisat_r${r}_k${k}.result 2>&1;
done;
     \endverbatim
     is still running after 5 hours on a single instance.
     </li>
    </ul>
   </li>
  </ul>

*/
