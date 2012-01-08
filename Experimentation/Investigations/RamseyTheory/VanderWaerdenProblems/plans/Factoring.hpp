// Oliver Kullmann, 4.11.2011 (Swansea)
/* Copyright 2011, 2012 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/Factoring.hpp
  \brief Lower bounds for van-der-Waerden problems via simplifying homomorphisms

  Generalising the approaches from [Rabung 1979] and Heule et al, aiming at
  finding an m-colouring of a hypergraph G as a composition of hypergraph-
  morphisms, where the final one has as target a hypergraph with at most m
  vertices and without 1-hyperedges (this yields an m-colouring of G).


  \todo vdw_2(6,6)=1132
  <ul>
   <li> First considering random creation of equivalence classes of size 5
   (resp. 2 for the last one):
   \verbatim
G66:arithprog_hg(6,1132)$
ncl_list_fcs(G66);
  [[6,127577]]
set_random(1);
G66p5 : random_projection_hg(G66,5)$
ncl_list_fcs(G66p5);
  [[4,91],[5,6560],[6,120533]]
G66p5m : min_hg(G66p5)$
ncl_list_fcs(G66p5m);
  [[4,91],[5,6460],[6,116949]]

F66p5 : tcol2sat_stdhg2stdfcs(G66p5)$
outputext_fcs("set_random(1),random_projection_hg(arithprog_hg(6,1132),5)",F66p5,"VanDerWaerden_p5_1132.cnf");
F66p5m : tcol2sat_stdhg2stdfcs(G66p5m)$
outputext_fcs("set_random(1),random_projection_min_hg(arithprog_hg(6,1132),5)",F66p5m,"VanDerWaerden_p5m_1132.cnf");

for s : 1 thru 20 do block([G], set_random(s), G : random_projection_hg(G66,5), print(s, ncl_list_fcs(G), ncl_list_fcs(min_hg(G))));
1 [[4,91],[5,6560],[6,120533]] [[4,91],[5,6460],[6,116949]]
2 [[4,100],[5,6300],[6,120777]] [[4,100],[5,6201],[6,117309]]
3 [[4,79],[5,6643],[6,120460]] [[4,79],[5,6558],[6,116795]]
4 [[4,92],[5,6613],[6,120476]] [[4,92],[5,6510],[6,116888]]
5 [[4,93],[5,6446],[6,120631]] [[4,93],[5,6343],[6,117128]]
6 [[4,106],[5,6442],[6,120602]] [[4,106],[5,6339],[6,117098]]
7 [[4,87],[5,6492],[6,120579]] [[4,87],[5,6403],[6,116973]]
8 [[4,78],[5,6391],[6,120695]] [[4,78],[5,6313],[6,117243]]
9 [[3,1],[4,120],[5,6420],[6,120626]] [[3,1],[4,118],[5,6312],[6,117028]]
10 [[4,89],[5,6351],[6,120755]] [[4,89],[5,6256],[6,117225]]
11 [[4,91],[5,6426],[6,120639]] [[4,91],[5,6343],[6,117098]]
12 [[4,97],[5,6390],[6,120689]] [[4,97],[5,6293],[6,117154]]
13 [[3,1],[4,53],[5,6554],[6,120566]] [[3,1],[4,52],[5,6498],[6,116962]]
14 [[3,1],[4,108],[5,6425],[6,120641]] [[3,1],[4,108],[5,6329],[6,117084]]
15 [[4,94],[5,6550],[6,120533]] [[4,94],[5,6463],[6,116918]]
16 [[4,100],[5,6290],[6,120795]] [[4,100],[5,6176],[6,117412]]
17 [[3,2],[4,115],[5,6539],[6,120512]] [[3,2],[4,111],[5,6425],[6,116965]]
18 [[4,102],[5,6445],[6,120622]] [[4,102],[5,6339],[6,117084]]
19 [[3,1],[4,72],[5,6506],[6,120645]] [[3,1],[4,70],[5,6413],[6,117062]]
20 [[3,2],[4,99],[5,6798],[6,120279]] [[3,2],[4,97],[5,6684],[6,116558]]

> E=run_ubcsat("VanDerWaerden_p5m_1132.cnf",runs=100,cutoff=1000000)
# computation not successfully completed, however it looks hopeless
# all minimums around 2700 - 3000

   \endverbatim
   </li>
   <li> The longer the clauses the better, one would guess? </li>
   <li> Partitioning into classes with 10 elements:
   \verbatim
for s : 1 thru 20 do block([G], set_random(s), G : random_projection_hg(G66,10), print(s, ncl_list_fcs(G), ncl_list_fcs(min_hg(G))));
1 [[3,8],[4,518],[5,13881],[6,112214]] [[3,8],[4,507],[5,13348],[6,104508]]
2 [[3,5],[4,499],[5,13600],[6,112516]] [[3,5],[4,494],[5,13101],[6,104915]]
3 [[3,7],[4,490],[5,13672],[6,112459]] [[3,7],[4,477],[5,13174],[6,104894]]
4 [[3,4],[4,496],[5,13718],[6,112420]] [[3,4],[4,491],[5,13231],[6,104821]]
5 [[3,5],[4,506],[5,13728],[6,112338]] [[3,5],[4,498],[5,13209],[6,104811]]
6 [[3,14],[4,449],[5,13973],[6,112129]] [[3,14],[4,437],[5,13506],[6,104378]]
7 [[3,3],[4,464],[5,14038],[6,112058]] [[3,3],[4,458],[5,13548],[6,104298]]
8 [[3,7],[4,490],[5,13760],[6,112343]] [[3,7],[4,476],[5,13265],[6,104756]]
9 [[3,6],[4,482],[5,14035],[6,112039]] [[3,6],[4,472],[5,13545],[6,104238]]
10 [[3,9],[4,469],[5,13909],[6,112224]] [[3,9],[4,458],[5,13412],[6,104397]]
11 [[3,3],[4,462],[5,14062],[6,112054]] [[3,3],[4,458],[5,13587],[6,104293]]
12 [[3,1],[4,509],[5,13752],[6,112325]] [[3,1],[4,507],[5,13225],[6,104758]]
13 [[3,8],[4,458],[5,13703],[6,112437]] [[3,8],[4,450],[5,13243],[6,104868]]
14 [[3,4],[4,466],[5,13640],[6,112517]] [[3,4],[4,464],[5,13184],[6,104945]]
15 [[3,6],[4,472],[5,14370],[6,111735]] [[3,6],[4,465],[5,13888],[6,103884]]
16 [[3,2],[4,498],[5,13477],[6,112666]] [[3,2],[4,494],[5,12951],[6,105320]]
17 [[3,8],[4,542],[5,13892],[6,112129]] [[3,8],[4,532],[5,13367],[6,104395]]
18 [[3,2],[4,431],[5,13898],[6,112256]] [[3,2],[4,428],[5,13439],[6,104652]]
19 [[3,5],[4,448],[5,14077],[6,112079]] [[3,5],[4,443],[5,13587],[6,104340]]
20 [[3,10],[4,487],[5,14044],[6,112069]] [[3,10],[4,475],[5,13504],[6,104266]]

set_random(1);
outputext_fcs("set_random(1),random_projection_min_hg(arithprog_hg(6,1132),10)",tcol2sat_stdhg2stdfcs(random_projection_min_hg(G66,10)),"VanDerWaerden_p10m_1132.cnf");
> OKsolver_2002-O3-DNDEBUG VanDerWaerden_p10m_1132.cnf
s UNSATISFIABLE
c sat_status                            0
c initial_maximal_clause_length         6
c initial_number_of_variables           114
c initial_number_of_clauses             236742
c initial_number_of_literal_occurrences 1391680
c number_of_initial_unit-eliminations   0
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           0
c reddiff_number_of_clauses             0
c reddiff_number_of_literal_occurrences 0
c number_of_2-clauses_after_reduction   0
c running_time(sec)                     88.2
c number_of_nodes                       319
c number_of_single_nodes                0
c number_of_quasi_single_nodes          0
c number_of_2-reductions                758
c number_of_pure_literals               0
c number_of_autarkies                   0
c number_of_missed_single_nodes         0
c max_tree_depth                        12
c number_of_table_enlargements          0
c number_of_1-autarkies                 0
c number_of_new_2-clauses               0
c maximal_number_of_added_2-clauses     0
c file_name                             VanDerWaerden_p10m_1132.cnf

> minisat-2.2.0 VanDerWaerden_p10m_1132.cnf
Simplification time:         51.85 s
conflicts             : 2966
CPU time              : 54.2827 s

set_random(18);
outputext_fcs("set_random(1),random_projection_min_hg(arithprog_hg(6,1132),10)",tcol2sat_stdhg2stdfcs(random_projection_min_hg(G66,10)),"VanDerWaerden_p10m18_1132.cnf");
> minisat-2.2.0 VanDerWaerden_p10m18_1132.cnf
Simplification time:         50.32 s
conflicts             : 5788
CPU time              : 55.0556 s
UNSATISFIABLE
   \endverbatim
   Looks highly unsatisfiable. </li>
   <li> The above for n=1131 (so that we have a chance to get a satisfiable
   instance):
   \verbatim
G1131:arithprog_hg(6,1131)$
set_random(1);
G1131p5 : random_projection_hg(G1131,5)$
ncl_list_fcs(G1131p5);
  [[4,80],[5,6455],[6,120415]]
G1131p5m : min_hg(G1131p5)$
ncl_list_fcs(G1131p5m);
  [[4,80],[5,6373],[6,116899]]

block([n:1131],outputext_fcs(sconcat("set_random(1),random_projection_hg(arithprog_hg(6,",n,"),5)"),tcol2sat_stdhg2stdfcs(G1131p5m),sconcat("VanDerWaerden_p5_",n,".cnf")));

> minisat-2.2.0 VanDerWaerden_p5_1131.cnf
restarts              : 59609
conflicts             : 42612232       (674 /sec)
UNSATISFIABLE
(time unreliable due the frequency change)

for s : 1 thru 20 do block([G], set_random(s), G : random_projection_hg(G1131,5), print(s, ncl_list_fcs(G), ncl_list_fcs(min_hg(G))));
1 [[4,80],[5,6455],[6,120415]] [[4,80],[5,6373],[6,116899]]
2 [[3,3],[4,96],[5,6480],[6,120353]] [[3,3],[4,91],[5,6389],[6,116810]]
3 [[4,85],[5,6558],[6,120315]] [[4,85],[5,6464],[6,116793]]
4 [[4,91],[5,6457],[6,120392]] [[4,91],[5,6354],[6,116822]]
5 [[4,97],[5,6646],[6,120215]] [[4,97],[5,6544],[6,116529]]
6 [[3,1],[4,107],[5,6555],[6,120300]] [[3,1],[4,105],[5,6449],[6,116689]]
7 [[4,103],[5,6496],[6,120348]] [[4,103],[5,6392],[6,116725]]
8 [[4,107],[5,6107],[6,120772]] [[4,107],[5,6014],[6,117336]]
9 [[4,89],[5,6487],[6,120356]] [[4,89],[5,6403],[6,116854]]
10 [[4,87],[5,6364],[6,120501]] [[4,87],[5,6264],[6,117015]]
11 [[4,86],[5,6474],[6,120381]] [[4,86],[5,6378],[6,116831]]
12 [[4,78],[5,6391],[6,120497]] [[4,78],[5,6311],[6,117046]]
13 [[4,87],[5,6619],[6,120246]] [[4,87],[5,6538],[6,116661]]
14 [[4,72],[5,6568],[6,120271]] [[4,72],[5,6480],[6,116667]]
15 [[4,81],[5,6141],[6,120721]] [[4,81],[5,6061],[6,117387]]
16 [[4,76],[5,6540],[6,120322]] [[4,76],[5,6456],[6,116704]]
17 [[4,101],[5,6378],[6,120479]] [[4,101],[5,6267],[6,116952]]
18 [[4,101],[5,6437],[6,120437]] [[4,101],[5,6324],[6,116921]]
19 [[3,2],[4,91],[5,6478],[6,120360]] [[3,2],[4,90],[5,6394],[6,116804]]
20 [[4,117],[5,6536],[6,120303]] [[4,117],[5,6407],[6,116734]]

block([n:1131,s:2],set_random(s),outputext_fcs(sconcat("set_random(",s,"),random_projection_min_hg(arithprog_hg(6,",n,"),5)"),tcol2sat_stdhg2stdfcs(random_projection_min_hg(arithprog_hg(6,n),5)),sconcat("VanDerWaerden_p5",s,"_",n,".cnf")));

> minisat-2.2.0 VanDerWaerden_p52_1131.cnf
restarts              : 43005
conflicts             : 31011213       (688 /sec)
UNSATISFIABLE
(time unreliable due the frequency change)

> minisat-2.2.0 VanDerWaerden_p53_1131.cnf
restarts              : 69630
conflicts             : 54816243       (583 /sec)
CPU time              : 94557.6 s
UNSATISFIABLE
> precosat-570.1 -v VanDerWaerden_p53_1131.cnf
UNSATISFIABLE
15594970 conflicts
21855.9 seconds
   \endverbatim
   So perhaps randomisation doesn't work (at least here!)? Why? </li>
   <li> One should write a variation, which multicolours the hyperedges, that
   is, chooses the equivalence classes to avoid intersecting any hyperedge
   in more than one element (doing it greedily, possibly failing). </li>
   <li> One could do this more greedily, trying to minimise the number
   of equivalence classes (the number of vertices in the image)?
    <ol>
     <li> One needs to inspect the literature on greedy hypergraph colouring
     (with as few colours as possible, but not fixed in advance) and on
     greedy hypergraph multicolouring. </li>
     <li> The simplest approach perhaps is to make every independent set
     (equivalence class) as large as possible, one after another. </li>
     <li> "As large as possible" just means avoiding that a unit-hyperedge
     is obtained, or, stronger, that any hyperedge gets shortened. </li>
    </ol>
   </li>
   <li> Now using projection via the modulo-operation:
   \verbatim
m : ceiling(1132/5);
  227
G66m5 : modulo_projection_hg(G66, m)$
ncl_list_fcs(G66m5);
  [[6,25651]]
F66m5 : tcol2sat_stdhg2stdfcs(G66m5)$
outputext_fcs("modulo_projection_hg(arithprog_hg(6,1132),227)",F66m5,"VanDerWaerden_m5_1132.cnf");

> E=run_ubcsat("VanDerWaerden_m5_1132.cnf",runs=100,cutoff=1000000)
# segmentation fault for sapsnr
1. rots:
358 364 365 366 368 369 370 371 372 373
 14   8  27   6  16  17   2   8   1   1
fps: 61118
5. g2wsat:
358 359 368 369 370 371 372 373 374 375 376 377 378 379 380 381 382 383 384
  3   3   1   2   2   2   1   5   4   6  13   8   8  10  11   8   8   2   3
fps: 41672
6. ddfw:
358 359 360 365 369 370 371 372 373 374 375 376 377 378 379 380 381 382 383 384
  1   4   3   2   2   1   1   5   2   1   8  13  13  13  12   6   6   4   1   2
fps: 3803
14. anovp:
391 409 413 415 417 419 420 421 422 423 424 425 426 427 428 429 430 431 432 433
  1   1   3   1   1   1   5   6   5   5   9   6  10  11  10   6   8   1   5   4
434
  1
fps: 49414
35. uwalk:
627 628 629 630 631 632 633 634 635 636 637 638 640 641 642 643 644 645 646 647
  1   1   1   3   1   3   2   4   4   5   9   6   6   4   9   5   3   6   5   5
648 649 650 651 652 653 654
  5   3   1   1   2   2   3
fps: 180326
# sapsnr:
374 375 376 377 378 380 381 382 383 384 385 386 387 388 389 390 391 392 393 394
  1   1   1   1   1   1   1   2   1   4   3   7  16  10   8   9   9   7  14   3

# on cs-oksvr:
> ubcsat-okl -alg rots -runs 100 -cutoff 10000000 -i VanDerWaerden_m5_1132.cnf | tee VanDerWaerden_m5_1132.cnf_OUT
> E=read_ubcsat("VanDerWaerden_m5_1132.cnf_OUT")
358 364 365
 83   4  10
97
# on csltok:
> ubcsat-okl -alg adaptg2wsat -runs 100 -cutoff 10000000 -i VanDerWaerden_m5_1132.cnf | tee VanDerWaerden_m5_1132.cnf_OUT
> E=read_ubcsat("VanDerWaerden_m5_1132.cnf_OUT",nrows=100)
358 364 365 366 368 369
 74   4  12   3   4   3
100
   \endverbatim
   Very little achieved (and rots might be a bit "better"; these instances are
   very hard for the ubcsat-algorithms (they are unsatisfiable --- what is
   the real min? Is there a meaning to 358?) </li>
   <li> Now for n=1131:
   \verbatim
m : ceiling(1131/5);
  227
G66m1m5 : modulo_projection_hg(G66m1, m)$
ncl_list_fcs(G66m1m5);
  [[6,25651]]
is(G66m1m5 = G66m5);
  true
# thus still unsatisfiable
   \endverbatim
   </li>
   <li> One needs to investigate when changes happen:
   \verbatim
for n : 1132 thru 1100 step -1 do block([m:ceiling(n/5)],print(n,m,ncl_list_fcs(modulo_projection_hg(arithprog_hg(6,n),m))));
1132 227 [[6,25651]]
1131 227 [[6,25651]]
1130 226 [[2,113],[6,25312]]
1129 226 [[2,113],[6,25312]]
1128 226 [[2,113],[6,25312]]
1127 226 [[2,113],[6,25312]]
1126 226 [[2,113],[6,25312]]
1125 225 [[3,75],[5,45],[6,24525]]
1124 225 [[3,75],[5,45],[6,24525]]
1123 225 [[3,75],[5,45],[6,24525]]
1122 225 [[3,75],[5,45],[6,24525]]
1121 225 [[3,75],[5,45],[6,24525]]
1120 224 [[2,112],[4,56],[6,24192]]
1119 224 [[2,112],[4,56],[6,24192]]
1118 224 [[2,112],[4,56],[6,24192]]
1117 224 [[2,112],[4,56],[6,24192]]
1116 224 [[2,112],[4,56],[6,24192]]
1115 223 [[6,24753]]
1114 223 [[6,24753]]
1113 223 [[6,24753]]
1112 223 [[6,24753]]
1111 223 [[6,24753]]
1110 222 [[2,111],[3,74],[6,24013]]
1109 222 [[2,111],[3,74],[6,24013]]
1108 222 [[2,111],[3,74],[6,24013]]
1107 222 [[2,111],[3,74],[6,24013]]
1106 222 [[2,111],[3,74],[6,24013]]
1105 221 [[6,24310]]
1104 221 [[6,24310]]
1103 221 [[6,24310]]
1102 221 [[6,24310]]
1101 221 [[6,24310]]
1100 220 [[2,110],[4,55],[5,44],[6,23320]]

block([n:1130,m],m:ceiling(n/5),outputext_fcs(sconcat("modulo_projection_hg(arithprog_hg(6,",n,"),",m,")"),tcol2sat_stdhg2stdfcs(modulo_projection_hg(arithprog_hg(6,n),m)),sconcat("VanDerWaerden_m5_",n,".cnf")));
> minisat-2.2.0 VanDerWaerden_m5_1130.cnf
restarts              : 803
conflicts             : 342082         (3457 /sec)
CPU time              : 98.957 s
SATISFIABLE
G1130 : min_hg(modulo_projection_hg(arithprog_hg(6,1130),226))$
ncl_list_fcs(G1130);
  [[2,113],[6,25312]]
# no subsumption here.
ubcsat-okl -runs 100 -alg adaptg2wsat -cutoff 1000000 -i VanDerWaerden_m5_1130.cnf
  0 112 334 337 340 350
 24  62   9   2   2   1
100
   \endverbatim
   </li>
   <li> Using a complete solver seems hopeless for n=1132:
   \verbatim
> minisat-2.2.0 VanDerWaerden_m5_1132.cnf
# aborted after apparently making no progress:
|   86287761 |     227    51302   307812 |   361059    75919     40 |  0.002 %
restarts              : 131070
conflicts             : 103554002      (1407 /sec)
CPU time              : 73592.2 s
> cryptominisat VanDerWaerden_m5_1132.cnf
# aborted after apparently making no progress:
c conflicts                : 39040336    (565.34    / sec)
c CPU time                 : 69056.95    s
> march_pl VanDerWaerden_m5_1132.cnf
# aborted after 2232m, apparently making no progress.
   \endverbatim
   Seems much harder than the palindromic problem. </li>
   <li> Can one create a certificate for n=1132 by Rabung's method or
   variations?:
   \verbatim
primep(227);
  true
5*227;
  1135
rabung_valid_param(2,6,1136);
  true
rabung_derived_parameters(2,6,1136);
  [227,2]
rabung_checkcriterion(2,6,1136);
  false
cm : rabung_colouring_map(2,6,1136);
L : map(cm,create_list(i,i,1,226));
C : seq2certificatevdw(L);
certificate_vdw_p([6,6],226,C);
  false
# First part contains 25,26,27,28,29,30.

ifactors(226);
  [[2,1],[113,1]]
smallest_primitive_modular_root(226);
  3
totient(226);
  112
ifactors(112);
  [[2,4],[7,1]]
XXX
   \endverbatim
   </li>
   <li> Perhaps these parameter values are difficult for these methods. </li>
   <li> Using two-times nested mirror-fold:
   \verbatim
G66 : arithprog_hg(6,1132)$
f2 : mirrorfold(2,1132)$
G66f2 : transport_hg(f2,G66)$
ncl_list_fcs(G66f2);
  [[3,170],[4,175],[5,157],[6,47342]]
G66f2m : min_hg(G66f2)$
ncl_list_fcs(G66f2m);
  [[3,170],[4,37],[6,46722]]

F66f2 : tcol2sat_stdhg2stdfcs(G66f2m)$
outputext_fcs("transportmin_hg(mirrorfold(2,1132),arithprog_hg(6,1132))",F66f2,"VanDerWaerden_f2_1132.cnf");

> minisat-2.2.0 VanDerWaerden_f2_1132.cnf
restarts              : 58513
conflicts             : 41977994       (3104 /sec)
CPU time              : 13523.7 s
UNSATISFIABLE

G66m1 : arithprog_hg(6,1131)$
f2m1 : mirrorfold(2,1131)$
G66m1f2 : transport_hg(f2m1,G66m1)$
ncl_list_fcs(G66m1f2);
  [[3,169],[4,360],[5,351],[6,46695]]
G66m1f2m : min_hg(G66m1f2)$
ncl_list_fcs(G66m1f2m);
  [[3,169],[4,224],[5,56],[6,45910]]

F66m1f2 : tcol2sat_stdhg2stdfcs(G66m1f2m)$
outputext_fcs("transportmin_hg(mirrorfold(2,1131),arithprog_hg(6,1131))",F66m1f2,"VanDerWaerden_f2_1131.cnf");

> minisat-2.2.0 VanDerWaerden_f2_1131.cnf
restarts              : 2523
conflicts             : 1302293        (5564 /sec)
CPU time              : 234.046 s
UNSATISFIABLE

block([n:1130],outputext_fcs(sconcat("transportmin_hg(mirrorfold(2,",n,"),arithprog_hg(6,",n,"))"), tcol2sat_stdhg2stdfcs(transportmin_hg(mirrorfold(2,n),arithprog_hg(6,n))),sconcat("VanDerWaerden_f2_",n,".cnf")));
> minisat-2.2.0 VanDerWaerden_f2_1130.cnf
restarts              : 12285
conflicts             : 7557377        (4218 /sec)
CPU time              : 1791.51 s
UNSATISFIABLE
   \endverbatim
   </li>
  </ul>


  \todo vdw_2(7,7) > 3703
  <ul>
   <li>
   \verbatim
G3704:arithprog_hg(7,3704)$
ncl_list_fcs(G3704);
  [[7,1141450]]
m : ceiling(3704/6);
  618
G3704m6 : modulo_projection_hg(G3704, m)$
ncl_list_fcs(G3704m6);
  [[2,309],[3,206],[6,103],[7,189108]]
G3704m6m : min_hg(G3704m6)$
ncl_list_fcs(G3704m6m);
  [[2,309],[3,206],[7,189108]]
block([n:3704,m],m:ceiling(n/6),outputext_fcs(sconcat("min_hg(modulo_projection_hg(arithprog_hg(7,",n,"),",m,"))"),tcol2sat_stdhg2stdfcs(G3704m6m),sconcat("VanDerWaerden_m6_",n,".cnf")));

# cs-oksvr:
> minisat-2.2.0 VanDerWaerden_m6_3704.cnf
restarts              : 131071
conflicts             : 109417211      (228 /sec)
CPU time              : 480418 s
INDETERMINATE
# looks hopeless

G3704f2 : mirrorfold_projection_hg(G3704,2)$
ncl_list_fcs(G3704f2);
  [[2,1],[4,680],[5,514],[6,420],[7,426203]]
G3704f2m : min_hg(G3704f2)$
ncl_list_fcs(G3704f2m);
  [[2,1],[4,680],[5,123],[7,425015]]
block([n:3704,k:2],outputext_fcs(sconcat("min_hg(mirrorfold_projection_hg(arithprog_hg(7,",n,"),",k,"))"),tcol2sat_stdhg2stdfcs(G3704f2m),sconcat("VanDerWaerden_f",k,"_",n,".cnf")));

> minisat-2.2.0 VanDerWaerden_f2_3704.cnf
restarts              : 3044
conflicts             : 1535295        (1171 /sec)
CPU time              : 1311.52 s
INDETERMINATE
# looks hopeless
# on cs-oksvr:
>  E=run_ubcsat("VanDerWaerden_f2_3704.cnf", runs=100, cutoff=1000000)
1. ddfw:
2797 2806 2807 2877 2878 2879 2882 2990 3095 3118 3507 3530 3541 3546 3548 3549
   1    2    1    1    1    1    1    1    1    2    1    1    1    1    1    1
3556 3567 3572 3576 3585 3587 3589 3590 3595 3603 3607 3609 3612 3618 3620 3645
   1    1    1    1    1    1    1    1    1    1    1    1    1    4    1    1
3648 3650 3656 3662 3666 3670 3671 3675 3676 3677 3680 3682 3683 3686 3689 3690
   1    1    1    1    1    1    1    2    1    1    1    1    1    1    1    1
3693 3694 3696 3697 3703 3704 3708 3710 3712 3713 3714 3715 3716 3717 3718 3720
   1    2    1    1    1    1    2    1    2    1    1    1    1    1    1    2
3721 3722 3723 3729 3731 3732 3733 3735 3743 3746 3749 3751 3752 3754 3757 3759
   1    1    1    1    2    1    1    2    1    2    1    3    2    1    1    1
3761 3767 3770 3772
   1    1    1    1
fps: 75
2. irots:
2799 2804 2808 2812 2814 2818 2822 2823 2830 2856 2858 2865 2867 2868 2871 2874
   1    2    1    1    1    1    1    1    1    1    1    1    1    1    1    2
2878 2880 2881 2882 2883 2884 2887 2888 2889 2890 2891 2892 2893 2894 2895 2897
   1    2    1    4    1    3    1    2    1    1    1    2    3    1    1    2
2898 2900 2902 2903 2908 2910 2913 3099 3101 3107 3114 3131 3159 3167 3177 3303
   2    2    1    1    1    1    1    1    1    2    1    1    1    1    1    1
3305 3308 3311 3313 3315 3320 3321 3324 3331 3334 3337 3339 3340 3341 3342 3344
   1    1    1    2    1    1    1    1    1    1    2    1    2    2    1    1
3346 3348 3349 3351 3359 3360 3367 3371 3376 3383 3523
   3    2    2    1    1    1    2    1    1    1    1
fps: 3464
3. ag2wsat:
2807 2808 2809 2810 2815 2816 2818 2820 2821 2823 2824 2826 2834 2867 2869 2872
   1    3    1    1    2    1    1    2    1    2    1    1    1    1    1    1
2875 2876 2881 2978 2981 2984 2986 2987 2994 2995 3073 3079 3088 3127 3131 3135
   1    1    1    1    1    1    1    3    1    1    1    1    1    2    2    1
3140 3170 3213 3216 3279 3284 3289 3290 3291 3292 3295 3302 3303 3305 3306 3319
   1    1    1    1    1    1    1    2    1    1    2    3    1    1    1    1
3331 3336 3339 3345 3351 3358 3361 3362 3366 3369 3371 3373 3374 3375 3376 3377
   1    1    1    1    1    1    1    1    1    1    1    2    2    1    1    1
3382 3383 3388 3389 3390 3393 3397 3400 3403 3404 3407 3412 3413 3414 3419 3606
   1    1    1    1    4    1    2    1    1    1    2    1    1    1    1    1
fps: 3001
4. g2wsat:
2827 2833 2849 2850 2856 2857 2858 2861 2872 2876 2890 2906 2907 2911 2913 2920
   1    1    1    1    1    2    1    1    1    2    1    1    1    1    2    1
2924 2926 2928 2932 2943 2968 3023 3024 3033 3037 3039 3042 3185 3197 3299 3313
   1    1    1    1    1    1    1    1    1    1    1    1    1    1    1    1
3315 3316 3317 3319 3320 3321 3325 3326 3328 3331 3332 3333 3334 3335 3336 3337
   1    1    2    1    1    1    2    1    1    5    1    1    4    1    1    1
3338 3339 3342 3343 3344 3347 3348 3349 3350 3355 3370 3386 3389 3392 3393 3394
   1    1    1    1    1    1    1    1    1    1    1    2    1    2    2    1
3396 3398 3401 3402 3408 3409 3410 3411 3412 3413 3414 3421 3423 3438 3456
   2    2    1    1    2    1    2    1    2    1    1    1    2    1    1
fps: 2988


G3704f3 : mirrorfold_projection_hg(G3704,3)$
ncl_list_fcs(G3704f3);
  [[1,1],[2,231],[4,462],[5,462],[6,462],[7,211827]]
# thus unsatisfiable.
   \endverbatim
   </li>
  </ul>


  \todo vanderwaerden_2(5,13) > 1204, vdw_2^pd(5,13) >= (1176,1205)
  <ul>
   <li> See
   Investigations/RamseyTheory/VanderWaerdenProblems/plans/5-k/13.hpp.
   </li>
   <li> First looking at the palindromic solution for n=1204:
   \verbatim
G1204a : arithprog_hg(5,1204)$
G1204b : arithprog_hg(13,1204)$
ncl_list_fcs(G1204a);
  [[5,180600]]
ncl_list_fcs(G1204b);
  [[13,59800]]

f2 : mirrorfold(2,1204)$

G1204af2 : transport_hg(f2,G1204a)$
G1204bf2 : transport_hg(f2,G1204b)$
ncl_list_fcs(G1204af2);
  [[3,220],[4,215],[5,67132]]
ncl_list_fcs(G1204bf2);
  [[7,114],[8,71],[9,76],[10,78],[11,70],[12,63],[13,21942]]
G1204af2m : min_hg(G1204af2)$
G1204bf2m : min_hg(G1204bf2)$
ncl_list_fcs(G1204af2m);
  [[3,220],[4,29],[5,66411]]
ncl_list_fcs(G1204bf2m);
  [[7,114],[8,6],[9,10],[10,5],[13,21343]]

F1204f2 : gtcol2sat_stdohg2stdfcl(hg2ohg(G1204af2m),hg2ohg(G1204bf2m))$
outputext_fcl("transportmin_hg(mirrorfold(2,1204),arithprog_hg(5|13,1204))",F1204f2,"VanDerWaerden_pd2_2-5-13_1204.cnf");

> minisat-2.2.0 VanDerWaerden_pd2_2-5-13_1204.cnf
restarts              : 25512
conflicts             : 17201140       (2461 /sec)
CPU time              : 6990.89 s
INDETERMINATE

> SplittingViaOKsolver -D10 VanDerWaerden_pd2_2-5-13_1204.cnf
> cat Md5sum
01bb784146360f684fd7c32e85d49cc0
> cat Result
s UNKNOWN
c sat_status                            2
c initial_maximal_clause_length         13
c initial_number_of_variables           301
c initial_number_of_clauses             88138
c initial_number_of_literal_occurrences 611276
c number_of_initial_unit-eliminations   0
c reddiff_maximal_clause_length         0
c reddiff_number_of_variables           0
c reddiff_number_of_clauses             0
c reddiff_number_of_literal_occurrences 0
c number_of_2-clauses_after_reduction   0
c running_time(sec)                     275.1
c number_of_nodes                       1649
c number_of_quasi_single_nodes          0
c number_of_2-reductions                0
c number_of_pure_literals               0
c number_of_autarkies                   0
c max_tree_depth                        10
c file_name                             VanDerWaerden_pd2_2-5-13_1204.cnf
c splitting_cases                       825
> cat Statistics
> summary(E$n)
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
  10.00   10.00   10.00   10.07   10.00   12.00
> table(E$n)
 10  11  12
771  51   3
> summary(E$d)
   Min. 1st Qu.  Median    Mean 3rd Qu.    Max.
  8.000  10.000  10.000   9.771  10.000  10.000
> table(E$d)
  8   9  10
 10 169 646
> ProcessSplitViaOKsolver SplitViaOKsolver_D10VanDerWaerden_pd2_2513_1204cnf_2011-11-20-123007/
# Aborted on the first splitting-instance:
restarts              : 36862
conflicts             : 27062591       (2556 /sec)
CPU time              : 10587.5 s
INDETERMINATE

> ubcsat-okl -alg adaptg2wsat -runs 100 -cutoff 10000000 -i VanDerWaerden_pd2_2-5-13_1204.cnf | tee VanDerWaerden_pd2_2-5-13_1204.cnf_OUT
> E=read_ubcsat("VanDerWaerden_pd2_2-5-13_1204.cnf_OUT",nrows=100)
20 21 22 23 24 25 26 27 28 29 30 31 32 33
 1  3  3  2  3  7  7  9 11 16 13 13 10  2
100
> ubcsat-okl -alg ddfw -runs 100 -cutoff 10000000 -i VanDerWaerden_pd2_2-5-13_1204.cnf | tee VanDerWaerden_pd2_2-5-13_1204.cnf_OUT
19 21 22 23 24 25 26 27 28 29 30
 1  2  4 11  9 12 23 14 13  7  4
100
   \endverbatim
   </li>
   <li> For n=1202:
   \verbatim
G1202a : arithprog_hg(5,1202)$
G1202b : arithprog_hg(13,1202)$
ncl_list_fcs(G1202a);
  [[5,180000]]
ncl_list_fcs(G1202b);
  [[13,59600]]

f2 : mirrorfold(2,1202)$

G1202af2 : transport_hg(f2,G1202a)$
G1202bf2 : transport_hg(f2,G1202b)$
ncl_list_fcs(G1202af2);
  [[3,370],[4,514],[5,66609]]
ncl_list_fcs(G1202bf2);
  [[7,123],[8,133],[9,152],[10,179],[11,161],[12,146],[13,21519]]
G1202af2m : min_hg(G1202af2)$
G1202bf2m : min_hg(G1202bf2)$
ncl_list_fcs(G1202af2m);
  [[3,370],[4,178],[5,65720]]
ncl_list_fcs(G1202bf2m);
  [[7,123],[8,16],[9,24],[10,30],[13,21109]]

F1202f2 : gtcol2sat_stdohg2stdfcl(hg2ohg(G1202af2m),hg2ohg(G1202bf2m))$
outputext_fcl("transportmin_hg(mirrorfold(2,1202),arithprog_hg(5|13,1202))",F1202f2,"VanDerWaerden_pd2_2-5-13_1202.cnf");

> minisat-2.2.0 VanDerWaerden_pd2_2-5-13_1202.cnf
restarts              : 441975
conflicts             : 381956261      (1515 /sec)
CPU time              : 252197 s
INDETERMINATE
# apparently no progress
   \endverbatim
   </li>
   <li> Now modular reduction:
   \verbatim
n : 1204$
m : ceiling(n/4);
  301
k1:5$
k2:13$
G1204a : arithprog_hg(k1,n)$
G1204b : arithprog_hg(k2,n)$
G1204am4 : modulo_projection_hg(G1204a, m)$
ncl_list_fcs(G1204am4);
  [[5,45150]]
G1204bm4 : modulo_projection_hg(G1204b, m)$
ncl_list_fcs(G1204bm4);
  [[7,43],[13,25802]]
G1204bm4m : min_hg(G1204bm4)$
ncl_list_fcs(G1204bm4m);
  [[7,43],[13,25802]]

F1204m4 : gtcol2sat_stdohg2stdfcl(hg2ohg(G1204am4),hg2ohg(G1204bm4))$
outputext_fcl("modulo_projection_hg(arithprog_hg(5|13,1204),301)",F1204m4,"VanDerWaerden_m4_2-5-13_1204.cnf");

> ubcsat-okl -alg adaptg2wsat -runs 100 -cutoff 1000000 -i VanDerWaerden_m4_2-5-13_1204.cnf | tee VanDerWaerden_m4_2-5-13_1204.cnf_OUT
> E=read_ubcsat("VanDerWaerden_m4_2-5-13_1204.cnf_OUT",nrows=100)
16 17 18 19 20 21
 8 18 30 25 17  2
100
# cutoff=10^7:
13 14 15 16 17 18
 4 10 22 35 26  3
100
# cutoff=10^8:
> E=read_ubcsat("VanDerWaerden_m4_2-5-13_1204.cnf_OUT",nrows=100)
11 12 13 14 15 16
 1  5 21 43 29  1
100
# cutoff=10^9:
> E=read_ubcsat("VanDerWaerden_m4_2-5-13_1204.cnf_OUT",nrows=100)
10 11 12 13 14
 2  8 44 41  5
100
   \endverbattim
   </li>
  </ul>

*/

