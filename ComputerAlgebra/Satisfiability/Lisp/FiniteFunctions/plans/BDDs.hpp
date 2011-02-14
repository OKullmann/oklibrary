// Oliver Kullmann, 14.2.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/FiniteFunctions/plans/BDDs.hpp
  \brief Plans regarding BDDs and variations


  \todo Connections
  <ul>
   <li> In Satisfiability/Lisp/FiniteFunctions/plans/BranchingPrograms.hpp
   we discuss branching programs. </li>
   <li> "BDD" is just another notion, however with a special connotation. </li>
   <li> We need to see how to organise the whole. </li>
  </ul>


  \todo Basic operations for boolean functions
  <ul>
   <li> We need the notion of a boolean function as a simple term (which can
   be put into a set). </li>
   <li> We actually need "formal boolean functions", which depend on a set of
   (named) variables; or perhaps just natural numbers are enough to represent
   variables. </li>
   <li> We need application of partial assignments to formal boolean functions.
   </li>
   <li> We need elimination of variables on which the formal boolean function
   does not depend. </li>
   <li> We need computation of forced assignments. </li>
  </ul>


  \todo OBDDs
  <ul>
   <li> The semantical representation:
    <ol>
     <li> Consider a formal boolean function f. </li>
     <li> Also consider a linear order L of the variables of f. </li>
     <li> The OBDD of f w.r.t. L should exactly correspond to the set of
     formal boolean functions
      <ol>
       <li> which are obtained from f by application of partial assignments
       whose domains are initial segments of L, </li>
       <li> and which depend on all their variables. </li>
      </ol>
     </li>
    </ol>
   </li>
   <li> We need to compute the semantical representation according to this
   description. It seems best to first get in the way the nodes of the
   ordinary OBDD, while we leave out the edges and their labels (which can be
   added later). </li>
  </ul>


  \todo Smurfs
  <ul>
   <li> http://www.cs.uc.edu/~weaversa/SBSAT.html is the homepage of SBSAT,
   which is based on "smurfs", a strong representation of boolean functions.
   </li>
   <li> The user manual is http://gauss.ececs.uc.edu/sbsat_user_manual/ . </li>
   <li> The semantical representation:
    <ol>
     <li> Consider a formal boolean function f. </li>
     <li> The smurf of f should exactly correspond to the set of
     formal boolean functions
      <ol>
       <li> which are obtained from f by application of (artitrary) partial
       assignments, </li>
       <li> and which depend on all their variables and have no forced
       assignments. </li>
      </ol>
     </li>
    </ol>
   </li>
   <li> We need to compute the semantical representation according to this
   description. It seems best to first get in the way the nodes of the
   smurf, while we leave out the edges and their labels (which can be added
   later). </li>
   <li> Another similar model is "Finite functions with full application of
   partial assignments" in
   Satisfiability/Lisp/FiniteFunctions/plans/general.hpp. </li>
  </ul>

*/


