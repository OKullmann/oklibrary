// Matthew Gwynne, 25.8.2009 (Swansea)
/* Copyright 2009, 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/RamseyProblems/plans/Ramsey_2_4_4.hpp
  \brief On investigations into %Ramsey problems of the form ramsey_2^2(4,4)


  \todo General information
  <ul>
   <li> Known is ramsey_2^2(4) = 18. </li>
   <li> So we get at most 6120 clauses. </li>
   <li> To generate an instance of SAT problem specifying "ramsey_2^2(4) > n?"
   use the following:
   \verbatim
Ramsey-O3-DNDEBUG 4 4 2 6 | ExtendedToStrictDimacs-O3-DNDEBUG > Ramsey_4_4_2_6.cnf
   \endverbatim
   where n has been replaced by 6 in this example.
   </li>
   <li> To generate the additional clauses for each of the symmetry 
   breaking techniques (see 
   ComputerAlgebra/Satisfiability/Lisp/Generators/RamseyTheory/plans/RamseyProblems.hpp)
   and cardinality constraints, run the following in maxima (generating
   each of the additional clauses for a range of n values):
   <ul>
    <li> Symmetry breaking by fixing colours/labels to break monochromatic
    k-subsets:
    \verbatim
for n : 5 thru 18 do output_ramsey2_symbr1_stdname(4,n);
    \endverbatim
    </li>
    <li> Symmetry breaking by fixing monochromatic cliques:
    \verbatim
for n : 5 thru 18 do output_ramsey2_symbr2_stdname(n);
    \endverbatim
    </li>
    <li> Symmetry breaking by recursive application of the pigeon hole
    principle:
    \verbatim
for n : 5 thru 18 do output_ramsey2_symbr3_stdname(n);
    \endverbatim
    </li>
    <li> Cardinality constraints:
    \verbatim
:lisp (ext:set-limit 'ext:heap-size 3000000000)
:lisp (ext:set-limit 'ext:frame-stack 10000)
:lisp (ext:set-limit 'ext:c-stack 200000)
:lisp (ext:set-limit 'ext:lisp-stack 200000)
for n : 5 thru 18 do block([n_e : binomial(n,2)],
  output_fcs(
    sconcat("Card ",n),
    fcl2fcs(standardise_fcl(cl2fcl(cardinality_cl([floor(n_e/2),create_list(i,i,1,n_e),ceiling(n_e/2)])))[1]),
    sconcat("Card_n",n,".cnf")))$
    \endverbatim
    </li>
   </ul>
   </li>
  </ul>


  \todo Using DPLL and conflict-driven solvers
  <ul>
   <li> Satisfiability for n=17:
    <ol>
     <li> Trivial for OKsolver. </li>
     <li> Grasp aborts by default (10000s exceeded). </li>
     <li> Trivial for minisat. </li>
     <li> Trivial for march_pl. </li>
    </ol>
   </li>
   <li> Unsatisfiability for n=18:
    <ol>
     <li> OKsolver
      <ul>
       <li> Running it with
       \verbatim
> OKsolver_2002-O3-DNDEBUG -M -D14 -F Ramsey_4_2_18.cnf
       \endverbatim
       seems to indicate that it will take roughly 15 days (on csltok). </li>
       <li> However, this seems to be too optimistic: The first 15 nodes
       at depth 14 (16384 nodes altogether) were solved all with around
       100,000 nodes in the subtrees, however with node 16 the solver
       falls into a deep hole, and yet 500,000,000 nodes have been processed
       there. </li>
       <li> A still deeper hole waited at node 19, and after 16.35 days and
       4,095,825,666 nodes (altogether; 3620490 single nodes, 16,220,739,545
       2-reductions, 22,409,527 pure literals, 0 autarkies; 1,412,661.8
       seconds) it was aborted. </li>
       <li> Interestingly it seems that cs-wsok here is quite a bit slower
       than csltok:
        <ol>
         <li>
         \verbatim
  1:110200,  208.1, 3408483.1
  2:100554,  418.0, 3423838.0
         \endverbatim
         but there is a lot of variability
         \verbatim
  1:110200,  161.6, 2647165.1
  2:100554,  302.3, 2476385.0
         \endverbatim
         (which is strange?) versus on csltok:
         \verbatim
  1:110200,   82.5, 1350942.2
  2:100554,  162.8, 1333822.4
         \endverbatim
         where actually csltok has a slower cpu?? </li>
         <li> After 3299m 19s, on csltok 487,747,031 nodes
         were processed, that makes 2460 nodes per second. </li>
         <li> After 254392.7 s, on cs-wsok 740,136,935 nodes
         where processed, that makes 2900 nodes per second. </li>
         <li> So it seems that cs-wsok was only initially that slow
         --- perhaps cache behaviour? But still, given that csltok
         runs with 1400 MHz and cs-wsok with 2100 MHz, executation
         on cs-wsok seems slow. </li>
        </ol>
       </li>
      </ul>
     </li>
     <li> Grasp aborts by default (10000s exceeded). </li>
     <li> minisat seems hopeless:
     \verbatim
> minisat Ramsey_4_2_18.cnf
==================================[MINISAT]===================================
| Conflicts |     ORIGINAL     |              LEARNT              | Progress |
|           | Clauses Literals |   Limit Clauses Literals  Lit/Cl |          |
==============================================================================
|         0 |    6120    36720 |    2040       0        0     nan |  0.000 % |
|       101 |    6120    36720 |    2244     101     1575    15.6 |  0.004 % |
|       251 |    6120    36720 |    2468     251     5553    22.1 |  0.004 % |
|       477 |    6120    36720 |    2715     477    12466    26.1 |  0.004 % |
|       814 |    6120    36720 |    2986     814    21935    26.9 |  0.004 % |
|      1321 |    6120    36720 |    3285    1321    36858    27.9 |  0.004 % |
|      2081 |    6120    36720 |    3613    2081    59272    28.5 |  0.004 % |
|      3222 |    6120    36720 |    3975    3222    96072    29.8 |  0.004 % |
|      4930 |    6120    36720 |    4372    2911    89091    30.6 |  0.004 % |
|      7492 |    6120    36720 |    4810    3256    99003    30.4 |  0.004 % |
|     11336 |    6120    36720 |    5291    4661   155593    33.4 |  0.004 % |
|     17104 |    6120    36720 |    5820    5079   165609    32.6 |  0.004 % |
|     25753 |    6120    36720 |    6402    4923   157105    31.9 |  0.004 % |
|     38727 |    6120    36720 |    7042    4980   164881    33.1 |  0.004 % |
|     58188 |    6120    36720 |    7746    6702   218017    32.5 |  0.004 % |
|     87382 |    6120    36720 |    8521    4635   144895    31.3 |  0.004 % |
|    131171 |    6120    36720 |    9373    5516   180171    32.7 |  0.004 % |
|    196857 |    6120    36720 |   10311    5186   157778    30.4 |  0.004 % |
|    295383 |    6120    36720 |   11342    5205   153268    29.4 |  0.004 % |
|    443172 |    6120    36720 |   12476   10504   355910    33.9 |  0.004 % |
|    664855 |    6120    36720 |   13724    6638   206592    31.1 |  0.004 % |
|    997380 |    6120    36720 |   15096    8503   275191    32.4 |  0.004 % |
|   1496168 |    6120    36720 |   16606   14871   493709    33.2 |  0.004 % |
|   2244351 |    6120    36720 |   18266   13391   473701    35.4 |  0.004 % |
|   3366625 |    6120    36720 |   20093   18188   616966    33.9 |  0.004 % |
|   5050036 |    6120    36720 |   22102   19370   676586    34.9 |  0.004 % |
|   7575152 |    6120    36720 |   24313   18788   607228    32.3 |  0.004 % |
|  11362827 |    6120    36720 |   26744   15544   522901    33.6 |  0.004 % |
|  17044340 |    6120    36720 |   29418   14652   462997    31.6 |  0.004 % |
|  25566609 |    6120    36720 |   32360   19947   657932    33.0 |  0.004 % |
|  38350012 |    6120    36720 |   35596   29494  1047723    35.5 |  0.004 % |
|  57525118 |    6120    36720 |   39156   34447  1165683    33.8 |  0.004 % |
|  86287776 |    6120    36720 |   43072   35595  1309147    36.8 |  0.004 % |
| 129431764 |    6120    36720 |   47379   37529  1285937    34.3 |  0.004 % |
| 194147746 |    6120    36720 |   52117   39588  1369541    34.6 |  0.004 % |
| 291221720 |    6120    36720 |   57328   31103  1057264    34.0 |  0.004 % |
| 436832680 |    6120    36720 |   63061   46147  1656671    35.9 |  0.004 % |
| 655249120 |    6120    36720 |   69368   23145   770418    33.3 |  0.004 % |

*** INTERRUPTED ***
restarts              : 38
conflicts             : 669017040
decisions             : 771042110
propagations          : 11023313626
conflict literals     : 23997088233    (23.37 % deleted)
Memory used           : 43.93 MB
     \endverbatim
     </li>
     <li> march_pl seems hopeless (after one hour).
      <ol>
       <li> Again, since the problem is hard, we should run march_pl for, say,
       2 weeks. </li>
      </ol>
     </li>
    </ol>
   </li>
   <li> Using the symmetry breaking by recursive application of the pigeon hole
   principle (see "ramsey2_symbr3_cs" in 
   ComputerAlgebra/Satisfiability/Lisp/Generators/RamseyTheory/RamseyProblems.mac )
   it seems several solvers can prove n=18 unsatisfiable:
   <ul>
    <li> To generate, first in maxima:
    \verbatim
output_ramsey2_symbr3_stdname(18);
    \endverbatim
    and then in the shell:
    \verbatim
Ramsey-O3-DNDEBUG 4 4 2 18 > Ramsey_4_4_2_18.ecnf
./merge_cnf.sh Ramsey_4_4_2_18.ecnf Ramsey_SB_PHP_18.ecnf > Ramsey_4_4_2_18_SB_PHP.ecnf
ExtendedToStrictDimacs-O3-DNDEBUG < Ramsey_4_4_2_18_SB_PHP.ecnf > Ramsey_4_4_2_18_SB_PHP.cnf
    \endverbatim
    where "merge_cnf.sh" is defined as follows:
    \verbatim
#!/bin/bash

# Grab the number of clauses from each
CL1=`grep "^p" $1  | cut -d " " -f "4"`
CL2=`grep "^p" $2  | cut -d " " -f "4"`

NewCL=`expr $CL1 + $CL2`
cat $1 | sed -e "s/p \+\([a-zA-Z]\+\) \+\([0-9]\+\).*$/p \1 \2 $NewCL/"
cat $2 | grep -v "^c" | grep -v "^p"
    \endverbatim
    </li>
    <li> minisat2:
    \verbatim
$ minisat2 Ramsey_4_4_2_18_SB_PHP.cnf 
This is MiniSat 2.0 beta
WARNING: for repeatability, setting FPU to use double precision
============================[ Problem Statistics ]=============================
|                                                                             |
|  Number of variables:  153                                                  |
|  Number of clauses:    6222                                                 |
|  Parsing time:         0.01         s                                       |
============================[ Search Statistics ]==============================
| Conflicts |          ORIGINAL         |          LEARNT          | Progress |
|           |    Vars  Clauses Literals |    Limit  Clauses Lit/Cl |          |
===============================================================================
|         0 |     153     6222    35578 |     2074        0    nan |  0.000 % |
|       101 |     153     6222    35578 |     2281      101     12 |  0.038 % |
|       251 |     153     6222    35578 |     2509      251     18 |  0.004 % |
|       477 |     153     6222    35578 |     2760      477     21 |  0.004 % |
|       814 |     153     6222    35578 |     3036      814     21 |  0.004 % |
|      1320 |     153     6222    35578 |     3340     1320     21 |  0.004 % |
|      2080 |     153     6222    35578 |     3674     2080     21 |  0.004 % |
|      3222 |     153     6222    35578 |     4041     3222     19 |  0.004 % |
===============================================================================
restarts              : 8
conflicts             : 4647           (23234 /sec)
decisions             : 7230           (1.72 % random) (36148 /sec)
propagations          : 62907          (314516 /sec)
conflict literals     : 81530          (25.95 % deleted)
Memory used           : 2.63 MB
CPU time              : 0.200012 s

UNSATISFIABLE
    \endverbatim
    </li>
    <li> OKsolver:
    \verbatim
$ OKsolver_2002-O3-DNDEBUG Ramsey_4_4_2_18_SB_PHP.cnf 
s UNSATISFIABLE
c sat_status=0 initial_maximal_clause_length=6 initial_number_of_variables=153 initial_number_of_clauses=6222 initial_number_of_literal_occurrences=36924 running_time(s)=0.2 number_of_nodes=163 number_of_single_nodes=4 number_of_quasi_single_nodes=0 number_of_2-reductions=286 number_of_pure_literals=0 number_of_autarkies=0 number_of_missed_single_nodes=0 max_tree_depth=11 number_of_table_enlargements=0 reduced_maximal_clause_length=0 reduced_number_of_variables=0 reduced_number_of_clauses=0 reduced_number_of_literal_occurrences=0 number_of_1-autarkies=0 number_of_initial_unit-eliminations=0 number_of_new_2-clauses=0 maximal_number_of_added_2-clauses=0 initial_number_of_2-clauses=102 file_name=Ramsey_4_4_2_18_SB_PHP.cnf
    \endverbatim
    </li>
    <li> As a partial check the same symmetry breaking for n=17 yields 
    satisfiable:
    \verbatim
$ minisat2 Ramsey_4_4_2_17_SB_PHP.cnf 
This is MiniSat 2.0 beta
WARNING: for repeatability, setting FPU to use double precision
============================[ Problem Statistics ]=============================
|                                                                             |
|  Number of variables:  136                                                  |
|  Number of clauses:    4844                                                 |
|  Parsing time:         0.01         s                                       |
============================[ Search Statistics ]==============================
| Conflicts |          ORIGINAL         |          LEARNT          | Progress |
|           |    Vars  Clauses Literals |    Limit  Clauses Lit/Cl |          |
===============================================================================
|         0 |     136     4844    27680 |     1614        0    nan |  0.000 % |
|       102 |     136     4844    27680 |     1776      102     12 |  0.043 % |
|       253 |     136     4844    27680 |     1953      253     15 |  0.005 % |
|       479 |     136     4844    27680 |     2149      479     17 |  0.005 % |
|       816 |     136     4844    27680 |     2364      816     15 |  0.005 % |
|      1323 |     136     4844    27680 |     2600     1323     17 |  0.011 % |
|      2085 |     136     4844    27680 |     2860     2085     18 |  0.005 % |
|      3225 |     136     4844    27680 |     3146     1768     20 |  0.005 % |
|      4934 |     136     4844    27680 |     3461     1881     17 |  0.005 % |
|      7497 |     136     4844    27680 |     3807     2694     22 |  0.065 % |
===============================================================================
restarts              : 10
conflicts             : 10812          (39172 /sec)
decisions             : 14637          (1.61 % random) (53029 /sec)
propagations          : 141515         (512704 /sec)
conflict literals     : 220783         (26.80 % deleted)
Memory used           : 2.62 MB
CPU time              : 0.276017 s

SATISFIABLE
    \endverbatim
    </li>
    <li> MG should finish providing a full proof that this symmetry breaking
    method is fully correct. </li>
    <li> Also for such a small instance, the symmetry breaking clauses can be 
    checked manually to ensure they are as expected, and there is not a bug
    in the implementation. </li>
   </ul>
   </li>
   <li> A more systematic investigation, involving each of the symmetry
   breaking techniques and cardinality constraints (see 
   Experimentation/Investigations/RamseyTheory/RamseyProblems/plans/SymmetryBreaking.hpp)
   can be generated and run using the experiment system discussed at "General 
   experiment system" in 
   Experimentation/Investigations/Cryptography/AES/plans/BreakingAES.hpp), with
   the following "generate_exp.sh" script:
   \verbatim
# Generates Ramsey experiments for "ramsey_2^2(4,4) > n?" for 5 <= n <= 49.


for n in `seq 5 49`; do
# No Symmetry breaking
    echo "PICOSAT_"$n "./Ramsey-O3-DNDEBUG 4 4 2 ${n} | ./ExtendedToStrictDimacs-O3-DNDEBUG > Ramsey_4_4_2_${n}.cnf && ./solvers/picosat913 -v Ramsey_exp.cnf > Ramsey_4_4_2_${n}.cnf.result.picosat 2>&1 && rm Ramsey_4_4_2_${n}.cnf" >> experiments;
    echo "MINISAT2_"$n "./Ramsey-O3-DNDEBUG 4 4 2 ${n} | ./ExtendedToStrictDimacs-O3-DNDEBUG > Ramsey_4_4_2_${n}.cnf && ./solvers/minisat2 Ramsey_4_4_2_${n}.cnf minisat-temp > Ramsey_4_4_2_${n}.cnf.result.minisat2 2>&1; cat minisat-temp >> Ramsey_4_4_2_${n}.cnf.result.minisat2;rm Ramsey_4_4_2_${n}.cnf" >> experiments;
    echo "MARCH_PL_"$n "./Ramsey-O3-DNDEBUG 4 4 2 ${n} | ./ExtendedToStrictDimacs-O3-DNDEBUG > Ramsey_4_4_2_${n}.cnf && ./solvers/march_pl Ramsey_4_4_2_${n}.cnf > Ramsey_4_4_2_${n}.cnf.result.march_pl 2>&1;rm Ramsey_4_4_2_${n}.cnf" >> experiments;
    echo "OKSOLVER_"$n "./Ramsey-O3-DNDEBUG 4 4 2 ${n} | ./ExtendedToStrictDimacs-O3-DNDEBUG > Ramsey_4_4_2_${n}.cnf && ./solvers/OKsolver_2002-O3-DNDEBUG -O -D20 -M -F Ramsey_4_4_2_${n}.cnf > Ramsey_4_4_2_${n}.cnf.result.OKsolver 2>&1; mv Ramsey_4_4_2_${n}.cnf.mo Ramsey_4_4_2_${n}.cnf.mo.OKsolver;mv Ramsey_4_4_2_${n}.cnf.pa Ramsey_4_4_2_${n}.cnf.pa.OKsolver;rm Ramsey_4_4_2_${n}.cnf" >> experiments;
    echo "OKSOLVERM2PP_"$n "./Ramsey-O3-DNDEBUG 4 4 2 ${n} | ./ExtendedToStrictDimacs-O3-DNDEBUG > Ramsey_4_4_2_${n}.cnf && ./solvers/OKsolver_2002-m2pp -O -D20 -M -F Ramsey_4_4_2_${n}.cnf > Ramsey_4_4_2_${n}.cnf.result.OKsolver-m2pp 2>&1; mv Ramsey_4_4_2_${n}.cnf_m2pp_*.mo Ramsey_4_4_2_${n}.cnf.mo.OKsolver-m2pp;mv Ramsey_4_4_2_${n}.cnf_m2pp_*.pa Ramsey_4_4_2_${n}.cnf.pa.OKsolver-m2pp;rm Ramsey_4_4_2_${n}.cnf" >> experiments;
# Symmetry breaking 1
   echo "PICOSAT_FC"$n "./Ramsey-O3-DNDEBUG 4 4 2 ${n} > Ramsey_4_4_2_${n}.ecnf && ./merge_cnf.sh Ramsey_4_4_2_${n}.ecnf Ramsey_SB_FC_4_${n}.ecnf > Ramsey_4_4_2_${n}_SB_FC.ecnf && ./ExtendedToStrictDimacs-O3-DNDEBUG < Ramsey_4_4_2_${n}_SB_FC.ecnf > Ramsey_4_4_2_${n}_SB_FC.cnf && ./solvers/picosat913 -v Ramsey_exp.cnf > Ramsey_4_4_2_${n}_SB_FC.cnf.result.picosat 2>&1 && rm -f Ramsey_4_4_2_${n}{,_SB_FC}.{e,}cnf" >> experiments;
    echo "MINISAT2_FC"$n "./Ramsey-O3-DNDEBUG 4 4 2 ${n} > Ramsey_4_4_2_${n}.ecnf && ./merge_cnf.sh Ramsey_4_4_2_${n}.ecnf Ramsey_SB_FC_4_${n}.ecnf > Ramsey_4_4_2_${n}_SB_FC.ecnf && ./ExtendedToStrictDimacs-O3-DNDEBUG < Ramsey_4_4_2_${n}_SB_FC.ecnf > Ramsey_4_4_2_${n}_SB_FC.cnf && ./solvers/minisat2 Ramsey_4_4_2_${n}_SB_FC.cnf minisat-temp > Ramsey_4_4_2_${n}_SB_FC.cnf.result.minisat2 2>&1; cat minisat-temp >> Ramsey_4_4_2_${n}_SB_FC.cnf.result.minisat2;rm -f Ramsey_4_4_2_${n}{,_SB_FC}.{e,}cnf" >> experiments;
  echo "MARCH_PL_FC"$n "./Ramsey-O3-DNDEBUG 4 4 2 ${n} > Ramsey_4_4_2_${n}.ecnf && ./merge_cnf.sh Ramsey_4_4_2_${n}.ecnf Ramsey_SB_FC_4_${n}.ecnf > Ramsey_4_4_2_${n}_SB_FC.ecnf && ./ExtendedToStrictDimacs-O3-DNDEBUG < Ramsey_4_4_2_${n}_SB_FC.ecnf > Ramsey_4_4_2_${n}_SB_FC.cnf && ./solvers/march_pl Ramsey_4_4_2_${n}_SB_FC.cnf > Ramsey_4_4_2_${n}_SB_FC.cnf.result.march_pl 2>&1;rm -f Ramsey_4_4_2_${n}{_SB_FC,}.{e,}cnf" >> experiments;
   echo "OKSOLVER_FC"$n "./Ramsey-O3-DNDEBUG 4 4 2 ${n} > Ramsey_4_4_2_${n}.ecnf && ./merge_cnf.sh Ramsey_4_4_2_${n}.ecnf Ramsey_SB_FC_4_${n}.ecnf > Ramsey_4_4_2_${n}_SB_FC.ecnf && ./ExtendedToStrictDimacs-O3-DNDEBUG < Ramsey_4_4_2_${n}_SB_FC.ecnf > Ramsey_4_4_2_${n}_SB_FC.cnf && ./solvers/OKsolver_2002-O3-DNDEBUG -O -D20 -M -F Ramsey_4_4_2_${n}_SB_FC.cnf > Ramsey_4_4_2_${n}_SB_FC.cnf.result.OKsolver 2>&1; mv Ramsey_4_4_2_${n}_SB_FC.cnf.mo Ramsey_4_4_2_${n}_SB_FC.cnf.mo.OKsolver;mv Ramsey_4_4_2_${n}_SB_FC.cnf.pa Ramsey_4_4_2_${n}_SB_FC.cnf.pa.OKsolver;rm -f Ramsey_4_4_2_${n}{,_SB_FC}.{e,}cnf" >> experiments;
    echo "OKSOLVERM2PP_FC"$n "./Ramsey-O3-DNDEBUG 4 4 2 ${n} > Ramsey_4_4_2_${n}.ecnf && ./merge_cnf.sh Ramsey_4_4_2_${n}.ecnf Ramsey_SB_FC_4_${n}.ecnf > Ramsey_4_4_2_${n}_SB_FC.ecnf && ./ExtendedToStrictDimacs-O3-DNDEBUG < Ramsey_4_4_2_${n}_SB_FC.ecnf > Ramsey_4_4_2_${n}_SB_FC.cnf && ./solvers/OKsolver_2002-m2pp -O -D20 -M -F Ramsey_4_4_2_${n}_SB_FC.cnf > Ramsey_4_4_2_${n}_SB_FC.cnf.result.OKsolver-m2pp 2>&1; mv Ramsey_4_4_2_${n}_SB_FC.cnf_m2pp_*.mo Ramsey_4_4_2_${n}_SB_FC.cnf.mo.OKsolver-m2pp;mv Ramsey_4_4_2_${n}_SB_FC.cnf_m2pp_*.pa Ramsey_4_4_2_${n}_SB_FC.cnf.pa.OKsolver-m2pp;rm -f Ramsey_4_4_2_${n}{,_SB_FC}.{e,}cnf" >> experiments;
# Symmetry breaking 2
   echo "PICOSAT_MC"$n "./Ramsey-O3-DNDEBUG 4 4 2 ${n} > Ramsey_4_4_2_${n}.ecnf && ./merge_cnf.sh Ramsey_4_4_2_${n}.ecnf Ramsey_SB_MC_${n}.ecnf > Ramsey_4_4_2_${n}_SB_MC.ecnf && ./ExtendedToStrictDimacs-O3-DNDEBUG < Ramsey_4_4_2_${n}_SB_MC.ecnf > Ramsey_4_4_2_${n}_SB_MC.cnf && ./solvers/picosat913 -v Ramsey_4_4_2_${n}_SB_MC.cnf > Ramsey_4_4_2_${n}_SB_MC.cnf.result.picosat 2>&1 && rm -f Ramsey_4_4_2_${n}{,_SB_MC}.{e,}cnf" >> experiments;
    echo "MINISAT2_MC"$n "./Ramsey-O3-DNDEBUG 4 4 2 ${n} > Ramsey_4_4_2_${n}.ecnf && ./merge_cnf.sh Ramsey_4_4_2_${n}.ecnf Ramsey_SB_MC_${n}.ecnf > Ramsey_4_4_2_${n}_SB_MC.ecnf && ./ExtendedToStrictDimacs-O3-DNDEBUG < Ramsey_4_4_2_${n}_SB_MC.ecnf > Ramsey_4_4_2_${n}_SB_MC.cnf && ./solvers/minisat2 Ramsey_4_4_2_${n}_SB_MC.cnf minisat-temp > Ramsey_4_4_2_${n}_SB_MC.cnf.result.minisat2 2>&1; cat minisat-temp >> Ramsey_4_4_2_${n}_SB_MC.cnf.result.minisat2;rm -f Ramsey_4_4_2_${n}{,_SB_MC}.{e,}cnf" >> experiments;
    echo "MARCH_PL_MC"$n "./Ramsey-O3-DNDEBUG 4 4 2 ${n} > Ramsey_4_4_2_${n}.ecnf && ./merge_cnf.sh Ramsey_4_4_2_${n}.ecnf Ramsey_SB_MC_${n}.ecnf > Ramsey_4_4_2_${n}_SB_MC.ecnf && ./ExtendedToStrictDimacs-O3-DNDEBUG < Ramsey_4_4_2_${n}_SB_MC.ecnf > Ramsey_4_4_2_${n}_SB_MC.cnf && ./solvers/march_pl Ramsey_4_4_2_${n}_SB_MC.cnf > Ramsey_4_4_2_${n}_SB_MC.cnf.result.march_pl 2>&1;rm -f Ramsey_4_4_2_${n}{_SB_MC,}.{e,}cnf" >> experiments;
    echo "OKSOLVER_MC"$n "./Ramsey-O3-DNDEBUG 4 4 2 ${n} > Ramsey_4_4_2_${n}.ecnf && ./merge_cnf.sh Ramsey_4_4_2_${n}.ecnf Ramsey_SB_MC_${n}.ecnf > Ramsey_4_4_2_${n}_SB_MC.ecnf && ./ExtendedToStrictDimacs-O3-DNDEBUG < Ramsey_4_4_2_${n}_SB_MC.ecnf > Ramsey_4_4_2_${n}_SB_MC.cnf && ./solvers/OKsolver_2002-O3-DNDEBUG -O -D20 -M -F Ramsey_4_4_2_${n}_SB_MC.cnf > Ramsey_4_4_2_${n}_SB_MC.cnf.result.OKsolver 2>&1; mv Ramsey_4_4_2_${n}_SB_MC.cnf.mo Ramsey_4_4_2_${n}_SB_MC.cnf.mo.OKsolver;mv Ramsey_4_4_2_${n}_SB_MC.cnf.pa Ramsey_4_4_2_${n}_SB_MC.cnf.pa.OKsolver;rm -f Ramsey_4_4_2_${n}{,_SB_MC}.{e,}cnf" >> experiments;
    echo "OKSOLVERM2PP_MC"$n "./Ramsey-O3-DNDEBUG 4 4 2 ${n} > Ramsey_4_4_2_${n}.ecnf && ./merge_cnf.sh Ramsey_4_4_2_${n}.ecnf Ramsey_SB_MC_${n}.ecnf > Ramsey_4_4_2_${n}_SB_MC.ecnf && ./ExtendedToStrictDimacs-O3-DNDEBUG < Ramsey_4_4_2_${n}_SB_MC.ecnf > Ramsey_4_4_2_${n}_SB_MC.cnf && ./solvers/OKsolver_2002-m2pp -O -D20 -M -F Ramsey_4_4_2_${n}_SB_MC.cnf > Ramsey_4_4_2_${n}_SB_MC.cnf.result.OKsolver-m2pp 2>&1; mv Ramsey_4_4_2_${n}_SB_MC.cnf_m2pp_*.mo Ramsey_4_4_2_${n}_SB_MC.cnf.mo.OKsolver-m2pp;mv Ramsey_4_4_2_${n}_SB_MC.cnf_m2pp_*.pa Ramsey_4_4_2_${n}_SB_MC.cnf.pa.OKsolver-m2pp;rm -f Ramsey_4_4_2_${n}{,_SB_MC}.{e,}cnf" >> experiments;
# Symmetry breaking 3
   echo "PICOSAT_PHP"$n "./Ramsey-O3-DNDEBUG 4 4 2 ${n} > Ramsey_4_4_2_${n}.ecnf && ./merge_cnf.sh Ramsey_4_4_2_${n}.ecnf Ramsey_SB_PHP_${n}.ecnf > Ramsey_4_4_2_${n}_SB_PHP.ecnf && ./ExtendedToStrictDimacs-O3-DNDEBUG < Ramsey_4_4_2_${n}_SB_PHP.ecnf > Ramsey_4_4_2_${n}_SB_PHP.cnf && ./solvers/picosat913 -v Ramsey_4_4_2_${n}_SB_PHP.cnf > Ramsey_4_4_2_${n}_SB_PHP.cnf.result.picosat 2>&1 && rm -f Ramsey_4_4_2_${n}{,_SB_PHP}.{e,}cnf" >> experiments;
    echo "MINISAT2_PHP"$n "./Ramsey-O3-DNDEBUG 4 4 2 ${n} > Ramsey_4_4_2_${n}.ecnf && ./merge_cnf.sh Ramsey_4_4_2_${n}.ecnf Ramsey_SB_PHP_${n}.ecnf > Ramsey_4_4_2_${n}_SB_PHP.ecnf && ./ExtendedToStrictDimacs-O3-DNDEBUG < Ramsey_4_4_2_${n}_SB_PHP.ecnf > Ramsey_4_4_2_${n}_SB_PHP.cnf && ./solvers/minisat2 Ramsey_4_4_2_${n}_SB_PHP.cnf minisat-temp > Ramsey_4_4_2_${n}_SB_PHP.cnf.result.minisat2 2>&1; cat minisat-temp >> Ramsey_4_4_2_${n}_SB_PHP.cnf.result.minisat2;rm -f Ramsey_4_4_2_${n}{,_SB_PHP}.{e,}cnf" >> experiments;
    echo "MARCH_PL_PHP"$n "./Ramsey-O3-DNDEBUG 4 4 2 ${n} > Ramsey_4_4_2_${n}.ecnf && ./merge_cnf.sh Ramsey_4_4_2_${n}.ecnf Ramsey_SB_PHP_${n}.ecnf > Ramsey_4_4_2_${n}_SB_PHP.ecnf && ./ExtendedToStrictDimacs-O3-DNDEBUG < Ramsey_4_4_2_${n}_SB_PHP.ecnf > Ramsey_4_4_2_${n}_SB_PHP.cnf && ./solvers/march_pl Ramsey_4_4_2_${n}_SB_PHP.cnf > Ramsey_4_4_2_${n}_SB_PHP.cnf.result.march_pl 2>&1;rm -f Ramsey_4_4_2_${n}{_SB_PHP,}.{e,}cnf" >> experiments;
    echo "OKSOLVER_PHP"$n "./Ramsey-O3-DNDEBUG 4 4 2 ${n} > Ramsey_4_4_2_${n}.ecnf && ./merge_cnf.sh Ramsey_4_4_2_${n}.ecnf Ramsey_SB_PHP_${n}.ecnf > Ramsey_4_4_2_${n}_SB_PHP.ecnf && ./ExtendedToStrictDimacs-O3-DNDEBUG < Ramsey_4_4_2_${n}_SB_PHP.ecnf > Ramsey_4_4_2_${n}_SB_PHP.cnf && ./solvers/OKsolver_2002-O3-DNDEBUG -O -D20 -M -F Ramsey_4_4_2_${n}_SB_PHP.cnf > Ramsey_4_4_2_${n}_SB_PHP.cnf.result.OKsolver 2>&1; mv Ramsey_4_4_2_${n}_SB_PHP.cnf.mo Ramsey_4_4_2_${n}_SB_PHP.cnf.mo.OKsolver;mv Ramsey_4_4_2_${n}_SB_PHP.cnf.pa Ramsey_4_4_2_${n}_SB_PHP.cnf.pa.OKsolver;rm -f Ramsey_4_4_2_${n}{,_SB_PHP}.{e,}cnf" >> experiments;
    echo "OKSOLVERM2PP_PHP"$n "./Ramsey-O3-DNDEBUG 4 4 2 ${n} > Ramsey_4_4_2_${n}.ecnf && ./merge_cnf.sh Ramsey_4_4_2_${n}.ecnf Ramsey_SB_PHP_${n}.ecnf > Ramsey_4_4_2_${n}_SB_PHP.ecnf && ./ExtendedToStrictDimacs-O3-DNDEBUG < Ramsey_4_4_2_${n}_SB_PHP.ecnf > Ramsey_4_4_2_${n}_SB_PHP.cnf && ./solvers/OKsolver_2002-m2pp -O -D20 -M -F Ramsey_4_4_2_${n}_SB_PHP.cnf > Ramsey_4_4_2_${n}_SB_PHP.cnf.result.OKsolver-m2pp 2>&1; mv Ramsey_4_4_2_${n}_SB_PHP.cnf_m2pp_*.mo Ramsey_4_4_2_${n}_SB_PHP.cnf.mo.OKsolver-m2pp;mv Ramsey_4_4_2_${n}_SB_PHP.cnf_m2pp_*.pa Ramsey_4_4_2_${n}_SB_PHP.cnf.pa.OKsolver-m2pp;rm -f Ramsey_4_4_2_${n}{,_SB_PHP}.{e,}cnf" >> experiments;
# Cardinality Constraints
   echo "PICOSAT_CC"$n "./Ramsey-O3-DNDEBUG 4 4 2 ${n} | ./ExtendedToStrictDimacs-O3-DNDEBUG > Ramsey_4_4_2_${n}.cnf && ./merge_cnf.sh Card/Card_n${n}.cnf Ramsey_4_4_2_${n}.cnf > Ramsey_4_4_2_${n}_CC.cnf && ./solvers/picosat913 -v Ramsey_4_4_2_${n}_CC.cnf > Ramsey_4_4_2_${n}_CC.cnf.result.picosat 2>&1 && rm -f Ramsey_4_4_2_${n}{,_CC}.{e,}cnf" >> experiments;
    echo "MINISAT2_CC"$n "./Ramsey-O3-DNDEBUG 4 4 2 ${n} | ./ExtendedToStrictDimacs-O3-DNDEBUG > Ramsey_4_4_2_${n}.cnf && ./merge_cnf.sh Card/Card_n${n}.cnf Ramsey_4_4_2_${n}.cnf > Ramsey_4_4_2_${n}_CC.cnf && ./solvers/minisat2 Ramsey_4_4_2_${n}_CC.cnf minisat-temp > Ramsey_4_4_2_${n}_CC.cnf.result.minisat2 2>&1; cat minisat-temp >> Ramsey_4_4_2_${n}_CC.cnf.result.minisat2;rm -f Ramsey_4_4_2_${n}{,_CC}.{e,}cnf" >> experiments;
#    echo "MARCH_PL_CC"$n "./Ramsey-O3-DNDEBUG 4 4 2 ${n} | ./ExtendedToStrictDimacs-O3-DNDEBUG > Ramsey_4_4_2_${n}.cnf && ./merge_cnf.sh Card/Card_n${n}.cnf Ramsey_4_4_2_${n}.cnf > Ramsey_4_4_2_${n}_CC.cnf && ./solvers/march_pl Ramsey_4_4_2_${n}_CC.cnf > Ramsey_4_4_2_${n}_CC.cnf.result.march_pl 2>&1;rm -f Ramsey_4_4_2_${n}{_CC,}.{e,}cnf" >> experiments;
    echo "OKSOLVER_CC"$n "./Ramsey-O3-DNDEBUG 4 4 2 ${n} | ./ExtendedToStrictDimacs-O3-DNDEBUG > Ramsey_4_4_2_${n}.cnf && ./merge_cnf.sh Card/Card_n${n}.cnf Ramsey_4_4_2_${n}.cnf > Ramsey_4_4_2_${n}_CC.cnf && ./solvers/OKsolver_2002-O3-DNDEBUG -O -D20 -M -F Ramsey_4_4_2_${n}_CC.cnf > Ramsey_4_4_2_${n}_CC.cnf.result.OKsolver 2>&1; mv Ramsey_4_4_2_${n}_CC.cnf.mo Ramsey_4_4_2_${n}_CC.cnf.mo.OKsolver;mv Ramsey_4_4_2_${n}_CC.cnf.pa Ramsey_4_4_2_${n}_CC.cnf.pa.OKsolver;rm -f Ramsey_4_4_2_${n}{,_CC}.{e,}cnf" >> experiments;
    echo "OKSOLVERM2PP_CC"$n "./Ramsey-O3-DNDEBUG 4 4 2 ${n} | ./ExtendedToStrictDimacs-O3-DNDEBUG > Ramsey_4_4_2_${n}.cnf && ./merge_cnf.sh Card/Card_n${n}.cnf Ramsey_4_4_2_${n}.cnf > Ramsey_4_4_2_${n}_CC.cnf && ./solvers/OKsolver_2002-m2pp -O -D20 -M -F Ramsey_4_4_2_${n}_CC.cnf > Ramsey_4_4_2_${n}_CC.cnf.result.OKsolver-m2pp 2>&1; mv Ramsey_4_4_2_${n}_CC.cnf_m2pp_*.mo Ramsey_4_4_2_${n}_CC.cnf.mo.OKsolver-m2pp;mv Ramsey_4_4_2_${n}_CC.cnf_m2pp_*.pa Ramsey_4_4_2_${n}_CC.cnf.pa.OKsolver-m2pp;rm -f Ramsey_4_4_2_${n}{,_CC}.{e,}cnf" >> experiments;
done
   \endverbatim
   </li>
   <li> The above experiment (in the generation script) is currently running on
   cssinope with a timeout of 2 hours. </li>
   <li> Combining all such investigations (different symmetry breaking 
   techniques, cardinality contraints etc) into a single experiment allows one
   to run through all such investigations for a short time and then split the
   experiments if certain aspects are slowing the entire experiment set down
   or it makes more sense to run certain parts, or solvers in parallel. </li>
  </ul>
 

*/

