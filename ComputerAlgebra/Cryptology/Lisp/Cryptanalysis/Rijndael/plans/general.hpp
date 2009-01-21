// Matthew Gwynne, 19.2.2008 (Swansea)
/* Copyright 2008, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/plans/general.hpp
  \brief Plans for the Cryptanalysis of Rijndael in Maxima/Lisp

  
  \todo Review and tidy todos


  \todo Create / update milestones
  <ul>
   <li> The first goal is with milestone 0.1, where we have a working
   translation. </li>
  </ul>


  \todo Discussion on Generalisation/Parameterisation based on [Algebraic
  Aspects of the AES]
  <ul>
   <li> [Algebraic Aspects of the AES] discusses several generalisations of
   Rijndael. </li>
   <li> Rather than using GF(2^8), a parameter <em>e</em> is introduced, which 
   specifies 4 or 8 to indicate whether the block should deal in elements in 
   GF(2^4) or elements in GF(2^8) where appropriate modulo polynomials and
   S_rd affine transforms are defined for GF(2^4). This seems interesting
   because inversion within GF(2^8) can be expressed as operations on the
   inversion of the two GF(2^4) elements comprising it (see discussion on
   efficient implementation  of AES in [Design of Rijndael]). </li>
   <li> <em>n_R</em> is the number of rows in the block and may range over
   {1,2,4} where the normal AES/Rijndael default is 4. Clearly here the main
   issue is with Mixcolumns which works on the columns of size 4 and so
   different constants over these 1, 2 or 4 element polynomials but with the
   same basic operation involved (multiplying each column by a constant in
   that Quotient Ring). </li>
   <li> <em>n_C</em> is the number of rows in the block and may range over
   {1,2,4}. This only affects ShiftRows and as with n_R, variants are defined
   for each of these. </li>
   <li> <em>r</em> is the number of rounds as normal. </li>
   <li> Such abstractions seem to offer more interesting ways of generalising
   and producing AES/Rijndael variants with reduced complexity, which might
   offer better and possibly interesting results with translations and the
   relationships between these variants and the full AES seems less explored in
   previous research than simple reduced round variants of the cipher. </li>
   <li> So we get a more general parameterised AES function of the form
   AES(r,n_R, n_C,e)(P,K,C), where
    <ol>
     <li> r is the number of rounds, </li>
     <li> n_R is the number of rows in the block, </li>
     <li> n_C is the number of columns of length n_R, </li>
     <li> and e is the word size, normally 8 (ie GF(2^8) elements), but
     extended to include values of e of 4 or 8. </li>
    </ol>
   </li>
   <li> It would be nice to include such generalisations (more than the obvious
   round variable r, which is fairly trivial to include) as including a variety
   of parameters which can be reduced to make more easily attackable, and more
   thoroughly analysable AES variants is advantageous, as most likely the full
   AES will not be broken and simple reduced round variants seem less
   interesting, than reducing parameters such as e. </li>
   <li> Especially because inversion within GF(2^8) can be expressed as
   operations on the inversion of the two GF(2^4) elements comprising it (see
   discussion on efficient implementation of AES in [Design of Rijndael]), and
   the relationships between these variants and the full AES seems less
   explored in previous research than simple reduced round variants of the
   cipher. </li>
  </ul>

*/

