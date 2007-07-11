// Oliver Kullmann, 16.12.2006 (Swansea)

/*!
  \file Cryptanalysis/plans/Rijndael.hpp
  \brief Plans for the module on factorisation


  \todo Overview:
  There are many other cryptanalytic scenarios which are
  worth investigating --- one needs an overview.


  \todo Computer algebra implementation
  <ul>
   <li> A module for the (pure) AES-implementation in the computer-algebra-system
   shall be created (we have already some Mupad-code). </li>
   <li> Perhaps there is already some implementation? </li>
   <li> But in any case we need our own implementation, as elegant as possible,
   exploiting finite field arithmetic. </li>
  </ul>


  \todo Rijndael:
  Of course, AES should also be generalised to Rijndael,
  using other block lengths than 128 bit and other cipher lengths
  than 128 bit.


  \todo Partitioning into active clauses
  <ul>
   <li> An overview on the possibilities of anatomising the AES-process into
   active clauses has to be gained. </li>
   <li> The roughest subdivision presents just the input-output relation (this
   shouldn't not be considerd as an active clause). </li>
   <li> At the finest level we have represented the whole of AES as a boolean
   CNF. </li>
   <li> Inbetween there are many possibilities to handle the round-computations. </li>
  </ul>


  \todo Encoding AES:
  <ul>
   <li> The following needs updating, so that from the beginning
   we consider families of encoding, using different "granularity
   levels" for the active clauses used. </li>
   <li> Only later should we consider a full C++ implementation, but first
   we create a prototype in the computer-algebra system. </li>
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
  </ul>


  \todo First tests AES:
  Once we have these formulas, the first test is how
  fast F_AES(p, k, ?) and F'_AES(?, k, c) can be solved ---
  this actually should be fast.
  Next is to solve F_AES(?, k, c) and F'_AES(p, k, ?) --- this
  could already be interesting, as a test for SAT solvers how
  good they understand the inversion.
  With
  <center>
    F2_AES(p, k, c) := F_AES(p, k, c) + F'_AES(p, k, c)
  </center>
  then both ways definitely should be fast.


  \todo Further tests:
  <ul>
   <li> Key inference:
        The most obvious next thing to do is to investigate
        key inference, that is, try to solve formulas
          F_AES(p, ?, c), F'_AES(p, ?, c), F2_AES(p, ?, c).
        And for r + s = 10 one can consider
          F_AES^r(p, ?, x) + F'_AES^s(x, ?, c).
   </li>
   <li> Partial key inference:
        Instead of investigating the full key inference problem,
        one could try how many bits of the key need to be supplied
        (as unit clauses) so that the problem finally becomes feasible.
   </li>
  </ul>


*/

