// Matthew Gwynne, 27.3.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/Cryptography/AES/plans/general.hpp
  \brief On investigations into the Advanced Encryption Standard


  \todo Improve formulations : MG (as soon as possible)
  <ul>
   <li> No "is useful". </li>
   <li> No "may be used". </li>
   <li> Clean-up minisat output (of course, no intermediate results). </li>
   <li> More care! (Of course, the doxygen-output needs to be inspected) </li>
   <li> Explanations needed! </li>
   <li> And discussions! </li>
  </ul>


  \todo More experiments : MG (as soon as possible)
  <ul>
   <li> For the trivial instances (message and key set) more solvers need to
   be considered. In its own file. </li>
   <li> Also ciphertext and key set are to be investigated. </li>
   <li> What is the meaning of
   "Experiments on AES with the first 32 key bits not set" ? And what does
   the "summary" show? Apparently only trivial results, which are likely of
   no interest. </li>
   <li> Plan for more experiments! </li>
  </ul>


  \todo Generate problem instances
  <ul>
   <li> To generate instances of AES as a SAT problem where the primary
   variables are the plaintext, key and ciphertext (each 128 variables),
   the following code is useful. 
   \verbatim
removeTopNVars(C,n) := subset(C, lambda([a], is(abs(a) <= (uaapply(max,listify(map(abs,C))) - n))));

hex2il(h) := block([cl],
  cl : charlist(h),
  print(cl),
  return(map(lambda([b], hex2int(simplode(b))), partition_elements(cl,2)))
)$

gen_uc_ass_aes(ph, pbn, kh ,kbn,ch, cbn) := block(
  return(
    map(lambda([a],{a}),union(
      removeTopNVars(AESHexToPA(ph,aes_make_vars_int("p",1,128)),pbn),
      removeTopNVars(AESHexToPA(kh,aes_make_vars_int("k",1,128)),kbn),
      removeTopNVars(AESHexToPA(ch,aes_make_vars_int("c",1,128)),cbn)))));

gen_uc_ass_aes_comp(ph, pbn, kh ,kbn, cbn) := block([ch],
 ch : il2hex(transpose_l(
         aes_encrypt_l(
           hex2il(ph),
           hex2il(kh)),
         4)),
 return(gen_uc_ass_aes(ph,pbn,kh,kbn,ch,cbn))
)$
plaintext_hex : "32488853038D31734198AA2E0370D450"$
key_hex : "3F6A2B7E151628AED2A6ABF7158809CF"$
for r from 2 step 1 thru 10 do block([aes_num_rounds:r],
  aes_cs : aes_cnf_fcs(),
  output_cs_f(sconcat("AES r=",r," NPKC"),aes_cs,sconcat("AES_r",r,".cnf")),
  ch: apply(sconcat,map(lambda([s],lpad(int2hex(s),"0",2)),
    aes_encrypt_l(
      hex2il(plaintext_hex),
      hex2il(key_hex)))
    ),
  for pn from 0 thru 0 do (
    for kn from 0 step 2 thru 64 do (
      for cn from 0 thru 0 do block(
        output_cs_f(
          sconcat("AES UC R=",r," PN=",pn,"KN=",kn,"CN=",cn,
            "P=",plaintext_hex,"K=",key_hex),
          cs_to_fcs(gen_uc_ass_aes(plaintext_hex,pn,key_hex,kn,ch,cn)),
          sconcat("AES_UC_r",r,"_pn",pn,"_kn",kn,"_cn",cn,
            "_P",plaintext_hex,"_K",key_hex,".cnf")),
        print(sconcat("AES UC R=",r," PN=",pn,"KN=",kn,"CN=",cn,
           " P=",plaintext_hex,"K=",key_hex))))));
   \endverbatim
   The above code generates files "AES_r2.cnf" (for round 2 for example) 
   containing the main AES translation (with no variable assignments) and then 
   "AES_UC_r2_pn0_kn0_cn0_P32488853038D31734198AA2E0370D450_K3F6A2B7E151628AED2A6ABF7158809CF.cnf"
   as the unit clauses for setting up a 2 round AES translation with all 
   plaintext, key and ciphertext bits set and the plaintext and key
   provided as "32488853038D31734198AA2E0370D450" and
   "3F6A2B7E151628AED2A6ABF7158809CF" in the standard AES format.
   </li>
   <li> To merge the AES translation cnf with the assignment unit clauses 
   generated, the following script may be used : 
   \verbatim
