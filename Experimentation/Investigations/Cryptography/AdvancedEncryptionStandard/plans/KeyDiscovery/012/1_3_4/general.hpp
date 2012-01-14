// Matthew Gwynne, 17.11.2011 (Swansea)
/* Copyright 2011, 2012 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/Cryptography/AdvancedEncryptionStandard/plans/KeyDiscovery/012/1_3_4/general.hpp
  \brief Investigations into small-scale AES key discovery with 1 row, 3 column and 4-bit field elements


  \todo Problem specification
  <ul>
  <li> We consider the small-scale AES with 1 row, 3 columns, using the 4-bit
   field size for rounds 1 to 20. </li>
   <li> We denote this AES instance by aes(r,1,3,4) for r in 1,...,20. </li>
   <li> Thus aes(r,1,3,4) has 1*3*4=12-bit plaintext/key/ciphertext. </li>
   <li> As a reminder, aes(r,1,3,4) applies the following operations (up to
   variable-permutations):
    <ol>
     <li> Key schedule applies the following operations r+1 times:
      <ul>
       <li> r * 1 = r S-boxes (first column).
       <li> r * 1 * 4 * 2 = 8*r additions of arity 2 (last two columns). </li>
       <li> r * 1 * 4 = 4*r additions of arity 3 (first column). </li>
      </ul>
     generating r+1 12-bit round keys. </li>
     <li> Application of the following operation (the "round") r times:
      <ol>
       <li> Addition of 12-bit round key. </li>
       <li> Application of 3 4x4-bit Sbox operations. </li>
       <li> Mixcolumn is just the identity, namely application of
       3 (1*4)x(1*4)=4x4-bit Mixcolumn operations, given by the 1x1 matrix (1)
       over the half-byte field. </li>
      </ol>
     </li>
     <li> Addition of round key r+1. </li>
     <li> The result of the last round key addition is the ciphertext. </li>
    </ol>
   </li>
   <li> Round key 0 is the input key. </li>
   <li> The S-box is a permutation from {0,1}^4 to {0,1}^4 which we consider
   as either:
    <ul>
     <li> a 8x1 boolean function; see ss_sbox_bf in
     Cryptology/Lisp/CryptoSystems/Rijndael/AdvancedEncryptionStandard.mac.
     </li>
     <li> 4 4x1 boolean functions (not implemented yet). </li>
    </ul>
   </li>
   <li> The decompositions and translations are listed in "Investigating
   dimensions" in
   Cryptography/AdvancedEncryptionStandard/plans/Experimentation.hpp.
   </li>
   <li> The plaintext and ciphertext variables are then set, and the
   SAT solver is run on this instance to deduce the key variables. </li>
  </ul>


  \todo Instance characteristics
  <ul>
   <li> In this instance, we have:
    <ul>
     <li> r full rounds (Key Addition, SubBytes; MixColumns is the identity).
     </li>
     <li> 4*r Sboxes:
      <ul>
       <li> 3*r from SubBytes = 3 columns * r rounds; </li>
       <li> r from key schedule = 1 column * r round. </li>
      </ul>
     </li>
     <li> 52*r + 12 additions:
      <ul>
       <li> 24*r additions of arity 1 (equivalence clauses):
        <ul>
         <li> 12*r from forward MixColumns = 12 bits * r rounds; </li>
         <li> 12*r from inverse MixColumns = 12 bits * r rounds. </li>
        </ul>
       </li>
       <li> 20*r + 12 additions of arity 2:
        <ul>
         <li> 12*r from key additions = 12 bits * r round; </li>
         <li> 12 from final key addition = 12 bits; </li>
         <li> 8*r from the key schedule = 4 bits * 2 column * r rounds. </li>
        </ul>
       </li>
       <li> 4*r additions of arity 3:
        <ul>
         <li> 4*r from the key schedule = 4 bits * 1 column * r rounds. </li>
        </ul>
       </li>
      </ul>
     </li>
     <li> 4*r bits from the key schedule constant = 4 bits * r rounds. </li>
    </ul>
   </li>
   <li> Therefore, each clause-set has:
    <ul>
     <li> 4*r*(s+41) + 48 clauses:
     \verbatim
c : 4*r*s + 24*r*2 + (20*r+12)*4 + 4*r*8 + 4*r;
expand(simplify_t(c));
  4*r*s+164*r+48
factorout(expand(simplify_t(c)), s);
  = 4*r*(s+41)+48
     \endverbatim
     clauses.
     </li>
    <ul>
   </li>
   <li> Instantiating for the canonical translation, where now
   s=145:
   \verbatim
expand(ev(c, s:145));
 744*r+48
   \endverbatim
   Checking:
   \verbatim
> ExtendedDimacsFullStatistics-O3-DNDEBUG < ssaes_r20_c3_rw1_e4_f0.cnf
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
2436 14928 42784 0 42784 2437 1

> ncl_full_dualts(8,16);
  145
> ev(c, s:145,r:20);
  14928
   \endverbatim
   </li>
   <li> Instantiating for the 1-base translation XXX </li>
   <li> Instantiating for the minimum translation XXX </li>
   <li> XXX Box sizes. </li>
  </ul>


  \todo Translations
  <ul>
   <li> The following translations are considered in this %plans %file:
    <ul>
     <li> The canonical box translation. </li>
     <li> The minimum box translation. </li>
     <li> The 1-base translation. </li>
    </ul>
   </li>
   <li> For a full list of the possible translations, see
   "Investigating dimensions" in
   Cryptography/AdvancedEncryptionStandard/plans/Experimentation.hpp.
   </li>
   <li> Generating instances for rounds 1-20 for a 20 random keys with each
   of the translations:
    <ul>
     <li> The canonical box translation:
     \verbatim
shell> mkdir ssaes_r1-20_c3_rw1_e4_f0_k1-20_aes_canon_box_aes_mc_bidirectional
shell> cd ssaes_r1-20_c3_rw1_e4_f0_k1-20_aes_canon_box_aes_mc_bidirectional
shell> oklib --maxima
num_rows : 1$
num_columns : 3$
exp : 4$
final_round_b : false$
box_tran : aes_ts_box$
seed : 1$
mc_tran : aes_mc_bidirectional$
for num_rounds : 1 thru 20 do (
  output_ss_fcl_std(
    num_rounds, num_columns, num_rows, exp, final_round_b, box_tran, mc_tran),
  for seed : 1 thru 20 do (
    output_ss_random_pc_pair(
      seed,num_rounds,num_columns,num_rows,exp,final_round_b)))$
exit();
shell> for r in $(seq 1 20); do
  for s in $(seq 1 20) do
    AppendDimacs-O3-DNDEBUG ssaes_r${r}_c3_rw1_e4_f0.cnf ssaes_pcpair_r${r}_c3_rw1_e4_f0_s${s}.cnf > r${r}_k${s}.cnf;
  done
done
     \endverbatim
     </li>
     <li> The minimum box translation:
     \verbatim
shell> mkdir ssaes_r1-20_c3_rw1_e4_f0_k1-20_aes_min_box_aes_mc_bidirectional
shell> cd ssaes_r1-20_c3_rw1_e4_f0_k1-20_aes_min_box_aes_mc_bidirectional
shell> oklib --maxima
num_rows : 1$
num_columns : 3$
exp : 4$
final_round_b : false$
box_tran : aes_small_box$
seed : 1$
mc_tran : aes_mc_bidirectional$
for num_rounds : 1 thru 20 do (
  output_ss_fcl_std(
    num_rounds, num_columns, num_rows, exp, final_round_b, box_tran, mc_tran),
  for seed : 1 thru 20 do (
    output_ss_random_pc_pair(
      seed,num_rounds,num_columns,num_rows,exp,final_round_b)))$
exit();
shell> for r in $(seq 1 20); do
  for s in $(seq 1 20) do
    AppendDimacs-O3-DNDEBUG ssaes_r${r}_c3_rw1_e4_f0.cnf ssaes_pcpair_r${r}_c3_rw1_e4_f0_s${s}.cnf > r${r}_k${s}.cnf;
  done
done
     \endverbatim
     </li>
     <li> The 1-base box translation:
     \verbatim
shell> mkdir ssaes_r1-20_c3_rw1_e4_f0_k1-20_aes_1base_box_aes_mc_bidirectional
shell> cd ssaes_r1-20_c3_rw1_e4_f0_k1-20_aes_1base_box_aes_mc_bidirectional
shell> oklib --maxima
num_rows : 1$
num_columns : 3$
exp : 4$
final_round_b : false$
box_tran : aes_rbase_box$
seed : 1$
mc_tran : aes_mc_bidirectional$
for num_rounds : 1 thru 20 do (
  output_ss_fcl_std(
    num_rounds, num_columns, num_rows, exp, final_round_b, box_tran, mc_tran),
  for seed : 1 thru 20 do (
    output_ss_random_pc_pair(
      seed,num_rounds,num_columns,num_rows,exp,final_round_b)))$
exit();
shell> for r in $(seq 1 20); do
  for s in $(seq 1 20) do
    AppendDimacs-O3-DNDEBUG ssaes_r${r}_c3_rw1_e4_f0.cnf ssaes_pcpair_r${r}_c3_rw1_e4_f0_s${s}.cnf > r${r}_k${s}.cnf;
  done
done
     \endverbatim
     </li>
    </ul>
   </li>
  </ul>


  \todo Overview of solver performance XXX ??? this is NOT an overview,
  however it should be XXX
  <ul>
   <li> Summary:
    <ul>
     <li> Comparing solvers (point-wise/per-instance) in terms of:
      <ul>
       <li> Time (canonical > minimum >= 1base) : ???
       </li>
       <li> r1 (canonical > 1base >= minimum) : ???
       </li>
       <li> Conflicts (minimum > canonical = 1-base) : ???
       </li>
      </ul>
     </li>
    </ul>
   </li>
   <li> Comparison of translations:
    <ul>
     <li> minisat-2.2.0:
      <ul>
       <li> Reading in experimental data:
       \verbatim
> git clone git://github.com/MGwynne/Experimental-data.git

> E_canon = read.table("Experimental-data/AES/1_3_4/ssaes_r1-20_c3_rw1_e4_f0_k1-20_aes_canon_box_aes_mc_bidirectional/MinisatStatistics",header=TRUE)
> E_1base = read.table("Experimental-data/AES/1_3_4/ssaes_r1-20_c3_rw1_e4_f0_k1-20_aes_1base_box_aes_mc_bidirectional/MinisatStatistics",header=TRUE)
> E_min = read.table("Experimental-data/AES/1_3_4/ssaes_r1-20_c3_rw1_e4_f0_k1-20_aes_min_box_aes_mc_bidirectional/MinisatStatistics",header=TRUE)

# Values averaged per round:
> E_canon_mean = aggregate(E_canon, by=list(r=E_canon$r), FUN=mean)
> E_1base_mean = aggregate(E_1base, by=list(r=E_1base$r), FUN=mean)
> E_min_mean = aggregate(E_min, by=list(r=E_min$r), FUN=mean)

> plot(E_canon)
> plot(E_1base)
> plot(E_min)
       \endverbatim
       </li>
       <li> Comparing the (individual) run-times for the three translations:
        <ul>
         <li> Comparing the canonical and minimum (in terms of time, r1 and
         cfs):
         \verbatim
> plot(E_canon$r, E_canon$t - E_min$t, , ylim=c(-max(abs(E_canon$t - E_min$t)), max(abs(E_canon$t - E_min$t))))
> m = lm(E_canon$t - E_min$t ~ E_canon$r)
> lines(E_canon$r, predict(m))
> summary(m)
              Estimate Std. Error t value Pr(>|t|)
(Intercept) -0.0002482  0.0202706  -0.012     0.99
E_canon$r    0.0085169  0.0016922   5.033 7.32e-07 ***
Residual standard error: 0.1951 on 398 degrees of freedom
Multiple R-squared: 0.05984,    Adjusted R-squared: 0.05748
F-statistic: 25.33 on 1 and 398 DF,  p-value: 7.323e-07
> mm = lm(E_canon_mean$t - E_min_mean$t ~ E_canon_mean$r); summary(mm)$r.squared
[1] 0.7233898


> plot(E_canon$r, E_canon$r1 - E_min$r1, ylim=c(-max(abs(E_canon$r1 - E_min$r1)), max(abs(E_canon$r1 - E_min$r1))))
> m = lm(E_canon$r1 - E_min$r1 ~ E_canon$r)
> lines(E_canon$r, predict(m))
> summary(m)
            Estimate Std. Error t value Pr(>|t|)
(Intercept)  -177602     131064  -1.355    0.176
E_canon$r      92916      10941   8.492 4.07e-16 ***
Residual standard error: 1262000 on 398 degrees of freedom
Multiple R-squared: 0.1534,     Adjusted R-squared: 0.1513
F-statistic: 72.12 on 1 and 398 DF,  p-value: 4.073e-16
> mm = lm(E_canon_mean$r1 - E_min_mean$r1 ~ E_canon_mean$r); summary(mm)$r.square
[1] 0.8788695


> plot(E_canon$r, E_canon$cfs - E_min$cfs, , ylim=c(-max(abs(E_canon$cfs - E_min$cfs)), max(abs(E_canon$cfs - E_min$cfs))))
> m = lm(E_canon$cfs - E_min$cfs ~ E_canon$r)
> lines(E_canon$r, predict(m))
> summary(m)
            Estimate Std. Error t value Pr(>|t|)
(Intercept)   567.49     286.36   1.982   0.0482 *
E_canon$r    -165.61      23.91  -6.928 1.73e-11 ***
Residual standard error: 2757 on 398 degrees of freedom
Multiple R-squared: 0.1076,     Adjusted R-squared: 0.1054
F-statistic:    48 on 1 and 398 DF,  p-value: 1.727e-11
> mm = lm(E_canon_mean$cfs - E_min_mean$cfs ~ E_canon_mean$r); summary(mm)$r.squared
[1] 0.6867762
         \endverbatim
         <li> Comparing the 1-base and minimum:
         \verbatim
XXX needs update

> plot(E_1base$r, E_1base$t - E_min$t, ylim=c(-max(abs(E_1base$t - E_min$t)), max(abs(E_1base$t - E_min$t))))
> m = lm(E_1base$t - E_min$t ~ E_1base$r)
> lines(E_1base$r, predict(m))
> summary(m)
(Intercept)  0.019724   0.012769   1.545  0.12324
E_1base$r   -0.003208   0.001066  -3.009  0.00279 **
Residual standard error: 0.1229 on 398 degrees of freedom
Multiple R-squared: 0.02224,    Adjusted R-squared: 0.01979
F-statistic: 9.054 on 1 and 398 DF,  p-value: 0.002787
> mm = lm(E_1base_mean$t - E_min_mean$t ~ E_1base_mean$r); summary(mm)$r.squared
[1] 0.2207844


> plot(E_1base$r, E_1base$r1 - E_min$r1, ylim=c(-max(abs(E_1base$r1 - E_min$r1)), max(abs(E_1base$r1 - E_min$r1))))
> m = lm(E_1base$r1 - E_min$r1 ~ E_1base$r)
> lines(E_1base$r, predict(m))
> summary(m)
(Intercept)    44300      65963   0.672    0.502
E_1base$r      -4236       5506  -0.769    0.442
Residual standard error: 635000 on 398 degrees of freedom
Multiple R-squared: 0.001485,   Adjusted R-squared: -0.001024
F-statistic: 0.5918 on 1 and 398 DF,  p-value: 0.4422
> mm = lm(E_1base_mean$r1 - E_min_mean$r1 ~ E_1base_mean$r); summary(mm)$r.squared
[1] 0.02052002

> plot(E_1base$r, E_1base$cfs - E_min$cfs, ylim=c(-max(abs(E_1base$cfs - E_min$cfs)), max(abs(E_1base$cfs - E_min$cfs))))
> m = lm(E_1base$cfs - E_min$cfs ~ E_1base$r)
> lines(E_1base$r, predict(m))
> summary(m)
(Intercept)   558.57     285.90   1.954   0.0514
E_1base$r    -159.34      23.87  -6.676 8.25e-11 ***
Residual standard error: 2752 on 398 degrees of freedom
Multiple R-squared: 0.1007,     Adjusted R-squared: 0.09845
F-statistic: 44.57 on 1 and 398 DF,  p-value: 8.254e-11
> mm = lm(E_1base_mean$cfs - E_min_mean$cfs ~ E_1base_mean$r); summary(mm)$r.squared
[1] 0.5608833
         \endverbatim
         </li>
         <li> Comparing the 1-base and minimum:
         \verbatim
> plot(E_1base$r, E_1base$t - E_canon$t, ylim=c(-max(abs(E_1base$t - E_canon$t)), max(abs(E_1base$t - E_canon$t))))
> m = lm(E_1base$t - E_canon$t ~ E_1base$r)
> lines(E_1base$r, predict(m))
> summary(m)
             Estimate Std. Error t value Pr(>|t|)
(Intercept)  0.019972   0.019726   1.012    0.312
E_1base$r   -0.011724   0.001647  -7.120 5.08e-12 ***
Residual standard error: 0.1899 on 398 degrees of freedom
Multiple R-squared: 0.113,      Adjusted R-squared: 0.1108
F-statistic: 50.69 on 1 and 398 DF,  p-value: 5.08e-12
> mm = lm(E_1base_mean$t - E_canon_mean$t ~ E_canon_mean$r); summary(mm)$r.squared
[1] 0.6872132

> plot(E_1base$r, E_1base$r1 - E_canon$r1, ylim=c(-max(abs(E_1base$r1 - E_canon$r1)), max(abs(E_1base$r1 - E_canon$r1))))
> m = lm(E_1base$r1 - E_canon$r1 ~ E_1base$r)
> lines(E_1base$r, predict(m))
> summary(m)
            Estimate Std. Error t value Pr(>|t|)
(Intercept)   221902     130185   1.705   0.0891 .
E_1base$r     -97152      10868  -8.940   <2e-16 ***
Residual standard error: 1253000 on 398 degrees of freedom
Multiple R-squared: 0.1672,     Adjusted R-squared: 0.1651
F-statistic: 79.92 on 1 and 398 DF,  p-value: < 2.2e-16
> mm = lm(E_1base_mean$r1 - E_canon_mean$r1 ~ E_canon_mean$r); summary(mm)$r.squared
[1] 0.7871271


> plot(E_1base$r, E_1base$cfs - E_canon$cfs, ylim=c(-max(abs(E_1base$cfs - E_canon$cfs)), max(abs(E_1base$cfs - E_canon$cfs))))
> m = lm(E_1base$cfs - E_canon$cfs ~ E_1base$r)
> lines(E_1base$r, predict(m))
> summary(m)
            Estimate Std. Error t value Pr(>|t|)
(Intercept)   -8.912    251.938  -0.035    0.972
E_1base$r      6.271     21.031   0.298    0.766
Residual standard error: 2425 on 398 degrees of freedom
Multiple R-squared: 0.0002234,  Adjusted R-squared: -0.002289
F-statistic: 0.08892 on 1 and 398 DF,  p-value: 0.7657
> mm = lm(E_1base_mean$cfs - E_canon_mean$cfs ~ E_canon_mean$r); summary(mm)$r.squared
[1] 0.002643576
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
         <li> We see the following relationships/distributions:
          <ul>
           <li> rounds vs r1: upper-bounded by a linear function, filling a
           triangle in the bottom left.
           \verbatim
# Upper bounding linear function
> E_canon_max = aggregate(E_canon, by=list(r=E_canon$r), FUN=max)
> m = lm(E_canon_max$r1 ~ E_canon_max$r)
> summary(m)
              Estimate Std. Error t value Pr(>|t|)
(Intercept)    -739835     196324  -3.768  0.00141 **
E_canon_max$r   400094      16389  24.413 3.01e-15 ***
Residual standard error: 422600 on 18 degrees of freedom
Multiple R-squared: 0.9707,	Adjusted R-squared: 0.9691

# Removing a lot of the variance due to the difference between keys
# yields a good linear relationship on the average time per round
> E_canon_mean = aggregate(E_canon, by=list(r=E_canon$r), FUN=mean)
> m = lm(E_canon_mean$r1 ~ E_canon_mean$r)
               Estimate Std. Error t value Pr(>|t|)
(Intercept)     -292840     127568  -2.296   0.0339 *
E_canon_mean$r   158358      10649  14.870 1.49e-11 ***
Residual standard error: 274600 on 18 degrees of freedom
Multiple R-squared: 0.9247,	Adjusted R-squared: 0.9205
F-statistic: 221.1 on 1 and 18 DF,  p-value: 1.49e-11
           \endverbatim
           </li>
           <li> r1 vs time: (strong) linear relationship:
           \verbatim
> m = lm(E_canon$t ~ E_canon$r1)
> summary(m)
             Estimate Std. Error t value Pr(>|t|)
(Intercept) 1.169e-02  8.894e-04   13.15   <2e-16 ***
E_canon$r1  1.466e-07  4.314e-10  339.76   <2e-16 ***

Residual standard error: 0.01329 on 398 degrees of freedom
Multiple R-squared: 0.9966,	Adjusted R-squared: 0.9966
F-statistic: 1.154e+05 on 1 and 398 DF,  p-value: < 2.2e-16
           \endverbatim
           </li>
           <li> r1 vs conflicts (weak) linear relationship ("cone-like"
           distribution):
           \verbatim
> m = lm(E_canon$cfs ~ E_canon$r1)
> summary(m)
             Estimate Std. Error t value Pr(>|t|)
(Intercept) 7.995e+02  6.990e+01   11.44   <2e-16 ***
E_canon$r1  1.112e-03  3.391e-05   32.79   <2e-16 ***

Residual standard error: 1045 on 398 degrees of freedom
Multiple R-squared: 0.7298,	Adjusted R-squared: 0.7291
F-statistic:  1075 on 1 and 398 DF,  p-value: < 2.2e-16
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
         <li> We see the following relationships/distributions:
          <ul>
           <li> rounds vs r1: upper-bounded by a linear relationship forming a
           triangle in the bottom left.
           \verbatim
# Upper bounding linear function
> E_1base_max = aggregate(E_1base, by=list(r=E_1base$r), FUN=max)
> m = lm(E_1base_max$r1 ~ E_1base_max$r)
> summary(m)
              Estimate Std. Error t value Pr(>|t|)
(Intercept)    -103382     140933  -0.734    0.473
E_1base_max$r   147126      11765  12.506 2.59e-10 ***
Residual standard error: 303400 on 18 degrees of freedom
Multiple R-squared: 0.8968,	Adjusted R-squared: 0.891
F-statistic: 156.4 on 1 and 18 DF,  p-value: 2.59e-10

# Removing a lot of the variance due to the difference between keys
# yields a reasonable linear relationship on the average time per round
> E_1base_mean = aggregate(E_1base, by=list(r=E_1base$r), FUN=mean)
> m = lm(E_1base_mean$r1 ~ E_1base_mean$r)
               Estimate Std. Error t value Pr(>|t|)
(Intercept)      -70937      69837  -1.016    0.323
E_1base_mean$r    61206       5830  10.499  4.2e-09 ***
Residual standard error: 150300 on 18 degrees of freedom
Multiple R-squared: 0.8596,	Adjusted R-squared: 0.8518
F-statistic: 110.2 on 1 and 18 DF,  p-value: 4.201e-09
           \endverbatim
           </li>
           <li> r1 vs time: (strong) linear relationship:
           \verbatim
> m = lm(E_1base$t ~ E_1base$r1)
> summary(m)
             Estimate Std. Error t value Pr(>|t|)
(Intercept) 4.814e-03  2.958e-04   16.27   <2e-16 ***
E_1base$r1  1.829e-07  3.520e-10  519.61   <2e-16 ***

Residual standard error: 0.004337 on 398 degrees of freedom
Multiple R-squared: 0.9985,	Adjusted R-squared: 0.9985
F-statistic: 2.7e+05 on 1 and 398 DF,  p-value: < 2.2e-16
           \endverbatim
           </li>
           <li> r1 vs conflicts (weak) linear relationship ("cone-like"
           distribution):
           \verbatim
> m = lm(E_1base$cfs ~ E_1base$r1)
> summary(m)
             Estimate Std. Error t value Pr(>|t|)
(Intercept) 6.181e+02  7.219e+01   8.562 2.46e-16 ***
E_1base$r1  3.081e-03  8.589e-05  35.869  < 2e-16 ***

Residual standard error: 1058 on 398 degrees of freedom
Multiple R-squared: 0.7637,	Adjusted R-squared: 0.7631
F-statistic:  1287 on 1 and 398 DF,  p-value: < 2.2e-16
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
         <li> We see (atthe following relationships/distributions:
          <ul>
           <li> rounds vs r1: (very weak) linear relationship forming a
           triangle in the bottom left.
           \verbatim
# Upper bounding linear function
> E_min_max = aggregate(E_min, by=list(r=E_min$r), FUN=max)
> m = lm(E_min_max$r1 ~ E_min_max$r)
> summary(m)
            Estimate Std. Error t value Pr(>|t|)
(Intercept)  -120616     143265  -0.842    0.411
E_min_max$r   161304      11960  13.487 7.53e-11 ***
Residual standard error: 308400 on 18 degrees of freedom
Multiple R-squared:  0.91,	Adjusted R-squared: 0.905
F-statistic: 181.9 on 1 and 18 DF,  p-value: 7.526e-11

# Removing a lot of the variance due to the difference between keys
# yields a reasonable linear relationship on the average time per round
> E_min_mean = aggregate(E_min, by=list(r=E_min$r), FUN=mean)
> m = lm(E_min_mean$r1 ~ E_min_mean$r)
             Estimate Std. Error t value Pr(>|t|)
(Intercept)   -115238      68114  -1.692    0.108
E_min_mean$r    65442       5686  11.509 9.85e-10 ***
Residual standard error: 146600 on 18 degrees of freedom
Multiple R-squared: 0.8804,	Adjusted R-squared: 0.8737
F-statistic: 132.5 on 1 and 18 DF,  p-value: 9.854e-10
           \endverbatim
           </li>
           <li> r1 vs time: (strong) linear relationship:
           \verbatim
> m = lm(E_min$t ~ E_min$r1)
> summary(m)
             Estimate Std. Error t value Pr(>|t|)
(Intercept) 5.009e-03  5.473e-04   9.152   <2e-16 ***
E_min$r1    2.069e-07  6.312e-10 327.761   <2e-16 ***

Residual standard error: 0.008227 on 398 degrees of freedom
Multiple R-squared: 0.9963,	Adjusted R-squared: 0.9963
           \endverbatim
           </li>
           <li> r1 vs conflicts (strong) sub-linear relationship:
           \verbatim
> m = lm(log(E_min$cfs+1) ~ log(E_min$r1+1))
> summary(m)
                   Estimate Std. Error t value Pr(>|t|)
(Intercept)       -1.432645   0.073707  -19.44   <2e-16 ***
log(E_min$r1 + 1)  0.736170   0.005998  122.73   <2e-16 ***

Residual standard error: 0.2747 on 398 degrees of freedom
Multiple R-squared: 0.9743,	Adjusted R-squared: 0.9742
F-statistic: 1.506e+04 on 1 and 398 DF,  p-value: < 2.2e-16
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


  \todo Minisat-2.2.0
  <ul>
   <li> Solving the key discovery problem over rounds 1 to 20. </li>
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
   <li> The canonical box translation:
    <ul>
     <li> The data:
     \verbatim
shell> for r in $(seq 1 20); do for s in $(seq 1 20); do RunMinisat r${r}_k${s}.cnf; done; done
shell> (ExtractMinisat header-only |  awk " { print \$0 \" r s\"}"; for r in $(seq 1 20); do for s in $(seq 1 20); do
    cat ExperimentMinisat_r${r}_k${s}cnf_*/Statistics | tail -n 1 | awk " { print \$0 \" ${r} ${s}\"}";
  done;
