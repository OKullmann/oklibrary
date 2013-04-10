// Oliver Kullmann, 24.12.2010 (Swansea)
/* Copyright 2010, 2011, 2012, 2013 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/FiniteFunctions/plans/TseitinTranslation.hpp
  \brief Plans regarding the Tseitin translation from DNF to CNF


  \todo Renaming the file to "CanonicalTranslation.mac"
  <ul>
   <li> Then "TseitinTranslation.mac" should be in module PropositionalLogic,
   where currently we have the Tseitin translation in Formulas.mac. See below
   for further plans. </li>
  </ul>


  \todo Add statistics for dualtsplus_fcl


  \todo Translation of boolean circuits into boolean CNF-clause-sets
  <ul>
   <li> Where shall the following go ?
    <ol>
     <li> In PropositionalLogic/Formulas.mac we have already the Tseitin
     translation for formulas. </li>
     <li> Either there, or even a new module "Circuits". </li>
    </ol>
   </li>
   <li> See [Jackson, Sheridan: Clause Form Conversions for Boolean Circuits;
   SAT 2004] for more refined forms than the basic Tseitin-translation. </li>
   <li> See
   Satisfiability/Transformers/Generators/plans/TseitinTransformation.hpp
   for the general considerations. </li>
   <li> See
   Satisfiability/Lisp/PropositionalLogic/plans/TseitinTranslation.hpp
   for the special case of propositional formulas. </li>
   <li> For each gate g in a circuit C, one represents the inputs i
   by variables v_i, the output by variable o, and then represents
   the equivalence o <-> g( ... v_i ...) by a CNF. </li>
   <li> The conjunction of all these CNFs yields the whole CNF, where
   once the values for all inputs are given, all other variables have
   a unique value so that the whole CNF becomes true. </li>
   <li> This also works for non-boolean circuits and various generalised
   forms of non-boolean CNF-clause-sets. </li>
  </ul>


  \todo Understanding dualts_fcl
  <ul>
   <li> The following needs to be updated according to the report of MG+OK
   on boolean functions and their representations. </li>
   <li> Then also all variations need to be implemented. </li>
   <li> And likely the notion of "dualts" is not appropriate: rename it to
   "canonical translation".
    <ol>
     <li> Perhaps "cantrans_fcl2fcl" ? </li>
     <li> The reduced form then "rcantrans_fcl2fcl" ? </li>
     <li> And the extended form (currently "plus-form") "ecantrans_fcl2fcl" ?
     </li>
     <li> The mathematical notation for reduced resp. extended form could use
     "-" resp. "+". </li>
     <li> The extended form can come in three versions: full equivalence, or
     only one of the two implication-directions. </li>
     <li> And these three extended-versions can also be applied to the reduced
     form. </li>
    </ol>
   </li>
   <li> Consider a DNF F. The task is to understand the CNF G := dualts_cl(F).
   </li>
   <li> Let n be the number of variables of F, and let c be the number of
   clauses of F. </li>
   <li> Then the variables of G are the n variables of F plus the variables
   dts(1), ..., dts(c). </li>
   <li> The boolean function of G is the conjunction of the equivalences
   (dts(i) <-> C_i) for F = (C_1 or ... or C_c) and the clause
   D := (dts(1) or ... or dts(c)). </li>
   <li> For simplicity we assume that none of the clauses C_i is empty. </li>
   <li> Now the natural CNF for an equivalence (a <-> (b_1 and ... and b_k)),
   given by the clauses {-a,b_1},...,{-a,b_k},{a,-b_1,...,-b_k} is exactly
   the set of all prime-CNF-clauses for this boolean function. </li>
   <li> These clauses are also prime-CNF-clauses of G. </li>
   <li> That the clause D is a prime-clause of G is equivalent to F being
   irredundant. </li>
   <li> For the applications we have in mind we can assume that F is
   irredundant (in all our cases it is actually a hitting clause-set (i.e.,
   a disjoint DNF)). </li>
   <li> So G is prime (consists only of prime-clauses). </li>
   <li> G is also irredundant. </li>
   <li> The other prime-CNF-clauses of G are exactly the prime-CNF-clauses
   of F. </li>
   <li> It is G thus (totally) properly r_1-based (see "r-based clause-sets" in
   Investigations/BooleanFunctions/plans/ReductionBasedRepresentations.hpp).
   </li>
  </ul>


  \todo Strengthening dualts_fcl by adding some redundant clauses
  <ul>
   <li> The following needs update: it seems to be implementent by
   dualtsplus_fcl; see also the todos there. </li>
   <li> Given a formal clause-list F as DNF, and a partial assignment
   phi, there are (of course) forced assignments on phi * dualts(F) which 
   are not derived by unit propagation (while if phi * dualts(F) is 
   unsatisfiable, then {} is in r_1(phi * dualts(F)) --- so it needs r_2 to
   compute all forced assignments). </li>
   <li> For example the formal DNF [[1,2,3],[{1,2},{1,3}]] has {1} as a forced
   assignment, however
   \verbatim
F : [[1,2,3],[{1,2},{1,3}]]$
G : fcs2cs(fcl2fcs(dualts_fcl(F)));
  {{-3,-1,dts(2)},{-2,-1,dts(1)},{1,-dts(1)},{1,-dts(2)},{2,-dts(1)},
   {3,-dts(2)},{dts(1),dts(2)}}
ucp_lpa_0_cs(G)[2];
   \endverbatim 
   yields [], not [{1}]. </li>
   <li> Adding for every literal x the clause (C -> x) to dualts(F), where C 
   is the conjunction of -dts(i) for every i where x does not occur in the 
   i-th clause in F, gives a representation which has that under any partial 
   assignment phi using only variables from F, all forced assignments on 
   phi * F follow by unit-clause propagation on dualts_fcl(F). </li>
   <li> More generally, adding for some (CNF-)clause D the clause (C -> D), 
   where C is the conjunction C of all negated indicator variables for clauses
   which have empty intersection with D, then for partial assignments phi
   such that D follows from phi * F, D is subsumed by one of the clauses
   of r_1(phi * F). Moreover, the added clause follows from dualts(F). </li>
   <li> Continuing the above example:
   \verbatim
G2 : union(G,
 {{dts(1),dts(2),-1},{1},{dts(1),dts(2),-2},{dts(2),2},{dts(1),dts(2),-3},{dts(1),3}})$
ucp_lpa_0_cs(G2)[2];
   \endverbatim
   correctly yields [{1}]. One sees here that only additional clauses are
   interesting where the literal x (as above) occurs in some clause; there are
   also other strong redundancies. </li>
   </li>
  </ul>

*/
