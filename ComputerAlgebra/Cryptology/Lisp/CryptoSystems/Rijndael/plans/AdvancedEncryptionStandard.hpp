// Oliver Kullmann, 20.7.2008 (Swansea)
/* Copyright 2008, 2009, 2010 Oliver Kullmann
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
   <li> Currently the specifications (in comments) and docus
   are incomplete and inconsistent, due to changes from lists
   of integers to matrices of polynomials, and the original 
   poor definitions. </li>
   <li> A comment section needs to be written at the top of
   the AdvancedEncryptionStandard.mac to describe the basic
   idea and the basic standard datatypes present. </li>
   <li> This should also be transferred to docus. </li>
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


  \todo DONE Sbox polynomial implementation
  <ul>
   <li> The polynomial representation should use operations
   on polynomials, but the Sbox operation is defined in 
   terms of an affine transform which works on a vector
   of bits. </li>
   <li> This results in the polynomial implementation of
   the Sbox function using a number of conversions and
   becoming quite convoluted. </li>
   <li> Better conversions functions between the data types,
   such as polynomials and maxima vectors (rather than just
   list vectors) should help, but one needs to also
   rethink how this can be best written to offer a simple
   and concise implementation. </li>
  </ul>

  
  \todo DONE Convert aes_key_expansion to return a list of matrices
  <ul>
   <li> Currently aes_key_expansion returns a 4x(4*(rounds+1)) matrix. </li>
   <li> This was motivated by the fact that it originally returned a list
   representation of the same structure, which was based on the formulation
   given in the AES standard. </li>
   <li> However, we can remove 2 conversions (to this matrix and back
   to the list) by simply returning a list of keys for each round. </li>
   <li> This should be much less confusing for the reader, and the only
   downside is that there is an additional type floating around
   (lists of matrices rather than simply matrices). </li>
  </ul>




  \todo DONE Remove addition from AES round
  <ul>
   <li> Removing the addition from the AES round will make
   the formulation easier as then 
    <ul>
     <li> there is no decision as to whether to include the addition before 
     the round or after within the round, and </li>
     <li> the implementation of the AES as an iterated block cipher
     (see CryptoSystems/IteratedBlockCipher.mac) becomes trivial
     as the functions that makeup the IBC are already defined. </li>
    </ul>
   </li>
   <li> This should be compared to the definition of AES and while
   straying from the definition is possible, it should only be done with 
   good reason. </li>
   <li> The standard definition of AES includes the key addition within the
   round and then the input to the first round is the plaintext added to
   the first round key block, although this differs from their standard
   definition of a "Key-Alternating Block Cipher" (a subset of Iterative
   Block Ciphers). </li>
   <li> It seems best to stick with the standard definition as given by the 
   book here, and so the round has been split into two parts, so that
   one function is available without the addition (postfixed with "_wa")
   and the normal round simply uses this function after applying
   the addition. </li>
  </ul>


  \todo DONE Alter internal AES functions to use polynomials and matrices
  <ul>
   <li> The functions used internally within the AES encryption
   implementation should use polynomials as the basic representation
   as then
    <ul>
     <li> there is no need for conversions to and from the 
     polynomial representations within each of these internal functions and 
     <li> the addition and multiplications within the byte field are
     simply the standard polynomials multiplications, modulo the rijndael
     polynomial (and such a standardisation can be applied at higher
     levels, although initially it should still be applied within
     each of these functions). </li>
    </ul>
   </li>
   <li> Additionally, using matrices of polynomials as a standard
   representations makes complete sense, as then the operations
   such as rijn_mixcolumns are just standard matrix multiplications,
   and there is again no need for unnecessary conversions. </li>
   <li> If natural, such as in the case of the sbox (where
   for experimentation etc, sometimes the different
   variations of the sbox can be used) the old versions
   (such as rijn_sbox_nat) can still be made available and 
   used in old functions (such as aes_key_expansion) until those
   functions have been updated to use polynomials as
   well. </li>
   <li> Functions to translate to matrix representation 
   (including lookup versions of any of the below):
    <ul>
     <li> aes_encrypt_l : DONE </li>
     <li> aes_decrypt_l : DONE </li>
     <li> aes_encrypt_f : DONE </li>
     <li> aes_decrypt_f : DONE </li>
     <li> aes_round : DONE </li>
     <li> aes_inv_round : DONE </li>
     <li> aes_key_expansion : DONE </li>
     <li> rijn_mixcolumns : DONE </li>
     <li> rijn_mixcolumn : DONE </li>
     <li> rijn_shiftrows : DONE </li>
     <li> rijn_subbytes : DONE </li>
    </ul>
   </li>
   <li> Such a translation should occur piecewise, first the translation
   to using polynomials, and then to using matrices. </li>
   <li> DONE Functions to translate to polynomial 
   representation (including lookup versions of any of the below):
    <ul>
     <li> aes_encrypt_l : DONE </li>
     <li> aes_decrypt_l : DONE </li>
     <li> aes_encrypt_f : DONE </li>
     <li> aes_decrypt_f : DONE </li>
     <li> aes_round : DONE </li>
     <li> aes_inv_round : DONE </li>
     <li> aes_key_expansion : DONE </li>
     <li> rijn_mixcolumns : DONE </li>
     <li> rijn_mixcolumn : DONE </li>
     <li> rijn_sbox : DONE </li>
    </ul>
   </li>
  </ul>

  \todo DONE Generating randomised AES parameters
  <ul>
   <li> There is no need to generate a random seed. Simply providing the 
   seed is enough, and randomly generating one gains nothing, and simply
   makes things more complicated as then this seed must always be recorded. 
   </li>
   <li> Otherwise, the same functionality can be had by simply doing:
   \verbatim
random_block : map(random, create_list(256,i,1,16))$
   \endverbatim
   </li>
   <li> For both testing and experimentation (see
   ComputerAlgebra/Cryptology/Lisp/CryptoSystems/Rijndael/plans/general.hpp), 
   we need some way of generating random plaintext, key and ciphertext blocks. 
   </li>
   <li> As plaintext, key and ciphertext blocks are of the same form, one
   function which generates a random block of a given size seems sensible. 
   </li>
   <li> Such a parameter generator needs:
   <ul>
    <li> to be able to take the number of bits for the block </li> 
    <li> To somehow return the random seed that it uses to generate this
    block. </li>
   </ul>
   </li>
   <li> Two functions are proposed:
   <ul>
    <li> One function takes a seed, and an integer "n" and returns a block 
    "p" where "n" is the number of block bits (in multiples of 32), and 
    "p" is the randomly generated block (as an integer list, as in the AES 
    functions themselves) given the random seed. 
    </li>
    <li> The other function takes an integer "n" and returns a list of the form
    [p,r_s] where "p" is as in the first function, given the randomly selected 
    seed "r_s". </li>
   </ul>
   </li>
  </ul>


  \todo Add todos.


  \todo DONE Truncate AES for reduced rounds
  <ul>
   <li> See "Truncate AES for reduced rounds" in 
   ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/plans/Translations.hpp
   . </li>
  </ul>


*/

