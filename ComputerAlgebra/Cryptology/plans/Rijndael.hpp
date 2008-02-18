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


  \todo Finishing old implementation (ComputerAlgebra/Cryptology/Rijndael.sage)
  <ul>
   <li> Likely we just remove this code (it is no longer properly maintained,
   and provides thus irritation). The following todos should be inspected,
   whether they contain some more general aspects, but otherwise they are
   outdated then. </li>
   <li> Also the docus etc. need to be inspected for something useful, but
   otherwise are removed then. </li>
   <li> Write documentation on how to use this program:
    <ol>
     <li> A basic problem is how to handle source code documentation (in the absence
     of doxygen?). </li>
     <li> See "Documentation" in ComputerAlgebra/plans/general.hpp. </li>
     <li> The option of simply including files in doxygen verbatim was suggested
     previously. This seems to be fairly simple if one creates a basic filter to
     wrap things up and then sets this as the input filter (it has some additional
     spaces due to use of verbatim tags inside verbatim tags):
     \verbatim
#!/usr/bin/python
import sys
import os

filename = sys.argv[1]
fileHandle = open(filename, 'r')
verbatimExtensions = ['.sage', '.mac']

root,ext  = os.path.splitext(filename)

if ext in verbatimExtensions :
    print "/*!"
    print "\\file " + filename
    print "\\verbatim"
    print fileHandle.read()
    print "\n\\ endverbatim * /"
else :
    print fileHandle.read()
     \endverbatim
     OK: What is the meaning of this??
     MG: Doxygen allows filters, which appear to just be scripts that take a filename
     as an argument, and then print out the file to be further processed by doxygen.
     This allows one to make custom filters (such as the above) that act based on
     file extensions or the structure of the code itself.
     Some sort of make variables could then perhaps set the verbatim extensions
     with the correct set of extensions that must be verbatim processed.
     MG : It seems best to consider this as a more general seperate issue (in
     another todo) and to look at the points raised in 
     ComputerAlgebra/plans/general.hpp</li>
     <li> This method would work and would probably be the most straightforward
     method for systems such as maxima, where there is no supported language which
     is very similar within doxygen. </li>
     <li> In the case of sage, the syntax is so close to python that it seems a
     waste to be reduced to a simple dump of the code, and given that sage produces
     an intermediate python file which seems to maintain comments, it would seem
     fairly straightforward to generate a valid python file (although how to do
     this without actually generating an intermediate file with sage is something
     that needs invesigating) that doxygen would then support (as python is
     supported in recent releases of doxygen. This would then have to make use of
     either INPUT_FILTER or FILTER_PATTERNS in the Doxygen configuration. </li>
    </ol>
   </li>
   <li> The main goal is cleaning up, so that this program can be used in the future
   (but no further developments on it). </li>
   <li> Most important a precise specification of the functions (and other components):
   What is the precise input, what is the precise output? </li>
   <li> Outline the general design (as a starting point for further investigations).
    <ul>
     <li> Split into MixColumns, ShiftRows and S_rd (which should really be called
     SBOX instead of the matrix used in S_Rd as is done) with additional functions
     for the inverse of these operations. </li>
     <li> Ideally the inverses would be directly inferred from the functions
     themselves but I'm not sure if this is entirely impossible in this environment?
     OK : as said, no big changes, just making it nice and clean. </li>
     <li> Conversions:
      <ol>
       <li> Helper Functions for conversion between GF(2), GF(2^8) were made so
       that the different stages within the rounds could work on the data at their
       respective levels as described by the algorithm (except for MixColumns due
       to issues with how to implement the 4-BYTE Ring that occurs). </li>
       <li> Was this really needed? It seems that better design here would have
       meant less of a need for some of these and some of them are fairly
       pointless given the fairly trivial nature of the conversions ("GF2t8ToNat"
       for example). </li>
      </ol>
     </li>
     <li> For ShiftRows, a function was allowed to provide an arbitrary permutation.
     The default was one called "aesPerm", although this is perhaps badly named as
     it eventually became a more generalised version that represented the permutation
     of the byte elements within Rijndael. </li>
     <li> A lot of pythonesque structures and functions were used which might detract
     from the idea here. Initially this was avoided when starting out but during
     the generalisation to Rijndael took place, a lack of design lead to a
     disorganised and inelegant method of generalisation. OK : can this
     be easily fixed? doesn't need to be perfect. </li>
     <li> Key schedule:
      <ol>
       <li> Initially when dealing with AES, the keyscheduler was simply a
       single function of the form "nextKey(k,r)" where k is the previous round
       key and r is the round number, and the function would return the next
       round key. </li>
       <li> When generalising this to Rijndael, more than one previous round
       key is necessary to calculate the next due to the possibility of
       asymmetric plaintext-key sizes  and so rather than keeping track of 
       these keys, it seemed better to generate the entire expanded key
       (done in "getExpandedKey") as a list of bytes using an altered version
       of the "nextKey" function internally and to then get each round key
       using a helper function called getRoundKey. </li>
       <li> This seems rather convoluted and the implementation of each of these
       functions is particularly messy and not particularly well thought out. </li>
      </ol>
     </li>
     <li> DONE (MG - A more standardised set of unit tests is an issue for
     future implementations in other computer algebra systems).
     Some basic "tests" were added by way of assertions to ensure the
     implementation matched what encryption via AES or Rijndael should give.
     Ideally this would have better been seperated into a runnable sets of tests
     rather than assertions as it simply served to slow down the loading of the
     file into sage significantly, and a properly seperated set of runnable tests
     would have allowed more freedom and been more structured. </li>
     <li> DONE (such considerations are for future developments)
     The overall view of the plaintext and key was that of a matrix of bytes/GF(2^8) elements.
     This seemed to make some sense as this is how AES/Rijndael is described, however some papers
     have described AES as a slightly altered vector rather than a matrix, and it seems that simply
     following the predescribed approach to AES/Rijndael implementation/conception may not necessarily be the best
     strategy. </li>
    </ul>
   </li>
   <li> Standardise variable names due to the suggested problems in design. DONE </li>
   <li> Enter the existing code in the OKlibrary. DONE </li>
   <li> Of course, AES should also be generalised to Rijndael,
   using other block lengths than 128 bit and other cipher lengths
   than 128 bit. DONE </li>
   <li> What is the meaning of the "Use By" paragraph? We should avoid machine-dependent
   information? DONE (MG - This was intended to document how to use the file within sage. The inclusion
   of machine-dependent paths was an oversight on my part and has been corrected and further fixes
   and documentations will be made as per other todos.) </li>
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
     <li> Likely the most useful translation is obvious translation to CNF,
     from the DNF via the Tseitin-translation. Perhaps some optimisation can
     be applied. </li>
     <li> This translation works independent of the specific nature of the S-boc
     etc. --- any table can be used. </li>
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


  \todo Docus and demos for the sage implementation : DONE
  <ul>
     <li> (DONE see ComputerAlgebra/Cryptology/demos/EncryptText.sage)
     We need the docus- as well as the demos-system for the Sage implementation. </li>
     <li> DONE (as proposed)
     Also how to run these programs needs to be explained. What are suitable
     examples here? Would a simple encryption scheme that takes a given ASCII string 
     message and produces the encrypted result, and then decrypts the result be a reasonable
     single example? Perhaps some use of the functions in producing pseudorandom data similar 
     in idea to the use of AES in Random generators elsewhere in OKlibrary? </li>
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