done) > MinisatStatistics
shell> oklib --R
R> E = read.table("MinisatStatistics", header=TRUE)
R> aggregate(E, by=list(r=E$r), FUN=mean)
 r          t     cfs     dec   rts         r1  stime      cfl
 1 0.00209900   11.25   19.40  1.00     660.00 0.0000    69.20
 2 0.00619855  177.20  225.20  2.15   14281.80 0.0000  2605.70
 3 0.01444730  656.05  837.75  5.30   49374.55 0.0000  9395.80
 4 0.06079030 2632.75 3264.25 14.95  252396.70 0.0000 48674.00
 5 0.08203690 1945.55 2257.70 11.65  427050.45 0.0030 38812.55
 6 0.11138255 2185.50 2529.10 12.75  637473.45 0.0050 44575.35
 7 0.16657410 2934.70 3373.85 15.80  998664.80 0.0060 52711.65
 8 0.17527285 2836.55 3278.80 15.50 1071410.70 0.0090 50784.80
 9 0.18567130 2633.35 3020.45 14.50 1153762.50 0.0100 44236.35
10 0.19621975 2585.75 3009.65 14.35 1221441.30 0.0100 44328.75
11 0.28540610 3394.70 3897.70 18.45 1819981.60 0.0100 57012.35
12 0.25701045 2818.85 3257.05 15.20 1664160.20 0.0100 48468.35
13 0.26745890 2717.20 3121.90 14.75 1741736.20 0.0100 45317.40
14 0.21241720 2011.65 2345.65 12.00 1411292.15 0.0100 32354.95
15 0.26685890 2395.90 2769.25 13.85 1774488.70 0.0100 38713.10
16 0.39878885 3271.80 3720.30 16.95 2675875.95 0.0105 51928.55
17 0.28070680 2172.45 2513.95 12.10 1844046.50 0.0100 34279.95
18 0.37214300 2734.45 3145.30 14.95 2532049.55 0.0110 44254.15
19 0.47567735 3408.80 3886.20 17.90 3228247.30 0.0115 53117.25
20 0.43298360 2923.95 3377.70 15.70 2879980.20 0.0145 46340.10
R> aggregate_statistics(E[c("r","t","cfs","r1")], by=list("r"))
 r     t.mean         t.sd    t.min    t.max cfs.mean      cfs.sd cfs.min cfs.max    r1.mean        r1.sd r1.min  r1.max
 1 0.00209900 0.0007181848 0.000999 0.002999    11.25    8.296321       0      26     660.00     395.7491    128    1348
 2 0.00619855 0.0026867473 0.001999 0.009998   177.20  109.930887       0     355   14281.80    8662.0507    232   28983
 3 0.01444730 0.0063025084 0.004999 0.026995   656.05  389.948374     157    1414   49374.55   30276.4735  11284  112242
 4 0.06079030 0.0357180089 0.004999 0.125980  2632.75 1629.268479      30    5589  252396.70  164043.5134   2391  597270
 5 0.08203690 0.0631297027 0.009998 0.219966  1945.55 1659.645348     143    5467  427050.45  356943.9064  20683 1221395
 6 0.11138255 0.0775167974 0.013997 0.267959  2185.50 1544.758353     232    5481  637473.45  455592.2654  53198 1525857
 7 0.16657410 0.1112723153 0.026995 0.394939  2934.70 1961.998152     363    6797  998664.80  678534.3169 128673 2408976
 8 0.17527285 0.1044432912 0.019996 0.413937  2836.55 1708.643675     197    6861 1071410.70  663768.7612  69656 2593923
 9 0.18567130 0.1443558773 0.013997 0.477927  2633.35 2086.233001      34    6639 1153762.50  926756.3247   9561 3024267
