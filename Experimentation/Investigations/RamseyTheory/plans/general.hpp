// Oliver Kullmann, 14.11.2008 (Swansea)
/* Copyright 2008, 2009, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/RamseyTheory/plans/general.hpp
  \brief General plans regarding investigations on %Ramsey theory (%Ramsey problems, van der Waerden problems, etc.)
  
  
  \todo Create milestones.


  \todo Connections
  <ul>
   <li> For colouring problems in general see
   Experimentation/Investigations/Colouring/plans/general.hpp. </li>
  </ul>


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
   <li> We have |E_t^N| = sum_{i=0}^{t-1} binomial(t,i) * N^i, where i stands
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
   <li> There is a project about Hales-Jewett numbers:
   http://michaelnielsen.org/polymath1/index.php
    <ol>
     <li> So there is actually considerable interest in computing Hales-Jewett
     numbers! </li>
     <li> We have http://www.math.ucsd.edu/~etressle/hj32.pdf, where
     halesjewett_2(3) = 4 is shown, directly with a proof by case distinctions,
     and mentioning also an algorithm. </li>
     <li> At
     http://michaelnielsen.org/polymath1/index.php?title=Hales-Jewett_theorem
     we find likely the most up-to-date bounds. </li>
     <li> halesjewett_r(3):
      <ol>
       <li> r=3: > 13 </li>
       <li> r=4: > 37 </li>
       <li> r=5: > 84 </li>
       <li> r=6: > 103 </li>
       <li> These case use vanderwaerden_r(3). </li>
      </ol>
     </li>
     <li> halesjewett_r(4):
      <ol>
       <li> r=2: > 11 </li>
       <li> r=3: > 97 </li>
       <li> r=4: > 349 </li>
       <li> r=5: > 751 </li>
       <li> r=6: > 3259 </li>
       <li> These case use vanderwaerden_r(4). </li>
      </ol>
     </li>
     <li> halesjewett_r(5):
      <ol>
       <li> r=2: > 59 </li>
       <li> r=3: > 302 </li>
       <li> r=4: > 2609 </li>
       <li> r=5: > 6011 </li>
       <li> r=6: > 14173 </li>
       <li> These case use vanderwaerden_r(5). </li>
      </ol>
     </li>
     <li> halesjewett_r(6):
      <ol>
       <li> r=2: > 226 </li>
       <li> r=3: > 1777 </li>
       <li> r=4: > 18061 </li>
       <li> r=5: > 49391 </li>
       <li> r=6: > 120097 </li>
       <li> These case use vanderwaerden_r(6). </li>
      </ol>
     </li>
     <li> halesjewett_r(7):
      <ol>
       <li> r=2: > 617 </li>
       <li> r=3: > 7309 </li>
       <li> r=4: > 64661 </li>
       <li> These case use vanderwaerden_r(7). </li>
      </ol>
     </li>
     <li> halesjewett_r(8):
      <ol>
       <li> r=2: > 1069 </li>
       <li> r=3: > 34057 </li>
       <li> These case use vanderwaerden_r(8). </li>
      </ol>
     </li>
     <li> halesjewett_r(9):
      <ol>
       <li> r=2: > 3389 </li>
       <li> These case use vanderwaerden_r(9). </li>
      </ol>
     </li>
     <li> Also the density considerations are of interest, since the
     hypergraphs sequences have the density property, i.e., the quotients
     alpha_halesjewett_hg(k,N)) / k^N -> 0 for fixed k. </li>
     <li> For k=3 these independence numbers are studied at
     http://michaelnielsen.org/polymath1/index.php?title=Upper_and_lower_bounds
     where c_n = alpha_halesjewett_hg(3,n)) is used. </li>
     <li> Precise values are known for 0 <= n <= 6: 1,2,6,18,52,150,450. </li>
     <li> A simple greedy algorithm is given there, but his works only up
     to n <= 3. </li>
     <li> Apparently their main algorithm is a genetic one:
     http://michaelnielsen.org/polymath1/index.php?title=Genetic_algorithm
     </li>
     <li> We must really work on hypergraph transversal algorithms! </li>
     <li> The general case is considered at
     http://michaelnielsen.org/polymath1/index.php?title=Higher-dimensional_DHJ_numbers
     where the notation c_{n,k} = alpha_halesjewett_hg(k,n)) is used. </li>
     <li> A simple upper bound is c_{n,k} <= (k-1)*k^{n-1}; parameter pairs
     where this upper bound is attained are called "saturated". </li>
    </ol>
   </li>
  </ul>

*/

