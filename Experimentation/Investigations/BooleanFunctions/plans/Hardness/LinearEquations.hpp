// Oliver Kullmann, 11.5.2013 (Swansea)
/* Copyright 2013 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/BooleanFunctions/plans/Hardness/LinearEquations.hpp
  \brief Investigations regarding hardness of representations of systems of linear equations


  \todo Connections
  <ul>
   <li> Currently the functions are in
   ComputerAlgebra/Satisfiability/Lisp/Generators/LinearEquations.mac. </li>
  </ul>


  \todo gen_2xor_fcl
  <ul>
   <li> Creation:
   \verbatim
output_gen_2xor_stdname(n);
   \endverbatim
   </li>
   <li> The hardness of output_gen_2xor_stdname(n) is precisely n. </li>
   <li> OKsolver_2002-O3-DNDEBUG behaves completely predictable:
    <ol>
     <li> max_tree_depth = n-2 </li>
     <li> number_of_nodes = 2^(n-1) - 1 </li>
     <li> number_of_quasi_single_nodes = 2^(n-3) </li>
     <li> number_of_2-reductions = 2^(n-2) </li>
    </ol>
   </li>
   <li> satz215 behaves completely predictable:
    <ol>
     <li> NB_BACK = 2^(n-3) </li>
     <li> NB_BRANCHE = 2^(n-2) - 1 </li>
    </ol>
    satz215 seems slower than the OKsolver.
   </li>
   <li> march_pl solves these instances quickly via equivalence-reasoning.
   </li>
   <li> minisat also solves all instances via (quick) preprocessing. Would be
   interesting to find it what happens here. </li>
   <li> Also minisat-no-pre solves large instances very quickly (for example
   n=10000 in 15 sec; now of course based on splitting). </li>
   <li> Picosat913 has problems already with n=1000 (aborted after 180 sec,
   apparently without progress). </li>
   <li> Precosat-570 solves instances quickly by preprocessing. </li>
   <li> lingelingala-b02aa1a-121013 solves instances even more quickly than
   minisat-no-pre. </li>
   <li> Also glucose-2.0 seems faster then minisat-no-pre. </li>
   <li> cryptominisat-296 uses apparently a mixture of splitting and
   xor-reasoning (very quick). </li>
   <li> The w-hardness of these instances seems to be 3:
   \verbatim
min_kresolution_closure_cs(fcl2cs(gen_2xor_fcl(3)),2);
[{{-5,-4},{-5,-3},{-5,-2,-1},{-5,-2,1},{-5,-1,2},{-5,1,2},{-4,-2,-1},
 {-4,-2,1},{-4,-1,2},{-4,1,2},{-4,3},{-3,-2,-1},{-3,-2,1},{-3,-1,2},
 {-3,1,2},{-3,4},{-2,-1,3},{-2,-1,4},{-2,-1,5},{-2,1,3},{-2,1,4},
 {-2,1,5},{-1,2,3},{-1,2,4},{-1,2,5},{1,2,3},{1,2,4},{1,2,5},{3,5},{4,5}},
[12,22,30,30]]
for k : 0 thru 5 do print(k, min_kresolution_closure_cs(fcl2cs(gen_2xor_fcl(k)),3));
0 [{{}},[1]]
1 [{{}},[2,1]]
2 [{{}},[4,4,1]]
3 [{{}},[12,30,36,10,1]]
4 [{{}},[20,58,216,122,12,1]]
5 [{{}},[28,90,388,1274,380,54,1]]
   \endverbatim
   </li>
   <li> The symmetric width of these instances seems to be 3:
   \verbatim
for k : 0 thru 10 do print(k, min_bresolution_closure_cs(fcl2cs(gen_2xor_fcl(k)),3));
0 [{{}},[1]]
1 [{{}},[2,1]]
2 [{{}},[4,4,1]]
3 [{{}},[12,30,36,10,1]]
4 [{{}},[20,58,240,142,16,1]]
5 [{{}},[28,90,508,1882,592,34,1]]
6 [{{}},[36,54,64,66,86,88,86,26,1]]
7 [{{}},[44,62,72,74,94,96,94,122,112,52,1]]
8 [{{}},[52,70,80,82,102,104,102,122,124,122,70,1]]
9 [{{}},[60,78,88,90,110,112,110,130,132,130,158,148,88,1]]
10 [{{}},[68,86,96,98,118,120,118,138,140,138,158,160,158,106,1]]
   \endverbatim
   </li>
  </ul>


  \todo Translating two xor-clauses
  <ul>
   <li> The conjecture is that in general the translation gprt2s2cl_aux_2
   (which translates two XOR-clauses via adding the derived XOR-clause)
   achieves relative p-hardness 1, while (absolute) hardness is unbounded:
   \verbatim
F1 : gprt2s2cl_aux_2([[1,2,3,10,7,14],0],[4,8,11,15],[[2,3,5,7,10],0],[6,9,12],[13,16]);
  [{-4,-2,-1},{-4,1,2},{-2,1,4},{-1,2,4},{-8,-4,-3},{-8,3,4},{-4,3,8},{-3,4,8},{-11,-8,-7},{-11,7,8},{-8,7,11},{-7,8,11},{-15,-11,-10},{-15,10,11},{-11,10,15},{-10,11,15},{-15,14},{-14,15},{-6,-3,-2},{-6,2,3},{-3,2,6},{-2,3,6},{-9,-6,-5},{-9,5,6},{-6,5,9},{-5,6,9},{-12,-9,-7},{-12,7,9},{-9,7,12},{-7,9,12},{-12,10},{-10,12},{-13,-5,-1},{-13,1,5},{-5,1,13},{-1,5,13},{-14,13},{-13,14}]
hardness_cs(cl2cs(F1));
  2
F2 : gprt2s2cl_aux_2([[1,2,3,4],0],[5,6],[[1,2,3,7],0],[8,9],[])$
hardness_cs(cl2cs(F2));
  2
F3 : gprt2s2cl_aux_2([[1,2,3,4,10],0],[5,6,11],[[1,2,3,7,10],0],[8,9,12],[])$
hardness_cs(cl2cs(F3));
  2
F4 : gprt2s2cl_aux_2([[1,2,3,4,10,13],0],[5,6,11,14],[[1,2,3,7,10],0],[8,9,12],[15])$
hardness_cs(cl2cs(F4));
  3
F5 : cl2cs(gprt2s2cl_aux_2([[1,2,3,4,10,13],0],[5,6,11,14],[[1,2,3,7,10,16],0],[8,9,12,17],[15,18]))$
F5i : ucp_0_cs(apply_pa(comp_sl({6,-9}),F5))$
current_satsolver(cs2fcs(F5i));
  false
hardness_u_cs(F5i);
  3
   \endverbatim
   </li>
   <li> Analysis of hardness:
    <ol>
     <li>
     \verbatim
F2 : gprt2s2cl_aux_2([[1,2,3,4],0],[5,6],[[1,2,3,7],0],[8,9],[])$
     \endverbatim
     is a minimal example with hardness 2. A prime implicate needing r_2 is
     {-8,5}. </li>
     <li> Considering the constraints as x_1+x_2+x_3+x_4=0 and
     x_1+x_2+x_3+x_7=0, the application of the partial assignment <-8,5 -> 0>
     creates x_1+x_2=0 and x_1+x_2=1. </li>
     <li> This effectively cuts off the first two variables from the equations,
     when considering the translations of these equations. </li>
     <li> In this case this causes then the determination of all other
     variables via r_2. </li>
     <li> But only due to the fact that there are only two other variables in
     each of the two equations. </li>
    </ol>
   </li>
   <li> In order to achieve (absolute!) hardness 1, we need to take into
   account that via instantiating the new variables we create effectively new
   equations.
    <ol>
     <li> If [v_1,...,v_m] is the list of common variables of the two
     constraints, then just treating the cases "v_1,...,v_i set to 0 and 1"
     seems to suffice. </li>
     <li> Then one better sorts the two prt-constraints, first the common
     variables. </li>
    </ol>
   </li>
   <li> Improved translation, not repeating the common literals:
   \verbatim
F1 : gprt2s2cl_aux_s([[1,2,3,10,7,14],0],[4,8,11,15],[[2,3,5,7,10],0],[6]);
  [{-4,-3,-2},{-4,2,3},{-3,2,4},{-2,3,4},{-8,-7,-4},{-8,4,7},{-7,4,8},{-4,7,8},{-11,-10,-8},{-11,8,10},{-10,8,11},{-8,10,11},{-11,5},{-5,11},{-11,-6,-1},{-11,1,6},{-6,1,11},{-1,6,11},{-14,6},{-6,14}]
hardness_cs(cl2cs(F1));
  1
F2 : gprt2s2cl_aux_s([[1,2,3,4],0],[5,6],[[1,2,3,7],0],[])$
hardness_cs(cl2cs(F2));
  1
F3 : gprt2s2cl_aux_s([[1,2,3,4,10],0],[5,6,11],[[1,2,3,7,10],0],[])$
hardness_cs(cl2cs(F3));
  1
F4 : gprt2s2cl_aux_s([[1,2,3,4,10,13],0],[5],[[1,2,3,7,10],0],[8,9,12])$
hardness_cs(cl2cs(F4));
  1
F5 : cl2cs(gprt2s2cl_aux_s([[1,2,3,4,10,13],0],[5,6,11,14],[[1,2,3,7,10,16],0],[8]))$
hardness_cs(F5);
  1
   \endverbatim
   </li>
   <li> While w-hardness also of the general case for the translation
   sprt2cl_aux_1 of two xor-clauses should be 3:
   \verbatim
F1 : cl2cs(sprt2cl_aux_1([[[1,2,3,4],[7,8]],[[3,4,5,6],[9,10]]]));
  {{-10,-9,-5},{-10,5,9},{-10,6},{-9,-4,-3},{-9,3,4},{-9,5,10},{-8,-7,-3},{-8,3,7},{-8,4},{-7,-2,-1},{-7,1,2},{-7,3,8},{-6,10},{-5,9,10},{-4,3,9},{-4,8},{-3,4,9},{-3,7,8},{-2,1,7},{-1,2,7}}
length(min_resolution_closure_cs(F1)[1]);
  110
whardness_cs(F1);
  2
F2 : cl2cs(sprt2cl_aux_1([[[1,2,3,4,11],[7,8,12]],[[3,4,5,6],[9,10,13]]]))$
P2 : min_resolution_closure_cs(F2)[1]$
length(P2);
  424
whardness_wpi_cs(F2,P2);
  2
F3 : cl2cs(sprt2cl_aux_1([[[1,2,3],[4]],[[1,2,-3],[5]]]))$
P3 : min_resolution_closure_cs(F3)[1]$
length(P3);
  1
whardness_wpi_cs(F3,P3);
  3
F4 : cl2cs(sprt2cl_aux_1([[[1,2,3,6],[4,7]],[[1,2,-3,8],[5,9]]]))$
P4 : min_resolution_closure_cs(F4)[1]$
length(P4);
  86
whardness_wpi_cs(F4,P4);
  3
F5 : cl2cs(sprt2cl_aux_1([[[1,2,3,6,10],[4,7,11]],[[1,2,-3,8,12],[5,9,13]]]))$
P5 : min_resolution_closure_cs(F5)[1]$
length(P5);
  240
whardness_wpi_cs(F5,P5);
  3
   \endverbatim
   </li>
  </ul>

*/