#!/bin/bash

# Grab the number of clauses from each
CL1=`grep "^p" $1  | cut -d " " -f "4"`
CL2=`grep "^p" $2  | cut -d " " -f "4"`

NewCL=`expr $CL1 + $CL2`
cat $1 | sed -e "s/p \+\([a-zA-Z]\+\) \+\([0-9]\+\).*$/p \1 \2 $NewCL/"
cat $2 | grep -v "^p"
   \endverbatim
   called "mergeSameVarCNFs.sh" and can be used in the following way : 
   \verbatim
./mergeSameVarCNFs.sh AES_r2.cnf AES_UC_r2_pn0_kn32_cn0_P00000000000000000000000000000000_K00000000000000000000000000000000.cnf > AES_r2_kn_32_P00000000000000000000000000000000_K00000000000000000000000000000000_SAT.cnf   
   \endverbatim
   </li>
   <li> For experimentation, the following is then an example for 
   running a particular SAT solver (minisat in the example below) on the given
   problem instance :
   \verbatim
for k in `seq 0 32 128`; do
  for r in `seq 2 2 10`; do
    echo "K = " $k "R = " $r;
    ./mergeSameVarCNFs.sh "AES_r${r}.cnf" "AES_UC_r${r}_pn0_kn${k}_cn0_P00000000000000000000000000000000_K00000000000000000000000000000000.cnf" > "AES_r${r}_k   n_${k}_P00000000000000000000000000000000_K00000000000000000000000000000000_SAT.cnf" ;
    time ./solvers/minisat/core/minisat "AES_r${r}_kn_${k}_P00000000000000000000000000000000_K00000000000000000000000000000000_SAT.cnf" > "AES_r${r}_kn_${k}_P   00000000000000000000000000000000_K00000000000000000000000000000000_SAT.cnf.results.minisat" 2> "AES_r${r}_kn_${k}_P00000000000000000000000000000000_K000000000   00000000000000000000000_SAT.cnf.results.minisat" ;
  done
done
   \endverbatim
   </li>
  </ul>


  \todo Computing AES ciphertext given plaintext and key
  <ul>
   <li> Using minisat2 :
   <ul>
    <li> Assignments have been generated and verified but are not included for
    brevity. </li>
    <li> r=2 with zeroed plaintext and zeroed key:
    \verbatim
SATISFIABLE
Sat 2.0 beta
WARNING: for repeatability, setting FPU to use double precision
============================[ Problem Statistics ]=============================
|                                                                             |
|  Number of variables:  2016                                                 |
|  Number of clauses:    29000                                                |
|  Parsing time:         0.02         s                                       |
============================[ Search Statistics ]==============================
| Conflicts |          ORIGINAL         |          LEARNT          | Progress |
|           |    Vars  Clauses Literals |    Limit  Clauses Lit/Cl |          |
===============================================================================
|         0 |    1744    17728   103288 |     5909        0    nan |  0.000 % |
===============================================================================
Verified 17728 original clauses.
restarts              : 1
conflicts             : 75             (2344 /sec)
decisions             : 525            (1.14 % random) (16405 /sec)
propagations          : 11002          (343791 /sec)
conflict literals     : 161            (2.42 % deleted)
Memory used           : 16.67 MB
CPU time              : 0.032002 s
    \endverbatim
    </li>
    <li> r=4 with zeroed plaintext and zeroed key:
    \verbatim
