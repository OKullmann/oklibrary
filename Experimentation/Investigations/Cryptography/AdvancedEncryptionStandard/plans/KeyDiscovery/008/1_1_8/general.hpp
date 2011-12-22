// Matthew Gwynne, 18.7.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/Cryptography/AdvancedEncryptionStandard/plans/KeyDiscovery/008/1_1_8/general.hpp
  \brief Investigations into small-scale AES key discovery with 1 row, 1 column and 8-bit field elements


  \todo Problem specification
  <ul>
  <li> We consider the small-scale AES with 1 row, 1 column, using the 8-bit
   field size for rounds 1 to 20. </li>
   <li> We denote this AES instance by aes(r,1,1,8) for r in 1,...,20. </li>
   <li> We investigate translations of the key discovery problem for
   aes(r,1,1,8) into SAT. </li>
   <li> aes(r,1,1,8) takes a 8-bit plaintext and 8-bit key and outputs a
   8-bit ciphertext. </li>
   <li> aes(r,1,1,8) applies the following operations:
    <ol>
     <li> Key schedule which takes the key and generates r+1 8-bit round
     keys. </li>
     <li> Application of the following operation (the "round") r times:
      <ol>
       <li> Addition of round key n-1. </li>
       <li> Application of Sbox operation. </li>
      </ol>
     </li>
     <li> Addition of round key r+1. </li>
     <li> The result of the last round key addition is the ciphertext. </li>
    </ol>
   </li>
   <li> Round key 0 is the input key. </li>
   <li> The key schedule computes the round key i, K_i, from round key i-1,
   K_(i-1), by:
   \verbatim
K_i := S-box(K_(i-1)) + C_i
   \endverbatim
   where C_i is the 8-bit round constant for round i. </li>
   <li> The S-box is a permutation from {0,1}^8 to {0,1}^8 which we consider
   as either:
    <ul>
     <li> a 16x1 boolean function; see rijn_sbox_bf in
     ComputerAlgebra/Cryptology/Lisp/CryptoSystems/Rijndael/AdvancedEncryptionStandard.mac.
     </li>
     <li> 4 8x1 boolean functions. </li>
    </ul>
   </li>
   <li> The decompositions and translations are listed in "Investigating
   dimensions" in
   Cryptography/AdvancedEncryptionStandard/plans/Experimentation.hpp.
   </li>
   <li> The plaintext and ciphertext variables are then set, and the SAT
   SAT solver is run on this instance to deduce the key variables. </li>
  </ul>


  \todo Overview
  <ul>
   <li> Comparing the translations:
    <ol>
     <li> "Minimum" translation:
      <ul>
       <li> All solvers solve all rounds up to 20 in less than a second. </li>
       <li> minisat-2.2.0, precosat236, precosat-570.1 all solve using far
       more conflicts than for the canonical translation. </li>
       <li> OKsolver_2002 and march_pl use a comparable number of nodes to
       the canonical translation. </li>
       <li> satz performs exponentially better using the minimum translation
       than with the canonical. </li>
      </ul>
     </li>
    </ol>
   </li>
   <li> Comparison of translations:
    <ul>
     <li> minisat-2.2.0:
      <ul>
       <li> Reading in experimental data:
       \verbatim
> git clone git://github.com/MGwynne/Experimental-data.git
> cd Experimental-data/AES/1_1_8/

> E_canon = read.table("ssaes_r1-20_c1_rw1_e8_f0_k1-20_aes_canon_box_aes_mc_bidirectional/MinisatStatistics",header=TRUE)
> E_1base = read.table("ssaes_r1-20_c1_rw1_e8_f0_k1-20_aes_1base_box_aes_mc_bidirectional/MinisatStatistics",header=TRUE)
> E_min = read.table("ssaes_r1-20_c1_rw1_e8_f0_k1-20_aes_min_box_aes_mc_bidirectional/MinisatStatistics",header=TRUE)

> plot(E_canon)
> plot(E_1base)
> plot(E_min)
       \endverbatim
       </li>
       <li> Overall:
        <ul>
         <li> Overall, all translations are comparable in terms of time taken
         to solve and the number of conflicts needed. </li>
         <li> Ratios of average solver times (fastest to slowest):
         canon -> 15.5 -> min -> x1.1 -> 1-base
	 \verbatim
sum(E_min$t) / sum(E_canon$t)
[1] 15.58217
> sum(E_1base$t) / sum(E_min$t)
[1] 1.107566
         \endverbatim
	 </li>
         <li> Ratios of average number of conflicts (low to high):
         canonical -> x10 -> 1base -> x79 -> min
	 \verbatim
> sum(E_1base$cfs) / sum(E_canon$cfs)
[1] 9.924345
> sum(E_min$cfs) / sum(E_1base$cfs)
[1] 78.91457
         \endverbatim
         </li>
        </ul>
       </li>
       <li> Canonical translation:
        <ul>
         <li> Consider:
         \verbatim
> plot(E_canon)
         \endverbatim
         </li>
         <li> We see (at least) the following interesting
         relationships/distributions:
          <ul>
           <li> rounds vs r1: bounded in the bottom right by a  polynomial
           relationship (less than quadratic).
           \verbatim
> E_canon_max = aggregate(E_canon, by=list(r=E_canon$r), FUN=max)
> m = lm(log(E_canon_max$r1) ~ log(E_canon_max$r))
> summary(m)
                   Estimate Std. Error t value Pr(>|t|)
(Intercept)         14.7856     0.5745  25.735 1.19e-15 ***
log(E_canon_max$r)   2.1345     0.2542   8.397 1.22e-07 ***
Residual standard error: 0.9005 on 18 degrees of freedom
Multiple R-squared: 0.7966,	Adjusted R-squared: 0.7853
F-statistic:  70.5 on 1 and 18 DF,  p-value: 1.222e-07

# Removing a lot of the variance due to the difference between keys
# yields a reasonable linear relationship on the average time per round
> E_canon_mean = aggregate(E_canon, by=list(r=E_canon$r), FUN=mean)
> m = lm(E_canon_mean$r1 ~ E_canon_mean$r)
                Estimate Std. Error t value Pr(>|t|)
(Intercept)    -21371894   15529152  -1.376    0.186
E_canon_mean$r  19174249    1296348  14.791 1.63e-11 ***
---
Signif. codes:  0 '***' 0.001 '**' 0.01 '*' 0.05 '.' 0.1 ' ' 1

Residual standard error: 33430000 on 18 degrees of freedom
Multiple R-squared: 0.924,	Adjusted R-squared: 0.9198
F-statistic: 218.8 on 1 and 18 DF,  p-value: 1.63e-11
           \endverbatim
           </li>
           <li> r1 vs time: (strong) sub-linear relationship:
           \verbatim
> m = lm(log(E_canon$t) ~ log(E_canon$r1))
> summary(m)
                 Estimate Std. Error t value Pr(>|t|)
(Intercept)     -11.22006    0.20233  -55.45   <2e-16 ***
log(E_canon$r1)   0.76079    0.01115   68.26   <2e-16 ***
Residual standard error: 0.4947 on 398 degrees of freedom
Multiple R-squared: 0.9213,	Adjusted R-squared: 0.9211
F-statistic:  4659 on 1 and 398 DF,  p-value: < 2.2e-16
           \endverbatim
           </li>
           <li> r vs time: linear relationship:
           \verbatim
> m = lm(E_canon$t ~ E_canon$r1)
> summary(m)
             Estimate Std. Error t value Pr(>|t|)
(Intercept) 1.761e+00  5.670e-01   3.106  0.00203 **
E_canon$r1  1.357e-07  2.207e-09  61.498  < 2e-16 ***
Residual standard error: 8.093 on 398 degrees of freedom
Multiple R-squared: 0.9048,	Adjusted R-squared: 0.9045
F-statistic:  3782 on 1 and 398 DF,  p-value: < 2.2e-16
           \endverbatim
           </li>
          </ul>
         </li>
        </ul>
       </li>
       <li> 1-base translation:
        <ul>
         <li> Consider:
         \verbatim
> plot(E_1base)
         \endverbatim
         </li>
         <li> We see (at least) the following interesting
         relationships/distributions:
          <ul>
           <li> rounds vs r1: bounded by quadratic forming a
           "convex curved triangle" in the bottom left.
           \verbatim
> E_1base_max = aggregate(E_1base, by=list(r=E_1base$r), FUN=max)
> m = lm(log(E_1base_max$r1) ~ log(E_1base_max$r))
> summary(m)
                   Estimate Std. Error t value Pr(>|t|)
(Intercept)         11.4972     0.5616   20.47 6.43e-14 ***
log(E_1base_max$r)   2.3679     0.2485    9.53 1.86e-08 ***
Residual standard error: 0.8802 on 18 degrees of freedom
Multiple R-squared: 0.8346,	Adjusted R-squared: 0.8254
F-statistic: 90.83 on 1 and 18 DF,  p-value: 1.863e-08

