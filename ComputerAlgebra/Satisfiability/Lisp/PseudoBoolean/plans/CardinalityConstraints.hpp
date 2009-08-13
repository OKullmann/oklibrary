// Matthew Gwynne, 16.7.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/PseudoBoolean/plans/CardinalityConstraints.hpp
  \brief Plans for Maxima-generators (of SAT-problems) related to cardinality constraints


  \bug Nonsensical documentation
  <ul>
   <li> In
   ComputerAlgebra/Satisfiability/Lisp/PseudoBoolean/CardinalityConstraints.mac
   two large blocks of text are (exactly??) identical. </li>
   <li> This makes no sense. </li>
  <ul>


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
   <li> We need some direct representation of a condition that the (integer)
   sum of some given boolean variables is =,>,>=,<,<= some bound B. </li>
   <li> We need also some general scheme for such special
   constraint-representations. </li>
   <li> See "The notion of a pseudo-boolean constraint" in
   ComputerAlgebra/Satisfiability/Lisp/PseudoBoolean/plans/general.hpp. </li>
   <li> The scope of "cardinality constraints":
    <ol>
     <li> Following our general philosophy of using lists, we could use a
     list with first element a string which identifies the constraint;
     in this case the string would be "cardinality". </li>
     <li> The second element would be one of the five relations. </li>
     <li> Then comes the set L of relevant literals. </li>
     <li> Finally the number B. </li>
     <li> See below for arguments to remove the constraint identifier. </li>
     <li> However, relations ">" and "<" are superfluous, and we should only
     consider "<=", ">=" and "=". </li>
     <li> Or, alternatively, only "<", ">" and "=" ? </li>
     <li> Additionally, inbetween-constraints "a <= sum(L) <= b" could
     be useful (see "cardinality_cs"). </li>
     <li> Perhaps the elements of the cardinality-constraint-lists are
     arranged as for the represented inequalities:
      <ul>
       <li> [L,">=",a] </li>
       <li> [L,"<=",a] </li>
       <li> [L,"=",a] </li>
       <li> [a,"<=",L,"<=",b] </li>
      </ul>
      Likely we should only allowed such standardised forms.
     </li>
     <li> One could just use the form [a,"<=",L,"<=",b], since all three other
     forms can be expressed by it (using [a,"<=",L,inf], [0,"<=",L,"<=",a]
     and [a,"<=",L,"<=",a]. </li>
     <li> Then one could just use triples [a,L,b]. </li>
     <li> Seems alright that one allows for b now also inf; one should then
     also allow it for a. </li>
    </ol>
   </li>
   <li> It would be good if for the relation we would not just use some string,
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
     <li> The literals here however are boolean literals, so substitution
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
     <li> So one sees that it is not really sensible, as done above, to
     add up *variables*, but literals are to be considered, which are (relative
     to some (total) assignment) either true or false, and one considers then
     only the literals which are true, and makes a requirement on their
     count. </li>
    </ol>
   </li>
   <li> Lists or sets:
    <ol>
     <li> With a list we generalise cardinality constraints in the direction
     of pseudo-boolean constraints. </li>
     <li> Perhaps it makes sense to have set-cardinality-constraints, and
     list-cardinality-constraints (as a generalisation). </li>
    </ol>
   </li>
   <li> Perhaps just "card" instead of "cardinality" is enough as the first
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
   <li> How to call these constraints?
    <ol>
     <li> "card" for "cardinality constraint"? </li>
     <li> And then further specialised regarding boolean or non-boolean
     variables, and CNF- or DNF-interpretation of variables? </li>
     <li> Perhaps we aim at a symbolism where "card" can just replace
     "clause", while otherwise everything else is reused?! </li>
    </ol>
   </li>
   <li> Mixed problems:
    <ol>
     <li> It seems mixed problem instances should be lists of problem
     instances. </li>
     <li> Using "cls-card" for a pair of (boolean) clause-set and (necessarily
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
   <li> Let L be a set of boolean literals (of size m).
    <ol>
     <li> sum(L) < B has the CNF-realisation consisting of all B-subsets
     of L, which are (then) complemented. </li>
     <li> Example L = {1,-2,3} and B=2: {{-1,2},{-1,-3},{2,-3}} </li>
     <li> The number of clauses is thus binom(m,B). </li>
     <li> sum(L) <= B is equivalent to sum(L) < B+1. </li>
     <li> sum(L) >= B has the DNF-realisation consisting of all B-subsets
     of L. </li>
     <li> Example L = {1,-2,3} and B=2: {{1,-2},{1,3},{-2,3}} </li>
     <li> So the number of clauses is binom(m,B). </li>
     <li> sum(L) > B is equivalent to sum(L) >= B+1. </li>
     <li> sum(L) >= B is equivalent to sum(-L) <= m-B. </li>
     <li> sum(L) <= B is equivalent to sum(-L) >= m-B. </li>
     <li> sum(L) = B is equivalent to sum(L) <= B and sum(L) >= B. </li>
     <li> So the CNF-representation of sum(L) = B is the union of the
     CNF-representations of sum(L) < B+1 and sum(-L) < m-B+1. </li>
     <li> That is, we have all (B+1)-subsets of L, complemented, and
     all (m-B+1)-subsets of L. </li>
     <li> The number of clauses is thus binom(m,B+1) + binom(m,m-B+1) =
     binom(m,B+1) + binom(m,B-1). </li>
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
     <li> The number of clauses is thus binom(m,B). </li>
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


  \todo Provide complete specifications (related to unary encoding)
  <ul>
   <li> This relates to the algorithm implemented by MG according to
   [Bailleux, Boufkhad, 2003]. </li>
   <li> Especially precise information on the added auxiliary variables are
   needed. </li>
   <li> For cardinality_totalizer_cs, cardinality_comparator_cs and
   cardinality_cs *combinatorial* specifications are needed.
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
  </ul>


  \todo Rename functions related to unary encoding
  <ul>
   <li> These functions realise only special implementations, and so a generic
   name like "cardinality_cs" is inappropriate. </li>
   <li> We should also use English spelling, for example "totaliser". </li>
  </ul>


  \todo Add statistics functions
  <ul>
   <li> These statistics functions rely on precise (combinatorial)
   specifications. </li>
  </ul>
   

  \todo Docus
  <ul>
   <li> The current amount of text one finds in
   ComputerAlgebra/Satisfiability/Lisp/PseudoBoolean/CardinalityConstraints.mac
   is inappropriate --- function specifications should be rather short and
   succinct. </li>
   <li> If appropriate, these texts should be moved to the docus. </li>
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


  \todo Implement other methods using unary representation
  <ul>
   <li> [Een, Soerensson, 2006, Translating Pseudo-Boolean Constraints into
   SAT] contains another translation. </li>
   <li> And so does [Sinz, 2005, Towards an Optimal Encoding of Boolean
   Cardinality Constraints]. </li>
  </ul>

*/