10 0.19621975 0.1583067006 0.021996 0.551916  2585.75 2076.081554     195    7390 1221441.30 1016359.3643  69736 3515718
11 0.28540610 0.1627646059 0.038994 0.562914  3394.70 1939.096157     400    6729 1819981.60 1038891.2096 173642 3584560
12 0.25701045 0.1792912152 0.065989 0.686895  2818.85 1978.336736     662    7590 1664160.20 1181515.1897 349124 4525741
13 0.26745890 0.1985975758 0.058991 0.723889  2717.20 2066.275077     476    7351 1741736.20 1325110.0452 305364 4779244
14 0.21241720 0.1446604372 0.033994 0.515921  2011.65 1427.647689     206    4831 1411292.15 1014151.0432 126374 3587238
15 0.26685890 0.2028711810 0.020996 0.673897  2395.90 1812.213270     113    5773 1774488.70 1412546.2407  22493 4595000
16 0.39878885 0.2869836774 0.040993 0.903862  3271.80 2348.890614     273    7543 2675875.95 1951189.7066 179810 6096492
17 0.28070680 0.2688636285 0.032994 0.878866  2172.45 2220.776239     140    7014 1844046.50 1837943.7135  91140 6098594
18 0.37214300 0.2631984243 0.038994 0.957854  2734.45 2006.621892     172    7216 2532049.55 1847561.5754 140151 6498739
19 0.47567735 0.3380297742 0.048992 1.006850  3408.80 2402.163008     328    7325 3228247.30 2338970.5910 208587 6893989
20 0.43298360 0.3191157711 0.054991 1.109830  2923.95 2163.098277     214    7286 2879980.20 2121299.5298 227504 7533385
     \endverbatim
     </li>
    </ul>
   </li>
   <li> The "minimum" box translation:
    <ul>
     <li> The data:
     \verbatim
