// Matthew Gwynne, 16.7.2009 (Swansea)
/* Copyright 2009, 2010, 2011, 2012 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/PseudoBoolean/plans/CardinalityConstraints.hpp
  \brief Plans for translations of cardinality constraints into CNF


  \todo Connections
  <ul>
   <li> See "Cardinality constraints" in
   ComputerAlgebra/Satisfiability/Lisp/FiniteFunctions/plans/Thresholds.hpp
   for the boolean-function point-of-view. </li>
   <li> And see "Translations to SAT" in
   ComputerAlgebra/Hypergraphs/Lisp/Transversals/Bounded/plans/general.hpp
   for the important application of finding hypergraph transversals of
   bounded size. </li>
  </ul>


  \todo Representing "cardinality constraints"
  <ul>
   <li> DONE (fixed now the notion of a "cardinality constraint")
   We need some direct representation of a condition that the (integer)
   sum of some given boolean variables is =,>,>=,<,<= some bound B. </li>
   <li> We need also some general scheme for such special
   constraint-representations. </li>
   <li> See "The notion of a pseudo-boolean constraint" in
   ComputerAlgebra/Satisfiability/Lisp/PseudoBoolean/plans/general.hpp. </li>
   <li> Let's use "crd" for "cardinality constraint", "crdl" for lists of
   cardinality constraints, "crds" for sets, and "fcrdl" resp. "fcrds" for
   "formal" lists resp. sets of cardinality constraints. </li>
   <li> DONE
   A "cardinality constraint" is a triple [a,L,b], where L is a
   list of literals, while a,b are natural numbers or 0 or inf. </li>
   <li> The meaning is that for the number s of satisfied literals in L
   we have a <= s <= b. </li>
   <li> DONE (for cardinality constraints the translations into sums is not
   of real relevance)
   It would be good if for the relation we would not just use some string,
   but the actually Maxima-presentation of the corresponding Maxima-operator:
    <ol>
     <li> Now how to refer to the Maxima-operator "<" ? Apparently this is done
     by using strings. Now how from a string to get back the operator?
     Apparently by for example subst("<=",r,r(3,4)), which yields 3 <= 4. </li>
     <li> For example
     \verbatim
C : ["cardinality", "<", {v1,-v2,v3}, 6];
Cs : subst(C[2], r, r(sum_l(listify(C[3])), C[4]));
  v3-v2+v1 < 6
Csa : at(Cs, [v1=1, v2=0, v3=1]);
  2 < 6
is(Csa);
  true
     \endverbatim
     </li>
     <li> DONE
     The literals here however are boolean literals, so substitution
     of values 0,1 seems inappropriate? On the one hand, boolean variables
     are supposed to be "positive functions", and also we have variables like
     "1". </li>
     <li> DONE (the sum-representation is not really appropriate here)
     Perhaps we make the distinction that if a cardinality constraint
     occurs in a mixed problem, then the interpretation of the other part
     (boolean or non-boolean CNF or DNF) is determinative, while free-standing
     cardinality constraints are interpreted in the ordinary arithmetic sense
     as above, based on further specifications of the domains of the variables
     involved. </li>
     <li> DONE (again, not really appropriate here)
     So in the context of a boolean clause-set one can use cardinality
     constraints like ["cardinality", "=", {1,2,3}, 2], which is equivalent
     to the CNF {{1,2},{1,3},{2,3},{-1,-2,-3}} (while
     ["cardinality", "=", {1,-2,3}, 2] is equivalent to the CNF
     {{1,-2},{1,3},{-2,3},{-1,2,-3}}). </li>
     <li> However, as argued below, the cardinality constraint is only
     concerned about *literals* which are set to true, and so implicitly
     in this context always literals set to false are regarded as zero. </li>
     <li> So it seems that the above Maxima-sum (like "v3-v2+v1") is not
     really appropriate here --- first the literals have to be evaluated,
     and then "true" is interpreted as 1, while "false" is interpreted as 0.
     </li>
    </ol>
   </li>
   <li> What are non-boolean cardinality constraints?
    <ol>
     <li> It seems the appropriate generalisation would be to ask that amongst
     the given set of literals at least, exactly, at most etc. of
     the literals are satisfied by the assignment. </li>
     <li> So one sees that it is not sensible to add up *variables*, but
     literals are to be considered, which are (relative to some (total)
     assignment) either true or false, and one considers then only the
     literals which are true, and makes a requirement on their count. </li>
    </ol>
   </li>
   <li> DONE (the distinction doesn't seem to be of importance)
    Lists or sets:
    <ol>
     <li> With a list we generalise cardinality constraints in the direction
     of pseudo-boolean constraints. </li>
     <li> Perhaps it makes sense to have set-cardinality-constraints, and
     list-cardinality-constraints (as a generalisation). </li>
    </ol>
   </li>
   <li> DONE (no type identifiers)
   Perhaps just "card" instead of "cardinality" is enough as the first
   element of the list.
    <ol>
     <li> Actually, yet we do not use such type-identifiers,
     but it is the context which determines the nature of the objects? </li>
     <li> So we should just drop the "card"-string as part of a cardinality
     constraint? </li>
     <li> A point in favour of having this identify would be if such
     cardinality constraints would be instances of more general invocations
     of "constraints", all just thrown together in one constraint-container.
     </li>
     <li> However, here we should consider cardinality-constraints just as
     generalisations of clauses, and so we should just employ triples like
      ["=", {1,2,3}, 2]. </li>
    </ol>
   </li>
   <li> DONE
   How to call these constraints?
    <ol>
     <li> "crd" for "cardinality constraint"? </li>
     <li> And then further specialised regarding boolean or non-boolean
     variables, and CNF- or DNF-interpretation of variables? </li>
     <li> Perhaps we aim at a symbolism where "crd" can just replace
     "clause", while otherwise everything else is reused?! </li>
    </ol>
   </li>
   <li> Mixed problems:
    <ol>
     <li> It seems mixed problem instances should be lists of problem
     instances. </li>
     <li> Using "cs-crd" for a pair of (boolean) clause-set and (necessarily
     boolean) cardinality constraint. </li>
     <li> For such lists we should have the same type of literals for all
     components. </li>
    </ol>
   </li>
  </ul>


  \todo Direct realisations
  <ul>
   <li> For all types of clause-sets (boolean and non-boolean, CNF and DNF)
   and all types of (in)equalities provide the direct (combinatorial)
   realisations. </li>
   <li> The CNF-representation for boolean literals is given now by
   direct_crd2cl.
    <ol>
     <li> We should also provide statistic-functions (which, of course,
     compute the statistics directly). </li>
     <li> We need to prove the assertions regarding prime-implicates
     (that is, direct_crd2cl_lt and direct_crd2cl_ge compute repetition-free
     lists of (exactly the) prime implicates; see below). </li>
    </ol>
   </li>
   <li> Let L be a set of boolean literals (of size m).
    <ol>
     <li> sum(L) < B has the CNF-realisation consisting of all B-subsets
     of L, which are (then) complemented. </li>
     <li> Example L = {1,-2,3} and B=2: {{-1,2},{-1,-3},{2,-3}} </li>
     <li> The number of clauses is thus binomial(m,B). </li>
     <li> sum(L) <= B is equivalent to sum(L) < B+1. </li>
     <li> sum(L) >= B has the DNF-realisation consisting of all B-subsets
     of L. </li>
     <li> Example L = {1,-2,3} and B=2: {{1,-2},{1,3},{-2,3}} </li>
     <li> So the number of clauses is binomial(m,B). </li>
     <li> sum(L) > B is equivalent to sum(L) >= B+1. </li>
     <li> sum(L) >= B is equivalent to sum(-L) <= m-B. </li>
     <li> sum(L) <= B is equivalent to sum(-L) >= m-B. </li>
     <li> sum(L) = B is equivalent to sum(L) <= B and sum(L) >= B. </li>
     <li> So the CNF-representation of sum(L) = B is the union of the
     CNF-representations of sum(L) < B+1 and sum(-L) < m-B+1. </li>
     <li> That is, we have all (B+1)-subsets of L, complemented, and
     all (m-B+1)-subsets of L. </li>
     <li> The number of clauses is thus binomial(m,B+1) + binomial(m,m-B+1) =
     binomial(m,B+1) + binomial(m,B-1). </li>
     <li> Example L = {1,-2,3} and B=2: {{-1,2,-3},{1,-2},{1,3},{-2,3}}. </li>
     <li> And the DNF-representation of sum(L) = B is the product of the
     DNF-representations of sum(L) >= B and sum(-L) >= m-B. </li>
     <li> That is, we have of B-subsets of L and all (m-B)-subsets of
     -L. </li>
     <li> Example L = {1,-2,3} and B=2:
     {{1,-2},{1,3},{-2,3}} x {{-1},{2},{-3}} = {{1,-2,-3},{1,2,3},{-1,-2,3}}.
     </li>
     <li> One sees that the DNF-representation just lists all total satisfying
     assignments (no resolution is possible between them). </li>
     <li> The number of clauses is thus binomial(m,B). </li>
    </ol>
   </li>
   <li> These clause-sets coincide with the sets of all prime implicates resp.
   prime implicants:
    <ol>
     <li> Only for the "="-forms resolutions are possible. </li>
     <li> For the CNF-representations all clashing clauses overlap
     in at least two literals, and thus all resolutions are blocked. </li>
     <li> And above we already realised that no resolution is possible
     regarding the DNF-realisations. </li>
    </ol>
    So without using additional variables these representations are optimal.
   </li>
   <li> And also using the DNF-representations seems always nearly to use
   the same number of clauses, so that translations DNF -> CNF seem fruitless.
   </li>
  </ul>


  \todo Simplifications
  <ul>
   <li> DONE (yes, sort is stable, and ">" is to be used)
   For crd2scrd we have the following problems:
    <ol>
     <li> Is sort stable?? </li>
     <li> Is ">" to be used instead of ">="?? </li>
    </ol>
    This problems are of general relevance. See "Stable sorting" in
    ComputerAlgebra/plans/Maxima.hpp. </li>
  </ul>


  \todo Application of partial assignments
  <ul>
   <li> As a special case of "Application of partial assignments" in
   ComputerAlgebra/Satisfiability/Lisp/PseudoBoolean/plans/general.hpp,
   the result of phi*[a,L,b] = [a',L',b'] is obtained by removing the
   variables of phi from L, and subtracting the number of satisfied literals
   in L from a and b. </li>
  </ul>


  \todo At-most-one as a CNF
  <ul>
   <li> Of special interest are the representations of the boolean function
   amo_bf(n) (see "Cardinality constraints" in
   Satisfiability/Lisp/FiniteFunctions/plans/Thresholds.hpp), that is, of the
   cardinality constraint [0, [x_1,...,x_n], 1]. </li>
   <li> The standard CNF-representation by direct_crd2cl uses binomial(n,2)
   clauses {-x_i,-x_j}. This is also the set of prime implicates (all are
   essential). </li>
   <li> There are n+1 prime implicants, namely those full clauses having
   at most one positive literal, and all these are (again) essential. </li>
   <li> CNF-representations using new variables:
    <ol>
     <li> The canonical transformation is likely not useful here, but due to
     general interest it should be studied. </li>
     <li> Using new variables s_1, ..., s_{n-1}, a CNF-representation is given
     by the clauses L(n):
      <ol>
       <li> x_i -> s_i for 1 <= i <= n-1 </li>
       <li> x_n -> -s_{n-1} </li>
       <li> s_i -> s_{i+1} for 1 <= i <= n-2 </li>
       <li> s_i -> -x_{i+1} for 1 <= i <= n-1. </li>
      </ol>
     </li>
     <li> The meaning of the equivalent boolean function
     f(x_1,...,x_n, s_1,...,s_{n-1}) is
      <ol>
       <li> The s_i are defined from the x_j by: s_i is true iff some
       x_j for j <= i is true. </li>
       <li> f is true iff at most one of the x_i is true. </li>
       <li> Actually, this function f is not equivalent to L(n), for
       example L(n) allows to set all x_i to false while setting all
       s_i to true (while one could also set all s_i to false here). </li>
       <li> The above L(n) does not have the unique-extension-property (uep).
       </li>
       <li> The correct explanations of the s_i is that setting one x_i to
       true forces all s_j for i <= j <= n-1 to be true, while otherwise they
       are undetermined. </li>
       <li> Making it uep, by actually implementing f, doesn't seem so cheap?
       </li>
      </ol>
     </li>
     <li> L(n) is primal. </li>
     <li> The propagation-hardness of L(n) is 1 (there are no forced
     assignments). </li>
     <li> And the hardness of L(n) is 1 (it is satisfiable, while not
     containing all prime implicates). </li>
     <li> Is L(n) also a 1-base? </li>
     <li> L(n) is first introduced in [Towards an optimal CNF encoding of
     Boolean cardinality constraints; Carsten Sinz, 2005, bottom of page 2].
     </li>
   </li>
  </ul>


  \todo Allow repeating literals for unary_bb_crd2fcl
  <ul>
   <li> It seems that the current implementation already can handle arbitrary
   cardinality-constraints (with repetition of literals), however this needs
   to be checked. </li>
  </ul>


  \todo Improved understanding/implementation of BB-method
  <ul>
   <li> The right starting point should be to explicitly use "the tree" alluded
   to in the paper. </li>
   <li> Of course, we need a name: "unary addition tree". </li>
   <li> The simplest structure is that of a labelled tree (recall
   ComputerAlgebra/Trees/Lisp/Basics.mac), where every inner %node has
   precisely two children, for a natural number n >= 1:
   \verbatim
unary_addition_tree_0_lrt(n) := if n=1 then [[1]] else
 block([a : floor(n/2), b], b : n - a,
  return([[n], unary_addition_tree_0_lrt(a), unary_addition_tree_0_lrt(b)]))$

draw_lrt_dbl(unary_addition_tree_0_lrt(5),d:inf);
/* Should be draw_lrt(unary_addition_tree_0_lrt(5),d:inf); */
   \endverbatim
   b could also be defined as ceiling(n/2). </li>
   <li> Next we provide additional labels, namely the variables used at the
   leaves for input, the auxiliary variables used at inner non-root nodes for
   the unary representation, and the list of output variables at the root.
   </li>
   <li> n is then implicitly given as the (coinciding) lengths of the lists
   of input and output variables. </li>
   <li> An important aspect of the unary addition to be performed via binary
   splitting (as outlined via unary_addition_tree_0_lrt) is, that always
   literals with indices from an interval [a,b] are added, where for the
   root we have a=1 and b=n, while for the leaves we have a=b=i for 1 <= i <=
   n. </li>
   <li> This is presented by the following tree:
   \verbatim
