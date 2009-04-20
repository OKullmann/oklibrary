// Oliver Kullmann, 19.7.2008 (Swansea)
/* Copyright 2008, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Hypergraphs/Lisp/plans/Generators.hpp
  \brief Plans regarding generators for hypergraphs
  
  
  \todo Statistics
  <ul>
   <li> For all generated hypergraphs we need accompanying
   statistic-functions. </li>
   <li> As for clause-set statistics. </li>
  </ul>


  \todo Create complete r-graphs
  <ul>
   <li> Implement "complete_hg(V,r), complete_stdhg(n,r), complete_ohg(V,r),
   complete_stdohg(n,r)", where the order is given by lexicographical order
   (see "Enumerating all k-subsets lexicographically" in
   ComputerAlgebra/Combinatorics/Lisp/Enumeration/plans/Subsets.hpp). </li>
   <li> However, see "Ramsey hypergraphs" below --- to be used as the
   vertex set of a %Ramsey hypergraph, we needed to use colexicographical
   ordering. </li>
   <li> This would also yield that the hyperedge list of complete_stdohg(n,r)
   extends that of complete_stdohg(n',r) for n' <= n. </li>
  </ul>


  \todo Sudoku
  <ul>
   <li> Create Sudoku-hypergraphs (such that the Sudoku-problems can be
   expressed as strong colouring problems). </li>
   <li> For box dimension p the vertex set should be {1,...,p}^2. </li>
   <li> The 3N hyperedges (N=p^2) just correspond to the rows, columns
   and boxes (the hypergraph is thus N-uniform). </li>
  </ul>


  \todo %Ramsey hypergraphs
  <ul>
   <li> One needs to revise the extreme cases; compare "Ramsey problems"
   in ComputerAlgebra/Satisfiability/Lisp/Generators/plans/RamseyProblems.hpp.
   </li>
   <li> Providing a standardised vertex set:
    <ol>
     <li> To handle large hypergraphs, we need to provide a version with
     standardised vertex names, using a standard enumeration of r-subsets
     (compare "Create complete r-graphs" above). </li>
     <li> We only need to consider lexicographical ordering of the
     vertex names. Actually, in order to preserve monotonicity, we better
     use colexicographical ordering. </li>
     <li> This standardised hypergraph has then a canonical order-preserving
     isomorphism to ramsey_ohg(q,r,n).
      <ol>
       <li> For this to hold (for the vertices) we needed ramsey_ohg
       to use colexicographical ordering. </li>
       <li> Perhaps this is a sensible thing to do; however it seems
       we don't need to do something on the hyperedges (i.e., we continue
       to use lexicographical order there). </li>
      </ol>
     </li>
     <li> The same numbering should also be used in the C++ generator
     (see Ramsey.cpp). </li>
     <li> So that we can easily create additional clauses with Maxima,
     added then to the C++-generated files. </li>
     <li> The new generators should be named "ramsey_stdhg" and
     "std_ramsey_stdohg". </li>
    </ol>
   </li>
   <li> The hypergraphs ramsey_hg use sets directly as vertex names
   (like "{1,3}"), not naming schemes like "rv(1,3)".
    <ol>
     <li> On the one hand, this is natural in this situation. </li>
     <li> But it makes the approach less flexible: Using unevaluated
     functions like "rv", renaming should be possible by just stipulating
     an interpretation of "rv"!. </li>
     <li> The task is to figure out, how (locally, in a block) we can
     evaluate expressions containing terms like "rv(i,j)" using
     some (locally) specified function f(i,j).
      <ol>
       <li> This works like follows:
       \verbatim
ev([rv(1,3),rv(2,3,5)],rv([L]):=rank_colex_subsets(setify(L)),nouns);
 [2,7]
       \endverbatim
       </li>
       <li> "nouns" is needed to evaluate the "nouns" rv. </li>
       <li> Shown is also how to handle n-ary functions. </li>
      </ol>
     </li>
     <li> It remains whether we should use, %e.g., rv(1,2,3) or rv({1,2,3}).
      <ol>
       <li> The form rv(1,2,3) is a bit shorter. </li>
       <li> But the vertices rv(1,2,3) and rv(2,1,3) are different, though
       they represent the same set {1,2,3}. </li>
       <li> One could enforce associativity and commutativity, but perhaps
       this would introduce unnecessary complexities here. </li>
       <li> Or one disallows rv(2,1,3). </li>
      </ol>
     </li>
     <li> DONE (it seems that it is most natural that the generators
     do not care about possible renamings)
     A general question is who is responsible for (controlled!)
     renaming: The generating facilities or the renaming facilities
     (where "controlled" here could be translated as "efficient"). </li>
     <li> DONE (it seems that later evaluating the variable terms is
     most natural)
     An alternative to evaluating rv-terms ("later") would be to
     replace ("early") the function which creates the rv-terms by that
     translation function f (as above). </li>
     <li> DONE (it is possible)
     Regarding "late" translation, a question is also whether a
     term like "{1,2,3}", which should stand for "set(1,2,3)", can
     also be locally evaluated by evaluating set(1,2,3) as f(1,2,3). </li>
    </ol>
   </li>
   <li> Accompanying statistics are needed.
    <ol>
     <li> See okltest_ramsey_hg for the formula for the number of
     hyperedges. </li>
     <li> Likely only basic statistics should be computed, while for example
     the size of the automorphism group etc. is handled in module.
     RamseyTheory/Lisp/Ramsey. </li>
    </ol>
   </li>
  </ul>


  \todo Generalised %Ramsey hypergraphs
  <ul>
   <li> We can define %Ramsey hypergraphs for arbitrary hypergraphs G
   (ramsey_hg uses the complete r-graph).
    <ol>
     <li> The vertices are the hyperedges of G. </li>
     <li> Hyperedges are sets of hyperedges of G which are the hyperedge-set
     of a complete q-graph. </li>
     <li> If G is a graph, then we are considering the hypergraph of
     "cliques", restricted to cliques of size q, and where the clique
     is viewed as a collection of edges. </li>
     <li> We need to check whether this is the common point of view. </li>
    </ol>
   </li>
  </ul>


  \todo Arithmetic progressions in prime numbers
  <ul>
   <li> It seems that statistics on the number of hyperedges in
   arithprog_primes_ohg are of interest (and non-trivial); see
   Experimentation/Investigations/RamseyTheory/GreenTaoProblems/plans/AdditiveNumberTheory.hpp
   . </li>
   <li> DONE (now reasonable fast, and for faster computations more
   complex algorithms are needed)
   The function arithprog_primes_ohg is very slow.
    <ol>
     <li> DONE
     Since the list of primes is sorted, we can stop trying to extend
     the progression in case the difference gets bigger than d. </li>
     <li> DONE
     Implement arithprog_primes_finish[k,n] (an array-function), which
     computes all arithmetic progressions of length k amongst the first n
     prime numbers which finish in n. </li>
     <li> DONE
     Then arithprog_primes_ohg is computed by a simple append. </li>
     <li> DONE
     So results are re-used; a certain inefficiency is still caused
     by the permanent re-computation of the set of the first primes, however
     this should be a rather small amount.
      <ul>
       <li> arithprog_primes should set variable primes_init_seg,
       by appending iteratively next_prime(last_prime). </li>
       <li> And arithprog_primes_finish inherits this variable. </li>
      </ul>
     </li>
     <li> DONE
     More efficient access to the primes:
      <ul>
       <li> One should additionally use a boolean array of length p_max, where
       p_max is the n-th prime number, containing true if the index is prime,
       and then after having fixed the initial p and p2 (and thus d), instead
       of running through all z we can directly investigate the primes in the
       right distance. </li>
       <li> This array is then created by arithprog_primes through a function
       from number theory. </li>
       <li> But we still need primes_rev_init_seg so that the available
       distances can be quickly calculated. </li>
       <li> Instead of using this array, we could just provide a memoising
       wrapper "mprimep" around primep (this is what the array would achieve,
       within the given bounds). This seems best. </li>
    </ol>
   </li>
  </ul>

*/