shell> for r in $(seq 1 20); do for s in $(seq 1 20); do RunMinisat r${r}_k${s}.cnf; done; done
shell> (ExtractMinisat header-only |  awk " { print \$0 \" r s\"}"; for r in $(seq 1 20); do for s in $(seq 1 20); do
    cat ExperimentMinisat_r${r}_k${s}cnf_*/Statistics | tail -n 1 | awk " { print \$0 \" ${r} ${s}\"}";
  done;
done) > MinisatStatistics
shell> oklib --R
R> E = read.table("MinisatStatistics", header=TRUE)
R> aggregate(E, by=list(r=E$r), FUN=mean)
 r          t     cfs     dec   rts         r1 stime      cfl
 1 0.00129920   12.60   17.50  1.00     274.25 0e+00    47.70
 2 0.00294900  151.80  170.00  2.00    5006.40 0e+00  1076.35
 3 0.01164775 1037.55 1164.25  6.80   35272.45 0e+00  8485.30
 4 0.02119625 1632.30 1817.00  9.80   84047.35 0e+00 16503.75
 5 0.04454275 2438.15 2702.40 13.70  209070.45 0e+00 27488.45
 6 0.07833770 3571.95 4021.95 18.00  384206.55 0e+00 39985.25
 7 0.10123410 4012.95 4555.20 19.30  497897.35 0e+00 44606.40
 8 0.07293845 2694.30 3162.85 14.20  346702.25 0e+00 29397.45
 9 0.11693170 3838.70 4482.25 19.30  574464.05 0e+00 42421.15
