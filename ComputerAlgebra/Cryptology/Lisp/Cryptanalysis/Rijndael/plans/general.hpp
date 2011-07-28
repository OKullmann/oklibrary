// Matthew Gwynne, 19.2.2008 (Swansea)
/* Copyright 2008, 2009, 2010, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/plans/general.hpp
  \brief Plans for the Cryptanalysis of Rijndael in Maxima/Lisp


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

*/

