// Oliver Kullmann, 12.2.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/ConflictCombinatorics/plans/general.hpp
  \brief Plans for Maxima-components related to conflict-patterns of clause-sets


  \todo Create milestones


  \todo Write tests


  \todo Write docus


  \todo Conflict graph
  <ul>
   <li> Write gcg_lcs(FF), the general conflict graph of a labelled clause-set.
    <ol>
     <li> Edge-labels are [[C,D],v], where C,D are clause-labels and v a
     variable, such that the clauses clash in literal v. </li>
    </ol>
   </li>
   <li> Since gcg is a functor, also write gcg_homfcs(f), the mapping for
   homomorphisms f.
    <ol>
     <li> Since f doesn't know its domain and codomain, perhaps they need to
     be supplied additionally. </li>
    </ol>
   </li>
   <li> Analogously, write gcdg_lcl(FF), the general conflict digraph of a
   labelled clause-set, and gcdg_homfcs(f). </li>
   <li> An important topic is how to "realise" (directed/undirected) conflict
   graphs; perhaps for special methods we have "Realisations.mac" ? </li>
  </ul>


  \todo Hermitian rank
  <ul>
   <li> DONE It seems we should use hermitian_rank_charpoly instead of
   hermitian_rank for hermitian_rank_cs. </li>
   <li> Computation of the characteristic polynomial of a clause-set:
    <ol>
     <li> What is the characteristic polynomial of the empty square matrix?
     </li>
    </ol>
   </li>
   <li> Of course, there are many graphs associated with a clause-set,
   but it seems that only the characteristic polynomial of the conflict
   multigraph is of interest?
    <ol>
     <li> So that we can speak of "the" characteristic polynomial? </li>
     <li> Or should we speak of the "characteristic conflict-polynomial"? </li>
    </ol>
   </li>
   <li> What is the meaning of charpoly_cs(F) ?
    <ol>
     <li> Of course, the roots of charpoly_cs(F) have some meaning. </li>
     <li> But what about the numeric values of the coefficients? </li>
    </ol>
   </li>
  </ul>


  \todo Relations to other modules
  <ul>
   <li> See Experimentation/Investigations/plans/general.hpp. </li>
   <li> See ComputerAlgebra/Graphs/Lisp/BicliquePartitions/plans/general.hpp.
   </li>
  </ul>


  \todo mcind_cs, cind_cs:
  <ul>
   <li> DONE The naming should reflect the special method used (here using
   transversals). </li>
   <li> Likely the best is just to refer to all possible ways of
   computing maximal independent sets of a hypergraph (or, of a graph;
   likely one can take advantage of this more special situation). </li>
   <li> It seems not to be worth to develop an algorithm which computes
   directly all independent sets? </li>
  </ul>
  
*/

