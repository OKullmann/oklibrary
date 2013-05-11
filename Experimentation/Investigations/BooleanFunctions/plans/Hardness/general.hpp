// Oliver Kullmann, 28.2.2012 (Swansea)
/* Copyright 2012, 2013 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/BooleanFunctions/plans/Hardness/general.hpp
  \brief On investigations regarding the hardness of representations of boolean functions


  \todo Connections
  <ul>
   <li> See Satisfiability/Lisp/FiniteFunctions/plans/TseitinTranslation.hpp.
   </li>
   <li> See Investigations/PigeonholeFormulas/plans/Hardness.hpp. </li>
   <li> See "Hardness" in
   Investigations/PigeonholeFormulas/plans/ExtendedResolution.hpp. </li>
  </ul>


  \todo Hardness of boolean function representations
  <ul>
   <li> The hardness h(F) of a CNF F is defined to be the minimum k such that
   for all implicates C of F, we have that r_k(phi_C * F) yields the
   empty clause. </li>
   <li> We have that:
    <ul>
     <li> h(F) = 0 if F contains all its prime implicates. </li>
     <li> h(F) <= h(F') where F' is the canonical (full) CNF representation
     of F. </li>
     <li> If F is a full clause-set, i.e., only clauses of length n(F)),
     then h(F) = n(F) - m where m is the size of a prime implicate of
     F with the minimum clause-length. </li>
     <li> If F is renamable horn, then h(F) <= 1. </li>
     <li> If F is a 2-CNF clause-set, then h(F) <= 2. </li>
    </ul>
   </li>
   <li> For a boolean function f the "hardest" representation F, without
   new variables, is the canonical CNF representation of f. This follows
   from the fact that for every partial assignment phi, applying phi to F
   yields the canonical unsatisfiable CNF on the remaining variables, which
   is of maximum hardness. </li>
   <li> How does this hardness notion relate to the performance of SAT solvers
   on existing problems? </li>
   <li> Investigations into how representations with different hardness
   affect SAT solving is being investigated in:
    <ul>
     <li> AES: Cryptography/AdvancedEncryptionStandard/plans/general.hpp. </li>
     <li> DES: Cryptography/DataEncryptionStandard/plans/general.hpp. </li>
    </ul>
   </li>
   <li> The hardness of the following should be investigated:
    <ul>
     <li> cardinality constraints, see
     Satisfiability/Lisp/PseudoBoolean/plans/CardinalityConstraints.hpp.
     </li>
    </ul>
   </li>
   <li> Computing the hardness of small functions:
    <ul>
     <li> We do not, in general, compute the hardness of boolean function
     representations, but construct representations with low hardness. </li>
     <li> However, for experimental evaluation and curiousities sake, we can
     and should measure the hardness of certain representations of small
     boolean functions. </li>
     <li> For example, we should measure the hardness of the minimum
     representations of the DES and AES boxes. See
     AdvancedEncryptionStandard/plans/Representations/general.hpp.
     </li>
     <li> We have the Maxima-functions hardness_wpi_cs, hardness_cs, and
     hardness_u_cs for computing hardness. </li>
     </li>
    </ul>
   </li>
  </ul>


  \todo Propagation hardness of 2-CNF
  <ul>
   <li> F in 2-CLS should even have phardness(F) <= 2. </li>
   <li> This should be investigated also experimentally (to strengthen our
   system). </li>
  </ul>


  \todo Hardness of prime-extremal satisfiable general Horn clause-sets
  <ul>
   <li> The following needs a complete update, based on the arXiv
   report http://arxiv.org/abs/1302.4421 . </li>
   <li> The clause-sets sat_genhorn_cs(k,l) are candidates to separate
   representations without new variables of softness l+1 from softness l:
    <ol>
     <li> sat_genhorn_cs(k,l) itself has hardness min(k,l). </li>
     <li> So for l >= 0 the sequence S = (sat_genhorn_cs(k,l+1))_{k >= l+1} has
     a linear-size representation without new variables of hardness precisely
     l+1, namely itself. </li>
     <li> Now the conjecture is that S has no polysize representations without
     new variables of softness l. </li>
     <li> This is true for l=0, as it follows from the results of the article
     [Sloan, Soereny, Turan, On k-term DNF with the largest number of prime
     implicants, 2007]. </li>
     <li> One experiment:
     \verbatim
F : sat_genhorn_cs(3,3)$
  8
P : min_resolution_closure_cs(F)[1]$
length(P);
  255
set_random(1);
B : rand_rbase_cs(P,ucp_0_cs)$
length(B);
  42
set_random(2);
B : rand_rbase_cs(P,ucp_0_cs)$
length(B);
  39
set_random(3);
B : rand_rbase_cs(P,ucp_0_cs)$
length(B);
  38
     \endverbatim
     We see here a rather small 1-base. </li>
    </ol>
   </li>
   <li> A systematic study of the boolean functions of sat_genhorn_cs(k,l) is
   needed:
    <ol>
     <li> A good representation (in the general sense) of the underlying
     boolean function is needed. Perhaps this should go to investigations on
     boolean functions in general. </li>
     <li> What is the precise number of variables, clauses, literal occurrences
     ? </li>
     <li> What are precisely the prime implicates and prime implicants?
      <ul>
       <li> From Lemma 8 of [Sloan, Soereny, Turan, On k-term DNF with the
       largest number of prime implicants, 2007] we have the following
       characterisation of the prime implicates:
       \verbatim
# Extracting the prime implicant from a subset F of the leaves of
# sat_genhorn_cs(k,l):
sat_genhorn_ul(F) := block([lit_s : olit_cs(F)],
  subset(lit_s, lambda([l],not(elementp(-l,lit_s)))))$

# Computing the prime implicates for sat_genhorn_cs(k,l):
sat_genhorn_prime_cs(k,l) :=
  map(sat_genhorn_ul, disjoin({},powerset(sat_genhorn_cs(k,l))))$

# Checking correctness:
F : sat_genhorn_cs(2,2)$
is(min_resolution_closure_cs(F)[1] = sat_genhorn_prime_cs(2,2));
 true
F : sat_genhorn_cs(3,3)$
is(min_resolution_closure_cs(F)[1] = sat_genhorn_prime_cs(3,3));
 true
       \endverbatim
       That is, the prime implicates of sat_genhorn_cs(k,l) are precisely
       the clauses {l in F: -l not in F} for subsets F of sat_genhorn_cs(k,l).
       </li>
      </ul>
     </li>
     <li> What are the m-bases for 0 <= m <= l ? </li>
    </ol>
   </li>
   <li> Are the sat_genhorn_cs(k,l) actually level-l Horn clause-sets? </li>
  </ul>


  \todo Hardness of canonical translation of positive DNF
  <ul>
   <li> For a positive DNF-clause-set F, the set of CNF-prime-clauses is
   the transversal hypergraph of F, with element-wise negation. </li>
   <li> One would guess that if F is far away from a variable-disjoint
   hitting clause-set, then the canonical translation dualts_cs(F) should have
   high hardness. </li>
   <li> The boolean function of the DNF F could then also have the property
   of not having a small CNF-representation of small hardness. </li>
   <li> If F is a positive 2-CLS, then we have hardness at most 1:
   \verbatim
test1(n) := hardness_cs(dualts_cs(powerset(setn(n),2)))$
   \endverbatim
   n <= 1 yield hardness 0, all other n yield hardness 1. </li>
   <li> 3-CLS:
   \verbatim
hardness_cs(dualts_cs({{1,2,3},{2,3,4}});
  1
   \endverbatim
   </li>
  </ul>

*/
