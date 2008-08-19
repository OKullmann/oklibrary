// Oliver Kullmann, 4.8.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/IncidenceStructures/Lisp/plans/general.hpp
  \brief Plans on incidence structures in general


  \todo Basic notions
  <ul>
   <li> In ComputerAlgebra/Sets we should have the notion of a
   "correspondence" (following Bourbaki), a triple [R,A,B], where
   A,B are sets and R is a subset of A x B. </li>
   <li> An "incidence structure" is now the version, where the set
   R is given implicitly, but with values 0,1, that is, an incidence
   structure is a triple [I,P,B], where P is the set of "points", B the
   set of "blocks", and I is a 0,1-valued function I(p,b). </li>
   <li> Abbreviation: "ics". </li>
   <li> So [I,P,B] is an incidence structure iff [P,B,I] is a combinatorial
   matrix over {0,1}; this is useful, however the concepts are different
   (so for example we have different morphisms). </li>
   <li> Natural representations are given by ics2ghg and ghg2ics. </li>
   <li> We should also introduce the notion of a "polar incidence structure",
   a pair [I,S] s.t. [I,S,S] is an incidence structure, and I is symmetric.
    <ol>
     <li> These are the structures which correspond to incidence structures
     together with a *given* polarity. </li>
     <li> Such polar incidence structures correspond 1-1 to graphs with loops
     (I is just the 0-1-representation of the the adjacency-relation), and they
     also have the same morphisms (so we have a non-full subcategory of the
     category of all incidence structures). </li>
     <li> See "Graphs as hypergraphs" in
     ComputerAlgebra/Graphs/Lisp/plans/general.hpp, and also
     ComputerAlgebra/Graphs/Lisp/StrongRegularity/plans/general.hpp. </li>
    </ol>
   </li>
   <li> Abbreviation: "pics". </li>
   <li> Natural representations are given by pics2pghg and pghg2pics, and by
   pics2gl and gl2pics. </li>
  </ul>


  \todo The main parameters
  <ul>
   <li> Apparently there is no useful notion for the main parameters, that
   "for t different points there are exactly lambda incident blocks". </li>
   <li> So we need to introduce it ourselves: An incidence structure [I,P,B]
   has *index lambda in NN_0 w.r.t. t points (t in NN_0)* if for each subset
   T of P of size t the subset of B of blocks incident with all elements of T
   has size lambda. </li>
   <li> The predicate is "balanced_ics_p(S,t,lambda)". </li>
  </ul>


  \todo Polar and bipolar incidence structures
  <ul>
   <li> A "bipolar incidence structure" is a pair [I,V] such that [I,V,V]
   is an incidence structure. </li>
   <li> A "polar incidence structure" is a bipolar incidence structure [I,V]
   such that I is symmetric. </li>
  </ul>

*/

