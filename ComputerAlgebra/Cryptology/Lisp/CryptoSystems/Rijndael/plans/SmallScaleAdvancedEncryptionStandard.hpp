// Matthew Gwynne, 28.1.2010 (Swansea)
/* Copyright 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Cryptology/Lisp/CryptoSystems/Rijndael/plans/SmallScaleAdvancedEncryptionStandard.hpp
  \brief Plans on the AES implementation

  
  \todo Use matrix of GF(b) matrices for MixColumn
  <ul>
   <li> To allow the rearrangement of the small scale AES
   such that the linear component of the Sbox is moved into
   MixColumns, the MixColumn matrix should be a matrix with elements
   in the range {1,...,b} where b is the characteristic of the  
   small scale word field. </li>
   <li> Not every square e-dimensional matrix over {1,...,b}
   can be represented as a multiplication by an element in 
   GF(b^e), as there are two many elements, and so for
   such a generalisation, the MixColumns should be changed
   so that the collection of Sbox matrices can be integrated with it. 
   </li>
   <li> Note that because of the nature of the ShiftRows and addition,
   the ShiftRows and Subbytes commute, allowing such an alternative
   representation. </li>
  </ul>


  \todo Datatypes and naming conventions
  <ul> 
   <li> A naming scheme for each of the datatypes used, as well
   as for the whole class of small scale AES functions must be
   devised. </li>
   <li> For now, functions are prefixed with "ss_"  for "small-scale".
   </li>
  </ul>


  \todo Generating test vectors
  <ul>
   <li> An implementation of the small scale variants presented in
   [Algebraic Aspects of the Advanced Encryption Standard] and 
   in more detail in [Small Scale Variants of the AES] can be found 
   in the SAGE system (see 
   http://www.sagemath.org/doc/reference/sage/crypto/mq/sr.html). </li>
   <li> To run the encryption for AES given a plaintext and key, one
   may do the following in the sage system (using version 4.2.1 below):
   \verbatim
import sage.crypto
sr = mq.SR(10,4,4,8, star=true, allow_zero_inversions=true)
AES_e = sr(sr.state_array([0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0]), sr.state_array([0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0]))
AES_e
   \endverbatim
with 
\verbatim
[                        a^7 + a^5 + 1                 a^7 + a^6 + a^5 + a^2                         a^7 + a^2 + a                 a^6 + a^5 + a^4 + a^3]
[      a^6 + a^5 + a^4 + a^3 + a^2 + a             a^7 + a^6 + a^5 + a^4 + a                                   a^5       a^7 + a^6 + a^5 + a^3 + a^2 + a]
[        a^7 + a^4 + a^3 + a^2 + a + 1                   a^6 + a^4 + a^3 + a                 a^7 + a^5 + a^4 + a^2 a^7 + a^6 + a^5 + a^4 + a^3 + a^2 + 1]
[                  a^6 + a^5 + a^3 + 1                     a^7 + a^3 + a + 1         a^7 + a^5 + a^3 + a^2 + a + 1         a^6 + a^5 + a^3 + a^2 + a + 1]
\endverbatim
   as the result, and this can be converted to the hexidecimal format, like so
   \verbatim
sr.hex_str(AES_e, typ="vector")
   \endverbatim
   resulting in 
   \verbatim
'66E94BD4EF8A2C3B884CFA59CA342B2E'
   \endverbatim
   </li>
   <li> Note above that the first four parameters to the "sr" constructor are 
   r,n_R,n_C,e as specified above, and then 
   <ol>
    <li> the "star" parameter specifies that the small scale AES variant should
    include a different final round (as occurs with the standard AES), </li>
    <li> "allow_zero_inversions" specifies that the field inversion should be 
    extended such that 0^(-1) = 0 (as in the AES), and </li>
   </ol>
   </li>
   <li> There also appears to be an "aes_mode" parameter which suggests that 
   the key schedule implemented is somehow different to the AES, but this 
   does not seem to be the case in either the definition or in any basic
   tests so far. This needs to be further considered, or the authors should
   be e-mailed. </li>
   <li> This system is not currently available in the OKlibrary as there
   are there are issues compiling it in our system (see 
   "Update to new Sage" in 
   Buildsystem/ExternalSources/SpecialBuilds/plans/Sage.hpp). </li>
   <li> This information should be moved to the docus. </li>
  </ul>


  \todo (DONE see oklib_test_ss_encrypt) Tests for implementations
  <ul>
   <li> There should be generic tests (as is the case in 
   the AdvancedEncryptionStandard.mac tests) for the implementation
   of the small scale AES, such that we can test the implementations
   (which include the default values for various parameters)
   as well as the general versions of all the functions. </li>
   <li> For instance, we should be able to do
   \verbatim
okltest_ss_encrypt_generic(ss_encrypt);
   \endverbatim
   and so on.
   </li>
  </ul>


  \todo (DONE %Implementation done, remaining issues addressed in other todos)
  Small scale AES implementation
  <ul> 
   <li> The parameters in the small scale
   variations are the number of rows and columns (n_r,n_c), along with
   the size of the field used for the elements of the AES
   key and message blocks (e), along with the number of rounds (r). </li>
   <li> The small scale AES implementation is implemented in a separate
   file with it's own prefix ("ss") because
    <ul> 
     <li> the nature of the OKlibrary dictates that code should act as a 
     specification, and therefore should be readable in it's own right, without
     unnecessary generalisations, </li>
     <li> the small scale variations can both include or not the final round and
     so this would add unnecessary generalisations to the existing AES code, and 
     </li>
     <li> the code for both should be as simple as possible. </li>
    </ul>
    Therefore, it is better to simply write the code again (using the standard AES code
    as a template). </li>
    <li> Rather than just implement the standard small scale variations as given
    in [Algebraic Aspects of the Advanced Encryption Standard], several natural 
    generalisations should be made.
    <ul>
     <li> Rather than just allowing n_C and n_R to be 1, 2 or 4, we should allow
     the matrix to be an arbitrary size between 1 and 4, where
     <ul>
      <li> Shiftrows is generalised in the natural way. </li>
      <li> MixColumns will lookup the correct matrix for multiplication but
      this lookup may be undefined for n_C and n_R values not considered in 
      [Algebraic Aspects of the Advanced Encryption Standard]. </li>
      <li> KeyExpansion/KeySchedule is generalised in the natural way. </li>
     </ul>
     </li>
     <li> Rather than just allowing the block elements to be elements of GF(2)
     GF(2^4) and GF(2^8) fields, one should just allow arbitrary finite fields
     of the form GF(b^e) where then the standard variations use b=2. This would
     then require custom rijn_stand, Mixcolumn and Sbox functions but no more
     as all other functionality just works on standard polynomial operations
     which are then standardised (quotiented) as late as possible using 
     rijn_stand. This would require either generating (in a canonical way) or
     providing a polynomial to define the finite field (for the division, see
     rijn_polynomial). </li>
     <li> Allow simply any natural number as the number of rounds, as such
     an operation is well-defined. </li>
    </ul>
   </li>
   <li> Therefore, to implement the following, it seems best that there be two
   basic levels.
   <ul>
    <li> At the one level, we define a small scale AES which allows one to 
    specify arbitrary Sbox, MixColumn, and standardisation functions and the
    number of rounds. </li>
    <li> On the other level we provide wrapper functions which generate the 
    Sbox, MixColumn and standardisation functions given the parameters. </li>
   </ul>
   </li>
   <li> The basic data types should be the same as in the current AES 
   implementation where 
   <ol>
    <li> the byte/word elements (elements over the small field defined using 
    parameters p,e and the division polynomial) are polynomials, which
    are then standardised by the provided standardisation functions. </li>
    <li> the overall message and key blocks are maxima matrices where the
    elements of the matrices are the polynomials mentioned above. </li>
   </ol>
   </li>
   <li> [Algebraic Aspects of the Advanced Encryption Standard] defines two 
   classes of small scale variant, one (the default) which has the same round 
   function for every round, and one (the AES-like variant) which has a final 
   round which is different from other rounds.
    <ul>
     <li> We should provide both variations. </li>
     <li> What naming scheme to use to distinguish them? For now we use "_wf" 
     to stand for "with final round", as the standard is without.</li>
    </ul>
   </li>
   <li> (DONE No, this creates complications) 
   The small scale AES variants as described in
   [Algebraic Aspects of the Advanced Encryption Standard]
   should be implemented using the current system as a 
   basis. </li>
   <li> (DONE New system to allow readability and simplicity)
   Therefore, the small scale variations could be implemented with
   minimal changes to the current system by 
    <ul>
     <li> parameterising the rijn_stand, such that each function that uses
     it takes a function rijn_stand_f or something similarly named, and 
     then uses this standardisation function, so as to ensure polynomials
     are then standardised to the small scale fields rather than
     the standard AES byte field. </li>
     <li> moving the number of round to a parameter of the respective
     functions which use it. </li>
     <li> writing generic functions which generate Sbox and MixColumn 
     operations for the given small scale variations (with the
     relevant parameters). </li>
     <li> ensure all functions throughout the AES determine the
     number of rows and columns in the matrix purely from the
     matrix and do not assume columns are of size 4 etc. </li>
    </ul>
    and then a simple small scale AES function can be written 
    which takes the parameters for the small scale variation 
    and then calls the normal AES, using the above defined 
    functions and pre-existing parameter. </li>
   <li> (DONE New system to allow readability and simplicity)
   The current system 
    <ul>
     <li> arbitrary polynomials as the block elements of the AES, where
     these polynomials are then standardised using rijn_stand. </li>
     <li> has the number of rounds as a  variable (which should be moved to a 
     parameter of the function), and 
     <li> allows arbitrary functions to be used (passed as parameters) for 
     the Sbox function and MixColumn function, which are the only 2 functions 
     which work at the element/word level in AES, and </li>
     <li> takes matrices (which have their respective sizes implicitly as
     part of their structure/representation). </li>
    </ul>
   </li>
  </ul>
*/