10 0.12957990 3968.90 4712.75 20.10  621640.00 0e+00 44220.20
11 0.12578050 3695.65 4491.20 19.45  603521.65 0e+00 40933.40
12 0.16132500 4493.40 5547.05 22.45  764176.80 0e+00 52200.25
13 0.10173405 2921.50 3859.25 15.85  466299.90 0e+00 32794.15
14 0.12433055 3432.65 4604.15 18.00  570111.75 0e+00 39562.95
15 0.14357760 3809.70 5128.45 19.55  657743.45 0e+00 44295.45
16 0.20856770 5128.80 6783.75 24.85  959844.05 0e+00 59931.85
17 0.19292015 4639.75 6343.30 22.75  873623.40 0e+00 54282.75
18 0.21926605 5214.70 7154.50 25.20  996572.55 5e-04 61865.75
19 0.31230205 6683.00 9047.25 30.70 1447884.50 2e-03 77313.65
20 0.29610445 6498.80 8992.55 30.05 1339773.50 0e+00 78293.55
R> aggregate_statistics(E[c("r","t","cfs","r1")], by=list("r"))
 r     t.mean         t.sd    t.min    t.max cfs.mean     cfs.sd cfs.min cfs.max    r1.mean       r1.sd r1.min  r1.max
 1 0.00129920 0.0008009732 0.000000 0.001999    12.60   10.40445       0      38     274.25    185.0894     68     682
 2 0.00294900 0.0012343760 0.000999 0.004999   151.80   84.17813      29     329    5006.40   2878.0053    979   12031
 3 0.01164775 0.0089209350 0.001999 0.033994  1037.55  996.54870      55    3614   35272.45  34598.4919   1603  129526
 4 0.02119625 0.0160289740 0.001999 0.065989  1632.30 1425.92087      47    5731   84047.35  73311.9963   2418  276682
 5 0.04454275 0.0467369145 0.000999 0.189971  2438.15 2869.14163       8   12149  209070.45 235044.5198    480  899674
 6 0.07833770 0.0652549156 0.007998 0.205968  3571.95 2927.67843     355    9290  384206.55 341737.4855  28295 1053167
 7 0.10123410 0.0801703915 0.003999 0.245962  4012.95 3105.24267      30    9532  497897.35 412838.7988   2119 1236544
 8 0.07293845 0.0798782079 0.008998 0.329949  2694.30 2772.04863     268   11630  346702.25 423178.3829  16885 1704241
 9 0.11693170 0.0889130329 0.024996 0.311952  3838.70 2643.94544    1015    9474  574464.05 465180.6441  99962 1606866