unary_addition_tree_1_lrt(n) := unary_addition_tree_1r_lrt(1,n)$
/* Prerequisite: a, b natural numbers, a <= b */
unary_addition_tree_1r_lrt(a,b) := if a=b then [a] else
 block([d : floor((b-a+1)/2)],
  [[a,b], 
   unary_addition_tree_1r_lrt(a,a+d-1), unary_addition_tree_1r_lrt(a+d,b)])$

draw_lrt_dbl(unary_addition_tree_1_lrt(5));
/* draw_lrt(unary_addition_tree_1_lrt(5),d:inf); */
   \endverbatim
   </li>
   <li> For T = unary_addition_tree_1_lrt(n), the leaves have labels i
   corresponding to input-literal i, while inner nodes have labels [a,b],
   corresponding to auxiliary variables vru(a,b,1), ..., vru(a,b,b-a+1). </li>
   <li> So the tree showing the literals and auxiliary variables used is as
   follows, where L is an arbitrary list of literals:
   \verbatim
unary_addition_tree_2_lrt(L) := unary_addition_tree_2r_lrt(L,1,length(L))$
unary_addition_tree_2r_lrt(L,a,b) := block([l : b-a+1],
 if l = 1 then L else
 block([d : floor(l/2)],
  [create_list(vru_var(a,b,i),i,1,l),
   unary_addition_tree_2r_lrt(take_elements(d,L),a,a+d-1),
   unary_addition_tree_2r_lrt(rest(L,d),a+d,b)]))$

