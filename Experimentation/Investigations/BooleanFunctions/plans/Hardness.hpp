// Oliver Kullmann, 28.2.2012 (Swansea)
/* Copyright 2012 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/BooleanFunctions/plans/Hardness.hpp
  \brief On investigations regarding the hardness of representations of boolean functions


  \todo Connections
  <ul>
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
     <li> h(F) = 0 if F contains all it's prime implicates. </li>
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
     <li> Prime-extremal satisfiable general Horn clause-sets:
     "Hardness of prime-extremal satisfiable general Horn clause-sets" below.
     </li>
     <li> Extended pigeon-hole formulas:
     "Hardness" in PigeonholeFormulas/plans/ExtendedResolution.hpp. </li>
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
     <li> Computed statistics for the formulas:
     \verbatim
# Statistics for original formulas and their prime implicates
# Annotated with the size of the smallest 1-base found so far (see below)
maxima> for l in [2,3,4] do for k : l thru 4 do block([F : sat_genhorn_cs(k,l)],
          print("k =", k, "l =", l, "n(F) =", length(var_cs(F)),
                " c(F) =", length(F), " l(F) = ", length(olit_cs(F)),
                " c(prc0(F)) = ", 2^(length(F)) -1));

k = 2 l = 2 n(F) = 7   c(F) = 4   l(F) =  10  c(prc0(F)) =  15     min_r1 <= 4
k = 3 l = 2 n(F) = 13  c(F) = 7   l(F) =  19  c(prc0(F)) =  127    min_r1 <= 7
k = 4 l = 2 n(F) = 21  c(F) = 11  l(F) =  31  c(prc0(F)) =  2047   min_r1 <= ?
k = 3 l = 3 n(F) = 15  c(F) = 8   l(F) =  22  c(prc0(F)) =  255    min_r1 <= 30
k = 4 l = 3 n(F) = 29  c(F) = 15  l(F) =  43  c(prc0(F)) =  32767  min_r1 <= ?
k = 4 l = 4 n(F) = 31  c(F) = 16  l(F) =  46  c(prc0(F)) =  65535  min_r1 <= ?
     \endverbatim
     </li>
     <li> Generating full CNFs for the formulas (ignoring how to standardise):
     \verbatim
# Outputting full clause-sets for sat_genhorn_cs(k,l)
# for l in [2,3] and k in [l,...,3].
maxima> for l in [2,3] do
          for k : l thru 3 do (
            print("Outputting ",l," ",k),
            output_fcs(
              sconcat("Satisfiable tree-clause-sets with height ",k, " and hs(F) = ",l),
              expand_fcs(standardise_fcs(cs2fcs(sat_genhorn_cs(k,l)))[1]),
              sconcat("genhorn_k",k,"_l",l,"_full.cnf")));
# The full clause-sets for sat_genhorn_cs(4,3) and sat_genhorn_cs(4,4) are
# too large, so we just output sat_genhorn_cs(4,2) for l=2.
maxima> block([k:4;l:2],
          output_fcs(
            sconcat("Satisfiable tree-clause-sets with height ",k, " and hs(F) = ",l),
            expand_fcs(standardise_fcs(cs2fcs(sat_genhorn_cs(k,l)))[1]),
            sconcat("genhorn_k",k,"_l",l,"_full.cnf")));

# Using generated full CNFs for the sat_genhorn_cs(k,l):
shell> ls genhorn*.cnf
genhorn_k2_l2_full.cnf  genhorn_k3_l3_full.cnf
genhorn_k3_l2_full.cnf  genhorn_k4_l2_full.cnf
     \endverbatim
     "%Variable standardisation for sat_genhorn_cs" in
     ConflictCombinatorics/plans/HittingClauseSets.hpp discusses the
     need for a standardised version of sat_genhorn_cs. </li>
     <li> Finding 1-bases for sat_genhorn_cs(k,l):
      <ul>
       <li> At the Maxima level (using rand_rbase_cs):
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


F : sat_genhorn_cs(4,2)$
  15
(%i5) P : min_resolution_closure_cs(F)[1]$
# Still running on cspcmg.


F : sat_genhorn_cs(4,3)$
  15
(%i5) P : min_resolution_closure_cs(F)[1]$
# Still running on cspcmg.
       \endverbatim
       We see here a rather small 1-base. </li>
       <li> Using RandomRUcpBases:
       \verbatim
# Finding random 1-bases for l=2
shell> RandomRUcpBases genhorn_k2_l2_full.cnf
*** Currently trying gs=50,bs=5
     pn      pc      n    nmi       c        l     n0   n0mi      c0       l0  cmts
      7       4      7      7       4       12     NA     NA       4       12     0
 length   count
      3       4
------------------------------------------------------------------------------------
CURRENT MINIMUM RBASE: *4* with gs=1,bs=1
------------------------------------------------------------------------------------
shell> RandomRUcpBases genhorn_k3_l2_full.cnf
*** Currently trying gs=50,bs=5
     pn      pc      n    nmi       c        l     n0   n0mi      c0       l0  cmts
     13      12     13     13      12       61     NA     NA      12       61     0
 length   count
      3       1
      4       6
      6       1
      7       4
------------------------------------------------------------------------------------
CURRENT MINIMUM RBASE: *7* with gs=16,bs=1
------------------------------------------------------------------------------------


# Finding random 1-bases for l=3
shell> RandomRUcpBases genhorn_k3_l3_full.cnf
*** Currently trying gs=50,bs=5
     pn      pc      n    nmi       c        l     n0   n0mi      c0       l0  cmts
     15      36     15     15      36      252     NA     NA      36      252     0
 length   count
      4       8
      7       4
      8      24
------------------------------------------------------------------------------------
CURRENT MINIMUM RBASE: *30* with gs=35,bs=2
------------------------------------------------------------------------------------
       \endverbatim
       We (again) see rather small 1-bases. Especially, for instance, for l=2,
       k=3 where we have 127 prime implicates but a 1-base of size 7.
       </li>
      </ul>
     </li>
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
     <li> What are precisely the prime implicates and prime implicants? </li>
     <li> What are the m-bases for 0 <= m <= l ? </li>
    </ol>
   </li>
   <li> Are the sat_genhorn_cs(k,l) actually level-l Horn clause-sets? </li>
   <li> Auxilliary functions for generating the prime implicates of
   sat_genhorn_cs and standardising it are needed;
   see "%Variable standardisation for sat_genhorn_cs" and
   "%Generators for satisfiable hitting clause-sets" in
   ConflictCombinatorics/plans/HittingClauseSets.hpp. </li>
  </ul>

*/
