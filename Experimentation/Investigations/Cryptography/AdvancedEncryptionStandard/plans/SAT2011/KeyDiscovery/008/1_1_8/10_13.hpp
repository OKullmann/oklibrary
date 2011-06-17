// Matthew Gwynne, 16.6.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/KeyDiscovery/008/1_1_8/10_13.hpp
  \brief Investigations into simplest 8-bit small scale AES key discovery for 10 + 1/3 round AES


  \todo Problem specification
  <ul>
   <li> In this file, we collect the investigations into translations of
   10 + 1/3 round small scale AES with one rows, one columns, using the 8-bit
   field size. </li>
   <li> The AES encryption scheme we model takes a 8-bit plaintext and
   8-bit key and outputs a 8-bit ciphertext.
   </li>
   <li> The 8-bit element (b_0,b_1,b_2,b_3,b_4,b_5,b_6,b_7) is considered as
   the polynomial b_0 * x^7 + b_1 * x^6 + b_2 * x^5 + b_4 * x^3 + b_5 * x^2 +
   b^6 * x + b_7. Addition and multiplication on these polynomials is defined
   as usual, modulo the polynomial x^8+x^4+x^3+x+1. </li>
   <li> The encryption scheme applies the following operations:
   <ol>
    <li> Key schedule which takes the key and generates twenty-one 8-bit round
    keys. </li>
    <li> Application of the following operation (the "round") twenty times:
     <ol>
      <li> Addition of round key n-1. </li>
      <li> Application of Sbox operation. </li>
     </ol>
    </li>
    <li> Addition of round key n. </li>
    <li> The result of the last round key addition is then the ciphertext.
    </li>
   </ol>
   </li>
   <li> The small scale AES variant is decomposed into small boolean functions
   which are then translated to SAT. </li>
   <li> The decompositions and translations are listed in "Investigating
   dimensions" in
   Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/Experimentation.hpp.
   </li>
  </ul>


  \todo Using the canonical box translation
  <ul>
   <li> Generating 8-bit small scale AES for 10 rounds:
   \verbatim
shell> mkdir aes_1_1_8/canon
shell> cd aes_1_1_8/canon
shell> oklib --maxima
oklib_load_all()$
num_rounds : 10$
num_rows : 1$
num_columns : 1$
exp : 8$
final_round_b : false$
box_tran : aes_ts_box$
mc_tran : aes_mc_bidirectional$
oklib_monitor : true$
output_ss_fcl_std(num_rounds, num_columns, num_rows, exp, final_round_b, box_tran, mc_tran)$

shell> cat ssaes_r10_c1_rw1_e8_f0.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG n
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
5624 88132 258736 0 258736 5625 1
 length count
1 80
2 82240
3 672
17 5120
256 20
   \endverbatim
   </li>
   <li> In this translation, we have:
    <ul>
     <li> Ten full rounds (Key Addition, SubBytes and MixColumns).
     </li>
     <li> 20 Sboxes (10 from SubBytes; 10 from key schedule). </li>
     <li> 328 additions (88 from key additions; 160 from MixColumns
     (encryption and decryption); 80 from Key Schedule). </li>
     <li> 80 bits for the constant in the key schedule. </li>
    </ul>
   </li>
   <li> The additions are translated by their prime implicates. </li>
   <li> The S-boxes are translated by the canonical representation. </li>
   <li> The number of clauses in the canonical translation:
   \verbatim
maxima> ncl_list_full_dualts(8,16);
[[2,4096],[17,256],[256,1]]
   \endverbatim
   </li>
   <li> We have the following number of clauses of the following size:
    <ul>
     <li> 80 unit-clauses (key schedule constant). </li>
     <li> 82440 binary clauses (20 S-boxes; 160 arity one additions). </li>
     <li> 672 ternary clauses (168 arity two additions). </li>
     <li> 5120 clauses of length nine (20 S-boxes) </li>
     <li> 20 clauses of length 256 (20 S-boxes). </li>
    </ul>
   </li>
   <li> Generating 20 random plaintext-ciphertext pairs and running
   solvers instances instantiated with these pairs to find the key:
    <ul>
     <li> Computing the random plaintext-ciphertext pairs:
     \verbatim
