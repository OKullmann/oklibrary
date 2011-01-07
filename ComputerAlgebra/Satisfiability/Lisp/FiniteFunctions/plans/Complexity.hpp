// Oliver Kullmann, 7.1.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/FiniteFunctions/plans/Complexity.hpp
  \brief Plans regarding complexity measurement of boolean functions (and generalisations)


  \todo Complexity measurements
  <ul>
   <li> For a boolean function f (or a finite function) an important task
   is to measure the complexity mu(f) w.r.t. some measure mu, which typically
   measures the size of some representation of f. </li>
   <li> Important examples w.r.t. clause-representations:
    <ol>
     <li> number of prime implicates and prime implicants </li>
     <li> more generally, the minimum number of clauses of an r_k-base of the
     set of prime implicates (also accordingly for the prime implicants) </li>
     <li> the minimum number of clauses in a CNF or DNF representation (
     included in the above for k large enough) </li>
     <li> the minimum size of a hitting CNF/DNF </li>
     <li> the minimum size of tree-hitting CNF/DNF. </li>
    </ol>
    One can also consider to minimise the sum of these CNF/DNF pairs.
   </li>
   <li> Important examples w.r.t. circuit-representations:
    <ol>
     <li> the minimum number of nodes in a representation w.r.t. some
     base </li>
     <li> important examples are the boolean base {not,and,or} and the full
     binary base (all 16 binary functions). </li>
    </ol>
   </li>
  </ul>


  \todo Complexity functions in dependency on n
  <ul>
   <li> For each complexity measure mu(f) of a boolean function we should
   consider the natural number mu(n), this maximum of mu(f) where f is a
   boolean function with n inputs. </li>
   <li> We need naming schemes. </li>
   <li> We need systems for handling concrete values (including upper and lower
   bounds). </li>
   <li> We need to incorporate general bounds. </li>
   <li> We should also have information on the boolean function which attain
   the maximum. </li>
  </ul>

*/
