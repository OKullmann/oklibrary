// Oliver Kullmann, 29.6.2007 (Swansea)
/* Copyright 2007, 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/plans/general.hpp
  \brief General plans regarding computer algebra (and SAT)


  \todo Naming conventions
  <ul>
   <li> What about clisp-files? </li>
   <li> How to postfix Axiom-files? </li>
   <li> How to postfix Aldor-files? </li>
   <li> DONE (.mac is to be used)
   Maxima-files: ".maxima" or ".mac" ? </li>
  </ul>


  \todo Documentation
  <ul>
   <li> Lisp/Maxima : DONE (moved to ComputerAlgebra/plans/Maxima.hpp) </li>
   <li> Aldor/Axiom ??? </li>
  </ul>


  \todo %Test system
  <ul>
   <li> Lisp/Maxima : DONE (moved to ComputerAlgebra/plans/Maxima.hpp) </li>
   <li> Aldor/Axiom ??? </li>
  </ul>


  \todo Input checking
  <ul>
   <li> How to handle input checking/type checking? </li>
   <li> Nothing at the Lisp/Maxima-level. </li>
   <li> And at the Aldor/Axiom level type-checking is performed by the language. </li>
   <li> Remains the question what to do with special conditions on the input?
    <ol>
     <li> For the C++ programming, this is relatively clear: Full checking of "real" input,
     while preconditions of "internal" inputs are checked by assertions. </li>
     <li> One could do the same here, however, due to the more interactive character,
     every function might be exposed to "real" input. </li>
     <li> Perhaps a global variable just says "all checks on" or "all checks off" ?? </li>
     <li> Ask on the Maxima mailing list. </li>
    </ol>
   </li>
  </ul>


  \todo Contexts
  <ul>
   <li> Is there the notion of a "context" ("block") ?
  </ul>


  \todo Aldor integration
  <ul>
   <li> Literature on Aldor? </li>
   <li> How to integrate Axiom with Aldor? </li>
   <li> We should only write Aldor-code, using the Axiom-library ---
   how to do this? </li>
  </ul>


  \todo Frobenius problem
  <ul>
   <li> http://en.wikipedia.org/wiki/Coin_problem </li>
   <li> What is the precise computational complexity ?
    <ol>
     <li> How to measure the input length?? Unary or binary notation??? </li>
     <li> What does the "NP-hardness" mean? Upper bounds??
      <ol>
       <li> Consider the decision version "g(a_1,...,a_n) >= K ?". </li>
       <li> It seems that measuring the a_i in unary, the problem is solvable
       in polynomial time, while for a_i taken binary the problem seems to
       be NP-complete. </li>
      </ol>
     </li>
     <li> Parameterised complexity:
      <ol>
       <li> A natural parameter is the dimension n. (Seems to be poly-time
       for fixed n; FPT??) </li>
       <li> Another is the bound K. </li>
      </ol>
     </li>
    </ol>
   </li>
   <li> Direct algorithms:
    <ol>
     <li> "Heap and Lynn method", reducing the problem in a direct way
     to the index of primitivity, should be implemented. (While
     the index of primitivity has a trivial direct solution.) </li>
     <li> See ComputerAlgebra/Graphs/Lisp/plans/general.hpp,
     "Primitive directed graphs etc.". </li>
    </ol>
   </li>
  </ul>

*/

