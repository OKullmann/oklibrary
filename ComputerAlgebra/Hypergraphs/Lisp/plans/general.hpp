// Oliver Kullmann, 30.11.2007 (Swansea)
/* Copyright 2007, 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Hypergraphs/Lisp/plans/general.hpp
  \brief Plans for Maxima-components for hypergraphs


  \todo Create milestones


  \todo Write tests


  \todo Write docus
  <ul>
   <li> We should have a list of all available functions. </li>
   <li> Seems that this list needs to be maintained manually? </li>
  </ul>


  \todo Set creation
  <ul>
   <li> It would be nice to have "create_set", as we have "create_list". </li>
   <li> However
   \verbatim
create_set([A]) := setify(apply(create_list,A))$
   \endverbatim
   does not work, since apply evaluates the argument A. </li>
   <li> It should be possible to make this work, but perhaps this is not 
   worth the effort? </li>
   <li> Ask on the Maxima mailing list. </li>
  </ul>


  \todo Hypergraphs/Lisp/Generators.mac
  <ul>
   <li> Create complete r-graphs. </li>
   <li> Create Sudoko-hypergraphs (such that the Sudoko-problems can be
   expressed as strong colouring problems).
    <ol>
     <li> For box dimension p the vertex set should be {1,...,p}^2. </li>
     <li> The 3N hyperedges (N=p^2) just correspond to the rows, columns
     and boxes (the hypergraph is thus N-uniform). </li>
    </ol>
   </li>
   <li> Create Ramsey graphs etc. </li>
  </ul>


  \todo Intersecting hypergraphs
  <ul>
   <li> See "Allowed parameter values" in
   ComputerAlgebra/Satisfiability/Lisp/ConflictCombinatorics/plans/HittingClauseSets.hpp
   for a motivation to study intersecting hypergraphs (no empty hyperedges,
   and every two hyperedges intersect). </li>
  </ul>

*/