SATISFIABLE
Sat 2.0 beta
WARNING: for repeatability, setting FPU to use double precision
============================[ Problem Statistics ]=============================
|                                                                             |
|  Number of variables:  4416                                                 |
|  Number of clauses:    59696                                                |
|  Parsing time:         0.04         s                                       |
============================[ Search Statistics ]==============================
| Conflicts |          ORIGINAL         |          LEARNT          | Progress |
|           |    Vars  Clauses Literals |    Limit  Clauses Lit/Cl |          |
===============================================================================
|         0 |    4128    48376   283660 |    16125        0    nan |  0.000 % |
|       100 |    3874    48376   283660 |    17737       81     10 | 23.167 % |
|       250 |    3777    48376   283660 |    19511      223     11 | 26.813 % |
|       475 |    3669    48376   283660 |    21462      440     11 | 31.771 % |
|       813 |    3645    48376   283660 |    23609      774     12 | 33.083 % |
|      1319 |    3590    48376   283660 |    25970     1276     13 | 36.271 % |
|      2078 |    3425    48376   283660 |    28567     2020     13 | 40.583 % |
|      3217 |    3197    48376   283660 |    31423     3137     12 | 48.293 % |
|      4925 |    2881    48376   283660 |    34566     4833     11 | 55.292 % |
===============================================================================
Verified 19408 original clauses.
restarts              : 9
conflicts             : 5536           (16475 /sec)
decisions             : 14000          (1.38 % random) (41664 /sec)
propagations          : 319597         (951122 /sec)
conflict literals     : 58384          (16.89 % deleted)
Memory used           : 19.63 MB
CPU time              : 0.336021 s
    \endverbatim
    </li>
    <li> r=6 with zeroed plaintext and zeroed key:
    \verbatim
SATISFIABLE
Sat 2.0 beta
WARNING: for repeatability, setting FPU to use double precision
============================[ Problem Statistics ]=============================
|                                                                             |
|  Number of variables:  6816                                                 |
|  Number of clauses:    90392                                                |
|  Parsing time:         0.07         s                                       |
============================[ Search Statistics ]==============================
| Conflicts |          ORIGINAL         |          LEARNT          | Progress |
|           |    Vars  Clauses Literals |    Limit  Clauses Lit/Cl |          |
===============================================================================
|         0 |    6512    79024   464032 |    26341        0    nan |  0.000 % |
|       102 |    6416    79024   464032 |    28975       83      7 | 14.347 % |
|       253 |    6303    79024   464032 |    31873      229      9 | 17.097 % |
|       479 |    6255    79024   464032 |    35060      450     10 | 18.528 % |
|       816 |    6237    79024   464032 |    38566      783     12 | 19.083 % |
|      1324 |    6230    79024   464032 |    42422     1289     12 | 20.028 % |
|      2083 |    6154    79024   464032 |    46665     2040     13 | 22.708 % |
|      3223 |    6122    79024   464032 |    51331     3166     13 | 25.083 % |
|      4933 |    6027    79024   464032 |    56464     4867     14 | 27.486 % |
|      7496 |    5022    58129   343718 |    62111     7236     15 | 32.097 % |
|     11341 |    4737    58129   343718 |    68322    11064     15 | 49.986 % |
|     17108 |    3404    36779   215587 |    75154    11851     14 | 54.722 % |
===============================================================================
Verified 32825 original clauses.
restarts              : 12
conflicts             : 21936          (12295 /sec)
decisions             : 63303          (1.63 % random) (35482 /sec)
propagations          : 1486412        (833139 /sec)
conflict literals     : 345161         (8.91 % deleted)
Memory used           : 22.93 MB
CPU time              : 1.78411 s
    \endverbatim
    </li>
    <li> r=8 with zeroed plaintext and zeroed key:
    \verbatim