for seed : 1 thru 20 do output_ss_random_pc_pair(seed,num_rounds,num_columns,num_rows,exp,final_round_b);
     \endverbatim
     </li>
     <li> Running minisat-2.2.0:
     \verbatim
shell> col=1; row=1; e=8; r=10; for s in $(seq 1 5); do
  for k in $(seq 1 20); do
    echo "Seed ${s}; Key ${k} Round ${r}";
    AppendDimacs-O3-DNDEBUG ssaes_r${r}_c${col}_rw${row}_e${e}_f0.cnf ssaes_pcpair_r${r}_c${col}_rw${row}_e${e}_f0_s${k}.cnf | RandomShuffleDimacs-O3-DNDEBUG $s > r${r}_k${k}_s${s}.cnf;
    minisat-2.2.0 r${r}_k${k}_s${s}.cnf > minisat_r${r}_k${k}_s${s}.result 2>&1;
  done;
done;
shell> echo "n  c  t  sat  cfs dec rts r1 mem ptime stime cfl r k s" > minisat_results; for s in $(seq 1 5); do
  for k in $(seq 1 20); do
    cat minisat_r${r}_k${k}_s${s}.result | awk -f $OKlib/Experimentation/ExperimentSystem/SolverMonitoring/ExtractMinisat.awk | awk " { print \$0 \"  $r  $k $s\" }";
  done;
done >> minisat_results;
     \endverbatim
     yields:
     \verbatim
shell> oklib --R
E = read.table("minisat_results", header=TRUE)
EM = aggregate(E, by=list(r=E$r), FUN=mean)
E
   r    n        c      t sat    cfs     dec  rts       r1 mem  ptime  stime
1 10 5624 87959.61 0.2082   1 267.53 3301.08 2.75 509016.3  26 0.0388 0.1149
       cfl  r    k s
1 68487.33 10 10.5 3
     \endverbatim
     </li>
     <li> Running OKsolver_2002:
     \verbatim
shell> col=1; row=1; e=8; r=10; for s in $(seq 1 5); do
  for k in $(seq 1 20); do
    echo "Seed ${s}; Key ${k} Round ${r}";
    AppendDimacs-O3-DNDEBUG ssaes_r${r}_c${col}_rw${row}_e${e}_f0.cnf ssaes_pcpair_r${r}_c${col}_rw${row}_e${e}_f0_s${k}.cnf | RandomShuffleDimacs-O3-DNDEBUG $s > r${r}_k${k}_s${s}.cnf;
    OKsolver_2002-O3-DNDEBUG r${r}_k${k}_s${s}.cnf > oksolver_r${r}_k${k}_s${s}.result 2>&1;
  done;
done;
shell> echo "n  c  l  t  sat  nds  r1  r2  pls  ats h file n2cr  dmcl dn  dc  dl snds qnds mnds  tel  oats  n2cs  m2cs r k s" > oksolver_results; for s in $(seq 1 5); do
  for k in $(seq 1 20); do
    cat oksolver_r${r}_k${k}_s${s}.result | awk -f $OKlib/Experimentation/ExperimentSystem/SolverMonitoring/ExtractOKsolver.awk | awk " { print \$0 \"  $r  $k $s\" }";
  done;
done >> oksolver_results;
     \endverbatim
     yields:
     \verbatim
shell> oklib --R
E = read.table("oksolver_results", header=TRUE)
EM = aggregate(E, by=list(r=E$r), FUN=mean)
EM
   r    n     c      l    t sat nds r1      r2 pls ats h file  n2cr dmcl dn  dc
1 10 5624 88148 258752 6.26   1   1 96 3490.58   0 0.7 0   NA 82432    0 96 288
   dl snds qnds mnds tel oats n2cs m2cs  r    k s
1 864    0    0    0   0    0    0    0 10 10.5 3
     \endverbatim
     </li>
    </ul>
   </li>
  </ul>


  \todo Using the "minimum" box translation
  <ul>
   <li> Generating 8-bit small scale AES for 10 rounds:
   \verbatim
