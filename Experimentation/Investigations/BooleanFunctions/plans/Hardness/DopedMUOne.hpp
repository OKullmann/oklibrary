// Oliver Kullmann, 11.5.2013 (Swansea)
/* Copyright 2013 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/BooleanFunctions/plans/Hardness/DopedMUOne.hpp
  \brief Investigations regarding hardness of doped elements of MU(1)


  \todo Hardness of prime-extremal satisfiable general Horn clause-sets
  <ul>
   <li> The following needs a complete update, based on the arXiv
   report http://arxiv.org/abs/1302.4421 . </li>
   <li> The clause-sets sat_genhorn_cs(k,l) are candidates to separate
   representations without new variables of softness l+1 from softness l:
    <ol>
     <li> sat_genhorn_cs(k,l) itself has hardness min(k,l). </li>
     <li> So for l >= 0 the sequence S = (sat_genhorn_cs(k,l+1))_{k >= l+1} has
     a linear-size representation without new variables of hardness precisely
     l+1, namely itself. </li>
     <li> Now the conjecture is that S has no polysize representations without
     new variables of softness l. </li>
     <li> This is true for l=0, as it follows from the results of the article
     [Sloan, Soereny, Turan, On k-term DNF with the largest number of prime
     implicants, 2007]. </li>
     <li> One experiment:
     \verbatim
F : sat_genhorn_cs(3,3)$
  8
P : min_resolution_closure_cs(F)[1]$
length(P);
  255
set_random(1);
B : rand_rbase_cs(P,ucp_0_cs)$
length(B);
  42
set_random(2);
B : rand_rbase_cs(P,ucp_0_cs)$
length(B);
  39
set_random(3);
B : rand_rbase_cs(P,ucp_0_cs)$
length(B);
  38
     \endverbatim
     We see here a rather small 1-base. </li>
    </ol>
   </li>
   <li> A systematic study of the boolean functions of sat_genhorn_cs(k,l) is
   needed:
    <ol>
     <li> A good representation (in the general sense) of the underlying
     boolean function is needed. Perhaps this should go to investigations on
     boolean functions in general. </li>
     <li> What is the precise number of variables, clauses, literal occurrences
     ? </li>
     <li> What are precisely the prime implicates and prime implicants?
      <ul>
       <li> From Lemma 8 of [Sloan, Soereny, Turan, On k-term DNF with the
       largest number of prime implicants, 2007] we have the following
       characterisation of the prime implicates:
       \verbatim
# Extracting the prime implicant from a subset F of the leaves of
# sat_genhorn_cs(k,l):
sat_genhorn_ul(F) := block([lit_s : olit_cs(F)],
  subset(lit_s, lambda([l],not(elementp(-l,lit_s)))))$

# Computing the prime implicates for sat_genhorn_cs(k,l):
sat_genhorn_prime_cs(k,l) :=
  map(sat_genhorn_ul, disjoin({},powerset(sat_genhorn_cs(k,l))))$

# Checking correctness:
F : sat_genhorn_cs(2,2)$
is(min_resolution_closure_cs(F)[1] = sat_genhorn_prime_cs(2,2));
 true
F : sat_genhorn_cs(3,3)$
is(min_resolution_closure_cs(F)[1] = sat_genhorn_prime_cs(3,3));
 true
       \endverbatim
       That is, the prime implicates of sat_genhorn_cs(k,l) are precisely
       the clauses {l in F: -l not in F} for subsets F of sat_genhorn_cs(k,l).
       </li>
      </ul>
     </li>
     <li> What are the m-bases for 0 <= m <= l ? </li>
    </ol>
   </li>
   <li> Are the sat_genhorn_cs(k,l) actually level-l Horn clause-sets? </li>
  </ul>


  \todo 3 different representations of HIT(1)
  <ul>
   <li> According to [Gywnne, Kullmann, 2013]. </li>
   <li> Experiment preparation:
   \verbatim
k:2;
for h in [22,32,42,52,62,72] do (output_ext1_sat_genhorn(h,k),output_ext2_sat_genhorn(h,k),output_ext3_sat_genhorn(h,k));
k:3;
for h in [23,33,43] do (output_ext1_sat_genhorn(h,k),output_ext2_sat_genhorn(h,k),output_ext3_sat_genhorn(h,k));
k:4;
for h in [24,34,44] do (output_ext1_sat_genhorn(h,k),output_ext2_sat_genhorn(h,k),output_ext3_sat_genhorn(h,k));
k:5;
for h in [25,35] do (output_ext1_sat_genhorn(h,k),output_ext2_sat_genhorn(h,k),output_ext3_sat_genhorn(h,k));

> for F in *.ecnf; do B=$(basename --suffix=".ecnf" ${F}); echo ${B}; cat ${F} | ExtendedToStrictDimacs-O3-DNDEBUG > ${B}.cnf; done
   \endverbatim
   </li>
   <li> Determining the sizes:
   \verbatim
for F in *.cnf; do echo ${F} " "; cat ${F} | ExtendedDimacsStatistics-O3-DNDEBUG; done
E1_SAT_genhorn_22_2.cnf
      n       c   l
    507     508   8604
E1_SAT_genhorn_23_3.cnf
      n       c   l
   4095    4096   80594
E1_SAT_genhorn_24_4.cnf
      n       c   l
  25901   25902   562542
E1_SAT_genhorn_25_5.cnf
      n       c   l
 136811  136812   3202912
E1_SAT_genhorn_32_2.cnf
      n       c   l
   1057    1058   24994
E1_SAT_genhorn_33_3.cnf
      n       c   l
  12035   12036   327384
E1_SAT_genhorn_34_4.cnf
      n       c   l
 105911  105912   3150408
E1_SAT_genhorn_35_5.cnf
      n       c   l
 768335  768336   24413776
E1_SAT_genhorn_42_2.cnf
      n       c   l
   1807    1808   54784
E1_SAT_genhorn_43_3.cnf
      n       c   l
  26575   26576   922524
E1_SAT_genhorn_44_4.cnf
      n       c   l
 299971  299972   11326724
E1_SAT_genhorn_52_2.cnf
      n       c   l
   2757    2758   101974
E1_SAT_genhorn_62_2.cnf
      n       c   l
   3907    3908   170564
E1_SAT_genhorn_72_2.cnf
      n       c   l
   5257    5258   264554
E2_SAT_genhorn_22_2.cnf
      n       c   l
    761    4811   17716
E2_SAT_genhorn_23_3.cnf
      n       c   l
   6143   44394   165284
E2_SAT_genhorn_24_4.cnf
      n       c   l
  38852  307174   1150986
E2_SAT_genhorn_25_5.cnf
      n       c   l
 205217 1738269   6542636
E2_SAT_genhorn_32_2.cnf
      n       c   l
   1586   13556   51046
E2_SAT_genhorn_33_3.cnf
      n       c   l
  18053  175729   666804
E2_SAT_genhorn_34_4.cnf
      n       c   l
 158867 1681117   6406728
E2_SAT_genhorn_35_5.cnf
     n        c   l
1152503 12975225  49595888
E2_SAT_genhorn_42_2.cnf
      n       c   l
   2711   29201   111376
E2_SAT_genhorn_43_3.cnf
      n       c   l
  39863  487839   1871624
E2_SAT_genhorn_44_4.cnf
      n       c   l
 449957 5963335   22953420
E2_SAT_genhorn_52_2.cnf
      n       c   l
   4136   53746   206706
E2_SAT_genhorn_62_2.cnf
      n       c   l
   5861   89191   345036
E2_SAT_genhorn_72_2.cnf
      n       c   l
   7886  137536   534366
E3_SAT_genhorn_22_2.cnf
      n       c   l
    761    4557   13160
E3_SAT_genhorn_23_3.cnf
      n       c   l
   6143   42346   122939
E3_SAT_genhorn_24_4.cnf
      n       c   l
  38852  294223   856764
E3_SAT_genhorn_25_5.cnf
      n       c   l
 205217 1669863   4872774
E3_SAT_genhorn_32_2.cnf
      n       c   l
   1586   13027   38020
E3_SAT_genhorn_33_3.cnf
      n       c   l
  18053  169711   497094
E3_SAT_genhorn_34_4.cnf
      n       c   l
 158867 1628161   4778568
E3_SAT_genhorn_35_5.cnf
      n       c   l
1152503 12591057  37004832
E3_SAT_genhorn_42_2.cnf
      n       c   l
   2711   28297   83080
E3_SAT_genhorn_43_3.cnf
      n       c   l
  39863  474551   1397074
E3_SAT_genhorn_44_4.cnf
      n       c   l
 449957 5813349   17140072
E3_SAT_genhorn_52_2.cnf
      n       c   l
   4136   52367   154340
E3_SAT_genhorn_62_2.cnf
      n       c   l
   5861   87237   257800
E3_SAT_genhorn_72_2.cnf
      n       c   l
   7886  134907   399460
   \endverbatim
   </li>
   <li> Compiling satz:
   \verbatim
ExternalSources> CPPFLAGS="-DMAX_CLAUSE_LENGTH=1000000 -DMAX_NUMBER_VARIABLES=2000000 -DMAX_NUMBER_CLAUSES=15000000" oklib satz
   \endverbatim
   </li>
   <li> Running experiments (on cs-wsok; file "Problems" contains all problems,
   "Problems23" those of type 2,3):
   \verbatim
> for F in *.cnf; do B=$(basename --suffix=".cnf" ${F}); echo ${B}; OKsolver_2002-O3-DNDEBUG --timeout=7200 ${F} > ${B}.oksolver; done
(E2_SAT_genhorn_35_5: aborted after 27097.7s and ~ 26 GB)

> for F in *.cnf; do B=$(basename --suffix=".cnf" ${F}); echo ${B}; OKsolver_2002_NTP-O3-DNDEBUG ${F} > ${B}.oksolver-ntp; done

> for F in $(cat Problems23); do B=$(basename --suffix=".cnf" ${F}); echo ${B}; satz215 ${F} > ${B}.satz; done
XXX

> for F in $(cat Problems); do B=$(basename --suffix=".cnf" ${F}); echo ${B}; glucose-2.0 ${F} > ${B}.glucose; done
(E2_SAT_genhorn_44_4: aborted after 535 min)
(E3_SAT_genhorn_44_4: aborted after 518 min)
(E2_SAT_genhorn_35_5: aborted after 674 min)
(E3_SAT_genhorn_35_5: aborted after 538 min)

> for F in $(cat Problems); do B=$(basename --suffix=".cnf" ${F}); echo ${B}; glucose-2.2 ${F} > ${B}.glucose22; done
(E2_SAT_genhorn_44_4: aborted after 1513m)
(E3_SAT_genhorn_44_4: aborted after 1140m)
(E2_SAT_genhorn_35_5: aborted after 6240m)
(E3_SAT_genhorn_35_5: aborted after 1298m)

> for F in $(cat Problems); do B=$(basename --suffix=".cnf" ${F}); echo ${B}; glucose-2.2 -no-pre ${F} > ${B}.glucose22-no; done
(E2_SAT_genhorn_44_4: aborted after 1516m)
(E3_SAT_genhorn_44_4: aborted after 1735m)
(E2_SAT_genhorn_35_5: aborted after 1441m)
(E3_SAT_genhorn_35_5: aborted after 1355m)

> for F in *.cnf; do B=$(basename --suffix=".cnf" ${F}); echo ${B}; picosat913 ${F} > ${B}.picosat; done
(E1_SAT_genhorn_35_5: gives up after 478.5 sec)
(E2_SAT_genhorn_35_5: out of memory, > 20 GB)
(E3_SAT_genhorn_35_5: out of memory, > 30 GB)
(E3_SAT_genhorn_44_4: out of memory, > 30 GB)
(E2_SAT_genhorn_44_4: out of memory, > 30 GB)

> for F in *.cnf; do B=$(basename --suffix=".cnf" ${F}); echo ${B}; precosat-570.1 -v ${F} > ${B}.precosat; done

> for F in *.cnf; do B=$(basename --suffix=".cnf" ${F}); echo ${B}; lingelingala-b02aa1a-121013 -v ${F} > ${B}.lingeling; done
(aborted on E2_SAT_genhorn_35_5 after 11464 min, on E2_SAT_genhorn_44_4 after
1571 min, on E3_SAT_genhorn_35_5 after 1580 min, on E3_SAT_genhorn_44_4 after
1015 min)
> ls *.cnf > AllProblems
> cp AllProblems ProblemsLingeling
> for F in $(cat ProblemsLingeling); do B=$(basename --suffix=".cnf" ${F}); echo ${B}; lingelingala-b02aa1a-121013 -v ${F} > ${B}.lingeling; done

ls *_2.cnf > Problems; ls *_3.cnf >> Problems; ls *_4.cnf >> Problems; ls *_5.cnf >> Problems

> for F in $(cat Problems); do B=$(basename --suffix=".cnf" ${F}); echo ${B}; minisat-2.2.0 -no-pre ${F} > ${B}.minisat-no; done
(E2_SAT_genhorn_44_4: aborted after 848 min)
(E3_SAT_genhorn_44_4: aborted after 533 min)
(E2_SAT_genhorn_35_5: segmentation fault)
(E3_SAT_genhorn_35_5: got killed by accident, > 600 min)

> for F in *.cnf; do B=$(basename --suffix=".cnf" ${F}); echo ${B}; minisat-2.2.0 ${F} > ${B}.minisat; done
(E2_SAT_genhorn_35_5: aborted after 1511 min)
(E2_SAT_genhorn_44_4: aborted after 296 min)
(E3_SAT_genhorn_35_5: aborted after 1011 min)
(E3_SAT_genhorn_44_4: segmentation fault)

> for F in *.cnf; do B=$(basename --suffix=".cnf" ${F}); echo ${B}; cryptominisat ${F} > ${B}.cryptominisat_296; done
(for E2_SAT_genhorn_35_5.cnf and E3_SAT_genhorn_35_5.cnf there are too long
clauses to be handled by it)

> for F in $(cat Problems); do B=$(basename --suffix=".cnf" ${F}); echo ${B}; march_pl ${F} > ${B}.march; done
(can't solve E2_SAT_genhorn_23_3, E3_SAT_genhorn_23_3 due to
Assertion `btb_size[ i ] == 0' failed)
(can't solve E2_SAT_genhorn_33_3, E3_SAT_genhorn_33_3 due to
segmentation fault)
(E1_SAT_genhorn_34_4: aborted after 370 min)
other instances not tried
   \endverbatim
   </li>
   <li> Extracting statistics:
   \verbatim
sfile="OKsolver.stats"
ssuffix=".oksolver"
sextract="ExtractOKsolver"
echo -n "type k h " > ${sfile}
${sextract} "header-only" >> ${sfile}
for ((k=2; k <= 5; ++k)); do for F in *_${k}${ssuffix}; do T=$(echo ${F} | cut -d"_" -f1 | cut -d"E" -f2); H=$(echo ${F} | cut -d"_" -f4); K=$(basename --suffix="${ssuffix}" ${F} | cut -d"_" -f5); echo -n "$T $K $H " >> ${sfile}; cat ${F} | ${sextract} extract >> ${sfile}; done; done

sfile="OKsolver-ntp.stats"
ssuffix=".oksolver-ntp"
sextract="ExtractOKsolver"
echo -n "type k h " > ${sfile}
${sextract} "header-only" >> ${sfile}
for ((k=2; k <= 5; ++k)); do for F in *_${k}${ssuffix}; do T=$(echo ${F} | cut -d"_" -f1 | cut -d"E" -f2); H=$(echo ${F} | cut -d"_" -f4); K=$(basename --suffix="${ssuffix}" ${F} | cut -d"_" -f5); echo -n "$T $K $H " >> ${sfile}; cat ${F} | ${sextract} extract >> ${sfile}; done; done

sfile="Satz.stats"
ssuffix=".satz"
sextract="ExtractSatz"
echo -n "type k h " > ${sfile}
${sextract} "header-only" >> ${sfile}
for ((k=2; k <= 5; ++k)); do for F in *_${k}${ssuffix}; do T=$(echo ${F} | cut -d"_" -f1 | cut -d"E" -f2); H=$(echo ${F} | cut -d"_" -f4); K=$(basename --suffix="${ssuffix}" ${F} | cut -d"_" -f5); echo -n "$T $K $H " >> ${sfile}; cat ${F} | ${sextract} extract >> ${sfile}; done; done

sfile="Glucose.stats"
ssuffix=".glucose"
sextract="ExtractGlucose"
echo -n "type k h " > ${sfile}
${sextract} "header-only" >> ${sfile}
for ((k=2; k <= 5; ++k)); do for F in *_${k}${ssuffix}; do T=$(echo ${F} | cut -d"_" -f1 | cut -d"E" -f2); H=$(echo ${F} | cut -d"_" -f4); K=$(basename --suffix="${ssuffix}" ${F} | cut -d"_" -f5); echo -n "$T $K $H " >> ${sfile}; cat ${F} | ${sextract} extract >> ${sfile}; done; done

sfile="Glucose22.stats"
ssuffix=".glucose22"
sextract="ExtractGlucose"
echo -n "type k h " > ${sfile}
${sextract} "header-only" >> ${sfile}
for ((k=2; k <= 5; ++k)); do for F in *_${k}${ssuffix}; do T=$(echo ${F} | cut -d"_" -f1 | cut -d"E" -f2); H=$(echo ${F} | cut -d"_" -f4); K=$(basename --suffix="${ssuffix}" ${F} | cut -d"_" -f5); echo -n "$T $K $H " >> ${sfile}; cat ${F} | ${sextract} extract >> ${sfile}; done; done

sfile="Glucose22-no.stats"
ssuffix=".glucose22-no"
sextract="ExtractGlucose"
echo -n "type k h " > ${sfile}
${sextract} "header-only" >> ${sfile}
for ((k=2; k <= 5; ++k)); do for F in *_${k}${ssuffix}; do T=$(echo ${F} | cut -d"_" -f1 | cut -d"E" -f2); H=$(echo ${F} | cut -d"_" -f4); K=$(basename --suffix="${ssuffix}" ${F} | cut -d"_" -f5); echo -n "$T $K $H " >> ${sfile}; cat ${F} | ${sextract} extract >> ${sfile}; done; done

sfile="Picosat.stats"
ssuffix=".picosat"
sextract="ExtractPicosat"
echo -n "type k h " > ${sfile}
${sextract} "header-only" >> ${sfile}
for ((k=2; k <= 5; ++k)); do for F in *_${k}${ssuffix}; do T=$(echo ${F} | cut -d"_" -f1 | cut -d"E" -f2); H=$(echo ${F} | cut -d"_" -f4); K=$(basename --suffix="${ssuffix}" ${F} | cut -d"_" -f5); echo -n "$T $K $H " >> ${sfile}; cat ${F} | ${sextract} extract >> ${sfile}; done; done

sfile="Precosat570.stats"
ssuffix=".precosat"
sextract="ExtractPrecosat570"
echo -n "type k h " > ${sfile}
${sextract} "header-only" >> ${sfile}
for ((k=2; k <= 5; ++k)); do for F in *_${k}${ssuffix}; do T=$(echo ${F} | cut -d"_" -f1 | cut -d"E" -f2); H=$(echo ${F} | cut -d"_" -f4); K=$(basename --suffix="${ssuffix}" ${F} | cut -d"_" -f5); echo -n "$T $K $H " >> ${sfile}; cat ${F} | ${sextract} extract >> ${sfile}; done; done

sfile="Lingeling.stats"
ssuffix=".lingeling"
sextract="ExtractLingeling"
echo -n "type k h " > ${sfile}
${sextract} "header-only" >> ${sfile}
for ((k=2; k <= 5; ++k)); do for F in *_${k}${ssuffix}; do T=$(echo ${F} | cut -d"_" -f1 | cut -d"E" -f2); H=$(echo ${F} | cut -d"_" -f4); K=$(basename --suffix="${ssuffix}" ${F} | cut -d"_" -f5); echo -n "$T $K $H " >> ${sfile}; cat ${F} | ${sextract} extract >> ${sfile}; done; done

sfile="Minisat.stats"
ssuffix=".minisat"
sextract="ExtractMinisat"
echo -n "type k h " > ${sfile}
${sextract} "header-only" >> ${sfile}
for ((k=2; k <= 5; ++k)); do for F in *_${k}${ssuffix}; do T=$(echo ${F} | cut -d"_" -f1 | cut -d"E" -f2); H=$(echo ${F} | cut -d"_" -f4); K=$(basename --suffix="${ssuffix}" ${F} | cut -d"_" -f5); echo -n "$T $K $H " >> ${sfile}; cat ${F} | ${sextract} extract >> ${sfile}; done; done

sfile="Minisat-no.stats"
ssuffix=".minisat-no"
sextract="ExtractMinisat"
echo -n "type k h " > ${sfile}
${sextract} "header-only" >> ${sfile}
for ((k=2; k <= 5; ++k)); do for F in *_${k}${ssuffix}; do T=$(echo ${F} | cut -d"_" -f1 | cut -d"E" -f2); H=$(echo ${F} | cut -d"_" -f4); K=$(basename --suffix="${ssuffix}" ${F} | cut -d"_" -f5); echo -n "$T $K $H " >> ${sfile}; cat ${F} | ${sextract} extract >> ${sfile}; done; done

sfile="Cryptominisat296.stats"
ssuffix=".cryptominisat_296"
sextract="ExtractCryptominisat"
echo -n "type k h " > ${sfile}
${sextract} "header-only" >> ${sfile}
for ((k=2; k <= 5; ++k)); do for F in *_${k}${ssuffix}; do T=$(echo ${F} | cut -d"_" -f1 | cut -d"E" -f2); H=$(echo ${F} | cut -d"_" -f4); K=$(basename --suffix="${ssuffix}" ${F} | cut -d"_" -f5); echo -n "$T $K $H " >> ${sfile}; cat ${F} | ${sextract} extract >> ${sfile}; done; done

sfile="Marchpl.stats"
ssuffix=".march"
sextract="ExtractMarchpl"
echo -n "type k h " > ${sfile}
${sextract} "header-only" >> ${sfile}
for ((k=2; k <= 5; ++k)); do for F in *_${k}${ssuffix}; do T=$(echo ${F} | cut -d"_" -f1 | cut -d"E" -f2); H=$(echo ${F} | cut -d"_" -f4); K=$(basename --suffix="${ssuffix}" ${F} | cut -d"_" -f5); echo -n "$T $K $H " >> ${sfile}; cat ${F} | ${sextract} extract >> ${sfile}; done; done
   \endverbatim
   </li>
   <li> Evaluation:
   \verbatim
> E=read_satstat("OKsolver.stats")
> for (t in seq(1,3)) for (k in seq(2,5)) cat(t,k,":",E$t[E$type==t & E$k==k & E$sat==0],"\n")
1 2 : 0 0 0.1 0.1 0.3 0.6
1 3 : 0 0.3 1.5
1 4 : 0.5 4.2 24.3
1 5 : 3.5 52
2 2 : 0 0 0.2 1.7 4.8 11.6
2 3 : 0.3 16.9 135.5
2 4 : 36.2 1164.6
2 5 : 932.1
3 2 : 0 0 0.1 0.4 1.1 5.1
3 3 : 0.1 6.9 65.3
3 4 : 17.9 638.7
3 5 : 553.7
> for (t in seq(1,3)) for (k in seq(2,5)) cat(t,k,":",E$nds[E$type==t & E$k==k & E$sat==0],"\n")
1 2 : 43 63 83 103 123 143
1 3 : 507 1057 1807
1 4 : 4095 12035 26575
1 5 : 25901 105911
2 2 : 1 1 1 1 1 1
2 3 : 1 1 1
2 4 : 1 1
2 5 : 1
3 2 : 1 1 1 1 1 1
3 3 : 1 1 1
3 4 : 1 1
3 5 : 1

> E=read_satstat("OKsolver-ntp.stats")
> for (t in seq(1,3)) for (k in seq(2,5)) cat(t,k,":",E$t[E$type==t & E$k==k & E$sat==0],"\n")
1 2 : 0 0 0 0.1 0.2 0.4
1 3 : 0 0.2 1
1 4 : 0.4 3.3 16.5
1 5 : 2.7 31.2
2 2 : 0 0 0.1 0.4 1 4
2 3 : 0.2 4.8 82.6
2 4 : 15.4 843.4 10232.8
2 5 : 664.6 36743.4
3 2 : 0 0 0.1 0.2 0.5 1
3 3 : 0.1 1.6 28.9
3 4 : 4.5 410.8 5295.6
3 5 : 348.7 20061.6
> for (t in seq(1,3)) for (k in seq(2,5)) cat(t,k,":",E$nds[E$type==t & E$k==k & E$sat==0],"\n")
1 2 : 43 63 83 103 123 143
1 3 : 507 1057 1807
1 4 : 4095 12035 26575
1 5 : 25901 105911
2 2 : 1 1 1 1 1 1
2 3 : 1 1 1
2 4 : 1 1 1
2 5 : 1 1
3 2 : 1 1 1 1 1 1
3 3 : 1 1 1
3 4 : 1 1 1
3 5 : 1 1

> E=read_satstat("Marchpl.stats")
> for (t in seq(1,3)) for (k in seq(2,5)) cat(t,k,":",E$t[E$type==t & E$k==k & E$sat==0],"\n")
1 2 : 0.11 0.89 4.17 14.77 41.26 100.15
1 3 : 5.33 90.37 763.13
1 4 : 190.6
1 5 :
2 2 : 0.14 1.24 6.98 13.98 38.89 513.06
2 3 : 617.42
2 4 :
2 5 :
3 2 : 0.04 0.34 1.86 25.01 93.8 228.52
3 3 : 940.86
3 4 :
3 5 :
> for (t in seq(1,3)) for (k in seq(2,5)) cat(t,k,":",E$nds[E$type==t & E$k==k & E$sat==0],"\n")
1 2 : 14 20 25 31 36 42
1 3 : 41 61 81
1 4 : 463
1 5 :
2 2 : 11 13 17 1 1 1
2 3 : 1
2 4 :
2 5 :
3 2 : 19 25 31 1 1 1
3 3 : 1
3 4 :
3 5 :

> E=read_satstat("Glucose.stats")
> for (t in seq(1,3)) for (k in seq(2,5)) cat(t,k,":",E$t[E$type==t & E$k==k & E$sat==0],"\n")
1 2 : 0.001999 0.005999 0.018997 0.035994 0.050992 0.19297
1 3 : 0.051992 0.517921 2.54861
1 4 : 2.95555 73.1469 538.97
1 5 : 102.221 4249.87
2 2 : 0.006998 0.026995 0.101984 0.19497 0.476927 1.01185
2 3 : 0.387941 19.0391 98.67
2 4 : 93.4108 5889.3
2 5 : 12921.5
3 2 : 0.004999 0.027995 0.088986 0.188971 0.470928 0.98285
3 3 : 0.429934 13.074 107.865
3 4 : 73.5918 5556.63
3 5 : 11711.3
> for (t in seq(1,3)) for (k in seq(2,5)) cat(t,k,":",E$cfs[E$type==t & E$k==k & E$sat==0],"\n")
1 2 : 350 853 1371 2017 3003 3969
1 3 : 3642 9976 21567
1 4 : 23265 92344 287335
1 5 : 129523 816139
2 2 : 383 934 1594 2694 3786 6127
2 3 : 2601 8770 22453
2 4 : 16170 73007
2 5 : 83431
3 2 : 400 929 1544 2588 3800 6127
3 3 : 2626 8633 22331
3 4 : 16463 75795
3 5 : 83959

> E=read_satstat("Glucose22.stats")
> for (t in seq(1,3)) for (k in seq(2,5)) cat(t,k,":",E$t[E$type==t & E$k==k & E$sat==0],"\n")
1 2 : 0.003999 0.021996 0.081987 0.226965 0.540917 1.12483
1 3 : 0.169974 2.18467 14.6958
1 4 : 5.80812 159.142 1910.98
1 5 : 152.08 9959.28
2 2 : 0.008998 0.046992 0.285956 0.45693 1.04484 1.3138
2 3 : 0.095985 16.6885 169.929
2 4 : 11.7832 9479.15
2 5 : 4903.1
3 2 : 0.007998 0.044993 0.266959 0.439933 1.04084 1.2868
3 3 : 0.089986 16.3895 180.317
3 4 : 12.4651 7804.1
3 5 : 3115.65
> for (t in seq(1,3)) for (k in seq(2,5)) cat(t,k,":",E$cfs[E$type==t & E$k==k & E$sat==0],"\n")
1 2 : 0 0 0 0 0 0
1 3 : 0 0 0
1 4 : 0 0 0
1 5 : 0 0
2 2 : 117 821 1815 2780 4049 5428
2 3 : 1203 8357 21365
2 4 : 10756 79382
2 5 : 74150
3 2 : 117 821 1810 2855 4034 5428
3 3 : 1203 8725 20823
3 4 : 10558 73212
3 5 : 70752

> E=read_satstat("Glucose22-no.stats")
> for (t in seq(1,3)) for (k in seq(2,5)) cat(t,k,":",E$t[E$type==t & E$k==k & E$sat==0],"\n")
1 2 : 0.001999 0.004999 0.018997 0.037994 0.087986 0.108983
1 3 : 0.071989 0.526919 1.9977
1 4 : 2.80157 43.8503 537.935
1 5 : 115.842 2728.58
2 2 : 0.004999 0.021996 0.067989 0.180972 0.39194 0.780881
2 3 : 0.514921 7.62484 102.642
2 4 : 75.2306 6502.82
2 5 : 9026.88
3 2 : 0.004999 0.019996 0.067989 0.178972 0.387941 0.778881
3 3 : 0.504923 11.4183 108.266
3 4 : 93.5008 5427.91
3 5 : 9719.65
> for (t in seq(1,3)) for (k in seq(2,5)) cat(t,k,":",E$cfs[E$type==t & E$k==k & E$sat==0],"\n")
1 2 : 350 828 1337 2098 2869 4134
1 3 : 3175 9642 21837
1 4 : 21260 85660 269273
1 5 : 118088 700202
2 2 : 372 944 1581 3149 4127 5707
2 3 : 2681 8222 21333
2 4 : 16919 79964
2 5 : 83562
3 2 : 372 944 1586 3153 4072 5751
3 3 : 2711 8333 21424
3 4 : 17800 73399
3 5 : 81337

> E=read_satstat("Picosat.stats")
> for (t in seq(1,3)) for (k in seq(2,5)) cat(t,k,":",E$t[E$type==t & E$k==k & E$sat==0],"\n")
1 2 : 0 0 0 0 0.1 0.1
1 3 : 0.1 0.4 1.5
1 4 : 1.3 15.9 135.7
1 5 : 25.6
2 2 : 0 0 0 0.1 0.1 0.2
2 3 : 0.1 3.6 125.1
2 4 : 44.5 5110.2
2 5 : 5523.5
3 2 : 0 0 0 0 0.1 0.1
3 3 : 0.1 3.8 125.2
3 4 : 53.5 5822.1
3 5 : 6540.2
> for (t in seq(1,3)) for (k in seq(2,5)) cat(t,k,":",E$cfs[E$type==t & E$k==k & E$sat==0],"\n")
1 2 : 254 585 964 1411 2023 2689
1 3 : 2217 6261 13635
1 4 : 13337 54002 152931
1 5 : 69336
2 2 : 254 529 904 1379 1954 2629
2 3 : 2048 7808 20567
2 4 : 16283 73501
2 5 : 83821
3 2 : 254 529 904 1379 1954 2629
3 3 : 2048 7774 20760
3 4 : 16270 73540
3 5 : 83829

> E=read_satstat("Precosat570.stats")
> for (t in seq(1,3)) for (k in seq(2,5)) cat(t,k,":",E$t[E$type==t & E$k==k & E$sat==0],"\n")
1 2 : 0 0 0 0.1 0.1 0.2
1 3 : 0 0.2 1.1
1 4 : 0.5 9.2 94.2
1 5 : 10.1 389
2 2 : 0 0.1 0.4 0.9 2 4.1
2 3 : 0.9 8.8 53.9
2 4 : 30.4 751.9 35356
2 5 : 3635.8
3 2 : 0 0.1 0.4 1 2.2 4.4
3 3 : 0.9 8.7 54.7
3 4 : 29.9 735.8 44808.4
3 5 : 3484.4
> for (t in seq(1,3)) for (k in seq(2,5)) cat(t,k,":",E$cfs[E$type==t & E$k==k & E$sat==0],"\n")
1 2 : 1 1 1 1 1 1
1 3 : 1 1 1
1 4 : 1 1 1
1 5 : 1 1
2 2 : 16 20 560 1438 2398 3493
2 3 : 17 209 17295
2 4 : 857 64688 410510
2 5 : 31092
3 2 : 16 20 560 1438 2398 3493
3 3 : 17 209 17295
3 4 : 857 64688 410510
3 5 : 31092

> E=read_satstat("Lingeling.stats")
> for (t in seq(1,3)) for (k in seq(2,5)) cat(t,k,":",E$t[E$type==t & E$k==k & E$sat==0],"\n")
1 2 : 0 0.2 0.3 0.5 2.1 3.8
1 3 : 0.9 7 54.6
1 4 : 33.2 389.4 3139
1 5 : 479.6 14845.9
2 2 : 0 0 0 0.1 1.4 3.4
2 3 : 0.3 15.6 834.3
2 4 : 201 25004.9
2 5 : 37201.4
3 2 : 0 0 0 0.1 1.2 1.6
3 3 : 0.3 34.8 683.6
3 4 : 411 18593.2
3 5 : 19148.9
> for (t in seq(1,3)) for (k in seq(2,5)) cat(t,k,":",E$cfs[E$type==t & E$k==k & E$sat==0],"\n")
1 2 : 100 100 100 100 1528 2100
1 3 : 655 4470 13585
1 4 : 13324 54187 152934
1 5 : 70177 392047
2 2 : 100 100 100 100 338 835
2 3 : 100 4822 28616
2 4 : 19113 103069
2 5 : 124208
3 2 : 100 100 100 100 343 745
3 3 : 100 4941 29862
3 4 : 20978 119524
3 5 : 117605

> E=read_satstat("Minisat.stats")
> for (t in seq(1,3)) for (k in seq(2,5)) cat(t,k,":",E$t[E$type==t & E$k==k & E$sat==0],"\n")
1 2 : 0.003999 0.021996 0.080987 0.234964 0.546916 1.14083
1 3 : 0.158975 2.06669 14.5708
1 4 : 5.48017 149.517 2117.11
1 5 : 143.273 11635.8
2 2 : 0.006998 0.045993 0.152976 0.597909 1.06584 2.6626
2 3 : 0.100984 15.0077 132.493
2 4 : 10.0605 6380.75
2 5 : 3281.57
3 2 : 0.008998 0.044993 0.142978 0.559914 0.991849 2.42663
3 3 : 0.088986 14.8467 134.714
3 4 : 9.93149 6894.03
3 5 : 3208.5
> for (t in seq(1,3)) for (k in seq(2,5)) cat(t,k,":",E$cfs[E$type==t & E$k==k & E$sat==0],"\n")
1 2 : 0 0 0 0 0 0
1 3 : 0 0 0
1 4 : 0 0 0
1 5 : 0 0
2 2 : 136 740 1529 2778 3887 5417
2 3 : 1276 8333 20033
2 4 : 10427 72123
2 5 : 67344
3 2 : 136 740 1529 2778 3887 5417
3 3 : 1276 8272 19939
3 4 : 10330 70250
3 5 : 66739

> E=read_satstat("Minisat-no.stats")
> for (t in seq(1,3)) for (k in seq(2,5)) cat(t,k,":",E$t[E$type==t & E$k==k & E$sat==0],"\n")
1 2 : 0 0.007998 0.017997 0.032994 0.06499 0.097985
1 3 : 0.077988 0.613906 3.32849
1 4 : 3.39048 62.2055 475.672
1 5 : 74.8946 2633.26
2 2 : 0.005999 0.030995 0.06399 0.164974 0.383941 1.35279
2 3 : 0.529919 10.3664 89.5344
2 4 : 71.8081 5375.87
2 5 : 4044.13
3 2 : 0.003999 0.027995 0.06399 0.153976 0.374943 1.26481
3 3 : 0.516921 10.4744 90.9752
3 4 : 62.2405 4749.34
3 5 : 4057.69
> for (t in seq(1,3)) for (k in seq(2,5)) cat(t,k,":",E$cfs[E$type==t & E$k==k & E$sat==0],"\n")
1 2 : 365 795 1366 1962 2861 3963
1 3 : 3344 10163 23355
1 4 : 23307 88280 232867
1 5 : 109898 608180
2 2 : 416 905 1563 2496 3697 4981
2 3 : 2719 8213 20101
2 4 : 15761 69651
2 5 : 82751
3 2 : 416 905 1563 2496 3697 4981
3 3 : 2683 8217 19817
3 4 : 15761 69651
3 5 : 82751

> E=read_satstat("Cryptominisat296.stats")
> for (t in seq(1,3)) for (k in seq(2,5)) cat(t,k,":",E$t[E$type==t & E$k==k & E$sat==0],"\n")
1 2 : 0.01 0.02 0.04 0.11 0.22 0.45
1 3 : 0.09 0.74 4.83
1 4 : 2.5 57.78 538.63
1 5 : 76.51 4440.04
2 2 : 0 0.01 0.02 0.04 0.07 0.11
2 3 : 0.06 0.4 0.97
2 4 : 1.1 706.17 34460.56
2 5 : 1323.26
3 2 : 0 0.01 0.01 0.03 0.06 0.09
3 3 : 0.05 0.35 1.15
3 4 : 1.36 614.75 34849.85
3 5 : 1282.69
> for (t in seq(1,3)) for (k in seq(2,5)) cat(t,k,":",E$cfs[E$type==t & E$k==k & E$sat==0],"\n")
1 2 : 20 25 53 50 55 56
1 3 : 349 1426 12547
1 4 : 14335 65608 332497
1 5 : 66235 942020
2 2 : 0 0 0 0 0 0
2 3 : 0 0 0
2 4 : 0 30501 114958
2 5 : 30561
3 2 : 0 0 0 0 0 0
3 3 : 0 0 0
3 4 : 0 30500 105312
3 5 : 30500
   \endverbatim
   </li>
   <li> From the look-ahead solvers OKsolver2002 is far best, and also likely
   overall the best. </li>
   <li> Some regressions:
   \verbatim
E = read_satstat("OKsolver-ntp.stats")

Et1 = E[E$type == 1,]
m = lm(Et1$t ~ Et1$l)
summary(m)
     Min       1Q   Median       3Q      Max
-1.32137 -0.13701  0.08398  0.14693  1.84109
              Estimate Std. Error t value Pr(>|t|)
(Intercept) -1.726e-01  2.118e-01  -0.815    0.431
Et1$l        1.309e-06  2.902e-08  45.125 9.14e-15 ***
Residual standard error: 0.713 on 12 degrees of freedom
Multiple R-squared: 0.9941,     Adjusted R-squared: 0.9937
F-statistic:  2036 on 1 and 12 DF,  p-value: 9.145e-15

plot(Et1$l,Et1$t)
lines(Et1$l,predict(m))

Et23 = E[E$type != 1,]
v = as.double(Et23$l) * as.double(Et23$n)
m23 = lm(Et23$t ~ v)
summary(m23)
    Min      1Q  Median      3Q     Max
-5128.9  -105.6  -102.1  -101.9  4055.2
             Estimate Std. Error t value Pr(>|t|)
(Intercept) 1.019e+02  2.838e+02   0.359    0.723
v           5.883e-10  2.072e-11  28.398   <2e-16 ***
Residual standard error: 1424 on 26 degrees of freedom
Multiple R-squared: 0.9688,     Adjusted R-squared: 0.9676
F-statistic: 806.4 on 1 and 26 DF,  p-value: < 2.2e-16

plot(Et23$t)
lines(predict(m23))
   \endverbatim
   </li>
  </ul>

*/
