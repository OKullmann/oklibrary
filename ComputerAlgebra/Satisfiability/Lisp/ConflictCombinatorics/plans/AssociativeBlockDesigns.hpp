// Oliver Kullmann, 24.5.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/ConflictCombinatorics/plans/AssociativeBlockDesigns.hpp
  \brief Plans related to unsatisfiable hitting clause-sets which are uniform and variable-regular


  \todo Organisation
  <ul>
   <li> This topic deserves its own sub-module. </li>
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
       <li> Considering only hitting clause-sets: see
       ComputerAlgebra/Satisfiability/Lisp/ConflictCombinatorics/plans/HittingClauseSets.hpp.
       </li>
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
     <li> We should determine all ABD(4,3)'s. </li>
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
   <li> See also related discussions in
   ComputerAlgebra/Satisfiability/Lisp/ConflictCombinatorics/plans/HittingClauseSets.hpp
   </li>
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


  \todo Properties of ABDs
  <ul>
   <li> sasg2000 allows 2-subsumption resolution, and the apparently
   unique result (up to isomorphism) is the unsatisfiable hitting
   clause-set of deficiency 2 with 3 variables (musatd2(3)). </li>
   <li> brouwer1999 allows 2-subsumption resolution.
    <ol>
     <li> Reducing it w.r.t. 2-subsumption-resolution, but without
     creating singular variables, we obtain quite a few clause-sets
     \verbatim
all_derived_hitting_cs_nsing_isoelim(brouwer1999[2],d_brouwer);
analyse_isorepo_def(d_brouwer);
 [[13,12],[14,72],[15,208],[16,398],[17,566],[18,633],[19,529],[20,333],[21,142],[22,43],[23,7],[24,1]]
     \endverbatim
     however none of them is reduced w.r.t. 2-subsumption resolution. </li>
     <li> We should repeat this experiment with the proper version of
     all_derived_hitting_cs_isoelim (to get all derived clause-sets). </li>
     <li> One more step is possible, creating a singular variable, and
     then one further step yields an instance reduced w.r.t 2-subsumption
     resolution, and also without singular variables, but with 7 variables:
     \verbatim
rbrouwer1999 : redtsr(brouwer1999[2]);
 {{-8,-7,-6,-4,3},{-8,-7,-3},{-8,-7,-2,3,4},{-8,-4,-3,6,7},{-8,-4,-2,3,6},{-8,-3,2,4,7},
 {-8,-2,4,6,7},{-7,-6,-4,2,8},{-7,-6,-2,5,8},{-7,-5,-2,8},{-6,-4,7},{-6,-3,2,4,8},
 {-6,-2,4,7},{-5,-3,2,6,8},{-5,-2,6,7,8},{-4,2,3,6},{-3,5,6,8},{-2,3,5,6,8},{2,3,4}}
redsingdpp(rbrouwer1999);
 true
redtsrp(rbrouwer1999);
 true
     \endverbatim
     </li>
     <li> It seems that rbrouwer1999 is unique up to isomorphism. If this is
     the case then rbrouwer1999 needs to be inspected carefully!
     \verbatim
r_brouwer1999 : rename_fcs(brouwer1999,[8,7,6,5,4,3,2,1]);
rrbrouwer1999 : redtsr(r_brouwer1999[2]);
 {{-7,-5,-1,3,6},{-7,-4,-2,1},{-7,-4,1,2,3},{-7,-3,-2,1,4},{-7,-3,2,5},{-7,-2,-1,5,6},
 {-7,-1,2,3,5},{-7,1,3,4,6},{-6,-5,-1,2,3},{-6,-4,1,3,7},{-6,-3,1,5,7},{-6,-2,-1},
 {-6,-1,2,5,7},{-6,1,3,4},{-5,-3,-2,-1,6},{-5,-3,-2,1,7},{-5,-3,2},{-5,3,6,7},{5,6,7}}
is_isomorphic_btr_cs(rrbrouwer1999,rbrouwer1999);
 true
     \endverbatim
     </li>
    </ol>
   </li>
   <li> The min-var-degree is not as high as possible (examples?). </li>
   <li> Applying partial assignments we obtain from brouwer1999 quite a
   lot of non-isomorphic clause-sets. </li>
   <li> brouwer1999 is eigensharp. </li>
   <li> One needs to find out about the symmetries of brouwer1999. </li>
  </ul>

*/

