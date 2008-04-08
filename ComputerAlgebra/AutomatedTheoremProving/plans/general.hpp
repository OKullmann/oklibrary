// Oliver Kullmann, 8.4.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/AutomatedTheoremProving/plans/general.hpp
  \brief Plans for automated theorem proving using computer algebra systems

  A long-term goal is to provide a experimental systems where SAT and ATP
  coexist, so that approaches for integrating SAT into ATP can be studied.


  \todo Clause-based approach
  <ul>
   <li> First we need a representation of terms. </li>
   <li> Then we need to implement unification algorithms. </li>
   <li> Then the various inference rules need to be implemented:
    <ol>
     <li> resolution rules </li>
     <li> equality rules. </li>
    </ol>
   </li>
   <li> Finally, the strategies guiding the inference process are
   implemented. </li>
  </ul>


  \todo Connections
  <ul>
   <li> Currently (8.4.2008) the only examples come from formalising
   set theory: See AutomatedTheoremProving/SetTheory/NeumannBernaysGoedel/plans/general.hpp. </li>
  </ul>
  
*/

