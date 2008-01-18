// Oliver Kullmann, 21.10.2007 (Swansea)
/* Copyright 2007, 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/Enumeration/plans/general.hpp
  \brief Plans on the supermodule on counting, enumerating and sampling SAT problems

  The typical problem are, given a class C of problem instances (finite, depending
  on some parameters):
  <ul>
   <li> Determine |C|, the size of the class (precisely, approximately,
   asymptotically). </li>
   <li> Enumerate the elements of C by an "efficient" algorithm. </li>
   <li> Find an "efficient" sampling procedure for C. </li>
  </ul>
  Given some C, typically by some form of syntax description, one obtains
  refined C' via for example
  <ul>
   <li> Considering the quotient of C by the automorphism group. </li>
   <li> Considering only reduced elements of C w.r.t. some reduction r
   (for example subsumption-elimination). </li>
   <li> Considering only unsatisfiable, satisfiable or minimally unsatisfiable
   elements. </li>
   <li> Considering the quotient of C by equivalence. </li>
  </ul>
  If feasible, catalogues can be created, which yield test examples for solvers
  and conjectures.


  \todo Update namespaces.


  \todo Create milestones.


  \todo Links to other modules
  <ul>
   <li> Isomorphisms of clause-sets are relevant here. </li>
   <li> And considerations regarding random clause-sets. </li>
   <li> The computer algebra system is a natural environment at least for the
   counting-formulas. </li>
  </ul>


  \todo CLS(n)
  <ul>
   <li> CLS(n) is the class of all clause-sets F with var(F) <= {1, ..., n}. </li>
   <li> |CLS(n)| = 2^(3^n), while the quotient modulo equivalence has 2^(2^n)
   elements. </li>
   <li> For all practical considerations it's best to consider CLS'(n), where
   var(F) = {1, ..., n}. </li>
   <li> Enumeration of CLS'(n) should be simple; one can consider some form of
   lexicographical order. </li>
   <li> Also sampling of CLS'(n) should be easy. </li>
   <li> Is efficient sampling of CLS'(n) / Aut (the automorphism group) possible? </li>
   <li> The trivial catalogues of CLS'(n)/Aut:
    <ol>
     <li> n = 0 : {}, {{}} </li>
     <li> n = 1 : {{1}}, {{1},{-1}} </li>
    </ol>
    n = 2 could still be done by hand, n = 3 by a program.
   </li>
   <li> What can be said about the sizes of the orbits of the operation of
   Aut on CLS'(n) ? </li>
   <li> Subsumption-free non-isomorphic elements of CLS'(n):
    <ol>
     <li> n = 0,1 as above. </li>
     <li> n = 2 : still somewhat ugly to do by hand. </li>
     <li> Is a catalogue feasible for n = 4 ?? Likely not. </li>
    </ol>
   </li>
   <li> And minimally unsatisfiable non-isomorphic elements of CLS'(n) ?
    <ol>
     <li> n = 0 : {{}} </li>
     <li> n = 1 : {{1},{-1}} </li>
     <li> n = 2 : {{1},{-1,-2},{2}},
                  {{1},{-1,-2},{-1,2}},
                  {{-1,-2},{-1,2},{1,2},{1,-2}}
     </li>
     <li> Something has been done by the Kleine-Buening-group. </li>
     <li> Can here CLS'(4) be catalogised? </li>
    </ol>
   </li>
  </ul>


  \todo CLS(n, c)
  <ul>
   <li> CLS(n) is the class of all clause-sets F with var(F) <= {1, ..., n}
   and c(F) = c. </li>
   <li> Sampling is basically the model with probability 1/3 to include a
   literal in a clause. </li>
  </ul>


  \todo CLS(n, c, k)
  <ul>
   <li> CLS(n) is the class of all clause-sets F with var(F) <= {1, ..., n}
   and c(F) = c, where for C in F we have |C| = k. </li>
   <li> Sampling is basically the constant-clause-length model. </li>
   <li> There are formulas in the Mupad-module. </li>
  </ul>
  
*/

