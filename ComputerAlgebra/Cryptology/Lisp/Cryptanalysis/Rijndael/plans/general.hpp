// Matthew Gwynne, 19.2.2008 (Swansea)
/* Copyright 2008, 2009, 2010, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/plans/general.hpp
  \brief Plans for the Cryptanalysis of Rijndael in Maxima/Lisp


  \todo Update
  <ul>
   <li> All stale todos need to be removed (after careful inspection). </li>
   <li> All current todos need to be inspected and updated accordingly. </li>
   <li> Special care must be taken that with version 0.1 some form of
   completion is achieved. </li>
   <li> DONE (see Cryptanalysis/Rijndael/plans/)
   Plans-files are needed for all sub-modules (files), and all todos need
   to be moved there. </li>
   <li> DONE (see milestones.hpp) All todos need to appear in the milestones. </li>
   <li> DONE (see milestones.hpp)
   Milestones need to be updated, to reflect the current state. </li>
  </ul>


  \todo Connections
  <ul>
   <li> See Applications/Cryptanalysis/plans/Rijndael.hpp for the C++ level.
   </li>
   <li> See
   Investigations/Cryptography/AdvancedEncryptionStandard/plans/general.hpp
   for investigations. </li>
  </ul>


  \todo Functions should not cache return values
  <ul>
   <li> Functions such as ss_mul_ts_gen and ss_sbox_ts_gen should
   not cache their result (see
   ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/ConstraintTemplateSmallScaleRewriteRules.mac). 
   </li>
   <li> While this may save a considerable amount of time in
   some computations, it is poor practice as it causes these functions
   to have unintended side-effects. We should try to make as many functions as
   possible side-effect free. </li>
   <li> Instead, we should use functionality that allows us to cache the 
   results of arbitrary function calls. See "Introduce memoise function 
   wrapper" in ComputerAlgebra/DataStructures/Lisp/plans/HashMaps.hpp . </li>
  </ul>


  \todo Generating defaults constant for small scale
  <ul>
   <li> At present, we only have Sbox matrices, affine constants,
   modulo polynomials and so on defined when the field exponent is 
   1, 4 or 8, as these are either trivial or given in 
   [Small scale variants of the AES; Cid, Murphy and Robshaw]. </li>
   <li> We would like to extend this to arbitrary exponents (although
   we still restrict ourselves to binary bases for simplicity), and
   therefore we need defaults for the following values for each 
   exponent:
   <ul>
    <li> sbox linear transformation matrix:
    <ul>
     <li> This bit-level matrix must be a invertible matrix which
     is a linear operation over the GF(b^e) field. </li>
     <li> There are also other properties involving correlation
     coefficients and so on (regarding security against linear
     and differential cryptanalysis) that need to be considered
     here (see [Design of Rijndael; Daemen and Rijmen]).
     </li>
    </ul>
    </li>
    <li> sbox affine constant:
    <ul>
     <li> It seems this can be arbitrary but shouldn't be completely
     trivial (0,01 etc). </li>
     <li> We need more information on the choice of this. </li>
    </ul>
    </li>
    <li> Field polynomial:
    <ul>
     <li> We aren't concerned with efficient circuit implementation,
     so presumably this can be arbitrary, as long as it defines
     a genuine GF(b^e) field. </li>
    </ul>
    </li>
    <li> MixColumn matrix:
    <ul>
     <li> This byte/word-level matrix operation must be
     invertible (else we don't have a cipher). </li>
     <li> [Design of Rijndael; Daemen and Rijmen] also suggest that
     the matrix should be maximal distance separate. A matrix is MDS
     iff every square sub-matrix is non-singular. See section 9 and
     specifically 9.6 in [Design of Rijndael; Daemen and Rijmen], as
     well as [Small scale variants of the AES; Cid, Murphy and Robshaw].
     </li>
    </ul>
    </li>
   </ul>
   </li>
   <li> We would also like to be able to calculate all the various 
   measurements considered in the AES literature on these components,
   so as to compare any that we introduce against such metrics. </li>
  </ul>


  \todo Generating polynomial representations of field operations
  <ul>
   <li> Any small scale field operation can be represented as a 
   polynomial of degree at most (b^e)-1. </li>
   <li> There are at most b^e points and it forms a field, therefore
   we find this polynomial via linear interpolation. </li>
   <li> We need to implement linear interpolation over the field
   for arbitrary fields. </li>
   <li> This todo should be moved to
   Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/Representations/.
   </li>
  </ul>


  \todo Evaluating AES "constraints"
  <ul>
   <li> We consider the AES constraints in
   ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/ConstraintTemplateSmallScaleRewriteRules.mac.
   </li>
   <li> As these are called "constraints", we should be able to evaluate the
   constraints given an assignment to the variables. </li>
   <li> Functions are needed to perform this evaluation. </li>
   <li> See also "Create constraint evaluation system" in
   ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/DataEncryptionStandard/plans/general.hpp.
   </li>
   <li> See also 'Notion of "constraint"'. </li>
  </ul>


  \todo Rewrite translation functions using ss_field_op_fulldnf_gen_fcl etc
  <ul>
   <li> Functions such as ss_sbox_fulldnf_gen_fcl should be rewritten using
   ss_field_op_fulldnf_gen_fcl and passing in the appropriate operations.
   </li>
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
   previous research than simple smaller variants (with less rounds) of the
   cipher. </li>
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
   AES will not be broken and smaller variants with less rounds seem less
   interesting, than reducing parameters such as e. </li>
   <li> Especially because inversion within GF(2^8) can be expressed as
   operations on the inversion of the two GF(2^4) elements comprising it (see
   discussion on efficient implementation of AES in [Design of Rijndael]), and
   the relationships between these variants and the full AES seems less
   explored in previous research than variants of the
   cipher with less rounds. </li>
  </ul>

*/