10 0.12957990 0.1008156611 0.023996 0.338948  3968.90 2799.33103    1015    9785  621640.00 522814.5371  86499 1746761
11 0.12578050 0.0824517993 0.008998 0.261960  3695.65 2182.10450     515    7152  603521.65 425248.6003  35160 1314706
12 0.16132500 0.0843856252 0.065989 0.400939  4493.40 2143.14963    1968   10636  764176.80 417913.6579 280673 1916129
13 0.10173405 0.0795967149 0.007998 0.258960  2921.50 1957.83924     124    6953  466299.90 393628.3502   5779 1222216
14 0.12433055 0.0940194734 0.031995 0.406938  3432.65 1928.31411    1098    9252  570111.75 480194.1559 111518 2018451
15 0.14357760 0.1143185382 0.026995 0.471928  3809.70 2202.92686    1242    9423  657743.45 570416.0447  95942 2345812
16 0.20856770 0.1453429394 0.018997 0.508922  5128.80 2853.20227     858   10954  959844.05 711320.3296  47785 2455814
17 0.19292015 0.1605805175 0.028995 0.488925  4639.75 3033.84817    1431    9772  873623.40 787127.4048 104618 2357194
18 0.21926605 0.1575421525 0.033994 0.530919  5214.70 2712.66110    1654    9915  996572.55 771566.2514 111519 2558907
19 0.31230205 0.1867321948 0.068989 0.649901  6683.00 2940.27992    3183   11156 1447884.50 936455.6293 247487 3280379
20 0.29610445 0.1918973678 0.048992 0.704892  6498.80 3117.63079    2138   13543 1339773.50 924026.6407 171767 3325637
     \endverbatim
     </li>
    </ul>
   </li>
   <li> The 1-base box translation:
    <ul>
     <li> The data:
     \verbatim
