// Oliver Kullmann, 25.7.2007 (Swansea)

/*!
  \file ComputerAlgebra/Cryptology/plans/Rijndael.hpp
  \brief Plans for cryptological tools regarding the Rijndael cipher


  \todo Computer algebra in general
  <ul>
   <li> Perhaps there is already some implementation out there? </li>
   <li> Migrate the mupad-code. </li>
   <li> Compare the discussion under "Condition" in ComputerAlgebra/Satisfiability/plans/SatisfactionProblems.hpp. </li>
   <li> But in any case we need our own implementation, as elegant as possible,
   exploiting finite field arithmetic. DONE (yes) </li>
   <li> A module for the (pure) AES-implementation in the computer-algebra-system
   shall be created (we have already some Mupad-code). DONE </li>
  </ul>


  \todo New implementation
  <ul>
   <li> Fundamental design discussion
    <ol>
     <li> Which of the system made available by Sage to use? </li>
     <li> Maxima: Does it have finite fields? 
      <ul>
       <li>Possibly through a third party package. (see http://www.math.utexas.edu/pipermail/maxima/2006/003255.html), 
       although this looks untested. </li>
      </ul>
     </li>
     <li> Axiom 
      <ul>
       <li>Axiom does have finite fields but how flexible is the implementation?</li>
      </ul>
     </li>
    </ol>
   </li>
  </ul>


  \todo Old implementation (deprecated; see ComputerAlgebra/Cryptology/Rijndael.sage)
  <ul>
   <li> Enter the existing code in the OKlibrary. </li>
   <li> Transfer it to the new system. </li>
   <li> Of course, AES should also be generalised to Rijndael,
  using other block lengths than 128 bit and other cipher lengths
  than 128 bit. DONE </li>
  </ul>


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

