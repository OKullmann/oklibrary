// Oliver Kullmann, 11.11.2007 (Swansea)
/* Copyright 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Configurations/MaxComponents/plans/general.hpp
  \brief Plans on the "Enigma1277" ("Faron's Puzzle") and generalisations


  \todo Update namespace.


  \todo Create milestones.


  \todo Empirical study
  <ul>
   <li> Under Experimentation/ExperimentSystem we need a program which allows
   to run a collection of solvers of a collection on benchmarks, storing
   the results in some appropriate directory structure. </li>
   <li> Solvers to investigate:
    <ol>
     <li> OKsolver2002 in all variations </li>
     <li> The march solvers </li>
     <li> Ubcsat solvers </li>
     <li> Minisat </li>
    </ol>
   </li>
   <li> Parameters to investigate:
    <ol>
     <li> Number of solutions, number of essentially different solutions
     (at least for the maximal score) </li>
     <li> Tree-resolution hardness </li>
     <li> Structure of autarky monoid </li>
    </ol>
   </li>
  </ul>


  \todo Alternative formulations
  <ul>
   <li> Quantified Boolean Formulas ? </li>
   <li> Obviously we can use non-boolean variables; also more powerful
   types of clauses? </li>
   <li> First-order logic ? </li>
   <li> Is there an alternative propositional translation? Using different
   auxiliary variables?
    <ol>
     <li> Instead of a total score k one can subdivide it into k = k_0 + k_1,
     that is, we have two parameters k_0, k_1, and k_e is the minimal number
     of components with vertices assigned value e. </li>
     <li> The primary variables stay as before, but instead of one type of
     auxiliary variables c_{i,j,p}, with k instances per field, we now have two
     types of auxiliary variables, with k_0 resp. k_1 instances per field:
      <ul>
       <li> c^e_{i,j,p} for 1 <= p <= k_e and e in {0,1} </li>
       <li> b_{i,j} -> not c^0{i,j,p} for all 1 <= p <= k_0 </li>
       <li> not b_{i,j} -> not c^1{i,j,p} for all 1 <= p <= k_1 </li>
      </ul>
     </li>
     <li> Each instance should now be simpler (since we have to solve more
     of them to establish the score). </li>
     <li> It appears, that now also all basic symmetries are broken? </li>
    </ol>
    Such generators shall go to module Satisfiability/Transformers/Generators.
   </li>
   <li> Active clauses and active clause-sets? </li>
  </ul>


  \todo Special algorithms
  <ul>
   <li> The brute-force search through all 2^(d^2) colourings should be
   implemented:
    <ol>
     <li> Input a graph. </li>
     <li> Run through all 0-1-assignments to the vertices, eliminate all
     edges having different colours, and determine the number of connected
     components. </li>
     <li> Report for each score how often it has been achieved. </li>
    </ol>
   </li>
  </ul>


  \todo Generalisation for arbitrary graphs
  <ul>
   <li> Instead of the chessboard-graphs G_d we can allow arbitrary graphs
   as inputs. </li>
   <li> The decision problem, now with two inputs G and k, should be
   NP-complete. </li>
  </ul>

*/