# Removing a lot of the variance due to the difference between keys
# yields a reasonable linear relationship on the average time per round
> E_1base_mean = aggregate(E_1base, by=list(r=E_1base$r), FUN=mean)
> m = lm(E_1base_mean$r1 ~ E_1base_mean$r)
               Estimate Std. Error t value Pr(>|t|)
(Intercept)    -2704567     853888  -3.167  0.00533 **
E_1base_mean$r  1465602      71281  20.561 5.97e-14 ***
Residual standard error: 1838000 on 18 degrees of freedom
Multiple R-squared: 0.9592,	Adjusted R-squared: 0.9569
F-statistic: 422.7 on 1 and 18 DF,  p-value: 5.972e-14
           \endverbatim
           </li>
           <li> r1 vs time: linear relationship:
           \verbatim
> m = lm(E_1base$t ~ E_1base$r1)
> summary(m)
              Estimate Std. Error t value Pr(>|t|)
(Intercept) -1.502e+01  1.295e+00   -11.6   <2e-16 ***
E_1base$r1   8.403e-06  7.081e-08   118.7   <2e-16 ***
Residual standard error: 18.66 on 398 degrees of freedom
Multiple R-squared: 0.9725,	Adjusted R-squared: 0.9724
F-statistic: 1.408e+04 on 1 and 398 DF,  p-value: < 2.2e-16
           \endverbatim
           </li>
           <li> r1 vs conflicts:  sub-linear relationship:
           \verbatim
> m = lm(log(E_1base$cfs) ~ log(E_1base$r1))
> summary(m)
                 Estimate Std. Error t value Pr(>|t|)
(Intercept)     -0.405254   0.111151  -3.646 0.000302 ***
log(E_1base$r1)  0.690970   0.007175  96.307  < 2e-16 ***
Residual standard error: 0.3233 on 398 degrees of freedom
Multiple R-squared: 0.9589,	Adjusted R-squared: 0.9588
F-statistic:  9275 on 1 and 398 DF,  p-value: < 2.2e-16
           \endverbatim
           </li>
          </ul>
         </li>
        </ul>
       </li>
       <li> minimum translation:
        <ul>
         <li> Consider:
         \verbatim
> plot(E_min)
         \endverbatim
         </li>
         <li> We see (at least) the following interesting
         relationships/distributions:
          <ul>
           <li> rounds vs r1: (weak) quadratic relationship forming a
           "convex curved triangle" in the bottom left.
           \verbatim
> m = lm(log(E_min$r1) ~ log(E_min$r))
> summary(m)
             Estimate Std. Error t value Pr(>|t|)
(Intercept)  10.07133    0.20155   49.97   <2e-16 ***
log(E_min$r)  4.47814    0.09522   47.03   <2e-16 ***
Residual standard error: 1.355 on 338 degrees of freedom
Multiple R-squared: 0.8674,	Adjusted R-squared: 0.8671
F-statistic:  2212 on 1 and 338 DF,  p-value: < 2.2e-16

# Removing a lot of the variance due to the difference between keys
# yields a quartic/quintic relationship on the average time per round
> E_min_mean = aggregate(E_min, by=list(r=E_min$r), FUN=mean)
> m = lm(log(E_min_mean$r1) ~ log(E_min_mean$r))
                  Estimate Std. Error t value Pr(>|t|)
(Intercept)        10.3867     0.4013   25.88 7.32e-14 ***
log(E_min_mean$r)   4.5902     0.1896   24.21 1.95e-13 ***
---
Signif. codes:  0 '***' 0.001 '**' 0.01 '*' 0.05 '.' 0.1 ' ' 1

Residual standard error: 0.6035 on 15 degrees of freedom
Multiple R-squared: 0.975,	Adjusted R-squared: 0.9734
F-statistic: 586.2 on 1 and 15 DF,  p-value: 1.949e-13
           \endverbatim
           </li>
           <li> r1 vs time: (strong) linear relationship:
           \verbatim
> m = lm(E_min$t ~ E_min$r1)
> summary(m)
              Estimate Std. Error t value Pr(>|t|)
(Intercept) -7.288e+02  9.600e+01  -7.592  3.1e-13 ***
E_min$r1     2.276e-06  2.291e-08  99.351  < 2e-16 ***
---
Signif. codes:  0 '***' 0.001 '**' 0.01 '*' 0.05 '.' 0.1 ' ' 1

Residual standard error: 1541 on 338 degrees of freedom
Multiple R-squared: 0.9669,	Adjusted R-squared: 0.9668
F-statistic:  9871 on 1 and 338 DF,  p-value: < 2.2e-16
           \endverbatim
           </li>
           <li> r1 vs conflicts (strong) sub-linear relationship:
           \verbatim
> m = lm(log(E_min$cfs+1) ~ log(E_min$r1+1))
> summary(m)
                   Estimate Std. Error t value Pr(>|t|)
(Intercept)       -1.471116   0.022771  -64.61   <2e-16 ***
log(E_min$r1 + 1)  0.908210   0.001182  768.11   <2e-16 ***
Residual standard error: 0.08093 on 338 degrees of freedom
Multiple R-squared: 0.9994,	Adjusted R-squared: 0.9994
F-statistic: 5.9e+05 on 1 and 338 DF,  p-value: < 2.2e-16
           \endverbatim
           </li>
          </ul>
         </li>
        </ul>
       </li>
      </ul>
     </li>
    </ul>
   </li>
  </ul>


  \todo Translations
  <ul>
   <li> The following translations are considered in this %plans %file:
    <ul>
     <li> The canonical box translation:
      <ul>
       <li> We treat S-boxes and additions as boxes. </li>
       <li> The S-box is considered as a 16x1 boolean function,
       translated using the canonical translation; see dualts_fcl in
       ComputerAlgebra/Satisfiability/Lisp/FiniteFunctions/TseitinTranslation.mac.
       </li>
       <li> Additions of arity k are considered bit-wise as (k+1)-bit to 1-bit
       boolean functions; translated using their prime implicates. </li>
       <li> The MixColumns operation is the identity. </li>
       <li> Due to limitations in the translation, clauses occur in this
       translation representing equivalence of variables in the MixColumns;
       See "Remove hard-coding of multiplication by 01 in small-scale
       MixColumn" in
       ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/plans/Translations.hpp.
       </li>
      </ul>
     </li>
     <li> The small box translation:
      <ul>
       <li> We treat S-boxes and additions as boxes. </li>
       <li> The S-box is considered as a 16x1 boolean function,
       translated using the minimum translation; see ss_sbox_cnfs in
       ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/data/SmallScaleSboxCNF.mac
       </li>
       <li> Additions of arity k are considered bit-wise as (k+1)-bit to 1-bit
       boolean functions; translated using their prime implicates. </li>
       <li> The MixColumns operation is the identity. </li>
       <li> Due to limitations in the translation, clauses occur in this
       translation representing equivalence of variables in the MixColumns;
       See "Remove hard-coding of multiplication by 01 in small-scale
       MixColumn" in
       ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/plans/Translations.hpp.
       </li>
      </ul>
     </li>
    </ul>
   </li>
   <li> For a full list of the possible translations, see
   "Investigating dimensions" in
   Cryptography/AdvancedEncryptionStandard/plans/Experimentation.hpp.
   </li>
   <li> Generating instances for rounds 1-20 for a single random key with each
   of the translations:
    <ul>
     <li> The canonical box translation:
     \verbatim
shell> mkdir ssaes_r1-20_c1_rw1_e8_f0_k1-1_aes_canon_box_aes_mc_bidirectional
shell> cd ssaes_r1-20_c1_rw1_e8_f0_k1-1_aes_canon_box_aes_mc_bidirectional
shell> oklib --maxima
num_rows : 1$
num_columns : 1$
exp : 8$
final_round_b : false$
box_tran : aes_ts_box$
mc_tran : aes_mc_bidirectional$
for num_rounds : 1 thru 20 do (
  output_ss_fcl_std(
    num_rounds, num_columns, num_rows, exp, final_round_b, box_tran, mc_tran),
  for seed : 1 thru 20 do (
    output_ss_random_pc_pair(
      seed,num_rounds,num_columns,num_rows,exp,final_round_b)))$
exit();
shell> for r in $(seq 1 20); do
  for k in $(seq 1 20); do
    AppendDimacs-O3-DNDEBUG ssaes_r${r}_c1_rw1_e8_f0.cnf ssaes_pcpair_r${r}_c1_rw1_e8_f0_s${k}.cnf > r${r}_keyfind.cnf;
  done;
done
     \endverbatim
     </li>
     <li> The 1-base box translation:
     \verbatim
shell> mkdir ssaes_r1-20_c1_rw1_e8_f0_k1-1_aes_1base_box_aes_mc_bidirectional
shell> cd ssaes_r1-20_c1_rw1_e8_f0_k1-1_aes_1base_box_aes_mc_bidirectional