shell> for r in $(seq 1 20); do for s in $(seq 1 20); do RunMinisat r${r}_k${s}.cnf; done; done
shell> (ExtractMinisat header-only |  awk " { print \$0 \" r s\"}"; for r in $(seq 1 20); do for s in $(seq 1 20); do
    cat ExperimentMinisat_r${r}_k${s}cnf_*/Statistics | tail -n 1 | awk " { print \$0 \" ${r} ${s}\"}";
  done;
done) > MinisatStatistics
shell> oklib --R
R> E = read.table("MinisatStatistics", header=TRUE)
R> aggregate(E, by=list(r=E$r), FUN=mean)
 r          t     cfs     dec   rts         r1 stime      cfl
 1 0.00104920   11.75   16.05  1.00     297.95 0e+00    40.85
 2 0.00264905  137.10  151.15  1.85    5022.70 0e+00   973.25
 3 0.00854825  551.00  603.85  4.35   22033.40 0e+00  4237.85
 4 0.02019650 1528.45 1685.45  9.05   75112.95 0e+00 14116.90
 5 0.04109315 1903.50 2098.85 11.45  190777.35 0e+00 23197.65
 6 0.05204165 2142.00 2402.45 12.20  250793.95 0e+00 25771.55
 7 0.08078715 2973.60 3341.55 15.60  406894.15 0e+00 35027.45
 8 0.09553500 3116.05 3496.65 15.75  483592.95 0e+00 35995.70
 9 0.14897685 4323.55 4835.10 20.95  764502.05 0e+00 49233.45
