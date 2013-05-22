// Oliver Kullmann, 22.5.2013 (Swansea)
/* Copyright 2013 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Solvers/Satz/docus/general.hpp
  \brief General documentation on the Satz solver


  <h1> Satz in version 215 </h1>

  Information on the original solver "Satz" can be found here:
  <ul>
   <li> The author's page:
   http://home.mis.u-picardie.fr/~cli/EnglishPage.html </li>
   <li> And Wikipedia: http://en.wikipedia.org/wiki/Satz_%28SAT_solver%29 </li>
  </ul>

  We have changed the code in the following aspects:
  <ul>
   <li> Safe and controllable input. </li>
   <li> Updated and extended output. </li>
   <li> Updated (and improved) code to C99. </li>
  </ul>
  The new code is Satisfiability/Solvers/Satz/satz215.2.c.

  <h2> Macro settings </h2>

  <ul>
   <li> WORD_LENGTH = 100 (currently unchecked!) </li>
   <li> MAX_NUMBER_VARIABLES = 200000 </li>
   <li> MAX_NUMBER_CLAUSES = 800000 </li>
   <li> MAX_CLAUSE_LENGTH = 1000 </li>
  </ul>


  <h2> Basic usage </h2>

  <ul>
   <li> Call with
   \verbatim
> satz215 DimacsFile.cnf
   \endverbatim
   </li>
   <li> If some macro (see above) has to be set, for example to enable longer
   clauses, then you need to recompile satz:
   \verbatim
ExternalSources> CPPFLAGS="-DMAX_CLAUSE_LENGTH=2000" oklib satz
   \endverbatim
   </li>
  </ul>


  <h2> Output </h2>

  <ul>
   <li> The output follows DIMACS-formatting rules. An example:
   \verbatim
> satz215 VanDerWaerden_2-3-12_134.cnf 
s SATISFIABLE
c sat_status                            1
c number_of_variables                   134
c initial_number_of_clauses             5172
c reddiff_number_of_clauses             0
c running_time(sec)                     89.06
c number_of_nodes                       240158
c number_of_binary_nodes                122132
c number_of_pure_literals               53
c number_of_1-reductions                4983797
c number_of_2-look-ahead                27193110
c number_of_2-reductions                1094520
c number_of_3-look-ahead                522702
c number_of_3-reductions                236814
c file_name                             VanDerWaerden_2-3-12_134.cnf
   \endverbatim
   where the instance was created by "VanderWaerdenCNF-O3-DNDEBUG 3 12 134".
   </li>
   <li> The meaning of the fields:
    <ol>
     <li> The status line (starting with "s") can be SATISFIABLE or
     UNSATISFIABLE. </li>
     <li> sat_status is 0 (unsatisfiable) or 1 (satisfiable). </li>
     <li> The preprocessing phase can add binary and ternary clauses (by
     resolution), and can delete clauses by subsumption and pure literal
     elimination. The difference "new - old" is given by
     reddiff_number_of_clauses. </li>
     <li> There are two possibilites that a node can have (exactly) one child:
      <ul>
       <li> The first child was detected satisfiable. </li>
       <li> Or (precisely) one child was detected unsatisfiable (possible,
       since the r_2 reduction is incomplete). </li>
      </ul>
     A "binary node" has precisely two children.
     </li>
     <li> An "r_2-look-ahead" is probing r_1((x -> 1> * F) for
     unsatisfiability, where x is one literal, while F is the residual
     clause-set. </li>
     <li> Accordingly, an "r_3-look-ahead" is probing r_2^-((x -> 1> * F) for
     unsatisfiability, where "r_2^-" is the incomplete version of r_2 used
     here. </li>
     <li> We note, that as usual the reduction (incomplete r_2 and r_3) is
     performed within a node. </li>
    </ol>
   </li>
   <li> The tool "ExtractSatz" converts the output into R-readable format:
   \verbatim
> satz215 VanDerWaerden_2-3-12_134.cnf | tee Output
> cat Output | ExtractSatz | tee Output2
rn rc t sat nds r1 r2 pls file bnds r2la r3 r3la dc
134 5172 89.06 1 240158 4983797 1094520 53 "VanDerWaerden_2-3-12_134.cnf" 122132 27193110 236814 522702 0
> oklib --R
# in R:
> oklib_load_all()
> E=read_satstat("Output2")
> E
   rn   rc     t sat    nds      r1      r2 pls                         file
1 134 5172 89.06   1 240158 4983797 1094520  53 VanDerWaerden_2-3-12_134.cnf
    bnds     r2la     r3   r3la dc
1 122132 27193110 236814 522702  0
   \endverbatim
   </li>
   <li> Return codes:
    <ol>
     <li> EXITCODE_SAT 10 </li>
     <li> EXITCODE_UNSAT 20 </li>
     <li> EXITCODE_PARAMETER_FAILURE 100 </li>
     <li> EXITCODE_INPUT_ERROR 101 </li>
     <li> EXITCODE_VERIFICATION_FAILED 102 </li>
    </ol>
   </li>
   <li> If the clause-set was found satisfiable, then the satisfying assignment
   is stored in file "satz.sol" (in the calling directory). </li>
   <li> Furthermore a file "satz215_timetable" is created (in the calling
   directory), and all runs result in an appended statistics-line. If the
   above call would have been the only call to satz (in this directory), then
   we would get:
   \verbatim
> cat satz215_timetable
 rn rc t sat nds r1 r2 pls file bnds r2la r3 r3la dc
134 5172 89.06 1 240158 4983797 1094520 53 "VanDerWaerden_2-3-12_134.cnf" 122132 27193110 236814 522702 0
   \endverbatim
   As shown above, this file can be read into R via "read_satstat".
   </li>
  </ul>


  <h2> Combination with pre-processors </h2>

  <ul>
   <li> Combination with the Minisat-2.2.0 pre-processor:
    <ol>
     <li> Provided by the wrapper script <code>satz215-m2pp</code>. </li>
     <li> Parameter is the filename. </li>
     <li> Creating a temporary file containing the preprocessed file. </li>
     <li> Three additional outputs are provided before calling satz215:
      <ol>
       <li> Statistics on the original input. </li>
       <li> The output of the Minisat-2.2.0 preprocessor. </li>
       <li> Statistics on the preprocessed input. </li>
      </ol>
     </li>
     <li> For example for the Sudoku-example created in Maxima by
     "output_sdk_EasterMonster();":
     \verbatim
OKplatform> satz215 S_EasterMonster.cnf
s SATISFIABLE
c sat_status                            1
c number_of_variables                   729
c initial_number_of_clauses             10551
c reddiff_number_of_clauses             -8984
c running_time(sec)                     0.02
c number_of_nodes                       5
c number_of_binary_nodes                1
c number_of_pure_literals               0
c number_of_1-reductions                313
c number_of_2-look-ahead                279
c number_of_2-reductions                19
c number_of_3-look-ahead                25
c number_of_3-reductions                13
c file_name                             S_EasterMonster.cnf

OKplatform> satz215-m2pp S_EasterMonster.cnf | tee Output
     pn      pc      n    nmi       c        l     n0   n0mi      c0       l0  cmts
    729   10551    729    729   10551    23349     NA     NA   10551    23349     3
 length   count
      1      21
      2   10206
      9     324

WARNING: for repeatability, setting FPU to use double precision
============================[ Problem Statistics ]=============================
|                                                                             |
|  Number of variables:           729                                         |
|  Number of clauses:           10530                                         |
|  Parse time:                   0.00 s                                       |
|  Simplification time:          0.01 s                                       |
|                                                                             |
==============================[ Writing DIMACS ]===============================
Wrote 1565 clauses with 239 variables.
restarts              : 0
conflicts             : 0              (0 /sec)
decisions             : 0              (-nan % random) (0 /sec)
propagations          : 490            (54457 /sec)
conflict literals     : 0              (-nan % deleted)
Memory used           : 20.00 MB
CPU time              : 0.008998 s

     pn      pc      n    nmi       c        l     n0   n0mi      c0       l0  cmts
    239    1565    239    239    1565     3606     NA     NA    1565     3606     0
 length   count
      2    1341
      3      64
      4      78
      5      72
      6      10

s SATISFIABLE
c sat_status                            1
c number_of_variables                   239
c initial_number_of_clauses             1565
c reddiff_number_of_clauses             0
c running_time(sec)                     0.00
c number_of_nodes                       4
c number_of_binary_nodes                1
c number_of_pure_literals               0
c number_of_1-reductions                447
c number_of_2-look-ahead                227
c number_of_2-reductions                51
c number_of_3-look-ahead                284
c number_of_3-reductions                42
c file_name                             S_EasterMonster.cnf_m2pp_9852

OKplatform> cat Output | ExtractSatz 
rn rc t sat nds r1 r2 pls file bnds r2la r3 r3la dc
239 1565 0.00 1 4 447 51 0 "S_EasterMonster.cnf_m2pp_10962" 1 227 42 284 0
     \endverbatim
     </li>
    </ol>
  </ul>

*/