# Generating a 1-base for the S-box
maxima> output_rijnsbox_fullcnf_stdname();
shell> QuineMcCluskey-n16-O3-DNDEBUG AES_Sbox_full.cnf > AES_Sbox_pi.cnf
shell> RandomShuffleDimacs-O3-DNDEBUG 103 < AES_Sbox_pi.cnf | SortByClauseLength-O3-DNDEBUG > AES_Sbox_sortedpi.cnf
shell> RUcpGen-O3-DNDEBUG AES_Sbox_sortedpi.cnf > AES_Sbox_gen.cnf
shell> RandomShuffleDimacs-O3-DNDEBUG 1 < AES_Sbox_gen.cnf | SortByClauseLengthDescending-O3-DNDEBUG | RUcpBase-O3-DNDEBUG > AES_Sbox_base.cnf
shell> cat AES_Sbox_base.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG nz
     pn      pc      n    nmi       c        l     n0   n0mi      c0       l0  cmts
     16    4398     16     16    4398    30108     NA     NA    4398    30108     0
 length   count
      5       1
      6    1187
      7    2703
      8     503
      9       4

shell> oklib --maxima
Sbox1baseCNF : read_fcl_f("AES_Sbox_base.cnf")$
set_hm(ss_sbox_rbase_cnfs,8, Sbox1baseCNF)$
num_rows : 1$
num_columns : 1$
exp : 8$
final_round_b : false$
box_tran : aes_rbase_box$
mc_tran : aes_mc_bidirectional$
for num_rounds : 1 thru 20 do (
  output_ss_fcl_std(
    num_rounds, num_columns, num_rows, exp, final_round_b, box_tran, mc_tran),
  for seed : 1 thru 20 do (
    output_ss_random_pc_pair(
      seed,num_rounds,num_columns,num_rows,exp,final_round_b)))$
exit();
shell> for r in $(seq 1 20); do
  for k in $(seq 1 20); do
    AppendDimacs-O3-DNDEBUG ssaes_r${r}_c1_rw1_e8_f0.cnf ssaes_pcpair_r${r}_c1_rw1_e8_f0_s${k}.cnf > r${r}_keyfind.cnf;
  done;
done
     \endverbatim
     </li>
     <li> The "minimum" box translation:
     \verbatim
shell> mkdir ssaes_r1-20_c1_rw1_e8_f0_k1-1_aes_min_box_aes_mc_bidirectional
shell> cd ssaes_r1-20_c1_rw1_e8_f0_k1-1_aes_min_box_aes_mc_bidirectional
shell> oklib --maxima
num_rows : 1$
num_columns : 1$
exp : 8$
final_round_b : false$
box_tran : aes_small_box$
mc_tran : aes_mc_bidirectional$
for num_rounds : 1 thru 20 do (
  output_ss_fcl_std(
    num_rounds, num_columns, num_rows, exp, final_round_b, box_tran, mc_tran),
  for seed : 1 thru 20 do (
    output_ss_random_pc_pair(
      seed,num_rounds,num_columns,num_rows,exp,final_round_b)))$
exit();
shell> for r in $(seq 1 20); do
  for k in $(seq 1 20); do
    AppendDimacs-O3-DNDEBUG ssaes_r${r}_c1_rw1_e8_f0.cnf ssaes_pcpair_r${r}_c1_rw1_e8_f0_s${k}.cnf > r${r}_keyfind.cnf;
  done;
done
     \endverbatim
     </li>
    </ul>
   </li>
  </ul>


  \todo Minisat-2.2.0
  <ul>
   <li> Solving the key discovery problem over rounds 1 to 20. </li>
   <li>
   <li> The environment for all of these experiments is:
   \verbatim
processor	: 0
vendor_id	: GenuineIntel
cpu family	: 6
model		: 23
model name	: Intel(R) Core(TM)2 Duo CPU     E8400  @ 3.00GHz
cpu MHz		: 2003.000
cache size	: 6144 KB
bogomips	: 5999.94

processor	: 1
vendor_id	: GenuineIntel
cpu family	: 6
model		: 23
model name	: Intel(R) Core(TM)2 Duo CPU     E8400  @ 3.00GHz
cpu MHz		: 2003.000
cache size	: 6144 KB
bogomips	: 5999.64
address sizes	: 36 bits physical, 48 bits virtual
             total       used       free     shared    buffers     cached
Mem:          3947       3363        583          0        165       3077
   \endverbatim
   </li>
   <li> Solving for twenty random plaintext/key/ciphertext combinations:
    <ul>
     <li> The canonical box translation:
      <ul>
       <li> The data:
       \verbatim
> cd ssaes_r1-20_c1_rw1_e8_f0_k1-1_aes_ts_box_aes_mc_bidirectional
> for r in $(seq 1 20); do for s in $(seq 1 20); do RunMinisat r${r}_k${s}.cnf; done; done
> (ExtractMinisat header-only |  awk " { print \$0 \" r s\"}"; for r in $(seq 1 20); do for s in $(seq 1 20); do
    cat ExperimentMinisat_r${r}_k${s}cnf_*/Statistics | tail -n 1 | awk " { print \$0 \" ${r} ${s}\"}";
  done;
done) > MinisatStatistics
> oklib --R
R> E = read.table("MinisatStatistics", header=TRUE)
R> aggregate(E, by=list(r=E$r), FUN=mean)
 r    rn     rc          t    cfs     dec  rts         r1 mem  ptime  stime       cfl
 1   584   8818 0.06248990 619.75 7985.35 4.70   66642.25   8 0.0015 0.0100 136724.60
 2  1144  17604 0.05849070 353.40 4523.20 3.20   77675.80   9 0.0090 0.0200  99964.50
 3  1704  26390 0.07463815 355.60 5913.25 3.25  126474.30  10 0.0110 0.0300 107089.15
 4  2264  35176 0.08748630 295.30 5017.00 2.90  159009.75  10 0.0100 0.0400  83223.95
 5  2824  43962 0.11273240 318.30 3703.65 3.05  261156.15  11 0.0175 0.0500  74814.65
 6  3384  52748 0.12038125 254.40 2689.30 2.70  234441.20  11 0.0210 0.0600  69354.50
 7  3944  61534 0.14182790 267.95 2715.60 2.55  320683.20  12 0.0230 0.0700  60043.00
 8  4504  70320 0.15902545 263.50 3236.50 2.65  346247.90  13 0.0300 0.0800  69703.75
 9  5064  79106 0.19741950 364.25 4968.40 3.35  569927.40  13 0.0290 0.0900  82017.75
10  5624  87892 0.20406850 312.35 3101.05 3.00  501814.75  15 0.0340 0.1000  73197.55
11  6184  96678 0.24321240 408.45 4537.00 3.70  664302.50  15 0.0380 0.1100 106804.20
12  6744 105464 0.24731200 347.05 3976.95 3.20  626600.40  15 0.0405 0.1200  90446.20
13  7304 114250 0.26800875 300.55 4771.80 3.00  690614.00  16 0.0435 0.1320  82817.85
14  7864 123036 0.31620150 473.65 6008.85 4.10  956348.35  17 0.0475 0.1425 117140.40
15  8424 131822 0.31390180 405.25 4116.20 3.55  825497.65  17 0.0500 0.1575 103677.40
16  8984 140608 0.32095065 408.60 4790.05 3.55  752915.85  19 0.0550 0.1685 105253.10
17  9544 149394 0.34899640 283.60 3474.90 2.90  974357.95  19 0.0595 0.1800  55337.75
18 10104 158180 0.36639390 349.05 4058.65 3.20  969733.95  20 0.0605 0.1900  86908.35
19 10664 166966 0.42628470 474.10 7317.55 3.95 1342253.95  20 0.0630 0.2000 109594.85
20 11224 175752 0.43908275 450.45 5056.25 3.80 1296728.75  20 0.0670 0.2100 114093.75
R> aggregate_statistics(E[c("r","t","cfs","r1")], by=list("r"))
 r     t.mean       t.sd    t.min    t.max cfs.mean   cfs.sd cfs.min cfs.max    r1.mean     r1.sd r1.min  r1.max
 1 0.06248990 0.05466919 0.011998 0.180972   619.75 623.4099       3    1743   66642.25  71833.26    771  214121
 2 0.05849070 0.03080966 0.025996 0.148977   353.40 330.4118       4    1265   77675.80  74372.44   2304  286786
 3 0.07463815 0.02156737 0.039993 0.133979   355.60 233.1203      29    1132  126474.30  73737.88  18225  320053
 4 0.08748630 0.03252278 0.051992 0.139978   295.30 289.8039       2     785  159009.75 145946.78   5385  442558
 5 0.11273240 0.03806041 0.066989 0.192970   318.30 277.7144       1     911  261156.15 198013.05   4969  634678
 6 0.12038125 0.03482124 0.082987 0.201969   254.40 246.7555      10     854  234441.20 188730.38  14156  586744
 7 0.14182790 0.04344615 0.094985 0.232964   267.95 311.1052       0     940  320683.20 253652.17   3880  782076
 8 0.15902545 0.04025407 0.110983 0.269958   263.50 269.2548       4    1108  346247.90 228189.13  15483  811490
 9 0.19741950 0.05206151 0.128980 0.318951   364.25 308.0027      18    1098  569927.40 353067.48  66034 1280961
