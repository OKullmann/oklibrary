// Oliver Kullmann, 17.10.2007 (Swansea)
/* Copyright 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Applications/CombinatorialOptimisation/plans/general.hpp
  \brief Plans regarding applications of (generalised) SAT to combinatorial optimisation


  \todo Update namespace


  \todo Write milestones


  \todo Travelling salesman problem
  <ul>
   <li> This seems to me the most widely explored topic of combinatorial
   optimisation, and thus we should connect to it.
    <ol>
     <li> It seems also that since 2000 or so there is stagnation in
     the field, at least w.r.t. algorithms and implementations, and
     perhaps something can be done here. </li>
    </ol>
   </li>
   <li> Under Combinatorics/Graphs we should consider generic algorithms
   (and/or links to respective software). </li>
   <li> One would guess that SAT should be most likely to succeed on
   non-geometric and exact TSP's.
    <ol>
     <li> One could run first a "normal" TSP algorithm to obtain a good
     approximation, and then try to show that this is optimal (or not). </li>
     <li> Interesting to consider how to embed geometric information into SAT,
     but likely this is not so easy. </li>
    </ol>
   </li>
   <li> Methods for employing SAT:
    <ol>
     <li> Of course, various direct translations need to be provided. </li>
     <li> And then active clauses and active clause-sets need to be
     explored. </li>
    </ol>
   </li>
   <li> Connect to the module Applications/Embeddings/HamiltonianPaths:
    <ol>
     <li> See HamiltonianPaths/plans/general.hpp ("TSP"). </li>
     <li> It might be that the "numerical treatment" via large weights
     is not optimal. </li>
    </ol>
   </li>
   <li> We should also explores the use of ordinal numbers in heuristics
   for the TSP as proposed by AB. </li>
  </ul>

*/