SATISFIABLE
Sat 2.0 beta
WARNING: for repeatability, setting FPU to use double precision
============================[ Problem Statistics ]=============================
|                                                                             |
|  Number of variables:  9216                                                 |
|  Number of clauses:    121088                                               |
|  Parsing time:         0.08         s                                       |
============================[ Search Statistics ]==============================
| Conflicts |          ORIGINAL         |          LEARNT          | Progress |
|           |    Vars  Clauses Literals |    Limit  Clauses Lit/Cl |          |
===============================================================================
|         0 |    8896   109672   644404 |    36557        0    nan |  0.000 % |
|       100 |    8794   109672   644404 |    40213       86      7 | 11.083 % |
|       250 |    8738   109672   644404 |    44234      231      9 | 11.771 % |
|       475 |    8654   109672   644404 |    48657      447      8 | 13.625 % |
|       812 |    8557   109672   644404 |    53523      778     10 | 14.875 % |
|      1318 |    8494   109672   644404 |    58875     1278     11 | 15.813 % |
|      2078 |    8477   109672   644404 |    64763     2036     14 | 15.990 % |
|      3217 |    8474   109672   644404 |    71239     3173     17 | 16.167 % |
|      4925 |    8410   109672   644404 |    78363     4870     16 | 17.708 % |
|      7487 |    8381   109672   644404 |    86200     7429     16 | 18.688 % |
|     11331 |    7786    91873   541905 |    94820    11079     16 | 18.896 % |
|     17098 |    7764    91873   541905 |   104302    16840     16 | 20.833 % |
|     25748 |    7559    88761   522793 |   114732    25474     17 | 22.854 % |
|     38722 |    6879    80145   472955 |   126205    38399     17 | 28.385 % |
|     58183 |    6557    78144   463801 |   138826    57833     17 | 32.417 % |
|     87375 |    6131    74280   439928 |   152709    86987     20 | 41.875 % |
|    131164 |    4386    56563   330856 |   167979   129153     20 | 60.000 % |
===============================================================================
Verified 39498 original clauses.
restarts              : 17
conflicts             : 156090         (5662 /sec)
decisions             : 334631         (1.69 % random) (12139 /sec)
propagations          : 10838868       (393201 /sec)
conflict literals     : 3157303        (12.32 % deleted)
Memory used           : 43.59 MB
CPU time              : 27.5657 s
    \endverbatim
    </li>
    <li> r=10 with zeroed plaintext and zeroed key:
    \verbatim
SATISFIABLE
Sat 2.0 beta
WARNING: for repeatability, setting FPU to use double precision
============================[ Problem Statistics ]=============================
|                                                                             |
|  Number of variables:  11616                                                |
|  Number of clauses:    151784                                               |
|  Parsing time:         0.13         s                                       |
============================[ Search Statistics ]==============================
| Conflicts |          ORIGINAL         |          LEARNT          | Progress |
|           |    Vars  Clauses Literals |    Limit  Clauses Lit/Cl |          |
===============================================================================
|         0 |   11280   140320   824788 |    46773        0    nan |  0.000 % |
|       101 |   11246   140320   824788 |    51450       95      8 |  6.817 % |
|       251 |   11140   140320   824788 |    56595      240      8 |  8.958 % |
|       476 |   11091   140320   824788 |    62255      462     11 |  9.392 % |
|       813 |   11011   140320   824788 |    68480      792     10 | 10.650 % |
|      1319 |   10971   140320   824788 |    75328     1297     12 | 10.942 % |
|      2078 |   10920   140320   824788 |    82861     2055     13 | 11.375 % |
|      3217 |   10894   140320   824788 |    91147     3185     13 | 12.442 % |
|      4926 |   10886   140320   824788 |   100262     4892     13 | 12.842 % |
|      7489 |   10593   140320   824788 |   110289     7425     13 | 17.008 % |
|     11333 |   10433   140320   824788 |   121317    11265     14 | 18.633 % |
|     17099 |    9741   115905   682878 |   133449    13952     16 | 19.333 % |
|     25749 |    9707   115905   682878 |   146794    22597     17 | 19.633 % |
|     38724 |    9452   113855   671994 |   161474    35507     17 | 24.008 % |
|     58185 |    8833   106984   632225 |   177621    53913     17 | 31.158 % |
|     87377 |    7585    89756   527905 |   195383    80367     19 | 38.942 % |
|    131166 |    7222    83656   492837 |   214922   123068     24 | 39.817 % |
|    196851 |    6636    78620   463932 |   236414   186704     24 | 47.517 % |
===============================================================================
Verified 68524 original clauses.
restarts              : 18
conflicts             : 277369         (3138 /sec)
decisions             : 577890         (1.70 % random) (6537 /sec)
propagations          : 22423789       (253670 /sec)
conflict literals     : 6961921        (11.99 % deleted)
Memory used           : 68.51 MB
CPU time              : 88.3975 s
    \endverbatim
    </li>
   </ul>
   </li>
  </ul>


  \todo Experiments on AES with the first 32 key bits not set
  <ul>
   <li> Using minisat2 :
   <ul>
    <li> Run using the following command : 
    \verbatim