shell> mkdir aes_1_1_8/small
shell> cd aes_1_1_8/small
shell> oklib --maxima
oklib_load_all()$
num_rounds : 10$
num_rows : 1$
num_columns : 1$
exp : 8$
final_round_b : false$
box_tran : aes_small_box$
mc_tran : aes_mc_bidirectional$
oklib_monitor : true$
output_ss_fcl_std(num_rounds, num_columns, num_rows, exp, final_round_b, box_tran, mc_tran)$

shell> cat ssaes_r10_c1_rw1_e8_f0.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG n
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
504 6952 41516 0 41516 505 1
 length count
1 80
2 320
3 672
6 2860
7 2540
8 480
   \endverbatim
   </li>
   <li> In this translation, we have:
    <ul>
     <li> Ten full rounds (Key Addition, SubBytes and MixColumns).
     </li>
     <li> 20 Sboxes (10 from SubBytes; 10 from key schedule). </li>
     <li> 328 additions (88 from key additions; 160 from MixColumns
     (encryption and decryption); 80 from Key Schedule). </li>
     <li> 80 bits for the constant in the key schedule. </li>
    </ul>
   </li>
   <li> The additions are translated by their prime implicates. </li>
   <li> The S-boxes are translated by the "minimum" representation. See
   Cryptography/AdvancedEncryptionStandard/plans/SAT2011/Representations/Sbox_8.hpp.
   </li>
   <li> The number of clauses in the minimum translation:
   \verbatim
maxima> ncl_list_fcs(ev_hm(ss_sbox_cnfs,8));
[[6,143],[7,127],[8,24]]
   \endverbatim
   </li>
   <li> We have the following number of clauses of the following size:
    <ul>
     <li> 80 unit-clauses (key schedule constant). </li>
     <li> 320 binary clauses (160 arity one additions). </li>
     <li> 672 ternary clauses (168 arity two additions). </li>
     <li> 2860 clauses of length six (20 S-boxes) </li>
     <li> 2540 clauses of length seven (20 S-boxes). </li>
     <li> 480 clauses of length eight (20 S-boxes). </li>
    </ul>
   </li>
   <li> Generating 20 random plaintext-ciphertext pairs and running
   solvers instances instantiated with these pairs to find the key:
    <ul>
     <li> Computing the random plaintext-ciphertext pairs:
     \verbatim
for seed : 1 thru 20 do output_ss_random_pc_pair(seed,num_rounds,num_columns,num_rows,exp,final_round_b);
     \endverbatim
     </li>
     <li> Running minisat-2.2.0:
     \verbatim
shell> col=1; row=1; e=8; r=10; for s in $(seq 1 5); do
  for k in $(seq 1 20); do
    echo "Seed ${s}; Key ${k} Round ${r}";
    AppendDimacs-O3-DNDEBUG ssaes_r${r}_c${col}_rw${row}_e${e}_f0.cnf ssaes_pcpair_r${r}_c${col}_rw${row}_e${e}_f0_s${k}.cnf | RandomShuffleDimacs-O3-DNDEBUG $s > r${r}_k${k}_s${s}.cnf;
    minisat-2.2.0 r${r}_k${k}_s${s}.cnf > minisat_r${r}_k${k}_s${s}.result 2>&1;
  done;
done;
shell> echo "n  c  t  sat  cfs dec rts r1 mem ptime stime cfl r k s" > minisat_results; for s in $(seq 1 5); do
  for k in $(seq 1 20); do
    cat minisat_r${r}_k${k}_s${s}.result | awk -f $OKlib/Experimentation/ExperimentSystem/SolverMonitoring/ExtractMinisat.awk | awk " { print \$0 \"  $r  $k $s\" }";
  done;
done >> minisat_results;
     \endverbatim
     yields:
     \verbatim
shell> oklib --R
E = read.table("minisat_results", header=TRUE)
EM = aggregate(E, by=list(r=E$r), FUN=mean)
EM
   r   n       c     t sat      cfs      dec   rts      r1   mem ptime  stime
1 10 504 6772.45 1.537   1 163743.8 235628.5 402.1 1974206 19.07     0 0.0227
      cfl  r    k s
1 2094753 10 10.5 3
     \endverbatim
     </li>
     <li> Running OKsolver_2002:
     \verbatim
