// Oliver Kullmann, 20.7.2008 (Swansea)
/* Copyright 2008, 2009, 2010, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Cryptology/Lisp/CryptoSystems/Rijndael/plans/AdvancedEncryptionStandard.hpp
  \brief Plans on the AES implementation


  \todo Provide function for generating AES term
  <ul>
   <li> Running something like
   \verbatim
maxima> aes_encrypt_ibc_gen(p,k,1,s,mc);
   \endverbatim
   should result in a term representing AES which we can then rewrite
   and utilise in our explanation of the AES system.
   </li>
   <li> Currently however, we get the following error:
   \verbatim
fullmapl: argument must be a list or matrix; found: "+"
#0: lambda([a,b],matrixmap(rijn_stand,a+b))(a=p,b=k)
#1: ibc_0(buildq([sbox_f],lambda([key,r],aes_keyschedule(key,r,sbox_f))),buildq([sbox_f,mixcolumn_f],lambda([p,r],aes_round_ibc(p,r,sbox_f,mixcolumn_f))),lambda([a,b],matrixmap(rijn_stand,a+b)))(plain_text=p,key=k,n=2)
#2: aes_encrypt_ibc_gen(plaintext=p,key=k,num_rounds=1,sbox_f=s,mixcolumn_f=mc)
 -- an error. To debug this try: debugmode(true);
   \endverbatim
   </li>
   <li> One needs to make sure that each of the functions in the AES 
   implementation is written carefully to ensure it collapses correctly
   to a term when all relevant details are provided. </li>
   <li> One can generate a general term if we provide matrices of terms
   and specify the MixColumn operation:
   \verbatim
maxima> aes_encrypt_ibc_gen(genmatrix(lambda([a,b],nounify(p)(a,b)),4,4),genmatrix(lambda([a,b],nounify(k)(a,b)),4,4),1,nounify(s),rijn_mixcolumn);
matrix([('s('p(2,2)+'k(2,2))+'s('p(1,1)+'k(1,1)))*x
               +'s('p(4,4)+'k(4,4))+'s('p(3,3)+'k(3,3))+'s('k(2,4))+'s('p(2,2)+'k(2,2))+'k(1,1)+1,
              ('s('p(2,3)+'k(2,3))+'s('p(1,2)+'k(1,2)))*x
               +'s('p(4,1)+'k(4,1))+'s('p(3,4)+'k(3,4))+'s('k(2,4))+'s('p(2,3)+'k(2,3))+'k(1,2)
               +'k(1,1)+1,
              ('s('p(2,4)+'k(2,4))+'s('p(1,3)+'k(1,3)))*x
               +'s('p(4,2)+'k(4,2))+'s('p(3,1)+'k(3,1))+'s('p(2,4)+'k(2,4))+'s('k(2,4))+'k(1,3)
               +'k(1,2)+'k(1,1)+1,
              ('s('p(2,1)+'k(2,1))+'s('p(1,4)+'k(1,4)))*x
               +'s('p(4,3)+'k(4,3))+'s('p(3,2)+'k(3,2))+'s('k(2,4))+'s('p(2,1)+'k(2,1))+'k(1,4)
               +'k(1,3)+'k(1,2)+'k(1,1)+1],
             [('s('p(3,3)+'k(3,3))+'s('p(2,2)+'k(2,2)))*x
               +'s('p(4,4)+'k(4,4))+'s('k(3,4))+'s('p(3,3)+'k(3,3))+'s('p(1,1)+'k(1,1))+'k(2,1),
              ('s('p(3,4)+'k(3,4))+'s('p(2,3)+'k(2,3)))*x
               +'s('p(4,1)+'k(4,1))+'s('p(3,4)+'k(3,4))+'s('k(3,4))+'s('p(1,2)+'k(1,2))+'k(2,2)
               +'k(2,1),
              ('s('p(3,1)+'k(3,1))+'s('p(2,4)+'k(2,4)))*x
               +'s('p(4,2)+'k(4,2))+'s('k(3,4))+'s('p(3,1)+'k(3,1))+'s('p(1,3)+'k(1,3))+'k(2,3)
               +'k(2,2)+'k(2,1),
              ('s('p(3,2)+'k(3,2))+'s('p(2,1)+'k(2,1)))*x
               +'s('p(4,3)+'k(4,3))+'s('k(3,4))+'s('p(3,2)+'k(3,2))+'s('p(1,4)+'k(1,4))+'k(2,4)
               +'k(2,3)+'k(2,2)+'k(2,1)],
             [('s('p(4,4)+'k(4,4))+'s('p(3,3)+'k(3,3)))*x
               +'s('p(4,4)+'k(4,4))+'s('k(4,4))+'s('p(2,2)+'k(2,2))+'s('p(1,1)+'k(1,1))+'k(3,1),
              ('s('p(4,1)+'k(4,1))+'s('p(3,4)+'k(3,4)))*x
               +'s('k(4,4))+'s('p(4,1)+'k(4,1))+'s('p(2,3)+'k(2,3))+'s('p(1,2)+'k(1,2))+'k(3,2)
               +'k(3,1),
              ('s('p(4,2)+'k(4,2))+'s('p(3,1)+'k(3,1)))*x
               +'s('k(4,4))+'s('p(4,2)+'k(4,2))+'s('p(2,4)+'k(2,4))+'s('p(1,3)+'k(1,3))+'k(3,3)
               +'k(3,2)+'k(3,1),
              ('s('p(4,3)+'k(4,3))+'s('p(3,2)+'k(3,2)))*x
               +'s('k(4,4))+'s('p(4,3)+'k(4,3))+'s('p(2,1)+'k(2,1))+'s('p(1,4)+'k(1,4))+'k(3,4)
               +'k(3,3)+'k(3,2)+'k(3,1)],
             [('s('p(4,4)+'k(4,4))+'s('p(1,1)+'k(1,1)))*x
               +'s('p(3,3)+'k(3,3))+'s('p(2,2)+'k(2,2))+'s('k(1,4))+'s('p(1,1)+'k(1,1))+'k(4,1),
              ('s('p(4,1)+'k(4,1))+'s('p(1,2)+'k(1,2)))*x
               +'s('p(3,4)+'k(3,4))+'s('p(2,3)+'k(2,3))+'s('k(1,4))+'s('p(1,2)+'k(1,2))+'k(4,2)
               +'k(4,1),
              ('s('p(4,2)+'k(4,2))+'s('p(1,3)+'k(1,3)))*x
               +'s('p(3,1)+'k(3,1))+'s('p(2,4)+'k(2,4))+'s('k(1,4))+'s('p(1,3)+'k(1,3))+'k(4,3)
               +'k(4,2)+'k(4,1),
              ('s('p(4,3)+'k(4,3))+'s('p(1,4)+'k(1,4)))*x
               +'s('p(3,2)+'k(3,2))+'s('p(2,1)+'k(2,1))+'s('p(1,4)+'k(1,4))+'s('k(1,4))+'k(4,4)
               +'k(4,3)+'k(4,2)+'k(4,1)])
   \endverbatim
   </li>
  </ul>


  \todo Standardise data types and documentation
  <ul>
   <li> A comment section needs to be written at the top of
   the AdvancedEncryptionStandard.mac to describe the basic
   idea and the basic standard datatypes present. </li>
   <li> This should also be transferred to docus. </li>
   <li> DONE Currently the specifications (in comments) and docus
   are incomplete and inconsistent, due to changes from lists
   of integers to matrices of polynomials, and the original 
   poor definitions. </li>
  </ul>


  \todo Requirements
  <ul>
   <li> This should be expanded and tidied up with a more formal 
   specification? </li>
   <li> This specification should then be moved to the docus. </li>
   <li> What specifically is needed from the maxima implementation?
    <ul>
     <li> DONE A function F_AES(p,k) = c and F_AES'(c,k) = p . </li>
     <li> (DONE Moved to Generalisations) 
     How to generalise this to Rijndael? Perhaps F_Rijndael(p,k,r) where 
     the size of p and k is implicit based on the structures used for p and k,
     and r is given? Is r really needed as it seems this is a function of the 
     size of p and k? 
     </li>
     <li> DONE A separate round function to allow greater flexibility 
     when investigating reduced round variants. </li>
     <li> (DONE Moved to Generalisations) What sort of generalisations are
     needed?
      <ol>
       <li> Should the elements of the round such as ShiftRows be
       interchangable to some arbitrary permutation (perhaps just across
       rows?)? </li>
       <li> Should MixColumns be generalised to an arbitrary 4-Byte value
       multiplication, should it even be viewed in this way? See ??? </li>
      </ol>
     </li>
    </ul>
   </li>
  </ul>


  \todo Add todos.


  \bug DONE (Added missing "_rijn" specifier)
  Sbox vector calculation yields wrong size
  <ul>
   <li> rijn_sbox_vec([0,0,0,0,0,0,0,0]) = [1,1,0,0,0,1,1] </li>
  </ul>

*/