10 0.11518200 3011.40 3379.95 15.25  585374.65 0e+00 35097.05
11 0.09308545 2215.30 2537.10 12.85  473967.50 0e+00 26525.05
12 0.16382460 3632.55 4098.35 18.60  862730.70 0e+00 41608.85
13 0.10703320 2227.85 2567.60 12.95  556348.70 0e+00 27326.20
14 0.15122650 2857.30 3241.50 15.45  801051.60 0e+00 33147.25
15 0.20156880 3572.30 4022.80 18.55 1072436.45 0e+00 41403.10
16 0.16627425 2729.70 3115.00 15.45  893246.00 0e+00 31818.50
17 0.17807240 2677.80 3029.60 15.10  980475.30 2e-03 30580.50
18 0.13832840 2015.25 2314.50 12.00  742436.80 0e+00 24118.00
19 0.24006285 3420.05 3883.25 17.65 1295140.10 1e-03 40041.90
20 0.18192175 2540.65 2914.35 14.20  972317.05 5e-04 30048.10
> aggregate_statistics(E[c("t","cfs","r1")], by=list("r"))
 r     t.mean         t.sd    t.min    t.max cfs.mean      cfs.sd cfs.min cfs.max    r1.mean       r1.sd r1.min  r1.max
 1 0.00104920 0.0006860107 0.000000 0.001999    11.75    9.999342       0      26     297.95    191.2332     68     602
 2 0.00264905 0.0013868689 0.000000 0.004999   137.10   86.965147      23     299    5022.70   3122.5793   1008   10976
 3 0.00854825 0.0051442687 0.000999 0.015997   551.00  429.715695      27    1178   22033.40  17357.0337   1155   47143
 4 0.02019650 0.0128846871 0.001999 0.038994  1528.45 1149.300679      42    3278   75112.95  55965.3428   2118  161302
 5 0.04109315 0.0309229686 0.001999 0.109983  1903.50 1547.444498       4    5270  190777.35 158866.3699    458  543624
 6 0.05204165 0.0423957343 0.016997 0.180972  2142.00 1807.511549     568    7504  250793.95 225510.6239  62576  945950
 7 0.08078715 0.0700535920 0.002999 0.236963  2973.60 2587.008339       6    8503  406894.15 366847.2775    795 1226977
 8 0.09553500 0.0857727663 0.005999 0.271958  3116.05 2817.265835     169    8637  483592.95 455822.6939  18631 1421394
 9 0.14897685 0.0887120218 0.024996 0.276957  4323.55 2560.181356     660    7748  764502.05 470099.0744 101474 1428512
10 0.11518200 0.1026149093 0.008998 0.306953  3011.40 2726.485870     163    7936  585374.65 547302.6535  26949 1596471
11 0.09308545 0.0721742973 0.012998 0.306953  2215.30 1721.798145     232    7200  473967.50 382437.8317  38059 1589447
12 0.16382460 0.1073268551 0.005999 0.347947  3632.55 2340.596134      96    7643  862730.70 581698.0947   7707 1882597
13 0.10703320 0.0571489960 0.026995 0.240963  2227.85 1181.956686     538    4883  556348.70 308009.5031 120051 1262303
14 0.15122650 0.1073580860 0.012998 0.405938  2857.30 2003.022482     221    7608  801051.60 584803.4967  55606 2197649
15 0.20156880 0.1231082702 0.021996 0.444932  3572.30 2155.523507     364    7761 1072436.45 669491.3892  98911 2422059
16 0.16627425 0.1084727048 0.026995 0.389940  2729.70 1756.797089     451    6494  893246.00 605388.0357 109688 2094820
17 0.17807240 0.1280684516 0.031995 0.448931  2677.80 1911.922582     420    6438  980475.30 728304.6427 152767 2493013
18 0.13832840 0.0976646326 0.011998 0.358945  2015.25 1437.085899      96    5190  742436.80 547055.4009  23797 1988347
19 0.24006285 0.1564269265 0.014997 0.568913  3420.05 2329.765032     125    8953 1295140.10 860937.1408  35806 3026753
20 0.18192175 0.1541351973 0.015997 0.453930  2540.65 2169.723637     202    6135  972317.05 832396.4751  53715 2488814
     \endverbatim
     </li>
     <li> Total times and conflicts over 20 rounds and keys:
     \verbatim
1_3_4> oklib --R
R> E_canon = read.table(Sys.glob("ssaes_r1-20_c*_aes_canon_box*/MinisatStatistics")[1], header=TRUE)
R> E_1base = read.table(Sys.glob("ssaes_r1-20_c*_aes_1base_box*/MinisatStatistics")[1], header=TRUE)
R> E_min = read.table(Sys.glob("ssaes_r1-20_c*_aes_min_box*/MinisatStatistics")[1], header=TRUE)
R> options(width=1000)
R> sum(E_canon$t); sum(E_1base$t); sum(E_min$t)
[1] 85.00288
[1] 43.74914
[1] 49.3313
R> sum(E_canon$cfs); sum(E_1base$cfs); sum(E_min$cfs)
[1] 928968
[1] 951743
[1] 1397543
     \endverbatim
     </li>
    </ul>
   </li>
  </ul>

*/
