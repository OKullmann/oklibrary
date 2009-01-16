// Oliver Kullmann, 21.6.2008 (Swansea)
/* Copyright 2008, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Graphs/Lisp/BicliquePartitions/plans/Search.hpp
  \brief Plans regarding searching for (special) biclique partitions


  \todo The role of maximal bicliques
  <ul>
   <li> One process for finding a biclique partition is to choose
   a maximal biclique, remove it, and iterate this process. </li>
   <li> The basic question is, whether in this way we can always find
   an optimal biclique partition (w.r.t. minimal number of parts, and
   assuming the right choices). </li>
   <li> This statement is equivalent to the assertion that there always
   exists an optimal biqlique partition containing at least one maximal
   biclique. </li>
   <li> Also for graphs I (OK) don't see a direct way of proving this
   property, so I guess it is wrong. </li>
  </ul>


  \todo Finding interesting biclique partitions
  <ul>
   <li> For creating random biclique partitions, one goal is to use only
   a minimal number of bicliques (i.e., variables for the corresponding
   clause-sets). </li>
   <li> Really minimising it is interesting in its own right (see
   "Finding specific biclique partitions via SAT" above), and we could
   investigate whether there are special algorithms. </li>
   <li> Another approach is heuristical.
    <ol>
     <li> This would be of interest for creating random biclique partitions,
     since we could randomise the algorithm, and then obtain "random"
     biclique partitions with some special properties (like using only
     relatively few bicliques). </li>
    </ol>
   </li>
   <li> The first question to consider are heuristics for creating
   as few bicliques as possible.
    <ol>
     <li> The greedy approach is to find a first biclique as large as
     possible, remove it, and iterate. </li>
     <li> We should study reductions of the problem of finding biclique
     partitions to closely related other problems, so that then we can
     transfer techniques from these fields (see
     ComputerAlgebra/Graphs/Lisp/BicliquePartitions/plans/Transformations.hpp).
     </li>
    </ol>
   </li>
  </ul>

*/