10 0.20406850 0.05544938 0.138978 0.332949   312.35 298.8107       8    1029  501814.75 374699.34  38068 1286465
11 0.24321240 0.07140769 0.153976 0.371943   408.45 338.0367       3     939  664302.50 470113.99  22027 1474055
12 0.24731200 0.06288074 0.170974 0.377942   347.05 333.5223      12    1389  626600.40 457183.48  75278 1456066
13 0.26800875 0.06605416 0.190970 0.404938   300.55 248.4344      21     780  690614.00 469292.06  87094 1749341
14 0.31620150 0.08193738 0.202969 0.531919   473.65 411.2749      17    1784  956348.35 550983.73 103493 1953139
15 0.31390180 0.08710774 0.210967 0.569913   405.25 498.2229       5    2177  825497.65 549192.55  38310 1830072
16 0.32095065 0.09979992 0.224965 0.587910   408.60 574.5044      12    2490  752915.85 625888.11  58981 2258676
17 0.34899640 0.07747342 0.243962 0.510922   283.60 262.6930       6     786  974357.95 595401.22  60440 2111769
18 0.36639390 0.07150749 0.261960 0.500923   349.05 275.0917      58     955  969733.95 554552.85 136066 2055028
19 0.42628470 0.10963220 0.269958 0.625904   474.10 418.8635       1    1365 1342253.95 874091.62  20569 3222708
20 0.43908275 0.12029414 0.295955 0.697893   450.45 422.8833      17    1409 1296728.75 874229.90 152888 2881355
       \endverbatim
       </li>
      </ul>
     </li>
     <li> The 1-base box translation:
      <ul>
       <li> The data:
       \verbatim
> cd ssaes_r1-20_c1_rw1_e8_f0_k1-1_aes_1base_box_aes_mc_bidirectional
> for r in $(seq 1 20); do for s in $(seq 1 20); do RunMinisat r${r}_k${s}.cnf; done; done
> (ExtractMinisat header-only |  awk " { print \$0 \" r s\"}"; for r in $(seq 1 20); do for s in $(seq 1 20); do
    cat ExperimentMinisat_r${r}_k${s}cnf_*/Statistics | tail -n 1 | awk " { print \$0 \" ${r} ${s}\"}";
  done;
done) > MinisatStatistics
> oklib --R
R> E = read.table("MinisatStatistics", header=TRUE)
R> aggregate(E, by=list(r=E$r), FUN=mean)
 r  rn     rc         t      cfs      dec   rts        r1   mem  ptime  stime       cfl
 1  72   8908 0.2738579    48.20    54.00  1.05    495.45  9.00 0.0055 0.2685    225.55
 2 120  17784 0.5686132   210.85   223.15  2.45   5017.75 10.00 0.0100 0.5385   1355.30
 3 168  26660 0.8324729   197.80   212.10  2.20   7352.45 10.00 0.0155 0.7800   1413.85
 4 216  35536 1.1292285   276.90   305.10  2.90  12354.35 11.00 0.0200 1.0500   2154.65
 5 264  44412 1.4228825   347.35   410.15  3.40  16836.70 12.00 0.0255 1.3200   2908.95
 6 312  53288 1.7830780   496.90   626.45  4.20  22975.85 13.00 0.0295 1.6500   5043.30
 7 360  62164 2.1331750   894.15  1168.15  6.75  45545.15 14.00 0.0350 1.8995   9365.70
 8 408  71040 2.4001345   881.10  1226.65  6.70  44764.35 14.00 0.0395 2.1690  10072.70
 9 456  79916 2.8214710  1507.90  2064.25  9.60  80738.40 16.00 0.0460 2.4350  17878.95
10 504  88792 3.3660370  2420.55  3295.05 14.05 128229.05 17.00 0.0510 2.7655  29831.05
11 552  97668 4.0328870  4230.20  5750.70 21.25 224842.40 17.00 0.0540 3.0100  53687.70
12 600 106544 4.0676805  3035.50  4228.45 15.80 165612.05 18.00 0.0610 3.2810  38397.75
13 648 115420 4.5645555  4136.65  5842.10 21.15 217903.25 18.00 0.0650 3.5500  53801.05
14 696 124296 5.1477175  4723.00  6652.00 22.20 266074.50 19.05 0.0700 3.8800  61283.85
15 744 133172 5.4367225  4955.15  7061.10 23.85 275479.05 25.00 0.0750 4.1300  65962.60
16 792 142048 6.3774825  6798.00  9392.65 29.90 406102.65 25.00 0.0810 4.4000  90812.95
17 840 150924 6.6658340  6913.55  9787.85 30.55 411150.20 26.00 0.0840 4.6700  92348.70
18 888 159800 8.3613785 11809.15 16343.40 48.15 689233.45 26.00 0.0895 5.0000 154956.90
19 936 168676 7.8849535  8666.85 12345.95 37.25 532861.80 27.00 0.0940 5.2500 119138.20
20 984 177552 8.5457525  9953.05 14232.85 40.90 598177.20 27.00 0.1015 5.5200 133165.20
R> aggregate_statistics(E[c("r","t","cfs","r1")], by=list("r"))
 r    t.mean        t.sd    t.min     t.max cfs.mean     cfs.sd cfs.min cfs.max   r1.mean       r1.sd r1.min  r1.max
 1 0.2738579 0.003092788 0.267959  0.278957    48.20   31.88895       3     117    495.45    287.1078     75    1151
 2 0.5686132 0.012387026 0.549916  0.589910   210.85  112.73876      22     402   5017.75   2855.2685    439    9485
 3 0.8324729 0.026898047 0.800878  0.881865   197.80  139.32114      31     450   7352.45   5604.7990   1163   17614
 4 1.1292285 0.037412695 1.073840  1.214810   276.90  180.03359       1     702  12354.35   8013.5737    178   31372
 5 1.4228825 0.061113566 1.346790  1.545760   347.35  249.00714      26     878  16836.70  12761.8468   1387   42961
 6 1.7830780 0.077793796 1.685740  1.951700   496.90  366.82333       9    1334  22975.85  17802.8583    770   62284
 7 2.1331750 0.145534610 1.949700  2.430630   894.15  663.18524      65    2178  45545.15  32876.1557   3338  113943
 8 2.4001345 0.149741447 2.206660  2.658600   881.10  650.14742       8    1964  44764.35  34103.2547    709  103364
 9 2.8214710 0.225499194 2.485620  3.281500  1507.90  960.68845       6    3226  80738.40  53381.9063    677  185631
10 3.3660370 0.384480140 2.867560  3.953400  2420.55 1652.71491     189    4956 128229.05  87813.7824  10125  259206
11 4.0328870 0.457978784 3.106530  4.879260  4230.20 1968.06955     192    8110 224842.40 103637.0270   8796  415669
12 4.0676805 0.668839219 3.394480  5.522160  3035.50 2660.37739     218    9323 165612.05 148943.8993  10388  477512
13 4.5645555 0.577056536 3.640450  5.714130  4136.65 2406.31584     122    8799 217903.25 128664.9063   5208  457067
14 5.1477175 0.991527267 4.019390  7.057930  4723.00 3612.84561     213   11267 266074.50 215094.5537  14018  673621
15 5.4367225 0.771293229 4.420330  7.083920  4955.15 2851.36673     807   10582 275479.05 166504.2423  47933  636748
16 6.3774825 1.820517735 4.484320 11.413300  6798.00 5695.44211       5   19251 406102.65 369622.5541    669 1363491
17 6.6658340 1.754156071 4.760280 10.246400  6913.55 5951.83698       6   19769 411150.20 362778.8763    707 1138326
18 8.3613785 1.713180643 5.936100 12.224100 11809.15 5025.95138    4023   21520 689233.45 330197.2532 191676 1393708
19 7.8849535 2.255907246 5.347190 14.508800  8666.85 6232.09954       4   22802 532861.80 441039.7359    784 1745812
20 8.5457525 2.830277660 5.650140 15.245700  9953.05 8412.85588     139   27440 598177.20 543940.1920   5863 1839270
       \endverbatim
       </li>
      </ul>
     </li>
     <li> The minimum box translation:
      <ul>
       <li> The data:
       \verbatim
