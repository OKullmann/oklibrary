// Oliver Kullmann, 29.6.2007 (Swansea)
/* Copyright 2007, 2008, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/plans/general.hpp
  \brief General plans regarding computer algebra (and SAT)


  \todo Module ComputerAlgebra/Sets
  <ul>
   <li> Connections (where plans are mentioned):
    <ol>
     <li> ComputerAlgebra/Hypergraphs/Lisp/plans/SetSystems.hpp </li>
     <li> ComputerAlgebra/plans/Matroids/Lisp/general.hpp </li>
     <li> ComputerAlgebra/DataStructures/Lisp/plans/HashMaps.hpp </li>
     <li> What about hash-maps? We should put the "general" things into
     Sets, leaving the "data structures" to ComputerAlgebra/DataStructures. </li>
    </ol>
   </li>
   <li> Relations:
    <ol>
     <li> An n-ary relation on a set M is a set of lists of length n, where
     the elements are all from M. </li>
     <li> Abbreviation: "nary_rel" in general, and "2ary_rel", "3ary_rel" etc.
     for instances. </li>
     <li> Test:
     \verbatim
nary_rel_p(R,M,n) := every_s(lambda([t],listp(t) and is(length(t)=n) and subsetp(setify(t),M)), listify(R))$
     \endverbatim
     </li>
     <li> Perhaps for binary relations we just use "rel". </li>
     <li> In generalisation of quasigroups we define a "n-jective relation"
     R on a set M as an n-ary relation R on M such that whenever n-1 values
     are arbitrary fixed, the remaining value is uniquely determined. </li>
     <li> Test:
     \verbatim
njective_rel_p(R,M,n) := nary_rel_p(R,M,n) and njectiverelp(R,M,n)$
njectiverelp(R,M,n) :=
 block([T : cartesian_product_l(create_list(listify(M),k,1,n-1))],
  every_s(lambda([i], 
    every_s(
      lambda([t], block([a : take_elements(i-1,t), b : rest(t,i-1)],
        is(length(intersection(R, setify(create_list(append(a,[x],b), x,listify(M))))) = 1))), 
      T)),
    create_list(i,i,1,n)))$
     \endverbatim
     </li>
     <li> Enumeration of all n-jective relations on set M:
     \verbatim
allnjective_rel(M,n) :=
 if n = 0 then [{}, {[]}]
 elseif n = 1 then create_list({[x]},x,listify(M))
 elseif n = 2 then listify(allperm_sm(M))
 else
   sublist(
     map(
       lambda([injf], lunion(create_list(
         map(lambda([x],endcons(p[1],x)),p[2]),
         p, listify(injf)))),
       allinj_sm(M,setify(allnjective_rel(M,n-1)))),
     lambda([R],njectiverelp(R,M,n)))$
     </li>
     <li> See "Counting the number of n-jective relations" in
     Experimentation/Investigations/LatinSquares/plans/general.hpp. </li>
    </ol>
   </li>
  </ul>


  \todo Frobenius problem
  <ul>
   <li> http://en.wikipedia.org/wiki/Coin_problem </li>
   <li> What is the precise computational complexity ?
    <ol>
     <li> How to measure the input length?? Unary or binary notation??? </li>
     <li> What does the "NP-hardness" mean? Upper bounds??
      <ol>
       <li> Consider the decision version "g(a_1,...,a_n) >= K ?". </li>
       <li> It seems that measuring the a_i in unary, the problem is solvable
       in polynomial time, while for a_i taken binary the problem seems to
       be NP-complete. </li>
      </ol>
     </li>
     <li> Parameterised complexity:
      <ol>
       <li> A natural parameter is the dimension n. (Seems to be poly-time
       for fixed n; FPT??) </li>
       <li> Another is the bound K. </li>
      </ol>
     </li>
    </ol>
   </li>
   <li> Direct algorithms:
    <ol>
     <li> "Heap and Lynn method", reducing the problem in a direct way
     to the index of primitivity, should be implemented. (While
     the index of primitivity has a trivial direct solution.) </li>
     <li> See ComputerAlgebra/Graphs/Lisp/plans/general.hpp,
     "Primitive directed graphs etc.". </li>
    </ol>
   </li>
  </ul>


  \todo Dynamical systems
  <ul>
   <li> On the one end we have an irreversible discrete dynamical system,
   that is a pair (X,f), where X is a set and f: X -> X. </li>
   <li> On the other end we have the operation of a monoid on a set. </li>
   <li> Special discrete dynamical system are finite systems (with finite X).
   </li>
   <li> Further specialised are sequential dynamical systems (SDS's), where
   X is the set of total assignments for a finite set V of variables with
   finite domains, and where f is a composition of f_v in some fixed order,
   where f_v: X -> X changes only the x-value and this only in dependence on
   the neighbours of x (and x) in an underlying graph on V. </li>
   <li> Since large X (potentially infinite) are of importance here, we should
   consider "explicit" and "implicit" representations:
    <ol>
     <li> An explicit discrete dynamical system is a pair [X,f], where X
     is a set and f a function with one argument (accepting inputs from X).
     </li>
     <li> An implicit discrete dynamical system is a pair [chi, f], where
     chi is a boolean function, which evaluates to true on the domain of
     the system, and f is a function as before. </li>
     <li> An explicit discrete system [X,f] is converted to an implicit
     system [buildq([X],lambda([x],elementp(x,X))), f] (as usual). </li>
    </ol>
   </li>
   <li> The basic task for discrete dynamical systems is the computation of
   orbits O(x), by O(x) = {f^i(x) : 0 <= i <= K} for K large enough.
    <ol>
     <li> For reversible systems (flagged as such, and with invertible f)
     we have O(x) = {f^i(x) : -K <= i <= K}. </li>
     <li> These computations only require an implicit system. </li>
     <li> For explicit systems also the set of all fixed points can be
     computed. </li>
     <li> K is at most |X|-1. </li>
    </ol>
   </li>
  </ul>


  \todo Bioinformatics
  <ul>
   <li> Should we create a module ComputerAlgebra/Bioinformatics, where we put
   all the (somewhat) diverse problems below, or do we strive to find more
   appropriate modules for them? </li>
   <li> A general and important organisational problem is as follows:
    <ol>
     <li> One of the aims of the OKlibrary is to present the "world of NP",
     as the problems and their translations. </li>
     <li> But, obviously, we consider in principle "all problems", not
     just in NP. </li>
     <li> These problems we be spread out over many modules. </li>
     <li> We need then some additional super-structure, which makes the
     problems and their translations accessible (as a general directed
     graph, where the vertices represent problems, and the arcs implemented
     translations). </li>
    </ol>
   </li>
   <li> The following is based on [Sperschneider, Bioinformatics, Springer,
   2008], which seems a rather sloppily written book, but can serve as
   a start (and perhaps the quality of the bioinformatics literature in
   general is rather low). </li>
   <li> "Consecutive Ones"
    <ol>
     <li> Given a combinatorial {0,1}-matrix A, find all linear orders of
     the column set such that for every row all 1's are consecutive. </li>
     <li> In other words, given a natural number n and m subsets A_i of
     {1, ..., n}, find all permutations pi of {1, ..., n}  such that for
     all i the set pi(A_i) is an arithmetic progression of slope 1. </li>
     <li> This reformulation makes it clear that no "wrapping around" is
     allowed. </li>
     <li> Apparently the decision problem (whether such a linear order exists)
     is poly-time solvable. </li>
     <li> And furthermore apparently "PQ-trees" allow for an efficient
     representation of all solutions (as permutations of the row-set), that
     is, this representation is computable in poly-time, and furthermore
     from such a representation the represented permutations can be
     enumerated with delay linear in the size of the representation. </li>
     <li> Questionable whether also the permutations can be counted
     (efficiently)? </li>
     <li> What is the general context of this problem? Looks like a special
     type of combinatorial problems. </li>
    </ol>
   </li>
   <li> "Single Digest"
    <ol>
     <li> In its general form, given is a group (G,*,1), and for a finite
     subset X of G - {1} the "full digest" is the (finite) multiset
     dg_X: G -> NN_0, where dg(x) counts for how many (y,z) in
     X^2 - id_X we have y * z^{-1} = x. </li>
     <li> The problem is then, given a finite multiset D over G, find
     X with dg_X = D, where X, X' are considered as equivalent if they
     differ only by a translation (since then we have dg_X = dg_X'). </li>
     <li> One can slightly generalise the problem by allowing X to contain
     1, and taking X to be a multiset. One can also consider for the "digest"
     all pairs (y,z) in X^2. But this seems simple to solve, once the
     fundamental problem is solved, and also not helpful. </li>
     <li> The special form of the problem considered in [Bioinformatics]
     uses G = ZZ and X subset of NN_0 with 0 in X (so that we do not
     need to consider equivalences between X, X' anymore), where
     furthermore for the digest only the positive results are used, and
     accordingly D is a multiset over NN. </li>
     <li> In the sequel only this problem is considered (for now). </li>
     <li> It is unknown whether the decision problem (deciding whether
     a solution exists) is poly-time or NP-complete (clearly it is in
     NP). </li>
     <li> Unclear, whether counting of all solutions, and representing them,
     is also of interest in bioinformatics; likely it is, since likely
     there are further conditions on the digest to be optimised. </li>
     <li> A backtracking algorithm is presented in [Bioinformatics], which
     is claimed to be rather efficient. </li>
     <li> Interesting here to consider formulations as generalised SAT
     problems or constraint satisfaction problems. </li>
     <li> Given D of size n * (n-1) / 2 (otherwise the problem is trivially
     unsolvable), a natural formulation as a constraint problem is to
     consider as a solution an assignment
     f: {(i,j) : 1 <= i < j <= n} -> D such that for all 1 <= i < j < k <= n
     we have f((i,k)) = f((i,j)) + f((j,k)), and such that f is a bijection
     (for the multiset D). </li>
     <li> Given such f, X can be easily constructed as
     X = {0} + {f(1,j) : 1 < j <= n}. </li>
     <li> The problem with this formulation is likely that a lot of symmetries
     are introduced in this way, which the backtracking algorithms avoids.
     </li>
     <li> We should implement the backtracking algorithm. </li>
     <li> Perhaps this problem is still too easy (from the complexity point
     of view) to be analysable as a generalised SAT problem. </li>
     <li> Is there a general context for this problem? It has a slight
     Ramsey-flavour (like the "Consecutive Ones"). </li>
    </ol>
   </li>
   <li> "Double Digest"
    <ol>
     <li> The advantage here is that this problem is NP-complete. </li>
     <li> Unfortunately the "definition" in [Bioinformatics] (page 30) is
     not precise enough to determine what this problem is about. </li>
     <li> Theorem 5.38 shows that this problem is NP-complete; we should
     implement the translation. </li>
    </ol>
   </li>
   <li> "Maximum Biclique"
    <ol>
     <li> This is the problem of finding a maximum-edges biclique in
     a directed graph, or, in the language of combinatorial matrices,
     finding a constant-1 submatrix of maximum size (number of entries)
     in a given {0,1}-matrix. </li>
     <li> See "Maximum bicliques" in
     ComputerAlgebra/Graphs/Lisp/Bicliques/plans/general.hpp. </li>
     <li> The context of this problem in [Bioinformatics] is that of
     clustering data. </li>
     <li> This problem is NP-complete, and definitely of interest to us. </li>
     <li> This problem should definitely be handled in Graphs or
     CombinatorialMatrices; however, perhaps in ComputerAlgebra/Bioinformatics
     we have applications of it. </li>
     <li> This could be the general pattern: In module Bioinformatics we
     consider the "application aspect", while for the problems we try to
     find their "true environment" (similar to Transitional/Applications).
     </li>
    </ol>
   </li>
  </ul>


  \todo Module ComputerAlgebra/Automata
  <ul>
   <li> As mentioned in "General plans" in
   ComputerAlgebra/Satisfiability/Lisp/FiniteFunctions/plans/general.hpp,
   we should have a module on automata. </li>
   <li> Including finite automata and Turing machines. </li>
  </ul>

*/

