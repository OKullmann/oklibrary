// Oliver Kullmann, 23.6.2007 (Swansea)
/* Copyright 2007, 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Mupad/plans/general.hpp
  \brief Plans for the (deprecated) Mupad-module
  \deprecated To be replaced by Maxima.


  \todo Rewrite hypergraph transversal functions in Maxima
  <ul>
   <li> Mupad/Orthogonal.mup : Hypergraphs transversals </li>
   <li>
    <ol>
     <li> streichen, hinzufuegen, streichen_bedingt </li>
     <li> union_S, star </li>
     <li> ist_exakt_transversal </li>
     <li> Transversals_exakt (decides whether a hypergraph is an
     exact transversal hypergraph) </li>
     <li> enthaelt_disjunkt </li>
     <li> ist_speziell_exakt_transversal (decides whether a hypergraph
     is of the special form of exact transversal hypergraphs according
     to the conjecture) </li>
     <li> Transversals  : DONE</li>
     <li> sub_min_l, sub_min, ist_sub_min : DONE </li>
    </ol>
   </li>
  </ul>


  \todo Rewriting:
  All modules are to be redesigned and reimplemented in Maxima
  <ul>
   <li> Mupad/Design.mup : Just the incidence matrix of the Fano plane. </li>
   <li> Mupad/Hilfsfunktionen.mup : Various general functions
    <ol>
     <li> lesenHilfsfunktionen </li>
     <li> NormMod </li>
     <li> SYSTEM </li>
     <li> Zahl : DONE (available via reverse(polyadic2int(L,B))) </li>
     <li> untfak </li>
     <li> obfak </li>
     <li> fac </li>
     <li> vuntfak </li>
     <li> vbinom </li>
     <li> ExtHad </li>
     <li> MatrixRowList </li>
     <li> RowPermutation </li>
     <li> ZeroesMatrix </li>
    </ol>
   </li>
   <li> Mupad/Orthogonal.mup : others than above
    <ol>
     <li> Singular reductions : DONE
      <ol>
       <li> MUSAT1, SMUSAT1, MMUSAT1 : DONE (todo in
       ComputerAlgebra/Satisfiability/Lisp/MinimalUnsatisfiability/plans/general.hpp)
       </li>
       <li> SingDPuncontracted, ISingDPuncontracted : DONE (todo in
       Satisfiability/Lisp/Reductions/plans/DP-Reductions.hpp) </li>
       <li> SingExt : DONE </li>
       <li> ISingExt : DONE </li>
      </ol>
     </li>
     <li> PNNIndices, herm, hermdef, kernKV, hermdefKV : DONE </li>
    </ol>
   </li>
   <li> Mupad/SATCounting.mup : Precise calculations of satisfiability
   probabilities in the standard random-formula model (constant density) </li>
   <li> Mupad/TestenFormelGenAES.mup : Testing the random clause-set
   generator </li>
   <li> Mupad/VanderWaerden.mup : Computing optimal resolution trees etc.
   for van der Waerden-clause-sets </li>
   <li> DONE (moved elsewhere)
   Mupad/tau.mup : See ComputerAlgebra/Satisfiability/TauMachinery/plans/general.hpp. </li>
  </ul>

*/

