// Matthew Gwynne, 28.7.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/plans/ConstraintTemplateSmallScaleRewriteRules.hpp
  \brief Plans for the small-scale AES constraint rewrite rules


  \todo Move AES box translations into separate file
  <ul>
   <li> Currently we have functions such as "aes_mul_ts_gen", and
   "ss_mul_pi_cst_cl" in
   ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/Translations.mac
   and
   ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/ConstraintTemplateSmallScaleRewriteRules.mac.
   </li>
   <li> It would be better to move these into a separate file where each type
   of box translation can be clearly separated. </li>
  </ul>


  \todo Update specifications
  <ul>
   <li> The comments and specifications in
   ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/ConstraintTemplateSmallScaleRewriteRules.mac
   and
   ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/ConstraintTemplateRewriteRules.mac
   are stale. </li>
   <li> The specifications need to be updated urgently. </li>
   <li> The notion of "constraint template" needs to be removed, as the
   objects we handle are constraints (in some sense). </li>
   <li> The notion of constraint needs to be discussed; see 'Notion of
   "constraint"' in
   Cryptanalysis/Rijndael/plans/ConstraintTemplateRewriteSystem.hpp. </li>
  </ul>


  \todo Remove hard-coding of multiplication by 01 in small-scale MixColumn
  <ul>
   <li> In the current of the small-scale MixColumn rewrite function, if the
   mixcolumns matrix contains a 1 in any position, then this is hard-coded
   in the rewrite function to be translated to an "eq_cst" constraint, which
   will then be removed by the global propagation stage later. </li>
   <li> A better way of handling this is to convert it to a multiplication
   constraint, and then simply have the multiplication constraint rewrite
   rule convert it to the "eq_cst" constraint. </li>
   <li> At the same time, one needs to handle multiplication by 0, which
   doesn't occur in any current AES or small-scale MixColumns matrices but
   will occur in later versions. </li>
   <li> Equalivalence constraints for trivial instances:
    <ul>
     <li> When an AES instance has only 1 row, then the MixColumns matrix
     is the identity matrix. </li>
     <li> In this instance, the multiplication is replaced by an equalivalence
     constraint which results in the variables being renamed. </li>
     <li> However, the additions that are part of the matrix computation
     are then arity 1 additions, i.e., equalivalence constraints, but these
     are not currently handled specially as equalivalence-constraints in the
     system. </li>
     <li> Therefore, when translating AES instances with 1 row, there
     are still some binary clauses in the translation representing
     equivalence of variables. </li>
     <li> These clauses need to be removed! </li>
     <li> There should be a simple check within the MixColumns constraint
     translation, which checks for "identity cases" and then rewrites the
     MixColumns constraint to a large equivalence constraint. </li>
    </ul>
   </li>
   <li> See also "Remove linear diffusion from translation" in
   Investigations/Cryptography/AdvancedEncryptionStandard/plans/SAT2011/KeyDiscovery/004/1_1_4/20_13.hpp.
   </li>
  </ul>


  \todo Rearranging linear components of Sbox and MixColumns
  <ul>
   <li> Due to the linearity of the Sboxes affine transform and the MixColumns
   operation, as well as the fact that the Shiftrows only permutes bytes,
   the linear/affine aspects of the Sbox can be moved out and combined with
   the MixColumns. </li>
   <li> The Sbox is essentially an operation (M . (s^(-1)) + Ac) where
   M is a bit matrix, A a bit vector and s^(-1) is inversion within the byte
   field. </li>
   <li> For each column [A,B,C,D] in the matrix, the MixColumns (for AES) is
   [M3 . A + M2 . B + C + D, A + M3 . B + M2 . C + D, A + B + M3 . C + M2 . D,
   M2 . A + B + C + M3 . D], where M3 and M2 are the bit matrices representing
   multiplication by 03 (x^2+1) and 02 (x) in the byte field respectively.
   </li>
   <li> We therefore have two additional possibilities other than the
   default AES translation:
   <ol>
    <li> The Sboxes affine constant addition becomes a separate operation
    giving:
    <ol>
     <li> Sbox: M . s^(-1)
     <li> MixColumns is the same as standard. </li>
     <li> Affine constant operation: +
     [M3 . Ac + M2 . Ac + Ac + Ac, ...,  M3 . Ac + M2 . Ac + Ac + Ac]
     performed after MixColumns and AddRoundKey. </li>
    </ol>
    </li>
    <li> The Sboxes affine constant addition becomes a separate operation and
    the linear matrix multiplication joins with the MixColumn:
    <ol>
     <li> Sbox: s^(-1)
     <li> MixColumns: Each column becomes:
     \verbatim
[(M. M3) . A + (M . M2) . B + M . C + M . D,
 M . A + (M . M3) . B + (M . M2) . C + M . D,
 M . A + M . B + (M . M3) . C + (M . M2) . D,
 (M . M2) . A + M . B + M . C + (M . M3) . D]
     \endverbatim
     </li>
     <li> Affine constant operation (same as without moving the linear matrix)
     : + [M3 . Ac + M2 . Ac + Ac + Ac, ...,  M3 . Ac + M2 . Ac + Ac + Ac]
     performed after MixColumns and AddRoundKey (all values are just
     constants).
     </li>
    </ol>
   </ol>
   </li>
   <li> Such rearrangements mean there need to be several additional
   functions written:
   <ul>
    <li> Each of the new boxes need minimum representations and generation
    functions, including (see SboxAnalysis.mac, FieldOperationsAnalysis.mac):
    <ul>
     <li> The combination of linear maps (i.e. M . M3, M . M2 etc for AES
     and small scale, including the multiplications for the inverse
     mixcolumns).
     </li>
     <li> The inverse map (i.e. s^(-1)). </li>
     <li> The inverse map with the linear matrix multiplication (i.e.
     M . s^(-1)). </li>
    </ul>
    </li>
    <li> Additional rewrite functions for the AES round which includes the
    affine constant addition as a separate constraint. </li>
    <li> Rewrite functions for the constraint denoting the affine constant
    addition. </li>
   </ul>
   </li>
  </ul>


  \bug DONE (Tests weren't updated when a new smaller S-box was found)
  Test failure with okltest_ss_sbox_pi_cst_cl
  <ul>
   <li>
   \verbatim
okltest_ss_sbox_pi_cst_cl(ss_sbox_pi_cst_cl)
ASSERT: Expression " {-10,-9,-7,-6,-5,3,4} = {-16,-15,-14,-13,4,11,12} " does not evaluate to true.
   \endverbatim
   </li>
  </ul>

*/