draw_lrt_dbl(unary_addition_tree_2_lrt([a,b,c,d,e]));
/* draw_lrt(unary_addition_tree_2_lrt(5),d:inf); */
   \endverbatim
   </li>
   <li> Now only the function for the clause-set describing the relation
   between a non-leaf %node and its two children is needed, and then the
   totaliser-function is obtained by traversing the tree. </li>
   <li> This relation is that of unary addition. </li>
   <li> In the case of [BB 2003], unary addition of a list of literals A
   and B with output variables C is encoded by the formal clause-list
   generated by the following function
   \verbatim
unary_bb_add_fcl(A,B,C) := block([F:[]],
    for alpha : 0 thru length(A) do
      for beta : 0 thru length(B) do block([sigma : alpha+beta],
        if sigma > 0 then
          F : cons(union(
              if alpha > 0 then {-A[alpha]} else {},
              if beta > 0 then {-B[beta]} else {},
              {C[sigma]}), F),
          if sigma < length(C) then
          F : cons(union(
              if alpha < length(A) then {A[alpha+1]} else {},
              if beta < length(B) then {B[beta+1]} else {},
              {-C[sigma+1]}),F)
          ),
    return([stable_unique(map(var_l,append(A,B,C))),F]))$
   \endverbatim
   </li>
   <li> See
   Experimentation/Investigations/BooleanFunctions/plans/UnaryAddition.hpp
   for investigations (there might be better choices than in the paper). </li>
  </ul>


  \todo Add statistics functions
  <ul>
   <li> These statistics functions rely on precise (combinatorial)
   specifications. </li>
  </ul>
   

  \todo Docus
  <ul>
   <li> DONE (these texts have been moved here)
   The current amount of text one finds in
   ComputerAlgebra/Satisfiability/Lisp/PseudoBoolean/CardinalityConstraints.mac
   is inappropriate --- function specifications should be rather short and
   succinct. </li>
   <li> If appropriate, these texts should be moved to the docus. </li>
   <li> So this text has a place only in the docus:
   \verbatim
   A CNF representation of a boolean cardinality constraint [a,L,b] with
   "detection of forced assignments via UCP" is a CNF representation of [a,L,b]
   with the property that for every partial assignment phi with
   var(phi) <= var(L) and every forced assignment <v -> e> for apply_pa(phi,F)
   with v in var(L), applying unit-clause-propagation to apply_pa(phi,F)
   includes this forced assignment (i.e., UCP derives all forced assignments
   for apply_pa(phi,F) which concern the cardinality constraint).
   \endverbatim
   </li>
   <li> And so does this text:
   \verbatim
   The following algorithm, from the above mentioned paper, computes a CNF
   representation F of the cardinality constraint [a,L,b] with detection of
   forced assignments via UCP.

   The algorithm takes as input a list E of literals, and then introduces
   a list of new (distinct) variables S of the same size as E, which under any
   assignment will have the unary representation (ones/trues appearing from
   the left) of the cardinality of E. The remainder of the algorithm consists
   of two phases, the totalizer and the comparator, and the clauses
   generated (and variables introduced) by these phases are then appended
   together to produce the final clause-set. The totalizer enforces that
   S is the unary representation of the cardinality of L and the comparator
   enforces that this unary representation represents a number in the range
   a to b.

   The totalizer takes as input a list E of input variables, and as
   output a list S of cardinality variables, and returns a list of
   clauses that enforce that S is a unary representation of E. This
   is done in a recursive manner, through the introduction of new variables,
   and clauses enforcing unary addition.
   
   In the totalizer, E is split into two sublists E_1 and E_2, where E_1 is
   the list of the first floor(length(E)/2) elements and E_2 is the remainder
   of E. The algorithm then introduces two lists of new (distinct) variables
   S_1 and S_2, where S_1 and S_2 are the same size as E_1 and E_2
   respectively. Totalizer is then run again for each of these sublists E_1
   with S_1 and E_2 with S_2 returning (new) variable and clause-lists V_1,C_1
   and V_2,C_2 respectively, and then the following clauses (C_3) are
   generated to enforce that in any satisfying assignment of F, we have that S
   is the unary addition of S_1 and S_2:


     for every 
       0 <= a <= length(E_1) and 
       0 <= b <= length(E_2) and
       0 <= c <= length(L) such that
       a + b = c we add the clauses

         1) {-S_1[a],-S_2[b],S[c]} and
	 2) {S_1[a+1], S_2[b+1], S[c+1]}

   where (1) ensures (S_1[a] and S_2[b]) => S[a+b]), i.e., the unary
   addition holds in one direction and (2) ensures
   (S[a+b+1] => (S_1[a+1] or S_2[b+1]), i.e., the other direction, and we have
       
     S_1[0] = S_2[0] = S[0] = 1 and 
     S_1[length(E_1)+1] = S_2[length(E_2)+1] = S[length(L)+1] = 0

   for the extreme cases for the above clauses.

   In the base case where length(E) = 1, totalizer takes S_1 = E_1, and
   S_2 = E2, since E_1 and E_2 are their own unary representations.

   The comparator generates a list of unit-clauses

       [{S[1]},...,{S[a]},{S[b+1]},...,{S[length(L)]}]

   which ensures that, given that the totalizer ensures that S is the unary
   representation of the cardinality of E, any satisfying assignment
   which results in E having a cardinality less than a, or greater than b, 
   is a falsifying assignment for F.

   To use this algorithm see unary_bb_crd2fcl.
   \endverbatim
   </li>
   <li> Also the explanations for unary_bb_totaliser_fcl are misplaced:
   \verbatim
   The list of new variables introduced by this function can therefore be 
   recursively defined using a function vc, where
   V = rest(vc(E,1,length(E)), length(E)):

   vc(E,a,b) = [] if a == b
             else append(
               E, 
               vc([vru(a,ceiling((a+b)/2-1),1),...,
                   vru(a,ceiling((a+b)/2-1),floor(length(E)/2))],
                 a,floor((a+b)/2)),
               vc([vru(ceiling((a+b)/2-1)+1,b,1),...,
                 vru(ceiling((a+b)/2-1)+1,ceiling(length(E)/2))],
                 ceiling((a+b)/2-1)+1,b))

   Therefore the number of new variables introduced given length(E) = n is
   nvc(n) - n where nvc is defined recursively in the following way:

   nvc(n) = 0 if n < 1
            n + nvc(floor(n/2)) + nvc(ceiling(n/2)) otherwise

   Assuming S is without complimentary literals, a partial assignment phi
   satisfies F iff phi is total and given n is the cardinality of E under phi,
   we have that

     phi(S[i]) = 1 for 1 <= i <= n and phi(S[i]) = 0 for i > n,

   and given m is the cardinality of E[1,...,floor(length(E)/2)] we have

     phi(vru(1,floor(length(E)/2),i)) = 1 for 1 <= i <= m and
     phi(vru(1,floor(length(E)/2),i)) = 0 otherwise

   and given m' is the cardinality of E[floor(length(E)/2)+1,...,length(E)]
   we have
   
     phi(vru(floor(length(E)/2)+1,length(E),i)) = 1 for 1 <= i <= m' and
     phi(vru(floor(length(E)/2)+1,length(E),i)) = 0 otherwise

   and for all a and b such that we have a new variable vru(a,b,i) for some i,
   if b-a = 2 then we have

     phi(vru(a,b,1)) = phi(E[a]) xor phi(E[b])
     phi(vru(a,b,2)) = phi(E[a]) and phi(E[b])

   if b-a = 3 then we have

     phi(vru(a,b,1)) = phi(E[a]) or phi(vru(a+1,b,1))
     phi(vru(a,b,2)) = (phi(E[a]) and phi(vru(a+1,b,1))) or phi(vru(a+1,b,2))
     phi(vru(a,b,3)) = phi(E[a]) and phi(vru(a+1,b,2))

   and if b-a > 3 then we have

     phi(vru(a,b,i)) =
       there exist j,k such that j+k=i
         phi(vru(a,ceiling(a+b/2-1),j)) and phi(vru(ceiling(a+b/2-1)+1,b,k))

   i.e., in each case [vru(a,b,1),...,vru(a,b,b-a+1)] is the
   representation of the unary addition of E[a] and E[b], E[a] and
   [vru(a+1,b,1),vru(a+1,b,2)] or [vru(a,b',1),...,vru(a,b',b'-a+1)] and
   [vru(b'+1,b,1),...,vru(b'+1,b,b-b')] respectively depending on
   a and b, where b' = ceiling((a+b)/2-1).

   unary_bb_totalizer_r_fcl is a function with additional parameters
   to keep track of any new variables introduced, and has the same
   specification as unary_bb_totaliser_fcl, except
   V = rest(vc(E,a,b), length(E)).
   \endverbatim
   </li>
  </ul>


  \todo Implement adder-circuit translation
  <ul>
   <li> Also provide the translation using the Tseitin translation of
   the boolean adder circuits. </li>
   <li> See Satisfiability/Transformers/Generators/plans/LinInequal.hpp for 
   a C++ implementation. </li>
   <li> Joost Warners dissertation also describes such a translation; are
   there differences? </li>
  </ul>


  \todo Cardinality constraints as active clauses
  <ul>
   <li> As a special case of "Pseudo-boolean constraints as active clauses"
   (see ComputerAlgebra/Satisfiability/Lisp/PseudoBoolean/plans/general.hpp),
   cardinality constraints should yield a very attractive first model of
   "active clauses". </li>
   <li> Their interface as a set of clauses is given by their prime implicates
   representation (see "Direct realisations" below). </li>
  </ul>


  \todo Implement other methods using unary representation
  <ul>
   <li> [Een, Soerensson, 2006, Translating Pseudo-Boolean Constraints into
   SAT] contains another translation. </li>
   <li> And so does [Sinz, 2005, Towards an Optimal Encoding of Boolean
   Cardinality Constraints]. </li>
   <li> The newest article is [Asin et al, SAT 2009, Cardinality networks and
   their applications]. Perhaps this should be the starting point. </li>
   <li> In [Bailleux et al, SAT 2009, New encodings for pseudo-boolean
   constraints into CNF] one finds further remarks regarding the unary
   encoding. </li>
  </ul>


  \todo DONE Rename functions related to unary encoding
  <ul>
   <li> These functions realise only special implementations, and so a generic
   name like "cardinality_cl" is inappropriate. </li>
   <li> We should also use English spelling, for example "totaliser". </li>
   <li> "cardinality_cl" should become "unary_bb_crdl2fcl". </li>
  </ul>


  \todo DONE Functions such as cardinality_totalizer_cl should take cardinality
  constraints
  <ul>
   <li> Currently functions such as cardinality_totalizer_cl and
   cardinality_comparator_cl take the parameters given in the
   boolean constraint [a,L,b] individually. </li>
   <li> These functions should take the whole constraint as a single argument
   as in other functions such as crd2cl. </li>
  </ul>


  \todo DONE Provide complete specifications (related to unary encoding)
  <ul>
   <li> This relates to the algorithm implemented by MG according to
   [Bailleux, Boufkhad, 2003]. </li>
   <li> For cardinality_totalizer_cs, cardinality_comparator_cs and
   cardinality_cl *combinatorial* specifications are needed.
    <ol>
     <li> Currently there are no specifications (which, of course, must
     allow to reconstruct the clause-sets(!)). </li>
     <li> Some properties are mentioned in
     ComputerAlgebra/Satisfiability/Lisp/PseudoBoolean/CardinalityConstraints.mac,
     but these properties shouldn't be there (but possibly in the docus). </li>
    </ol>
   </li>
   <li> Then two levels of tests are needed: One which checks the
   (precise) combinatorial specification, and another one which checks
   the (general) specification as a presentation of some constraint.
    <ol>
     <li> Currently the tests are not specified at all. </li>
     <li> Of course, first the basic functions need to be specified. </li>
    </ol>
   </li>
   <li> This todo has been replaced by
    <ul>
     <li> "Partial assignments for CNFs with detection of forced assignments 
     via UCP" </li>
     <li> "Ordering for CNFs with detection of forced assignments via UCP" 
     </li>
    </ul>
   </li>
   <li> DONE Especially precise information on the added auxiliary variables are
   needed. </li>
  </ul>


  \bug DONE Nonsensical documentation
  <ul>
   <li> In
   ComputerAlgebra/Satisfiability/Lisp/PseudoBoolean/CardinalityConstraints.mac
   two large blocks of text are (exactly??) identical. </li>
   <li> This makes no sense. </li>
   <li> OK started to update the documentation, and the requests posed by OK
   should be answered as soon as possible (both in
   ComputerAlgebra/Satisfiability/Lisp/PseudoBoolean/CardinalityConstraints.mac
   and in
   ComputerAlgebra/Satisfiability/Lisp/PseudoBoolean/tests/CardinalityConstraints.mac
   . </li>
   <li> Also all the bad repetitions need to be removed. </li>
   <li> See 
    <ul>
     <li> "Partial assignments for CNFs with detection of forced assignments 
     via UCP" </li>
     <li> "Ordering for CNFs with detection of forced assignments via UCP" 
     </li>
    </ul>
   </li>
  </ul>


  \todo DONE Change specification of variables in cardinality_cl etc.
  <ul>
   <li> Currently the variables in cardinality_totalizer_cl, 
   cardinality_totalizer_cl etc are of the form "ctt(l,i)" where
   l is a list specifying where in the recursive procedure these variables
   are introduced. </li>
   <li> This representation relies heavily on the algorithm, and means
   that it is essentially impossible to properly define or describe the
   variables separately from the algorithm. </li>
   <li> However, these variables are simply the unary representation of
   the cardinality of a certain sublist L' of the list of input literals L, and
   therefore a better representation would be "ctt(a,b,i)" where a and b 
   define the upper and lower bound of the L' in L. </li>
   <li> Then for example, the initial variables list passed into
   cardinality_totalizer_cl as S, would be 
   [ctt(1,length(E),1),...,ctt(1,length(E),length(E))] and then within 
   cardinality_totalizer_cl, the two new lists of variables produced would be
   [ctt(1,length(E_1).1),...,ctt(1,length(E_1),length(E_1))] 
   and 
   [ctt(length(E_1)+1,length(E),1)...,ctt(length(E_1)+1,length(E),length(E_2))]. 
   </li>
  </ul>

*/

