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


  \todo mcind_cs, cind_cs:
  <ul>
   <li> DONE The naming should reflect the special method used (here using
   traversals). </li>
   <li> Likely the best is just to refer to all possible ways of
   computing maximal independent sets of a hypergraph (or, of a graph;
   likely one can take advantage of this more special situation). </li>
   <li> It seems not to be worth to develop an algorithm which computes
   directly all indepedent sets? </li>
  </ul>


  \todo Generators for hitting clause-sets
  <ul>
   <li> Implement the construction from Lemma 3 in [Savicky, Sgall, 2000,
   DNF tautologies with a limited number of occurrences of every variable],
   which from uniform unsatisfiable hitting clause-sets F,F' of rank k,k'
   construct a uniform unsatisfiable hitting clause-set of rank k * k'.
   Apparently, if F, F' are variable-uniform, then so is the result.
   </li>
   <li> Then, is in Theorem 4 there, apply this construction to sasg2000.
   </li>
   <li> There is also the important question for what k,r,n we have
   F with uuhittingcsp(F) which are k-uniform and r-variable-regular.
    <ol>
     <li> sasg2000 is an example for k=3,r=6,n=4. </li>
     <li> Ignoring r (i.e., admitting an arbitrary r) we arrive exactly at
     the notion of an "associative block design" (ABD). </li>
     <li> We are allowed to do so since r = 2^k * k / n. </li>
     <li> Apparently ABD's are denoted as "ABD(n,k)"). </li>
     <li> In order to improve the bound from [Savicky, Sgall] we do not
     require variable-regularity, but if we do so, then we are asking for
     k, n with log_k(n) > log_3(4). </li>
     <li> Apparently an ABD(8,5) exists which improves the bound. </li>
     <li> To establish s_k = Theta(2^k / k) for the function s_k introduced
     by [Kratochvil, Savicky, Tuza, 1993], which is best possible, we need to
     find an example with n = k^2. </li>
     <li> This seems very demanding --- perhaps it is essential to give
     up variable-regularity for s_k. But we need to search the literature!
     </li>
     <li> On the other side, demanding variable-regularity is sensible,
     and we arrive at a somewhat different question. </li>
    </ol>
   </li>
   <li> See [Hoory, Szeider, 2006]. </li>
   <li> Are there other generators for unsatisfiable hitting clause-sets? </li>
  </ul>
*/