for r in `seq 2 2 10`; do
 for k in `seq 0 2 16`; do
     echo "K = " $k "R = " $r;
     ./mergeSameVarCNFs.sh "AES_r${r}.cnf" "AES_UC_r${r}_pn0_kn${k}_cn0_P32488853038D31734198AA2E0370D450_K3F6A2B7E151628AED2A6ABF7158809CF.cnf" > "AES_r${r}_kn_${k}_P32488853038D31734198AA2E0370D450_K3F6A2B7E151628AED2A6ABF7158809CF_SAT.cnf";
     time ./solvers/minisat/core/minisat "AES_r${r}_kn_${k}_P32488853038D31734198AA2E0370D450_K3F6A2B7E151628AED2A6ABF7158809CF_SAT.cnf" > "AES_r${r}_kn_${k}_P32488853038D31734198AA2E0370D450_K3F6A2B7E151628AED2A6ABF7158809CF_SAT.cnf.results.minisat" 2> "AES_r${r}_kn_${k}_P32488853038D31734198AA2E0370D450_K3F6A2B7E151628AED2A6ABF7158809CF_SAT.cnf.results.minisat" ;
 done
done
    \endverbatim
    </li>
    <li> Summary of results so far :
    \verbatim
cs360678@cselara:~/Experiments/AES> for x in AES_*.results.minisat; do echo $x; cat $x | grep "SAT\|CPU"; done
AES_r2_kn_0_P32488853038D31734198AA2E0370D450_K3F6A2B7E151628AED2A6ABF7158809CF_SAT.cnf.results.minisat
SATISFIABLE
CPU time              : 0.044002 s
AES_r2_kn_10_P32488853038D31734198AA2E0370D450_K3F6A2B7E151628AED2A6ABF7158809CF_SAT.cnf.results.minisat
SATISFIABLE
CPU time              : 0.040002 s
AES_r2_kn_12_P32488853038D31734198AA2E0370D450_K3F6A2B7E151628AED2A6ABF7158809CF_SAT.cnf.results.minisat
SATISFIABLE
CPU time              : 0.052003 s
AES_r2_kn_14_P32488853038D31734198AA2E0370D450_K3F6A2B7E151628AED2A6ABF7158809CF_SAT.cnf.results.minisat
SATISFIABLE
CPU time              : 0.040002 s
AES_r2_kn_16_P32488853038D31734198AA2E0370D450_K3F6A2B7E151628AED2A6ABF7158809CF_SAT.cnf.results.minisat
SATISFIABLE
CPU time              : 0.048003 s
AES_r2_kn_2_P32488853038D31734198AA2E0370D450_K3F6A2B7E151628AED2A6ABF7158809CF_SAT.cnf.results.minisat
SATISFIABLE
CPU time              : 0.036002 s
AES_r2_kn_4_P32488853038D31734198AA2E0370D450_K3F6A2B7E151628AED2A6ABF7158809CF_SAT.cnf.results.minisat
SATISFIABLE
CPU time              : 0.040002 s
AES_r2_kn_6_P32488853038D31734198AA2E0370D450_K3F6A2B7E151628AED2A6ABF7158809CF_SAT.cnf.results.minisat
SATISFIABLE
CPU time              : 0.068004 s
AES_r2_kn_8_P32488853038D31734198AA2E0370D450_K3F6A2B7E151628AED2A6ABF7158809CF_SAT.cnf.results.minisat
SATISFIABLE
CPU time              : 0.044002 s
AES_r4_kn_0_P32488853038D31734198AA2E0370D450_K3F6A2B7E151628AED2A6ABF7158809CF_SAT.cnf.results.minisat
SATISFIABLE
CPU time              : 0.152009 s
AES_r4_kn_2_P32488853038D31734198AA2E0370D450_K3F6A2B7E151628AED2A6ABF7158809CF_SAT.cnf.results.minisat
SATISFIABLE
CPU time              : 0.588036 s
AES_r4_kn_4_P32488853038D31734198AA2E0370D450_K3F6A2B7E151628AED2A6ABF7158809CF_SAT.cnf.results.minisat
SATISFIABLE
CPU time              : 0.624039 s
AES_r4_kn_6_P32488853038D31734198AA2E0370D450_K3F6A2B7E151628AED2A6ABF7158809CF_SAT.cnf.results.minisat
SATISFIABLE
CPU time              : 2.90818 s
AES_r4_kn_8_P32488853038D31734198AA2E0370D450_K3F6A2B7E151628AED2A6ABF7158809CF_SAT.cnf.results.minisat
    \endverbatim
    </li>
   </ul>
   </li>
   <li> Using OKsolver : 
   <ul>
    <li> Run using :
    \verbatim
  for r in `seq 2 2 10`; do
   for k in `seq 0 2 16`; do
       echo "K = " $k "R = " $r;
       ./mergeSameVarCNFs.sh "AES_r${r}.cnf" "AES_UC_r${r}_pn0_kn${k}_cn0_P32488853038D31734198AA2E0370D450_K3F6A2B7E151628AED2A6ABF7158809CF.cnf" > "AES_r${r}_kn_${k}_P32488853038D31734198AA2E0370D450_K3F6A2B7E151628AED2A6ABF7158809CF_SAT.cnf" ;
       time /tmp/OKlib/OKlibrary-0.2.1.2_00102/OKplatform/bin/OKsolver_2002-O3-DNDEBUG "AES_r${r}_kn_${k}_P32488853038D31734198AA2E0370D450_K3F6A2B7E151628AED2A6ABF7158809CF_SAT.cnf" > "AES_r${r}_kn_${k}_P32488853038D31734198AA2E0370D450_K3F6A2B7E151628AED2A6ABF7158809CF_SAT.cnf.results.oksolver" 2> "AES_r${r}_kn_${k}_P32488853038D31734198AA2E0370D450_K3F6A2B7E151628AED2A6ABF7158809CF_SAT.cnf.results.oksolver" ;
     done
   done
    \endverbatim
    </li>
    <li> Current results : 
    \verbatim
