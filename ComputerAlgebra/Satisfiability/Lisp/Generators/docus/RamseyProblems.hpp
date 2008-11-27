// Matthew Gwynne, 27.11.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/Generators/docus/RamseyProblems.hpp
  \brief How to use the Ramsey problem generators


  <h1> Ramsey problems via Maxima in the OKlibrary </h1>


  <h2> Generating additional clauses for symmetry breaking </h2>

  <ul>
   <li> One can generate additional clauses for symmetry breaking of a given
   Ramsey problem of the form R(q,q;2) > n in the following way : 
   \verbatim
n : 6$
RSB : ramsey2_sym_break_rec(map(colv,powerset(setn(n),2)), lambda([a], colv(a)),lambda([a], args(a)[1]));   
   \endverbatim
   </li>
   <li> The above assumes that variables are given in the form "colv({a,b})" 
   for edges connecting vertices "a" and "b". This need not be the case, and the
   variables may be of any form (assuming they can be negated, and "abs" works 
   on their negation), and then the second and third argument to the function are
   functions mapping from the undirected edges (e.g {a,b}) to their corresponding 
   undirected edges, and vice versa. </li>
   <li> The reason for the above notation ("colv({a,b})") etc in the example 
   above is that the result of this can easily be converted to the format "aSb"
   sometimes used when generating clause sets for the Ramsey problems in the 
   extended Dimacs format. This can be done like so :
   \verbatim
RSB_e : map(lambda([a], map(lambda([b], sconcat(if sign(b) = neg then "-" else "",listify(args(abs(b))[1])[1],"S",listify(args(abs(b))[1])[2])),a)), RSB);
   \endverbatim
   </li>  
  </ul>
*/
