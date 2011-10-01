// Oliver Kullmann, 12.3.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/5-k/09.hpp
  \brief On investigations into vdw_2(5,9)


  \todo vdw_2(5,9) > 472
  <ul>
   <li> Certificates for n=472:
   \verbatim
4,6,8,9,13,17,19,24,27,28,
34,39,41,43,44,45,48,52,53,56,
57,59,63,65,70,72,77,78,80,82,
85,94,95,97,98,100,101,102,106,108,
112,114,116,117,123,126,130,139,140,144,
149,153,156,162,163,165,167,171,173,174,
177,178,179,182,184,188,192,194,197,198,
199,202,203,205,209,211,213,214,220,223,
227,236,237,246,250,253,259,260,262,264,
268,270,271,274,275,276,279,281,285,289,
291,294,295,296,299,300,302,306,308,310,
311,317,320,324,329,333,334,343,347,350,
356,357,359,361,365,367,371,372,373,375,
376,378,379,388,391,393,395,396,401,403,
408,410,414,416,417,420,421,425,428,429,
430,432,434,439,445,446,449,454,456,460,
464,465,467,469
   \endverbatim
   </li>
   <li> Starting from scratch:
   \verbatim
> k1=5 k2=9 n0=10 alg="adaptg2wsat" runs=1000 cutoff=400000000; nohup RunVdWk1k2 ${k1} ${k2} ${n0} "${alg}" ${runs} ${cutoff} &

# Found a solution for n=446 (derived from n=442, where 93 runs were needed).
> E=read_ubcsat("VanDerWaerden_2-5-9_447.cnf_OUT")
  1   2   3
 16 130  34
180
   \endverbatim
   This looks inefficient. </li>
   <li> Best ubcsat-algorithm:
   \verbatim
> VanderWaerdenCNF-O3-DNDEBUG 5 9 472
n=472 c=41300 l=261252
 length   count
      5   27612
      9   13688

> E=run_ubcsat("VanDerWaerden_2-5-9_472.cnf",runs=100,cutoff=1000000)
> eval_ubcsat_dataframe(E)
1. ddfw:
 6  7  8  9 10 11 12
 1  1  7 16 32 32 11
fps: 40002
2. dano:
 6  8  9 10 11 12 13
 1  3  9 24 34 26  3
fps: 124881
3. rnovp:
 7  8  9 10 11 12 13
 5  5 20 31 24 13  2
fps: 126714
4. rnov:
 7  8  9 10 11 12 13
 4 13 16 34 20 10  3
fps: 126621
5. ag2wsat:
 8  9 10 11 12 13
10 24 28 23 12  3
fps: 180183
6. anovp:
 8  9 10 11 12 13 14
 3 12 26 39  9 10  1
fps: 125041
7. anovpp:
 8  9 10 11 12 13
 2 10 22 34 25  7
fps: 124513
8. rots:
10 11 12 13 14 15 16 17
 2  4 12 22 23 25 11  1
fps: 184468

> E=run_ubcsat("VanDerWaerden_2-5-9_472.cnf",runs=100,cutoff=10000000,include_algs=list("ddfw","dano","rnovp","rnov","ag2wsat","anovp","anovpp"))
> eval_ubcsat_dataframe(E)
1. rnov:
 5  6  7  8  9 10
 1  7 28 40 23  1
fps: 126922
2. rnovp:
 5  6  7  8  9 10
 1  5 21 41 29  3
fps: 126519
3. ag2wsat:
 6  7  8  9 10
 5 10 37 45  3
fps: 182104
4. anovpp:
 6  7  8  9 10 11
 4  5 26 39 24  2
fps: 124440
5. ddfw:
 6  7  8  9 10
 3 18 33 39  7
fps: 38850
6. anovp:
 6  7  8  9 10 11
 3  8 26 44 17  2
fps: 125147
7. dano:
 6  7  8  9 10
 1 13 21 45 20
