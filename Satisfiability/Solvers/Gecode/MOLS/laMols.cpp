// Oleg Zaikin, 6.3.2022 (Swansea)
/* Copyright 2022, 2023, 2024 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  A Gecode-based solver for general MOLS-LS-related problems.
  using look-ahead for reduction and branching

  Remarks on command-line arguments:

   - The branch-order are permutations of the branches:
    - order tprob computes one branch according to the
      tau-probabilities, and swaps this with the first branch (otherwise
      we just have the natural order of values)
    - so tprob likely only makes sense when computing one branch
    - the order rand and tprob use randomisation (the only randomised aspects
      of laMols).

   - When a stopping-criterion is given, then the first time the given measure
     is noticed for (strictly) exceeding the given value, the algorithm stops;
     so "lvs,0" with one thread means stopping once the first leaf has been
     reached (while with more than one thread, more leaves might be created).

   - Estimation of the number of leaves:
    - For each leaf we have estlvs and uestlvs.
    - The former is the reciprocal of the product of the tau-probabilities.
    - The latter is the product of the widths.
    - For these measurements their global statistics are the usual
      one (taking each leaf with the same probability).
    - On the other hand, for the global statistics nsel and nsuel the
      probability of a leaf is the reciprocal value of its prediction
      value. With this distribution, the expection is exactly the number
      of leaves, and reported is the "normalised standard deviations"
      (also coefficient of variation (CV), the ratio of standard deviation
      divided by the number of leaves.
    - These quantities make only sense for the completed tree (while all
      other measures make sense for partial trees).

Examples:

1. Counting the 432 Euler-squares (pairs of mutually orthogonal latin squares)
of order 5, which are row-reduced (stating the conditions in the command-line
argument, and making all values explicit):

Inserted are basic explanations via "[ ]":

MOLS> ./laMols 5 "@squares A B aux\nls A B aux\nrred A B\nrprod B aux A\n" "" count dom enu wdL asc "" relpr 1 3 "0.1" "" ""
# laMols 0.105.2 caed91f79f53d21da1d724ded7ca4ac70cfeb0d6
# command-line: "./laMols" "5" "@squares A B aux\nls A B aux\nrred A B\nrprod B aux A\n" "" "count" "dom" "enu" "wdL" "asc" "" "relpr" "1" "3" "0.1" "" ""
# N: 5
# k=3 total_num_sq=3: "@squares A B aux\nls A B aux\nrred A B\nrprod B aux A\n"
#   num_uc=5 num_eq=0 num_peq=1
#   hash=773446287326109641

[ the hash-value of the specs does not depend on the order of conditions,
  nor on the names of the squares, but does depend on the order of squares
  specified in the first line of specifications ]

# no_ps
# num_runs=1
# threads=3
# rt=count-solutions(count)
# no_stopping
# output-options: show-info(+info) show-weights(+w) show-headers(+headers) perform_computations(+computations) without-tree-logging(-tree) all no-negation-sign(+sign) show-stopped(+stop) arithmetic-mean(ave) inner-node(inode)
#   propagation-level: domain-prop(dom)
#   la-branching-type: enumerative-branching(enu)
#   distance-type: weighted-delta-literals(wdL)
#   la-order-heuristic: ascending-order(asc)
#   la-reduction-level: basic-lookahead(labsc)
#   la-reduction-algorithm: relaxed-pruning(relpr)
#   commit-distance: 1

[ Showing the real weights: ]
#   weights: 0.1 0.1 0.1 -> 0 0 1 1.0717734625362931642 1.1486983549970350067 1.2311444133449162843

[ Input parameters and total counts / measures: ]

  N       rt  pl lbt       dis   lbo    rdl    lar gcd     satc           t        ppc st      nds      lvs          nsel         nsuel
  5    count dom enu       wdL   asc  labsc  relpr   1      432       0.349       5225  0      125       72  4.754312e-02  0.000000e+00

[ satc : satisfying assignments
  t    : time (s)
  ppc  : propagation-calls (reported by gecode)
  st   : 0 means false (i.e., no stopping)
  nds  : nodes
  lvs  : leaves
  nsel  : normalised standard deviation of estlvs-measure
  nsuel : normalised standard deviation of uestlvs-measure ]

[ Statistics for reductions at inner nodes: ]

   mu0     dmu0  pprunes  pmprune  pprobes   rounds  solc         tr  pelvals       dp  efv
148.38  0.90566   8.8165   99.547   125.38   1.6792     0  0.0076145  0.45284    2.566    0
   143        0        0   99.005   98.684        1     0   0.001478        0        0    0
   180        2    16.35      100   144.78        2     0   0.013217  0.99502        3    0
8.5355  0.74069   6.3903   0.3688    18.24  0.47123     0  0.0027152   0.3688  0.72083    0

[ mu0     : number of variables, weighted by domainsize-1, before reduction
  dmu0    : Delta of mu0 (before and after reduction)
  pprunes : percentage of pruned values (i.e., didn't need testing) relative to probed values
  pmprune : percentage of max-pruning-size amongst all open values
  pprobes : percentage of probed values amongst all open values
  rounds  : la-reduction rounds
  solc    : solution-count (found at the node)
  tr      : time for reduction
  pelvals : percentage of eliminated values amongst all open values
  dp      : depth (per node)
  efv     : number of eliminated formal (non-occurring) variables  ]
[ Per column: average, minimum, maximum, standard deviation (same below) ]

[ Statistics for leaves: ]

    mu0   dmu0  pprunes  pmprune  pprobes   rounds  solc         tr  pelvals  dp  efv
 129.67  40.25     6.52   34.388   83.838      1.5     6  0.0055236    25.16   4    0
    129     20   1.1628   16.304   45.652        1     6   0.000931   12.022   4    0
    130     63   12.308     62.5   124.46        2     6   0.011202   41.304   4    0
0.47471  10.79   3.0145   13.191   28.464  0.50351     0  0.0026263   7.1985   0    0

[ Per leaf we have estlvs and uestlvs, which yield the following statistics: ]

      estlvs       uestlvs
7.216275e+01  7.200000e+01
6.739697e+01  7.200000e+01
7.454563e+01  7.200000e+01
3.393564e+00  0.000000e+00

[ Statistics on branchings (for inner nodes): ]

dn        w  ltausp      minp     meanp      maxp       sddp         tb
 0   2.3396  2.7476   0.44239   0.44497   0.45012  0.0036448  0.0055959
 0        2  1.0456      0.25      0.25      0.25          0   0.001676
 0        4  3.4742       0.5       0.5       0.5   0.016098   0.009875
 0  0.51677  0.8992  0.085275  0.081622  0.074711  0.0068016  0.0023966

[ dn : number of eliminated open variables by the reduction
  w : width (of chosen branching)
  ltausp :  quotient of worst / best logarithmic-tau of all branchings considered
  minp, meanp, maxp, sddp : tau-probabilities (of chosen branching)
  tb : time for branching ]

Remark: This tree is the perfect tree with 4 levels and widths 4,3,3,2 at these
levels; so there are 4 * 3^2 * 2 = 72 leaves and 1 + 4 + 4*3 + 4*3^2 = 53 inner
nodes, while the average width is (1*4 + 4*3 +12*3 + 36*2)/53 = 124/53.
So nsuel = 0.
At each leaf there are 6 solutions.

Just picking one random leaf:
MOLS> ./laMols 5 "@squares A B aux\nls A B aux\nrred A B\nrprod B aux A\n" "" count "" "" wdL "rand" "" "" "" 1 0.1 lvs,0 ""
# laMols 0.105.2 caed91f79f53d21da1d724ded7ca4ac70cfeb0d6
# command-line: "./laMols" "5" "@squares A B aux\nls A B aux\nrred A B\nrprod B aux A\n" "" "count" "" "" "wdL" "rand" "" "" "" "1" "0.1" "lvs,0" ""
# N: 5
# k=3 total_num_sq=3: "@squares A B aux\nls A B aux\nrred A B\nrprod B aux A\n"
#   num_uc=5 num_eq=0 num_peq=1
#   hash=773446287326109641
# no_ps
# num_runs=1
# threads=1
# rt=count-solutions(count)
# stopping: by-leaf-count(lvs),0
# output-options: show-info(+info) show-weights(+w) show-headers(+headers) perform_computations(+computations) without-tree-logging(-tree) all no-negation-sign(+sign) show-stopped(+stop) arithmetic-mean(ave) inner-node(inode)
#   propagation-level: domain-prop(dom)
#   la-branching-type: enumerative-branching(enu)
#   distance-type: weighted-delta-literals(wdL)
#   la-order-heuristic: random-order(rand)
#   la-reduction-level: basic-lookahead(labsc)
#   la-reduction-algorithm: relaxed-pruning(relpr)
#   order-seeds: (empty)
#   commit-distance: 1
#   weights: 0.1 0.1 0.1 -> 0 0 1 1.0717734625362931642 1.1486983549970350067 1.2311444133449162843
  N       rt  pl lbt  dis   lbo    lar gcd     satc           t        ppc st      nds      lvs          nsel         nsuel
  5    count dom enu  wdL  rand  relpr   1        6       0.017        166  1        5        1  8.148433e+00  8.426150e+00
   mu0  dmu0  pprunes  pmprune  pprobes  rounds  solc         tr  pelvals     dp  efv
161.75   0.5    2.511   99.751   110.37    1.25     0  0.0055485  0.24876    1.5    0
   145     0        0   99.005   98.684       1     0   0.003437        0      0    0
   180     2   8.7108      100   142.79       2     0   0.007642  0.99502      3    0
15.735     1   4.1807  0.49751   21.621     0.5     0  0.0019999  0.49751  1.291    0
mu0  dmu0  pprunes  pmprune  pprobes  rounds  solc        tr  pelvals  dp  efv
129    60   8.3333   32.787   72.131       1     6  0.002237   38.251   4    0
129    60   8.3333   32.787   72.131       1     6  0.002237   38.251   4    0
129    60   8.3333   32.787   72.131       1     6  0.002237   38.251   4    0
  0     0        0        0        0       0     0         0        0   0    0
      estlvs       uestlvs
6.739697e+01  7.200000e+01
6.739697e+01  7.200000e+01
6.739697e+01  7.200000e+01
0.000000e+00  0.000000e+00
dn       w  ltausp     minp    meanp     maxp       sddp         tb
 0       3  2.0283  0.35132  0.35417  0.35986  0.0040245  0.0030178
 0       2  1.0456     0.25     0.25     0.25          0   0.002933
 0       4  3.4742      0.5      0.5      0.5   0.016098    0.00313
 0  0.8165  1.1092  0.10576  0.10486  0.10397  0.0080489  9.901e-05


2. There is no Euler-square for N=6:

Good weights for wdl:
MOLS> time ./laMols 6 data/SpecsCollection/Euler/red "" count "" "" wdL "" "" "" 1 6 "2.5701,0.0201,0.87,5.19" "" ""
# laMols 0.105.2 caed91f79f53d21da1d724ded7ca4ac70cfeb0d6
  N       rt  pl lbt       dis   lbo    rdl    lar gcd     satc           t        ppc st      nds      lvs          nsel         nsuel
  6    count dom enu       wdL   asc  labsc  relpr   1        0      14.610     380593  0     5213     3072  9.973704e-01  1.731722e+00
   mu0    dmu0  pprunes  pmprune  pprobes   rounds  solc        tr  pelvals      dp  efv
 131.1  30.644   143.14   86.481    74.69   1.6857     0  0.018482   17.017  7.1406    0
    12       0        0    19.92   4.1667        1     0         0        0       0    0
   310     171      900      100   242.86        5     0  0.091526   90.514      11    0
82.725  54.223    174.1   23.867   52.466  0.87367     0  0.017686    30.98   1.641    0
   mu0    dmu0  pprunes  pmprune  pprobes   rounds  solc         tr  pelvals      dp  efv
73.322  25.831   1.3383   8.7889   20.589   1.0527     0  0.0038774   20.779   8.418    0
     8       1        0        0   2.0725        1     0          0   1.9608       5    0
   189     165   32.258   94.444    250.4        4     0    0.07706   94.167      12    0
75.768  38.348   3.6615   14.905     30.6  0.30274     0  0.0096184    18.96  1.4619    0
      estlvs       uestlvs
6.127865e+03  1.228450e+04
8.189471e+02  3.840000e+02
3.809268e+04  1.474560e+05
6.115190e+03  2.486265e+04
    dn        w  ltausp      minp     meanp     maxp      sddp        tb
7.7721   2.4344  3.1433   0.37837   0.44072  0.51722   0.06208  0.013654
     0        2       1  0.053005      0.25     0.25         0         0
    58        4  11.453       0.5       0.5  0.86316   0.36316  0.055639
16.817  0.74884  2.5118   0.16297  0.098232  0.10367  0.094051   0.01352
real	0m2.654s
user	0m14.621s
sys	0m0.446s

Showing only the leaf-count and whether the run was stopped or not (no "info", showing the
parameters, and not showing the weights):
MOLS> time ./laMols 6 data/SpecsCollection/Euler/red "" count "" "" wdL "" "" "" 1 6 "2.5701,0.0201,0.87,5.19" "" "-info,-w,lvs"
3072 0
real	0m2.698s
user	0m14.202s
sys	0m0.425s

0 means no-stop, while 1 would mean stop.
This used 6 threads.

MOLS> time ./laMols 6 data/SpecsCollection/Euler/red "" count "" "" newv "" "" "" "" 6 "0,4.9,-5.26,2.89,0" "" ""
# laMols 0.105.2 caed91f79f53d21da1d724ded7ca4ac70cfeb0d6
  N       rt  pl lbt       dis   lbo    rdl    lar gcd     satc           t        ppc st      nds      lvs          nsel         nsuel
  6    count dom enu      newv   asc  labsc  relpr   1        0      18.290     337822  0     4581     2424  1.041209e+00  1.125054e+00
   mu0    dmu0  pprunes  pmprune  pprobes   rounds  solc        tr  pelvals      dp  efv
162.92  42.448   105.71   82.938    81.41   1.8016     0  0.022124   28.263  6.9764    0
    12       0        0   23.457   6.5217        1     0         0        0       0    0
   310     173      900      100   246.96        4     0   0.11567   90.588      11    0
58.966  61.402   166.45   23.754   50.307  0.76176     0   0.01646   37.672  1.6776    0
   mu0    dmu0  pprunes  pmprune  pprobes   rounds  solc         tr  pelvals      dp  efv
67.718   24.88   6.7254   15.539   30.195   1.1213     0  0.0066076   19.758  8.6188    0
     8       1        0        0   2.7778        1     0          0   2.7778       5    0
   189     178      400   78.968   206.45        3     0   0.068277    94.51      12    0
78.508  37.997   32.183   23.975   38.251  0.36247     0   0.012322   17.136  1.4324    0
      estlvs       uestlvs
5.051897e+03  5.492172e+03
5.769906e+02  5.120000e+02
2.591588e+04  3.276800e+04
5.085978e+03  5.145612e+03
    dn        w  ltausp     minp     meanp      maxp      sddp        tb
14.631   2.1233  10.671   0.4268   0.48289   0.53896  0.055158   0.01709
     0        2       1  0.11284      0.25      0.25         0         0
    59        4  34.585      0.5       0.5   0.80583   0.30583   0.05392
22.232  0.43672  9.7284  0.10553  0.058487  0.082535  0.072658  0.013854
real	0m3.356s
user	0m18.296s
sys	0m0.651s
M

MOLS> time ./laMols 6 data/SpecsCollection/Euler/red "" count "" "" newv "" "" "" 1 6 "0,4.9,-5.26,2.89,0" "" "-info,-w,pelvals"
28.263448798762791129 0
real	0m3.978s
user	0m18.039s
sys	0m0.560s



3. Randomly finding some solutions for N=7:

Sampling 100 leaves, and outputting only the number of solutions found:
MOLS> ./laMols 7 data/SpecsCollection/Euler/red "" count "" "" wdL "rand;0" "" "" 1 1 "0.1" "lvs,99" -info,-w,-stop,satc
1
MOLS> ./laMols 7 data/SpecsCollection/Euler/red "" count "" "" wdL "rand;2" "" "" 1 1 "0.1" "lvs,99" -info,-w,-stop,satc
0

For branch-order "rand" (or "tprob"; see below) only 1 thread can be used (for reproducibility).


4.

Probing one branch:

MOLS> ./laMols 6 data/SpecsCollection/Euler/basis "" count "" "" wdL "rand;0" "" "" 1 1 0.1 lvs,0 uestlvs,-info,-w,-stop
30233088000
MOLS> ./laMols 6 data/SpecsCollection/Euler/basis "" count "" "" wdL "rand;1" "" "" 1 1 0.1 lvs,0 uestlvs,-info,-w,-stop
15116544000

(Note that is is always a natural number.)

MOLS> ./laMols 6 data/SpecsCollection/Euler/basis "" count "" "" wdL "tprob;0" "" "" 1 1 0.1 lvs,0 estlvs,-info,-w,-stop
8072167675.1987981838
MOLS> ./laMols 6 data/SpecsCollection/Euler/basis "" count "" "" wdL "tprob;1" "" "" 1 1 0.1 lvs,0 estlvs,-info,-w,-stop
15510712536.085001176

Showing full information, and using hash-seeds plus a final 777:
MOLS> ./laMols 6 data/SpecsCollection/Euler/basis "" count "" "" wdL "tprob;h,777" "" "" "" 1 0.1 lvs,0 ""
# laMols 0.105.2 caed91f79f53d21da1d724ded7ca4ac70cfeb0d6
# command-line: "./laMols" "6" "data/SpecsCollection/Euler/basis" "" "count" "" "" "wdL" "tprob;h,777" "" "" "" "1" "0.1" "lvs,0" ""
# N: 6
# k=3 total_num_sq=3: "data/SpecsCollection/Euler/basis"
#   num_uc=3 num_eq=0 num_peq=1
#   hash=12022445848885784817
# no_ps
# num_runs=1
# threads=1
# rt=count-solutions(count)
# stopping: by-leaf-count(lvs),0
# output-options: show-info(+info) show-weights(+w) show-headers(+headers) perform_computations(+computations) without-tree-logging(-tree) all no-negation-sign(+sign) show-stopped(+stop) arithmetic-mean(ave) inner-node(inode)
#   propagation-level: domain-prop(dom)
#   la-branching-type: enumerative-branching(enu)
#   distance-type: weighted-delta-literals(wdL)
#   la-order-heuristic: tauprob-first(tprob)
#   la-reduction-level: basic-lookahead(labsc)
#   la-reduction-algorithm: relaxed-pruning(relpr)
#   order-seeds: 8047511768777539459 12022445848885784817 0 14182395947665704810 0 0 0 777

Explanation:
The first three seeds are hashes for:
  - N
  - conditions (see above)
  - partial square (none here)
Then come 4 seeds for the weights:
  - hashing the weight-pattern
  - the seed used for random weights (none here)
  - two seeds for further details.

#   commit-distance: 1
#   weights: 0.1 0.1 0.1 0.1 -> 0 0 1 1.0717734625362931642 1.1486983549970350067 1.2311444133449162843 1.3195079107728942591
  N       rt  pl lbt       dis   lbo    rdl    lar gcd     satc           t        ppc st      nds      lvs          nsel         nsuel
  6    count dom enu       wdL tprob  labsc  relpr   1        0       0.228        832  1       19        1  5.953027e+04  8.693832e+04
   mu0     dmu0  pprunes  pmprune  pprobes  rounds  solc         tr  pelvals      dp  efv
375.94  0.22222   2.4653   99.951   102.61  1.0556     0  0.0067154  0.04852     8.5    0
   243        0        0   99.127    88.82       1     0   0.002914        0       0    0
   540        4   12.587      100   186.68       2     0   0.031728  0.87336      17    0
98.405  0.94281    3.593  0.20585    21.23  0.2357     0   0.006557  0.20585  5.3385    0
mu0  dmu0  pprunes  pmprune  pprobes  rounds  solc        tr  pelvals  dp  efv
232    95        8   53.247   105.52       2     0  0.004186    40.26  18    0
232    95        8   53.247   105.52       2     0  0.004186    40.26  18    0
232    95        8   53.247   105.52       2     0  0.004186    40.26  18    0
  0     0        0        0        0       0     0         0        0   0    0
      estlvs       uestlvs
3.543853e+09  7.558272e+09
3.543853e+09  7.558272e+09
3.543853e+09  7.558272e+09
0.000000e+00  0.000000e+00
dn       w  ltausp      minp    meanp     maxp      sddp         tb
 0  3.7222  2.3444   0.28312  0.29722  0.31652  0.015382  0.0056824
 0       2       1   0.16667  0.16667  0.16667         0   0.003584
 0       6  6.6612       0.5      0.5  0.60955   0.19532   0.013115
 0  1.2274  1.3984  0.098471  0.09756  0.12334  0.046361  0.0023419


Remark: For branching-order "rand" one can consider estlvs: this is just
for one "random leaf" (as given by random branchings) the tau-estimation
of the number of leaves (while using tprob we have that the expected value
of this estimation is exactly the number of leaves).
And for branching-order tprob one can use uestlvs (for comparison).

MOLS> ./laMols 6 data/SpecsCollection/Euler/basis "" count "" "" wdL "rand;0" "" "" 1 1 0.1 lvs,0 estlvs,-info,-w,-stop
6827808605.0310231736
MOLS> ./laMols 6 data/SpecsCollection/Euler/basis "" count "" "" wdL "tprob;0" "" "" 1 1 0.1 lvs,0 uestlvs,-info,-w,-stop
6718464000

Choosing the first two weights randomly (from a restricted pool), and showing the weights:
MOLS> ./laMols 6 data/SpecsCollection/Euler/basis "" count "" "" wdL "rand;0" "" "" 1 1 r,r lvs,0 estlvs,-info,-stop
#   weights: 3.2 1.6 1.6 1.6 -> 0 0 1 9.1895868399762800551 27.857618025475972455 84.448506289465232612 256.00000000000000003
87823146264.256520517

Remark: these weights are proper random weights: the current timestamp
is used as seed for them.
So repeated runs yield different results:
MOLS> ./laMols 6 data/SpecsCollection/Euler/basis "" count "" "" wdL "rand;0" "" "" 1 1 r,r lvs,0 estlvs,-info,-stop
#   weights: 2 0.60000000000000000002 0.60000000000000000002 0.60000000000000000002 -> 0 0 1 4 6.0628662660415923295 9.1895868399762800542 13.928809012737986226
88609148990.883491397
To repeat, use the same weights:
MOLS> ./laMols 6 data/SpecsCollection/Euler/basis "" count "" "" wdL "rand;0" "" "" 1 1 2,0.6 lvs,0 estlvs,-info,-stop
#   weights: 2 0.60000000000000000002 0.60000000000000000002 0.60000000000000000002 -> 0 0 1 4 6.0628662660415923295 9.1895868399762800542 13.928809012737986226
88609148990.883491397

Further remark: the last weight-parameter is "recycled", if further weights are needed.

The parameters w for the weights on the command-line yield factors 2^w
for the previous weight; so "2,0.6" means first a multplication of the
initial weight 1 with 2^2 = 4, and then all other weights are obtained by
successive multiplications with 2^0.6 ~ 1.515717.


*/

