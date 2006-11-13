// Oliver Kullmann, 12.11.2006 (Swansea)

/*!
  \file Cryptanalysis/plans/Cryptanalysis.hpp
  \brief Plans for the module on cryptanalysis

  \todo Implementing AES
  First the formulas F_AES(p, k, c) encoding
  the encryption via AES, and F'_AES(p, k, c) encoding
  the decryption via AES have to be created, as
  plain CNF. The formulas F_AES etc. actually need a
  further parameter 0 <= r <= 10, the number of rounds.

  (Compare the 2006-BSc-project by Gareth Thomas. The difference
  between F_AES(p, k, c) and F'_AES(p, k, c) is in the use
  of the auxiliary variables.)

  \todo First test AES
  Once we have these formulas, the first test is how
  fast F_AES(p, k, ?) and F'_AES(?, k, c) can be solved ---
  this actually should be fast.
  Next is to solve F_AES(?, k, c) and F'_AES(p, k, ?) --- this
  could already be interesting, as a test for SAT solvers how
  good they understand the inversion.
  With
    F2_AES(p, k, c) := F_AES(p, k, c) and F'_AES(p, k, c)
  then both ways definitely should be fast.

  \todo Key inference AES
  The most obvious next thing to do is to investigate
  key inference, that is, try to solve formulas
    F_AES(p, ?, c), F'_AES(p, ?, c), F2_AES(p, ?, c).

  \todo Instead of investigating the full key inference problem,
  one could try how many bits of the key need to be supplied
  (as unit clauses) so that the problem finally becomes feasible.

  \todo There are many other cryptanalytic scenarios which are
  worth investigating --- one needs some overview.

  \todo Of course, AES could also be generalised to Rijndael,
  using other block lengths than 128 bit and other cipher lengths
  than 128 bit.

  \todo Possibly implementing F_AES etc. as active clause-sets
  is interesting?!?

  \todo What about DES? (see \cite{MassacciMarraro2000DESSAT})
  Is this easier or harder than AES to implement?

*/

/*!
  \namespace OKlib::Cryptanalysis
  \brief Components for cryptanalysis via SAT
*/

namespace OKlib {
  namespace Cryptanalysis {
  }
}

