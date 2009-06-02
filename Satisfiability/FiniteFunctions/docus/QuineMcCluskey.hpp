// Matthew Gwynne, 2.6.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/FiniteFunctions/docus/QuineMcCluskey.hpp
  \brief Docus for the QuineMcCluskey implementation for finding prime implicates.


  <h1> Overview </h1>
  
  QuineMcCluskey provides a tool for generating all prime implicates (all 
  clauses which follow from the original clause set but for which no 
  smaller clause follows) of a given clause set.


  <h1> Input and Output </h1>

  The only argument to any of the QuineMcCluskey versions is the filepath
  to the input clauseset. Output is given on STDOUT.

  <ul>
   <li> <em> Input:</em> A CNF or DNF clauseset of fixed variable size in
   Dimacs format. </li>
   <li> <em> Output:</em> All prime implicates of the input clause set, in
   Pseudo-Dimacs format (i.e excluding any "p" and "c" lines). </li>
  </ul>


  <h1> Versions </h1>
  
  There are four versions of the QuineMcCluskey application built by default
  by the build system.

  <ul>
   <li> QuineMcCluskey-n4 allows a maximum of 4 variables. 
   </li>
   <li> QuineMcCluskey-n4-O3-DNDEBUG is the optimised version of 
   QuineMcCluskey-n4 . </li>
   <li> QuineMcCluskey-n16 allows a maximum of 16 variables. </li>
   <li> QuineMcCluskey-n16-O3-DNDEBUG is the optimised version of 
   QuineMcCluskey-n16. </li>
  </ul>

  One can build other versions of QuineMcCluskey allowing larger numbers of 
  variables by setting up a preprocessor variable when building like so:
  
  \verbatim
> oklib clean
> oklib all CXXFLAGS="-DNUMBER_VARIABLES=20"
  \endverbatim


  <h1> Example </h1>

  \verbatim
> echo -en "1 2 3 0\n1 2 -3 0\n" > test.cnf
> QuineMcCluskey-n4 test.cnf 
Number of Partial Assignments 81
Level 4
Level 3
Level 2
Level 1
2 1 0
  \endverbatim


  <h1> Algorithm </h1>

  High level algorithm :

  <ol>
   <li> Input is a clause-set F (either CNF or DNF). </li>
   <li> In general a "subsumption-resolution" is one resolution step where
   the resolvent subsumes at least one parent clause. A
   "2-subsumption-resolution" is where both parent clauses are subsumed
   (that is, iff both parent clauses are identical except of the clashing
   literal). </li>
   <li> Let k := n(F), F_k := F. </li>
   <li> While k > 0 do
    <ol>
     <li> F_{k-1} := all 2-subsumption-resolvents from F_k. </li>
     <li> Remove from F_k all clauses which were involved in a
     resolution; and furthermore check whether "by chance" a clause
     in F_k is otherwise subsumed by some clause in F_{k-1} (then that
     clause is also removed). </li>
    </ol>
   </li>
   <li> The union of F_n, ..., F_0 then is the set of prime implicates
   (for CNF) resp. prime implicants (for DNF), where F_k contains the
   clauses of length k. </li>
  </ol>

*/
