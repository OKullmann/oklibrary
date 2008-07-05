// Oliver Kullmann, 29.6.2007 (Swansea)
/* Copyright 2007, 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/plans/general.hpp
  \brief General plans regarding computer algebra (and SAT)


  \todo Module Enumeration
  <ul>
   <li> We should have a module ComputerAlgebra/Combinatorics, which is
   concerned especially about enumerating (standard) combinatorial
   objects. </li>
   <li> Compare Combinatorics/Enumeration/plans/general.hpp and the sub-modules
   Combinatorics/Enumeration/plans/GrayCodes.hpp,
   Combinatorics/Enumeration/plans/Permutations.hpp and
   Combinatorics/Enumeration/plans/Subsets.hpp. </li>
   <li> According to [Stanton, While; Constructive Combinatorics], the
   basic sub-modules would be:
    <ol>
     <li> Permutations; perhaps with submodule Involutions </li>
     <li> Subsets </li>
     <li> Set partitions </li>
     <li> Integer partitions </li>
     <li> Product spaces </li>
     <li> Trees (not as data structures; and in Graphs we do not handle
     the enumerative aspects) </li>
     <li> Tableaux </li>
    </ol>
   </li>
   <li> The appendix of [Stanton, While; Constructive Combinatorics] contains
   enumeration algorithms to start with. </li>
  </ul>


  \todo DONE Transfer completed todos to the docus
  <ul>
   <li> Naming conventions : DONE </li>
   <li> Input checking : DONE </li>
  </ul>


  \todo Naming conventions : DONE
  <ul>
   <li> DONE What about clisp-files? They have the suffix ".lisp". </li>
   <li> What about Maxima demo-files?
    <ol>
     <li> They are just ordinary .mac-files, but placed in demos-directories.
     </li>
     <li> However, in the Maxima-package they are called ".dem", and this we
     should also do. </li>
    </ol>
   </li>
   <li> DONE (.mac is to be used)
   Maxima-files: ".maxima" or ".mac" ? </li>
  </ul>


  \todo DONE (no input checking is performed at the Maxima/lisp level;
  for testing purpose, via predicates all objects can be checked for
  being well-defined, but this is used only in unit-tests)
  Input checking
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


  \todo Dynamical systems
  <ul>
   <li> On the one end we have an irreversible discrete dynamical system,
   that is a pair (X,f), where X is a set and f: X -> X. </li>
   <li> On the other end we have the operation of a monoid on a set. </li>
   <li> Special discrete dynamical system are finite systems (with finite X).
   </li>
   <li> Further specialised are sequential dynamical systems (SDS's), where
   X is the set of total assignments for a finite set V of variables with
   finite domains, and where f is a composition of f_v in some fixed order,
   where f_v: X -> X changes only the x-value and this only in dependence on
   the neighbours of x (and x) in an underlying graph on V. </li>
   <li> Since large X (potentially infinite) are of importance here, we should
   consider "explicit" and "implicit" representations:
    <ol>
     <li> An explicit discrete dynamical system is a pair [X,f], where X
     is a set and f a function with one argument (accepting inputs from X).
     </li>
     <li> An implicit discrete dynamical system is a pair [chi, f], where
     chi is a boolean function, which evaluates to true on the domain of
     the system, and f is a function as before. </li>
     <li> An explicit discrete system [X,f] is converted to an implicit
     system [buildq([X],lambda([x],elementp(x,X))), f] (as usual). </li>
    </ol>
   </li>
   <li> The basic task for discrete dynamical systems is the computation of
   orbits O(x), by O(x) = {f^i(x) : 0 <= i <= K} for K large enough.
    <ol>
     <li> For reversible systems (flagged as such, and with invertible f)
     we have O(x) = {f^i(x) : -K <= i <= K}. </li>
     <li> These computations only require an implicit system. </li>
     <li> For explicit systems also the set of all fixed points can be
     computed. </li>
     <li> K is at most |X|-1. </li>
    </ol>
   </li>
  </ul>


  \todo Documentation : DONE
  <ul>
   <li> Lisp/Maxima : DONE (moved to ComputerAlgebra/plans/Maxima.hpp) </li>
   <li> DONE (moved to Axiom.hpp) Aldor/Axiom ??? </li>
  </ul>


  \todo %Test system : DONE
  <ul>
   <li> Lisp/Maxima : DONE (moved to ComputerAlgebra/plans/Maxima.hpp) </li>
   <li> DONE (moved to Axiom.hpp) Aldor/Axiom ??? </li>
  </ul>

*/

