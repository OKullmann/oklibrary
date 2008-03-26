// Matthew Gwynne, 26.3.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/plans/Translations.hpp
  \brief Plans for the translation of Rijndael into active clauses etc in Maxima

  
  \todo Partitioning into active clauses
  <ul>
   <li> This todo has to be updated according to
   ComputerAlgebra/Satisfiability/plans/SatisfactionProblems.hpp. </li>
   <li> An overview on the possibilities of anatomising the AES-process into
   active clauses has to be gained. </li>
   <li> The roughest subdivision presents just the input-output relation (this
   shouldn't not be considerd as an active clause). </li>
   <li> At the finest level we have represented the whole of AES as a boolean
   CNF. </li>
   <li> Inbetween there are many possibilities to handle the round-computations. </li>
  </ul>


  \todo Encoding AES (top down)
  <ul>
   <li> Break this todo into seperate todos </li>
   <li> For an initial translation to CNF, the following seems sensible
    <ol>
     <li> Function of the form aes_cp(p1,...,p128,k1,...,k128,c1,...,c128) 
     which given the plaintext, key and ciphertext variables, produces a
     set of conditions. </li>
     <li> At the highest level, the "aes_cp" could produce a set of conditions
     in terms of an "aes_round_cd" condition, which could then later be defined
     through functions which would then be substituted for conditions in the
     set. </li>
     <li> The conditions can be replaced by functions that 
     are only *then* evaluated by use of "obsubst", providing a more
     structured intentional approach, which makes substituting different
     conditions into the final expansion far easier (without constant 
     function renaming). </li>
     <li> This methodology can be applied down to levels such as the sbox
     and field element multiplications (within reason) and then additional
     must be made of these operations. </li>
     <li> An issue that arises is the naming/production of new variables. 
     Given that these will be needed at various levels to "join" various result 
     bits to input bits of different conditions, but if such a scheme uses
     function evaluations, how to produce such variables without sharing some
     kind of global state? </li>
     <li> Passing an initial variable pool (list of variables) into the 
     conditions could work, but this doesn't seem to fit well with the conditions
     being simple conditions given a set of input bits. Perhaps a method which 
     evaluates a given set of conditions and condition producing functions and
     then defines this variable pool, which is then in the scope of the 
     condition producing functions when they are substituted and evaluated? 
     </li>
     <li> Also, if each of these functions produces a set of conditions, then
     simple substitution would yield a set of sets, and then a set of set of
     sets etc. A simple solution here seems to be, to return the union of 
     any unevaluated conditions, where this union will be applied 
     (automatically by maxima) only when the entire expression has been 
     sufficiently evaluated, such that each of these conditions yields
     a set. </li>
     <li> On second thought, it seems more natural, considering each of
     these functions can be run in isolation and should produce a set of
     conditions, that only at the substitution stage should the issue,
     of joining the set produced by a condition to the parent set, be
     considered. </li>
     <li> For the unevaluated conditions (placeholders) within the sets, 
     perhaps the postfix "_c" for condition, could be used, and for the
     functions producing the conditions, perhaps "_cp" as a postfix? </li>
     <li> Could such condition sets also be used as a representation when
     dealing with active clauses? A generalised solver that looked for 
     operators within the set and then applied appropriate given functions
     for those operators that would determine heuristics etc? </li>
     <li> DONE (Not needed, we want a set of conditions to be produced)
     "nounify" appears to be useful here, to force functions such as
     "union" to become "nouns" rather than "verbs", which are then not 
     immediately evaluated. This is only an issue in some cases such as "union"
     where it expects it's arguments to be of the correct type and won't return
     the unevaluated expression if not, but instead returns an error. </li>
    </ol>
   </li>
   <li> Using the concept of a "Constraint" (rather than condition), where this
   can be represented by a tuple consisting of a name, a set of named variables
   and a function to evaluate it, given a partial assignment. </li>
   <li> Compare the discussion under "Condition" in
   ComputerAlgebra/Satisfiability/plans/SatisfactionProblems.hpp. </li>
   <li> The following needs updating, so that from the beginning
   we consider families of encoding, using different "granularity
   levels" for the "active clauses" used; see "Partitioning into active clauses"
   above. </li>
   <li> First the formulas F_AES(p, k, c) encoding
   the encryption via AES, and F'_AES(p, k, c) encoding
   the decryption via AES have to be created, as
   plain CNF. The formulas F_AES etc. actually need a
   further parameter 0 <= r <= 10, the number of rounds.
   </li>
   <li> That is, we can consider F_AES^r(p, k, c') for 0 <= r < = 10
   which encodes that encryption of p via k in r rounds yields c',
   and F'_AES^s(p', k, c) for 0 <= s < = 10
   which encodes that decryption of c via k in s rounds yields p'.
   We have  F_AES(p, k, c) = F_AES^10(p, k, c) and
   We have  F'_AES(p, k, c) = F'_AES^10(p, k, c).
   </li>
   <li> It seams reasonable to demand that F_AES^r(p, k, c') contains
   F_AES^r'(p, k, c') for r' <= r, and the same for F'_AES^s(p', k, c).
   </li>
   <li> The variables used in  F_AES^r(p, k, c') and F'_AES^s(p', k, c)
   must be consistenly named, so that these formulas can be combined.
   </li>
   <li> (Compare the 2006-BSc-project by Gareth Thomas. The difference
   between F_AES(p, k, c) and F'_AES(p, k, c) is in the use
   of the auxiliary variables.)
   </li>
   <li> Only later should we consider a full C++ implementation, but first
   we create a prototype in the computer-algebra system. DONE </li>
  </ul>


  \todo Active clauses for field operations
  <ul>
   <li> Likely the two best first candidates for active clauses
   are the S-box (as map GF(2^8) -> GF(2^8)) and multiplication with
   some constant a in GF(2^8)^* (again, as map GF(2^8) -> GF(2^8)). </li>
   <li> Both types of functions yield boolean functions in 16 variables. </li>
   <li> As bijections, they all have trivial DNF representations (with 256
   satisfying assignments). </li>
   <li> The CNF representations are more interesting (perhaps one should better
   say challenging), and one should study good CNF's.
    <ol>
     <li> Compute all prime implicates.
      <ol>
       <li> See below for some further comments. </li>
       <li> For every permutation of GF(2^8) we have at least
       2 * 2^8 * 8 = 2^12 = 4096 prime implicates, given by fixing 8 bit in either
       the input or the output. </li>
       <li> Are there others? In case of multiplication, the multiplication
       with 1 obviously has others. </li>
      </ol>
     </li>
     <li> Also other optimisation should be in reach. </li>
     <li> Computing optimal hitting-clause-set-representations should
     be possible (they allow many services needed for active clauses). </li>
     <li> Likely the most useful translation is obvious translation to CNF,
     from the DNF via the Tseitin-translation. Perhaps some optimisation can
     be applied. </li>
     <li> This translation works independent of the specific nature of the S-box
     etc. --- any table can be used. </li>
     <li> One can also run a SAT-solver on the DNF-representation, taken as CNF,
     in order to get a hitting clause-set representation of the falsifying
     assignments. Interesting whether something smaller than the full CNF
     is created. </li>
    </ol>
   </li>
   <li> Obviously also of interest are OBDD representations of these boolean
   functions.
    <ol>
     <li> One should come pretty close to finding an optimal variable
     ordering. </li>
    </ol>
   </li>
   <li> These allow efficient handling of all basic tasks for active clauses
   (see ComputerAlgebra/Satisfiability/Lisp/plans/SatisfactionProblems.hpp). </li>
   <li> Of course, special algorithms should be investigated. </li>
   <li> It seems, that actually the DNF representation we have, which actually
   is a full DNF, and just having 256 clauses, yields an unbeatable active
   clause:
    <ol>
     <li> Given any DNF representation D of a boolean function F, satisfiability
     of phi * F is just checked by testing whether phi * D is not the empty
     clause-set. </li>
     <li> In general, for a clause-set F, considered as CNF or DNF, the opposite
     representation is just obtained by the transversal hypergraph, from which
     non-clauses are eliminated, and which then is complemented. </li>
     <li> So forced literals for phi * D are literals which occur in every
     clause. This can be checked by just checking the literal degrees. </li>
     <li> And the number of satisfying assignments for a hitting D can be
     computed by the standard counting arguments. The given case is even
     simpler, since we have a full clause-set (where full clause-sets are
     stable under application of partial assignments), and so we just need
     to count the remaining clauses. </li>
     <li> Nevertheless we need to compute the prime-implicate representation,
     since the minimal size of a prime implicate tells us how many variables
     have to be set until we may obtain a contradiction --- this is important
     information for the analysis, and furthermore for the active clause it
     can be used as threshold which triggers some action (before, we are
     just lazy and don't do anything (w.r.t. updating the counters)). </li>
     <li> Given a full CNF F, isn't it then easy (relative to the size of F) to
     compute the set of prime implicates? (In our case we have 2^16 - 2^8 =
     65280 full clauses, which shouldn't be too bad.)
      <ol>
       <li> Yes; see
       Transitional/Satisfiability/FiniteFunctions/plans/general.hpp,
       "Prime implicants and implicates". </li>
      </ol>
     </li>
     <li> All these generalisations are very general, and should go to
     supermodule Satisfiability/ProblemInstances. </li>
    </ol>
    Using these active clauses should give us a good advantage over any CNF
    translation!
   </li>
   <li> We should aim at "high integration":
    <ol>
     <li> The more active clauses can manage the better. </li>
     <li> So we should have the full S-box an active clause, and not
     dividing it further. </li>
     <li> Perhaps in combination with the various permutations we can combine
     several "micro-steps" into one. Perhaps the ShiftRows step doesn't need
     to be made explicit at all. And also MixColumns operates on the bytes. </li>
     <li> Perhaps we create "generic active clauses" for these cases, and
     instantiate them appropriately (so that many variations of the same basic
     active clause appear). </li>
     <li> Identifying transformations of GF(2^8) seems most promising, since
     this yield active clauses with 16 bits, which can be thoroughly
     analysed. 32 bits likely is too much (since we won't have much
     exploitable structure(?)). </li>
    </ol>
   </li>
   <li> We have also the field addition, which can be broken down into binary
   xor, and perhaps a dedicated active clause(-set) handles all these equations
   over GF(2) (via Gaussian elimination). </li>
   <li> The main underlying theoretical question is whether the conditions
   "x * y = 1" and "a * x = y" are active clauses (for arbitrary GF(2^n).
    <ol>
     <li> This depends on the choice of literals (i.e., which partial assignments
     are allowed). </li>
     <li> A coarse choice is to use byte-valued variables (x, y here are just
     elements of the byte-field) and standard literals: Here we have active clauses,
     since the field operations as well as inversions can be performed in polynomial
     time. </li>
     <li> The next level (perhaps our preferred level) is the boolean level
     (now the bits of x and y can be queried). Using the natural representation
     of elements of GF(2^n) as vectors in the canonical base, we obtain
     one quadratic equation in both cases. If quadratic equations in GF(2^n)
     can be solved in polynomial time, then we are done, if not then we have
     to investigate whether the special form of equations we have matters or
     not. </li>
     <li> A fine level considers again byte-valued variables, but signed
     literals. It seems rather unlikely that this can be solved in polynomial
     time. However for n=8 perhaps we can perform heavy preprocessing? </li>
    </ol>
   </li>
  </ul>

*/

