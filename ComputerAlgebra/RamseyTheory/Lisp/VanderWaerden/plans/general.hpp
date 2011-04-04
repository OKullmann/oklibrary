// Oliver Kullmann, 20.9.2008 (Swansea)
/* Copyright 2008, 2009, 2010, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/RamseyTheory/Lisp/VanderWaerden/plans/general.hpp
  \brief Plans regarding van der Waerden problems


  \todo Relations to other modules
  <ul>
   <li> See ComputerAlgebra/Hypergraphs/Lisp/plans/Generators.hpp for
   the function ramsey_hg(q,r,n) and its variations. </li>
   <li> See
   ComputerAlgebra/Satisfiability/Lisp/Generators/RamseyTheory/plans/VanderWaerdenProblems.hpp
   for the SAT translation. </li>
   <li> See
   Experimentation/Investigations/RamseyTheory/VanderWaerdenProblems/plans/general.hpp
   for investigations. </li>
  </ul>


  \todo Handling of duplicate entries for vdW-numbers
  <ul>
   <li> The functions (pd)vanderwaerdenxk, (pd)vanderwaerdend2 etc. overlap
   in their stored values. </li>
   <li> Perhaps it is best to remove these duplications. </li>
   <li> On the other hand, it might be a burden to plan for all cases, and
   one might just write a test which checks for consistency. </li>
  </ul>


  \todo Lower bounds
  <ul>
   <li> See "Lower bounds" in
   Investigations/RamseyTheory/VanderWaerdenProblems/plans/general.hpp. </li>
  </ul>


  \todo Considering finite commutative groups G
  <ul>
   <li> For a natural number k >= 1 assume that every non-null element of G
   has order at least k. Then one can define (non-degenerated) arithmetic
   progressions of length k as sets {a + i*d : 0 <= i < k} for a, d in G,
   d <> 0. <li>
   <li> Let the hypergraph arithprog_hg_cgrp(k,G) have vertices the elements
   of G, while the hyperedges are the arithmetic progressions of length k.
   </li>
   <li> So for a fixed G one can consider the transversal- and independence
   number of arithprog_hg_cgrp(k,G) as well as the chromatic number. </li>
   <li> In order to consider sequences, one possibility is to consider powers
   G^n for some fixed G. </li>
   <li> It seems that in this context only the transversal- and independence
   numbers are considered; however we should also consider the chromatic
   number. For a given minimal order mo(G) of the non-null elements we
   can consider all 1 <= k <= mo(G). </li>
   <li> For G = ZZ_n we can also define vanderwaerden_m^mod(k_1, ..., k_m)
   as the smallest n >= max(k_1,..,k_m) such that for every m-labelling
   there exists i such that colour-class i is independent w.r.t.
   k_i-progressions. </li>
   <li> If G is infinite, then we can consider an enumeration of the
   elements, obtaining a generalisation of vanderwaerden_m(k_1,...,k_m)
   in this way (allowing then for given n only arithmetic progressions
   which lie inside the first n elements). </li>
   <li> Apparently alpha_arithprog_hg_cgrp(k,G) and especially
   n -> alpha_arithprog_hg_cgrp(k,G^n) are only considered in the literature.
   </li>
   <li> And this especially for G in {ZZ_3, ZZ_5}. </li>
  </ul>


  \todo Automorphisms of van-der-Waerden hypergraphs
  <ul>
   <li> We need to investigate experimentally the symmetries of these
   hypergraphs. </li>
  </ul>


  \todo Automorphisms of van-der-Waerden clause-sets
  <ul>
   <li> The obvious automorphisms of diagonal van-der-Waerden clause-sets are
   given by the inner product of automorphisms for the underlying
   van-der-Waerden hypergraph and the S_m when using m parts. </li>
   <li> Are there more? </li>
  </ul>

*/

