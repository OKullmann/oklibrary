// Matthew Gwynne, 27.11.2008 (Swansea)
/* Copyright 2008, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/Generators/RamseyTheory/docus/RamseyProblems.hpp
  \brief How to use the %Ramsey problem generators


  <h1> %Ramsey problems via Maxima in the OKlibrary </h1>


  <h2> Generating additional clauses for symmetry breaking </h2>

  <ul>
   <li> One may generate a symmetry breaking partial assignment for a given
   %Ramsey problem of the form "R(q,q;2) > n" in the following way : 
   \verbatim
RSP_pass : ramsey_symbr1_pass(q,n);
   \endverbatim
   </li>
   <li> One can also generate additional clauses for symmetry breaking of a 
   given %Ramsey problem of the form "R(q,q;2) > n" in the following way :
   \verbatim
RSB : ramsey_symbr2_cs(n);
   \endverbatim
   </li>
   <li> The above assumes that variables are given in the form "colv({a,b})" 
   for edges connecting vertices "a" and "b". This need not be the case, and
   then the second and third argument to the function are functions mapping
   from the undirected edges (%e.g., {a,b}) to their corresponding undirected
   edges, and vice versa. </li>
   <li> The reason for the above notation ("colv({a,b})") etc. in the example 
   above is that the result of this can easily be converted to the format "aSb"
   sometimes used when generating clause-sets for the %Ramsey problems in the 
   extended Dimacs format. This can be done like so :
   \verbatim
RSB_e : map(lambda([a], map(lambda([b], sconcat(if sign(b) = neg then "-" else "",listify(args(abs(b))[1])[1],"S",listify(args(abs(b))[1])[2])),a)), RSB);
   \endverbatim
   </li>
   <li> For a generated clause-set, the following code may be used to output
   the clause-set in extended Dimacs format: 
   \verbatim
colv2str(x) := block([vals_abs],
  vals_abs : listify(args(abs(x))[1]),
  return(sconcat(if sign(x) = neg then "-" else "",
      vals_abs[1], "S", vals_abs[2])))$

output_cs_ef(n,F,f) := 
  with_stdout(n, block(
    print(sconcat("p ", length(var_cs(F)), " ",length(F))),
    for C in F do block([C_str],
      C_str : uaapply(sconcat, listify(
          map(lambda([a], sconcat(f(a)," ")), C))),
      C_str : sconcat(C_str, " 0"),
      print(C_str))))$

output_cs_ef(filename, clause_set, colv2str)$
   \endverbatim
   </li>  
  </ul>
*/
