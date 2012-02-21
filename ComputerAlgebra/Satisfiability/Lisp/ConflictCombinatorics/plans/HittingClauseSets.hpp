// Oliver Kullmann, 23.2.2008 (Swansea)
/* Copyright 2008, 2009, 2012 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/ConflictCombinatorics/plans/HittingClauseSets.hpp
  \brief Plans related to hitting clause-sets


  \todo Organisation
  <ul>
   <li> Create a new module Satisfiability/Lisp/HittingClauseSets. </li>
   <li> Move all about hitting cls and ABDs there. </li>
   <li> And move the uhit_def catalogue there.
    <ol>
     <li> See "Checking the uhit_def catalogue" in
     ComputerAlgebra/Satisfiability/Lisp/MinimalUnsatisfiability/Basics.mac
     </li>
    </ol>
   </li>
   <li> And then split it into several sub-modules.
    <ol>
     <li> "SplittingTrees.mac" </li>
     <li> "RepresentingClauseSets.mac" </li>
     <li> "Generators.mac" </li>
     <li> "TwoSubsumption.mac" </li>
     <li> "AssociativeBlockDesigns.mac" </li>
     <li> "UhitCatalogue.mac" </li>
     <li> "Search.mac" (e.g., searching for special hitting cls via SAT) </li>
    </ol>
   </li>
  </ul>


  \todo Connections
  <ul>
   <li> See
   Experimentation/Investigations/UHittingCatalogue/plans/general.hpp
   for investigations. </li>
  </ul>


  \todo Allowed parameter values
  <ul>
   <li> A fundamental question about boolean hitting clause-sets is to
   determine the possible parameter-values. </li>
   <li> Of course, unsatisfiable hitting clause-sets are of high interest,
   but likely the questions then become much more complicated; see for example
   todo "Non-singular unsatisfiable hitting clause-sets of given deficiency"
   below. </li>
   <li> Possible parameters are:
    <ol>
     <li> n, c, l </li>
     <li> the lists of variable-degrees, literal-degrees, clause-ranks </li>
     <li> minimal/maximal values for these degrees and ranks </li>
     <li> the conflict-matrix. </li>
    </ol>
   </li>
   <li> The corresponding questions for intersecting hypergraphs should be
   simpler, and they are also basic here, since the variable-hypergraphs of
   hitting clause-sets are intersecting.
    <ol>
     <li> A basic example is that for a 2-uniform hitting clause-set F we have
     n(F) <= 1/2 + (1/4 + 2 * c(F))^(1/2), since their variable-hypergraphs
     are complete graphs. </li>
     <li> For c(F) = 3 we obtain n(F) <= 3 (compared to n(F) <= 6 if F would
     be arbitrary). </li>
     <li> This needs to be generalised to arbitrary ranks. </li>
    </ol>
   </li>
  </ul>


  \todo Constructions
  <ul>
   <li> The set of hitting clause-sets is a sub-monoid (with zero) of
   CLS with "or_cs" (see
   ComputerAlgebra/Satisfiability/Lisp/ClauseSets/plans/Constructions.hpp).
   </li>
  </ul>


  \todo max_var_hitting_def
  <ul>
   <li> Perhaps this function should be an array-function. </li>
   <li> It would also be nice, if additionally we could give a direct
   construction. </li>
  </ul>


  \todo Representations
  <ul>
   <li> hitting_cls_rep_st(T) represents the boolean function F underlying T
   in the following ways in case T involves reduction r:
    <ol>
     <li> In any case both representations (CNF and DNF) are satisfiability-
     equivalent. </li>
     <li> If r only used forced assignments, then after amendment
     the DNF-representation yields an equivalent clause-set. </li>
    </ol>
   </li>
   <li> This should be documented, and perhaps r-splitting trees using only
   forced assignments get a proper name. </li>
  </ul>


  \todo Decomposing clause-sets into hitting clause-sets
  <ul>
   <li> DONE
   Write a function which actually computes the conflict-partition-number
   of a clause-set (the smallest number of hitting sub-clause-sets which
   partition the clause-set). </li>
  </ul>


  \todo %Generators for unsatisfiable hitting clause-sets
  <ul>
   <li> Topic "Generators" in
   Satisfiability/Lisp/ConflictCombinatorics/plans/AssociativeBlockDesigns.hpp
   is concerned with unsatisfiable hitting clause-sets which
   are uniform and variable-regular (and thus literal-regular). </li>
   <li> If on the other hand we drop (variable-)regularity (so we are only
   considering unsatisfiable uniform hitting clause-sets), what constructions
   are then available?
    <ol>
     <li> We can inspect the ABD-constructions, and see to what degree they are
     generic, i.e., for assumptions are actually needed and which are
     transferred to the result. </li>
    </ol>
   </li>
   <li> And what about general unsatisfiable hitting clause-sets?
    <ol>
     <li> One could also consider regularity (variable- or literal-regularity)
     with uniformity. </li>
     <li> In any case, the ABD-constructions need to be inspected. </li>
     <li> We have three approaches, based on applying partial assignments,
     applying DP-reductions, and applying 2-subsumption resolution. </li>
    </ol>
   </li>
   <li> Questions regarding the minimal possible variable degree of uniform
   unsatisfiable clause-sets in general are handled in
   Satisfiability/Lisp/MinimalUnsatisfiability/plans/SmallVariableDegrees.hpp
   </li>
  </ul>


  \todo derived_hitting_cs_pred_isoelim
  <ul>
   <li> For monitoring we should make it possible to output the information
   in monitor_check_dhcpfi_entry only up to a given depth in the search
   tree. </li>
   <li> How to enable continuation after stopping the computation?
    <ol>
     <li> The current path needs to be available, so that one can continue
     with it. </li>
     <li> And some given global variable mirrors always its current value.
     </li>
     <li> Setting this value appropriately should also make it possible to
     jump around in the search tree, and visit "later" parts. </li>
    </ol>
   </li>
   <li> Are there heuristics for choosing the 2-subsumption step and the
   first branch, in order to find instances earlier?
    <ol>
     <li> One can maximise the min-var-degree, especially if one is searching
     for such examples (see "Maximal min-var-degrees" in
     Satisfiability/Lisp/MinimalUnsatisfiability/plans/general.hpp.
     </li>
     <li> This seems reasonably, and should also give a better chance of
     reaching a low deficiency. </li>
     <li> So we should use a heuristics h(F,FP,forb_pairs), which returns
     [R,G,new_forb_pairs] respectively [new_forb_pairs]. </li>
     <li> Yet, as a prototype, this is hardcoded into
     "all_derived_hitting_cs_pred_isoelim_mvd". </li>
     <li> Another heuristics would be to maximise the min-lit-degree. </li>
    </ol>
   </li>
   <li> We should clean-up handling of V:
    <ol>
     <li> Do we assume that no variable gets eliminated, or not? </li>
     <li> See all_derived_hitting_cs_isoelim. </li>
    </ol>
   </li>
   <li> We should clean-up all these different versions:
    <ol>
     <li> Perhaps we should have a dedicated sub-module. </li>
    </ol>
   </li>
   <li> Generalise nearly_full_hitting_fcs:
    <ol>
     <li> For n and 0 <= i <= n we can find F with deficiency
     2^n - n - i which attain the upper bound on the min-var-degree. </li>
     <li> i=0 is full_fcs(n), i=1 is nearly_full_hitting_fcs(n). </li>
     <li> For i >= 2 we don't have uniqueness. </li>
     <li> Finding all cases can be done by derived_hitting_cs_pred_isoelim.
      <ul>
       <li> But for getting only the sharp cases it seems necessary not
       to use the same variable twice. </li>
       <li> This should also be sufficient. </li>
       <li> So we can speed up the computation. </li>
      </ul>
     </li>
     <li> And then we should have a simple procedure which can find
     one clause-set for each i. </li>
    </ol>
   </li>
  </ul>


  \todo Non-singular unsatisfiable hitting clause-sets of given deficiency
  <ul>
   <li> Conjecture of XSZ+OK: For fixed deficiency k, there are up to
   isomorphism only finitely many unsatisfiable hitting clause-sets which
   are reduced w.r.t. singular DP-reduction. </li>
   <li> In other words, there is a function maxN(k): N -> N such that
        check_hitting_nsing_def(F) = [k] => nvar_cs(F) <= maxN(k). </li>
   <li> More precisely, OK+XSZ conjecture the following:
    <ol>
     <li> Let N(k,n) be the number of isomorphism types of "claw-free"
     unsatisfiable hitting clause-sets with deficiency k and with n
     variables. </li>
     <li> Obviously for fixed k there is a minimal n s.t. N(k,n) > 0;
     call this minN(k). </li>
     <li> Let a(n) be the smallest n such that 2^n - n >= k.
     We have minN(k) >= a(n) (due to the completeness of
     2-subsumption resolution; see below). </li>
     <li> We conjecture that for all k we have equality here. </li>
     <li> Let maxN(k) be the supremum of n s.t. N(k,n) > 0.
     So the conjecture states that maxN(k) < inf for all k. </li>
     <li> Now we conjecture that maxN(k) = 3 + (k-2)*4; see
     max_var_hitting_def in
     Satisfiability/Lisp/ConflictCombinatorics/HittingClauseSets.mac
     for the (simple) realising construction. </li>
     <li> Furthermore we believe that
      <ul>
       <li> The set of n with N(k,n) is exactly the interval
       {minN(k), ..., maxN(k)}. </li>
       <li> For k >= 2 N(k,-) is first strictly increasing, and
       then strictly decreasing on this interval. </li>
       <li> And N(k,maxN(k)) = 1. </li>
      </ul>
     </li>
    </ol>
   </li>
   <li> In
   Satisfiability/Lisp/MinimalUnsatisfiability/data/uhit_def.mac
   one finds a catalogue of claw-free unsatisfiable hitting clause-sets for
   given deficiency.
    <ol>
     <li> A main tool are the functions all_unsinghitting_def(k,n) and
     all_unsinghitting(n), which for given k,n resp. given n determine
     all isomorphism types of claw-free unsatisfiable hitting clause-sets.
     </li>
     <li> The basis idea is that from full_fcs(n) by 2-subsumption resolution
     we can create all hitting clause-sets using at most n variables. </li>
     <li> Once we have test cases (if the above functions take too long,
     one can abort the computation and use what has been found until now),
     then they can be checked against the catalogue via
     classify_candidates_uhit_def. </li>
     <li> Another source is the application of partial assignments to an
     "interesting" hitting clause-set.
      <ul>
       <li> Experiment:
       \verbatim
R3 : representatives_cs(map(sdp_reduction_cs,all_hittinginstances_def(brouwer1999[2],3)))$
map(nvar_cs,R3);
  {3, 4, 5}
length(R3);
  12
classify_candidates_uhit_def(listify(R3));
       \endverbatim (see
       (see ComputerAlgebra/Satisfiability/Lisp/Symmetries/Symmetries.mac
       for tools for isomorphism testing).
       It seems that brouwer1999 is rather rich w.r.t. creating various
       hitting clause-sets by applying partial assignments.
       </li>
       <li> Possible this "richness" makes brouwer1999 so special (and
       hard to find)? </li>
      </ul>
     </li>
     <li> Another source is the application of DP-reductions:
      <ul>
       <li> Experiment:
       \verbatim
dp_inst_brr : all_hitting_DP_reductions_def(brouwer1999[2],3,'dp_inst_br)$
       \endverbatim
       This appears to take a week, and possibly no instance is found,
       so this appears not to be suitable here. </li>
      </ul>
     </li>
    </ol>
   </li>
   <li> See Experimentation/Investigations/plans/MaximiseMinVarDegrees.hpp
   for related investigations into maximising the min-var-degree. </li>
  </ul>


  \todo all_cld_uhit_minvd
  <ul>
   <li> We should try to strengthen the filtering.
    <ul>
     <li> all_cld_uhit_minvd(6,4,9) = {{[2,2],[4,8]}}:
      <ol>
       <li> Here we have c = 6 + 4 = 10 = 9 + 1. </li>
       <li> The case is impossible since due to the hitting condition
       the two binary clauses must overlap, but then some variables
       occurs only 10 - 2 = 8 times. </li>
      </ol>
     </li>
     <li> all_cld_uhit_minvd(6,5,9) = {
      {[1,1],[3,1],[4,3],[5,6]},
      {[1,1],[3,2],[5,8]},
      {[1,1],[4,6],[5,4]},
      {[2,3],[5,8]} }
      <ol>
       <li> We have here c = 6 + 5 = 11 = 9 + 2. </li>
       <li> In the first three cases we can apply singular DP-reduction
       (aka unit-clause elimination), reducing the case from (6,5,9) to
       (6,4,9), which we already know is impossible. </li>
       <li> The remaining case {[2,3],[5,8]} is impossible:
        <ul>
         <li> The 2-clauses must pairwise overlap, and thus there
         exists a variable not in any of them, which then exists 11 - 3
         times; contradiction. </li>
        </ul>
       </li>
      </ol>
     </li>
     <li> See Experimentation/Investigations/plans/MaximiseMinVarDegrees.hpp
     for similar examples. </li>
    </ul>
   </li>
   <li> Likely this function should become an array-function, since
   due to unit-clauses we have a recursion step here.
    <ol>
     <li> Perhaps first we only consider the case of filtering out
     impossible cases for all_cld_uhit_minvd. </li>
     <li> If we are only interested in non-singular cases, then we
     can drop the cases with unit-clauses at all. </li>
    </ol>
   </li>
   <li> Likely we do not obtain a complete (efficient) rule-system
   here, but we can have a collection of rules for filtering out
   cases. </li>
   <li> Of course, we also obtain (general) satisfiability problems,
   for each clause-length-distribution one problem.
    <ol>
     <li> These problems are easier since we only need to handle
     the hitting condition (not the unsatisfiability condition). </li>
     <li> Compare with "Searching for ABD(n,k) (via SAT)" in
     Lisp/ConflictCombinatorics/plans/AssociativeBlockDesigns.hpp
     </li>
     <li> First we create propagators; this is straight-forward, see
     "Hitting clause-sets" in
     Satisfiability/Lisp/ConstraintProblems/plans/Generators.hpp
     </li>
    </ol>
   </li>
  </ul>
*/

