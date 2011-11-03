// Matthew Gwynne, 2.1.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/plans/Representations/general.hpp
  \brief Investigations into representations for components of the AES related to the SAT 2011 paper


  \todo Connections
  <ul>
   <li> See
   ComputerAlgebra/Satisfiability/Lisp/LinearConditions/plans/general.hpp for
   the handling of linear constraints in general. </li>
  </ul>


  \todo Considering output bits on their own
  <ul>
   <li> For better comparison with the DES-paper (see
   Cryptography/AdvancedEncryptionStandard/plans/general.hpp), we also need
   to consider the treatment of the boxes as 8 boolean functions (one for each
   output bit). </li>
   <li> In this way at least encryption by just UCP is ensured. </li>
   <li> We need to compare it with our standard approach, considering the
   whole boolean function. </li>
   <li> This needs to be done for all approaches (minimum, canonical, and
   bases). </li>
  </ul>


  \todo Overview
  <ul>
   <li> One needs to systematically explore CNF representations, with and
   without new variables. </li>
   <li> We consider methods for computing the following representations
   for the AES boxes:
   <ul>
    <li> the prime implicates.
    <li> minimum CNF representations. </li>
    <li> small (minimal) CNF representations
    (in cases where minimum representations are infeasible). </li>
   </ul>
   These are described in
   Cryptography/AdvancedEncryptionStandard/plans/Representations/Methods.hpp .
   </li>
   <li> There are currently open investigations in the following files:
   <ul>
    <li> 8-bit Sbox:
    Cryptography/AdvancedEncryptionStandard/plans/Representations/Sbox_8.hpp
    </li>
    <li> 8-bit Multiplication by 3:
    Cryptography/AdvancedEncryptionStandard/plans/Representations/Mul_3_8.hpp
    </li>
    <li> 8-bit Multiplication by 9:
    Cryptography/AdvancedEncryptionStandard/plans/Representations/Mul_9_8.hpp
    </li>
    <li> 8-bit Multiplication by 11:
    Cryptography/AdvancedEncryptionStandard/plans/Representations/Mul_11_8.hpp
    </li>
    <li> 8-bit Multiplication by 13:
    Cryptography/AdvancedEncryptionStandard/plans/Representations/Mul_13_8.hpp
    </li>
    <li> 8-bit Multiplication by 14:
    Cryptography/AdvancedEncryptionStandard/plans/Representations/Mul_14_8.hpp
    </li>
   </ul>
   </li>
   <li> For k-based representations see "k-based representations". </li>
  </ul>


  \todo Combining linear components
  <ul>
   <li> A full description of all of the possibilities for recombining
   AES and small scale linear components (from the Sbox and MixColumns)
   needs to be provided. </li>
   <li> The idea here is that, due to the linearity of the Sbox's affine
   transformation, and the MixColumns operations, as well as the
   fact the Shiftrows simply permutes bytes, the linear aspects of the Sbox
   can be moved out, and seperated (in the case of the affine addition),
   or merged into the boxes for the MixColumn. </li>
   <li> For the Sbox, we have 3 possibilities:
    <ul>
     <li> Full Sbox (M . s^(-1) + A) . </li>
     <li> Sbox minus addition of the affine constant (M . s^(-1)). </li>
     <li> Sbox minus affine transform entirely (s^(-1)). </li>
    </ul>
   </li>
   <li> For the MixColumn multiplications, we have 2 possibilities:
   <ul>
    <li> Standard byte-field multiplications. </li>
    <li> Sbox linear component (M) and Standard byte-field multiplications
    together. </li>
   </ul>
   </li>
   <li> A description of how to generate such translation is needed. </li>
   <li> See also "Rearranging linear components of Sbox and MixColumns" in
   ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/plans/Translations.hpp.
   </li>
  </ul>


  \todo Scripts for generating statistics on random boxes
  <ul>
   <li> We currently wish to investigate various types of random
   boxes to see how they compare with the standard AES components. </li>
   <li> Therefore, we need scripts to generate and almalgamate this data.
   </li>
   <li> We can generate the various random boxes using:
   <ul>
    <li> Random boolean function - random_full_fcs in
    ComputerAlgebra/Satisfiability/Lisp/Generators/RandomClauseSets.mac .
    </li>
    <li> Random linear maps - ss_sbox_linmap_gen_cnfp in
    ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/SboxAnalysis.mac .
    </li>
    <li> Random permutations - random_permutation and perm2cnffcs in
    ComputerAlgebra/Satisfiability/Lisp/FiniteFunctions/Permutations.mac .
    </li>
   </ul>
   </li>
   <li> We also need scripts to investigate the small scale multiplications,
   as well as the multiplications combined with the Sbox linear map (see
   ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/FieldOperationsAnalysis.mac).
   </li>
  </ul>


  \todo Find "best" solver(s) and local search algorithms for minimisation
  <ul>
   <li> There are currently a considerable number of problems in our
   investigation which require the solving of minimisation problems
   and finding transversals of subsumption hypergraphs when looking for
   minimum CNF representations
   (see
   Cryptography/AdvancedEncryptionStandard/plans/Representations/Methods.hpp)
   </li>
   <li> We need a survey of the various solvers and local search algorithms
   that we can use with these problems, along with their performance on
   small instances, so that we can apply the best methods we have given
   limited resources. </li>
  </ul>


  \todo Standard naming scheme for experiment files
  <ul>
   <li> We need to think of a naming scheme for the AES boxes so
   we can create the hpp files discussing them in this directory. </li>
   <li> We have the following boxes to investigate:
   <ul>
    <li> Sboxes with the following variants and parameters:
    <ul>
     <li> Sboxes with exponent / number of bits ranging from 1-8. </li>
     <li> Sboxes without and without the affine constant addition
     and linear multiplication (see "Combining linear components"). </li>
     <li> random permutations for the Sbox. </li>
     <li> random linear maps inside and outside the Sbox. </li>
    </ul>
    At the simplest level we vary the exponent (as in the
    [Small Scale Variants of the AES; Cid, Murphy, Robshaw]) and
    keep the rest as defaults.
    </li>
    <li> Multiplication within the field with following variants:
    <ul>
     <li> the field element to multiply by. </li>
     <li> the exponent / number of bits ranging from 1-8. </li>
     <li> multiplications with and without the combination of
     the Sbox linear map (see "Combining linear components").
    </ul>
    </li>
   </ul>
   </li>
   <li> For now, we name:
   <ul>
    <li> the small scale Sboxes: Sbox_${e}.cnf , where ${e} is
    the field of the Sbox is a GF(2^e) finite field. </li>
    <li> the small scale multiplications by element "a", in the
    default field with exponent e: Mul_${a}_${e}.cnf . </li>
   </ul>
  </ul>


  \todo k-based representations
  <ul>
   <li> We consider k-bases for k in {1,2,...}? (see rand_rbase_cs(F,r) in
   ComputerAlgebra/Satisfiability/Lisp/Reductions/RBases.mac.) </li>
   <li> One could consider certain prime implicates more important than others;
   for example ensuring that at least given a full input and/or a full output
   to one permutation the output resp. input can be inferred.
    <ol>
     <li> Can one formulate (relatively efficiently) the minimisation target
     that these inferences are "easily" available while otherwise using the
     smallest representation? </li>
     <li> We could generalise the notion of r-base w.r.t. specific clauses
     which have to be deducible via r, while all (other) removed clauses just
     need to follow logically, or perhaps using some stronger reduction. </li>
    </ol>
   </li>
   <li> When investigations begin fully in this area, this todo should be
   moved to a new file, and most likely a new sub-module. </li>
   <li> We need to bring together here all information on currently known
   on 1-bases for the AES boxes. </li>
   <li> We also need to synchronise the notions in Annotations, here in
   this todo and in
   ComputerAlgebra/Satisfiability/Lisp/Reductions/plans/RBases.hpp. </li>
  </ul>


  \todo The square of the Sbox
  <ul>
   <li> As a start into the consideration of mergers within the AES-"circuit"
   we consider the square of the sbox. </li>
   <li> As a Maxima boolean-function this is obtained by
   \verbatim
