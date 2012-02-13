// Matthew Gwynne, 7.2.2012 (Swansea)
/* Copyright 2012 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ExperimentSystem/SolverMonitoring/plans/Glucose.hpp
  \brief General plans monitoring glucose and glucose-2.0


  \todo Extracted data attribute names
  <ul>
   <li> A typical glucose run on PHP(5,4):
   \verbatim
maxima> output_weak_php(6,5,"PHP_6_5.cnf");
> glucose PHP_6_5.cnf

c This is glucose 1.0 --  based on MiniSAT (Many thanks to MiniSAT team)

c ============================[ Problem Statistics ]=============================
c |                                                                             |
c |  Number of variables:  30                                                   |
c |  Number of clauses:    81                                                   |
c |  Parsing time:         0.00         s                                       |
============================[ Search Statistics ]==============================
| Conflicts |          ORIGINAL         |          LEARNT          | Progress |
|           |    Vars  Clauses Literals |    Limit  Clauses Lit/Cl |          |
===============================================================================
|         0 |      30       81      180 |       27        0   -nan |  0.000 % |
===============================================================================
c restarts              : 1
c nb ReduceDB           : 0
c nb learnts DL2        : 15
c nb learnts size 2     : 11
c nb learnts size 1     : 4
c conflicts             : 139            (inf /sec)
c decisions             : 162            (1.23 % random) (inf /sec)
c propagations          : 800            (inf /sec)
c conflict literals     : 868            (19.63 % deleted)
c Memory used           : 1.88 MB
c CPU time              : 0 s

s UNSATISFIABLE
   \endverbatim
   </li>
   <li> A typical glucose-2.0 run:
   \verbatim
maxima> output_weak_php(6,5,"PHP_6_5.cnf");
> glucose-2.0 PHP_6_5.cnf

c This is glucose 2.0 --  based on MiniSAT (Many thanks to MiniSAT team)
c WARNING: for repeatability, setting FPU to use double precision
c ============================[ Problem Statistics ]=============================
c |                                                                             |
c |  Number of variables:            30                                         |
c |  Number of clauses:              81                                         |
c |  Parse time:                   0.00 s                                       |
c |                                                                             |
c ============================[ Search Statistics ]==============================
c | Conflicts |          ORIGINAL         |          LEARNT          | Progress |
c |           |    Vars  Clauses Literals |    Limit  Clauses Lit/Cl |          |
c ===============================================================================
c |       148 |       4       79      170 |        0       35      5 | 86.667 % |
c ===============================================================================
c restarts              : 1
c nb ReduceDB           : 0
c nb removed Clauses    : 0
c nb learnts DL2        : 22
c nb learnts size 2     : 12
c nb learnts size 1     : 6
c conflicts             : 148            (inf /sec)
c decisions             : 198            (0.00 % random) (inf /sec)
c propagations          : 956            (inf /sec)
c conflict literals     : 879            (17.15 % deleted)
c nb reduced Clauses    : 32
c CPU time              : 0 s

s UNSATISFIABLE
   \endverbatim
   </li>
   <li> We see that the two key differences in output are
    <ul>
     <li> "Parsing time" for glucose vs "Parse time" for glucose-2.0. </li>
     <li> "Memory used" isn't given by glucose-2.0 but is by glucose. </li>
     <li> "nb removed Clauses" isn't given by glucose but is by glucose-2.0.
     </li>
    </ul>
   </li>
   <li> As there are only minor differences, as well as the fact that
   glucose is deprecated in favour of glucose-2.0, it seems best to
   have just one script that handles both glucose and glucose-2.0.
   </li>
   <li> We now have ExtractGlucose:
   \verbatim
maxima> output_weak_php(6,5,"PHP_6_5.cnf");
> glucose PHP_6_5.cnf | ExtractGlucose
rn rc t sat cfs dec rts r1 mem ptime cfl rdb rrc ldlc l2c l1c
30 81 0 0 139 162 1 800 1.88 0.00 868 0 0 15 11 4

> glucose-2.0 PHP_6_5.cnf | ExtractGlucose
rn rc t sat cfs dec rts r1 mem ptime cfl rdb rrc ldlc l2c l1c
30 81 0 0 148 198 1 956 0 0.00 879 0 0 22 12 6
   \endverbatim
   </li>
   <li> Attributes that are given by glucose but not glucose-2.0 or
   vice-versa are given default "zero" values. </li>
   <li> We have the following attributes:
    <ul>
     <li> rn : the (solver-specific) number of variables. </li>
     <li> rc : the (solver-specific) number of clauses. </li>
     <li> t : the time taken in seconds. </li>
     <li> sat: satisfiable (1), unsatisfiable (0), UNKNOWN (2). </li>
     <li> cfs : number of conflicts. </li>
     <li> dec: number of decisions. </li>
     <li> rts: number of restarts. </li>
     <li> r1: number of unit-clauses propagated. </li>
     <li> mem: memory used in MB. </li>
     <li> ptime: time spent preprocessing in seconds. </li>
     <li> cfl: number of conflict literals. </li>
     <li> rdb: number of "ReduceDB" calls. </li>
     <li> rrc: number of "removed clauses". </li>
     <li> ldlc: number of learnt clauses "DL2". </li>
     <li> l2c: number of learnt clauses of size 2. </li>
     <li> l1c: number of learnt clauses of size 1. </li>
    </ul>
   </li>
   <li> We should investigate:
    <ul>
     <li> What is "ReduceDB"? </li>
     <li> When are clauses "removed" and why? </li>
     <li> What is DL2? </li>
    </ul>
   </li>
   <li> We need full explanations for each of these attributes in the docus.
   </li>
  </ul>



*/
