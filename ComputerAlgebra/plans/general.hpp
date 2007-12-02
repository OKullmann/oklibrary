// Oliver Kullmann, 29.6.2007 (Swansea)
/* Copyright 2007 Oliver Kullmann
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
   <li> DONE (.max is to be used)
   Maxima-files: ".maxima" or ".mac" ? </li>
  </ul>


  \todo Documentation
  <ul>
   <li> The biggest problem we have at this time is the lack of source code
   documentation methods! </li>
   <li> At least some work-arounds need to be found.
    <ol>
     <li> Maxima : The solution seems to be to start the .mac-files
     with the usual preamble, and then via "\htmlonly" and
     "\endhtmlonly" to surpress the extraction of code-comments
     (the source code is shown verbatim!). </li>
     <li> We should discuss this on the doxygen mailing list:
     Perhaps a dedicated doxygen-command could be introduced? </li>
     <li> Or should one use the verbatim-commands? </li>
    </ol>
   </li>
   <li> First experience and ideas should be developed in the context of
   ComputerAlgebra/Cryptology/plans/Rijndael.hpp. </li>
  </ul>


  \todo %Test system
  <ul>
   <li> Is there some special test system? </li>
   <li> Lisp/Maxima:
    <ol>
     <li> Similar to the C++ test-system, we have generic test functions,
     which take as argument the function to be tested. </li>
     <li> Likely only functions are to be tested. </li>
     <li> So we could just use, as for the C++ test-system, sub-directories
     "tests" and "testobjects", containing the generic test functions and the
     test instantiations, respectively. </li>
     <li> But, due to the simpler character of programming here, we just use
     asserts for conditions (and no recovering of test conditions). </li>
     <li> Ask on the Maxima mailing list, whether they have a system in use. </li>
    </ol>
   </li>
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


  \todo Partial compilation
  <ul>
   <li> Is there compilation? </li>
   <li> Is there partial compilation (using special properties to
   produce more efficient code)? </li>
  </ul>


  \todo Contexts
  <ul>
   <li> Is there the notion of a "context" ("block") ?
  </ul>


  \todo Lisp integration
  <ul>
   <li> The Lisp-dialect is "CLisp" --- are there books? </li>
   <li> How to integrate CLisp with Maxima ? </li>
   <li> Shouldn't we consider our code as Lisp-code, which uses the
   maxima-library ? Perhaps we can discuss this on the Maxima mailing
   list. </li>
  </ul>


  \todo Aldor integration
  <ul>
   <li> Literature on Aldor? </li>
   <li> How to integrate Axiom with Aldor? </li>
   <li> We should only write Aldor-code, using the Maxima-library ---
   how to do this? </li>
  </ul>


  \todo Abstraction : DONE (no abstraction in Lisp/Maxima, full abstraction
  in Aldor/Axiom)
  <ul>
   <li> How to handle abstraction? </li>
   <li> Can we abstractly specify types, functions ? </li>
  </ul>


  \todo General principles : DONE
  <ul>
   <li> Formulate these principles in ComputerAlgebra/docus/general.hpp. DONE </li>
   <li> DONE (the Maxima level is the "fixed" level, without polymorphism
   and abstract concepts, but "flowing with Lisp" and providing simple
   and obvious implementations)
   The computer-algebra-system is also based on generic
   concepts, which mirror the more refined C++ concepts. </li>
   <li> DONE (as said above, at the Maxima level we do not aim
   at generality at the concept level, only at generality which
   can naturally expressed in Maxima; however with Axiom/Aldor
   we seek also conceptual generality)
   These concepts express (typically) the most general
   level, without all the distinctions regarding
   efficiency. </li>
   <li> DONE (yes; we try a 3-level system as discussed below)
   The computer-algebra-system thus also serves as an
   experimental area for the design of concepts in the OKlibrary
   (regarding the "mathematical" concepts, not the programming ones). </li>
   <li> DONE (first level is Maxima/CLisp, "concrete" and disregarding efficiency;
   next level is Axiom/Aldor, where abstract concepts are formulated, and basic
   algorithmic problems are tackled; finally we have the C++ level as the full level,
   where we can also treat creation and destruction of objects, and where algorithms
   can be fully developed and tuned --- however, this does not need to be the case, and
   also in C++ the "C level" when done properly is a good entry)
   Concepts and (generic) algorithm for the OKlibrary are first
   prototyped in the context of the computer-algebra-system (if
   possible). </li>
  </ul>


  \todo Comparisons : DONE (we focus at Maxima and Axiom/Aldor (at least at this time))
  <ul>
   <li> At his time, Maxima and Axiom look most promising to me as general
   purpose systems (for prototyping the SAT algorithms). </li>
   <li> Maxima is a gold old Lisp system.
    <ol>
     <li> Might actually be not too bad for prototyping. </li>
     <li> It seems there is no active develoment anymore(?). </li>
     <li> For the SAT-part this isn't needed, but of course regarding
     combinatorics, graph theory etc., it would be nice to have (much) more. </li>
     <li> Perhaps here the Sage-surface could come to our rescue? </li>
    </ol>
   </li>
   <li> Axiom has strong typing (like Mupad).
    <ol>
     <li> This might be closer to our global world (the C++ development) </li>
     <li> Though, according to the experience with mupad, development takes longer. </li>
     <li> It seems, Aldor is a strong extension, and when considering Axiom,
     then we should do programming in Aldor (also because of the stronger
     syntax); see http://www.aldor.org/index.html. </li>
    </ol>
   </li>
   <li> At least at the beginning, we could develop the basics both in Maxima and Axiom. </li>
  </ul>


  \todo How to use Sage? DONE (we observe Sage, and install newer versions,
  but we concentrate on Maxima and Axiom (at least at this time))
  <ul>
   <li> One gets the impression that Sage does not really support
   using the underlying systems, for example Maxima ? </li>
   <li> So perhaps it would be easier for us (at least at this
   stage) to not use Sage, but install Maxima ourselves? </li>
   <li> How can we customise Sage from within the OKlibrary? </li>
   <li> If we have code for Maxima, then obviously we use
   \verbatim
sage: maxima_console()
   \endverbatim
   So what use is Sage at all here? Isn't it just in the way, making
   it harder (or impossible) to configure Maxima properly? </li>
  </ul>

*/

