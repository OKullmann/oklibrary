// Oliver Kullmann, 12.11.2006 (Swansea)
/* Copyright 2006 - 2007, 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Applications/Cryptanalysis/plans/general.hpp
  \brief Plans for the module on cryptanalysis


  \todo Update namespaces
  <ul>
   <li> Using alias "CrA". </li>
   <li> Create declarations.hpp. </li>
  </ul>


  \todo DES
  <ul>
   <li> See <cite>MassacciMarraro2000DESSAT</cite>. </li>
   <li> Is this easier or harder than AES to implement? </li>
  </ul>


  \todo RSA
  <ul>
   <li> In analogy to the general structure of the translation
   as discussed in Rijndael.hpp we can also consider RSA, but
   here the two directions (encryption and decryption) are really
   different (and thus one cannot consider a combined translation). </li>
   <li> For the encryption direction we have primary variables (m, (n,e), c).
   </li>
   <li> For the decryption direction we have primary variables (m, (n,d), c).
   </li>
   <li> Additionally, to get a better analogy, for both directions we use the
   private key (p, q, e) for the primary variables (in the encryption
   direction we derive (p*q, e) from it, in the decryption direction
   (p*q, d)). </li>
   <li> Here definitely active clause-sets seem to be needed
   (while a boolean translation looks hopeless); however obviously
   one must break the computations of p*q and of d into smaller parts
   (so that strong active clause-sets are obtained). </li>
  </ul>


  \todo MD4, MD5
  <ul>
   <li> In SAT2007 there is a paper regarding MD4 and MD5 (De,
   Kumarasubramanian, Venkatesan). </li>
  </ul>

*/

/*!
  \namespace OKlib::Cryptanalysis
  \brief Components for cryptanalysis via (generalised) SAT
*/

namespace OKlib {
  namespace Cryptanalysis {
  }
}