cs360678@csnereid:~/Experiments/AES> cat AES_r2*.results.oksolver
s SATISFIABLE
c sat_status=1 initial_maximal_clause_length=9 initial_number_of_variables=2272
initial_number_of_clauses=29000 initial_number_of_literal_occurrences=173136
running_time(s)=0.1 number_of_nodes=3 number_of_single_nodes=0
number_of_quasi_single_nodes=0 number_of_2-reductions=81
number_of_pure_literals=0 number_of_autarkies=0 number_of_missed_single_nodes=0
max_tree_depth=1 number_of_table_enlargements=0 reduced_maximal_clause_length=0
reduced_number_of_variables=696 reduced_number_of_clauses=12033
reduced_number_of_literal_occurrences=78281 number_of_1-autarkies=0
number_of_initial_unit-eliminations=696 number_of_new_2-clauses=0
maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=1310
file_name=AES_r2_kn_0_P32488853038D31734198AA2E0370D450_K3F6A2B7E151628AED2A6ABF7158809CF_SAT.cnf
s SATISFIABLE
c sat_status=1 initial_maximal_clause_length=9 initial_number_of_variables=2272
initial_number_of_clauses=28990 initial_number_of_literal_occurrences=173126
running_time(s)=0.1 number_of_nodes=3 number_of_single_nodes=0
number_of_quasi_single_nodes=0 number_of_2-reductions=81
number_of_pure_literals=0 number_of_autarkies=0 number_of_missed_single_nodes=0
max_tree_depth=1 number_of_table_enlargements=0 reduced_maximal_clause_length=0
reduced_number_of_variables=666 reduced_number_of_clauses=10905
reduced_number_of_literal_occurrences=70583 number_of_1-autarkies=0
number_of_initial_unit-eliminations=666 number_of_new_2-clauses=0
maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=1319
file_name=AES_r2_kn_10_P32488853038D31734198AA2E0370D450_K3F6A2B7E151628AED2A6ABF7158809CF_SAT.cnf
s SATISFIABLE
c sat_status=1 initial_maximal_clause_length=9 initial_number_of_variables=2272
initial_number_of_clauses=28988 initial_number_of_literal_occurrences=173124
running_time(s)=0.1 number_of_nodes=3 number_of_single_nodes=0
number_of_quasi_single_nodes=0 number_of_2-reductions=81
number_of_pure_literals=0 number_of_autarkies=0 number_of_missed_single_nodes=0
max_tree_depth=1 number_of_table_enlargements=0 reduced_maximal_clause_length=0
reduced_number_of_variables=660 reduced_number_of_clauses=10833
reduced_number_of_literal_occurrences=70149 number_of_1-autarkies=0
number_of_initial_unit-eliminations=660 number_of_new_2-clauses=0
maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=1321
file_name=AES_r2_kn_12_P32488853038D31734198AA2E0370D450_K3F6A2B7E151628AED2A6ABF7158809CF_SAT.cnf
s SATISFIABLE
c sat_status=1 initial_maximal_clause_length=9 initial_number_of_variables=2272
initial_number_of_clauses=28986 initial_number_of_literal_occurrences=173122
running_time(s)=0.1 number_of_nodes=3 number_of_single_nodes=0
number_of_quasi_single_nodes=0 number_of_2-reductions=82
number_of_pure_literals=0 number_of_autarkies=0 number_of_missed_single_nodes=0
max_tree_depth=1 number_of_table_enlargements=0 reduced_maximal_clause_length=0
reduced_number_of_variables=654 reduced_number_of_clauses=10682
reduced_number_of_literal_occurrences=69159 number_of_1-autarkies=0
number_of_initial_unit-eliminations=654 number_of_new_2-clauses=0
maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=1325
file_name=AES_r2_kn_14_P32488853038D31734198AA2E0370D450_K3F6A2B7E151628AED2A6ABF7158809CF_SAT.cnf
s SATISFIABLE
c sat_status=1 initial_maximal_clause_length=9 initial_number_of_variables=2272
initial_number_of_clauses=28984 initial_number_of_literal_occurrences=173120
running_time(s)=0.1 number_of_nodes=3 number_of_single_nodes=0
number_of_quasi_single_nodes=0 number_of_2-reductions=84
number_of_pure_literals=0 number_of_autarkies=0 number_of_missed_single_nodes=0
max_tree_depth=1 number_of_table_enlargements=0 reduced_maximal_clause_length=0
reduced_number_of_variables=648 reduced_number_of_clauses=10418
reduced_number_of_literal_occurrences=67188 number_of_1-autarkies=0
number_of_initial_unit-eliminations=648 number_of_new_2-clauses=0
maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=1329
file_name=AES_r2_kn_16_P32488853038D31734198AA2E0370D450_K3F6A2B7E151628AED2A6ABF7158809CF_SAT.cnf
s SATISFIABLE
c sat_status=1 initial_maximal_clause_length=9 initial_number_of_variables=2272
initial_number_of_clauses=28998 initial_number_of_literal_occurrences=173134
running_time(s)=0.1 number_of_nodes=3 number_of_single_nodes=0
number_of_quasi_single_nodes=0 number_of_2-reductions=79
number_of_pure_literals=0 number_of_autarkies=0 number_of_missed_single_nodes=0
max_tree_depth=1 number_of_table_enlargements=0 reduced_maximal_clause_length=0
reduced_number_of_variables=690 reduced_number_of_clauses=11918
reduced_number_of_literal_occurrences=77655 number_of_1-autarkies=0
number_of_initial_unit-eliminations=690 number_of_new_2-clauses=0
maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=1312
file_name=AES_r2_kn_2_P32488853038D31734198AA2E0370D450_K3F6A2B7E151628AED2A6ABF7158809CF_SAT.cnf
s SATISFIABLE
c sat_status=1 initial_maximal_clause_length=9 initial_number_of_variables=2272
initial_number_of_clauses=28996 initial_number_of_literal_occurrences=173132
running_time(s)=0.1 number_of_nodes=3 number_of_single_nodes=0
number_of_quasi_single_nodes=0 number_of_2-reductions=79
number_of_pure_literals=0 number_of_autarkies=0 number_of_missed_single_nodes=0
max_tree_depth=1 number_of_table_enlargements=0 reduced_maximal_clause_length=0
reduced_number_of_variables=684 reduced_number_of_clauses=11752
reduced_number_of_literal_occurrences=76662 number_of_1-autarkies=0
number_of_initial_unit-eliminations=684 number_of_new_2-clauses=0
maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=1313
file_name=AES_r2_kn_4_P32488853038D31734198AA2E0370D450_K3F6A2B7E151628AED2A6ABF7158809CF_SAT.cnf
s SATISFIABLE
c sat_status=1 initial_maximal_clause_length=9 initial_number_of_variables=2272
initial_number_of_clauses=28994 initial_number_of_literal_occurrences=173130
running_time(s)=0.1 number_of_nodes=3 number_of_single_nodes=0
number_of_quasi_single_nodes=0 number_of_2-reductions=81
number_of_pure_literals=0 number_of_autarkies=0 number_of_missed_single_nodes=0
max_tree_depth=1 number_of_table_enlargements=0 reduced_maximal_clause_length=0
reduced_number_of_variables=678 reduced_number_of_clauses=11467
reduced_number_of_literal_occurrences=74717 number_of_1-autarkies=0
number_of_initial_unit-eliminations=678 number_of_new_2-clauses=0
maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=1317
file_name=AES_r2_kn_6_P32488853038D31734198AA2E0370D450_K3F6A2B7E151628AED2A6ABF7158809CF_SAT.cnf
s SATISFIABLE
c sat_status=1 initial_maximal_clause_length=9 initial_number_of_variables=2272
initial_number_of_clauses=28992 initial_number_of_literal_occurrences=173128
running_time(s)=0.1 number_of_nodes=3 number_of_single_nodes=0
number_of_quasi_single_nodes=0 number_of_2-reductions=83
number_of_pure_literals=0 number_of_autarkies=0 number_of_missed_single_nodes=0
max_tree_depth=1 number_of_table_enlargements=0 reduced_maximal_clause_length=0
reduced_number_of_variables=672 reduced_number_of_clauses=10968
reduced_number_of_literal_occurrences=70911 number_of_1-autarkies=0
number_of_initial_unit-eliminations=672 number_of_new_2-clauses=0
maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=1321
file_name=AES_r2_kn_8_P32488853038D31734198AA2E0370D450_K3F6A2B7E151628AED2A6ABF7158809CF_SAT.cnf
    \endverbatim
    </li>
   </ul>
   </li>
  </ul>

*/
