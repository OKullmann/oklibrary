// Oliver Kullmann, 25.7.2007 (Swansea)
/* Copyright 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Cryptology/plans/Rijndael.hpp
  \brief Plans for cryptological tools regarding the Rijndael cipher


  \todo Finishing old implementation (ComputerAlgebra/Cryptology/Rijndael.sage)
  <ul>
   <li> Write documentation on how to use this program:
    <ol>
     <li> A basic problem is how to handle source code documentation (in the absence
     of doxygen?). 
     
     The option of simply including files in doxygen verbatim was suggested previously. This seems
     to be fairly simple if one creates a basic filter to wrap things up and then sets
     this as the input filter (it has some additional spaces due to use of verbatim tags
     inside verbatim tags) - 
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
     Some sort of make variables could then perhaps set the verbatim extensions with the 
     correct set of extensions that must be verbatim processed.

     This method would work and would probably be the most straightforward method for systems
     such as maxima where there is no supported language which is very similar within doxygen.
     
     In the case of sage, the syntax is so close to python that it seems a waste to be
     reduced to a simple dump of the code, and given that sage produces an intermediate python
     file which seems to maintain comments, it would seem fairly straightforward to generate
     a valid python file (although how to do this without actually generating an intermediate
     file with sage is something that needs invesigating) that doxygen would then support (as
     python is supported in recent releases of doxygen. This would then have to make use of
     either INPUT_FILTER or FILTER_PATTERNS in the Doxygen configuration.
     </li>
     <li> And we need the docus- as well as the demos-system. </li>
     <li> Also how to run these programs needs to be explained. </li>
    </ol>
   </li>
   <li> The main goal is cleaning up, so that this program can be used in the future
   (but no further developments on it). </li>
   <li> Most important a precise specification of the functions (and other components):
   What is the precise input, what is the precise output? </li>
   <li> Outline the general design (as a starting point for further investigations).
    <ul>
     <li> Split into MixColumns, ShiftRows and S_rd (which should really be called SBOX instead of the 
     matrix used in S_Rd as is done) with additional functions for the inverse of these operations. Ideally
     the inverses would be directly inferred from the functions themselves but I'm not sure if this is entirely
     impossible in this environment? OK : as said, no big changes, just making it nice and clean. </li>
     <li> Conversions:
      <ol>
       <li> Helper Functions for conversion between GF(2), GF(2^8) were made so that the different stages
       within the rounds could work on the data at their respective levels as described by the algorithm 
       (except for MixColumns due to issues with how to implement the 4-BYTE Ring that occurs). </li>
       <li> Was this really needed? It seems that better design here would have meant less of a need for some
       of these and some of them are fairly pointless given the fairly trivial nature of the conversions 
       (GF2t8ToNat for example). </li>
      </ol>
     </li>
     <li> For ShiftRows, a function was allowed to provide an arbitrary permutation. The default was one called
     "aesPerm", although this is perhaps badly named as it eventually became a more generalised version that
     represented the permutation of the byte elements within Rijndael. </li>
     <li> A lot of pythonesque structures and functions were used which might detract from the idea here. 
     Initially this was avoided when starting out but during the generalisation to Rijndael took place,
     a lack of design lead to a disorganised and inelegant method of generalisation. OK : can this
     be easily fixed? doesn't need to be perfect. </li>
     <li> Key schedule:
      <ol>
       <li> Initially when dealing with AES, the keyscheduler was simply a single function of the form 
       nextKey(k,r) where k is the previous round key and r is the round number, and the function would
       return the next round key. </li>
       <li> When generalising this to Rijndael, more than one previous round key is necessary to calculate the
       next due to the possibility of asymmetric plaintext-key sizes  and so rather than keeping track of 
       these keys, it seemed better to generate the entire expanded key (done in getExpandedKey) as a list
       of bytes using an altered version of the nextKey function internally and to then get each round key
       using a helper function called getRoundKey. </li>
       <li> This seems rather convoluted and the implementation of each of these functions is particularly messy
       and not particularly well thought out. </li>
      </ol>
     </li>
     <li> Some basic "tests" were added by way of assertions to ensure the implementation matched
     what encryption via AES or Rijndael should give. Ideally this would have better been seperated
     into a runnable sets of tests rather than assertions as it simply served to slow down the loading
     of the file into sage significantly, and a properly seperated set of runnable tests would have allowed
     more freedom and been more structured. </li> 
     <li> DONE (such considerations are for future developments)
     The overall view of the plaintext and key was that of a matrix of bytes/GF(2^8) elements. 
     This seemed to make some sense as this is how AES/Rijndael is described, however some papers 
     have described AES as a slightly altered vector rather than a matrix, and it seems that simply
     following the predescribed approach to AES/Rijndael implementation/conception may not necessarily be the best
     strategy. </li>
    </ul>
   </li>
   <li> Standardise variable names due to the suggested problems in design. </li>
   <li> Enter the existing code in the OKlibrary. DONE </li>
   <li> Of course, AES should also be generalised to Rijndael,
  using other block lengths than 128 bit and other cipher lengths
  than 128 bit. DONE </li>
   <li> What is the meaning of the "Use By" paragraph? We should avoid machine-dependent
   information? DONE (MG - This was intended to document how to use the file within sage. The inclusion
   of machine-dependent paths was an oversight on my part and has been corrected and further fixes
   and documentations will be made as per other todos.) </li>
  </ul>


  \todo Maxima: design
  <ul>
   <li> The following points need review --- the more general discussions belong to Axiom/Aldor
   (see below), and should be moved to there, while here we have to decide what is the "right"
   level of abstraction. </li>
   <li>What specifically is needed from the maxima implementation?
    <ul>
     <li> A function F_AES(p,k) = c and F_AES'(c,k) = p . </li>
     <li> How to generalise this to Rijndael? Perhaps F_Rijndael(p,k,r) where the size of p and k is implicit
     based on the structures used for p and k, and r is given? Is r really needed as it seems this is
     a function of the size of p and k? </li>
     <li> Potentially a separate round function to allow greater flexebility when investigating reduced round
     variants? </li>
     <li> What sort of generalisations are needed? Should the elements of the round such as ShiftRows be
     interchangable to some arbitrary permutation (perhaps just across rows?)? Should MixColumns be generalised
     to an arbitrary 4-Byte value multiplication, should it even be viewed in this way? </li>
    </ul>
   </li>
   <li>Should it be split up into each of the individual round functions (MixColumns etc) or should these
   be combined? Combining them might make certain relationships/conditions more obvious, but it also makes
   things much less flexible and depending on how it is done, could make generalising to Rijndael harder.
   </li>
   <li> How to approach key scheduling?
    <ol>
     <li> AES and symmetric plaintext-key sizes provide a fairly elegant recursive key generation where
     each round key is just the result of the key generation on the last round key, whereas asymetric sizes
     such as 192bit plaintext and a 128bit key means you will have to use parts of the previous two round keys. </li>
     <li>  This seems to be done with an expanded key which is a large array/list of round keys in Design of
     Rijndael. </li>
    </ol>
   </li>
   <li> How should the implementation of the individual round operations be approached?
    <ol>
     <li> The initial example code uses matrices as this seemed natural from the description of AES but
     this potentially makes generalisation to Rijndael more difficult unless a seperate implementation
     is created for each plaintest-key pair size configuration is made which seems excessive. Using maxima
     matrices in such ways also seems to bias the implementation to taking a view of the problem from one
     particular aspect (ie GF(2) bytes etc) which may not be best. </li>
     <li> The advantage of matrices is that, the semantics of the operations are more easily visible at
     a glance, although any well designed and implemented would probably provide this. </li>
     <li> Perhaps for use a simple list would be best as this seems to be the common unit in LISP and therefore
     maxima? This can be converted to a matrix if a given operation would be best accomplished this way... </li>
    </ol>
   </li>
   <li> Create a general design (here in the plans) which is stepwise refined to a Maxima implementation. </li>
   <li> Compare the discussion under "Condition" in ComputerAlgebra/Satisfiability/plans/SatisfactionProblems.hpp. </li>
   <li> Look into whether/how maxima supports symbolic manipulation, so once a simple AES implementation is complete,
   basic equations can be generated for study. This is seemingly possible in Sage by generating variables from PolynomialRings
   and using these in the system although this seems to yield some issues with typing in some cases.</li>
  </ul>


  \todo Maxima: implementation issues
  <ul>
   <li> Some initial/example code for this has been added in ComputerAlgebra/Cryptology/AES.mac .
   OK : where does the suffix ".mac" come from? looks unmotivated to me? is this the standard
   ending for maxima-files, or for special ones? MG - Looking through the maxima documentation and at
   the Finite Fields package and others, it seemed to be the common file extension, and due to a misunderstanding
   on my part, I believed that it was required, however I simply misunderstood the use of the load function and
   a more explicit file extension of .maxima following the TauMachinery example seems sensible. OK : one
   could ask this question on the (main) maxima mailing list. </li>
   <li> The finite field packages function names, along with maximas syntax make things a little verbose
   and perhaps a little longer than is really necessary although this is more of a nuisance than a real
   problem.
   OK : Why is there a nuisance? In general in the OKlibrary "full" names are appreciated; is this
   somewhat special here?
   MG : It only arose as an issue due to my wish to keep the code relatively short and concise and so while trying
   to keep to a fixed line length of say 80 characters, this meant that the code become much longer. The syntax when
   dealing with the binary operations as well doesn't seem to immediately make clear things such as associativity when
   reading, in my mind, potentially making simplification more difficult. I imagine that this is more my unfamiliarity 
   with the language and more importantly problems occuring due to lack of design.
   as suggested below.</li>
   <li> Since we need the package, and it doesn't come with Maxima, we need to handle it as an ExternalSource. 
    <ul>
     <li> What is the procedure for this?
      <ol>
       <li> At least make the package (pyhsically) available. </li>
       <li> Installation will at least unpack it, and put it to a right place. </li>
       <li> There should be configuration variables which help us using the package. </li>
      </ol>
     </li>
     <li> When doing this, it might be beneficial to add some basic functionality such as a gf_matadd (matrix addition 
     within the set field) and so on which isn't currently offered but seems trivial to implement using the functions 
     defined in gf.mac, although whether this is useful would depend on the design. </li>
    </ul>
   </li>
   <li> Right form of abstraction : DONE (these problems will go away through proper design)
    <ol>
     <li> Approaching the implementation from the perspective of the 4-byte block using this package (and with sage's
     finite fields) seems to be a little harder as the 4-byte block only forms a ring and it needs to be looked into
     how to form rings that behave in the same way (with a given polynomial modulus etc). Trying to create a field object
     without checking if the modulus is irreducible and then somehow casting that to a Ring doesn't seem to work although
     I'm not sure why it would. </li>
     <li> It seems easier to me to express the system in terms of the byte field elements (GF(2^8)) as most of the operations
     are easily expressed as operations on these elements although it doesn't appear that many others have looked at the 
     problem from the perspectives other than GF(2^8) and GF(2) so other perspectives (4-byte block, considering things
     as operations on integers etc) might yield something different. </li>
     <li> OK: The problem I see here is that there is no proper design --- design (in the plans) must come first!
     (Implementations come later!!) </li>
    </ol>
   </li>
   <li> Finite fields? Possibly through a third party package. (see http://www.math.utexas.edu/pipermail/maxima/2006/003255.html), 
   although this looks untested. DONE (started using it) </li>
  </ul>


  \todo Migrate the mupad-code.


  \todo Axiom: design and implementation


  \todo Partitioning into active clauses
  <ul>
   <li> This todo has to be updated according to ComputerAlgebra/Satisfiability/plans/SatisfactionProblems.hpp. </li>
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

*/

