// Matthew Gwynne, 25.5.2011 (Swansea)
/* Copyright 2011, 2012 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Investigations/Cryptography/DataEncryptionStandard/plans/KeyDiscovery/4.hpp
  \brief On investigations into the 4-round Data Encryption Standard key discovery


  \todo Overview
  <ul>
   <li> We consider the 4 round DES given by the encryption function
   des_encryption_gen in
   ComputerAlgebra/Cryptology/Lisp/CryptoSystems/DataEncryptionStandard/Cipher.mac.
   </li>
   <li> The translation of one round DES to SAT is given at the Maxima level by
   des_fcl_gen in
   ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/DataEncryptionStandard/GeneralisedConstraintTranslation.mac.
   </li>
   <li> The DES consists of certain rewiring of the bits, additions (XOR) and
   the application of 8 S-boxes (substitution boxes) for each round. </li>
   <li> We consider the DES S-boxes as 6x4 boolean functions,
   given by des_sbox_bf in
   ComputerAlgebra/Cryptology/Lisp/CryptoSystems/DataEncryptionStandard/Sboxes.mac.
   </li>
   <li> We should also consider the DES S-boxes as 4 6x1 functions.
   See "Basic translation" in
   Investigations/Cryptography/DataEncryptionStandard/plans/general.hpp. </li>
   <li> We translate the DES by treating the additions and S-boxes as the
   boolean functions, which we consider our units of translation. </li>
   <li> The additions are translated by the set of their prime implicates.
   </li>
   <li> The S-boxes are translated using each of the following CNF
   representations:
    <ul>
     <li> canonical(+) representation, see dualts_fcl and dualtsplus_fcl in
     ComputerAlgebra/Satisfiability/Lisp/FiniteFunctions/TseitinTranslation.mac;
     </li>
     <li> 1-base translations, see
     Investigations/Cryptography/DataEncryptionStandard/plans/Sboxes/general.hpp;
     </li>
     <li> minimum translations, see
     Investigations/Cryptography/DataEncryptionStandard/plans/Sboxes/general.hpp;
     </li>
     <li> their prime implicates; </li>
     <li> their canonical CNF representations. </li>
    </ul>
   All such translations apply to both the 6x4 S-box functions and
   the 4 decomposed 6x1 functions.
   </li>
   <li> For initial experiments we use the Argosat-desgen
   plaintext-ciphertext-pairs. See "Transferring the Argosat-desgen example"
   in Investigations/Cryptography/DataEncryptionStandard/plans/KeyDiscovery/KnownKeyBits.hpp.
   </li>
   <li> Solving 20 random keys (avg number of conflicts/nodes):
    <ul>
     <li> 1-base (minisat-2.2.0: 982,518.8, OKsolver_2002: 4,134,272). </li>
     <li> canonical (minisat-2.2.0: 1,050,555, OKsolver_2002: 56,743). </li>
     <li> Massacci DES translator (minisat-2.2.0: 1,118,152). </li>
     <li> minimum (minisat-2.2.0: 1,331,979, OKsolver_2002:  6,566,953). </li>
    </ul>
   </li>
   <li> Solving 20 random keys (time in seconds):
    <ul>
     <li> minimum (minisat-2.2.0: 31.2, OKsolver_2002:  1551.3). </li>
     <li> 1-base (minisat-2.2.0: 35.9, OKsolver_2002: 1,974.8). </li>
     <li> Massacci DES translator (minisat-2.2.0: 74.42). </li>
     <li> canonical (minisat-2.2.0: 305.66, OKsolver_2002: 3,200.5). </li>
    </ul>
   </li>
  </ul>


  \todo Using the Massacci DES translator
  <ul>
   <li> Generating plaintext and ciphertext pairs and instances:
   \verbatim
> oklib --maxima
maxima> oklib_load_all()$

  format_massacci(hex) :=
    apply(sconcat,map(lambda([s],sconcat("0x",apply(sconcat,s),"_")),partition_elements(charlist(hex),2)))$

  rounds : 4$ with_stdout(sconcat("des_r",rounds,"_pkc_triples"),
    for seed : 1 thru 20 do block(
      set_random(make_random_state(seed)),
      P_hex : lpad(int2hex(random(2**64)),"0",16),
      K_hex : lpad(int2hex(random(2**64)),"0",16),
      C_hex : des_encryption_hex_gen(rounds, P_hex,K_hex),
      print(sconcat(
        format_massacci(P_hex), " ",
        format_massacci(K_hex)," ",
        format_massacci(C_hex)))))$
   \endverbatim
   </li>
   <li> Generating the instances:
   \verbatim
ExternalSources/builds/SAT/Des/des2fml-0.9> mkdir experiments && cd experiments
experiments> rounds=4; s=1;
cat des_r${rounds}_pkc_triples | grep -v '^ *$' | while read p k c; do
  echo ${p} | sed -e 's/_/ /g' > plaintxt; echo ${k} | sed -e 's/_/ /g' > key_des; rm ciph_txt
  ./des -r${rounds} && ./des2fml -r${rounds} -p -c -f1 && ./clausify formulae des_massacci_r${rounds}_s${s}.cnf
  let s=$s+1
done
   \endverbatim
   </li>
   <li> Running the solver:
   \verbatim
experiments> rounds=4;
for s in $(seq 1 20); do
  minisat-2.2.0 des_massacci_r${rounds}_s${s}.cnf > minisat_r${rounds}_s${s}.result;
done;
   \endverbatim
   </li>
   <li> Results:
   \verbatim
> rounds=4; results_file=r${rounds}_minisat.results; ExtractMinisat header-only | awk ' { print $0 " s" } ' > ${results_file};
for s in $(seq 1 20); do cat minisat_r${rounds}_s${s}.result | ExtractMinisat data-only | awk " { print  \$0 \" ${s}\" }" >> ${results_file}; done

> oklib --R
> E = read.table("r4_minisat.results", header=TRUE)
> summary(E)
      rc              t               cfs               dec               rts             r1                ptime            stime            cfl
Min.   :14812   Min.   :  8.97   Min.   : 169646   Min.   : 200392   Min.   : 460   Min.   : 41489684   Min.   :0.0000   Min.   :0.020   Min.   : 3487867
1st Qu.:14812   1st Qu.: 49.07   1st Qu.: 792121   1st Qu.: 889442   1st Qu.:1686   1st Qu.:211985394   1st Qu.:0.0000   1st Qu.:0.030   1st Qu.:15039986
Median :14812   Median : 66.81   Median :1046956   Median :1170468   Median :2046   Median :283783523   Median :0.0000   Median :0.030   Median :19814716
Mean   :14812   Mean   : 74.42   Mean   :1118152   Mean   :1253541   Mean   :2210   Mean   :310448339   Mean   :0.0015   Mean   :0.031   Mean   :21568194
3rd Qu.:14812   3rd Qu.: 97.62   3rd Qu.:1489823   3rd Qu.:1649706   3rd Qu.:2901   3rd Qu.:408642168   3rd Qu.:0.0000   3rd Qu.:0.030   3rd Qu.:27999495
Max.   :14812   Max.   :198.64   Max.   :2647135   Max.   :3118703   Max.   :4604   Max.   :777567318   Max.   :0.0100   Max.   :0.040   Max.   :54642660


>  sd(E)
           t          cfs          dec          rts           r1        ptime        stime          cfl 
4.299050e+01 5.766169e+05 6.606163e+05 1.002503e+03 1.674194e+08 3.663475e-03 5.525063e-03 1.141013e+07
   \endverbatim
   </li>
  </ul>



  \todo Using the canonical translation for the S-boxes (6-to-4)
  <ul>
   <li> Translating the DES Sboxes, as 6x4 boolean functions, using the
   canonical representation. That is, each Sbox is represented with the
   canonical representation given by dualts_fcl in
   ComputerAlgebra/Satisfiability/Lisp/FiniteFunctions/TseitinTranslation.mac.
   </li>
   <li> Generating the instance:
   \verbatim
shell> mkdir -p des_4/canon
shell> cd des_4/canon
shell> oklib --maxima
oklib_load_all()$
rounds : 4$
sbox_fcl_l : create_list(dualts_fcl([listify(setn(10)), des_sbox_fulldnf_cl(i)]), i, 1, 8)$
for seed : 1 thru 20 do block(
  print(sconcat("Generating ", rounds, "-round DES with seed ", seed)),
  set_random(make_random_state(seed)),
  P_hex : lpad(int2hex(random(2**64)),"0",16),
  K_hex : lpad(int2hex(random(2**64)),"0",16),
  C_hex : des_encryption_hex_gen(rounds, P_hex,K_hex),
  P : des_plain2fcl_gen(hexstr2binv(P_hex),rounds),
  C : des_cipher2fcl_gen(hexstr2binv(C_hex),rounds),
  F : des2fcl_gen(sbox_fcl_l,rounds),
  Fs : standardise_fcl([F[1],append(F[2],P[2],C[2])]),
  output_fcl_v(
    sconcat(rounds, "-round DES instantiated with plaintext and ciphertext generated from seed ", seed, "; translated using the canonical translation for the S-boxes (6-to-4)."),
    Fs[1],
    sconcat("des_6t4_canon_r",rounds,"_s",seed,".cnf"),
    Fs[2]))$
print("DONE!");
   \endverbatim
   </li>
   <li> Statistics:
   \verbatim
shell> cat des_6t4_canon_r4.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG n
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
2624 23968 69504 0 69504 2625 1
 length count
1 128
2 20480
3 1280
11 2048
64 32
   \endverbatim
   </li>
   <li> S-box statistics (canonical translation):
   \verbatim
ncl_list_fcl(dualts_fcl([listify(setn(10)), des_sbox_fulldnf_cl(1)]));
[[2,640],[11,64],[64,1]]
   \endverbatim
   </li>
   <li> We have the following number of clauses of the following sizes:
    <ul>
     <li> 128 unit-clauses (setting plaintext + ciphertext); </li>
     <li> 20480 binary clauses (8 * 4 = 32 S-boxes); </li>
     <li> 1280 ternary clauses (80 * 4 = 320 binary additions); </li>
     <li> 2048 clauses of length eleven (8 * 4 = 32 S-boxes); </li>
     <li> 32 clauses of length 64 (8 * 4 = 32 S-boxes). </li>
    </ul>
   </li>
   <li> Running minisat-2.2.0 on these instances:
   \verbatim
shell> r=4;
shell> for k in $(seq 1 20); do
    echo "Round ${r}; Key Seed ${k}; Random Seed ${s}...";
    minisat-2.2.0 des_6t4_canon_r${r}_s${k}.cnf > minisat_r${r}_k${k}.result 2>&1;
  done;
done;
shell> echo "rn  rc  t  sat  cfs dec rts r1 mem ptime stime cfl r k" > minisat_results;
for k in $(seq 1 20); do
    cat minisat_r${r}_k${k}.result | ExtractMinisat | awk " { print \$0 \"  $r  $k\" }";
done >> minisat_results;
   \endverbatim
   yields:
   \verbatim
shell> oklib --R
E = read.table("minisat_results", header=TRUE)
EM = aggregate(E, by=list(r=E$r), FUN=mean)
EM
  r    rn     rc      t sat     cfs     dec     rts        r1  mem  ptime stime
1 4 2624 23840 305.66   1 1050555 1986753 2021.65 344645057 71.6 0.0105  0.01
        cfl r    k
1 147441100 4 10.5
   </li>
   <li> Running OKsolver_2002 on these instances:
   \verbatim
shell> r=4;
shell> for k in $(seq 1 20); do
    echo "Round ${r}; Key Seed ${k}...";
    OKsolver_2002-O3-DNDEBUG des_6t4_canon_r${r}.cnf > oksolver_r${r}_k${k}.result 2>&1;
done;
shell> echo "n  c  l  t  sat  nds  r1  r2  pls  ats h file n2cr  dmcl dn  dc  dl snds qnds mnds  tel  oats  n2cs  m2cs r k" > oksolver_results;
for k in $(seq 1 20); do
    OKP=~/Work/OKlibrary/OKplatform/; cat oksolver_r${r}_k${k}.result | awk -f ${OKP}/OKsystem/OKlib/Experimentation/ExperimentSystem/SolverMonitoring/ExtractOKsolver.awk | awk " { print \$0 \"  $r  $k\" }";
done >> oksolver_results;
   \endverbatim
   yields:
   \verbatim
shell> oklib --R
E = read.table("oksolver_results", header=TRUE)
EM = aggregate(E, by=list(r=E$r), FUN=mean)
EM
  r    n     c     l        t sat     nds  r1      r2 pls  ats    h file  n2cr
1 4 2616 23968 69504 3200.494   1 56743.6 128 6521081   0 0.05 26.2   NA 20928
  dmcl  dn  dc   dl snds qnds mnds tel oats n2cs m2cs r    k
1    0 128 576 1920 0.25    0  0.3   0    0    0    0 4 10.5
   \endverbatim
   </li>
  </ul>


  \todo Using the 1-base translations for the S-boxes (6-to-4)
  <ul>
   <li> Translating the DES Sboxes, as 6-to-4 bit boolean functions, using
   1-bases. </li>
   <li> Generating the 1-bases:
   \verbatim
maxima> for i : 1 thru 8 do output_dessbox_fullcnf_stdname(i)$
shell> gen_seed[1]=7;gen_seed[2]=71;gen_seed[3]=185;gen_seed[4]=346;gen_seed[5]=67;gen_seed[6]=327;gen_seed[7]=148;gen_seed[8]=167;
shell> base_seed[1]=1;base_seed[2]=1;base_seed[3]=2;base_seed[4]=4;base_seed[5]=2;base_seed[6]=1;base_seed[7]=2;base_seed[8]=1;
shell> for i in $(seq 1 8); do
  QuineMcCluskey-n16-O3-DNDEBUG DES_Sbox_${i}_fullCNF.cnf > DES_Sbox_${i}_pi.cnf;
  RandomShuffleDimacs-O3-DNDEBUG ${gen_seed[$i]} < DES_Sbox_${i}_pi.cnf | SortByClauseLength-O3-DNDEBUG > DES_Sbox_${i}_sortedpi.cnf;
  RUcpGen-O3-DNDEBUG DES_Sbox_${i}_sortedpi.cnf > DES_Sbox_${i}_gen.cnf;
  RandomShuffleDimacs-O3-DNDEBUG ${base_seed[$i]}  < DES_Sbox_${i}_gen.cnf | SortByClauseLengthDescending-O3-DNDEBUG | RUcpBase-O3-DNDEBUG > DES_Sbox_${i}_1base.cnf;
done
   \endverbatim
   </li>
   <li> The numbers of clauses in the 1-bases are 124, 129, 138, 128, 134,
   136, 123, 152 respectively. </li>
   <li> All the 1-bases used have clauses of sizes 5 and 6, except Sbox 4
   which has clauses of size 5 and 6 as well as 2 of size 7. </li>
   <li> Generating the instance:
   \verbatim
rounds : 4$
sbox_fcl_l : create_list(read_fcl_f(sconcat("DES_Sbox_",i,"_1base.cnf")), i, 1, 8)$
for seed : 1 thru 20 do block(
  print(sconcat("Generating ", rounds, "-round DES with seed ", seed)),
  set_random(make_random_state(seed)),
  P_hex : lpad(int2hex(random(2**64)),"0",16),
  K_hex : lpad(int2hex(random(2**64)),"0",16),
  C_hex : des_encryption_hex_gen(rounds, P_hex,K_hex),
  P : des_plain2fcl_gen(hexstr2binv(P_hex),rounds),
  C : des_cipher2fcl_gen(hexstr2binv(C_hex),rounds),
  F : des2fcl_gen(sbox_fcl_l,rounds),
  Fs : standardise_fcl([F[1],append(F[2],P[2],C[2])]),
  output_fcl_v(
  sconcat(rounds, "-round DES instantiated with plaintext and ciphertext generated from seed ", seed, "; translated using the 1-base translation for the S-boxes (6-to-4)."),
    Fs[1],
    sconcat("des_6t4_1base_r",rounds,"_s",seed,".cnf"),
    Fs[2]))$
print("DONE!");
   \endverbatim
   </li>
   <li> Statistics:
   \verbatim
shell> cat des_6t4_1base_r4.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG n
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
576 5664 27076 0 27076 577 1
 length count
1 128
3 1280
5 2432
6 1820
7 4
   \endverbatim
   </li>
   <li> S-box statistics (1-base translations):
   \verbatim
for F in sbox_fcl_l do print(ncl_list_fcl(F));
[[5,84],[6,39],[7,1]]
[[5,75],[6,54]]
[[5,76],[6,62]]
[[5,69],[6,59]]
[[5,78],[6,56]]
[[5,83],[6,53]]
[[5,75],[6,48]]
[[5,68],[6,84]]
  \endverbatim
   </li>
   <li> We have the following number of clauses of the following sizes:
    <ul>
     <li> 128 unit-clauses (setting plaintext + ciphertext); </li>
     <li> 1280 ternary clauses (80 * 4 = 32 binary additions); </li>
     <li> 2432 clauses of length 5 (8 * 4 = 32 S-boxes); </li>
     <li> 1820 clauses of length 6 (8 * 4 = 32 S-boxes); </li>
     <li> 4 clauses of length seven (1 * 4 = 4 S-boxes). </li>
    </ul>
   </li>
   <li> Running minisat-2.2.0 on these instances:
   \verbatim
shell> r=4;
shell> for k in $(seq 1 20); do
    echo "Round ${r}; Key Seed ${k}; Random Seed ${s}...";
    minisat-2.2.0 des_6t4_1base_r${r}_s${k}.cnf > minisat_r${r}_k${k}.result 2>&1;
  done;
done;
shell> echo "rn  rc  t  sat  cfs dec rts r1 mem ptime stime cfl r k" > minisat_results;
for k in $(seq 1 20); do
    cat minisat_r${r}_k${k}.result | ExtractMinisat data-only | awk " { print \$0 \"  $r  $k\" }";
done >> minisat_results;
   \endverbatim
   yields (original results):
   \verbatim
shell> oklib --R
E = read.table("minisat_results", header=TRUE)
EM = aggregate(E, by=list(r=E$r), FUN=mean)
EM
  r   rn    rc       t sat      cfs     dec    rts       r1 mem  ptime stime
1 4 576 5536 35.9002   1 982518.8 1116844 1927.3 71803310 8.8 0.0035  0.01
       cfl r    k
1 18830930 4 10.5
   \endverbatim
   </li>
   <li> Running OKsolver_2002 on these instances:
   \verbatim
shell> r=4;
shell> for k in $(seq 1 20); do
    echo "Round ${r}; Key Seed ${k}...";
    OKsolver_2002-O3-DNDEBUG des_6t4_1base_r${r}.cnf > oksolver_r${r}_k${k}.result 2>&1;
done;
shell> echo "n  c  l  t  sat  nds  r1  r2  pls  ats h file n2cr  dmcl dn  dc  dl snds qnds mnds  tel  oats  n2cs  m2cs r k" > oksolver_results;
for k in $(seq 1 20); do
    OKP=~/Work/OKlibrary/OKplatform/; cat oksolver_r${r}_k${k}.result | awk -f ${OKP}/OKsystem/OKlib/Experimentation/ExperimentSystem/SolverMonitoring/ExtractOKsolver.awk | awk " { print \$0 \"  $r  $k\" }";
done >> oksolver_results;
   \endverbatim
   yields:
   \verbatim
shell> oklib --R
E = read.table("oksolver_results", header=TRUE)
EM = aggregate(E, by=list(r=E$r), FUN=mean)
EM
  r   n    c     l        t sat     nds  r1       r2 pls ats    h file n2cr
1 4 568 5664 27076 1974.769   1 4134272 128 10648856   0 0.1 35.7   NA  448
  dmcl  dn  dc   dl   snds qnds   mnds tel oats n2cs m2cs r    k
1    0 128 576 1920 1528.5    0 4259.2   0    0    0    0 4 10.5
   \endverbatim
   </li>
  </ul>


  \todo Using the "minimum" translation for the S-boxes (6-to-4)
  <ul>
   <li> Translating the DES Sboxes, as 6-to-4 bit boolean functions, using the
   "minimum" (inf-based) representations. </li>
   <li> Generating the "minimum" CNFs for the Sboxes:
   \verbatim
maxima> for i : 1 thru 8 do output_dessbox_fullcnf_stdname(i)$
shell> for i in $(seq 1 8); do
  QuineMcCluskeySubsumptionHypergraph-n16-O3-DNDEBUG DES_Sbox_${i}_fullCNF.cnf > DES_Sbox_${i}_shg.cnf;
  cat DES_Sbox_${i}_shg.cnf | MinOnes2WeightedMaxSAT-O3-DNDEBUG > DES_Sbox_${i}_shg.wcnf;
done
shell> ubcsat-okl  -alg gsat -w -runs 100 -cutoff 400000 -wtarget 67 -solve 1 -seed 2444475534 -i DES_Sbox_1_shg.wcnf -r model DES_Sbox_1.ass;
shell> ubcsat-okl  -alg gsat -w -runs 100 -cutoff 400000 -wtarget 67 -solve 1 -seed 2521057446 -i DES_Sbox_2_shg.wcnf -r model DES_Sbox_2.ass;
shell> ubcsat-okl  -alg gsat -w -runs 100 -cutoff 400000 -wtarget 68 -solve 1 -seed 3544367510 -i DES_Sbox_3_shg.wcnf -r model DES_Sbox_3.ass;
shell> ubcsat-okl  -alg gsat -w -runs 100 -cutoff 400000 -wtarget 69 -solve 1 -seed 3808694681 -i DES_Sbox_4_shg.wcnf -r model DES_Sbox_4.ass;
shell> ubcsat-okl  -alg gsat -w -runs 100 -cutoff 400000 -wtarget 67 -solve 1 -seed 1876503362 -i DES_Sbox_5_shg.wcnf -r model DES_Sbox_5.ass;
shell> ubcsat-okl  -alg gsat -w -runs 100 -cutoff 400000 -wtarget 66 -solve 1 -seed 68018538 -i DES_Sbox_6_shg.wcnf -r model DES_Sbox_6.ass;
shell> ubcsat-okl  -alg gsat -w -runs 100 -cutoff 400000 -wtarget 67 -solve 1 -seed 1856244582 -i DES_Sbox_7_shg.wcnf -r model DES_Sbox_7.ass;
shell> ubcsat-okl  -alg gsat -w -runs 100 -cutoff 400000 -wtarget 69 -solve 1 -seed 4223500633 -i DES_Sbox_8_shg.wcnf -r model DES_Sbox_8.ass;
shell> for i in $(seq 1 8); do
  cat DES_Sbox_${i}_fullCNF.cnf_primes | FilterDimacs DES_Sbox_${i}.ass > DES_Sbox_${i}_min.cnf;
done
   \endverbatim
   </li>
   <li> The numbers of clauses in the CNFs are 67, 67, 68, 69, 67, 66, 67, and
   69 respectively. </li>
   <li> Generating the instance:
   \verbatim
rounds : 4$
sbox_fcl_l : create_list(read_fcl_f(sconcat("DES_Sbox_",i,"_min.cnf")), i, 1, 8)$
for seed : 1 thru 20 do block(
  print(sconcat("Generating ", rounds, "-round DES with seed ", seed)),
  set_random(make_random_state(seed)),
  P_hex : lpad(int2hex(random(2**64)),"0",16),
  K_hex : lpad(int2hex(random(2**64)),"0",16),
  C_hex : des_encryption_hex_gen(rounds, P_hex,K_hex),
  P : des_plain2fcl_gen(hexstr2binv(P_hex),rounds),
  C : des_cipher2fcl_gen(hexstr2binv(C_hex),rounds),
  F : des2fcl_gen(sbox_fcl_l,rounds),
  Fs : standardise_fcl([F[1],append(F[2],P[2],C[2])]),
  output_fcl_v(
  sconcat(rounds, "-round DES instantiated with plaintext and ciphertext generated from seed ", seed, "; translated using the minimum translation for the S-boxes (6-to-4)."),
    Fs[1],
    sconcat("des_6t4_min_r",rounds,"_s",seed,".cnf"),
    Fs[2]))$
print("DONE!");
   \endverbatim
   </li>
   <li> Statistics:
   \verbatim
shell> cat des_6t4_min_r4.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG n
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
576 3568 16048 0 16048 577 1
 length count
1 128
3 1280
5 928
6 1184
7 48
   \endverbatim
   </li>
   <li> S-box statistics ("minimum" translation):
   \verbatim
for F in sbox_fcl_l do print(ncl_list_fcl(F));

[[5,30],[6,35],[7,2]]
[[5,33],[6,33],[7,1]]
[[5,28],[6,38],[7,2]]
[[5,29],[6,38],[7,2]]
[[5,29],[6,36],[7,2]]
[[5,28],[6,38]]
[[5,29],[6,37],[7,1]]
[[5,26],[6,41],[7,2]]
  \endverbatim
   </li>
   <li> We have the following number of clauses of the following sizes:
    <ul>
     <li> 128 unit-clauses (setting plaintext + ciphertext); </li>
     <li> 1280 ternary clauses (80 * 4 = 320 binary additions); </li>
     <li> 928 clauses of length 5 (8 * 4 = 32 S-boxes); </li>
     <li> 1184 clauses of length 6 (8 * 4 = 32 S-boxes); </li>
     <li> 48 clauses of length seven (7 * 4 = 28 S-boxes). </li>
    </ul>
   </li>
   <li> Running minisat-2.2.0 on these instances:
   \verbatim
shell> r=4;
shell> for k in $(seq 1 20); do
    echo "Round ${r}; Key Seed ${k}; Random Seed ${s}...";
    minisat-2.2.0 des_6t4_min_r${r}_s${k}.cnf > minisat_r${r}_k${k}.result 2>&1;
  done;
done;
shell> echo "rn  rc  t  sat  cfs dec rts r1 mem ptime stime cfl r k" > minisat_results;
for k in $(seq 1 20); do
    OKP=~/Work/OKlibrary/OKplatform/; cat minisat_r${r}_k${k}.result | ExtractMinisat data-only | awk " { print \$0 \"  $r  $k\" }";
done >> minisat_results;
   \endverbatim
   yields:
   \verbatim
shell> oklib --R
E = read.table("minisat_results", header=TRUE)
EM = aggregate(E, by=list(r=E$r), FUN=mean)
EM
  r   rn    rc        t sat     cfs     dec     rts       r1 mem ptime stime
1 4 576 3440 31.20535   1 1331979 1655787 2558.05 79038583 8.2     0 0.001
       cfl r    k
1 24357557 4 10.5
   \endverbatim
   </li>
   <li> Running OKsolver_2002 on these instances:
   \verbatim
shell> r=4;
shell> for k in $(seq 1 20); do
    echo "Round ${r}; Key Seed ${k}...";
    OKsolver_2002-O3-DNDEBUG des_6t4_min_r${r}.cnf > oksolver_r${r}_k${k}.result 2>&1;
done;
shell> echo "n  c  l  t  sat  nds  r1  r2  pls  ats h file n2cr  dmcl dn  dc  dl snds qnds mnds  tel  oats  n2cs  m2cs r k" > oksolver_results;
for k in $(seq 1 20); do
    OKP=~/Work/OKlibrary/OKplatform/; cat oksolver_r${r}_k${k}.result | awk -f ${OKP}/OKsystem/OKlib/Experimentation/ExperimentSystem/SolverMonitoring/ExtractOKsolver.awk | awk " { print \$0 \"  $r  $k\" }";
done >> oksolver_results;
   \endverbatim
   yields:
   \verbatim
shell> oklib --R
E = read.table("oksolver_results", header=TRUE)
EM = aggregate(E, by=list(r=E$r), FUN=mean)
EM
  r   n    c     l        t sat     nds  r1       r2 pls ats     h file n2cr
1 4 568 3568 16048 1551.255   1 6566953 128 17938032   0 0.1 38.35   NA  448
  dmcl  dn  dc   dl    snds qnds    mnds tel oats n2cs m2cs r    k
1    0 128 576 1920 7552.55    0 13192.2   0  9.7    0    0 4 10.5
   \endverbatim
   </li>
  </ul>

*/