> cd ssaes_r1-20_c1_rw1_e8_f0_k1-1_aes_min_box_aes_mc_bidirectional
> for r in $(seq 1 20); do for s in $(seq 1 20); do RunMinisat r${r}_k${s}.cnf; done; done
> (ExtractMinisat header-only |  awk " { print \$0 \" r s\"}"; for r in $(seq 1 20); do for s in $(seq 1 20); do
    cat ExperimentMinisat_r${r}_k${s}cnf_*/Statistics | tail -n 1 | awk " { print \$0 \" ${r} ${s}\"}";
  done;
done) > MinisatStatistics
> oklib --R
R> E = read.table("MinisatStatistics", header=TRUE)
R> aggregate(E, by=list(r=E$r), FUN=mean)
 r  rn    rc           t       cfs        dec     rts          r1   mem  ptime  stime         cfl
 1  72   700  0.00524875    207.25     226.25    2.45     1812.50  8.00 0.0000 0.0000     1417.20
 2 120  1368  0.01899655   1838.85    2149.15   11.05    17954.30  8.00 0.0000 0.0070    17729.60
 3 168  2036  0.03799375   3888.75    5055.20   19.50    41573.25  8.00 0.0000 0.0095    42114.35
 4 216  2704  0.11658185  13218.05   18113.55   49.10   149330.20  8.00 0.0000 0.0100   152706.70
 5 264  3372  0.44103245  50036.35   67245.40  147.95   573506.70  8.00 0.0000 0.0100   601322.60
 6 312  4040  0.91845955  98412.45  133016.45  264.00  1164225.85  8.00 0.0005 0.0115  1210243.35
 7 360  4708  1.18651965 123257.15  169503.60  317.60  1456147.50  8.00 0.0015 0.0190  1541567.20
 8 408  5376  1.22536300 125262.20  175875.25  327.85  1492935.90  8.00 0.0035 0.0200  1587664.70
 9 456  6044  2.09357940 200843.60  282771.70  464.35  2430662.90  8.10 0.0040 0.0200  2550639.70
10 504  6712  2.10237940 201155.50  288352.80  480.00  2428389.85  8.10 0.0060 0.0205  2567895.40
11 552  7380  2.79842290 254392.30  366817.45  579.80  3112054.10  9.25 0.0060 0.0255  3276301.05
12 600  8048  1.98659905 187125.60  276103.95  455.75  2280804.55  9.10 0.0065 0.0295  2416003.10
13 648  8716  1.92245745 175830.95  262127.20  432.95  2170045.85  9.20 0.0085 0.0300  2281911.80
14 696  9384  5.08807930 414492.00  608811.85  866.45  5209658.35 10.00 0.0090 0.0300  5399250.55
15 744 10052 10.45975140 801543.10 1173975.75 1582.20 10178961.30 11.30 0.0085 0.0300 10479143.85
16 792 10720  3.68363910 317292.20  481174.25  720.00  3972314.00  9.90 0.0080 0.0375  4136374.25
17 840 11388  5.22720500 439184.20  668375.10  959.75  5522705.65 10.90 0.0095 0.0400  5748162.75
18 888 12056 12.51028980 923446.15 1386182.00 1828.50 11878627.95 12.90 0.0100 0.0400 12141267.75
19 936 12724 11.78302075 873229.15 1319434.05 1711.05 11274412.80 13.35 0.0100 0.0400 11499679.75
20 984 13392  6.65289110 516871.75  800851.10 1089.65  6668056.80 12.30 0.0100 0.0480  6787001.90
R> aggregate_statistics(E[c("r","t","cfs","r1")], by=list("r"))
 r      t.mean        t.sd    t.min     t.max  cfs.mean       cfs.sd cfs.min cfs.max     r1.mean        r1.sd r1.min   r1.max
 1  0.00524875  0.00151707 0.002999  0.008998    207.25     116.3094      10     517     1812.50     1032.995    125     4620
 2  0.01899655  0.00847150 0.007998  0.036994   1838.85    1241.9656     405    4366    17954.30    11983.426   3982    43097
 3  0.03799375  0.02292435 0.011998  0.090986   3888.75    3083.2961     377   10880    41573.25    33048.577   3942   115995
 4  0.11658185  0.15656933 0.010998  0.713891  13218.05   19099.1632      64   85605   149330.20   215120.819    785   964958
 5  0.44103245  0.62242919 0.052991  2.572610  50036.35   68828.6829    4955  279814   573506.70   794127.766  55518  3239349
 6  0.91845955  1.08287976 0.025996  4.475320  98412.45  109946.1634     975  448545  1164225.85  1313744.631  12178  5359059
 7  1.18651965  1.50572494 0.037994  5.924100 123257.15  149280.7331    2141  582733  1456147.50  1767148.782  26611  6896010
 8  1.22536300  1.36171219 0.055991  6.264050 125262.20  127872.1464    4283  584098  1492935.90  1569503.856  52129  7220864
 9  2.09357940  2.89803292 0.069989 12.956000 200843.60  255195.6822    5326 1133462  2430662.90  3114337.870  66136 13855056
10  2.10237940  2.22520893 0.060990  7.385880 201155.50  202440.7271    4195  679880  2428389.85  2449483.291  50601  8207024
11  2.79842290  3.76182050 0.063990 14.963700 254392.30  320751.6863    4510 1259796  3112054.10  3932869.397  55582 15425490
12  1.98659905  2.33011619 0.107983 10.684400 187125.60  202322.8128    8948  919729  2280804.55  2482618.016 110090 11323511
13  1.92245745  2.47283612 0.120981  9.492560 175830.95  210246.2887   10610  784546  2170045.85  2606380.697 126862  9804456
14  5.08807930  8.19702446 0.204968 36.365500 414492.00  578773.8763   19633 2537463  5209658.35  7472036.747 234823 32889731
15 10.45975140 13.93612656 0.144977 45.486100 801543.10  982199.3664   11759 3283845 10178961.30 12620561.130 151285 41641726
16  3.68363910  4.61291015 0.367944 22.178600 317292.20  339224.0375   35578 1645220  3972314.00  4348765.290 443762 21091938
17  5.22720500  4.36141744 0.355945 17.637300 439184.20  341710.5789   34207 1374364  5522705.65  4294686.038 433948 17168233
18 12.51028980 15.44997107 0.323950 59.759900 923446.15 1018492.0577   29966 3951340 11878627.95 13322497.164 389323 51702761
19 11.78302075 13.16968492 0.128980 46.689900 873229.15  883915.2234    8819 3111177 11274412.80 11591712.691 116641 41043886
20  6.65289110  6.68448439 0.238963 27.066900 516871.75  463554.0700   21581 1886822  6668056.80  6055048.220 270478 24653191
       \endverbatim
       </li>
      </ul>
     </li>
    </ul>
   </li>
   <li> Solving for one random plaintext/key/ciphertext combination:
    <ul>
     <li> The canonical box translation:
      <ul>
       <li> The data:
       \verbatim
shell> for r in $(seq 1 20); do minisat-2.2.0 r${r}_keyfind.cnf > minisat_r${r}.result 2>&1; done
shell> echo "n  c  t  sat  cfs dec rts r1 mem ptime stime cfl" > minisat_results
shell> for r in $(seq 1 20); do cat minisat_r${r}.result | awk -f $OKlib/Experimentation/ExperimentSystem/SolverMonitoring/ExtractMinisat.awk; done >> minisat_results
shell> oklib --R
R> E = read.table("minisat_results", header=TRUE)
R> E
   t  cfs   dec rts      r1 mem ptime stime     cfl
0.06  409  2107   4   46162  19  0.00  0.04   90629
0.20 1436 11129  10  306463  20  0.00  0.07  472603
0.20  696  7797   6  348743  21  0.00  0.11  162406
0.17  102   739   2   63456  21  0.01  0.15   30154
0.27  478  4035   4  399887  22  0.02  0.18  119983
0.81 2940 42135  15 1803445  26  0.01  0.24 1277362
0.31  139  2979   2   85761  23  0.02  0.27   60933
0.34  110  3842   2   38974  24  0.02  0.31   42998
0.49  600  4343   5  796689  25  0.02  0.35  162990
0.53  753 14647   6  425323  26  0.02  0.39  363891
0.68  643  5826   6 1383777  26  0.03  0.44  190498
0.61  440 10654   4  541434  26  0.04  0.47  165969
0.69  796  9559   6  746193  27  0.04  0.51  266522
0.80  532  4294   5 1502391  29  0.04  0.55   97600
0.76  740 10193   6  681178  30  0.04  0.60  255280
0.71  225  4370   3  300923  30  0.03  0.63  107865
0.73   30   412   1  125556  30  0.03  0.68   14478
0.90  255  3075   3  855796  31  0.06  0.72   93811
1.15 1140 14137   7 1969980  31  0.07  0.75  346361
1.17  631  2511   6 2285569  32  0.07  0.80  127148
       \endverbatim
       </li>
       <li> The number of conflicts is sporadic but always less than 3000. </li>
       <li> The "minimum" translation needs anywhere from 2-1000 times this!
       </li>
      </ul>
     </li>
     <li> The "minimum" box translation:
      <ul>
       <li> The data:
       \verbatim
