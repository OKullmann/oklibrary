// Oliver Kullmann, 12.1.2006 (Swansea)
/* Copyright 2006 - 2007, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Logic/ComplexityTheory/plans/general.hpp
  \brief Plans for the module on complexity theoretical constructions


  \todo Update namespaces.


  \todo Module Computability
  <ul>
   <li> Connect to the development of Turing machines
   (see Logic/Computability/plans/general.hpp). </li>
   <li> Connect to the developments of Circuit models
   (see Logic/Computability/plans/general.hpp). </li>
  </ul>


  \todo NP-completeness
  <ul>
   <li> Implement the construction of SAT problems from
   the NP-completeness proof of SAT (see for example
   [Complexity and Cryptography: An Introduction; John Talbot and
   Dominic Welsh; 2006], Theorem 3.10): Given a Turing machine M and
   natural numbers n and t, construct F_M(n,t) which is satisfiable iff
   there exists an input x of size n such that M accepts x, and this
   in at most t steps. </li>
   <li> Perhaps this should go to module Computability? </li>
  </ul>


  \todo NP-completeness 2
  <ul>
   <li> Implement the construction of circuits from Turing machines
   as in [Complexity and Cryptography: An Introduction; John Talbot and
   Dominic Welsh; 2006], Theorem 4.20: Given an accepting TM M
   (with outputs 0,1) and n in NN, construct a circuit C_n which
   simulates M on inputs of size n. </li>
   <li> Translating these circuits into SAT problems we get an example for
   the above SAT-NP-completeness construction. On the other hand, from the
   CNF's constructed there by circuit-reconstruction we should be able to get
   the circuits here. </li>
   <li> Perhaps this should go to module Computability? </li>
  </ul>

*/

/*!
  \namespace OKlib::ComplexityTheory
  \brief Components for constructions from complexity theory (especially
  those related to propositional logic)

  Mainly for educational purposes; but potentially also interesting
  as a provider of SAT problems (and generalised SAT problems).
*/

namespace OKlib {
  namespace ComplexityTheory {
  }
}