fps: 124954
   \endverbatim
   Looks like rnovelty is the relatively clear winner. </li>
   <li> Using the palindromic solution:
   \verbatim
> cat VanDerWaerden_pd_2-5-9_472.cnf_sol | PdExtend-O3-DNDEBUG 472 > VanDerWaerden_2-5-9_472.cnf_sol
> k1=5 k2=9 n0=472 alg="rnovelty" runs=1000 cutoff=400000000; RunVdWk1k2 ${k1} ${k2} ${n0} "${alg}" ${runs} ${cutoff} VanDerWaerden_2-${k1}-${k2}_${n0}.cnf_sol
> E=read_ubcsat("VanDerWaerden_2-5-9_473.cnf_OUT")
 3  4  5  6  7
 1  1  5 26 10
43
XXX
   \endverbatim
   </li>
  </ul>


  \todo Best complete solver for palindromic problems
  <ul>
   <li> It seems that from the conflict-driven solvers minisat-2.2.0 is best,
   from the look-ahead solvers march_pl (followed by OKsolver_2002, then
   satz215), and that overall minisat-2.2.0 is best. </li>
  </ul>


  \todo Best local-search solver for palindromic problems


  \todo vdw_2^pd(5,9) >= (446,473)
  <ul>
   <li> Certificates:
    <ol>
     <li> n=445:
     \verbatim
1,2,4,11,12,20,21,26,31,34,
36,38,42,45,48,49,51,53,58,59,
60,62,65,66,67,69,70,72,73,82,
83,85,86,88,89,90,95,97,98,102,
110,113,119,121,123,124,126,128,129,131,
133,139,142,150,155,157,162,163,164,166,
167,169,170,179,180,182,183,185,186,187,
192,194,199,207,210,216,218,220,221,223
     \endverbatim
     Another solution (closely related):
     \verbatim
1,2,11,12,20,21,26,28,31,34,
36,42,45,48,49,51,53,58,59,60,
62,65,66,67,69,70,72,73,82,83,
85,86,88,89,90,95,97,98,102,110,
113,119,121,123,124,126,128,129,131,133,
139,142,150,155,157,162,163,164,166,167,
169,170,179,180,182,183,185,186,187,192,
194,199,207,210,216,218,220,221,223
     \endverbatim
     </li>
     <li> n=472:
     \verbatim
4,6,8,9,13,17,19,24,27,28,
34,39,41,43,44,45,48,52,53,56,
57,59,63,65,70,72,77,78,80,82,
85,94,95,97,98,100,101,102,106,108,
112,114,116,117,123,126,130,139,140,144,
149,153,156,162,163,165,167,171,173,174,
177,178,179,182,184,188,192,194,197,198,
199,202,203,205,209,211,213,214,220,223,
227,236
     \endverbatim
    </ol>
   </li>
   <li> Using SplittingViaOKsolver together with minisat-2.2.0:
    <ol>
     <li> n=447:
     \verbatim
> SplittingViaOKsolver -D17 -SD VanDerWaerden_pd_2-5-9_447.cnf
> cd SplitViaOKsolver_D17VanDerWaerden_pd_259_447cnf_2011-05-18-182137/
> more Md5sum
f11f67fd06ffd653818137f130c07a7c
> more Statistics
> E=read.table("Data")
> summary(E$n)
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
  17.00   24.00   29.00   30.12   35.00   86.00
> table(E$n)
  17   18   19   20   21   22   23   24   25   26   27   28   29   30   31   32
  84  621 1875 3835 6179 8093 8762 8359 7418 6268 5585 4964 5027 5166 5479 5613
  33   34   35   36   37   38   39   40   41   42   43   44   45   46   47   48
