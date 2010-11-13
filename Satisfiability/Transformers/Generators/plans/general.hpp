// Oliver Kullmann, 20.11.2005 (Swansea)
/* Copyright 2005 - 2007, 2008, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/Transformers/Generators/plans/general.hpp
  \brief Plans for the module for creating SAT problems from parameters


  \todo Update
  <ul>
   <li>
    <ol> 
     <li> Filter.cpp: what is the purpose of this?? Likely to be removed.
     </li>
     <li> Ramsey.cpp
      <ul>
       <li> Has now a public link (needs to be updated when moved). </li>
      </ul>
     </li>
     <li> Ramsey.hpp </li>
     <li> Generators/FaronsPuzzle.cpp: </li>
     <li> Tests_Ramsey: </li>
     <li> Test_Transformations.cpp: </li>
     <li> VanderWaerden.cpp: DONE (basic updates) </li>
     <li> VanderWaerden.hpp: DONE (basic updates) </li>
     <li> OKsolver/Experimental/AllgKlassen200203/TransformationsBiclique.hpp:
     </li>
     <li> makefile: This old makefile should be replaced by the standard
     system : DONE </li>
     <li> LinInequal.hpp: DONE </li>
     <li> FaronsPuzzle.hpp: DONE </li>
     <li> Test.cpp: DONE </li>
    </ol>
   </li>
   <li> Transfer
   OKsolver/Experimental/AllgKlassen200203/TransformationsBiclique to here.
   </li>
   <li> The main generators should produce generalised clause-sets.
   If output and/or boolean formulas is needed, then further
   transformers are to be used. The variables are tuples of values.
   </li>
   <li> Also in Applications/RamseyTheory/ (see
   Applications/RamseyTheory/plans/general.hpp) we have components related
   to Ramsey theory:
    <ol>
     <li> The special applications GTTransversals(Inc) and
     VDWTransversals(Inc)(PB) should go there. </li>
    </ol>
   </li>
  </ul>


  \todo Standardising variable names


  \todo Sudoku
  <ul>
   <li> Similar to
   ComputerAlgebra/Satisfiability/Lisp/ClauseSets/Generators.mac,
   write a generator for weak, dual-weak and strong Sudoku problems,
   for the empty initial position. </li>
   <li> Fixing fields then happens by just appending to the generated
   base-files. </li>
   <li> For creating the weak clause-sets, we already avoid creating
   those box-constraints-clauses which are already covered by the row
   and column constraints. </li>
  </ul>


  \todo Biclique-transformations
  <ul>
   <li> We need active clause-sets for biclique-transformations for
   interesting classes of graphs (including complete graphs). </li>
  </ul>


  \todo MUSAT
  <ul>
   <li> MUSAT_{delta=1} </li>
   <li> The HKB-formulas (MUSAT_{delta=2}, where every literal occurs
   at least once) have to be constructed. </li>
   <li> Do we actually need MUSaT(1,2), since we have it already in
   the Maxima/Lisp system (see
   ComputerAlgebra/Satisfiability/Lisp/ClauseSets/Generators.mac) ? </li>
  </ul>


  \todo Matching formulas
  <ul>
   <li> Generator for PHP, and more generally for matching formulas,
   given an arbitrary graph and a set of vertices to be matched. </li>
   <li> Directly, and as active clause-sets. </li>
  </ul>


  \todo %Generators for finding incidence structures and designs
  <ul>
   <li> Naturally modelled by boolean variables I(p, b) ("I" like
   "incident", "p" like "point", "b" like "block"). </li>
   <li> Searching for example for balanced designs. </li>
   <li> Very simple the search for projective planes of order n:
    <ul>
     <li> n^2 + n + 1 p's and b's </li>
     <li> for all p <> p' there is exactly one b with I(p,b) and I(p',b) </li>
     <li> for all b <> b' there is exactly one p with I(p, b) and I(p, b')
     </li>
     <li> for all b there are two p, p' of p1,p2,p3,p4 (fixed variables!)
     with not I(p, b) and not I(p', b). </li>
    </ul>
   </li>
   <li> Modelled directly or via active clause-sets (especially that every
   two points are on one line, and every two lines intersect in one point!).
   </li>
   <li> QUESTION OK: Where does this topic belong to? To module LatinSquares?
   Or perhaps we should have a dedicated module on projective plans etc.?
   Sounds reasonable, but only after we really have something to start.
   </li>
  </ul>


  \todo Extended resolution
  <ul>
   <li> Very interesting are also the Extended Resolution extensions of PHP
   (and potentially other classes). </li>
   <li> Seeing the performance of SAT solvers
   on them would be interesting (or perhaps one can tune the SAT solvers
   to make good use of the extension clauses?!). </li>
  </ul>


  \todo From non-boolean clause-sets
  <ul>
   <li> Transformers from non-boolean clause-sets to boolean
   clause-sets are needed. </li>
   <li> The standard translation, with or without
   the at-most-one-clauses. </li>
   <li> And also the other translations known from
   the literature; see [Prestwich; Local Search on SAT-encoded Colouring
   Problems; SAT 2003] for an overview, and also [Frisch,Peugniez;
   Solving Non-Boolean Satisfiability Problems with Stochastic Local
   Search; 2001]), and [Ansotegui,Manya; Mapping Problems with
   Finite-Domain Variables to Problems with Boolean Variables]. </li>
   <li> And see the new nested translation from [Kullmann 2007;
   CSR-12 2007 Swansea]. </li>
  </ul>


  \todo Pebbling formulas
  <ul>
   <li> Given a dag G, where each node v is labelled with some boolean
   formula F_v, the boolean formula Peb(G) is the conjunction of
   the following formulas:
   <ol>
    <li> For each node v with direct predecessors w_1, ..., w_s
    the formula
      P_v := (F_{w_1} and ... and F_{w_s}) -> F_v.
    </li>
    <li> For each sink v the formula
      not F_v.
    </li>
   </ol>
   </li>
   <li> An interesting feature is that if we can branch on the
   formulas F_v then Peb(G) has a (generalised) input resolution
   refutation, but if the don't have this power then its tree resolution
   hardness is linked to the pebbling complexity of G (explore!). </li>
   <li> Especially if the F_v are disjunctive clauses then the
   P_v can be (easily) directly expressed as clause-sets (via
   distributivity).
    <ol>
     <li> If there is only one target then we get minimally
     unsatisfiable clause-sets (further special properties?). </li>
     <li> Multiple targets could be suitable for testing how could
     is a solver to break a problem into components. </li>
     <li> Regular resolution is able to handle such formulas
     efficient; see [Beame, Kautz, Sabharwal] for algorithmic
     exploitation. </li>
     <li> If the maximal clause-length for the F_v is k then,
     as mentioned above, Res(k) yields short proofs. </li>
     <li> But, as mentioned above, Res(k-1) has tree-resolution
     complexity linked to the pebbling complexity of k (compare
     [Kullmann ECCC 1999]). </li>
    </ol>
   </li>
  </ul>


  \todo Implement the general construction of Soeren Riis (especially
  interesting when we know that the formulas have short refutations).

*/

/*!
  \namespace OKlib::Satisfiability::Transformers::Generators
  \brief Generators for (active) clause-sets (which are not yet in their own module).
*/

namespace OKlib {
  namespace Satisfiability {
    namespace Transformers {
      namespace Generators {
      }
      namespace Gen = Generators;
    }
  }
}

