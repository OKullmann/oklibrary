// Matthew Gwynne, 19.2.2008 (Swansea)
/* Copyright 2008, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Cryptology/Lisp/CryptoSystems/Rijndael/plans/general.hpp
  \brief Plans for the Rijndael crypto system in Maxima/Lisp


  \todo Tidy todos and update milestones
  <ul>
   <li> Milestone 0.0.10 should be marked as done and remaining issues
   moved to 0.0.11. </li>
   <li> Otherwise, there are only minor issues, including basic
   documentation remaining, which should make up the todos for
   0.0.11 and 0.0.12, after which point we should read 0.1. </li>
   <li> Other todos which we do not intend to do in the immediate
   future should then be moved to after 0.1. </li>
  </ul>


  \todo Notions and notations
  <ul>
   <li> Introduce systematic notations and abbreviations for basic
   "data types". </li>
   <li> At present there are several data types within the AES implementation, 
   namely:
    <ul>
     <li> Byte (represented as a polynomial or as an integer). </li> 
     <li> %Matrices of polynomials representing blocks. </li>
     <li> List of integers representing blocks.
    </ul>
   </li>
   <li> There are also other representations including:
   <ul>
    <li> Representing the whole AES block as an integer (see aes_int). </li>
    <li> Representing the whole AES block as a hexidecimal string (see 
    aes_encrypt_hex). </li>
   </ul>
   </li>
   <li>
   Given future generalisations, it seems best to consider 
    <ul>
     <li> polynomials - signified by "_p". </li>
     <li> integers - signified by "_i". </li>
     <li> %Matrices of polynomials - signified by "_pm" (i.e. polynomial 
     matrix). </li>
     <li> List of integers - signified by "_il" (i.e. integer list). </li>
     <li> Hexidecimal strings - signified by "_hex". </li>
    </ul>
   </li>
   <li> There is still the issue of some functions that use different, more 
   specific data types; either convenience functions etc (using the Rijndael 
   byte field, for example), or many other fields or permutations, if later 
   generalisations are considered.
    <ul>
     <li> For the Rijndael byte field, perhaps "_b" would be appropriate? </li>
     <li> What is an appropriate convention for others? Some general scheme 
     that can be used for all or most of such types? </li>
    </ul>
   </li>
  </ul>


  \todo Modularising the Rijndael-implementation
  <ul>
   <li> The following ideas need to be integrated into the plans (after
   milestone 0.1).
   <li> It would also be interesting to have an expression-based
   implementation.
    <ol>
     <li> Based on the basic abstract operations (perhaps there are options).
     </li>
     <li> Likely the whole creates an expression which is too big (since
     expression cannot re-use subterms). </li>
     <li> But one-round expressions should be feasible. </li>
    </ol>
   </li>
   <li> It would be better to parameterise the AES functions using global 
   variables which can then be overriden in blocks etc (simulating optional
   function parameters). Otherwise many options must be specified for which
   a default is usually the most appropriate. </li>
   <li> DONE All field-operations should be abstracted away (most obvious, by
   using an S-box function as parameter). </li>
   <li> DONE And then we provide different implementations:
    <ol>
     <li> As now. </li>
     <li> Via table-lookup (using arrays; this is then much faster). </li>
     <li> Also trivial (other) functions, for easy "variations" of
     Rijndael. </li>
    </ol>
   </li>
  </ul>


  \todo Docus and Demos
  <ul>
   <li> Demos are needed of the system. </li>
   <li> Docus are needed of the system. </li>
  </ul>


  \todo Coding Standards
  <ul>
   <li> Most important: Follow the standard coding practice!
    <ol>
     <li> Use named constants. </li>
     <li> Do not use abbreviated names except of well-established cases. </li>
     <li> Add code documentation for the specification of functions. </li>
    </ol>
    Furthermore declarations of local variables are needed.
   </li>
   <li> (DONE no longer an issue, as we don't use the gf package now) 
   The finite field packages function names, along with maximas syntax
   make things a little verbose and perhaps a little longer than is really
   necessary although this is more of a nuisance than a real problem.
   OK : Why is there a nuisance? In general in the OKlibrary "full" names are
   appreciated; is this somewhat special here?
   MG : It only arose as an issue due to my wish to keep the code relatively
   short and concise and so while trying to keep to a fixed line length of say
   80 characters, this meant that the code become much longer. The syntax when
   dealing with the binary operations as well doesn't seem to immediately make
   clear things such as associativity when reading, in my mind, potentially
   making simplification more difficult. I imagine that this is more my
   unfamiliarity with the language and more importantly problems occuring
   due to lack of design, as suggested below.</li>
  </ul>


  \todo DONE Update the following todos
  <ul>
   <li> Separate general plans from specific implementation details (which go
   to
   ComputerAlgebra/Cryptology/Lisp/CryptoSystems/Rijndael/plans/AdvancedEncryptionStandard.hpp.
   </li>
  </ul>


  \todo DONE The Rijndael cipher as an iterated condition system
  <ul>
   <li> See "AES as an ics" in
   ComputerAlgebra/Satisfiability/Lisp/plans/IteratedConditionSystems.hpp
   for the task of representing Rijndael in a certain boolean framework. </li>
  </ul>


  \todo (DONE implemented, individual issues should be raised separately,
  or now addressed in the documentation)
  Design of round functions
  <ul>
   <li> DONE The round functions have been implemented as discussed below with
   the exception that each takes a list of GF(2^8) elements as input rather 
   than GF(2) elements. </li> 
   <li> How should the implementation of the individual round operations be
   approached?
    <ul>
     <li> (DONE we now consider the MixColumn operation to take a column of 
     word elements, and it acts as a permutation over this space)
     The MixColumn operation, which would then take a list of GF(2) 
     elements or bits and convert it to a list of elements in the 4-Byte PID, 
     would simply be a mapping of the multiplication by the constant polynomial
     (2x^3 + 3x^2 + x + 1) across the list. </li>
     <li> (DONE matrix multiplication results in a simple, easy to read 
     solution and has been implemented) There may be a way to represent this
     multiplication by simple operations over polynomials and so on, or it may 
     have to be represented via a matrix multiplication over GF(2^8). </li>
     <li> DONE Such a  mapping would be general across AES and Rijndael. </li>
     <li> DONE The list would then be converted back to GF(2) using the 
     appropriate helper function. </li>
     <li> (DONE we now work at the word level using polynomials) 
     The SubBytes operation would take a list of GF(2) elements, convert
     this list to a list of GF(2^8) elements, and then map the S_rd operation
     across it. The resultant list would then be converted back to a list of
     GF(2)/bit elements. Such a mapping would be general across AES and
     Rijndael. Discussion is needed on potential generalisation to different
     parameters. </li>
     <li> (DONE See implementation) 
     The ShiftRows operation is simply the shift operation applied to each row
     in the block seperately depending on the size of the input. This can be
     achieved by taking the list of GF(2) elements, and partitioning it into a
     list of 4 lists of GF(2) elements, which can then simply be shifted by the
     appropriate amounts, perhaps determining the amount to shift/rotate by for
     each list/row by looking at the length of the input list. 
     </li>
     <li> (DONE see implementation)
     It seems that defining such operations as operations on matrices
     provides the most clear representation of each round function. The
     question of how to parameterise such an implementation, may be addressed
     by parameterising the round functions or at least their basic operations
     themselves. </li>
    </ul>
   </li>
   <li> DONE Should it be split up into each of the individual round functions
   ("MixColumns" etc) or should these be combined?
    <ol>
     <li> Combining them might make certain relationships/conditions more
     obvious, but it also makes things much less flexible and depending on how
     it is done, could make generalising to Rijndael harder. </li>
     <li> It seems a better understanding of the nature of the cipher might
     yield a different perspective that isn't as focused on these functions?
     </li>
    </ol>
    Further reading of Algebraic Aspects of the AES may yield some insight but
    this also appears to consider such functions (although generalised to
    produce a larger number of AES variants), perhaps this is most natural?
    This has been implemented as seperate functions, this can be returned to 
    later if deemed necessary.
   </li>
  </ul>
  
  
  \todo (DONE handled by SmallScaleAdvancedEncryptionStandard.hpp)
  Generalisations
  <ul>
   <li> There are several parameters which are considered when generalising 
   the AES in [Algebraic Aspects of the Advanced Encryption Standard].
   These are
    <ul>
     <li> Allowing arbitrary field elements as the basic unit of computation 
     rather than the Rijndael byte elements in GF(2^8). Such a unit is then 
     considered to be a "word" within the AES system. The parameter here is
     e which denotes the size of the finite field the element should be 
     considered in, i.e., the field used is GF(2^e).</li>
     <li> Allowing the number of columns n_C in the AES block to be varied. 
     </li>
     <li> Allowing the number of rows n_R in the AES block to be varied. </li>
    </ul>
   There are also then other generalisations which we wish to allow for
   further experimentation.
   <ul>
     <li> Generalisations based on [A simplified AES algorithm and it's linear
     and differential cryptanalyses] and other papers should also be 
     considered, and whether they can fit into the framework. </li>
     <li> [A simplified AES algorithm and it's linear and differential 
     cryptanalyses] simply uses a field size of 4 (i.e., GF(2^4) finite field)
     but uses different affine transforms in the Sbox and so on to
     the generalisations given in [Algebraic Aspects of the Advanced 
     Encryption Standard]. Such translations should fit reasonably into
     the same generalisation scheme. </li>
     <li> DONE Allowing arbitrary permutations in the place of the Sbox, 
     Mixcolumn, and field multiplications. </li>
   </ul>   
   </li>
   <li> Generalising this based on parameter n_R discussed in [Algebraic
   Aspects of the AES}, seems possible by parameterising the
   MixColumn operation, as this is the only operation which considers columns
   and their size. Such a parameterisation has already been done and so
   such new versions of MixColumn simply need to be written. </li>
   <li> Generalising over n_C should be trivial as the MixColumn operation
   only maps over the columns, and everything else either acts on bytes or
   rows. The ShiftRows operation is specified for each different value of n_C 
   in [Algebraic Aspects of the Advanced Encryption Standard] and so this must
   just be taken into account by the ShiftRows implementation. </li>
   <li> Generalising over e, seems to tie in closely with generalising over
   n_R, as the polynomial the elements in columns of the block form, are over
   elements of size e, and should just require using different functions
   for the Sbox and field multiplications (in conjunction with the different
   MixColumn operation), and such generalisations have been made already. 
   </li>
   <li> The small scale key schedule for each of these parameters is 
   relatively simple, in that it depends only on n_R and whereas the standard
   AES key schedule applies the Sbox and the addition of a constant to a 
   single byte and then applies various XORs across the column to generate a 
   new column, the small scale versions for n_R = 2 and n_R=1 simply apply the
   same Sbox and round constant addition (the small scale versions) but remove
   the additional XOR operations for the rows which are not present for
   the small scale versions. Such generalisations should be possible by
   simply parameterising the key schedule. See [Small Scale Variants of the 
   AES]. </li>
  </ul>

  
  \todo DONE Key Schedule
  <ul>
   <li> How to approach key scheduling?
    <ol>
     <li> AES and symmetric plaintext-key sizes provide a fairly elegant
     recursive key generation where each round key is just the result of
     the key generation on the last round key, whereas asymetric sizes such
     as 192bit plaintext and a 128bit key means you will have to use parts
     of the previous two round keys. </li>
     <li>  This seems to be done with an expanded key which is a large
     array/list of round keys in Design of Rijndael. </li>
     <li> Current implementation simply implements the key schedule as 
     described in [Design of Rijndael], taking a list of GF(2^8) elements
     and returning a longer list of GF(2^8) elements that can be partitioned
     into round keys. </li>
     <li> The keyschedule now uses a recursive scheme as the AES has been 
     restricted to symmetric message and key block sizes for simplicity. </li>
     <li> DONE A simpler, more elegant key schedule description should be 
     possible? </li>
     <li> DONE
     The Key Scheduling operation could simply take a list of GF(2) elements
     convert this to a list of GF(2^8) elements and perform the key schedule
     repeatedly in an iterative or recursive manner to produce the expanded
     key, for which the algorithm is relatively simple and described in Design
     of Rijndael and in various other places, returning an expanded key (list
     of GF(2) elements) of size of r+1 times the block size, which the
     individual round keys can then be extracted from using a helper function.
     </li>
    </ol>
   </li> 
  </ul>
  
  
  \todo Algebraic aspects DONE
  <ul>
   <li> (DONE Moved to Notions and notations)
   There seem to be 3 different representations considered when dealing
   with AES, byte sized, bit sized, and in 4 byte columns, each represented
   by different algebraic structures (GF(2^8),GF(2) and a Euclidean domain 
   (is this the closest fitting name?) respectively) </li>
   <li> (DONE Moved to Modularising the Rijndael-implementation) 
   Look into whether/how maxima supports symbolic manipulation, so once a
   simple AES implementation is complete, basic equations can be generated for
   study. </li>
   <li> (DONE Moved to Generalisations) 
   With the generalisations made to the implementation, can we provide
   further generalisations based on certain algebraic aspects of the AES? Is
   there any need? </li>
   <li> DONE Is this todo needed any longer? No. </li>
   <li> (DONE Lists are better for understanding and generalising here)
   The initial example code uses matrices as this seemed natural from the
   description of AES but this potentially makes generalisation to Rijndael
   more difficult unless a seperate implementation is created for each
   plaintext-key pair size configuration is made which seems excessive. </li>
   <li> DONE 
   Using maxima matrices in such ways also seems to bias the implementation
   to taking a view of the problem from one particular aspect (ie GF(2) bytes 
   etc) which may not be best. </li>
   <li> (DONE Not needed. Better achieved with lists and more fundamental 
   operations)
   The advantage of matrices is that, the semantics of the operations are
   more easily visible at a glance, although any well designed and implemented
   system would probably provide this. </li>
   <li> (DONE Lists were used. See "New design and implementation")
   Perhaps for use a simple list would be best as this seems to be the
   common unit in LISP and therefore maxima? </li>
   <li> (DONE Handled by other todos. See "New design and implementation")
   Create a general design (here in the plans) which is stepwise refined to a
   Maxima implementation. </li>
   <li> (DONE Sage implementation has been replaced by maxima)
   This is seemingly possible in Sage by generating variables from 
   PolynomialRings and using these in the system although this seems to yield 
   some issues with typing in some cases. </li>
  </ul>

*/