shell> for r in $(seq 1 20); do minisat-2.2.0 r${r}_keyfind.cnf > minisat_r${r}.result 2>&1; done
shell> echo "n  c  t  sat  cfs dec rts r1 mem ptime stime cfl" > minisat_results
shell> for r in $(seq 1 20); do cat minisat_r${r}.result | awk -f $OKlib/Experimentation/ExperimentSystem/SolverMonitoring/ExtractMinisat.awk; done >> minisat_results
shell> oklib --R
R> E = read.table("minisat_results", header=TRUE)
R> E
    t     cfs     dec  rts       r1 mem  stime      cfl
 0.00     517     547    5     4620  18   0.00     3081
 0.02    4366    5097   22    43097  19   0.00    40306
 0.07   10880   14117   46   115995  19   0.00   116827
 0.21   30859   41750  112   343798  19   0.00   353791
 0.04    6559    9038   31    74037  19   0.00    80695
 0.97  117049  159563  317  1388453  19   0.01  1462206
 0.64   78912  111171  252   937577  19   0.01  1000078
 0.15   18532   28638   63   220023  19   0.01   226702
 0.41   47272   67466  141   575478  19   0.01   599580
 0.62   72342  108465  223   871012  19   0.01   934208
 3.81  401564  587313  978  4884099  20   0.01  5208548
 0.94  103914  155092  264  1270577  19   0.02  1328705
 1.73  188027  283150  509  2287632  19   0.02  2434198
 1.56  161699  251844  443  2000257  20   0.03  2099626
23.76 1929716 2823253 3768 24701055  27   0.03 25268546
 1.29  136359  216718  380  1704360  20   0.02  1762894
 0.43   46432   75897  137   575767  20   0.03   619246
 7.81  702076 1068512 1532  8812093  25   0.03  9173558
37.32 2766520 4105833 4842 36254116  32   0.04 36572031
 6.83  590138  895574 1273  7513499  27   0.04  7665893
       \endverbatim
       </li>
      </ul>
     </li>
    </ul>
   </li>
  </ul>


  \todo precosat236
  <ul>
   <li> Solving for rounds 1 to 20. </li>
   <li> The canonical box translation:
    <ul>
     <li> The data:
     \verbatim
shell> for r in $(seq 1 20); do precosat236 r${r}_keyfind.cnf > precosat236_r${r}.result 2>&1; done
shell> echo "n  c  t  sat  cfs  dec  rts  r1  mem rnd  its  skip  enl  shk  resc  rebi  simp red  nfix  neq  nel  nmg  sb  sbn  sba  sbx  sbi  ar  arx  pb  pbph  pbr  pbf  pblf  pbmg sccnt  sccf  sccm  hshu  hshm  minln  mindel minst  mind  subf  subb  subdm  strf  strb  dom domh  domlow  mpr  memr" > precosat236_results
shell> for r in $(seq 1 20); do cat precosat236_r${r}.result | awk -f $OKlib/Experimentation/ExperimentSystem/SolverMonitoring/ExtractPrecosat236.awk; done >> precosat236_results
shell> oklib --R
R> E = read.table("precosat236_results", header=TRUE)
R> options(width=1000)
R> E
   t       r1 mem rnd its skip enl shk resc rebi simp red  nfix neq nel nmg sb sbn sba sbx sbi   ar arx   pb pbph pbr  pbf pblf pbmg sccnt sccf sccm hshu hshm minln mindel minst mind subf subb subdm strf strb  dom domh domlow  mpr memr
 0.0    97536   1   0   0    0   0   0    0    0    1   0   562  21   1  40  0   0   0   0   0 0.00   0  278    1   2  254    0    3     0    0    0    0   37     0      0     0    0    0    0    22    0    0   38    0      0 9.75    0
 0.0   244771   2   0   0    0   0   0    0    0    1   0  1144   0   0  48  0   0   0   0   0 0.00   0  305    1   2  255    0    0     0    0    0    0   48     0      0     0    0    0    0    16    0    0  112    0      0 6.12    1
 0.1   392820   3   0   0    0   0   0    0    0    1   0  1704   0   0  67  0   0   0   0   0 0.00   0  337    1   2  255    0    0     0    0    0    0   67     0      0     0    0    0    0    16    0    0  206    0      0 5.61    1
 0.1   591456   4   0   0    0   0  12    0    0    1   0  2109 152   3 169 33   0 100   0   0 2.00   0  552    1   2  406  853   70     0    0    0    0   99     0      0     0    0   12  364    31    0    2  461    0      0 6.57    2
 0.1   688963   5   0   0    0   0   0    0    0    1   0  2824   0   0  99  0   0   0   0   0 0.00   0  401    1   2  255    0    0     0    0    0    0   99     0      0     0    0    0    0    16    0    0  478    0      0 5.74    2
 0.1   837026   6   0   0    0   0   0    0    0    1   0  3384   0   0 115  0   0   0   0   0 0.00   0  433    1   2  255    0    0     0    0    0    0  115     0      0     0    0    0    0    16    0    0  591    0      0 5.58    3
 0.2  1010721   7   0   0    0   0   0    0    0    1   0  3765 178   1 239  0   0   0   0   0 0.00   0  484    1   2  268 2640  102     0    0    0    0  137     0      0     0    0    0    0    23    0    0  733    0      0 5.62    3
 0.2  1385680   8   0   0    0   0  11    0    0    1   0  4213 288   3 324 33   0 100   0   0 2.00   0  933    1   2  625 2339  141     0    0    0    0  183     0      0     0    0   11  704    38    0    2  990    0      0 5.77    4
 0.2  1281219   9   0   0    0   0   0    0    0    1   0  5064   0   0 163  0   0   0   0   0 0.00   0  529    1   2  255    0    0     0    0    0    0  163     0      0     0    0    0    0    16    0    0  929    0      0 5.34    4
 0.4  2524166  10   0   0    0   0  52    0    0    1   0  5187 430   7 450 71   0  80   0  20 2.25   0 2469    1   2 2001 1928  178     0    0    0    0  272     0      0     0    0   71 1584    26    0   23 1293    0      0 5.87    5
 0.3  1577347  13   0   0    0   0   0    0    0    1   0  6184   0   0 195  0   0   0   0   0 0.00   0  593    1   2  255    0    0     0    0    0    0  195     0      0     0    0    0    0    16    0    0 1173    0      0 5.09    5
 0.3  1725411  13   0   0    0   0   0    0    0    1   0  6744   0   0 211  0   0   0   0   0 0.00   0  625    1   2  255    0    0     0    0    0    0  211     0      0     0    0    0    0    16    0    0 1279    0      0 5.07    5
 0.5  2548532  14   0   0    0   0  11    0    0    1   0  6814 487   3 533 33   0 100   0   0 2.00   0 1535    1   2 1017 4152  232     0    0    0    1  301     0      0     0    0   11 1239    56    0    2 1671    0      0 5.10    6
 0.4  2021539  14   0   0    0   0   0    0    0    1   0  7864   0   0 243  0   0   0   0   0 0.00   0  689    1   2  255    0    0     0    0    0    0  243     0      0     0    0    0    0    16    0    0 1522    0      0 4.93    6
 0.4  2169603  15   0   0    0   0   0    0    0    1   0  8424   0   0 259  0   0   0   0   0 0.00   0  721    1   2  255    0    0     0    0    0    0  259     0      0     0    0    0    0    16    0    0 1621    0      0 5.05    7
 0.6  3102031  16   0   0    0   0  10    0    0    1   0  8396 585   3 651 33   0 100   0   0 2.00   0 1707    1   2 1071 4814  285     0    0    0    2  366     0      0     0    0   10 1482    56    0    2 2208    0      0 5.00    7
 0.9  5105072  18   0   0    0   0  62    0    0    1   0  8797 740   7 773 71   0  80   0  20 2.25   0 4204    1   2 3412 3628  332     0    0    0    0  441     0      0     0    0   80 2771    33    0   21 2380    0      0 5.49    8
 0.5  2632345  18   0   0    0   0   0    0    0    1   0  9704 399   1 571  0   0   0   0   0 0.00   0  823    1   2  255 8407  260     0    0    0    0  311     0      0     0    0    0    0    19    0    0 2068    0      0 4.87    8
 0.6  2761859  18   0   0    0   0   0    0    0    1   0 10664   0   0 323  0   0   0   0   0 0.00   0  849    1   2  255    0    0     0    0    0    0  323     0      0     0    0    0    0    16    0    0 2093    0      0 4.85    8
 0.6  2909923  19   0   0    0   0   0    0    0    1   0 11224   0   0 339  0   0   0   0   0 0.00   0  881    1   2  255    0    0     0    0    0    0  339     0      0     0    0    0    0    16    0    0 2226    0      0 4.77    9
     \endverbatim
     </li>
     <li> All solved in "0" time. </li>
     <li> precosat236 uses no conflicts or decisions! </li>
     <li> Note that minimum translation has uses from
     ~300 to ~500000 conflicts! </li>
     <li> The number of r1 reductions grows linearly. </li>
    </ul>
   </li>
   <li> The "minimum" box translation:
    <ul>
     <li> The data:
     \verbatim
