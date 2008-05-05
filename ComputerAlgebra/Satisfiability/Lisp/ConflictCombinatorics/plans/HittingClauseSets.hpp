// Oliver Kullmann, 23.2.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/ConflictCombinatorics/plans/HittingClauseSets.hpp
  \brief Plans related to hitting clause-sets


  \todo Constructions
  <ul>
   <li> The set of hitting clause-sets is a sub-monoid (with zero) of
   CLS with "or_cs" (see
   ComputerAlgebra/Satisfiability/Lisp/ClauseSets/plans/Constructions.hpp).
   </li>
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


  \todo %Generators for unsatisfiable uniform and variable-regular hitting
  clause-sets
  <ul>
   <li> There is the important question for what k,r,n we have
   F with uuhittingcsp(F) which are k-uniform and r-variable-regular.
    <ol>
     <li> sasg2000 is an example for k=3,r=6,n=4. </li>
     <li> Ignoring r (i.e., admitting an arbitrary r) we arrive exactly at
     the notion of an "associative block design" (ABD). </li>
     <li> We are allowed to do so since r = 2^k * k / n. </li>
     <li> Apparently ABD's are denoted as "ABD(n,k)"). </li>
     <li> The problem in [Savicky, Sgall, 2000, DNF tautologies with a limited
     number of occurrences of every variable] is, as stated about arbitrary
     uniform unsatisfiable hitting clause-sets and how low a maximal
     variable-degree can be achieved. However their examples achieve actually
     variable-regularity (and also literal-regularity); so we can pose a
     hierarchy of questions for bounds on the maximal variable- and
     literal-degree (how low is possible):
      <ol>
       <li> For arbitrary k-uniform clause-sets: This should be the
       Tovey-problem studied in [Hoory, Szeider, 2006], and apparently
       these considerations belong to another module (where those
       constructions should be implemented). </li>
       <li> Considering only hitting clause-sets; this is the subject of this
       module. </li>
       <li> And then we have the requirements of variable-regularity (ABD's),
       and literal-regularity. </li>
       <li> By Theorem 9.7 in [Lint, Wilson, a Course in %Combinatorics, 2nd ed.]
       actually variable-regularity here already implies literal-regularity.
       </li>
      </ol>
     </li>
     <li> In order to improve the bound from [Savicky, Sgall] we do not
     require variable-regularity, but if we do so, then we are asking for
     k, n with log_k(n) > log_3(4). </li>
     <li> An ABD(8,5) exists which improves the bound. </li>
     <li> To establish s_k = Theta(2^k / k) for the function s_k introduced
     by [Kratochvil, Savicky, Tuza, 1993] for the Tovey-problem, which is best
     possible, we needed to find an example with n = k^2. </li>
     <li> This seems very demanding --- perhaps it is essential to give
     up variable-regularity and hittingness for s_k. But we need to search the
     literature! </li>
    </ol>
   </li>
   <li> Constructions:
    <ol>
     <li> The construction from Lemma 3 in [Savicky, Sgall, 2000], which from
     uniform unsatisfiable hitting clause-sets F,F' of rank k,k' construct a
     uniform unsatisfiable hitting clause-set of rank k * k'. </li>
     <li> This construction should be the same as in [Lint, Wilson], Lemma 9.9,
     and thus should transfer variable-uniformity. </li>
     <li> There are two other constructions in [Lint, Wilson], Theorem 9.10, and
     Theorem 9.11, which need to be implemented. </li>
    </ol>
   </li>
   <li> Sporadic cases:
    <ol>
     <li> We have already sasg2000, an ABD(4,3). </li>
     <li> Perhaps we can determine all ABD(4,3)'s. </li>
     <li> Another sporadic case is ABD(8,5). </li>
     <li> Again, the question to get "all". </li>
     <li> Are there more sporadic examples? </li>
    </ol>
   </li>
   <li> Sequences
    <ol>
     <li> The general constructions, applied to the sporadic examples, yield
     sequences, for which at least for the "prominent cases" we should provide
     convenience functions. </li>
     <li> So Theorem 4 in [Savicky, Sgall] applies the product-construction to
     sasg2000. </li>
    </ol>
   </li>
  </ul>


  \todo Searching for ABD(n,k) (via SAT)
  <ul>
   <li> Most natural seems the clause-based approach:
    <ol>
     <li> For each of the 2^k clauses we have a variable, with domain
     all binom(n,k) * 2^k clauses. </li>
     <li> We have one injectivity condition over all variables (an active
     clause). </li>
     <li> We have one condition stating that all assigned clauses clash
     pairwise.
      <ul>
       <li> Unclear whether this yields an active clause. </li>
       <li> The underlying general problem is for a hitting clause-set to find
       a satisfying partial assignment of minimal size --- unclear whether this
       can be done in polynomial time. </li>
      </ul>
     </li>
     <li> And for each variable we have the condition that it occurs in
     exactly 2^k * k / n clauses; this should yield (n) active clauses. </li>
     <li> Stronger, instead just requiring variable-regularity, we should
     better require literal-regularity. And there are other conditions as well.
     </li>
     <li> Finally, symmetry-breaking can be established by choosing a linear
     order on all clauses (for example lexicographical) and also order the
     variables, and by demanding that the first variable gets the smallest
     clause and that the order is variable-values is strictly ascending.
     Are there suitable orderings, so that this becomes an active clause??
     Perhaps we shouldn't demand strict ascending, since this is already
     covered by the injectivity constraint. </li>
    </ol>
   </li>
   <li> Apparently the smallest open case is to find an ABD(12,6) (or to
   determine that it doesn't exist). An easier test case should be ABD(8,5)
   (here there are solutions; perhaps we can determine them all?). </li>
   <li> Regarding the quality-measure mu(n,k) = log_k(n) (the more the better)
   we have mu(4,3) < mu(8,5) < mu(12,6). </li>
  </ul>


  \todo Rules for ABD(n,k)
  <ul>
   <li> Implement <code>abd_exists(n,k)</code> which returns "true", "false" or
   "unknown". </li>
   <li> For that we should use Maxima's ability to use rules. </li>
  </ul>


  \todo %Generators for unsatisfiable hitting clause-sets
  <ul>
   <li> The above is concerned with unsatisfiable hitting clause-sets which
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
   ComputerAlgebra/Satisfiability/Lisp/MinimalUnsatisfiability/plans/general.hpp
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
     <li> And an given global variable mirrors always its current value. </li>
     <li> Setting this value appropriately should also make it possible to
     jump around in the search tree, and visit "later" parts. </li>
    </ol>
   </li>
   <li> Are there heuristics for choosing the 2-subsumption step and the
   first branch, in order to find instances earlier? </li>
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
     ComputerAlgebra/Satisfiability/Lisp/ConflictCombinatorics/HittingClauseSets.mac
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
   <li> In ComputerAlgebra/Satisfiability/Lisp/MinimalUnsatisfiability/data/uhit_def.mac
   one finds a catalogue of claw-free unsatisfiable hitting clause-sets for
   given deficiency.
    <ol>
     <li> A main tool are the functions all_unsinghitting_def(k,n) and
     all_unsinghitting(n), which for given k,n resp. given n determine
     all isomorphism types of claw-free unsatisfiable hitting clause-sets. </li>
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
R3 : representatives_cs(map(singular_dp_reduction,all_hittinginstances_def(brouwer1999[2],3)))$
map(nvar_cs,R3);
  {3, 4, 5}
length(R3);
  12
classify_candidates_uhit_def(listify(R3));
       \endverbatim (see 
       (see ComputerAlgebra/Satisfiability/Lisp/ClauseSets/Symmetries.mac
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
  </ul>

*/