/*

BUGS:

*/

/* TODOS:

See Todos in rlaMols, gcMols and LookaheadBranching.

-10. New reduction-types
   - Later for this reduction the count can be corrected, and solutions
     can be made partial (so that all solutions are still enumerated).
     But for now solutions (and their count) maybe lost.
   - "laeaut" includes laefv, and eliminates assignments found which have
     all their variables set having degree 0 (at the end).
   - In the function(s) probe there is another succesful case: when
     laeaut is enabled, then after checking that status one needs to see
     whether all newly-set variables have degree 0 (now, i.e., after
     the assignment), in which case an autarky was found.
   - Concerning forced assignments, we run through all (current) values
     of the current variable v; however if an autarky was found for v,
     then this loop is aborted, and v is set to the autarky-value.
     This is new behaviour (currently also in case of a solution found we
     just continue), where the current loop starting with
     "if (elimvals.empty())" is side-stepped.
   - The two solve-functions are split into two forms each, without and
     with autarky-consideration.
   - The case with pruning handles an autarky the same way as a forced
     assignment (so that it removes previous prunings).
   - We need at least another counter, namely for the number of autarkies.
     Would also be good to have the total number of variables assigned by
     autarkies; but currently we don't have that for forced literals either.

-9. Extended output "lvs"
   - DONE Perhaps "+lvs" ? Better "lvs+". Or "ess" (for "essentials").
   - DONE For scanning, outputting the most important measures:
    - DONE lvs
    - DONE st
    - DONE nsel, nsuel
    - DONE t
    - DONE average rounds&pelvals for inner nodes and leaves :
        aroundsi, aroundsl, apelvalsi, apelvalsl
    - DONE average dp for inner nodes and leaves : adpi, adpl
    - DONE average estlvs&uestlvs : aestlvs, auestlvs
    - DONE standard deviation estlvs&uestlvs : sddestlvs, sdduestlvs
    - DONE average dm0 : adm0
    - DONE average w : aw
    - DONE average ltausp : altausp
    - DONE average minp, meanp, maxp, sdd : aminp, ameanp, amaxp, asddp
    - DONE minimum minp : minminp
    - DONE maximum maxp : maxmaxp
   - DONE We currently have the mode "single-valued" -- is this such a case?
     Should we just add "ess" to Options::SIVA?
     A problem with the formatting-options is that "ess" overwrites all
     the others.
     Perhaps only headers, negation, stop-info, stat-type, node-type are
     ignored.
     +headers is currently ignored anyway for single-values.
     So we have as for single-values, but outputting several values;
     likely using "select" for all the selected values.
   - For batch-mode (weights: cin), the empty input should trigger
     output of the column-names, space-separated.
   - For now just using
     "lvs st nsel nsuel t aroundsi aroundsl apelvalsi apelvalsl adpi adpl
      aestlvs sddestlvs auestlvs sdduestlvs adm0 aw altausp
      aminp ameanp amaxp asddp minminp maxmaxp"
     for BBSample.

-8. In the help-text, break the overlong line for run-type:
   - Perhaps introducing a variation of Environment::WRPO,
     which tries to avoid overlong lines.

-7. Provide hashing for seeds
   - DONE First update documentation on seed-provision.
   - DONE Allowing key-word "h".
   - DONE (in second form)
     Perhaps that belongs to namespace RandGen, which allows for
     seed-construction besides "t, r" also "h", which means a function will
     be called, delivering the "hash-values" (an eseed-vector).
     One could also just always provide that vector (computational costs
     should be negligible).
   - Currently the computation of the seeds occurs according to the order
     on the command-line:
  const list_dis_t disv = read_opt<DIS>(argc, argv, 7, "dis",
                                        "distance");
  const auto [brov, randgen, seeds] = read_lbro(argc, argv, 8);
  const list_lar_t larv = read_opt<LAR>(argc, argv, 9, "lar",
                                        "lookahead-reduction");
     This must now be moved to the first point where all
     the data is available.
   - Seeding (the hash-vector) has 11+4*#weights values:
      - N : as is
      - DONE file_cond : one hash-value for all the conditions (listed together
        with summary of conditions)
      - DONE file_ps : also one hash-value
      - DONE (not included) run_type
      - DONE (not included) list of prop-levels
      - DONE (not included) list of branch-type
      - DONE (not included) list of distance
      - DONE (not included) list of branch-order
      - DONE (not included) list of la-type
      - DONE (not included) list of gcd
      - DONE (not included) list of stop-types
      - DONE
        the WGenerator-object yields 4 seeds:
        Just hashing the data-members into seeds:
         - DONE
           pattern_t: list of float80 and OP::EXW, so just translating
           into a list of float80 ? Using +-inf and NaN for encoding.
         - DONE seed : as is
         - DONE leveluse, randomuse, into one
         - DONE sp put into the final seed.
        Remark: from this *one* generator all weight-vectors are obtained.

-6. Provide higher-precision computation of quantities related to tau
   - First step is to provide a special class TauV for the tau-values and
     derived quantities.
   - At the beginning TauV just wrappes FP::float80, and the first task is
     to reproduce everything old in this new framework.
   - From the tau-values for a branch we get the tau-probabilities,
     which are used outside of branching, in the statistics.
   - Namely in the quantity estlvs for a leaf, and the corresponding
     4 statistics, and for nsel.
   - So TauV wraps the value and the operations.
   - The statistics in Numerics/Statistics.hpp need to be able to handle
     TauV.
   - Easiest to take the statistics for TauV as a further, dedicated member
     of the statistics-classes (where currently it is part of an array).
   - How to parameterise TauV? As a concept, with the float80-TauV and
     arbitrary-precision-TauV (as in Numerics/Tau.hpp) as instances.
   - If FP::float80 would also fit this concept, then the transition would
     be seamless; of perhaps just a simple wrapper around float80.
   - Would then also nice to support the Boost higher precision types.
   - In C++23 we have <stdfloat>, with std::float128_t (which of course
     should also be usable).

-4. Output of tree-logging (once this has been implemented for laMols,
    we have version 1.0 of laMols).
   - DONE Fixed file for output; name as for output of solutions, but
     with "TREE" instead of "SOLUTIONS".
   - DONE Also for rlaMols.
   - DONE Should (r)laParams know about this, by annother boolean "tree_logging"?
     Perhaps that's best here, since there are some difficulties associated
     with tree-logging.
   - The data provided is:
      - DONE the node-data: id, pid, branch, and depth (the last item is
        redundant, but should help to check for correctness of the
        interpretation);
        DONE (known now)
        the width of the branching is not available at this point, which
        is a problem for rlaMols, so perhaps, in RlaBranching::commit,
        the computation of v is most foreward for non-leaves?

        Perhaps rlaMols should have branching data (as laMols has)?
        Likely best leaving for a later revision.
      - DONE branching-data for inner nodes: variable and values
      - DONE ReductionStatistics
      - MeasureStatistics, BranchingStatistics (these two only for laMols)
      - also only for laMols (for inner nodes): tau-value, and for the
        branches distances and probabilities.
   - New formatting options "-tree", "+tree".
   - DONE (without changing the existing system)
     The add-member in rlaStats gets a new (reference-)parameter, the
     NodeData (in GenericMols1): from this the depth is read, and the id is
     set to the current (total) node-count (starting with 1).
     (So ReductionStatistics doesn't need the depth-variable anymore.)
   - In this function also the basic tree-data (node-data and reduction-
     statistics) are output (if activated, that is, iff the pointer
     is not null).
   - DONE (not via add, but using dedicated functions)
     Both overloads of "add" in laStats use the pointer from the rlaStats-
     object and output further statistics (measures and branching-statistics).
   - Possibly for the output of tau-value and branching variable,
     and for the distances, probabilities and values of the branches, there
     is an additional function.
   - DONE Actually branching-variable and branching-values are also needed for
     rlaMols (while tau and probs are only for laMols).
     So this output-function belongs to rlaStats.
   - For binary branching one needs to indicate whether the value is set or
     unset; possibly by using "+" and "-" (where one needs to distinguish
     between +0 and -0).
   - DONE Another member needs to be added to NodeData, identifying the
     branch; this is set in update_clone, with new signature
       update_clone(unsigned a).
   - Perhaps it should be an option to call flush after every addition
     of data to the file?

-3. Reject non-positive weights for wdL
   - This leads to infinite loops.
   - Perhaps one should also provide a "strict" input-format, where the
     exact number of weights must be provided (no extension or cutting),
     in order to avoid input-errors.

-1. Controlling the output:
   - "+-cond": output of full conditions (+cond -> +info).
     normal and batchmode: off.
   - "+-commentout": also statistics-output with leading "#"
     normal: on, batchmode: off.
   - "+-hex": in case of solution-output, use hexadecimal notation.
   - The above also for rlaMols and gcMols.
   - Perhaps for the weights-info also N could be output, together with
     the number of essential weights?
     Like "N=8,numw=6". One could also state the number of given weights.
   - These are additional options for the formatting-options.

0. Provide global statistics "open total assignments"
   - Let muld be the measure log_2(# total assignments), where the
     number of total assignments is N^(N^2 * K), where
       K = cond.num_squares() (so n = N^2 * K).
   - This is not most efficient for solving, but it is very natural;
     so perhaps it should always be provided:
     - DONE
       Likely "vals" for la-reduction should be the measure mu0 (so
       that it is zero when the problem has been solved); currently it is
       "sumdomsizes".
     - "mu" for wdL is naturally as given by the weights (generalising
       mu0).
     - For newvars possibly mu then is muld; but we always want it?
   - We want to have as global variable the number of open total
     assignments -- this can be used as a good achievement-measure
     also for unsatisfiable instances.
   - This global variable ota can be rather easily computed by setting
     the variable initially to exp2(muld), and then after every
     la-reduction, for the original muld0 before the reduction and muld
     after the reduction:
         ota := ota - exp2(muld0) + exp2(muld).
     - And after the branching has been computed, the current ota is updated
       in the same way, for each child.
     - Since this update is needed for each branch (so that the update can
       "pick up the old value" exp2(muld0)), we only compute ota for
       laMols.
     - Then we don't need the update in the middle, but can use in the
       above ota-formula "exp2(muld)" for each child, except for the case
       that we found a leaf, where then "exp2(muld) = 0", that is, only
       the subtraction takes place.
   - A problem here is that pseudo-leaves should also compute their branchings,
     so that ota is accurate for them.
     So perhaps pseudoleaves should have their data submitted to the global
     statistics not together with the leaves, but later, together with
     the inner nodes.
     This seems also appropriate for the other branching-measures, to obtain
     them for the pseudoleaves (perhaps best separately).
   - All distances compute pairs (distance, Delta muld).
     After the reduction, exp2(muld) is subtracted, and after the
     branching has been computed, for each branch exp2(newmuld)
     is added.
   - The node itself maintains muld (like depth): the commit updates
     this value (subtracting Delta muld).
   - Perhaps what is reported is log2(ota) (the smaller this number,
     the more work has been done).
     One likely needs to output this number with full precision.
   - Perhaps more informative is the ratio
       # total-assignments-handled / # total assignments
       = (exp2(muld_root) - ota) / exp2(muld_root)
       = 1 - ota / exp2(muld_root).
   - float80 should be sufficient for a good deal of problems.

   - CHANGE: it seems more informative to have each node reporting
     on the change in ota; see "Update member vals_ of BranchingStatistics"
     in LookaheadBranching.hpp.
     Additionally it should also be interesting, now as a global variable,
     to see the total current change achieved (the above "ota").
     This is just one global variable, maintained by the branching statistics.

1. Early abortion of runs
    - Allowing bounds for total (wallclock) runtime; which should perhaps
      anyway be part of the general statistics (not separately handled
      by the solver).

2. Early abortion of branches
   - Considering now say "depth >= 10" or "mu <= 10000" for making a node,
     after la-reduction, a leaf.
   - Usual syntax: "depth, 10" and "mu, 10000".
   - The current handling of "leafcount" needs to be generalised.
   - Then we need to count three types of leaves: falsified, satisfied,
     and "completed-early" (perhaps "closed"?).
   - One could also include criteria related to the branching, e.g.,
     ltau too bad.
   - So we handle this after the branching-computation, just expanding
     S->add(stats0, stats1) by adding the information on whether we
     have a "pseudo-leaf" (no branching), and in such a case adding
     the reduction-statistics to the third type of reduction-statistics.
     We have an early check-point for pseudo-leaves, which can then
     expand the operation S->add(stats0).
   - Instead of just having "ClosingData" (similar to "StoppingData"),
     now this object also handles the decision whether the branch
     is to be closed.

3. Better values for qfppc
   - DONE
     Currently it is meant to be the quotient of eliminated values
     per reduced la-variable (not considering inferred variables);
     that is elimvals_ / props_/
   - DONE
     The problem is that there are (many) cases with qfppc=0,
     which means that zero variables have been eliminated (props_ = 0).
   - DONE In that case, perhaps qfppc=1 is more appropriate?
   - DONE
     But elimvals_ is already reported via "pelvals", as
     100 * float_t(elimvals_) / vals_.
     That seems reasonable.
   - DONE
     While props_ is not reported otherwise.
     And neither is vars_ reported.
     So one could report 100 * float_t(props_) / vars_ as qfppc.
   - An analogy to "pelvals", perhaps renaming "qfppc" to "prvars",
     and putting prvars directly before pelvals ?

4. DONE Correct vals_:
   - As a basis for elimination-statistics, this should be the real
     number of values (sum of domain-sizes of variables not set).
   - That is, equal to sumdomsizes() - sizes() + vars() = mu0 + vars_.
   - While mu0 should be another member, "mu0_" and "mu0()".
   - Perhaps renaming "vals()" to "numvals()", and "vals_" to "numvals_" ?
   - Same with "vars()" and "vars_" ?

*/


