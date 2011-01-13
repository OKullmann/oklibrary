// Oliver Kullmann, 29.12.2008 (Swansea)
/* Copyright 2008, 2009, 2010, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/FiniteFunctions/plans/BasicFunctions.hpp
  \brief Plans regarding the 16 binary boolean functions


  \todo Naming conventions
  <ul>
   <li> We need the standard names for the 16 binary functions (which are
   then provided as constants).
    <ol>
     <li> Inputs are ordered by counting, that is, 00, 01, 10, 11. </li>
     <li> A function is then given by a 4-bit word, e.g., "or" is 0111. </li>
     <li> With Table 1 in Section 7.1.1 in [Knuth, The Art of Computer
     Programming: Introduction to Combinatorial Algorithms and Boolean
     Functions, 2008] we choose the following names. </li>
     <li> 0000: constant 0 (b2_0), 1111: constant 1 (b2_1) </li>
     <li> 0011: left projection (b2_lp), 0101: right projection (b2_rp) </li>
     <li> 1100: left complement (b2_lc), 1010: right complement (b2_rc) </li>
     <li> 0001: conjunction (b2_and), 0111: disjunction (b2_or) </li>
     <li> 1101: implication (b2_i), 0010: nonimplication (b2_ni) </li>
     <li> 1011: converse implication (b2_ci), 0100: converse nonimplication
     (b2_cni) </li>
     <li> 1110: nonconjunction (b2_nand), 1000: nondisjunction (b2_nor) </li>
     <li> 0110: exclusive disjunction (b2_xor), 1001: equivalence (b2_eq).
     </li>
    </ol>
   </li>
   <li> Shouldn't these names coincide with those in
   ComputerAlgebra/Satisfiability/Lisp/PropositionalLogic/plans/Formulas.hpp ?
   </li>
  </ul>


  \todo DONE (Moved to 
  ComputerAlgebra/Satisfiability/Lisp/Generators/plans/RandomClauseSets.hpp)
  Random boolean function
  <ul>
   <li> We need a script to generate a random boolean function
   in n variables. </li>
   <li> See 
   Experimentation/Investigations/Cryptography/AdvancedEncryptionStandard/plans/general.hpp. </li>
  </ul>

*/