5610 5581 5324 4833 4192 3606 2998 2614 2184 1799 1487 1153  971  794  629  488
  49   50   51   52   53   54   55   56   57   58   59   60   61   62   63   64
 427  361  285  219  219  170  127  105   72   87   54   64   40   39   25   22
  65   66   67   68   69   70   71   72   73   74   75   76   77   78   80   81
  23   24   18   18    9   11    5   10    8    6    3    2    1    3    3    1
  85   86
   1    1
> more Result
s UNKNOWN
c sat_status                            2
c initial_maximal_clause_length         9
c initial_number_of_variables           224
c initial_number_of_clauses             18025
c initial_number_of_literal_occurrences 113027
c number_of_initial_unit-eliminations   0
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           0
c reddiff_number_of_clauses             0
c reddiff_number_of_literal_occurrences 0
c number_of_2-clauses_after_reduction   0
c running_time(sec)                     6306.2
c number_of_nodes                       261725
c number_of_single_nodes                0
c number_of_quasi_single_nodes          0
c number_of_2-reductions                25028
c number_of_pure_literals               0
c number_of_autarkies                   0
c number_of_missed_single_nodes         0
c max_tree_depth                        17
c number_of_table_enlargements          0
c number_of_1-autarkies                 0
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             VanDerWaerden_pd_2-5-9_447.cnf
c splitting_directory                   SplitViaOKsolver_D17VanDerWaerden_pd_259_447cnf_2011-05-18-182137/Instances
c splitting_cases                       130054

> E=read.table("Data")
> E[E$n==20,][1:10,]
          i  n
123640 1255 20
123641 1646 20
123642 1647 20
123643 1663 20
123644 1664 20
123645 1711 20
123646 1712 20
123647 1883 20
123648 1884 20
123649 1885 20

> cat VanDerWaerden_pd_2-5-9_447.cnf | ApplyPass-O3-DNDEBUG Instances/1255 > Test1255.cnf
> minisat-2.2.0 Test1255.cnf
restarts              : 13820
conflicts             : 8521124        (12794 /sec)
decisions             : 9898141        (0.00 % random) (14861 /sec)
propagations          : 212201322      (318597 /sec)
conflict literals     : 195070169      (27.77 % deleted)
Memory used           : 44.00 MB
CPU time              : 666.05 s
INDETERMINATE
     \endverbatim
     This looks rather bleak.
     </li>
     <li> Okay, let's set n=23, "n-interpretation":
     \verbatim
> SplittingViaOKsolver -D23 VanDerWaerden_pd_2-5-9_447.cnf
> cd SplitViaOKsolver_D23SNVanDerWaerden_pd_259_447cnf_2011-05-19-162049
> more Md5sum
4ff22969902730f5ccbebea4e17f1db1
> more Statistics
> E=read.table("Data")
> summary(E$n)
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
   23.0    23.0    23.0    23.9    24.0    43.0
> table(E$n)
   23    24    25    26    27    28    29    30    31    32    33    34    35
52207 26728 13411  5934  2457   963   410   178    88    50    30    24    11
   36    37    38    40    43
    8     4     4     1     1
> more Result
s UNKNOWN
c sat_status                            2
c running_time(sec)                     4262.6
c number_of_nodes                       205017
c number_of_2-reductions                1769
c max_tree_depth                        23
c splitting_cases                       102509

> tail Data
102500 102478 23
102501 102480 23
102502 102481 23
102503 102482 23
102504 102488 23
102505 102494 23
102506 102495 23
102507 102503 23
102508 102504 23
102509 102505 23

> cat VanDerWaerden_pd_2-5-9_447.cnf | ApplyPass-O3-DNDEBUG Instances/102478 > I102478.cnf
> minisat-2.2.0 I102478.cnf
conflicts             : 272490         (17725 /sec)
CPU time              : 15.373 s
UNSATISFIABLE
> cat VanDerWaerden_pd_2-5-9_447.cnf | ApplyPass-O3-DNDEBUG Instances/102505 > I102505.cnf
> minisat-2.2.0 I102505.cnf
conflicts             : 51219          (17279 /sec)
CPU time              : 2.96419 s
UNSATISFIABLE
     \endverbatim
     </li>
     <li> Okay, let's then try n=25:
     \verbatim
