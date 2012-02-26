// Matthew Gwynne, 19.2.2008 (Swansea)
/* Copyright 2008, 2009, 2010, 2011, 2012 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/plans/general.hpp
  \brief Plans for the cryptanalysis of Rijndael in Maxima/Lisp


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
   Cryptanalysis/Rijndael/ConstraintTemplateSmallScaleRewriteRules.mac).
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


  \todo Removing parts of the AES
  <ul>
   <li> We need the ability to make each of the following
   components the identity (i.e, essentially remove them):
    <ul>
     <li> Key Schedule. </li>
     <li> MixColumns. </li>
     <li> S-boxes. </li>
    </ul>
   </li>
   <li> We must also implement this at the implementation
   level, as discussed in "Removing parts of the AES" in
   CryptoSystems/Rijndael/plans/SmallScaleAdvancedEncryptionStandard.hpp.
   </li>
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
   Cryptography/AdvancedEncryptionStandard/plans/Representations/.
   </li>
  </ul>


  \todo S-box and multiplication boolean 6xm functions
  <ul>
   <li> We wish to translate the AES where the S-box and multiplications are
   considered as 8 8x1 boolean functions. </li>
   <li> There is also the possibility of generalising this to 8/m boolean 8xm
   functions, or even combinations of 1 8x4 and 2 6x2 boolean functions etc.
   </li>
   <li> See also "S-box boolean 6xm functions" in
   Cryptology/Lisp/Cryptanalysis/DataEncryptionStandard/plans/general.hpp.
   </li>
  </ul>


  \todo Translating the constraint-system into SAT, CSP, ...
  <ul>
   <li> The SAT translation is available as output_ss_fcl_std in
   ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/Translations.mac.
   </li>
   <li> Constraint translation:
    <ul>
     <li> We must provide a translation of the "constraints"
     we generate in the AES translation to more concrete constraints.
     </li>
     <li> See 'Notion of "constraint"' in
     Cryptanalysis/Rijndael/plans/ConstraintTemplateRewriteSystem.hpp. </li>
     <li> For the DES constraint translation, see
     "Translating the constraint-system into SAT, CSP, ..." in
     Cryptanalysis/DataEncryptionStandard/plans/general.hpp. </li>
     <li> S-box constraints:
      <ul>
       <li> For the full AES-S-box, there is one type of S-box, so we could
       represent it as a constraint by a list [[v_1,...,v_8],[w_1,...,w_8]].
       </li>
       <li> For small-scale S-boxes we must specify *which* S-box is being
       considered. </li>
       <li> We represent a small-scale S-box as a constraint by the list
       [b,e,p,mat,c,[v_1,...,v_(b^e)],[w_1,...,w_(b^e)]] where
       the S-box represented is a finite function from {1,...,b^e} to
       {1,...,b^e} given by:
       \verbatim
S-box(b,e,p,mat,c)(v) = mod(mat . v + c,p)
       \endverbatim
       where
        <ul>
         <li> v is a vector of elements in {1,...,b} of length e; </li>
         <li> mat is an e x e matrix of elements in {1,...,b}; </li>
         <li> c is a constant vector with elements in {1,...,b} of length
         e; </li>
         <li> p is a polynomial over {1,...,b}. </li>
        </ul>
       </li>
      </ul>
     </li>
     <li> Multiplication constraints:
      <ul>
       <li> For the full AES-multiplcations, we could represent them as a
       constraint by a list [e,[v_1,...,v_8],[w_1,...,w_8]], where e is
       the element in {0,...,255} to be multiplied by. </li>
       <li> For small-scale multiplications further parameters are needed to
       specify *which* small-scale AES variant and multiplication are being
       considered. </li>
       <li> We represent a small-scale multiplication as a constraint by the
       list
       \verbatim
       [i,b,e,p,[v_1,...,v_(b^e)],[w_1,...,w_(b^e)]]
       \endverbatim
       where the constraints represents the multiplication of an element in
       {1,...,b^e} by i modulo the polynomial p resulting in an element again
       in {1,...,b^e}. </li>
      </ul>
     </li>
     <li> XOR constraints:
      <ul>
       <li> See 'The notion of a "linear constraint"' for the details of
       the XOR constraint. </li>
       <li> An XOR constraint is a list [x,b] where x is a list of literals
       and b is in {0,1}, such that the constraint is that
       \verbatim
x[1] + ... + x[l] = b
       \endverbatim
       where l is the length of x.
       </li>
      </ul>
     </li>
     <li> We must also consider the following constraints:
      <ul>
       <li> S-box linear component. </li>
       <li> Field inversion. </li>
       <li> Combined S-box linear component and field multiplications. </li>
       <li> MixColumn: MixColumns on a single column. </li>
       <li> 8 x m boolean function versions of all constraints. </li>
      </ul>
     </li>
    </ul>
   </li>
   <li> Translating high level constraints into positive boolean constraints:
    <ul>
     <li> This might be a good idea, especially for the S-box and
     multiplications, given their canonical DNF representations. </li>
     <li> We could translate a list of S-box and multiplication constraints
     directly to a positive boolean constraints; see
     ComputerAlgebra/Satisfiability/Lisp/ConstraintProblems/Conditions.mac.
     </li>
     <li> However, this would yield a much larger representation which
     would repeat the definition of the boxes for each instance of the box
     (modulo variable renaming), and hide the underlying constraints. </li>
     <li> Therefore, we reserve translation of constraints to the level
     of full clause-sets, or "truth tables" until we (possibly) later
     translate directly to some CSP solver input language. </li>
     <li> We need functions, taking the same parameters as ss_fcl:
      <ul>
       <li> ss_sboxc: generates the small-scale AES S-box constraints.
       </li>
       <li> ss_mulc: generates the small-scale AES multiplication
       constraints. </li>
       <li> ss_xor: generates the small-scale AES XOR constraints. </li>
       <li> Other constraints based on the linear components, field
       inversion and so on, which should be added to this list, as
       they are considered. </li>
      </ul>
     </li>
    </ul>
   </li>
   <li> Translating AES constraints to Minion:
    <ul>
     <li> We should provide a translation for input into the Minion solver.
     </li>
     <li> We could first provide a model of the Minion input language at
     the Maxima level, which is then only output to file, but such a language
     isn't likely useful for us outside of input to Minion; we already model
     the constraints in our own way; it is best to translate as we output to
     file. </li>
     <li> S-box and multiplication constraints should be translated to "table"
     constraints where we specify the truth table for each S-box. </li>
     <li> XOR constraints should be translated to "watchsumleq" and
     "watchsumgeq" constraints. </li>
     <li> We need a function "output_ss_minion" which takes the same
     parameters as ss_fcl and uses ss_sboxc, ss_mulc etc to generate
     the constraints associated with the AES given the input parameters,
     and outputs the corresponding Minion input file. </li>
     <li> We also need a function "output_ss_minion_6tm" which takes the
     number of rounds, and the parameter m and generates the constraints
     associated with the AES where the S-boxes and multiplications are
     considered as boolean 6 x m functions. See "S-box and multiplication
     boolean 6xm functions". </li>
    </ul>
   </li>
   <li> We should also translate to one of the constraint modelling languages;
   see "Constraint modelling languages" in
   Buildsystem/ExternalSources/SpecialBuilds/plans/CSP.hpp. </li>
  </ul>


  \todo Evaluating AES "constraints"
  <ul>
   <li> We consider the AES constraints in
   Lisp/Cryptanalysis/Rijndael/ConstraintTemplateSmallScaleRewriteRules.mac.
   </li>
   <li> As these are called "constraints", we should be able to evaluate the
   constraints given an assignment to the variables. </li>
   <li> Functions are needed to perform this evaluation. </li>
   <li> See also "Create constraint evaluation system" in
   Cryptology/Lisp/Cryptanalysis/DataEncryptionStandard/plans/general.hpp.
   </li>
   <li> See also 'Notion of "constraint"' in
   Cryptanalysis/Rijndael/plans/ConstraintTemplateRewriteSystem.hpp. </li>
  </ul>


  \todo Rewrite translation functions using ss_field_op_fulldnf_gen_fcl etc
  <ul>
   <li> Functions such as ss_sbox_fulldnf_gen_fcl should be rewritten using
   ss_field_op_fulldnf_gen_fcl and passing in the appropriate operations.
   </li>
  </ul>

*/