shell> col=1; row=1; e=8; r=10; for s in $(seq 1 5); do
  for k in $(seq 1 20); do
    echo "Seed ${s}; Key ${k} Round ${r}";
    AppendDimacs-O3-DNDEBUG ssaes_r${r}_c${col}_rw${row}_e${e}_f0.cnf ssaes_pcpair_r${r}_c${col}_rw${row}_e${e}_f0_s${k}.cnf | RandomShuffleDimacs-O3-DNDEBUG $s > r${r}_k${k}_s${s}.cnf;
    OKsolver_2002-O3-DNDEBUG r${r}_k${k}_s${s}.cnf > oksolver_r${r}_k${k}_s${s}.result 2>&1;
  done;
done;
shell> echo "n  c  l  t  sat  nds  r1  r2  pls  ats h file n2cr  dmcl dn  dc  dl snds qnds mnds  tel  oats  n2cs  m2cs r k s" > oksolver_results; for s in $(seq 1 5); do
  for k in $(seq 1 20); do
    cat oksolver_r${r}_k${k}_s${s}.result | awk -f $OKlib/Experimentation/ExperimentSystem/SolverMonitoring/ExtractOKsolver.awk | awk " { print \$0 \"  $r  $k $s\" }";
  done;
done >> oksolver_results;
     \endverbatim
     yields:
     \verbatim
shell> oklib --R
E = read.table("oksolver_results", header=TRUE)
EM = aggregate(E, by=list(r=E$r), FUN=mean)
EM
   r   n    c     l     t sat     nds r1      r2 pls ats     h file n2cr dmcl
1 10 504 6968 41532 1.269   1 1083.65 96 6051.63   0   0 18.95   NA  512    0
  dn  dc  dl snds qnds mnds tel oats n2cs m2cs  r    k s
1 96 288 864    0    0 0.03   0    0    0    0 10 10.5 3
     \endverbatim
     </li>
    </ul>
   </li>
  </ul>


  \todo Using the 1-base box translation
  <ul>
   <li> Generating a 1-base for the S-box from
   Cryptography/AdvancedEncryptionStandard/plans/SAT2011/Representations/Sbox_8.hpp. :
   \verbatim
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
   <li> Generating 8-bit small scale AES for 10 rounds:
   \verbatim
shell> mkdir aes_1_1_8/small
shell> cd aes_1_1_8/small
shell> oklib --maxima
oklib_load_all()$
set_hm(ss_sbox_rbase_cnfs,8,read_fcl_f("AES_Sbox_base.cnf"))$
num_rounds : 10$
num_rows : 1$
num_columns : 1$
exp : 8$
final_round_b : false$
box_tran : aes_rbase_box$
mc_tran : aes_mc_bidirectional$
oklib_monitor : true$
output_ss_fcl_std(num_rounds, num_columns, num_rows, exp, final_round_b, box_tran, mc_tran)$

shell> cat ssaes_r10_c1_rw1_e8_f0.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG n
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
504 89032 604896 0 604896 505 1
 length count
1 80
2 320
3 672
5 20
6 23740
7 54060
8 10060
9 80
   \endverbatim
   </li>
   <li> In this translation, we have:
    <ul>
     <li> Ten full rounds (Key Addition, SubBytes and MixColumns).
     </li>
     <li> 20 Sboxes (10 from SubBytes; 10 from key schedule). </li>
     <li> 328 additions (88 from key additions; 160 from MixColumns
     (encryption and decryption); 80 from Key Schedule). </li>
     <li> 80 bits for the constant in the key schedule. </li>
    </ul>
   </li>
   <li> The additions are translated by their prime implicates. </li>
   <li> The S-boxes are translated by the "minimum" representation. See
   Cryptography/AdvancedEncryptionStandard/plans/SAT2011/Representations/Sbox_8.hpp.
   </li>
   <li> We have the following number of clauses of the following size:
    <ul>
     <li> 80 unit-clauses (key schedule constant). </li>
     <li> 320 binary clauses (160 arity one additions). </li>
     <li> 672 ternary clauses (168 arity two additions). </li>
     <li> 20 clauses of size five (20 S-boxes). </li>
     <li> 23740 clauses of size six (20 S-boxes). </li>
     <li> 54060 clauses of length seven (20 S-boxes). </li>
     <li> 10060 clauses of length eight (20 S-boxes). </li>
     <li> 80 clauses of length nine (20 S-boxes). </li>
    </ul>
   </li>
   <li> Generating 20 random plaintext-ciphertext pairs and running
   solvers instances instantiated with these pairs to find the key:
    <ul>
     <li> Computing the random plaintext-ciphertext pairs:
     \verbatim