> SplittingViaOKsolver -D25 VanDerWaerden_pd_2-5-9_447.cnf
> cd SplitViaOKsolver_D25SNVanDerWaerden_pd_259_447cnf_2011-05-19-182415
> cd more Md5sum
bd884564a5da1944e4820c0120010133
> more Statistics
> E=read.table("Data")
> summary(E$n)
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
  25.00   25.00   26.00   25.97   27.00   48.00
> table(E$n)
    25     26     27     28     29     30     31     32     33     34     35
112730  60677  31829  14740   6319   2772   1229    573    261    173     93
    36     37     38     39     40     41     42     43     45     47     48
    65     35     20     12      6      1      5      6      2      1      1
> more Result
c running_time(sec)                     9183.4
c number_of_nodes                       463099
c number_of_2-reductions                5286
c max_tree_depth                        25
c splitting_cases                       231550

> cd Instances
> I="../$(cat ../F)"; echo " i n t cfs" > Stats; time tail -n +2 ../Data | while read C F N; do cat $I | ApplyPass-O3-DNDEBUG $F Temp.cnf; minisat-2.2.0 Temp.cnf >Temp.out 2>&1; S=$?; if [[ $S != 20 ]]; then echo -e "UNEXPECTED RETURN VALUE ${S}\!"; break; else T=$(cat Temp.out | awk '/CPU time/ {print $4}'); CF=$(cat Temp.out | awk '/conflicts/ {print $3}'); echo "$C $F $N $T $CF" >> Stats; echo -n "$C:$T "; fi; done
# INTERRUPTED:
real    10123m47.215s
user    9945m21.669s
sys     9m22.735s

# Monitoring in R via
#> E=read.table("Stats",header=TRUE,colClasses=c("integer","integer","integer","numeric","numeric")); plot(E$t); cat(sprintf("%d: %.2fh, sum-cfs=%e, mean-t=%.3fs, mean-cfs=%.0f",length(E$t),sum(E$t)/60/60,sum(E$cfs),mean(E$t),mean(E$cfs)),"\n")
13246: 165.07h, sum-cfs=9.404562e+09, mean-t=44.863s, mean-cfs=709993
     \endverbatim
     Still instances are too hard. </li>
     <li> To have a chance, let's try n=28:
     \verbatim
> SplittingViaOKsolver -D28 VanDerWaerden_pd_2-5-9_447.cnf
> cd SplitViaOKsolver_D28SNVanDerWaerden_pd_259_447cnf_2011-05-27-174054/
> more Md5sum
54431bf25ea1625939ac37201678490a
> more Statistics
> E=read.table("Data")
> summary(E$n)
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
  28.00   28.00   29.00   29.11   30.00   85.00
> table(E$n)
    28     29     30     31     32     33     34     35     36     37     38
341102 190356 109098  55076  25984  12113   5818   2722   1521    882    567
    39     40     41     42     43     44     45     46     47     48     49
   333    190    125     77     53     39     33     21     20     10     14
    50     51     52     53     54     55     56     57     59     62     70
     7      6      1      3      3      2      4      2      2      1      1
    78     85
     1      1
> more Result
c running_time(sec)                     32155.7
c number_of_nodes                       1492387
c number_of_2-reductions                24983
c max_tree_depth                        28
c splitting_cases                       746188

> tail -2 Data
746187 746184 28
746188 746188 28
> cat VanDerWaerden_pd_2-5-9_447.cnf | ApplyPass-O3-DNDEBUG Instances/746184 746184.cnf
> minisat-2.2.0 746184.cnf
conflicts             : 278932         (18417 /sec)
CPU time              : 15.1449 s
> cat VanDerWaerden_pd_2-5-9_447.cnf | ApplyPass-O3-DNDEBUG Instances/746188 746188.cnf
> minisat-2.2.0 746188.cnf
conflicts             : 54483          (20857 /sec)
CPU time              : 2.61216 s

