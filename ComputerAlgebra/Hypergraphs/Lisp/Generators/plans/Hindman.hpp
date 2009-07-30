// Oliver Kullmann, 12.7.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Hypergraphs/Lisp/Generators/plans/Hindman.hpp
  \brief Plans regarding hypergraphs generators related to Hindman's theorems and conjectures


  \todo Update names
  <ul>
   <li> The current "quad"-forms should be replaced by
   hindman_k2_ohg(a,n) and hindmani_k2_ohg(a,n) (for x <> y). </li>
   <li> Actually, the missing a-parameter should be indicated by "a1". </li>
  </ul>


  \todo Implement general functionality (arbitrary k)
  <ul>
   <li> hindman_ohg_0(a,k,n) and hindmani_ohg_0(a,k,n) need to improved.
   </li>
   <li> Not all tuples [x_1, ..., x_k] have their sums and products
   in {1, ..., n} --- actually most won't.
    <ol>
     <li> Perhaps we solve first the problem with "all products" (only;
     see 'Implement "all products"' below), since except of trivial
     cases the product dominates the sum. </li>
     <li> And then we add the summands to the hyperedges, filtering out
     the unsuitable ones. </li>
    </ol>
   </li>
   <li> stable_unique is rather slow. </li>
   <li> DONE (achieved by all_ord_tuples)
   We need a function producing all tuples [x_1, ..., x_k]
   over {1, ..., m} with x_1 <= ... <= x_k (while x_1 < ... < x_k
   is obtained by computing all k-subsets and listifying the result).
    <ol>
     <li> Simplest is to compute all k-tuples, and then to sort the
     results. </li>
     <li> This is obtained by
     setify(map(sort,all_tuples_l(setn(m),k))). </li>
     <li> This is wasteful, but a simple solution, and it yields the
     canonical order. </li>
    </ol>
   </li>
   <li> DONE
   A simple first implementation ignores all efficiency considerations.
    <ol>
     <li> First a "generic" hyperedge is constructed, e.g. for k=3 the
     hyperedge
     {x1,x2,x3,x1+x2,x1+x3,x2+x3,x1+x2+x3,x1*x2,x1*x3,x2*x3,x1*x2*x3}. </li>
     <li> This is likely best achieved as a union of the sums-only and the
     products-only case:
     \verbatim
union(
  map(sum_l,map(listify,disjoin({},powerset({x1,x2,x3})))),
  map(prod_l,map(listify,disjoin({},powerset({x1,x2,x3}))))
);
  {x1,x2,x1*x2,x2+x1,x3,x1*x3,x3+x1,x2*x3,x3+x2,x1*x2*x3,x3+x2+x1}
     \endverbatim
     </li>
     <li> Then [x1,x2,x3] runs through all possibilities, and the substitution
     is performed. </li>
    </ol>
   </li>
  </ul>


  \todo Implement "all products"
  <ul>
   <li> As with Folkman-hypergraphs, but now using "all products" instead of
   "all sums". </li>
  </ul>

*/

