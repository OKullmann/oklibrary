// Oliver Kullmann, 27.7.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/RamseyTheory/Lisp/Ramsey/plans/general.hpp
  \brief Plans regarding Ramsey problems


  \todo
  <ul>
   <li> See ComputerAlgebra/Hypergraphs/Lisp/plans/Generators.hpp for
   the function ramsey_hg(q,r,n) and its variations. </li>
   <li> See ComputerAlgebra/Satisfiability/Lisp/Generators/plans/general.hpp
   for the SAT translation. </li>
   <li> See Experimentation/Investigations/plans/RamseyProblems.hpp for
   investigations. </li>
  </ul>


  \todo Automorphisms of Ramsey hypergraphs
  <ul>
   <li> First the natural operation of S_n on ramsey_hg(q,r,n) needs to
   be made available.
    <ol>
     <li> Implemented in
     ComputerAlgebra/RamseyTheory/Lisp/Ramsey/Hypergraphs.mac. </li>
     <li> Easiest first to consider the elements of S_n as lists. </li>
    </ol>
   </li>
   <li> The operation of S_n should be faithful (in most cases), and we
   need to extract the induced automorphisms of ramsey_hg(q,r,n). </li>
   <li> Then we need to find out whether these automorphisms cover all.
    <ol>
     <li> For small parameters values (including q=3,r=2) we can
     enumerate all automorphisms by brute force. </li>
    </ol>
   </li>
  </ul>


  \todo Automorphisms of Ramsey clause-sets
  <ul>
   <li> The obvious automorphisms of diagonal Ramsey clause-sets are
   given by the inner product of automorphisms for the underlying Ramsey
   hypergraph and the S_s when using s colours. </li>
   <li> Are there more?
    <ol>
     <li> It seems that by brute force (enumerating all clause-set
     automorphisms we cannot process any non-trivial example? </li>
     <li> We need stronger tools for computing automorphisms of clause-sets.
     </li>
    </ol>
   </li>
   <li> And what about non-diagonal Ramsey clause-sets? One would guess
   that in "most" cases there are no automorphisms?? </li>
  </ul>


  \todo Ramsey numbers
  <ul>
   <li> In RamseyTheory/Lisp/Ramsey/Numbers.mac we need to provide everything
   known about Ramsey numbers. </li>
   <li> A "Ramsey parameter tuple" is a tupe [[q_1,...,q_s],r], where s
   is the number of colours (all q_i and r are natural numbers). </li>
   <li> The function ramsey_number(par_tuple) computes a pair, consisting of
   a lower and an upper bound on the Ramsey number for this tuple. </li>

*/

