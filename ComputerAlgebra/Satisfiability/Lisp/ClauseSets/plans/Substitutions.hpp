// Oliver Kullmann, 27.7.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/ClauseSets/plans/Substitutions.hpp
  \brief Plans regarding substitutions for clause-sets


  \todo The notion of a "literal substitution":
  <ul>
   <li> These are functions with literals as values; the question is
   whether variables or literals are the basic arguments.
    <ul>
     <li> Mathematically, (all) literals (relevant here) seems appropriate.
     </li>
     <li> But programmatically variables seems more appropriate, since
     so redundancy in the presentation is avoided. </li>
     <li> Perhaps it's best to view substitutions as homomorphisms, and
     then we can specify a literal by an arbitrary mapping on a "free"
     set of literals, for example, a set of variables; in general
     a (finite) free sets of literals is just a clause. </li>
     <li> But perhaps this complication is not worth the effort. </li>
    </ul>
   </li>
   <li> On a set V of variables: each variable v in V -> literal f(v);
   more general, instead of V we have a clause. </li>
   <li> We need then a standard wrapper which extends it to the literals:
   if the input is a negative literal, return -f(-v). This is more expensive
   if we originally defined the substitution on an arbitrary clause. </li>
   <li> Applying a literal substitution f to a clause C is possible iff C
   is contained in the domain of f, and then yields f(C) = {f(x) : x in C}.
   </li>
   <li> Applying a literal substitution f to a clause-set F is possible iff
   f is applicable to all C in F, and the result then is f(F) = {f(C) :
   C in F}. </li>
   <li> A "var-substitution" is a literal substitution which doesn't flip
   signs, i.e., for a variable v in the domain of f also f(v) is a variable.
   </li>
   <li> A "sign-substitution" is a literal substitution which doesn't change
   underlying variables, i.e., for x in the domain of f we have var(f(x)) =
   var(x). </li>
   <li> A problem is that substitution are not generally applicable
   to arbitrary literals.
    <ol>
     <li> One could extend them, using that "false" or "0" is not a literal.
     </li>
     <li> We could use (additionally) "elsub" etc. for these extended
     versions? </li>
     <li Or should we only use the extended versions? No, it is unnatural
     to assume all maps are global. </li>
     <li> But these extended versions should know about their domains? </li>
     <li> Perhaps these extended substitutions are pairs [V,f], where V is
     the variable-domain. </li>
     <li> Not necessarily: the most natural case for an extended substitution
     is given by a hash-map, where the domain is extractable, but where
     it would be unnatural to additionally carry it around. </li>
     <li> So an "extended substitution" in difference to a "normal
     substitution" can be applied to arbitrary literals, acting
     identical for literals not in their domain, but the domain is
     only implicite. </li>
    </ol>
   </li>
   <li> Abbreviations:
    <ol>
     <li> "lsub" for "literal-substitution". </li>
     <li> "vsub" for "variable-substitution". </li>
     <li> "ssub" for "sign-substitution". </li>
     <li> These only operate on variables. Extended to literals, clauses,
     clause-sets, we could use the prefixes used for them, perhaps
     with an underscore. </li>
     <li> So for example "fcs_lsub". </li>
    </ol>
   </li>
   <li> Given a basic literal-substitution, it needs to be extended to
   literals:
    <ol>
     <li> Since it's just a case distinction, it seems just a trivial
     lambda-wrapper is appropriate. </li>
     <li>
    </ol>
   </li>
   <li> So all-together, we have lsub,vsub,ssub for the behaviour of the
   substitutions, and v_xsub, l_xsub, c_xsub, cs_xsub etc. for their
   extensions. There are basic tools for creating v_xsub objects,
   and tools to extend v_xsub. These tools can create global l_xsub
   objects, but this is not the responsibility of the substitution,
   but only of the user. </li>
  </ul>


  \todo standardise_fcs
  <ul>
   <li> Perhaps, once a framework for literal-substitutions is in place,
   then the renaming-functions are updated using these more general
   substitution-maps instead of hash-maps. </li>
   <li> Ask on maxima-mailing-list whether a parallel substitution
   is available (this should speed up renaming). </li>
   <li> DONE The current implementation (using iterated substitution) is
   incorrect in case the clause-set uses already natural numbers as
   variables. </li>
   <li> DONE Otherwise, investigate how hash-maps can be made available,
   store the (whole) substitution via a hash-map, and compute
   the new clause-set via transforming clause for clause. </li>
   <li> DONE (we can now rename w.r.t. a given list of variables)
   Perhaps we could establish general renaming functionality. </li>
  </ul>

*/