#include <iostream>
#include <string>
#include <ostream>
#include <fstream>
#include <string_view>
#include <array>

#include <ProgramOptions/Environment.hpp>
#include <Numerics/NumInOut.hpp>
#include <Numerics/NumBasicFunctions.hpp>

#include "Conditions.hpp"
#include "Encoding.hpp"
#include "PartialSquares.hpp"
#include "Solvers.hpp"
#include "Options.hpp"
#include "CommandLine.hpp"
#include "LookaheadReduction.hpp"
#include "LookaheadBranching.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        CommandLine::version_laMols,
        CommandLine::date_laMols,
        __FILE__,
        "Oliver Kullmann and Oleg Zaikin",
        "https://github.com/OKullmann/oklibrary/blob/master/Satisfiability/Solvers/Gecode/MOLS/laMols.cpp",
        "GPL v3"};

  const std::string error = "ERROR[" + proginfo.prg + "]: ";

  using namespace Conditions;
  using namespace Encoding;
  using namespace PartialSquares;
  using namespace Solvers;
  using namespace Options;
  using namespace CommandLine;
  using namespace LookaheadReduction;
  using namespace LookaheadBranching;

  constexpr int commandline_args = commandline_args_laMols;

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
    "> " << proginfo.prg <<
      " has " << commandline_args << " command-line arguments:\n"
      " N  file_cond  file_ps  run-type\n"
      "   prop-level  branch-type  distance  branch-order\n"
      "   la-reduction  la-algorithm  gcd\n"
      "   threads  weights  (stop-type,stop-value)*  formatting\n\n"
      " - N            : \";\"-separated list of \"a[,b][,c]\"-sequences\n"
      " - file_cond    : filename/string for conditions-specification\n"
      " - file_ps      : filename/string for partial-squares-specification\n"
      " - run-type     : " << Environment::WRPO<RT>{} << "\n" <<
      " - prop-level   : " << Environment::WRPO<PropO>{} << "\n" <<
      " - branch-type  : " << Environment::WRPO<LBRT>{} << "\n" <<
      " - distance     : " << Environment::WRPO<DIS>{} << "\n" <<
      " - branch-order : " << Environment::WRPO<LBRO>{} << "\n" <<
      " - la-reduction : " << Environment::WRPO<RDL>{} << "\n" <<
      " - la-algorithm : " << Environment::WRPO<LAR>{} << "\n" <<
      " - gcd          : Gecode commit-distance; list as for N\n"
      " - threads      : floating-point for number of threads\n"
      " - weights      : either comma-separated list of weights for distance,"
      " possibly using\n"
      "   - variables  : " << Environment::WRPO<EXW>{} << "; or one of\n" <<
      "   - specials   : " << Environment::WRPO<SPW>{} << "\n" <<
      " - stop-type    : " << Environment::WRPO<LRST>{} << "\n" <<
      " - formatting   : comma-separated list of\n" <<
      "   - info       : " << Environment::WRPO<Info>{} << "\n" <<
      "   - weights    : " << Environment::WRPO<Weights>{} << "\n" <<
      "   - headers    : " << Environment::WRPO<Headers>{} << "\n" <<
      "   - compute    : " << Environment::WRPO<Computations>{} << "\n" <<
      "   - values     : " << Environment::WRPO<SIVA>{} << "\n" <<
      "   - negation   : " << Environment::WRPO<NEG>{} << "\n" <<
      "   - stop-info  : " << Environment::WRPO<STOP>{} << "\n" <<
      "   - stat-type  : " << Environment::WRPO<STAT>{} << "\n" <<
      "   - node-type  : " << Environment::WRPO<NOTY>{} << "\n\n" <<
      "Here\n"
      "  - to use a string instead of a filename, a leading \"@\" is needed\n"
      "  - file_ps can be the empty string (no partial instantiation)\n"
      "  - the seven algorithmic options (propagation, branch-type,\n"
      "      distance, branch-order, la-type, gcd, weights) can be lists\n"
      "      (all combinations)\n"
      "    - these lists can have a leading + (inclusion) or - (exclusion)\n"
      "  - for branch-orders \"rand,tprob\" a comma-separated seed-sequence"
      " can be given after \";\"\n"
      "    - this sequence can include \"t\" (timestamp) and \"r\" (random)\n"
      "  - weights are patterns, with the last entry used for filling (thus"
      " the tail is always constant)\n"
      "    - the default for weights (empty string) is \"all specials\"\n"
      "    - for input \"cin\", the weights are read from standard-input,"
      " space-separated,\n"
      "        and batch-mode is used (no additionaly info-output)\n"
      "  - stop-values are unsigned int\n"
      "    - different pairs of stop-types/values are separated by \"|\"\n"
      "  - formatting uses the given defaults for fields not specified\n"
      "    - these are flipped in batch-mode for the first three fields\n"
      "    - the final three fields are only relevant for single-values\n"
      "  - for sat-solving and enumeration, output goes to file \"" <<
      "SOLUTIONS_" << proginfo.prg << "_N_timestamp\".\n\n"
