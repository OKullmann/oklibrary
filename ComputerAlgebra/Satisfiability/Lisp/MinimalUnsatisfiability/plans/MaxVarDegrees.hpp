// Oliver Kullmann, 24.3.2008 (Swansea)
/* Copyright 2008, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/MinimalUnsatisfiability/plans/MaxVarDegrees.hpp
  \brief Plans related to the determination of the maximal variable degrees of (typically uniform) minimally unsatisfiable clause-sets


  \todo Minimal variable-degree of uniform minimally unsatisfiable clause-sets
  <ul>
   <li> The basic notions:
    <ol>
     <li> We have max_variable_degree_cs. </li>
     <li> This is to be minimised for given k over all uniform minimally
     unsatisfiable clause-sets which are k-uniform. </li>
     <li> The function yielding precise values and bounds shall be called
     "maxvardegree_umu" ("umu" for "uniform minimally unsatisfiable"). </li>
     <li> That is, uniformcsp(F) and min_unsat_bydef(F) and max_rank_cs(F)=k
     => max_variable_degree_cs(F) >= maxvardegree_umu(k). </li>
     <li> Better, maxvardegree_umu(k) should be a number, iff the precise
     value is known, and otherwise a pair [lower_bound, upper_bound]. </li>
     <li> DONE (we speak of "maxvardegree" now)
     Let minvardeg_umu(k) for k >= 0 be the minimal possible (maximal)
     variable-degree of (minimally) unsatisfiable k-uniform boolean
     clause-sets F. </li>
    </ol>
   </li>
   <li> We need to implement maxvardegree_umu(k) so that it yields the
   currently known lower and upper bounds.
    <ol>
     <li> We need a table with the best known values. </li>
     <li> Then there are several functions for computing upper bounds, which
     can be invoked for unknown k. </li>
     <li> Once we computed such an upper bound, then we might wish to store it
     (especially if the computation was rather expensive); how to do this?
     </li>
    </ol>
   </li>
   <li> We also need witnesses for the upper and lower bounds:
    <ol>
     <li> Upper bounds are realised by single examples. </li>
     <li> See topic "Pumping up binary clauses" for a method of constructing
     clause-sets, which is directly connected to the NP-completeness proof.
     </li>
     <li> In [Hoory, Szeider, arXiv, 2004], identical with [Hoory, Szeider,
     SIAM Discrete Mathematics, 2006], a general construction is discussed;
     see todo below. </li>
     <li> See "Small variable-degrees in MUSAT(1)" in
     Satisfiability/Lisp/MinimalUnsatisfiability/plans/DeficiencyOne.hpp
     for methods, which apparently yield the strongest examples currently.
     </li>
     <li> Apparently the examples are rather large, so that also more
     compressed representations (term-based, or merely stating main
     parameters), are needed. </li>
     <li> For lower bounds one needs algorithms which find quickly a
     satisfying assignments. </li>
     <li> The lower bound k+1 (k >= 1) is realised by matching-satisfiability.
     </li>
    </ol>
   </li>
   <li> The restrictions to hitting clause-sets F are considered in
   Satisfiability/Lisp/ConflictCombinatorics/plans/HittingClauseSets.hpp.
   </li>
  </ul>


  \todo Pumping up binary clauses
  <ul>
   <li> See "Translations reducing the number of variable-occurrences" in
   ComputerAlgebra/Satisfiability/Lisp/ClauseSets/plans/Translations.hpp
   for the background of the general translations. </li>
   <li> Let m(k) be the minimal variable-degree s for a k-uniform clause-set
   F_v, which forces v to be false and where v has variable-degree at most s-1.
   Then we have maxvardegree_umu(k) <= m(k), as follows by the satisfiability-
   equivalence of the general transformation (pumping up binary clauses by
   such a helper clause-set). </li>
   <li> Perhaps m(k) is easier to determine than maxvardegree_umu(k). Or do we
   have maxvardegree_umu(k) = m(k) ??? </li>
   <li> In [Tovey, 1984, DAM] such an F_v for k=3, s=4 is given, which is
   implemented as "tovey84" in
   Satisfiability/Lisp/MinimalUnsatisfiability/MinVariableDegrees.mac
   </li>
   <li> Due to m(3) <= 4 we get m(k) <= 2^(k-1) for all k >= 3, simply by
   expanding the above F_v to higher clause-length using inverse 2-subsumption
   resolution (using new variables each time). </li>
  </ul>


  \todo Implementing [Hoory, Szeider, SIAM Discrete Mathematics, 2006]
  <ul>
   <li> In Section 4, Definition 4.1, Lemma 4.2, rules 1,2, and the variation
   on page 527 top, a general (nondeterministic) construction is introduced,
   which we should implement. </li>
   <li> Then (page 527) the two special families from Sections 2, 3 are
   derived; again we should implement this. </li>
   <li> Let maxvardegree_umu_hs(k) be the minimal (maximal) variable-degree
   obtainable by this construction (in the article called "f_2(k) + 1"). </li>
   <li> At the end of Section 4 a simple algorithm is presented for computing
   maxvardegree_umu_hs(k), which we should implement. </li>
   <li> Likely the formulas constructed are minimally unsatisfiable? </li>
   <li> What are their parameters (deficiency, number of variables)? </li>
   <li> How regular are these formulas? </li>
  </ul>

*/

