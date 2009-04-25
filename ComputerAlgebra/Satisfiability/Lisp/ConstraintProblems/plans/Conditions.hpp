// Oliver Kullmann, 25.4.2009 (Swansea)
/* Copyright 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/Satisfiability/Lisp/ConstraintProblems/plans/Conditions.hpp
  \brief Plans regarding the basic notions of "conditions" and "constraints"


  \todo Connections
  <ul>
   <li> See ComputerAlgebra/Satisfiability/Lisp/plans/SatisfactionProblems.hpp
   for (older) general plans.
   </li>
  </ul>


  \todo 0,1 versus false,true
  <ul>
   <li> Shall a condition return 0,1 or false,true ? </li>
  </ul>


  \todo Notions, names and abbreviations
  <ul>
   <li> "cdn" for "condition". </li>
   <li> "cst" for "constraint". </li>
   <li> The notion of "constraint"
    <ol>
     <li> Are  positional arguments underlying the evaluation function,
     or named arguments, i.e., variables? That is, is a solution presented
     by a tuple or by a clause? </li>
     <li> I (OK) regard it as an innovation and improvement to use
     clauses (in the various forms). </li>
     <li> But then, since constraints are then nothing else then formal
     clause-sets (in their various forms), perhaps we should drop the notion?
     </li>
     <li> We can not just speak of "clause-sets", but must qualify them
     as CNF or DNF. </li>
     <li> Perhaps we should just use "constraint" for a full formal clause-set
     in DNF-interpretation, and in general we speak of CNF/DNF formal
     clause-sets? </li>
     <li> Or we use "constraint" really when using sets of tuples of values
     and an ordered domain of variables? As a kind of (slighly) compressed
     representation of formal clause-sets? </li>
     <li> What about active literals, active clauses and active clause-sets?
     Likely these are refinements which speak about the various interfaces.
     </li>
    </ol>
   </li>
   <li> Falsifying versus satisfying assignments in a constraint
    <ol>
     <li> We could speak of "negative" versus "positive" constraints,
     or of "CNF-" versus "DNF-" constraints, or just of CNF- and
     DNF-clause-sets. </li>
    </ol>
   </li>
  </ul>


  \todo snbl2cdn
  <ul>
   <li> See "Creating function-terms via lambda", especially "How to simplify
   the function term?", for the general discussion of how to simplify the
   lambda-function in case the sign is a concrete value. </li>
   <li> For variables and values we can not distinguish between "concrete
   instances" and meta-variables, since they are just arbitrary terms
   (if not standardised), however a sign is either -1 or +1, and so
   here we can allow (or not) meta-variables, e.g., [1,2,x]. </li>
   <li> When allowing meta-variables, the question then is what do we
   assume on their evaluation (for the created lambda-function)? If the
   evaluation neither determines +1 or -1, should we then just return
   the unevaluated term (itself), or shall we return "unknown" (as we
   do now)? </li>
   <li> The is(x=y)-test used returns false if equality can not be established,
   and accordingly we have then is(x#y)=true in such cases, where actually
   "unknown" would perhaps be more appropriate. </li>
   <li> This could be repaired by using "is(equal(x,y))" instead of "is(x=y)",
   since the former uses a "full" evaluation, while the latter "approximates"
   syntactical equality by only performing "simple" evaluations. </li>
   <li> The default for the Maxima-variable "prederror" is false, and
   setting it to true does not change anything here when using "=", but when
   using "equal" then an error would be triggered. </li>
   <li> The current general understanding in the Maxima-part of the OKlibrary
   is that "variables, literals" etc. are simple syntactical objects, and
   we should not assume that we can make them variable in sophisticated
   ways. But nevertheless, by using "is(x=y)" in this context we treat
   positive literals rather different from negative literals (for undetermined
   values)? But this might be justified, since positive literals in "most"
   cases are false, while negative literals in "most" cases are true. </li>
  </ul>

*/