;
    return true;
  }


  OutputOptions read_output_options(const std::string& s) {
    return Environment::translate<output_options_t>()(s,',');
  }
  void output_options(std::ostream& out, const OutputOptions& outopt) {
    out << "# output-options: ";
    Environment::print1d(out, outopt.options, {0});
    out << "\n";
  }

  constexpr size_t wnsel = 13;
  void rh(std::ostream& out) {
    out.width(wN); out << "N" << " ";
    Environment::header_policies<RT, PropO, LBRT, DIS, LBRO, RDL, LAR>(out);
    out.width(wgcd); out << "gcd" << " ";
    out << std::string(sep_spaces, ' ');
    rh_genstats(out);
    out << " "; out.width(wnsel); out << "nsel";
    out << " "; out.width(wnsel); out << "nsuel";
    /* to be actived once pseudo-leaves are possible:
    out << " "; out.width(wnds); out << "plvs";
    */
    out << "\n";
  }

  // Let the probability of leaf v be p(v) (either uniform or tau).
  // Then the variance is sigma = (sum_v p(v) * prediction(v)^2) - lvs^2,
  // where p(v) * prediction(v)^2 = p(v) * (p(v)^-2) = prediction(v).
  std::array<LookaheadBranching::float_t, 2>
  tauprob_variances(const laSR& res) noexcept {
    using LookaheadBranching::float_t;
    const float_t lvs = res.S[1].N();
    const float_t variance = res.mS.sum()[0] - lvs*lvs,
      uvariance = res.mS.sum()[1] - lvs*lvs;
    const float_t normalised_stddev = FloatingPoint::sqrt(variance) / lvs,
      normalised_ustddev = FloatingPoint::sqrt(uvariance) / lvs;
    return {normalised_stddev, normalised_ustddev};
  }

  void rs(std::ostream& out, const laSR& res, const bool with_headers) {
    const auto state = FloatingPoint::fixed_width(out, precision);
    out << std::string(sep_spaces, ' ');
    rs_genstats(out, res);
    {const auto state =
       FloatingPoint::engineering_width(out, precision_engineering);
     const auto [normalised_stddev, normalised_ustddev] =
       tauprob_variances(res);
     out << " "; out.width(wnsel); out << normalised_stddev;
     out << " "; out.width(wnsel); out << normalised_ustddev;
     FloatingPoint::undo(out, state);
    }
    /* to be actived once pseudo-leaves are possible:
    {const auto plvs = res.mS.N() - res.S[1].N();
     out << " "; out.width(wnds); out << plvs;}
    */
    out << "\n";

    {const auto state = FloatingPoint::default_width(out, precision+2);
     res.outS(out, with_headers);
     FloatingPoint::undo(out, state);
    }
    res.outmS(out, with_headers);
    {const auto state = FloatingPoint::default_width(out, precision+2);
     res.outbS(out, with_headers);
     FloatingPoint::undo(out, state);
    }
    FloatingPoint::undo(out, state);
  }
  void select(std::ostream& out, const laSR& res,
              const SIVA sv, const STAT st, const NOTY nt, const bool neg) {
    assert(sv != SIVA::all and sv != SIVA::ess);

    const auto val = [&res,st,nt](const std::string& s){
      const auto i = ReductionStatistics::index(s);
      const size_t nti = size_t(nt);
      assert(nti < res.S.size());
      const auto& R = res.S[nti];
      switch (st) {
      case STAT::ave : return R.amean()[i];
      case STAT::min : return R.min()[i];
      case STAT::max : return R.max()[i];
      case STAT::stddev : return R.sd_corrected()[i];
      default :std::ostringstream ss;
        ss << "ERROR[laMols::single::val]: STAT-value " << int(st) <<
          " not handled.\n";
        throw std::runtime_error(ss.str());} };
    const auto valm = [&res,st](const std::string& s){
      const auto i = MeasureStatistics::index(s);
      const auto& R = res.mS;
      switch (st) {
      case STAT::ave : return R.amean()[i];
      case STAT::min : return R.min()[i];
      case STAT::max : return R.max()[i];
      case STAT::stddev : return R.sd_corrected()[i];
      default :std::ostringstream ss;
        ss << "ERROR[laMols::single::valm]: STAT-value " << int(st) <<
          " not handled.\n";
        throw std::runtime_error(ss.str());} };
    const auto valb = [&res,st](const std::string& s){
      const auto i = BranchingStatistics::index(s);
      const auto& R = res.bS;
      switch (st) {
      case STAT::ave : return R.amean()[i];
      case STAT::min : return R.min()[i];
      case STAT::max : return R.max()[i];
      case STAT::stddev : return R.sd_corrected()[i];
      default :std::ostringstream ss;
        ss << "ERROR[laMols::single::valb]: STAT-value " << int(st) <<
          " not handled.\n";
        throw std::runtime_error(ss.str());} };

    if (neg) out << "-";
    switch (sv) {
    case SIVA::satc : out << res.b.sol_found; return;
    case SIVA::t : out << res.ut; return;
    case SIVA::ppc : out << res.gs.propagate; return;
    case SIVA::nds : out << res.S[0].N()+res.S[1].N(); return;
    case SIVA::inds : out << res.S[0].N(); return;
    case SIVA::lvs : out << res.S[1].N(); return;

    case SIVA::mu0 : out << val("mu0"); return;
    case SIVA::dmu0 : out << val("dmu0"); return;
    case SIVA::pprunes : out << val("pprunes"); return;
    case SIVA::pmprune : out << val("pmprune"); return;
    case SIVA::pprobes : out << val("pprobes"); return;
    case SIVA::rounds : out << val("rounds"); return;
    case SIVA::solc : out << val("solc"); return;
    case SIVA::tr : out << val("tr"); return;
    case SIVA::pelvals : out << val("pelvals"); return;
    case SIVA::dp : out << val("dp"); return;
    case SIVA::efv : out << val("efv"); return;

    case SIVA::dn : out << valb("dn"); return;
    case SIVA::w : out << valb("w"); return;
    case SIVA::ltausp : out << valb("ltausp"); return;
    case SIVA::minp : out << valb("minp"); return;
    case SIVA::meanp : out << valb("meanp"); return;
    case SIVA::maxp : out << valb("maxp"); return;
    case SIVA::sddp : out << valb("sddp"); return;
    case SIVA::tb : out << valb("tb"); return;

    case SIVA::estlvs : out << valm("estlvs"); return;
    case SIVA::uestlvs : out << valm("uestlvs"); return;

    default: {
      std::ostringstream ss;
      ss << "ERROR[laMols::single]: SIVA-value " << int(sv) <<
        " not handled.\n";
      throw std::runtime_error(ss.str());} }
  }

  size_t mult(const size_t basis, const list_size_t& list_N,
              const list_lbrt_t& brtv, const list_dis_t& disv,
              const WGenerator& wg) noexcept {
    size_t sum = 0;
    for (const size_t N : list_N)
      for (const LBRT brt : brtv)
        for (const DIS dis : disv)
          sum += wg.size(N, brt, dis);
    return basis * sum;
  }

}


