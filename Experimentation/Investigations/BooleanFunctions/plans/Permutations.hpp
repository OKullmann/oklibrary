// Oliver Kullmann, 5.12.2009 (Swansea)
/* Copyright 2009, 2010, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/BooleanFunctions/plans/Permutations.hpp
  \brief Investigating boolean functions representing permutations of {0,1}^n


  \todo Generalities
  <ul>
   <li> For a natural number n >= 0 one considers the set V_n = {0,1}^n of all
   bit-vectors. </li>
   <li> The functions to be studied here are permutations f of V_n. </li>
   <li> Permutations are represented as relations by boolean functions
   f^*: {0,1}^(2n), that is, f^*(x,y) = 1 iff f(x) = y. </li>
   <li> These boolean function have exactly one DNF-representation (using 2^n
   clauses of length 2*n). </li>
   <li> The CNF-representations are of special interest here. </li>
   <li> In ComputerAlgebra/Satisfiability/Lisp/FiniteFunctions/Permutations.mac
   we have perm2cnffcs(P), which for a permutation P in list-form creates
   the full cnf-fcs. </li>
   <li> In
   ComputerAlgebra/Satisfiability/Lisp/Resolution/PrimeImplicatesImplicants.mac
   we have all_minequiv_bvs_fcs to compute than all minimum CNF's (that is,
   by all_minequiv_bvs_fcs(perm2cnffcs(P)). </li>
   <li> A simpler function there is rsubsumption_hg_full_fcs: By
   rsubsumption_hg_full_fcs(FF)[2] we obtain the list of necessary prime
   implicates (that is, by rsubsumption_hg_full_fcs(perm2cnffcs(P))[2]). </li>
   <li> The permutations are (according to our standard representation)
   not permutations of bit-vectors, but of the numbers 1,..,2^n; which
   are considered as bit-vectors using binary representation of numbers. </li>
   <li> So all permutations are created by permutations(setn(2^n)). </li>
   <li> There are 2^(2^(2n)) boolean functions altogether of 2n arguments,
   while there are (2^n)! such permutations f. </li>
   <li> V_n can be considered as an n-dimensional algebra over F_2 (the
   two-element field), consisting of the n-dimensional vectorspace over F_2
   plus the field-structure of V_n as a field of order 2^n. </li>
   <li> Special permutations of interest are as follows (based on the
   algebraic structures).
    <ol>
     <li> Linear automorphisms, of which there are order_gl(n,2). </li>
     <li> Special linear automorphisms are given by multiplication with
     non-zero field elements, of which there are 2^n-1. </li>
     <li> Translations, of which there are 2^n. </li>
     <li> Affine automorphisms, of which there are order_gl(n,2) * 2^n. </li>
     <li> The multiplicative inverse x -> x^-1, extended by 0 -> 0. </li>
     <li> The compositions of inversions with affine automorphisms. </li>
     <li> For such compositions, is there a fundamental difference between
     first applying the inversion or first applying the affine automorphism?
     </li>
    </ol>
    We need general tools (including representations and conversions) to handle
    these objects.
   </li>
   <li> Of course, besides these "algebraic permutations" we need to study
   random permutations. </li>
   <li> Isomorphism of boolean functions:
    <ol>
     <li> Most powerful is to admit permutations of variables and individual
     flips of variables. </li>
     <li> A basic question is how this compares with conjugatedness of
     permutations (equivalent to having the same cycle type)? </li>
    </ol>
   </li>
  </ul>


  \todo What to investigate for these boolean functions
  <ul>
   <li> The number of prime implicates is of importance, the number of
   necessary clauses amongst them, the size of minimum CNF representations,
   and their number. </li>
   <li> At Maxima-level this is computed as available by
   investigate_permutations(n) (in
   Experimentation/Investigations/BooleanFunctions/Permutations.mac). </li>
   <li> Shortest r_k-compressions of the set of prime implicates are of
   high interest (since we expect them to be most useful for their use
   in SAT-translations). See rand_rbase_cs(F,r) in
   ComputerAlgebra/Satisfiability/Lisp/Reductions/RBases.mac. </li>
   <li> Also OBDD-representations are to be studied. </li>
   <li> And shortest circuit-representations. </li>
  </ul>


  \todo Trivial cases
  <ul>
   <li> The case n=0: investigate_permutations(0) yields "1 1 1 1". </li>
   <li> The case n=1: investigate_permutations(1) yields two times
   "2 2 2 1". </li>
  </ul>


  \todo The case n=2
  <ul>
   <li> Here we have just (2^2)! = 24 permutations altogether, so we can
   conveniently list them all (by permutations({1,2,3,4})). </li>
   <li> We get
   \verbatim
h2 : investigate_permutations(2)$
ev_ip(h2);

  [4,4,4,1] 8
  [10,0,5,2] 16
   \endverbatim
   </li>
   <li> So we have two cases: One with 4 prime implicates, which all are
   necessary, and one with 10 prime implicates, none of which are
   necessary, and having 2 minimum representations, each with 5 clauses. </li>
   <li> The first case is given by the identity, the second case by
   the permutation [1,2,4,3]:
   \verbatim
last(ev_hm(h2,[4,4,4,1]));
  [1,2,3,4]
last(ev_hm(h2,[10,0,5,2]));
[1,2,4,3]

all_minequiv_bvs_fcs(perm2cnffcs([1,2,3,4]));
  [{{-4,2},{-3,1},{-2,4},{-1,3}}]
all_minequiv_bvs_fcs(perm2cnffcs([1,2,4,3]));
  [{{-4,-3,2},{-4,-2,1},{-3,-2,4},{-1,3},{1,2,4}},
   {{-4,-2,3},{-4,-1,2},{-3,1},{-2,-1,4},{2,3,4}}]
   \endverbatim
   </li>
   <li> The identity is treated in general below. For the second case
   (a transposition) actually the two minimum clause-sets are disjoined
   (a partitioning of the set of all prime-clauses). </li>
   <li> The number of linear automorphisms is order_gl(2,2) = 6, while there
   are 2^2=4 translationen, which makes 24 affine automorphisms altogether.
   </li>
   <li> So here every permutation is an affine automorphism. </li>
  </ul>


  \todo The case n=3
  <ul>
   <li> Here we have just (2^3)! = 40320 permutations altogether, so we
   can still consider them all (algorithmically). </li>
   <li> Experiment:
   \verbatim
oklib_monitor:true;
h3 : investigate_permutations(3)$
[1,2,3,4,5,6,7,8] [6,6,6,1]
[1,2,3,4,5,6,8,7] [20,4,10,256]
[1,2,3,4,5,7,6,8] [26,2,10,288]
[1,2,3,4,5,7,8,6] [30,2,11,512]
[1,2,3,4,6,5,8,7] [12,2,7,2]
[1,2,3,4,6,7,8,5] [28,0,10,48]
[1,2,3,4,6,8,5,7] [26,0,9,2]
[1,2,3,4,8,7,6,5] [26,0,8,2]
[1,2,3,5,4,6,7,8] [36,0,14,870]
[1,2,3,5,4,6,8,7] [36,0,12,2]
[1,2,3,5,4,7,6,8] [44,0,12,485]
[1,2,3,5,4,7,8,6] [38,0,12,106]
[1,2,3,5,6,4,7,8] [36,0,12,1]
[1,2,3,5,6,4,8,7] [34,0,10,6]
[1,2,3,5,6,7,4,8] [41,0,12,592]
[1,2,3,5,6,7,8,4] [38,0,11,82]
[1,2,3,5,6,8,4,7] [38,0,11,1]
[1,2,3,5,8,4,6,7] [38,0,11,67]
[1,2,3,5,8,4,7,6] [37,0,12,4]
[1,2,3,5,8,7,6,4] [40,0,10,8]
[1,2,3,6,5,4,8,7] [40,0,11,64]
[1,2,3,6,5,7,4,8] [39,0,12,2088]
[1,2,3,6,5,7,8,4] [41,0,11,60]
[1,2,3,6,7,8,5,4] [28,2,10,192]
[1,2,3,6,8,7,4,5] [40,0,10,64]
[1,2,3,6,8,7,5,4] [44,0,11,100]
[1,2,3,8,6,5,7,4] [35,2,12,7680]
[1,2,3,8,6,7,5,4] [45,0,12,576]
[1,2,4,3,6,5,7,8] [36,0,10,4]
[1,2,4,3,6,7,5,8] [40,0,11,70]
[1,2,4,3,7,8,6,5] [30,0,9,6]
[1,2,4,5,6,3,7,8] [48,0,12,722]
[1,2,4,5,6,7,3,8] [47,0,12,694]
[1,2,4,5,7,8,6,3] [42,0,10,3]
[1,2,4,5,8,7,3,6] [44,0,11,84]
[1,2,4,7,6,8,3,5] [48,0,11,2]
[1,2,4,7,8,5,6,3] [51,0,12,1152]
[1,4,6,7,8,5,3,2] [40,0,10,20]
XXX
   \endverbatim
   Computation aborted (took too long; unclear whether there might be more
   cases --- better monitoring is needed, and faster computation).
   </li>
   <li> The number of linear automorphisms is order_gl(3,2) = 168, while
   there are 2^3=8 translations, which makes 1344 affine automorphisms
   altogether. </li>
  </ul>


  \todo The case n=8
  <ul>
   <li> This case is especially interesting because of AES; see
   Cryptography/AdvancedEncryptionStandard/plans/SAT2011/FieldMulInvestigations.hpp
   and
   Cryptography/AdvancedEncryptionStandard/plans/SAT2011/SboxInvestigations.hpp.
   </li>
   <li> Here we have (2^8)! ~ 8.578*10^506 permutations altogether (while
   there are ~ 2.003*10^19728 boolean functions (in 16 variables). </li>
   <li> The number of linear automorphisms is order_gl(8,2) ~ 5.348*10^18.
   </li>
   <li> Special linear (affine) automorphisms to consider are the ones
   involved in the S-box and its inverse. </li>
  </ul>


  \todo Prime implicates of simple permutations
  <ul>
   <li> The identity function: 
   <ul>
    <li> For the identity function id over {0,1}^n the prime implicates of
    id^* are exactly the clauses {v_{1,i},-v_{2,i}} and
    {-v_{1,i},v_{2,i}} for all 1 <= i <= n. </li>
    <li> These clauses encode the equality of the variables in the input set
    with those of the output set using binary constraints. </li>
    <li> We have here the case of a parallel and independent composition of
    n boolean functions, namely v_{1,i} = v_{2,i}. </li>
    <li> In general, for such independent parallel compositions the prime
    clauses are just taken alltogether. </li>
   </ul>
   </li>
   <li> The negation:
    <ol>
     <li> x -> neg x componentwise. </li>
     <li> Here we have parallel independent composition of
     v_{i,1} = not v_{i,2}, and so the prime implicates are exactly the
     clauses {v_{1,i},v_{2,i}} and {-v_{1,i},-v_{2,i}} for all 1 <= i <= n.
     </li>
    </ol>
   </li>
  </ul>


  \todo Affine bijections over ZZ_2
  <ul>
   <li> We need to find out all about the representations of boolean functions
   x -> a+x, i.e., translations by vector a. </li>
   <li> More complicated are linear bijections x -> M*x for invertible matrices
   M. </li>
   <li> Finally we need to consider x -> a + M*x. </li>
  
*/

