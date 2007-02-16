// Oliver Kullmann, 12.11.2006 (Swansea)

/*!
  \file Cryptanalysis/plans/Cryptanalysis.hpp
  \brief Plans for the module on cryptanalysis

  \todo What about DES? (see \cite{MassacciMarraro2000DESSAT})
  Is this easier or harder than AES to implement?

  \todo In analogy to the general structure of the translation
  as discussed in Rijndael.hpp we can also consider RSA, but
  here the two directions (encryption and decryption) are really
  different (and thus one cannot consider a combined translation):
  <ul>
   <li> For the encryption direction we have primary variables (m, (n,e), c). </li>
   <li> For the decryption direction we have primary variables (m, (n,d), c). </li>
  </ul>
  Additionally, to get a better analogy, for both directions we use the
  private key (p, q, e) for the primary variables (in the encryption
  direction we derive (p*q, e) from it, in the decryption direction
  (p*q, d)).

  Here definitely active clause-sets seem to be needed
  (while a boolean translation looks hopeless); however obviously
  one must break the computations of p*q and of d into smaller parts
  (so that strong active clause-sets are obtained).

  \todo To SAT2007 there was a submission regarding MD4 and MD5.

*/

/*!
  \namespace OKlib::Cryptanalysis
  \brief Components for cryptanalysis via (generalised) SAT
*/

namespace OKlib {
  namespace Cryptanalysis {
  }
}

