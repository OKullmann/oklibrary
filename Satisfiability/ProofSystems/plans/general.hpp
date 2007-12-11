// Oliver Kullmann, 11.12.2007 (Swansea)
/* Copyright 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ProofSystems/plans/general.hpp
  \brief General plans on proof systems


  \todo Update namespaces.


  \todo Create milestones


  \todo Stronger propositional proof systems than resolution
  <ul>
   <li> Already the "Logic Theorist" (LT) considered derivation from
   a Hilbert-style calculus (from Principia Mathematica). </li>
   <li> This is likely the same as what in the proof-complexity-community
   are called "full Frege systems". </li>
   <li> The Gentzen-calculus (with cut) is another possibility. </li>
   <li> These systems should be more approachable than extended
   resolution. </li>
   <li> Can search for bounded Frege-proofs be implemented more efficiently?
   </li>
   <li> As with LT one could also consider incomplete search. </li>
   <li> Perhaps first a literature overview is needed. </li>
   <li> Then these systems should go into their own module, called for
   example "FregeSystems". </li>
  </ul>

*/

