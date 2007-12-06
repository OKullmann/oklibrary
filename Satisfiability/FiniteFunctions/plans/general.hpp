// Oliver Kullmann, 6.12.2007 (Swansea)
/* Copyright 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Transitional/Satisfiability/FiniteFunctions/plans/general.hpp
  \brief Super-module for finite functions (especially boolean functions)

  Considered are functions f: D_1 x ... X D_n -> {0,1}, where D_i
  are finite sets (in the boolean case we have F_i = {0,1}).


  \todo Create milestones.


  \todo Prime implicants and implicates
  <ul>
   <li> What terminology to use for *clause-sets*, where we could have
   CNF's or DNF (thus prime implicates or prime implicants) ? </li>
   <li> We consider the whole range of generalised clause-sets, from boolean
   clause-sets to signed clause-sets. </li>
   <li> Implicates for F are generalised clauses C which follows from F. </li>
   <li> Between generalised clauses we have the partial order "implies",
   which is quickly decidable. </li>
   <li> Prime implicates are implicates which are minimal w.r.t. to the
   partial order of implication amongst the type of generalised clauses
   considered. </li>
   <li> The algorithms which works always is to create all resolvents and
   keep the minimal ones (the "resolution algorithm").
    <ol>
     <li> See ProofSystems/Resolution/plans/ResolutionClosure.hpp. </li>
     <li> So in this module we only connect the resolution algorithm
     (which likely shouldn't care about "DNF" or "CNF") to the
     concept of finite functions. </li>
    </ol>
   </li>
   <li> The other main alternative is to start with a CNF or DNF (generalised)
   given, and then compute the minimal satisfying partial assignments (using
   the given literal type), which yields the prime implicates resp.
   the prime implicants (call it the "dualisation algorithm").
    <ol>
     <li> See AllSolutions/plans/MinimalAssignments.hpp. </li>
     <li> So again, likely the general algorithm is not implemented in
     this module. </li>
    </ol>
   </li>
   <li> A special situation is given when we start with the full table,
   that is, with all full implicates and implicants given.
    <ol>
     <li> Likely this module is the right place for this specialisation. </li>
     <li> For the boolean case the algorithm derived fromt the general
     resolution algorithm together with some obvious simplications regarding
     subsumption-elimination is called the "Quine-McClusky algorithm". </li>
     <li> Actually, the algorithm only needs either all full implicates
     (for the prime implicates) or all full implicants (for the prime
     implicants), and both together are only used to make the algorithm
     polynomial time (since then, e.g. in the boolean case, the input
     is of size 2^n, while a trivial essential uppper bound is 3^n). </li>
     <li> Is there a way of using both the satisfying and the falsifying
     total assignments? Likely the way to do is to combine the resolution
     algorithm with the dualisation algorithm. </li>
    </ol>
   </li>
  </ul>


  \todo Minimisation


  \todo BDDs
  <ul>
   <li> In general, the decision diagrams are no longer "binare".
   Perhaps it's called "FDD" ("finite decision diagram") ? </li>
  </ul>

*/

/*!
  \namespace OKlib::Satisfiability::FiniteFunctions
  \brief Algorithms and representations for finite (especially boolean) functions

  Alias "FiF".
*/

namespace OKlib {
  namespace Satisfiability {
    namespace FiniteFunctions {
    }
    namespace FiF = FiniteFunctions;
  }
}

