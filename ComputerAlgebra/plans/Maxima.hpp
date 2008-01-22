// Oliver Kullmann, 20.12.2007 (Swansea)
/* Copyright 2007, 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/plans/Maxima.hpp
  \brief General plans regarding the Maxima computer algebra system


  \todo Debugging
  <ul>
   <li> Again and again it happens that somehow the Lisp-debugger is entered,
   and apparently there is NO ESCAPTE (":top" apparently should be the escape,
   but doesn't work). </li>
   <li> How to disable the debugger ?? </li>
  </ul>
  
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
   <li> This needs to be established now! </li>
   <li> Ask on the Maxima mailing list, whether they have a system in use. </li>
   <li> Similar to the C++ test-system, we have generic test functions,
   which take as argument the function to be tested. </li>
   <li> Likely only functions are to be tested. </li>
   <li> So we could just use, as for the C++ test-system, sub-directories
   "tests" and "testobjects", containing the generic test functions and the
   test instantiations (i.e., expressions evaluating the test function on
   the function to be tested), respectively.
    <ol>
     <li> Execution of the tests just means loading the testobjects-files.
     <li> Or perhaps better, like with the C++ system, in the testobjects-files
     one finds instructions for loading the "testobjects" into a global list
     (provided via dynamic binding when running the tests).
      <ol>
       <li> These "testobjects" perhaps are just the respective function calls,
       unevaluated, while executing the tests means evaluating these terms. </li>
       <li> So we need one function "install_testokl(t)", which stores the term
       t, unevaluated, on a global list "testobjects_testokl". </li>
      </ol>
     </li>
     <li> The files in the "tests"-directories get loaded with oklib_load_all(),
     but not the testobjects-files. </li>
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
     <li> Is it possible to provide information about the file etc. where
     the error-message was issued? Seems not to be possible. So perhaps
     some global variables are set, and in case of an error a maxima session
     is opened? For that we need to evaluate each term with "errcatch", and
     printing actively the error-message with "errormsg()".
      <ul>
       <li> With "errcatch(t,true)" the testterm is evaluated, and true is
       returned if no error was found, and [] otherwise. </li>
       <li> Via "errormsg()" then the error message is printed, and also
       the term t should be displayed. </li>
       <li> For this to be visible it is perhaps needed that an interactive
       session is started. </li>
       <li> So we want just batch-processing without output if no error occurs.
       </li>
       <li> Seems difficult; perhaps the error-output is stored in a file? </li>
       <li> This would then be the error-output and the testobject-term. </li>
       <li> It seems not possible to redirect the output of the
       backtrace-function? </li>
       <li> Perhaps we use "load" first to load the testobject-files (without
       output), and then via "batch" the testobjects are processed, without
       (much) output in case of no error, while we have all the above
       error-information in case of an error. </li>
       <li> We issue then an error inside the batch-file, and so the make-process
       notices the error and halts. </li>
       <li> If an error occurred, perhaps with "trace(all)" everything is traced,
       and the error term is re-evaluated? "trace" seems to be more informative
       than "backtrace". </li>
      </ul>
     </li>
     <li> There is a global variable for the test-level. </li>
     <li> Since we don't have namespaces, we need naming-conventions.
     Perhaps "testokl_" as generic prefix. </li>
     <li> Each test-function has one argument, the function to be tested. </li>
    </ol>
   </li>
   <li> "oklib check" is also responsible for the maxima-tests, via a sub-goal
   (so that also only the maxima-tests can be involved).
    <ol>
     <li> After loading all testobject-files, a function "run_testokl" is called
     which evaluates the terms in "testobjects_testokl". </li>
     <li> A complication arises for functions to be tested which require
     special contexts. Best to avoid this. However if needed, then the
     testobject should just also contain this context. </li>
     <li> "Contexts" seem just to refer to "facts" etc. It could be that
     a special environment is needed, with special variables and functions
     defined; but again this should be provided by the testobject. </li>
     <li> As usual all testobjects for the calling directory level are
     executed. </li>
     <li> Likely we should not provide a mechanism for running only tests when
     needed (too complicated). Just run always all respective tests (and "basic"
     tests really should run quickly). </li>
    </ol>
   </li>
  </ul>


  \todo Handling of demos
  <ul>
   <li> The demos-files are put into demos-subdirectory, and are plain
   .mac-files (intented to be processed). </li>
   <li> How to call the maxima-demos-files? </li>
   <li> How to print out explanations? </li>
   <li> How to create pauses? </li>
   <li> How is it integrated into the general demos-system for the OKlibrary?
   Likely nothing special is done, only we need load-capabilities, likely
   here only for single files. </li>
   <li> Extend "oklib_load" to process the maxima-demos? Perhaps better
   a dedicated function. </li>
  </ul>


  \todo New module "Trees" : DONE


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
   <li> How to handle local variables (see above).
    <ol>
     <li> A source of errors regarding the block-expression is that
     returns inside it just leave *this* block --- so one has to
     be careful by introducing blocks (since they change the semantics)! </li>
    </ol>
   </li>
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
     in h.
      <ol>
       <li> For an assignment A ("x -> y") we obtain the argument by
       "part(A,1)" and the value by "part(A,2)". </li>
      </ol>
     </li>
    </ol>
   </li>
   <li> Dynamic binding: Important to understand this, since nowadays
   static binding is most common. </li>
   <li> Arbitrary precision:
    <ol>
     <li> For a local computation Comp with d decimal digits use
     \verbatim
block([fpprec : fpprec], fpprec : d, Comp)
     \endverbatim
     </li>
     <li> Numbers used in Comp must be converted to bfloat, since otherwise
     exact computations are used, while number-literals like
     "1.0" are by default interpreted as float. </li>
     <li> However numerical procedures often enforce usage of type
     float (for example "newton"), and thus can't be used (succesfully)
     in Comp. </li>
     <li> It seems that an expression involving some bfloat-argument
     is always evaluated in bfloat-arithmetic. </li>
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

