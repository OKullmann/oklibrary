// Oliver Kullmann, 11.2.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Logic/Computability/plans/TuringMachines.hpp
  \brief Plans on implementations around Turing Machines

  \todo Turing machines: general plans
  <ul>
   <li> First we need formal grammars for the important types of
   Turing machines (there should be out there some code). </li>
   <li> Then we should (directly) simulate Turing machines. </li>
   <li> First implementations, as usual, at the Maxima/Lisp level. </li>
   <li> We define several models, and
    <ol>
     <li> "syntax checking" (does the machine fulfil the requirements, is
     it non-deterministic etc.); </li>
     <li> translations between the models; </li>
     <li> universal machines. </li>
    </ol>
   </li>
  </ul>


  \todo Types of Turing machines
  <ul>
   <li> [Kozen 2006, Theory of Computation], Section 1 defines only 1-tape
   Turing machines, and which are only acceptors. We could have it as a
   very restricted model. Features:
    <ol>
     <li> Arbitrary finite input alphabet A. </li>
     <li> Finite tape alphabet T with A <= T, and two special symbols
     "blank" in T - A and "begin" in T - A (there called "left endmarker").
     </li>
     <li> Three specific states: start, accept, reject, where reject <>
     accept. </li>
     <li> The transition relation delta takes a state and a symbol and
     "returns" a state, a symbol and "L" or "R". Thus the tape cell is
     always  overwritten, and the head never stays. </li>
    </ol>
    There are requirements on the transition relation:
    <ol>
     <li> If ((p, "begin"), (q, s, m)) in delta, then s = "begin" and
     m = R. </li>
     <li> If ((accept, s), (p,s',m)) in delta, then p = accept. </li>
     <li>  If ((reject, s), (p,s',m)) in delta, then p = reject. </li>
    </ol>
    Apparently at the beginning the tape has the begin-symbol and the
    rest is "blank".
   </li>
   <li> [Reischuk 1999, Komplexitaetstheorie, Band I] has in Chapter 1
   a more complete approach, and can be used for other, more general
   models (multi-tapes etc.). </li>
  </ul>

*/

