// Oliver Kullmann, 6.12.2007 (Swansea)
/* Copyright 2007, 2008, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file OKlib/Satisfiability/FiniteFunctions/plans/general.hpp
  \brief Super-module for finite functions (especially boolean functions)

  Considered are functions f: D_1 x ... X D_n -> {0,1}, where D_i
  are finite sets (in the boolean case we have F_i = {0,1}).


  \todo Links and plans
  <ul>
   <li> Establish links to other modules (especially computer algebra).
    <ol>
     <li> See
     ComputerAlgebra/Satisfiability/Lisp/FiniteFunctions/plans/general.hpp.
     </li>
     <li> See
     ComputerAlgebra/Satisfiability/Lisp/PropositionalLogic/plans/general.hpp.
     </li>
    </ol>
   </li>
   <li> See
   Buildsystem/ExternalSources/SpecialBuilds/plans/BooleanFunctions.hpp. </li>
   <li> Create a development plan and further milestones. </li>
  </ul>


  \todo Prime implicants and implicates
  <ul>
   <li> This whole topic seems to deserve its own module (and plans-file).
   </li>
   <li> What terminology to use for *clause-sets*, where we could have
   CNF's or DNF (thus prime implicates or prime implicants) ? </li>
   <li> We consider the whole range of generalised clause-sets, from boolean
   clause-sets to signed clause-sets. </li>
   <li> Implicates for F are generalised clauses which follows from F,
   while implicants are generalised clauses which imply F. </li>
   <li> Between generalised clauses we have the partial order "implies",
   which is quickly decidable. </li>
   <li> Prime implicates are implicates which are minimal %w.r.t. to the
   partial order of implication amongst the type of generalised clauses
   considered. </li>
   <li> The algorithms which works always is to create all resolvents and
   keep the minimal ones (the "resolution algorithm").
    <ol>
     <li> For given F as CNF, this computes the prime implicates/implicants.
     </li>
     <li> See ProofSystems/Resolution/plans/ResolutionClosure.hpp. </li>
     <li> So in this module we only connect the resolution algorithm
     (which likely shouldn't care about "DNF" or "CNF") to the
     concept of finite functions. </li>
    </ol>
   </li>
   <li> The other main alternative is to start with a CNF or DNF (generalised)
   given, and then compute the minimal satisfying partial assignments (using
   the given literal type), which yields the prime implicants resp.
   the prime implicates (call it the "dualisation algorithm").
    <ol>
     <li> See AllSolutions/plans/MinimalAssignments.hpp. </li>
     <li> So again, likely the general algorithm is not implemented in
     this module. </li>
    </ol>
   </li>
   <li> Given clause-sets F, G (as CNF), deciding whether G is the
   set of prime implicates of F:
    <ol>
     <li> G must be subsumption-free. </li>
     <li> For every clause C in F there must be D in G with D <= C. </li>
     <li> Given two resolvable clauses D, D' in G, their resolvent must
     be subsumed by some element from G. </li>
     <li> These conditions are all easily checkable; in general the last
     condition, that all C in G follow from F, however is hard to establish.
     </li>
     <li> If for F and partial assignments phi one can decide satisfiability
     of phi*F in polynomial time, then also this check is easy. </li>
     <li> Examples are given by hitting F, Horn F or F in 2-CNF. </li>
    </ol>
    One should establish a test algorithm for these basic cases, so that
    for example we can verify the results of QuineMcCluskey.cpp.
   </li>
  </ul>


  \todo Minimisation


  \todo BDDs
  <ul>
   <li> In general, the decision diagrams are no longer "binary".
   Perhaps it's called "FDD" ("finite decision diagram") ? </li>
   <li> But at the beginning, especially at the Maxima/Lisp level, we only
   consider the binary case. </li>
  </ul>

*/

/*!
  \namespace OKlib::Satisfiability::FiniteFunctions
  \brief %Algorithms and representations for finite (especially boolean) functions

  Alias "FiF".
*/

namespace OKlib {
  namespace Satisfiability {
    namespace FiniteFunctions {
    }
    namespace FiF = FiniteFunctions;
  }
}

