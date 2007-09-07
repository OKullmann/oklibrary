// Oliver Kullmann, 25.7.2007 (Swansea)

/*!
  \file ComputerAlgebra/Cryptology/plans/Rijndael.hpp
  \brief Plans for cryptological tools regarding the Rijndael cipher


  \todo Computer algebra in general : DONE (tasks completed or delegated)
  <ul>
   <li> Compare the discussion under "Condition" in ComputerAlgebra/Satisfiability/plans/SatisfactionProblems.hpp.
   DONE (postponed until the design of the Maxima installation) </li>
   <li> Migrate the mupad-code. DONE (new todo) </li>
   <li> Perhaps there is already some implementation out there? DONE (doesn't seem
   to be the case) </li>
   <li> But in any case we need our own implementation, as elegant as possible,
   exploiting finite field arithmetic. DONE (yes) </li>
   <li> A module for the (pure) AES-implementation in the computer-algebra-system
   shall be created (we have already some Mupad-code). DONE </li>
  </ul>


  \todo New implementation : DONE (fundamental discussions completed)
  <ul>
   <li> Fundamental design discussion
    <ol>
     <li> Which of the system made available by Sage to use? DONE (concentrating
     on first Maxima then Axiom) </li>
     <li> Maxima: Does it have finite fields? DONE (we use some existing implementation)
     </li>
     <li> Axiom : DONE (comes later)
      <ul>
       <li> Axiom does have finite fields but how flexible is the implementation? </li>
      </ul>
     </li>
    </ol>
   </li>
  </ul>


  \todo Finishing old implementation (ComputerAlgebra/Cryptology/Rijndael.sage)
  <ul>
   <li> Write documentation on how to use this program (including how to load it into Sage). </li>
   <li> Outline the general design (as a starting point for further investigations). </li>
   <li> What is the meaning of the "Use By" paragraph? We should avoid machine-dependent
   information? </li>
   <li> Enter the existing code in the OKlibrary. DONE </li>
   <li> Of course, AES should also be generalised to Rijndael,
  using other block lengths than 128 bit and other cipher lengths
  than 128 bit. DONE </li>
  </ul>


  \todo Maxima: design
  <ul>
   <li> Create a general design (here in the plans) which is stepwise refined to a Maxima implementation. </li>
   <li> Compare the discussion under "Condition" in ComputerAlgebra/Satisfiability/plans/SatisfactionProblems.hpp. </li>
   <li> Look into whether/how maxima supports symbolic manipulation, so once a simple AES implementation is complete,
   basic equations can be generated for study. This is seemingly possible in Sage by generating variables form PolynomialRings
   and using these in the system although this seems to yield some issues with typing in some cases.</li>
  </ul>


  \todo Maxima: implementation issues
  <ul>
   <li> Some initial/example code for this has been added in ComputerAlgebra/Cryptology/AES.mac .
   OK : where does the suffix ".mac" come from? looks unmotivated to me? is this the standard
   ending for maxima-files, or for special ones? </li>
   <li> The finite field packages function names, along with maximas syntax make things a little verbose
   and perhaps a little longer than is really necessary although this is more of a nuisance than a real
   problem.
   OK : Why is there a nuisance? In general in the OKlibrary "full" names are appreciated; is this
   somewhat special here? </li>
   <li> Since we need the package, and it doesn't come with Maxima, we need to handle it as an ExternalSource. </li>
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

