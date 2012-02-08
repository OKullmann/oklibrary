// Matthew Gwynne, 16.6.2011 (Swansea)
/* Copyright 2011, 2012 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/Cryptography/AdvancedEncryptionStandard/plans/KeyDiscovery/008/1_2_4/20_13.hpp
  \brief Investigations into small-scale AES key discovery for 20+1/3 round AES with 1 row, 2 columns and 4-bit field size


  \todo Problem specification
  <ul>
   <li> We investigate the 20 + 1/3 round small-scale AES with 1 row,
   2 column, using the 4-bit field size. </li>
   <li> We denote this AES instance by aes(20,1,2,4). </li>
   <li> aes(20,1,2,4) takes a 8-bit plaintext and 8-bit key and
   outputs a 8-bit ciphertext. </li>
   <li> For the full specification of this AES instance, see
   "Problem specification" in
   Cryptography/AdvancedEncryptionStandard/plans/KeyDiscovery/008/1_1_8/general.hpp.
   </li>
   <li> The decompositions and translations are listed in "Investigating
   dimensions" in
   Cryptography/AdvancedEncryptionStandard/plans/Experimentation.hpp.
   </li>
   <li> Note that we consider the canonical CNF translation, as
   this is an example of the "hardest" representation without
   new variables. See "Hardness of boolean function representations"
   in
   Experimentation/Investigations/BooleanFunctions/plans/general.hpp. </li>
  </ul>


  \todo Using the 1-base box translation
  <ul>
   <li> Translation of aes(20,1,2,4):
    <ul>
     <li> We treat S-boxes and additions as boxes. </li>
     <li> The S-box is considered as a 8x1 boolean function,
     translated using 1-bases; see ss_sbox_rbase_cnfs in
     ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/data/SmallScaleSboxCNF.mac.
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
   <li> Generating simplest small-scale AES for 20 rounds:
   \verbatim
shell> mkdir aes_1_2_4/1base
shell> cd aes_1_2_4/1base
shell> oklib --maxima
oklib_load_all()$
num_rounds : 20$
num_rows : 1$
num_columns : 2$
exp : 4$
final_round_b : false$
box_tran : aes_rbase_box$
mc_tran : aes_mc_bidirectional$
oklib_monitor : true$
output_ss_fcl_std(num_rounds, num_columns, num_rows, exp, final_round_b, box_tran, mc_tran)$

shell> cat ssaes_r20_c2_rw1_e4_f0.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG n
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
824 3972 12656 0 12656 825 1
 length count
1 80
2 640
3 1712
4 1540
   \endverbatim
   </li>
   <li> In this translation, we have:
    <ul>
     <li> 20 full rounds (Key Addition, SubBytes, and MixColumns
     operation). </li>
     <li> 60 Sboxes:
      <ul>
       <li> 20 from SubBytes = 1 byte * 20 round; </li>
       <li> 40 from key schedule = 2 row * 1 byte * 20 round. </li>
      </ul>
     </li>
     <li> 648 additions:
      <ul>
       <li> 320 additions of arity 1:
        <ul>
         <li> 160 from forward MixColumns = 8 bits * 20 round; </li>
         <li> 160 from inverse MixColumns = 8 bits * 20 rounds. </li>
        </ul>
       </li>
       <li> 248 additions of arity 2:
        <ul>
         <li> 160 from key additions = 8 bits * 20 round; </li>
         <li> 8 from final key addition = 8 bits; </li>
         <li> 80 from the key schedule = 4 bits * 1 column * 20 rounds. </li>
        </ul>
       </li>
       <li> 80 additions of arity 3:
        <ul>
         <li> 80 from the key schedule = 4 bits * 1 column * 20 rounds. </li>
        </ul>
       </li>
      </ul>
     </li>
     <li> 80 bits from the key schedule constant = 4 bits * 20 rounds. </li>
    </ul>
   </li>
   <li> The additions are translated by their prime implicates. </li>
   <li> The S-boxes are translated by a 1-base representation. See
   Cryptography/AdvancedEncryptionStandard/plans/Representations/Sbox_4.hpp.
   </li>
   <li> The number of clauses for the 1-base representation of the S-box:
   \verbatim
maxima> ncl_list_fcs(ev_hm(ss_sbox_rbase_cnfs,4));
[[3,12],[4,15]]
   \endverbatim
   </li>
   <li> This instance has the following number of clauses of length:
    <ul>
     <li> 1 : 80 = key schedule constant * 1; </li>
     <li> 2 : 640 = 320 "additions" (arity 1) * 2; </li>
     <li> 3 : 1712 = 60 S-boxes * 12 + 248 additions (arity 2) * 4; </li>
     <li> 4 : 1540 = 60 S-boxes * 15 + 80 additions (arity 3) * 8. </li>
    </ul>
   </li>
   <li> Generating 20 random plaintext-ciphertext-pairs and running
   solvers instances instantiated with these pairs to find the key:
    <ul>
     <li> Computing the random plaintext-ciphertext-pairs:
     \verbatim
for seed : 1 thru 20 do output_ss_random_pc_pair(seed,num_rounds,num_columns,num_rows,exp,final_round_b);
     \endverbatim
     </li>
     <li> Running minisat-2.2.0:
     \verbatim
shell> col=2; row=1; e=4; r=20; for s in $(seq 1 5); do
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
   r   n      c      t sat    cfs    dec  rts       r1 mem ptime stime   cfl  r
1 20 824 3704.1 0.0056   1 126.91 161.15 1.77 33210.35  19     0 1e-04 978.7 20
     k s
1 10.5 3
     \endverbatim
     </li>
     <li> Running OKsolver_2002:
     \verbatim
shell> col=2; row=1; e=4; r=20; for s in $(seq 1 5); do
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
   r   n    c     l     t sat   nds r1     r2 pls  ats    h file n2cr dmcl dn
1 20 824 3988 12672 0.019   1 64.35 96 101.15   0 0.15 9.53   NA  672    0 96
   dc   dl snds qnds mnds tel oats n2cs m2cs  r    k s
1 448 1824    0    0    0   0    0    0    0 20 10.5 3
     \endverbatim
     </li>
    </ul>
   </li>
  </ul>


  \todo Using the minimum box translation
  <ul>
   <li> Translation of aes(20,1,2,4):
    <ul>
     <li> We treat S-boxes and additions as boxes. </li>
     <li> The S-box is considered as a 8x1 boolean function,
     translated using the minimum translation; see ss_sbox_cnfs in
     ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/data/SmallScaleSboxCNF.mac
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
   <li> Generating simplest small-scale AES for 20 rounds:
   \verbatim
shell> mkdir aes_1_2_4/min
shell> cd aes_1_2_4/min
shell> oklib --maxima
oklib_load_all()$
num_rounds : 20$
num_rows : 1$
num_columns : 2$
exp : 4$
final_round_b : false$
box_tran : aes_small_box$
mc_tran : aes_mc_bidirectional$
oklib_monitor : true$
output_ss_fcl_std(num_rounds, num_columns, num_rows, exp, final_round_b, box_tran, mc_tran)$

shell> cat ssaes_r20_c1_rw1_e4_f0.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG n
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
824 3672 11816 0 11816 825 1
 length count
1 80
2 640
3 1472
4 1360
5 120
   \endverbatim
   </li>
   <li> In this translation, we have:
    <ul>
     <li> 20 full rounds (Key Addition, SubBytes, and MixColumns
     operation). </li>
     <li> 60 Sboxes:
      <ul>
       <li> 20 from SubBytes = 1 byte * 20 round; </li>
       <li> 40 from key schedule = 2 row * 1 byte * 20 round. </li>
      </ul>
     </li>
     <li> 640 additions:
      <ul>
       <li> 320 additions of arity 1:
        <ul>
         <li> 160 from forward MixColumns = 8 bits * 20 round; </li>
         <li> 160 from inverse MixColumns = 8 bits * 20 rounds. </li>
        </ul>
       </li>
       <li> 248 additions of arity 2:
        <ul>
         <li> 160 from key additions = 8 bits * 20 round; </li>
         <li> 8 from final key addition = 8 bits; </li>
         <li> 80 from the key schedule = 4 bits * 1 column * 20 rounds. </li>
        </ul>
       </li>
       <li> 80 additions of arity 3:
        <ul>
         <li> 80 from the key schedule = 4 bits * 1 column * 20 rounds. </li>
        </ul>
       </li>
      </ul>
     </li>
     <li> 80 bits from the key schedule constant = 4 bits * 20 rounds. </li>
    </ul>
   </li>
   <li> The additions are translated by their prime implicates. </li>
   <li> The S-boxes are translated by a minimum representation. See
   Cryptography/AdvancedEncryptionStandard/plans/Representations/Sbox_4.hpp.
   </li>
   <li> The number of clauses for the "minimum" representation of the S-box:
   \verbatim
maxima> ncl_list_fcs(ev_hm(ss_sbox_cnfs,4));
[[3,8],[4,12],[5,2]]
   \endverbatim
   </li>
   <li> This instance has the following number of clauses of length:
    <ul>
     <li> 1 : 80 = key schedule constant * 1; </li>
     <li> 2 : 640 = 320 "additions" (arity 1) * 2; </li>
     <li> 3 : 1472 = 60 S-boxes * 8 + 248 additions (arity 2) * 4; </li>
     <li> 4 : 1360 = 60 S-boxes * 12 + 80 additions (arity 3) * 8; </li>
     <li> 5 : 120 = 60 S-boxes * 2. </li>
    </ul>
   </li>
   <li> Generating 20 random plaintext-ciphertext-pairs and running
   solvers instances instantiated with these pairs to find the key:
    <ul>
     <li> Computing the random plaintext-ciphertext-pairs:
     \verbatim
for seed : 1 thru 20 do output_ss_random_pc_pair(seed,num_rounds,num_columns,num_rows,exp,final_round_b);
     \endverbatim
     </li>
     <li> Running minisat-2.2.0:
     \verbatim
shell> col=2; row=1; e=4; r=20; for s in $(seq 1 5); do
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
   r   n       c      t sat     cfs    dec   rts       r1 mem ptime stime
1 20 824 3416.29 0.0249   1 1816.14 2891.7 11.55 120005.4  19     0     0
       cfl  r    k s
1 18631.45 20 10.5 3
     \endverbatim
     </li>
     <li> Running OKsolver_2002:
     \verbatim
shell> col=2; row=1; e=4; r=20; for s in $(seq 1 5); do
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
   r   n    c     l     t sat    nds r1      r2 pls ats     h file n2cr dmcl dn
1 20 824 3688 11832 0.117   1 341.98 96 2323.89   0   0 21.66   NA  672    0 96
   dc   dl snds qnds mnds tel oats n2cs m2cs  r    k s
1 448 1824 7.14    0  1.1   0    0    0    0 20 10.5 3
     \endverbatim
     </li>
    </ul>
   </li>
  </ul>


  \todo Using the canonical box translation
  <ul>
   <li> Translation of aes(20,1,2,4):
    <ul>
     <li> We treat S-boxes and additions as boxes. </li>
     <li> The S-box is considered as a 8x1 boolean function,
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
   <li> Generating simplest small-scale AES for 20 rounds:
   \verbatim
shell> mkdir aes_1_2_4/canon
shell> cd aes_1_2_4/canon
shell> oklib --maxima
oklib_load_all()$
num_rounds : 20$
num_rows : 1$
num_columns : 2$
exp : 4$
final_round_b : false$
box_tran : aes_ts_box$
seed : 1$
mc_tran : aes_mc_bidirectional$
oklib_monitor : true$
output_ss_fcl_std(num_rounds, num_columns, num_rows, exp, final_round_b, box_tran, mc_tran)$

shell> cat ssaes_r20_c2_rw1_e4_f0.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG n
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
1784 11052 31856 0 31856 1785 1
 length count
1 80
2 8320
3 992
4 640
9 960
16 60
   \endverbatim
   </li>
   <li> In this translation, we have:
    <ul>
     <li> 20 full rounds (Key Addition, SubBytes, and MixColumns
     operation). </li>
     <li> 60 Sboxes:
      <ul>
       <li> 20 from SubBytes = 1 byte * 20 round; </li>
       <li> 40 from key schedule = 2 row * 1 byte * 20 round. </li>
      </ul>
     </li>
     <li> 648 additions:
      <ul>
       <li> 320 additions of arity 1:
        <ul>
         <li> 160 from forward MixColumns = 8 bits * 20 round; </li>
         <li> 160 from inverse MixColumns = 8 bits * 20 rounds. </li>
        </ul>
       </li>
       <li> 248 additions of arity 2:
        <ul>
         <li> 160 from key additions = 8 bits * 20 round; </li>
         <li> 8 from final key addition = 8 bits; </li>
         <li> 80 from the key schedule = 4 bits * 1 column * 20 rounds. </li>
        </ul>
       </li>
       <li> 80 additions of arity 3:
        <ul>
         <li> 80 from the key schedule = 4 bits * 1 column * 20 rounds. </li>
        </ul>
       </li>
      </ul>
     </li>
     <li> 80 bits from the key schedule constant = 4 bits * 20 rounds. </li>
    </ul>
   </li>
   <li> The additions are translated by their prime implicates. </li>
   <li> The S-boxes are translated by the canonical translation. </li>
   <li> The number of clauses for the canonical representation of the S-box:
   \verbatim
maxima> ncl_list_full_dualts(8,16);
[[2,128],[9,16],[16,1]]
   \endverbatim
   </li>
   <li> This instance has the following number of clauses of length:
    <ul>
     <li> 1 : 80 = key schedule constant * 1; </li>
     <li> 2 : 8320 = 60 S-boxes * 128 +  320 "additions" (arity 1) * 2; </li>
     <li> 3 : 992 = 248 additions (arity 2) * 4; </li>
     <li> 4 : 640 = 80 additions (arity 3) * 8; </li>
     <li> 9 : 960 = 60 S-boxes * 16; </li>
     <li> 16 : 60 = 60 S-boxes * 1. </li>
    </ul>
   </li>
   <li> Generating 20 random plaintext-ciphertext-pairs and running
   solvers instances instantiated with these pairs to find the key:
    <ul>
     <li> Computing the random plaintext-ciphertext-pairs:
     \verbatim
for seed : 1 thru 20 do output_ss_random_pc_pair(seed,num_rounds,num_columns,num_rows,exp,final_round_b);
     \endverbatim
     </li>
     <li> Running minisat-2.2.0:
     \verbatim
shell> col=2; row=1; e=4; r=20; for s in $(seq 1 5); do
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
   r    n        c      t sat    cfs    dec  rts       r1 mem ptime  stime
1 20 1784 10792.23 0.0252   1 214.83 287.32 2.54 111353.7  20 6e-04 0.0115
      cfl  r    k s
1 2642.47 20 10.5 3
     \endverbatim
     </li>
     <li> Running OKsolver_2002:
     \verbatim
shell> col=2; row=1; e=4; r=20; for s in $(seq 1 5); do
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
   r    n     c     l     t sat   nds r1     r2 pls  ats    h file n2cr dmcl dn
1 20 1784 11068 31872 0.345   1 37.42 96 364.51   0 0.24 8.12   NA 8352    0 96
   dc   dl snds qnds mnds tel oats n2cs m2cs  r    k s
1 448 1824    0    0    0   0    0    0    0 20 10.5 3
     \endverbatim
     </li>
    </ul>
   </li>
  </ul>


  \todo Using the canonical CNF box translation
  <ul>
   <li> Translation of aes(20,1,2,4):
    <ul>
     <li> We treat S-boxes and additions as boxes. </li>
     <li> S-boxes are translated using the canonical CNF translation;
     see ss_sbox_fullcnf_fcs in
     ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/SboxAnalysis.mac.
     </li>
     <li> Additions are translated using their prime implicates. </li>
     <li> The MixColumns operation is translated by translating both
     the MixColumns operation and its inverse. </li>
    </ul>
   </li>
   <li> Generating simplest small-scale AES for 20 rounds:
   \verbatim
shell> mkdir aes_1_2_4/full
shell> cd aes_1_2_4/full
shell> oklib --maxima
oklib_load_all()$
num_rounds : 20$
num_rows : 1$
num_columns : 2$
exp : 4$
final_round_b : false$
box_tran : aes_full_box$
seed : 1$
mc_tran : aes_mc_bidirectional$
oklib_monitor : true$
output_ss_fcl_std(num_rounds, num_columns, num_rows, exp, final_round_b, box_tran, mc_tran)$

shell> cat ssaes_r20_c2_rw1_e4_f0.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG n
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
824 16752 122096 0 122096 825 1
 length count
1 80
2 640
3 992
4 640
8 14400
   \endverbatim
   </li>
   <li> In this translation, we have:
    <ul>
     <li> 20 full rounds (Key Addition, SubBytes, and MixColumns
     operation). </li>
     <li> 60 Sboxes:
      <ul>
       <li> 20 from SubBytes = 1 byte * 20 round; </li>
       <li> 40 from key schedule = 2 row * 1 byte * 20 round. </li>
      </ul>
     </li>
     <li> 640 additions:
      <ul>
       <li> 320 additions of arity 1:
        <ul>
         <li> 160 from forward MixColumns = 8 bits * 20 round; </li>
         <li> 160 from inverse MixColumns = 8 bits * 20 rounds. </li>
        </ul>
       </li>
       <li> 248 additions of arity 2:
        <ul>
         <li> 160 from key additions = 8 bits * 20 round; </li>
         <li> 8 from final key addition = 8 bits; </li>
         <li> 80 from the key schedule = 4 bits * 1 column * 20 rounds. </li>
        </ul>
       </li>
       <li> 80 additions of arity 3:
        <ul>
         <li> 80 from the key schedule = 4 bits * 1 column * 20 rounds. </li>
        </ul>
       </li>
      </ul>
     </li>
     <li> 80 bits from the key schedule constant = 4 bits * 20 rounds. </li>
    </ul>
   </li>
   <li> The additions are translated by their prime implicates. </li>
   <li> The S-boxes are translated by the canonical CNF. </li>
   <li> The number of clauses for the canonical representation of the S-box:
   \verbatim
maxima> ncl_list_fcs(ss_sbox_fullcnf_fcs(2,4,ss_polynomial_2_4));
[[8,240]]
   \endverbatim
   </li>
   <li> This instance has the following number of clauses of length:
    <ul>
     <li> 1 : 80 = key schedule constant * 1; </li>
     <li> 2 : 640 = 320 "additions" (arity 1) * 2; </li>
     <li> 3 : 992 = 248 additions (arity 2) * 4; </li>
     <li> 4 : 640 = 80 additions (arity 3) * 8; </li>
     <li> 8 : 14400 = 60 S-boxes * 240. </li>
    </ul>
   </li>
   <li> Generating 20 random plaintext-ciphertext-pairs and running
   solvers instances instantiated with these pairs to find the key:
    <ul>
     <li> Computing the random plaintext-ciphertext-pairs:
     \verbatim
for seed : 1 thru 20 do output_ss_random_pc_pair(seed,num_rounds,num_columns,num_rows,exp,final_round_b);
     \endverbatim
     </li>
     <li> Running minisat-2.2.0:
     \verbatim
shell> col=2; row=1; e=4; r=20; for s in $(seq 1 5); do
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
   r   n        c      t sat      cfs     dec  rts       r1 mem  ptime  stime
1 20 824 16493.35 0.4068   1 15310.35 25657.8 59.7 545828.6  20 0.0072 0.1371
       cfl  r    k s
1 157323.7 20 10.5 3
     \endverbatim
     </li>
    </ul>
   </li>
  </ul>

*/
