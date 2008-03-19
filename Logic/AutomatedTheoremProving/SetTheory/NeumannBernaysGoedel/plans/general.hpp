// Oliver Kullmann, 19.3.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file AutomatedTheoremProving/SetTheory/NeumannBernaysGoedel/plans/general.hpp
  \brief General plans about formalising NBG


  \todo Create milestones.


  \todo Updating and extending axioms.bel
  <ul>
   <li> Is axioms.bel still valid for Prover9 ? </li>
   <li> Perhaps we should subdivide the file into different sub-files,
   for the different axiom-groups.
    <ol>
     <li> What support does Prover9 offer for using a more organised
     directory structure? </li>
     <li> And can we distinguish between different roles of formulas
     (like axioms, definitions, lemmas) ? </li>
    </ol>
   </li>
   <li> And perhaps we should even create sub-modules providing the
   basic definitions and lemmas directly regarding the axiom system. </li>
   <li> The axioms themselves need to be carefully considered, whether
   by providing new (Skolem) functions and predicates they can be improved.
    <ol>
     <li> As with the Bourbaki-system, perhaps we treat "pair(x,y)" as
     an undefined fundamental notion (an "ADT"). </li>
     <li> The constant "0" is used for the empty set, but apparently
     nowhere we have "- member(x,0)" ?? </li>
     <li> The "disjoint"-predicate is introduced, but then not used in
     the sequel. </li>
     <li> The set-predicate "x is a set" is expressed as "member(x,V)".
     Somewhat uggly. </li>
     <li> Why are some predicates written in all-capitals? </li>
     <li> Apparently the axiom of regularity got a name "AxReg", and also
     the axiom of choice "AxCh", likely so that they can be turned on
     and off. Perhaps the other axioms should get names as well> </li>
    </ol>
   </li>
   <li> And we need good documentation on each single axiom. </li>
  </ul>

*/