shell> for r in $(seq 1 20); do precosat236 r${r}_keyfind.cnf > precosat236_r${r}.result 2>&1; done
shell> echo "n  c  t  sat  cfs  dec  rts  r1  mem rnd  its  skip  enl  shk  resc  rebi  simp red  nfix  neq  nel  nmg  sb  sbn  sba  sbx  sbi  ar  arx  pb  pbph  pbr  pbf  pblf  pbmg sccnt  sccf  sccm  hshu  hshm  minln  mindel minst  mind  subf  subb  subdm  strf  strb  dom domh  domlow  mpr  memr" > precosat236_results
shell> for r in $(seq 1 20); do cat precosat236_r${r}.result | awk -f $OKlib/Experimentation/ExperimentSystem/SolverMonitoring/ExtractPrecosat236.awk; done >> precosat236_results
shell> oklib --R
R> E = read.table("precosat236_results", header=TRUE)
R> options(width=1000)
R> E
   t    cfs    dec rts      r1 mem rnd its skip enl shk resc rebi simp red nfix neq nmg   pb pbph pbr hshm   minln mindel minst mind subf subb subdm strf strb dom domh domlow  mpr memr
 0.0    276    295   2    1711   0   1   0    0   0  19    2    0    1   0   24  32  32   48    1   1   32    1635     24     0    3    5   19    32   18    2  32    0      0 0.00    0
 0.0   2480   2844   6   18523   0   1   1    9   4   1   24    2    1   0   33  48  48   96    1   1   48   21310     17     5   11    0    0    37    0    0  37    0      5 0.93    0
 0.0    855   1188   6    7057   0   1   0    0   0   0    8    0    1   0   40  64  64  144    1   1   64    8951      7     0   10    0    0    32    0    0  32    0      0 0.71    0
 0.0   4491   5867   6   38385   1   1   0   16   3   0   43    3    1   0   48  80  80  192    1   1   80   47635     11     2   15    0    0    32    0    0  32    0      0 0.77    0
 0.2  18051  23205   6  170094   1   1   0   56   8   1  176   12    2   0   56  96  96  240    1   1   96  203950      9     0   21    0    0    32    0    0  32    0      0 0.81    1
 0.2  19330  25413   5  164876   1   1   0   58   8   0  188   12    1   0   64 112 112  288    1   1  112  224227      7     1   29    0    0    32    0    0  32    0      0 0.72    1
 3.5 158902 189017   6 1589271   4   1   0  423  17   1 1553   61    2   0   72 128 128  656    2   2  128 1861754      7     8   35    0    0    32    0    0  32    0      0 0.45    9
 4.4 189729 236219   6 2173889   4   1   1  530  17   3 1854   62    3   0   81 145 145  752    2   2  145 2287121      7    15   48    0    0   321    0    0 321  273     16 0.49   11
19.6 559607 679101   5 7055110   8   1   0 1158  21   1 5470  172    2   0   88 160 160  848    2   2  160 7337196      8    19   50    0    0    33    0    0  33    0      1 0.36   38
 3.1 162774 214606   6 1835747   5   2   0  437  15   2 1591   62    3   0   96 176 176  944    2   2  176 2000911      7     5   72    0    0    32    0    0  32    0      0 0.60   10
 0.6  45143  59876   5  464747   3   1   0  124   9   0  441   23    1   0  104 192 192  528    1   1  192  548797      6     3   71    0    0    32    0    0  32    0      0 0.75    2
13.4 373295 462385   6 4852180   7   2   0  886  18   2 3649  125    3   0  112 208 208 1696    3   3  208 4702888      6    24   94    0    0    32    0    0  32    0      0 0.36   24
 0.9  63778  96995   6  732899   4   2   0  183  10   0  623   29    1   0  120 224 224  624    1   1  224  840623      6     1   65    0    0    32    0    0  32    0      0 0.79    3
20.9 469354 585482   6 6709231  11   2   0 1016  18   1 4588  140    2   1  128 240 240 1984    3   3  240 6130316      7    14   98    0    0    32    0    0  32    0      0 0.32   31
 6.2 280785 400652   6 3426519   7   2   0  659  16   2 2746  100    3   0  136 257 257 2126    3   3  257 3754082      7    11  101    0    0    32    0    0  32    0      0 0.55   18
 0.1   8650  16578   6  107081   2   2   0   30   0   0   84    6    1   0  144 272 272  768    1   1  272  123666      5     2   74    0    0    32    0    0  32    0      0 0.76    0
 7.0 284158 389980   6 3399054   8   2   0  667  15   1 2778  101    2   1  152 288 288 1616    2   2  288 3651460      6    13   99    0    0    32    0    0  32    0      0 0.48   17
 6.3 217971 295925   6 2747044   8   2   0  504  14   1 2131   77    2   0  160 304 304 1712    2   2  304 2862254      7     6  120    0    0    32    0    0  32    0      0 0.44   13
31.1 564267 730122   6 9106872  14   2   0 1174  18   2 5516  176    3   0  168 320 320 2704    3   3  320 7428406      6    20  107    0    0    32    0    0  32    0      0 0.29   37
15.3 397872 539835   6 5316500  11   2   0  953  16   1 3890  126    2   0  176 336 336 1904    2   2  336 5211466      6    17  104    0    0    32    0    0  32    0      0 0.35   25
     \endverbatim
     </li>
     <li> The number of conflicts and decisions seem quadratic in the number
     of rounds. </li>
    </ul>
   </li>
  </ul>


  \todo OKsolver_2002
  <ul>
   <li> Solving for rounds 1 to 20. </li>
   <li> The canonical box translation:
    <ul>
     <li> The data:
     \verbatim
shell> for r in $(seq 1 20); do OKsolver_2002-O3-DNDEBUG r${r}_keyfind.cnf > oksolver_r${r}.result 2>&1; done
shell> echo "n  c  l  t  sat  nds  r1  r2  pls  ats h file n2cr  dmcl dn  dc  dl snds qnds mnds  tel  oats  n2cs  m2cs" > oksolver_results
shell> for r in $(seq 1 20); do cat oksolver_r${r}.result | awk -f $OKlib/Experimentation/ExperimentSystem/SolverMonitoring/ExtractOKsolver.awk; done >> oksolver_results
shell> oklib --R
R> E = read.table("oksolver_results", header=TRUE)
R> options(width=200);
R> E
  t  r1   r2 ats            file   n2cr dmcl  dn  dc   dl
0.0  24  254   1  r1_keyfind.cnf   8272    0  24  72  216
0.1  32  255   0  r2_keyfind.cnf  16512    0  32  96  288
0.3  40  255   0  r3_keyfind.cnf  24752    0  40 120  360
0.7  48 1032   1  r4_keyfind.cnf  32992    0  48 144  432
0.9  56  255   0  r5_keyfind.cnf  41232    0  56 168  504
1.2  64  255   0  r6_keyfind.cnf  49472    0  64 192  576
1.6  72  773   1  r7_keyfind.cnf  57712    0  72 216  648
2.1  80 2146   1  r8_keyfind.cnf  65952    0  80 240  720
1.9  88  255   0  r9_keyfind.cnf  74192    0  88 264  792
3.5  96 3692   1 r10_keyfind.cnf  82432    0  96 288  864
2.4 104  255   0 r11_keyfind.cnf  90672    0 104 312  936
2.7 112  255   0 r12_keyfind.cnf  98912    0 112 336 1008
3.8 120 3922   1 r13_keyfind.cnf 107152    0 120 360 1080
3.1 128  255   0 r14_keyfind.cnf 115392    0 128 384 1152
3.3 136  255   0 r15_keyfind.cnf 123632    0 136 408 1224
4.8 144 4304   1 r16_keyfind.cnf 131872    0 144 432 1296
7.8 152 5896   1 r17_keyfind.cnf 140112    0 152 456 1368
4.9 160 1682   1 r18_keyfind.cnf 148352    0 160 480 1440
4.2 168  255   0 r19_keyfind.cnf 156592    0 168 504 1512
4.5 176  255   0 r20_keyfind.cnf 164832    0 176 528 1584
     \endverbatim
     </li>
     <li> Note that the OKsolver uses no nodes to solve any of these instances.
     </li>
     <li> The height for all instances is 0. </li>
     <li> Why is the height 0 here but non-zero for the "minimum" translation,
     even though both use 0 nodes? </li>
     <li> The number of r1 reductions climbs by exactly 8 each time. </li>
    </ul>
   </li>
   <li> The "minimum" box translation:
    <ul>
     <li> The data:
     \verbatim
shell> for r in $(seq 1 20); do OKsolver_2002-O3-DNDEBUG r${r}_keyfind.cnf > oksolver_r${r}.result 2>&1; done
shell> echo "n  c  l  t  sat  nds  r1  r2  pls  ats h file n2cr  dmcl dn  dc  dl snds qnds mnds  tel  oats  n2cs  m2cs" > oksolver_results
shell> for r in $(seq 1 20); do cat oksolver_r${r}.result | awk -f $OKlib/Experimentation/ExperimentSystem/SolverMonitoring/ExtractOKsolver.awk; done >> oksolver_results
shell> oklib --R
R> E = read.table("oksolver_results", header=TRUE)
R> options(width=200);
R> E
  t   r1    r2   h n2cr  dn  dc   dl
