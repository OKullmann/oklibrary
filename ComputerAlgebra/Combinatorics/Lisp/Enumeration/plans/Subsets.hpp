// Oliver Kullmann, 6.1.2009 (Swansea)
/* Copyright 2009, 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Combinatorics/Lisp/Enumeration/plans/Subsets.hpp
  \brief Plans regarding enumerating subsets (all, or only specific ones)


  \todo Connections
  <ul>
   <li> Compare Combinatorics/Enumeration/plans/Subsets.hpp. </li>
  </ul>


  \todo Iteration through lexicographical order
  <ul>
   <li> State-free iteration for lexicographical order is given as follows:
   \verbatim
first_lex_ksubsets(n,k) := setn(k)$
next_lex_ksubsets(S,n) := block(
 [L : listify(S), l : length(S), i, prev : n+1],
  i : l-1,
  for x in reverse(L) do
    if x+1 < prev then 
      return(if i = -1 then done else
             setify(append(take_elements(i,L), create_list(x+k,k,1,l-i))))
    else (i : i-1, prev : x))$
   \endverbatim
   This algorithm is essentially the same as algorithm L in [Knuth, Vol. 4,
   Fascicle 3, Section 7.2.1.3]. </li>
   <li> Compare function Combinatorics::choose_next in
   General/Combinatorics.hpp. </li>
   <li> Usage example:
   \verbatim
block([x : first_lex_ksubsets(6,3)], 
  while x#done do (print(x), x : next_lex_ksubsets(x,6)));
   \endverbatim
   </li>
   <li> Colexicographical order: XXX </li>
   <li> (General) Iteration for lexicographical order XXX
    <ol>
     <li> The following needs updating according to
     ComputerAlgebra/Combinatorics/Lisp/Enumeration/plans/general.hpp. </li>
     <li> "itgen_lex_ksubsets(M,k) yields an iterator "it" "pointing" to
     the first element. </li>
     <li> "iteval_lex_ksubsets(it)" yields the element. </li>
     <li> "itend_lex_ksubsets(it) = it[1]" returns true iff it points "past the
     end". </li>
     <li> "itadv_lex_ksubsets(it)" advances a valid iterator (pointing to an
     element) one step, based on a shallow copy of it. </li>
     <li> Algorithm T in [Knuth, Vol. 4, Fascicle 3, Section 7.2.1.3]
     should yield an appropriate algorithm. </li>
     <li> An application example:
     \verbatim
block([it : itgen_lex_ksubsets(M,k)], while not itend_lex_ksubsets(it) do
  print(iteval_lex_ksubsets(it)), itadv_lex_ksubsets(it));
     \endverbatim
     </li>
    </ol>
   </li>
  </ul>


  \todo Improving colexicographical unranking
  <ul>
   <li> unrank_colex_ksubsets(x,n,k) does not really depend on n, it is only
   a large enough start value needed for L. </li>
   <li> It would be better if from x and k we could compute a good value n
   (and thus it wouldn't be needed as input). </li>
   <li> The smallest n is given by the condition x <= binomial(n,k). </li>
  </ul>


  \todo Enumerating all k-subsets in Gray-code manner
  <ul>
   <li> The point here is that only one element is changed at a time (when
   proceeding to the successor). </li>
   <li> There should be a standard, recursive way of achieving such an
   order. </li>
  </ul>

*/

