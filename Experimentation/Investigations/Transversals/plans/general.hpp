// Oliver Kullmann, 20.3.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/Transversals/plans/general.hpp
  \brief Plans on investigating transversals of hypergraphs


  \todo Connections
  <ul>
   <li> The transversal hypergraph of van-der-Waerden hypergraphs is
   investigated in
   Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/Transversals/plans/general.hpp
   </li>
  </ul>
  
  
  \todo Exact transversals of hypergraphs of partitions
  <ul>
   <li> It seems interesting to investigate the transversals of the
   set systems set_partitions(setn(n),k) (the partitions of {1,...,n}
   of size k). </li>
   <li> Yet, in the absence of an algorithm for computing all exact
   transversals (and in the presence of incorrect set_partitions):
   \verbatim
resimplify(expr):=expand(expr,1,1)$

etrans_part(n,k) := block([S : resimplify(set_partitions(setn(n),k)), T],
  T : transversal_ses_rs(S),
  return(subset(T, lambda([t], etransversal_p(t,S)))))$

etrans_fpart(n) := block([S : resimplify(set_partitions(setn(n))), T],
  T : transversal_ses_rs(S),
  return(subset(T, lambda([t], etransversal_p(t,S)))))$
   \endverbatim
   </li>
   <li> etrans_fpart(n) consists exactly of the n ultrafilters on {1,...,n},
   that is, for i in {1,...,n} the set systems consisting of {i} and its
   supersets. </li>
   <li> A question is whether etrans_part(n,3) contains other transversals.
   </li>
   <li> For n=4 we have one additional transversal in etrans_part(4,3),
   consisting of all six 2-element subsets. </li>
   <li> But for larger n we might only have the n trivial ultrafilter. </li>
   <li> Same question for k > 3. </li>
   <li> For k=2 there are more; can something be said about them? </li>
  </ul>

*/