for seed : 1 thru 20 do output_ss_random_pc_pair(seed,num_rounds,num_columns,num_rows,exp,final_round_b);
     \endverbatim
     </li>
     <li> Running minisat-2.2.0:
     \verbatim
shell> col=1; row=1; e=8; r=10; for s in $(seq 1 5); do
  for k in $(seq 1 20); do
    echo "Seed ${s}; Key ${k} Round ${r}";
    AppendDimacs-O3-DNDEBUG ssaes_r${r}_c${col}_rw${row}_e${e}_f0.cnf ssaes_pcpair_r${r}_c${col}_rw${row}_e${e}_f0_s${k}.cnf | RandomShuffleDimacs-O3-DNDEBUG $s > r${r}_k${k}_s${s}.cnf;
    minisat-2.2.0 r${r}_k${k}_s${s}.cnf > minisat_r${r}_k${k}_s${s}.result 2>&1;
  done;
done;
shell> echo "n  c  t  sat  cfs dec rts r1 mem ptime stime cfl r k s" > minisat_results; for s in $(seq 1 5); do
  for k in $(seq 1 20); do
    cat minisat_r${r}_k${k}_s${s}.result | awk -f $OKlib/Experimentation/ExperimentSystem/SolverMonitoring/ExtractMinisat.awk | awk " { print \$0 \"  $r  $k $s\" }";
  done;
done >> minisat_results;
     \endverbatim
     yields:
     \verbatim
shell> oklib --R
E = read.table("minisat_results", header=TRUE)
EM = aggregate(E, by=list(r=E$r), FUN=mean)
EM
   r   n        c      t sat     cfs     dec   rts       r1 mem  ptime  stime
1 10 504 88855.71 3.7938   1 2129.09 2955.32 12.41 108869.1  28 0.0473 3.3283
    cfl  r    k s
1 26099 10 10.5 3
     \endverbatim
     </li>
     <li> Running OKsolver_2002:
     \verbatim
shell> col=1; row=1; e=8; r=10; for s in $(seq 1 5); do
  for k in $(seq 1 20); do
    echo "Seed ${s}; Key ${k} Round ${r}";
    AppendDimacs-O3-DNDEBUG ssaes_r${r}_c${col}_rw${row}_e${e}_f0.cnf ssaes_pcpair_r${r}_c${col}_rw${row}_e${e}_f0_s${k}.cnf | RandomShuffleDimacs-O3-DNDEBUG $s > r${r}_k${k}_s${s}.cnf;
    OKsolver_2002-O3-DNDEBUG r${r}_k${k}_s${s}.cnf > oksolver_r${r}_k${k}_s${s}.result 2>&1;
  done;
done;
shell> echo "n  c  l  t  sat  nds  r1  r2  pls  ats h file n2cr  dmcl dn  dc  dl snds qnds mnds  tel  oats  n2cs  m2cs r k s" > oksolver_results; for s in $(seq 1 5); do
  for k in $(seq 1 20); do
    cat oksolver_r${r}_k${k}_s${s}.result | awk -f $OKlib/Experimentation/ExperimentSystem/SolverMonitoring/ExtractOKsolver.awk | awk " { print \$0 \"  $r  $k $s\" }";
  done;
done >> oksolver_results;
     \endverbatim
     yields:
     \verbatim
shell> oklib --R
E = read.table("oksolver_results", header=TRUE)
EM = aggregate(E, by=list(r=E$r), FUN=mean)
EM
   r   n     c      l      t sat    nds r1     r2 pls ats   h file n2cr dmcl dn
1 10 504 89048 604912 51.873   1 200.75 96 468.36   0 0.1 9.9   NA  512    0 96
   dc  dl snds qnds mnds tel oats n2cs m2cs  r    k s
1 288 864    0    0    0   0    0    0    0 10 10.5 3
     \endverbatim
     </li>
    </ul>
   </li>
  </ul>

*/
