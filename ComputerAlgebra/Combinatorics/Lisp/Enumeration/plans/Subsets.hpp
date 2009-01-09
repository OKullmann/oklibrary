// Oliver Kullmann, 6.1.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
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


  \todo Enumerating all k-subsets lexicographically
  <ul>
   <li> Given natural numbers n, k >= 0, the task is to enumerate the set
   binom({1,...,n},k) of all k-subsets of {1,...,n}. (See "Main concepts"
   in ComputerAlgebra/Combinatorics/Lisp/Enumeration/plans/general.hpp.) </li>
   <li> We have lexicographical and colexicographical ordering.
    <ol>
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
     <li> For a given Maxima-set S, and k >= 0, the list of all k-subsets
     of S in lexicographical order is computed as follows:
     \verbatim
lex_subsets_l(S,k) := if k=0 then [{}] elseif emptyp(S) then [] else
  block([x : first_element(S), R],
    R : disjoin(x,S),
    append(add_element_l(x,lex_subsets_l(R,k-1)), lex_subsets_l(R,k)))$
      \endverbatim
     </li>
     <li> And colexicographical order:
     \verbatim
colex_subsets_l(S,k) := if k=0 then [{}] elseif emptyp(S) then [] else
  block([x : last_element(S), R],
    R : disjoin(x,S),
    append(colex_subsets_l(R,k), add_element_l(x,colex_subsets_l(R,k-1))))$
     \endverbatim
     </li>
     <li> Regarding ranking, for colex-order we restrict attention to
     the base-set {1, ..., n} (that is, S is a subset of {1, ..., n}):
     \verbatim
rank_colex_subsets(S,n) := block([L : listify(S)],
  sum_l(create_list(binomial(L[i]-1,i), i,1,length(L))) + 1)$
     \endverbatim
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
rank_lex_subsets(S,n) := block([L : listify(S), k : length(S)],
  binomial(n,k) - sum_l(create_list(binomial(n-L[i],k-i+1), i,1,k)))$
     \endverbatim
     </li>
     <li> This formula comes from considering how many subsets are *after*
     S in the order: Let S = {v_1, ..., v_k} (as above). The elements after S
     have either every element larger than v_1, these are binomial(n-v_1,k),
     plus the k-subsets containing v_1 as first element, which without
     this element are after {v_2, ..., v_k}. Via recursion then the
     formula is obtained. </li>
     <li> Conjecture: Regarding unranking for lex-order, we greedily
     choose the values which produce the largest possible binomial
     coefficients, and then move down to the next value: 
     \verbatim
unrank_lex_subsets(x,n,k) := block([t_ip : 0, rs : []],
  for i : 1 thru k do block([j : t_ip + 1, bc, t_i : t_ip + 1],
    bc : binomial(n - j, k - i),
    while (x > bc) and (bc > 0) do (
      x : x - bc,
      j : j + 1,
      t_i : t_i + 1,
      bc : binomial(n - j, k - i)
    ),
    t_ip : t_i,
    rs : cons(t_i, rs)
  ),
  return(setify(rs))
)$
     \endverbatim
     </li>
    </ol>
   </li>
  </ul>


  \todo Enumerating all k-subsets in Gray-code manner
  <ul>
   <li> The point here is that only one element is changed at a time (when
   proceeding to the successor). </li>
   <li> There should be a standard, recursive way of achieving such an
   order. </li>
  </ul>

*/

