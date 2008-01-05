// Oliver Kullmann, 20.12.2007 (Swansea)
/* Copyright 2007, 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/plans/Maxima.hpp
  \brief General plans regarding the Maxima computer algebra system

  
  \todo Documentation
  <ul>
   <li> A work-around to obtain doxygen-documentation:
    <ol>
     <li> The solution seems to be to start the .mac-files
     with the usual preamble, and then via "\htmlonly" and
     "\endhtmlonly" to surpress the extraction of code-comments
     (the source code is shown verbatim!). </li>
     <li> We should discuss this on the doxygen mailing list:
     Perhaps a dedicated doxygen-command could be introduced? </li>
     <li> Or should one use the verbatim-commands? </li>
    </ol>
   </li>
   <li> How to create function-documentation inside Maxima (usable via
   "? function-name") ? </li>
  </ul>


  \todo %Test system
  <ul>
   <li> Similar to the C++ test-system, we have generic test functions,
   which take as argument the function to be tested. </li>
   <li> Likely only functions are to be tested. </li>
   <li> So we could just use, as for the C++ test-system, sub-directories
   "tests" and "testobjects", containing the generic test functions and the
   test instantiations (i.e., expressions evaluating the test function on
   the function to be tested), respectively.
    <ol>
     <li> Execution of the tests just means loading the testobject-files.
    </ol>
   </li>
   <li> Due to the simpler character of programming here, we just use
   asserts for conditions and print error-messages.
    <ol>
     <li> But desirable if we establish a system a bit similar to the C++
     system! This strengthens the design of the test-system. </li>
     <li> Every test-function returns just true in case of success,
     while otherwise false is returned --- though the return value
     likely is not much of use, but the real output is the side
     effect that an error-message is printed, using "error" (this
     should cause abortion). </li>
     <li> The "backtrace()"-call is useful here: In case of
     an error not just the error message is printed, but also the trace of
     function calls. </li>
     <li> Perhaps we create a macro for this error-output (similar
     to the C++-macro). </li>
     <li> There is a global variable for the test-level. </li>
    </ol>
   </li>
   <li> "oklib check" is also responsible for the maxima-tests, via a sub-goal
   (so that also only the maxima-tests can be involved). </li>
   <li> Ask on the Maxima mailing list, whether they have a system in use. </li>
  </ul>


  \todo Handling of demos
  <ul>
   <li> How to call the maxima-demos-files? </li>
   <li> How is it integrated into the general demos-system for the OKlibrary?
   Perhaps it is just the incarnation of the general system in the context
   of Maxima/Lisp. </li>
   <li> Extend "oklib_load" to process the maxima-demos. </li>
  </ul>


  \todo Monitoring
  <ul>
   <li> Perhaps we introduce a global variable "oklib_monitor",
   which our functions can use, and if set to true then they
   output progress information. </li>
   <li> This output should happen in a standardised way, so that
   the source of the output is recognisable. </li>
   <li> On the other hand, in this way we uglify the code ?!? </li>
   <li> Could there be a general scheme, that certain variables are
   watched and printed if they changed value, and this happens
   non-intrusive? </li>
   <li> Or functions which support monitoring come in two versions? </li>
   <li> For the introduction of "oklib_monitor", apparently "define_variable"
   should be used? </li>
   <li> See ComputerAlgebra/Satisfiability/Lisp/ClauseSets/Resolution.mac for
   first examples. </li>
  </ul>


  \todo Timing
  <ul>
   <li> Is there a timing command ? </li>
   <li> "time (%o1, %o2, %o3, ...)" returns the time it took to
   compute the respective outputs. </li>
   <li> With "showtime: true " Maxima outputs the timing result
   after each computation. </li>
  </ul>


  \todo Local variables
  <ul>
   <li> How to make sure, that variables are local? </li>
   <li> Is "local" needed? For what is it good for? </li>
   <li> It seems not necessary to protect the parameter, or? </li>
   <li> Can everything done with "block" ? </li>
   <li> How to obtain information about symbols (so that for example clashes
   can be avoided) ? </li>
  </ul>


  \todo Function application
  <ul>
   <li> What is the difference between "apply" and "xreduce" ? </li>
  </ul>


  \todo List operations
  <ul>
   <li> A frequent operation is to add an element x to a list L
   *in-place*. </li>
   <li> Apparently the most efficient operations seems to be
   <code>L : cons(x,L)</code> resp. <code>L : endcons(x,L)</code>,
   where both operations seem to take place not in-place, but involve
   copying and re-assignment, making it very inefficient?? </li>
   <li> In general we want to avoid any efficiency-considerations, however
   having in-place modifications of lists would also increase ease of
   use. </li>
  </ul>


  \todo Document important programming techniques:
  <ul>
   <li> How to handle local variables (see above). </li>
   <li> All different types of loops (see the existing code).
   Especially nested loops ("create_list"). </li>
   <li> How to create lambda-terms (see the existing code). </li>
   <li> Function-application (also "map" etc.; see above). </li>
   <li> Hash tables
    <ol>
     <li> Package "graphs" establishes the following interface to
     lisp-hash-tables. </li>
     <li> First use "load(graphs);" </li>
     <li> Via "h : hash_table()" a new hash table is created. </li>
     <li> Via "set_hash("abc", h, 78)" we set h("abc") = 78. </li>
     <li> Via "get_hash("abc", h)" we get (h("abc") if defined, and
     false otherwise. </li>
     <li> And via "hash_table_data(h)" we get the list of all assignments
     in h. </li>
    </ol>
   </li>
  </ul>


  \todo Partial compilation:
  <ul>
   <li> "compfile" writes Lisp-code into files? </li>
   <li> It seems that we should use "compile(all)" from time to time, and also
   with "oklib".
    <ol>
     <li> There are warnings about undefined global variables, but since they
     involve even for example "inf" it seems we can ignore these warnings? </li>
    </ol>
   </li>
  </ul>


  \todo Lisp integration
  <ul>
   <li> The Lisp-dialect is "CLisp" --- are there books? </li>
   <li> How to integrate CLisp with Maxima ? </li>
   <li> Shouldn't we consider our code as Lisp-code, which uses the
   maxima-library ? Perhaps we can discuss this on the Maxima mailing
   list. </li>
  </ul>

*/

