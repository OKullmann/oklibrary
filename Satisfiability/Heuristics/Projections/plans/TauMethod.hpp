// Oliver Kullmann, 16.6.2007 (Swansea)
/* Copyright 2007, 2013, 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/Heuristics/Projections/plans/TauMethod.hpp
  \brief Plans regarding the tau-projection


  \todo Connections
  <ul>
   <li> See
   Satisfiability/Transformers/Generators/Queens/GenericSAT/BranchingTuples.hpp
   for currently the most up-to-date implementations. </li>
   <li> See Satisfiability/Solvers/TawSolver/plans/general.hpp for experiments
   and implementations with the tawSolver. </li>
  </ul>


  \todo Efficient computation of the tau-function:
  <ul>
   <li> Various versions:
    <ol>
     <li> Two arguments. </li>
     <li> At most three arguments. </li>
     <li> At most k arguments (k constant; using metaprogramming). </li>
     <li> Arbitrary many arguments. </li>
    </ol>
   </li>
   <li> Using tables should speed up the computation. </li>
  </ul>


  \todo Experimentation
  <ul>
   <li> More experimentation about the difference between tau with
   2 arguments and the product. </li>
   <li> For the OKsolver we didn't get a decrease in node-count, but for the
   tawSolver on the vdW-instances we get one. </li>
  </ul>


  \todo Decision
  <ul>
   <li> For theoretical purposes we should also implement a decision
   algorithm for comparing tau-values of branching tuples. </li>
  </ul>


  \todo Theory
  <ul>
   <li> First collect all the tau-theory from the
   [Kullmann/Luckhardt]-papers. </li>
  </ul>

*/
