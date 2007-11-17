// Oliver Kullmann, 24.7.2005 (Swansea)
/* Copyright 2005 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Concepts/plans/Literals.hpp
  \brief Plans for concepts for literals.


  \todo Arity
  <ul>
   <li> Now actually it seems best to have literals with arity n. </li>
   <li> This is accomplished by using the generalised AtomicConditions-concept
   (see Concepts/plans/AtomicConditions.hpp), and by generalising traits::var_type
   so that is actually returns a tuple type (of the same arity as the value-tuple-type
   in traits::cond_type). </li>
   <li> For arity 1 there should be also short-cuts, so that the current syntax works
   (can this be accomplished? there is no conversion from an unary tuple to its
   value-type?) </li>
  </ul>


  \todo Generalised literals:
  <ul>
   <li> More general than above under "Arity" perhaps there should be four types of literals:
    <ul>
     <li> Unary literals (as the current concept of Literal) </li>
     <li> Binary literals </li>
     <li> Literals with arity (that is, with a variable tuple; as under "Arity"); perhaps
     only allowing arities >= 3 here? </li>
     <li> Literals with variable sequence (that is, arbitrary and dynamic arities) </li>
    </ul>
   </li>
   <li> Literals without atomic conditions:
    <ul>
     <li> For the case the literal does not need a full-fledged atomic condition, but just
     expresses a certain fixed relation about the variables (for example "equality" or
     "disequality"), there is an associated type "literal_type<Lit>::type" usable for
     tagging-polymorphism. </li>
     <li> But perhaps the atomic condition should always be available, only for certain literal-types
     it's not really needed. </li>
     <li> These literal-types might sometimes make sense only for certain arities (but equality and disequality
     make sense for arbitrary arities). </li>
    </ul>
   <li> For unary literals we could have the forms
    <ul>
     <li> mono-valued (represented by one value) </li>
     <li> segment-valued (also represented by one value) </li>
     <li> internal-value (represented by two values) </li>
     <li> multi-value (represented by sets of values) </li>
    </ul>
   where for the second and third form we could furthermore distinguish between the open and the closed
   segments resp. intervals. </li>
   <li> Perhaps a general form of literals does not have negation, but there is a refinement
   "with_complement". Potentially one could also have the forms "positive" and "negative", which
   would help for example with equality-literals (the disequality would be the negative form).
   For the first 3 forms of unary literals the positive and negative forms would be meaningful (while
   the fourth form is with_complement). So we would have a hierarchy with "no_complement" as the root,
   two derived forms "positive", "negative", and from both we derive "with_complement" (is this
   meaningful? perhaps all three forms should be derived directly from the root). For partial assignments we need a distinction between "(potential) decision literals" and "non-decision literals", where the decision literals need negation, but the others do not. </li>
  </ul>


  \todo Requirements literals
  <ul>
   <li> It seems best that only refined versions of Literals offer operation
   static_cast<cond_type&>(OKlib::Literals::cond(l));
   while for general literals we have only
   set_cond(x, cond).
   On the other hand, not all literal-types allow changing the condition? So perhaps changing the condition
   is only available for special literal types. </li>
   <li> A general question is whether literals are assumed to be easily copyable, or should we pass
   them by reference? For literals like integers, we should pass them by value, but for literals
   representing sets we should pass them by reference?! So do we need to employ a traits function,
   which decides whether to pass by value or const reference? Seems to be the case, but would make
   writing functions using literals a bit more complicated. First we pass them by const reference,
   and then later we'll see. </li>
   <li> Out of literals we make partial assignments (DNF-clauses) and (CNF-)clauses. </li>
  </ul>

*/

