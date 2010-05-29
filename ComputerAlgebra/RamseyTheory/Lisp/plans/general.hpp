// Oliver Kullmann, 16.4.2010 (Swansea)
/* Copyright 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/RamseyTheory/Lisp/plans/general.hpp
  \brief Plans regarding Ramsey-type problems in general


  \todo Architectures of the systems for showing/computing Ramsey-type numbers
  <ul>
   <li> A system with the basics completed one finds in
   ComputerAlgebra/RamseyTheory/Lisp/VanderWaerden/Numbers.mac. </li>
   <li> Basically the same system is
   ComputerAlgebra/RamseyTheory/Lisp/GreenTao/Numbers.mac (to be completed).
   </li>
   <li> A different system is
   ComputerAlgebra/RamseyTheory/Lisp/Ramsey/Numbers.mac (which needs update):
   Here there are no transversal numbers. </li>
   <li> The "main" function:
    <ol>
     <li> In general we have the notion of a "parameter tuple" P, and the main
     function N(P) (for example vanderwaerden(P). </li>
     <li> N(P) either computes a natural number, in which case this is the
     Ramsey-type number, or a pair [a,b], where a is a natural number and b
     is a natural number >= a or inf, meaning that the Ramsey-type number is in
     the closed interval from a to b. </li>
     <li> That the underlying number could actually be inf allows the
     possibility, that actually no underlying Ramsey-type theorem holds (while
     still some numbers < inf can be computed). </li>
     <li> If we however know that the number if finite (i.e., an underlying
     Ramsey-type theorem holds), then we use the upper bound inf-1 (to express
     "< inf"; see "Handling lower bounds" below). </li>
     <li> To check that a Maxima-term t is a parameter tuple, the predicate
     N_p(t) is provided which is true is t is a parameter tuple, and false
     otherwise. </li>
     <li> N(P) shall contain all the knowledge we have currently in our
     system. </li>
    </ol>
   </li>
   <li> The "side" functions:
    <ol>
     <li> Mostly, N(P) delegates the computation to "side functions" S(P).
     </li>
     <li> These side-functions typically consider a restricted domain of
     parameter tuples, and also they typically do not consider all the
     possibilities for computing bounds, but they consider only a well-defined
     single case. </li>
     <li> Furthermore, the S actually typically do not take the parameter
     tuple P, but a non-empty list of parameters which more naturally (and
     typically more compactly) encode the class of parameter tuples considered
     by S. </li>
     <li> A function S_a(P) is provided for a side function, with domain all
     parameter tuples, returning [] if P is not in the domain of the parameter
     tuples considered by S, and otherwise a (non-empty) list L such that
     apply(S,L) is applicable. (All possible parameter lists L for S can be
     obtained by S_a(P).) </li>
    </ol>
   </li>
  </ul>


  \todo Handling lower bounds
  <ul>
   <li> Currently we use intervals [a,b], meaning a <= N <= b for the
   corresponding Ramsey-type-number N. </li>
   <li> Instead we could use a < N < b, which for the lower bound would
   correspond to the way they are presented in the literature, and for the
   upper bound would represent the corresponding Ramsey-type-theorem. </li>
   <li> However, that N is finite can also be represented by the upper bound,
   inf-1, using inf-1 < inf, while n < inf-1 for all natural n. </li>
   <li> DONE
   These properties should be put into our test system, since likely
   Maxima doesn't guarantee them? </li>
   <li> In case we don't conjecture the lower bound ">= b", one could use
   the lower bound "(b-1)+1", where b-1 is evaluated, but the "+1" not. </li>
  </ul>


  \todo How to annotate numbers with justifications?
  <ul>
   <li> Once the computations get a bit more involved, the need arises to
   annotate the computed bounds/numbers with justifications (how did the
   system compute them?). </li>
   <li> Perhaps we introduce a global boolean variable "oklib_term_annotation",
   with default-value false, when when activated switches annotations on for
   the various bounds-functions involved. </li>
   <li> Then the old value (either an integer or a pair of integers/inf) is
   replaced by a pair, where the first component is the old value, while
   the second component is the annotation. </li>
   <li> It seems annotations must be performed "ad-hoc" by each of the
   side-functions and by the main function (see above) --- using some general
   scheme very likely it gets unreadable. </li>
   <li> In order to avoid that the functions become hard to read, likely
   they invoke a global switch at the beginning: normally without annotation,
   while when annotation is activated then the code is repeated but now using
   resp. providing the annotations. </li>
   <li> Likely we can not use the Maxima mechanisms, however as an additional
   aid they could be useful:
    <ol>
     <li> How to get the history of a computation? </li>
     <li> At least at Lisp-level one can have a "hidden" annotation (that's
     the point of oklib_load_annotation!) --- how to use this? </li>
    </ol>
   </li>
  </ul>

*/