> cd Instances
> OKP=~/OKplatform; I="../$(cat ../F)"; echo " i n t sat cfs dec rts r1 mem ptime stime cfl" > Stats; time tail -n +2 ../Data | while read C F N; do cat $I | ApplyPass-O3-DNDEBUG $F Temp.cnf; minisat-2.2.0 Temp.cnf >Temp.out 2>&1; S=$?; if [[ $S != 20 ]]; then echo -e "UNEXPECTED RETURN VALUE ${S}\!"; break; else echo -n "$C " >> Stats; awk -f ${OKP}/OKsystem/OKlib/Experimentation/ExperimentSystem/SolverMonitoring/ExtractMinisat.awk Temp.out >> Stats; echo -n "$C "; fi; done
# Aborted

# Monitoring in R via
#> E=read.table("Stats",header=TRUE,colClasses=c(rep("integer",3),"numeric","integer",rep("numeric",8))); plot(E$t); cat(sprintf("%d: %.2fh, sum-cfs=%e, mean-t=%.3fs, mean-cfs=%.0f",length(E$t),sum(E$t)/60/60,sum(E$cfs),mean(E$t),mean(E$cfs)),"\n")
30257: 44.36h, sum-cfs=3.240764e+09, mean-t=5.278s, mean-cfs=107108
     \endverbatim
     Still instances are too hard. </li>
     <li> Now n=30:
     \verbatim
> SplittingViaOKsolver -D30 VanDerWaerden_pd_2-5-9_447.cnf
> more Md5sum
48e67af9785a537c45a78f86c403346d
> more Statistics
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
  30.00   30.00   31.00   31.22   32.00   85.00
    30     31     32     33     34     35     36     37     38     39     40
682765 395286 238455 126854  63102  30353  15045   7750   4201   2493   1489
    41     42     43     44     45     46     47     48     49     50     51
   961    639    462    264    206    158    111     78     67     45     39
    52     53     54     55     56     57     58     59     60     61     62
    33     15     18     18     15      6      9      5      5      4      6
    63     64     65     66     68     70     73     76     77     78     84
     1      3      1      3      1      2      2      1      1      1      1
    85
     1
> more Result
c running_time(sec)                     65870.3
c number_of_nodes                       3142047
c number_of_2-reductions                68934
c max_tree_depth                        29
c splitting_cases                       1570975

> cd Instances
> OKP=~/OKplatform; I="../$(cat ../F)"; echo " i n t sat cfs dec rts r1 mem ptime stime cfl" > Stats; time tail -n +2 ../Data | while read C F N; do cat $I | ApplyPass-O3-DNDEBUG $F Temp.cnf; minisat-2.2.0 Temp.cnf >Temp.out 2>&1; S=$?; echo -n "$C " >> Stats; awk -f ${OKP}/OKsystem/OKlib/Experimentation/ExperimentSystem/SolverMonitoring/ExtractMinisat.awk Temp.out >> Stats; if [[ $S != 20 ]]; then echo -e "UNEXPECTED RETURN VALUE ${S}\!"; break; else echo -n "$C "; fi; done

# interim results:
647756: 1551.97h, sum-cfs=1.084401e+11, mean-t=8.625s, mean-cfs=167409
# the plot shows clear epochs, likely coming from the different sizes
# of the partial assignments
# aborted:
>  E = read.table(file="Instances/Stats", header = T, colClasses = c(rep("integer",3),"numeric","integer",rep("numeric",8)))
> cat(sprintf("%d: %s, sum-cfs=%e, mean-t=%.3fs, mean-cfs=%.0f",length(E$t),display_seconds(sum(E$t)),sum(E$cfs),mean(E$t),mean(E$cfs)),"\n")
688712: 70.612d, sum-cfs=1.182415e+11, mean-t=8.858s, mean-cfs=171685
>   basic_stats(E$t)
    Min.  1st Qu.   Median     Mean  3rd Qu.     Max.
   0.012    0.776    2.096    8.858    6.296 3490.000