s2 : square_bf(rijn_sbox_bf);
   \endverbatim
   </li>
   <li> The full CNF representation is then obtained by
   \verbatim
F2 : bf2relation_fullcnf_fcs(s2,8)$
   \endverbatim
   </li>
   <li> A DIMACS file is created via
   \verbatim
output_fcs(
    sconcat("The squared AES Sbox in full CNF representation."),
    F2,
    "AES_Sbox2_full.cnf")$
   \verbatim
   </li>
   <li> Prime clauses:
   \verbatim
> QuineMcCluskey-n16-O3-DNDEBUG AES_Sbox2_full.cnf > AES_PK2.cnf
> cat AES_PK2.cnf | ExtendedDimacsFullStatistics-O3-DNDEBUG
 n non_taut_c red_l taut_c orig_l comment_count finished_bool
16 137185 1007214 0 1007214 1 1
 length count
5 5
6 3898
7 83267
8 49203
9 812
   \endverbatim
   </li>
   <li> This doesn't look much different from the Sbox itself; one needs to
   consider further properties. </li>
  </ul>


  \todo Understanding prime implicates after any partial assignment
  <ul>
   <li> To consider the AES boxes as an "active clause", we want to first be
   able, given a partial assignment, to infer as many forced assignments
   as possible. This can be done simply with the DNF representation. </li>
   <li> However, secondly one needs, given a partial assignment, to be able to
   determine various measures for heuristics. </li>
   <li> Therefore, investigating several statistics (most notably the number of
   clauses for a given variable) of the prime implicates of the clause-set,
   formed after taking the Sbox and applying each partial assignment, is
   necessary to try and discern a pattern. </li>
   <li> If such patterns can be deduced for particular clause-set measures,
   then the active clause can use this pattern, given a partial assignment,
   to return reasonable values for these measures which can be used for
   statistics. </li>
   <li> A C++ implementation of such a system whereby the set of prime
   implicates is taken as input, and each partial assignment along with
   the relevant statistics is returned is necessary. </li>
   <li> Such a C++ implementation would need to be able to apply a partial
   assignment to a clause-set and then compute various statistics on the
   result. This would need to be done for every partial assignment. </li>
   <li> After applying the partial assignment, to gain the prime implicates
   of the new boolean function, one must simply apply subsumption elimination
   to the new clause-set (which is just result of applying a partial assignment
   to the prime implicates of the original function). This can be done using
   functionality already in the library (MG: Where?). </li>
  </ul>

*/
