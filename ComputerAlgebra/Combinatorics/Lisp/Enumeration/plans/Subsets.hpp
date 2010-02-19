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


  \todo Completing the basic functionality
  <ul>
   <li> DONE Also the counting-functions need testing. </li>
   <li> DONE Should it be "ksubsets", or just "subsets"? (Later we will
   actually enumerate all subsets, so perhaps "ksubsets" is better.) </li>
   <li> Write a basic docus. </li>
   <li> The explanations from "Enumerating all k-subsets lexicographically"
   need to be transferred to the docus. </li>
  </ul>


  \todo Enumerating all k-subsets lexicographically : DONE
  <ul>
   <li> Given natural numbers n, k >= 0, the task is to enumerate the set
   binomial({1,...,n},k) of all k-subsets of {1,...,n}. (See "Main concepts"
   in ComputerAlgebra/Combinatorics/Lisp/Enumeration/plans/general.hpp.) </li>
   <li> We have lexicographical and colexicographical ordering. </li>
   <li> More precisely, one considers lists of integers, ordered
   lexicographically, and for the lexicographical ordering of subsets
   the subset is transformed into a list in ascending order, while
   for the colexicographical order descending order is used. </li>
   <li> The lexicographical order is realised by
   \verbatim
listify(powerset(setn(n),k))
   \endverbatim
   (however this is obviously very inefficient, since it needs to
   enumerate always *all* subsets, and this at once). </li>
   <li> Another, fundamental, problem here is that apparently neither
   is the order of elements of a set specified, nor can it be
   controlled --- this issue must be raised at the Maxima mailing
   list! (It renders the set-concept useless.) </li>
   <li> For a given Maxima-set M, and k >= 0, the list of all k-subsets
   of M in lexicographical order is computed as follows:
   \verbatim
lex_ksubsets_l(M,k) := if k=0 then [{}] elseif emptyp(M) then [] else
  block([x : first_element(M), R],
    R : disjoin(x,M),
    append(add_element_l(x,lex_ksubsets_l(R,k-1)), lex_ksubsets_l(R,k)))$
   \endverbatim
   </li>
   <li> And colexicographical order:
   \verbatim
colex_ksubsets_l(M,k) := if k=0 then [{}] elseif emptyp(M) then [] else
  block([x : last_element(M), R],
    R : disjoin(x,M),
    append(colex_ksubsets_l(R,k), add_element_l(x,colex_ksubsets_l(R,k-1))))$
   \endverbatim
   </li>
   <li> Regarding ranking, for colex-order we restrict attention to
   the base-set {1, ..., n} (that is, S is a subset of {1, ..., n}):
   \verbatim
rank_colex_ksubsets(S) := block([L : listify(S)],
  sum_l(create_list(binomial(L[i]-1,i), i,1,length(L))) + 1)$
   \endverbatim
   (note that the rank w.r.t. colex-order does not depend on n).
   </li>
   <li> This formula comes from considering how many subsets are before
   S in the order: Let S = {v_1, ..., v_k} (k = length(S)) be sorted
   in ascending order. Now the elements before S are those k-subsets with
   elements all smaller than v_k, these are binomial(v_k-1,k), plus the
   k-subsets containing v_k as last element, and without this last element
   being before {v_1, ..., v_{k-1}}. The formula is obtained then by
   recursion. </li>
   <li> Now ranking for lex-order:
   \verbatim
rank_lex_ksubsets(S,n) := block([L : listify(S), k : length(S)],
  binomial(n,k) - sum_l(create_list(binomial(n-L[i],k-i+1), i,1,k)))$
   \endverbatim
   </li>
   <li> This formula comes from considering how many subsets are *after*
   S in the order: Let S = {v_1, ..., v_k} (as above). The elements after S
   have either every element larger than v_1, these are binomial(n-v_1,k),
   plus the k-subsets containing v_1 as first element, which without
   this element are after {v_2, ..., v_k}. Via recursion then the
   formula is obtained. </li>
   <li> The inverse of rank_colex_ksubsets is the following function:
   \verbatim
unrank_colex_ksubsets(x,n,k) := block([S : [], L : n],
  x : x - 1,
  for i : k thru 1 step -1 do (
    while binomial(L-1,i) > x do L : L - 1,
    S : cons(L,S), x : x - binomial(L-1, i)
  ),
  return(setify(S)))$
   \endverbatim
   By definition we have
   rank_colex_ksubsets(unrank_colex_ksubsets(x,n,k),n) = x
   for 1 <= x <= binomial(n,k). </li>
   <li> The inverse of rank_lex_ksubsets is the following function:
   \verbatim
unrank_lex_ksubsets(x,n,k) := block([S : [], L : 1],
  x : binomial(n,k) - x,
  for i : 1 thru k do block([j : k-i+1],
    while binomial(n-L, j) > x do L : L + 1,
    S : endcons(L,S), x : x - binomial(n-L, j)
  ),
  return(setify(S)))$
   \endverbatim
   By definition we have
   rank_lex_ksubsets(unrank_lex_ksubsets(x,n,k),n) = x
   for 1 <= x <= binomial(n,k).
   </li>
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