int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  if (argc !=  commandline_args + 1) {
    std::cerr << error << "Exactly " << commandline_args << " command-line"
      " arguments needed, but the real number is " << argc-1 << ":\n";
    commandline_output(std::cerr, argc, argv);
    return 1;
  }

  /* Reading the command-line parameters: */

  const auto list_N = read_N(argc, argv);
  const size_t N_hash = FloatingPoint::hash_UInt_range()(list_N);
  const auto [ac, name_ac] = read_ac(argc, argv);
  const size_t ac_hash = ac.hash();
  const auto [ps0, name_ps] = read_ps(argc, argv, list_N);
  const size_t ps_hash = ps0 ? ps0.value().hash() : 0;

  const RT rt = read_rt(argc, argv);
  const list_propo_t pov = read_opt<PropO>(argc, argv, 5, "po",
                                           "propagation");
  const list_lbrt_t brtv = read_opt<LBRT>(argc, argv, 6, "brt",
                                          "branching-type");
  const list_dis_t disv = read_opt<DIS>(argc, argv, 7, "dis",
                                        "distance");
  const list_rdl_t rdlv = read_opt<RDL>(argc, argv, 9, "rdl",
                                        "lookahead-reduction");
  const list_lar_t larv = read_opt<LAR>(argc, argv, 10, "lar",
                                        "lookahead-algorithm");
  const list_unsigned_t gcdv = read_comdist(argc, argv, 11);

  const auto [wg, batch_mode] = read_weights(argc, argv, 13);
  const std::vector<size_t> wg_hash = wg.hash();
  const std::vector<size_t> hash_seeds = [&N_hash,&ac_hash,&ps_hash,&wg_hash]{
    std::vector<size_t> res{N_hash, ac_hash, ps_hash};
    for (const auto& x : wg_hash) res.push_back(x);
    return res;}();
  const auto [brov, randgen, seeds] = read_lbro(argc, argv, 8, hash_seeds);

  const double threads = read_threads(argc, argv, 12);
  if (threads != 1 and randgen) {
    std::cerr << error << "In the presence of branching-order rand the"
      " number of threads must be 1, but is " << threads << ".\n";
    return 1;
  }

  const size_t num_runs =
    mult(pov.size() * brov.size() * rdlv.size() * larv.size() * gcdv.size(),
         list_N, brtv, disv, wg);
  if (num_runs != 1 and batch_mode) {
    std::cerr << error << "In batch-mode the number of runs must be 1, but is "
              << num_runs << ".\n";
    return 1;
  }

  const auto stod = read_rlast(argc, argv, 14);

  OutputOptions::set_def(batch_mode);
  const auto outopt = read_output_options(argv[15]);

  /* Reading of command-line parameters completed. */


  const std::string outfile = output_filename(proginfo.prg, list_N);

  const bool with_file_output = Options::with_file_output(rt);
  if (with_file_output and batch_mode) {
    std::cerr << error << "In batch-mode there can not be file-output.\n";
    return 1;
  }
  if (with_file_output and num_runs != 1) {
    std::cerr << error << "For solution-output the number of runs must be 1,"
      " but is " << num_runs << ".\n";
    return 1;
  }
  std::ostream* const out = with_file_output ?
    new std::ofstream(outfile) : nullptr;
  if (with_file_output and (not out or not *out)) {
    std::cerr << error << "Can not open file \"" << outfile << "\" for "
      "writing.\n";
    return 1;
  }
  const bool with_log = Options::with_log(rt);
  if (with_log and batch_mode) {
    std::cerr << error << "In batch-mode there can not be log-output.\n";
    return 1;
  }
  std::ostream* const log = with_log ? &std::cout : nullptr;

  if (outopt.with_info()) {
    std::cout << "# " << proginfo.prg << " " << proginfo.vrs << " "
              << proginfo.git << "\n";
    commandline_output(std::cout, argc, argv);
    info_output(std::cout,
                list_N, ac, name_ac, ps0, name_ps, rt,
                num_runs, threads, outfile, with_file_output,
                hash_seeds);
    st_output(std::cout, stod);
    output_options(std::cout, outopt);
    algo_output(std::cout, std::make_tuple(pov, brtv, disv, brov, rdlv, larv));
    if (randgen) oseed_output(std::cout, seeds);
    cd_output(std::cout, gcdv);
    wseed_output(std::cout, wg);
    std::cout.flush();
  }

  for (const size_t N : list_N)
    for (const PropO po : pov) {
      const EncCond enc(ac, ps0 ? ps0.value() : PSquares(N,psquares_t{}),
                        prop_level(po));
      for (const LBRT brt : brtv)
        for (const DIS dis : disv) {
          const auto wv = wg(N, brt, dis);
          for (const auto& weights0 : wv) {
            if (outopt.with_weights()) weights_output(std::cout, weights0);
            const weights_t* const weights = &weights0.w;
            for (const LBRO bro : brov)
              for (const RDL rdl : rdlv)
              for (const LAR lar : larv)
                for (unsigned gcd : gcdv) {
                  const laSR res = outopt.with_computations() ?
                    lasolver(enc, rt, brt, dis, bro, rdl, lar,
                             gcd, threads, weights,
                             needs_randgen(bro) ? randgen.get() : nullptr,
                             stod, log, nullptr) :
                    laSR{};
                  if (with_log and
                      rt != RT::enumerate_with_log and
                      rt != RT::unique_s_with_log)
                    std::cout << "\n";
                  if (outopt.single_valued()) {
                    const auto old =
                      FloatingPoint::fullprec_float80(std::cout);
                    select(std::cout, res, outopt.values(),
                           outopt.stat(),outopt.node_type(),outopt.negated());
                    if (outopt.with_stop())
                      std::cout << " " << res.stopped;
                    std::cout << std::endl;
                    std::cout.precision(old);
                  }
                  else if (outopt.essentials()) {
                    const auto old =
                      FloatingPoint::fullprec_float80(std::cout);

                    select(std::cout,res, SIVA::lvs, {},{},{});
                    std::cout << " " << res.stopped << " ";
                    Environment::out_line(std::cout, tauprob_variances(res));
                    std::cout << " ";
                    select(std::cout,res, SIVA::t, {},{},{});
                    std::cout << " ";
                    select(std::cout,res, SIVA::rounds,{},NOTY::inode, {});
                    std::cout << " ";
                    select(std::cout,res, SIVA::rounds,{},NOTY::leaf, {});
                    std::cout << " ";
                    select(std::cout,res, SIVA::pelvals,{},NOTY::inode, {});
                    std::cout << " ";
                    select(std::cout,res, SIVA::pelvals,{},NOTY::leaf, {});
                    std::cout << " ";
                    select(std::cout,res, SIVA::dp,{},NOTY::inode, {});
                    std::cout << " ";
                    select(std::cout,res, SIVA::dp,{},NOTY::leaf, {});
                    std::cout << " ";
                    select(std::cout,res, SIVA::estlvs, {},{},{});
                    std::cout << " ";
                    select(std::cout,res, SIVA::estlvs,STAT::stddev, {},{});
                    std::cout << " ";
                    select(std::cout,res, SIVA::uestlvs, {},{}, {});
                    std::cout << " ";
                    select(std::cout,res, SIVA::uestlvs,STAT::stddev, {},{});
                    std::cout << " ";
                    select(std::cout,res, SIVA::mu0, {},{},{});
                    std::cout << " ";
                    select(std::cout,res, SIVA::w, {},{},{});
                    std::cout << " ";
                    select(std::cout,res, SIVA::ltausp, {},{},{});
                    std::cout << " ";
                    select(std::cout,res, SIVA::minp, {},{},{});
                    std::cout << " ";
                    select(std::cout,res, SIVA::meanp, {},{},{});
                    std::cout << " ";
                    select(std::cout,res, SIVA::maxp, {},{},{});
                    std::cout << " ";
                    select(std::cout,res, SIVA::sddp, {},{},{});
                    std::cout << " ";
                    select(std::cout,res, SIVA::minp,STAT::min, {},{});
                    std::cout << " ";
                    select(std::cout,res, SIVA::maxp,STAT::max, {},{});

                    std::cout << std::endl;
                    std::cout.precision(old);
                  }
                  else {
                    if (outopt.with_headers()) rh(std::cout);
                    std::cout.width(wN); std::cout << N << " ";
                    Environment::data_policies(std::cout,
                      std::make_tuple(rt, po, brt, dis, bro, rdl, lar));
                    std::cout.width(wgcd); std::cout << gcd << " ";
                    rs(std::cout, res, outopt.with_headers());
                    if (with_file_output)
                      Environment::out_line(*out, res.b.list_sol, "\n");
                    std::cout.flush();
                  }
                }
          }
        }
    }
  delete out;
}