0.0   24    26   7   80  24  72  216
0.0   32   924  10  128  32  96  288
0.1   40  1648  12  176  40 120  360
0.1   48  2402  13  224  48 144  432
0.2   56  3156  15  272  56 168  504
0.3   64  3957  17  320  64 192  576
0.1   72   759  17  368  72 216  648
0.5   80  5432  18  416  80 240  720
0.6   88  6174  19  464  88 264  792
0.5   96  4556  20  512  96 288  864
0.8  104  7775  20  560 104 312  936
1.0  112  8557  21  608 112 336 1008
1.1  120  9305  22  656 120 360 1080
1.3  128 10076  24  704 128 384 1152
1.5  136 10909  23  752 136 408 1224
0.3  144  1657  23  800 144 432 1296
1.9  152 12418  26  848 152 456 1368
2.1  160 13310  26  896 160 480 1440
2.4  168 14020  27  944 168 504 1512
2.6  176 14786  30  992 176 528 1584
     \endverbatim
     </li>
     <li> Note that the OKsolver uses no nodes to solve any of these instances.
     </li>
     <li> The number of r1 reductions climbs by exactly 8 each time (the same
     as the canonical translation). </li>
     <li> The number of r2 reductions climbs linearly, but in a sporadic
     manner. </li>
     <li> Note that the number of r2 reductions is comparable with the
     canonical translation in this instance. </li>
    </ul>
   </li>
  </ul>


  \todo march_pl
  <ul>
   <li> Solving for rounds 1 to 20. </li>
   <li> The canonical box translation:
    <ul>
     <li> The data:
     \verbatim
shell> for r in $(seq 1 20); do march_pl r${r}_keyfind.cnf > march_pl_r${r}.result 2>&1; done
shell> echo "n c t sat nds r1 r2" > march_pl_results
shell> for r in $(seq 1 20); do cat march_pl_r${r}.result | awk -f $OKlib/Experimentation/ExperimentSystem/SolverMonitoring/ExtractMarchpl.awk; done >> march_pl_results
shell> oklib --R
R> E = read.table("march_pl_results", header=TRUE)
R> E
     t nds    r1   r2
  7.41   3   560  519
 13.34   2  1112  257
 20.61   2  1664  257
 28.74   4  2216  850
 35.32   2  2768  257
 42.56   2  3320  257
 53.49   3  3872  542
 61.85   4  4424 1598
 64.36   2  4976  257
 84.07   4  5528 3768
 78.77   2  6080  257
 86.38   2  6632  257
106.72   4  7184 3184
101.01   2  7736  257
108.42   2  8288  257
134.41   4  8840 3543
156.20   4  9392 6672
156.82   3  9944 1075
137.40   2 10496  257
144.90   2 11048  257
     \endverbatim
     </li>
     <li> march_pl uses at most 4 nodes to solve these instances, but the
     number of nodes remains fairly constant. </li>
     <li> r1 = 552 * r + 8. What are these 552 variables? </li>
     <li> Time is related linearly to the number of rounds. However, the
     increase in time per round is significant. </li>
     <li> The canonical translation outperforms the "minimum" by a factor of
     20-1000x in terms of the number of nodes, but the "minimum" translation
     vastly outperforms the canonical here when it comes to time. </li>
    </ul>
   </li>
   <li> The "minimum" box translation:
    <ul>
     <li> The data:
     \verbatim
shell> for r in $(seq 1 20); do march_pl r${r}_keyfind.cnf > march_pl_r${r}.result 2>&1; done
shell> echo "n c t sat nds r1 r2" > march_pl_results
shell> for r in $(seq 1 20); do cat march_pl_r${r}.result | awk -f $OKlib/Experimentation/ExperimentSystem/SolverMonitoring/ExtractMarchpl.awk; done >> march_pl_results
shell> oklib --R
R> E = read.table("march_pl_results", header=TRUE)
R> E
   t  nds    r1     r2
0.01   56   134    745
0.07  289   864  12223
0.16  466  3505  28017
0.17  334  4134  23693
0.45  967 12848  79932
0.43  938 15454  53411
0.64 1411 25190  79915
0.28  434  7772  30387
1.19 2099 41058 210471
0.85 1200 23318 166197
2.23 3048 60262 542893
2.94 3660 71135 781462
0.64 1142 22686  60756
0.45  593 12163  39037
0.51  631 13473  49788
0.33  236  5315  21442
0.45  349  7866  55636
0.61  485 11571  99054
0.76  606 13200 143224
1.05  813 18504 249992
     \endverbatim
     </li>
    </ul>
   </li>
  </ul>


  \todo satz
  <ul>
   <li> Solving for rounds 1 to 20. </li>
   <li> The canonical box translation:
    <ul>
     <li> The data:
     \verbatim
shell> for r in $(seq 1 20); do satz215 r${r}_keyfind.cnf > satz_r${r}.result 2>&1; done
shell> echo "n  c  t  sat  nds  r1  pls  file  bck src  fix  dc  src2  fix2" > satz_results
shell> for r in $(seq 1 20); do cat satz_r${r}.result | awk -f $OKlib/Experimentation/ExperimentSystem/SolverMonitoring/ExtractSatz.awk; done >> satz_results
shell> oklib --R
R> E = read.table("satz_results", header=TRUE)
R> options(width=150)
R> E
      t     nds       r1    bck       src     fix  dc src2 fix2
  0.004       3       52      0        89       3  44    1    1
  0.005       2       98      0       137      10  64    8    3
  0.006       2      148      0       226       5  84    8    3
  0.009      24     1052     10      1807      29 104    3    0
  0.009       8      356      1      1055       5 124    3    0
  0.015     112     5291     54     11173     141 144    0    0
  0.022     221    10505    108     24791     279 164    0    0
  0.039     414    19546    204     52745     519 184    0    0
  0.080     826    39237    410    115163    1041 204    0    0
  0.038     286    12334    139     45217     355 224    0    0
  0.168    1635    73279    814    273538    2061 244    0    0
  0.694    6764   320281   3378   1233737    8473 264    0    0
  0.731    6851   306103   3422   1335270    8605 284    0    0
  3.049   26837  1269930  13414   5643741   33589 304    0    0
  5.591   47538  2236988  23765  10590324   59522 324    0    0
 13.287  106918  5057826  53454  25474627  133741 344    0    0
 24.498  189283  8896080  94637  47464186  236810 364    0    0
 57.987  428867 20296263 214428 114187055  536279 384    0    0
105.671  755389 35476989 377689 210566490  944654 404    0    0
254.204 1713789 81100021 856888 504283111 2142635 424    0    0
     \endverbatim
     </li>
     <li> The time, number of nodes and r1 reductions seem to grow
     exponentially!?!? </li>
     <li> Why is this solver so much worse than the rest? </li>
    </ul>
   </li>
   <li> The "minimum" box translation:
    <ul>
     <li> The data:
     \verbatim
shell> for r in $(seq 1 20); do satz215 r${r}_keyfind.cnf > satz_r${r}.result 2>&1; done
shell> echo "n  c  t  sat  nds  r1  pls  file  bck src  fix  dc  src2  fix2" > satz_results
shell> for r in $(seq 1 20); do cat satz_r${r}.result | awk -f $OKlib/Experimentation/ExperimentSystem/SolverMonitoring/ExtractSatz.awk; done >> satz_results
shell> oklib --R
R> E = read.table("satz_results", header=TRUE)
R> options(width=150)
R> E
    t nds   r1 bck  src fix  dc src2 fix2
0.003   2   20   0  106   3  40   22    1
0.003   2   40   0  132   7  56   11    4
0.004   8  204   3  361  37  72   92   21
0.004  10  313   4  433  45  88  101   21
0.004   5  144   1  447  19 104   88   10
0.003   4  117   0  368   9 120   28    5
0.003   3  134   0  419  27 136   50   20
0.004  11  587   5  756 108 152  204   69
0.006   6  369   2  714  84 168  135   60
0.006   3  192   0  550  33 184   39   23
0.006   3  209   0  609  53 200  109   40
0.007   8  567   3  845 129 216  180  101
0.007   4  309   2  973 110 232  276   91
0.009  15 1367   7 1517 191 248  288  112
0.008   7  812   3 1634  99 264  142   60
0.009   8  875   4 1589 151 280  191   92
0.008   4  327   1 1074  54 296  140   40
0.011  19 1929   9 2338 289 312  458  181
0.009   8  736   3 1277  92 328  214   52
0.010   6  706   2 1984  94 344  156   59
     \endverbatim
     </li>
     <li> The number of nodes are very sporadic but all less than 20. </li>
     <li> This is considerably better than in the case of the canonical
     translation where there was an exponential growth in the number of nodes
     over the number of rounds. </li>
    </ul>
   </li>
  </ul>

*/
