// Oliver Kullmann, 14.11.2008 (Swansea)
/* Copyright 2008, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/plans/general.hpp
  \brief General plans regarding investigations on %Ramsey theory (%Ramsey problems, van der Waerden problems, etc.)
  
  
  \todo Create milestones.


  \todo Hales-Jewett problems
  <ul>
   <li> The length k of arithmetic progressions is now called t. </li>
   <li> And N now stands for the "dimension" of the set of vertices, while
   its size is n = t^N. </li>
   <li> And the vertices are not arbitrary elements but the elements of
   V_t^N := {1,...,t}^N. </li>
   <li> Instead of an arithmetic progression now "lines" are considered,
   which are t-tuples of elements of V such that for each coordinate we
   have a possibly degenerated (ascending) arithmetic progression
   ("degenerated" allows slope 0), where for at least one coordinate the
   arithmetic progression (which must be just (1,...,t)) is non-degenerated.
   </li>
   <li> So the hypergraphs are (V_t^N, E_t^N), where the hyperedges are
   the t-subsets of V such that an ordering exists making this subset
   to a "line" (such an ordering is then unique). </li>
   <li> We have |E_t^N| = sum_{i=0}^{t-1} binom(t,i) * N^i, where i stands
   for the number of degeneration-coordinates. </li>
   <li> The Hales-Jewett theorem now asserts the existence of
   halesjewett_r(t) = N, so that N' >= N is equivalent to the hypergraph
   (V_t^N, E_t^N) not being r-colourable. </li>
   <li> We have vanderwaerden_r(t) <= t^halesjewett_r(t), since using the
   bijection from {1,...,n} to V_t^N given by interpreting the elements
   of V_t^n as base-t-representation of natural numbers, but where we
   have to subtract 1 from each such digit, lines yield special arithmetic
   progressions. </li>
   <li> It seems not possible to create natural "mixed forms", since for
   different line-lengths t we have to use different vertices (namely tuples
   over {1,..,t}). </li>
   <li> On the other hand, using the notion of arithmetic progression as
   we did it, one could for example consider arithmetic progressions in
   a base set {1,...,T} with T = max {t_1,...,t_r} of slope 0 or 1 (i.e.,
   in each coordinate we must have such an arithmetic progression, and where
   at least for one coordinate the slope is 1). </li>
   <li> There is a generalisation halesjewett_r^d(t), where d=1 for the above
   form, and where instead of lines d-dimensional "subspaces" are considered.
   <li>
   <li> It is known that halesjewett_r(2) = r. </li>
   <li> Likely we should create a new module
   ComputerAlgebra/RamseyTheory/Lisp/HalesJewett. </li>
  </ul>

*/

