// Oliver Kullmann, 25.7.2007 (Swansea)
/* Copyright 2007, 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Cryptology/plans/Rijndael.hpp
  \brief Plans for cryptological tools regarding the Rijndael cipher

  
  \todo Restructuring
  <ul>
   <li> Link to module Structures/Cryptology (see
   Structures/Cryptology/plans/general.hpp). </li>
   <li> All the plans in this file should move to appropriate
   sub-modules of the Lisp-modules. </li>
   <li> DONE (moved to ComputerAlgebra/Cryptology/Lisp/plans/Rijndael.hpp)
   Some todos need to be reworked:
    <ol>
     <li> "Maxima: implementation issues" and "Maxima: design" needs
     unification/splitting. </li>
    </ol>
   </li>
   <li> Create sub-modules for "Lisp" and "Aldor". DONE </li>
   <li> DONE (yes, only Maxima and Axiom (at this time))
   Likely "Sage" is not treated on its own. </li>
  </ul>

  
  \todo Expand docus
  <ul>
   <li> Add basic overview of AES/Rijndael.</li>
   <li> Add additional terminology and refine current terminology</li>
  </ul>

  
  \todo Discussion on Generalisation/Parameterisation based on [Algebraic Aspects
  of the AES]
  <ul>
   <li> [Algebraic Aspects of the AES] discusses several generalisations of
   Rijndael. </li>
   <li> Rather than using GF(2^8), a parameter <em>e</em> is introduced, which 
   specifies 4 or 8 to indicate whether the block should deal in elements in 
   GF(2^4) or elements in GF(2^8) where appropriate modulo polynomials and S_rd 
   affine transforms are defined for GF(2^4). This seems interesting because 
   inversion within GF(2^8) can be expressed as operations on the inversion of 
   the two GF(2^4) elements comprising it (see discussion on efficient
   implementation  of AES in [Design of Rijndael]). </li>
   <li> <em>n_R</em> is the number of rows in the block and may range over {1,2,4}
   where the normal AES/Rijndael default is 4. Clearly here the main issue is with
   Mixcolumns which works on the columns of size 4 and so different constants over
   these 1, 2 or 4 element polynomials but with the same basic operation involved
   (multiplying each column by a constant in that Quotient Ring). </li>
   <li> <em>n_C</em> is the number of rows in the block and may range over {1,2,4}. 
   This only affects ShiftRows and as with n_R, variants are defined for each 
   of these. </li>
   <li> <em>r</em> is the number of rounds as normal. </li>
   <li> Such abstractions seem to offer more interesting ways of generalising and
   producing AES/Rijndael variants with reduced complexity, which might offer
   better and possibly interesting results with translations and the
   relationships between these variants and the full AES seems less explored in
   previous research than simple reduced round variants of the cipher. </li>
   <li> So we get a more general parameterised AES function of the form
   AES(r,n_R, n_C,e)(P,K,C), where
    <ol>
     <li> r is the number of rounds, </li>
     <li> n_R is the number of rows in the block, </li>
     <li> n_C is the number of columns of length n_R, </li>
     <li> and e is the word size, normally 8 (ie GF(2^8) elements), but extended
     to include values of e of 4 or 8. </li>
    </ol>
   </li>
   <li> It would be nice to include such generalisations (more than the obvious
   round variable r, which is fairly trivial to include) as including a variety
   of parameters which can be reduced to make more easily attackable, and more
   thoroughly analysable AES variants is advantageous, as most likely the full
   AES will not be broken and simple reduced round variants seem less interesting,
   than reducing parameters such as e. </li>
   <li> Especially because inversion within GF(2^8) can be expressed as operations
   on the inversion of the two GF(2^4) elements comprising it (see discussion on
   efficient implementation of AES in [Design of Rijndael]), and the relationships
   between these variants and the full AES seems less explored in previous research
   than simple reduced round variants of the cipher. </li>
  </ul>


  \todo Algebraic aspects
  <ul>
   <li> The initial example code uses matrices as this seemed natural from the
   description of AES but this potentially makes generalisation to Rijndael
   more difficult unless a seperate implementation is created for each
   plaintext-key pair size configuration is made which seems excessive. </li>
   <li> Using maxima matrices in such ways also seems to bias the implementation
   to taking a view of the problem from one particular aspect (ie GF(2) bytes etc)
   which may not be best. </li>
   <li> The advantage of matrices is that, the semantics of the operations are
   more easily visible at a glance, although any well designed and implemented
   would probably provide this. </li>
   <li> Perhaps for use a simple list would be best as this seems to be the
   common unit in LISP and therefore maxima? </li>
   <li> This can be converted to a matrix if a given operation would be best
   accomplished this way... </li>
   <li> There seem to be 3 different representations considered when dealing
   with AES, byte sized. </li>
   <li> Create a general design (here in the plans) which is stepwise refined to a
   Maxima implementation. </li>
   <li> Compare the discussion under "Condition" in
   ComputerAlgebra/Satisfiability/plans/SatisfactionProblems.hpp. </li>
   <li> Look into whether/how maxima supports symbolic manipulation, so once a
   simple AES implementation is complete, basic equations can be generated for
   study. </li>
   <li> This is seemingly possible in Sage by generating variables from PolynomialRings
   and using these in the system although this seems to yield some issues with typing
   in some cases. </li>
  </ul>


  \todo Migrate the mupad-code.
  <ul>
   <li> What is to be achieved here? A movement of the relevant AES code from
   Transitional/ComputerAlgebra/Mupad/TestenFormelGenAES.mup to a seperate mupad
   AES implementation in ComputerAlgebra/Cryptology/ with docus and demos attached?</li>
  </ul>


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


  \todo Terminology (DONE Moved to 
  ComputerAlgebra/Cryptology/docus/Rijndael.hpp . See "Expand docus" todo)
  <ul>
   <li> A bit could be called 
    <ul>
     <li> an element in GF(2) </li>
     <li> an element in the "bit field". </li>
    </ul>
   <li> An 8-bit element, representing a polynomial over GF(2) or an element
   in GF(2^8) can be called
    <ul>
     <li> an element in GF(2^8) </li>
     <li> an element in the "byte field". </li>
     <li> an element in the "rijndael byte field" </li>
    </ul>
   </li>
   <li> The most significant bit represents the highest order term in the polynomial,
   and values are usually written in hexadecimal notation, so, for example, 
   A2 is 10100010. Some clarification may be needed on the implementation details
   for the specific GF(2^8) field. </li>
   <li> A 4-byte element formed by taking a column of GF(2^8) elements in the 
   Rijndael block might be called
    <ul>
     <li> a 4-byte column </li>
     <li> an element in the 4-Byte PID (Principal Ideal Domain) </li>
     <li> an element in the QR (Quotient Ring) </li>
    </ul>
   as this is not actually a field (some elements don't have an inverse as the 
   modulus x^4 + 1 isn't irreducible over GF(2^8)). </li>
   <li> Which of these is most appropriate? It seems "Quotient Ring" but further
   reading is necessary and perhaps a more elegant and more specific name can be 
   found. Again, the most significant byte of the 4-bytes represents the coefficient
   of the highest order term in the polynomial. </li>
  </ul>

*/

