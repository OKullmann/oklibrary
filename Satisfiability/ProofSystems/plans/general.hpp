// Oliver Kullmann, 11.12.2007 (Swansea)
/* Copyright 2007, 2008, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/ProofSystems/plans/general.hpp
  \brief General plans on proof systems


  \todo DONE Update namespaces.


  \todo Create further milestones


  \todo Stronger propositional proof systems than resolution
  <ul>
   <li> Perhaps first a literature overview is needed; [Segerlind 2007;
   The complexity of propositional proofs] should help. </li>
   <li> In ComputerAlgebra/Satisfiability representations and proof checkers
   for all systems should be established. And known proof translations.
   See "Proof systems" in ComputerAlgebra/Satisfiability/Lisp/plans/general.hpp.
   </li>
   <li> Res(k) (here "literals" can be conjunctions of up k boolean
   literals).
    <ol>
     <li> This should have a natural interpretation via splitting trees. </li>
     <li> So as resolution is connected with normal clause-sets, Res(k) is
     connected to generalised clause-sets where literals consists of up to
     k boolean literals. </li>
    </ol>
   </li>
   <li> Frege systems:
    <ol>
     <li> Already the "Logic Theorist" (LT) considered derivation from
     a Hilbert-style calculus (from Principia Mathematica). </li>
     <li> This is likely the same as what in the proof-complexity-community
     are called "full Frege systems". </li>
     <li> The Gentzen-calculus (with cut) is another possibility. </li>
     <li> These systems should be more approachable than extended
     resolution. </li>
     <li> Can search for bounded Frege-proofs be implemented more efficiently?
      <ol>
       <li> Adding (just) counting axioms modulo m, or adding counting gates
       (and corresponding axioms)?! For what can this be used? </li>
      </ol>
     </li>
     <li> As with LT one could also consider incomplete search. </li>
     <li> Then these systems should go into their own module, called for
     example "FregeSystems". </li>
    </ol>
   </li>
   <li> Algebraic systems:
    <ol>
     <li> Nullstellensatz </li>
     <li> Polynomial calculus </li>
     <li> Polynomial calculus with resolution </li>
    </ol>
   </li>
   <li> Cutting planes
    <ol>
     <li> Compare with ActiveClauseSets/PseudoBoolean/plans/general.hpp. </li>
    </ol>
   </li>
   <li> Lovasz-Schrijver systems </li>
   <li> Systems using OBDD's
    <ol>
     <li> More generally, any system of active clauses could be used. </li>
    </ol>
   </li>
  </ul>


  \todo Systems related to extended resolution
  <ul>
   <li> OK's blocked clauses.
    <ol>
     <li> One could only allow a restricted number of new variables, and then add all
     possible blocked clauses. </li>
    </ol>
   </li>
   <li> OK's notion of "quasi-autarkies", which again lead to clauses which do
   not logically follow, and which can speed up resolution proofs. </li>
   <li> Frege systems with substitution. </li>
   <li> Quantified Frege systems. </li>
  </ul>

*/

/*!
  \namespace OKlib::Satisfiability::ProofSystems
  \brief Super-module on proof systems

  Alias "PSy".
*/

namespace OKlib {
  namespace Satisfiability {
    namespace ProofSystems {
    }
    namespace PSy = ProofSystems;
  }
}

