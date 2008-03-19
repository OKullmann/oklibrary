// Matthew Gwynne, 19.2.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Cryptology/Lisp/CryptoSystems/Rijndael/plans/general.hpp
  \brief Plans for the Rijndael crypto system in Maxima/Lisp

  
  \bug Finite Fields package function doesn't terminate
  <ul>
   <li> This has been raised on the maxima bug tracker as #1913047 
   https://sourceforge.net/tracker/?func=detail&atid=104933&aid=1913047&group_id=4933
   </li>
   <li> "gf_set" doesn't terminate when passed 1 as an argument for the degree. 
   </li>
   <li> "gf_set(2,1,[x]);" leads to non-termination (MG : or at least it takes
   much longer than expected) </li>
   <li> This should be moved when the wrapper functions are moved into a 
   seperate module </li>
  </ul>


  \todo Convert AES functions to use more natural representation
  <ul>
   <li> Make functions take lists of bit vectors or more likely, lists of 
   integers corresponding to bit vectors.
    <ul>
     <li> Lists of integers seem more likely here as then the sbox lookup table
     can be changed to use simple arrays rather than the less well defined
     hashed arrays. </li>
    </ul>
   <li> This seems more friendly in terms of input and output, and makes more 
   sense when considering a wider use of AES. </li>
  </ul>

  
  \todo Modularising the Rijndael-implementation
  <ul>
   <li> The following ideas need to be integrated into the plans (after
   milestone 0.1).
   <li> All field-operations should be abstracted away (most obvious, by
   using an S-box function as parameter). </li>
   <li> And then we provide different implementations:
    <ol>
     <li> As now. </li>
     <li> Via table-lookup (using arrays; this is then much faster). </li>
     <li> Also trivial (other) functions, for easy "variations" of
     Rijndael. </li>
    </ol>
   </li>
   <li> It would also be interested to have an expression-based implementation.
    <ol>
     <li> Based on the basic abstract operations (perhaps there are options).
     </li>
     <li> Likely the whole creates an expression which is too big (since
     expression cannot re-use subterms). </li>
     <li> But one-round expressions should be feasible. </li>
    </ol>
   </li>
  </ul>


  \todo New design and implementation
  <ul>
   <li> Move items from below to here, if appropriate. </li>
   <li> Regarding generalisations (AES -> Rijndael -> small-scale variations):
   If they come natural then they are done now, otherwise later (after we have
   more experience). </li>
   <li> Document functions and constants with explanation of use </li>
   <li> DONE Move list helper functions to a seperate List module </li>
   <li> (DONE Moved to ComputerAlgebra/Cryptology/Lisp/plans/general.hpp) 
   Produce list of conversion functions (bit lists, hex etc) which can 
   later be moved to Cryptology. </li>
   <li> DONE How best to implement multiplication by the 4-byte constant in the 
    MixColumns step (or it's equivalent), as it is not a field element?
     <ol>
      <li> It seems Maxima has functions regarding various operations on
      polynomials (multiplication, quotient/modulus etc) but is this general
      enough in terms of the field it works over or is it specific to the reals? </li>
      <li> It appears you can override the functions used for multiplication etc and
      so generalising the field used, even if not easily possible, seems at
      least possible? </li>
      <li> Is there a great deal of benefit from representing the
      multiplication by this fixed constant in the 4-Byte PID/QR (or variants)
      or is it better to simply represent this as matrix operation, does this
      lose anything? </li>
      <li> It seems there is no real advantage to representing this as the 
      operation in the 4-byte PID/QR, so the approach of representing this as a
      matrix multiplication over GF(2^8) has been taken and seems adequate. 
      </li>
     </ol>
   </li>
   <li> DONE How to pass data around and represent field elements ?
    <ol>
     <li> It seems the simplest way to pass data around would be as a list of 
     bits, given that this is one of the simplest representations, it translates
     nicely when considering translation to SAT and translation from this to 
     finite field elements and elements within the 4-Byte PID seems trivial. </li>
     <li> This has now been achieved by passing around a list of bytes/GF(2^8)
     field elements. This seemed most natural as most operations can be 
     represented trivially as operations over GF(2^8). </li>
    </ol>
   </li>
   <li> DONE Basic aes-functions:
    <ol>
     <li> DONE S-box </li>
     <li> DONE round-function </li>
     <li> DONE key-schedule </li>
    </ol>
   </li>
   <li> Right form of abstraction : DONE (these problems will go away through proper design)
    <ol>
     <li> Approaching the implementation from the perspective of the 4-byte block
     using this package (and with sage's finite fields) seems to be a little harder
     as the 4-byte block only forms a ring and it needs to be looked into
     how to form rings that behave in the same way (with a given polynomial modulus etc).
     Trying to create a field object without checking if the modulus is irreducible and then
     somehow casting that to a Ring doesn't seem to work although I'm not sure why it would. </li>
     <li> It seems easier to me to express the system in terms of the byte field
     elements (GF(2^8)) as most of the operations  are easily expressed as operations
     on these elements although it doesn't appear that many others have looked at the 
     problem from the perspectives other than GF(2^8) and GF(2) so other perspectives
     (4-byte block, considering things as operations on integers etc) might yield
     something different. </li>
     <li> OK: The problem I see here is that there is no proper design --- design
     (in the plans) must come first! (Implementations come later!!) </li>
    </ol>
   </li>
  </ul>


  \todo Finite Field operations
  <ol>
   <li> Move the interface to the gf-package into a seperate module / .mac
   file. This should be moved to Algebra. MG : Should a new module 
   "ComputerAlgebra/Algebra" be created or does this mean LinearAlgebra? </li>
   <li> The gf_set operation in the finite field package takes a considerable
   amount of time to complete and doesn't seem to cache 
   any of the results in any way and so each call takes the same amount of 
   time and so simply calling gf_set inside wrappers, as has been done, 
   causes issues where these functions are called often (such as in the case 
   of egf_add).
   <ol>
    <li> Perhaps a potential solution would be to keep track of which field
    is currently being used, and only if the field being used is different 
    to the last call would gf_set be called? (although in general, this 
    concept of keeping global state in such a way seems like it will cause 
    problems) .</li>
    <li> It seems the best way to approach this is to provide an
    egf_set function that calls gf_set, which keeps track of
    the finite field being used in a global variable. Then each of the
    other functions then offer the option to pass in the field. If the field
    is passed in, then the operation is done in that field, but the overall
    global variable is not changed so any other operations are still done
    in the field set with egf_set, otherwise the operation is done in the 
    field set with egf_set. This would allow one to set a default field but then
    perform other operations in other fields if necessary without 
    interrupting the flow of code. </li>
    <li> Calling egf_set once in a file causes problems as then if that file 
    is loaded and then other operations are performed then this original set
    can not be relied on, but clearly there is an overhead (of around 2 
    seconds or more on a current machine) to this operation. </li>
    <li> Perhaps have this operation check to see if the field being set is
    different to the current field and only then call gf_set? </li>
   </ol>
   </li>
   <li> DONE Since we need the package, and it 
   doesn't come with Maxima, we need to handle it as an ExternalSource. 
    <ul>
     <li> What is the procedure for this?
      <ol>
       <li> At least make the package (pyhsically) available. </li>
       <li> Installation will at least unpack it, and put it to a right place. </li>
       <li> There should be configuration variables which help us using the package. </li>
      </ol>
     </li>
     <li> When doing this, it might be beneficial to add some basic functionality
     such as a gf_matadd (matrix addition within the set field) and so on which
     isn't currently offered but seems trivial to implement using the functions 
     defined in gf.mac, although whether this is useful would depend on the design. </li>
    </ul>
   </li>
   <li> Create an interface to the gf-package. DONE </li>
   <li> Create field-identification list [p, n, poly]. DONE </li>
   <li> Field-operation-functions take such an "object" as additional argument,
   to specify the field. DONE </li>
   <li> Finite fields? Possibly through a third party package. (see
   http://www.math.utexas.edu/pipermail/maxima/2006/003255.html), 
   although this looks untested. DONE (started using it) </li>
  </ol>

  
  \todo Key Schedule
  <ul>
   <li> How to approach key scheduling?
    <ol>
     <li> AES and symmetric plaintext-key sizes provide a fairly elegant recursive
     key generation where each round key is just the result of the key generation
     on the last round key, whereas asymetric sizes such as 192bit plaintext and a
     128bit key means you will have to use parts of the previous two round keys. </li>
     <li>  This seems to be done with an expanded key which is a large array/list
     of round keys in Design of Rijndael. </li>
     <li> Current implementation simply implements the key schedule as 
     described in [Design of Rijndael] taking a list of GF(2^8) elements
     and returning a longer list of GF(2^8) elements that can be partitioned
     into round keys. </li>
     <li> A simpler, more elegant key schedule description should be possible?</li>
     <li> DONE
     The Key Scheduling operation could simply take a list of GF(2) elements
     convert this to a list of GF(2^8) elements and perform the key schedule
     repeatedly in an iterative or recursive manner to produce the expanded key,
     for which the algorithm is relatively simple and described in Design of
     Rijndael and in various other places, returning an expanded key (list
     of GF(2) elements) of size of r+1 times the block size, which the individual
     round keys can then be extracted from using a helper function. </li>
    </ol>
   </li> 
  </ul>


  \todo Design of round functions
  <ul>
   <li> The round functions have been implemented as discussed below with the
   exception that each takes a list of GF(2^8) elements as input rather than
   GF(2) elements. </li> 
   <li> How should the implementation of the individual round operations be approached?
    <ul>
     <li> The MixColumn operation, which would then take a list of GF(2) 
     elements or bits and convert it to a list of elements in the 4-Byte PID, 
     would simply be a mapping of the multiplication by the constant polynomial
     (2x^3 + 3x^2 + x + 1) across the list. </li>
     <li> There may be a way to represent this
     multiplication by simple operations over polynomials and so on, or it may 
     have to be represented via a matrix multiplication over GF(2^8). </li>
     <li> Such a  mapping would be general across AES and Rijndael. </li>
     <li> The list would then be converted back to GF(2) using the appropriate
     helper function. </li>
     <li> The SubBytes operation would take a list of GF(2) elements, convert this
     list to a list of GF(2^8) elements, and then map the S_rd operation across it. 
     The resultant list would then be converted back to a list of GF(2)/bit elements.
     Such a mapping would be general across AES and Rijndael. Discussion is needed 
     on potential generalisation to different parameters.
     </li>
     <li> (DONE See implementation) 
     The ShiftRows operation is simply the shift operation applied to each row
     in the block seperately depending on the size of the input. This can be achieved 
     by taking the list of GF(2) elements, and partitioning it into a list of 4 lists of
     GF(2) elements, which can then simply be shifted by the appropriate amounts, perhaps
     determining the amount to shift/rotate by for each list/row by looking at the length
     of the input list. 
     </li>
     <li> It seems that defining such operations as operations on matrices provides the 
     most clear representation of each round function. The question of how to 
     parameterise such an implementation, may be addressed by parameterising
     the round functions or at least their basic operations themselves. </li>
     </ol>
     </li>
    </ul>
   </li>
   <li> DONE Should it be split up into each of the individual round functions
   ("MixColumns" etc) or should these be combined?
    <ol>
     <li> Combining them might make certain relationships/conditions more obvious,
     but it also makes things much less flexible and depending on how it is done,
     could make generalising to Rijndael harder. </li>
     <li> It seems a better understanding of the nature of the cipher might yield
     a different perspective that isn't as focused on these functions? </li>
    </ol>
    Further reading of Algebraic Aspects of the AES may yield some insight but this also 
    appears to consider such functions (although generalised to produce a larger number
    of AES variants), perhaps this is most natural?
    This has been implemented as seperate functions, this can be returned to 
    later if deemed necessary.
   </li>
  </ul>


  \todo Docus and Demos
  <ul>
   <li> Demos are needed of the system. </li>
   <li> Docus are needed of the system. </li>
  </ul>


  \todo Requirements
  <ul>
   <li> This should be expanded and tidied up with a more formal 
   specification? </li>
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
     <li> (DONE Moved to Generalisations) What sort of generalisations are needed?
      <ol>
       <li> Should the elements of the round such as ShiftRows be interchangable
       to some arbitrary permutation (perhaps just across rows?)? </li>
       <li> Should MixColumns be generalised to an arbitrary 4-Byte value
       multiplication, should it even be viewed in this way? See ??? </li>
      </ol>
     </li>
    </ul>
   </li>
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
   <li> The finite field packages function names, along with maximas syntax make
   things a little verbose and perhaps a little longer than is really necessary a
   lthough this is more of a nuisance than a real problem.
   OK : Why is there a nuisance? In general in the OKlibrary "full" names are
   appreciated; is this somewhat special here?
   MG : It only arose as an issue due to my wish to keep the code relatively
   short and concise and so while trying to keep to a fixed line length of say
   80 characters, this meant that the code become much longer. The syntax when
   dealing with the binary operations as well doesn't seem to immediately make
   clear things such as associativity when reading, in my mind, potentially making
   simplification more difficult. I imagine that this is more my unfamiliarity 
   with the language and more importantly problems occuring due to lack of design.
   as suggested below.</li>
  </ul>
  
  
  \todo Generalisations
  <ul>
   <li> Generalising this based on parameter n_R discussed in [Algebraic
   Aspects of the AES}, seems possible by use of a lookup function for the
   constant to multiply based on n_R, as well as looking up the field to
   multiply over. </li>
   <li> Generalising over n_C should be trivial as given a function on elements
   in the QR/PID of size n_R (columns), the result of MixColumns is just a
   mapping over n_C of these elements and so the n_C parameter seems irrelevant
   here. </li>
   <li> Generalising over e, seems to tie in closely with generalising over n_R,
   as the polynomial the elements in columns of the block form, are over elements
   of size e. </li>
  </ul>
  
  

*/

