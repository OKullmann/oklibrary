// Oliver Kullmann, 19.3.2011 (Swansea)
/* Copyright 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/LinearConditions/plans/general.hpp
  \brief Plans regarding pseudo-boolean constraints


  \todo Connections
  <ul>
   <li> AES involves several linear maps; see
   Investigations/Cryptography/AdvancedEncryptionStandard/plans/Representations/general.hpp. </li>
   <li> See Investigations/BooleanFunctions/plans/Permutations/Linear.hpp
   for investigations into linear maps as boolean functions. </li>
  </ul>


  \todo The notion of a "linear constraint"
  <ul>
   <li> Consider an mxn matrix A of ZZ_2 and two vectors x, b of length n,
   with entries either literals or from ZZ_2 (the latter not for x). </li>
   <li> The condition to be modelled is "A * x = b". </li>
   <li> Likely for the specification of x, b one must explicitly distinguish
   between values 0, 1 and literals, since 1 can be a variable, and -1 is
   a literal, but could also be interpreted as +1 in ZZ_2. </li>
   <li> Complementation of literals is also to be distinguished from negation
   in ZZ_2. </li>
   <li> This could be represented as a list [A,x,[b1,b2]], where b1
   contains the constant parts of b, while b2 contains the literals. x as well
   as b1,b2 are lists, while A is a Maxima-matrix. </li>
   <li> One had then to sort the rows of A, x accordingly, which seems
   alright. </li>
   <li> A special case is an xor-constraint x_1 + ... + x_n = b with b in {0,1}
   (if b is a literal, then it can be moved to the left-hand side). </li>
   <li> This can be represented as a list [x, b], where x is a list of
   literals, and b in {0,1}. </li>
   <li> For discussions on the notion of constraint, see
   'Notion of "constraint"' in
   ComputerAlgebra/Cryptology/Lisp/Cryptanalysis/Rijndael/plans/Translations.hpp.
   </li>
  </ul>


  \todo Evaluation functions
  <ul>
   <li> We need functions to evaluate linear constraints. </li>
   <li> The evaluation function should:
   <ul>
    <li> Take (in some form) the (partial) assignment to variables in x.
    </li>
    <li> Return the new constraint after the application of the assignment.
    </li>
   </ul>
   </li>
   <li> Taking assignments as a set of literals seems best. This would be the
   same as with assignments relating to clause-sets. </li>
   <li> The names "apply_pa_lin" and "apply_pa_xor" should be enough. </li>
   <li> Each condition we implement is a specific notion. </li>
   <li> Therefore, adding "_cnd" or "_cst" or similar to specify that these
   are condition/constraint functions is unnecessary. </li>
  </ul>

*/