sd= 30.40892
       95%        96%        97%        98%        99%       100%
  34.16610   41.80660   54.71808   70.86440  124.12180 3489.79000
sum= 6100855
>   basic_stats(E$t)
    Min.  1st Qu.   Median     Mean  3rd Qu.     Max.
   0.012    0.776    2.096    8.858    6.296 3490.000
sd= 30.40892
       95%        96%        97%        98%        99%       100%
  34.16610   41.80660   54.71808   70.86440  124.12180 3489.79000
sum= 6100855
>   basic_stats(E$cfs)
    Min.  1st Qu.   Median     Mean  3rd Qu.     Max.
      12    19140    50830   171700   142300 41050000
sd= 475336.7
       95%        96%        97%        98%        99%       100%
  669208.9   799272.7  1028591.7  1287994.8  2106368.9 41050365.0
sum= 118241474871
     \endverbatim
     That seems feasible. </li>
     <li> Finally D=32 should do the job:
     \verbatim
> SplittingViaOKsolver -D32 VanDerWaerden_pd_2-5-9_447.cnf
> cd SplitViaOKsolver_D32VanDerWaerden_pd_259_447cnf_2011-08-25-125535/
> more Md5sum
9c4a963031d9171746f1557891cd382d
> more Statistics
> E=read.table("Data")
> summary(E$n)
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
  32.00   32.00   33.00   33.33   34.00   86.00
> table(E$n)
     32      33      34      35      36      37      38      39      40      41
1331324  792944  500247  279063  143368   73074   37656   19971   11130    6707
     42      43      44      45      46      47      48      49      50      51
   4266    2765    1860    1269     928     683     486     335     292     223
     52      53      54      55      56      57      58      59      60      61
    182     135     113      89      67      50      48      39      23      27
     62      63      64      65      66      67      68      69      70      71
     25      13      22      21       9       8       8       6      11       7
     72      73      74      75      76      77      78      79      80      84
      7       5       5       4       5       2       3       5       1       1
     85      86
      2       1
> summary(E$d)
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
  12.00   23.00   24.00   23.74   25.00   31.00
> table(E$d)
    12     13     14     15     16     17     18     19     20     21     22
    17    131    816   3287   8143  14733  24639  43255  93290 209747 396597
    23     24     25     26     27     28     29     30     31
585401 660314 562783 361275 171665  58524  13047   1789     82
> more Result
c running_time(sec)                     116641.1 (cs-oksvr)
c number_of_nodes                       6419715
c number_of_2-reductions                188048
c splitting_cases                       3209535

> ProcessSplitViaOKsolver SplitViaOKsolver_D32VanDerWaerden_pd_259_447cnf_2011-08-25-125535
> E=read_processsplit_minisat()
863092: 20.865d, sum-cfs=3.982016e+10, mean-t=2.089s, mean-cfs=46137
$t:
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
  0.020   0.312   0.724   2.089   1.712 773.400
sd= 6.944693
XXX
     \endverbatim
     </li>
    </ol>
   </li>
   <li> "RunPdVdWk1k2 5 9 adaptg2wsat 100 2000000" yields
   \verbatim
Break point 1: 446
Break point 2: 451
   \endverbatim
   finding all solutions in at most 53 runs.
   </li>
   <li> "RunPdVdWk1k2 5 9 adaptg2wsat 100 10000000" yields
   \verbatim
Break point 1: 446
Break point 2: 473
   \endverbatim
   finding all solutions in at most 21 runs.
   </li>
  </ul>

*/
