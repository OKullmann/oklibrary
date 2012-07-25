// Oliver Kullmann, 30.11.2007 (Swansea)
/* Copyright 2007, 2008, 2009, 2010, 2011, 2012 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Hypergraphs/Lisp/plans/general.hpp
  \brief Plans for Maxima-components for hypergraphs


  \todo Place of IndependentSets.mac
  <ul>
   <li> The question is whether this belongs to module Transversals or not.
   </li>
   <li> Yet it is some kind of interface, which could stay at this top level.
   </li>
  </ul>


  \todo Write tests


  \todo Redesign
  <ul>
   <li> See "Plan the redesign" in ComputerAlgebra/plans/Maxima.hpp for
   the general overview. </li>
   <li> See "Lists instead of sets" and "Naming conventions" below. </li>
  </ul>


  \todo Write docus
  <ul>
   <li> We should have a list of all available functions. </li>
   <li> Seems that this list needs to be maintained manually? </li>
  </ul>


  \todo Statistics
  <ul>
   <li> Similar to ClauseSets/Statistics.mac we should have
   Hypergraphs/Statistics.mac. </li>
   <li> Simplest:
   \verbatim
statistics_hg(G) := statistics_fcs(G)$
   \endverbatim
   </li>
  </ul>


  \todo Special properties of hypergraphs
  <ul>
   <li> Perhaps we have a module "Properties.mac", where basic properties
   of hypergraphs are tested. </li>
   <li> Whether a general hypergraph has repeated hyperedges. </li>
   <li> Whether a hypergraph has subsumed hyperedges. </li>
   <li> Was a hypergraph is "downward hereditary" or "upward hereditary". </li>
   <li> What about all forms of stability under set-operations? Likely these
   special hypergraphs should go to ComputerAlgebra/Sets. </li>
  </ul>


  \todo Intersecting hypergraphs
  <ul>
   <li> See "Allowed parameter values" in
   Satisfiability/Lisp/ConflictCombinatorics/plans/HittingClauseSets.hpp
   for a motivation to study intersecting hypergraphs (no empty hyperedges,
   and every two hyperedges intersect). </li>
  </ul>


  \todo Helly property
  <ul>
   <li> In [Chepoi, Creignou, Hermann, Salzer, The {H}elly property and
   satisfiability of {B}oolean formulas defined on set families, European
   Journal of Combinatorics, 2010, 31(2):502-516] the Helly property of
   hypergraphs (every intersecting sub-hypergraph is a star) is applied to
   signed CNF (the vertices are the possible values, the hyperedges the
   allowed positive signs). </li>
   <li> They also discuss two algorithms for deciding the Helly property in
   polynomial time. </li>
   <li> We should implement these algorithms. (And, of course, also the other
   algorithms from the paper.) </li>
  </ul>


  \todo Hypergraph automorphisms
  <ul>
   <li> See "Automorphisms of Ramsey hypergraphs" in
   ComputerAlgebra/RamseyTheory/Lisp/Ramsey/plans/general.hpp for first
   applications (and first thoughts). </li>
  </ul>
  
  
  \todo Representing edge and vertex labellings
  <ul>
   <li> One needs to be able to represent an edge, or vertex labelling of a 
   given hypergraph in the maxima system. </li>
   <li> See also "Representing edge and vertex labellings" in 
   ComputerAlgebra/Graphs/Lisp/plans/general.hpp . </li>
  </ul>


  \todo Palindromic versions
  <ul>
   <li> We have palindromise_vdw_ohg as well as schurtriples_pd_ohg and
   wschurtriples_pd_ohg. </li>
   <li> Perhaps a more general framework is possible? </li>
  </ul>

*/

