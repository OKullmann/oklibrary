// Matthew Gwynne, 19.2.2008 (Swansea)
/* Copyright 2008, 2009, 2010, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Cryptology/Lisp/CryptoSystems/Rijndael/plans/general.hpp
  \brief Plans for the Rijndael crypto system in Maxima/Lisp


  \todo Tidy todos and update milestones
  <ul>
   <li> Otherwise, there are only minor issues, including basic
   documentation remaining, which should make up the todos for
   0.0.11 and 0.0.12, after which point we should read 0.1. </li>
   <li> Other todos which we do not intend to do in the immediate
   future should then be moved to after 0.1. </li>
   <li> DONE Milestone 0.0.10 should be marked as done and remaining issues
   moved to 0.0.11. </li>
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
   <li> Given future generalisations, it seems best to consider 
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

*/

