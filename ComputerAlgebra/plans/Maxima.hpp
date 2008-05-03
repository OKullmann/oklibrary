// Oliver Kullmann, 20.12.2007 (Swansea)
/* Copyright 2007, 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/plans/Maxima.hpp
  \brief General plans regarding the Maxima computer algebra system


  \todo File load and include
  <ul>
   <li> The issue occurs that various maxima modules such as "graphs" take a 
   considerable time to load (~0.5 seconds on a modern machine) and such a load
   occurs in various very basic modules in the library such as 
   Satisfiability/Lisp/BasicOperations.mac which is included in many files. </li> 
   <li> (DONE An errant oklib_load instead of oklib_include caused this)
   It appears that after the last submit of MG loading times nearly
   trippled? </li>
   <li> DONE (inclusions of OKlib-files happens only once, so there is
   no problem here)
   This isn't a problem usually but each new file that then includes 
   BasicOperations.mac then increases the time for oklib_load_all() to run by at
   least that ~0.5 seconds which adds up quite considerably over time. </li>
   <li> Perhaps such modules could only be loaded once? </li>
   <li> So a function very similar to "oklib_include" should be written, which
   only loads the named file once, while not doing any path-administration, but
   just using plain "load". </li>
  </ul>


  \todo Recovering of partial results after long (unsuccessful) computations
  <ul>
   <li> See "all_unsinghitting" in
   ComputerAlgebra/Satisfiability/Lisp/ConflictCombinatorics/HittingClauseSets.mac
   for an example how to pass variable-names as parameters, and how to store
   the elements of the result-set piecewise in this variable. </li>
   <li> And see all_hitting_DP_reductions_def in
   ComputerAlgebra/Satisfiability/Lisp/ConflictCombinatorics/HittingClauseSets.mac
   for a more advanced example how to make make these recovering-variables
   optional; here we have also a state-variable (the permutation count),
   which allows re-starting the computation from an arbitrary point. </li>
   <li> In this way we should rewrite all functions which perform long computations,
   and for which it makes sense to collect partial results. </li>
   <li> Using "oklib --maxima -g", a running computation can be interrupted by
   Ctrl-C, a variable "var" can be displayed by "$var" (using "(displa $var)"
   one gets Maxima-representation), and by "continue" (at
   break-level 1 !) the computation can be continued. </li>
   <li> Does this slow down computations? Some tests seem to indicate that this
   is not the case. </li>
   <li> So perhaps this should be our default? </li>
   <li> If something goes wrong with displaying values, then suddenly
   "continue" doesn't work anymore (but is treated as variable name)?!? </li>
   <li> And also for some other reasons continuation becomes impossible??
   (Then we don't have a "Continuable Error"). </li>
   <li> This looks like a clisp compiler weakness. Perhaps with 2.44.1 this
   is solved? </li>
   <li> Perhaps the problem is that when the execution is inside a sub-function
   then continuation is not possible? </li>
   <li> Anyway, this mechanism doesn't look really reliable yet. </li>
  </ul>


  \todo Debugging
  <ul>
   <li> Again and again it happens that somehow the Lisp-debugger is entered,
   and apparently there is NO ESCAPE (":top" apparently should be the escape,
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


  \todo Handling of demos
  <ul>
   <li> The demos-files are put into demos-subdirectory, and are plain
   .mac-files (intented to be processed). </li>
   <li> How to run the maxima-demos-files?
    <ol>
     <li> Apparently, "batch" is when we want to "run through it", while
     "demo" is when we want to pause after each expression. </li>
    </ol>
   </li>
   <li> How to name the demos-files?
    <ol>
     <li> If the demonstation accompanies a file, then it has the same name. </li>
     <li> Otherwise any appropriate name. </li>
     <li> The suffix ".dem" is mentioned in the Maxima-manual. This or
     ".mac" ?! </li>
    </ol>
   </li>
   <li> How to create pauses?
    <ol>
     <li> On a global level one has the distinction between "oklib_batch"
     and "oklib_demo". </li>
    </ol>
   </li>
   <li> Integrate the demos into the test-system. </li>
   <li> DONE How to print out explanations?
    <ol>
     <li> The problem is that when using "batch" or "demo", where expressions
     are reported before being printed, the explanations, for which we only
     have the print-statement, are printed twice?? </li>
     <li> The solution seems to be to use just the expression
     \verbatim
"Text to be shown"$
     \endverbatim
     </li>
    </ol>
   </li>
   <li> DONE How is it integrated into the general demos-system for the OKlibrary?
   Likely nothing special is done, only we need load-capabilities, likely
   here only for single files. </li>
   <li> DONE (oklib_batch, oklib_demo)
   Extend "oklib_load" to process the maxima-demos? Perhaps better
   a dedicated function. </li>
  </ul>


  \todo New module "Trees" : DONE


  \todo Monitoring
  <ul>
   <li> We introduce a global variable "oklib_monitor",
   which our functions can use, and if set to true then they
   output progress information. </li>
   <li> Additionally we have "oklib_monitor_level":
    <ol>
     <li> The default is level 0, meaning that only basic messages
     are print. </li>
     <li> Greater values are then used at the discretion of the affected
     function. </li>
     <li> "inf" always indicates full output. </li>
    </ol>
   </li>
   <li> This output should happen in a standardised way, so that
   the source of the output is recognisable.
    <ol>
     <li> We use "M[function_name]:" to start the message. </li>
     <li> Perhaps we can offer some general services, like functions
     which print the opening and ending. Something similar to the
     Messages-system for C++ perhaps.
      <ul>
       <li> But we don't consider internationalisation. </li>
       <li> And, of course, no compile-time-issues. </li>
       <li> Is there a general facility to find out the current
       function? Otherwise perhaps we use some standard variable,
       which is then set to the name of the function. </li>
      </ul>
     </li>
    </ol>
   </li>
   <li> The monitoring code uglifies the procedure:
    <ol>
     <li> Could there be a general scheme, that certain variables are
     watched and printed if they changed value, and this happens
     non-intrusively? But we need dedicated text etc., so this doesn't
     seem to hlep.</li>
     <li> Or functions which support monitoring come in two versions?
     This would created too much code-bloat. </li>
     <li> Output should always happen via helper-functions, so the
     interruption is minimal. Standard: "monitorcheck". </li>
     <li> This procedure just inherits all variable etc. via
     dynamic binding. </li>
    </ol>
   </li>
   <li> DONE For the introduction of "oklib_monitor", apparently "define_variable"
   should be used? </li>
   <li> See first examples:
    <ol>
     <li> ComputerAlgebra/Satisfiability/Lisp/Resolution/Basics.mac </li>
     <li> ComputerAlgebra/Satisfiability/Lisp/Backtracking/DLL_solvers.mac </li>
    </ol>
   </li>
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
   <li> A serious problem is that function-parameters are overwritten by
   existing functions with the same name.
    <ol>
     <li> So we reserve names "f", possibly followed by digits: Nobody
     shall define functions with these names. </li>
     <li> For function definitions using function parameters, we use then
     these special names, and also kill the ones used before. </li>
    </ol>
   </li>
  </ul>


  \todo Declaring variables
  <ul>
   <li> Declaring global variables:
    <ol>
     <li> The modes ("types") of "define_variable" are
     \verbatim
number,fixnum,rational,boolean,float,list,any
     \endverbatim
     (for example "inf" is of type "any"). </li>
     <li> Unclear what fixnum means: It can store for example 100000!.
     It appears just to be an integer. Perhaps this is special for
     CLisp? </li>
     <li> It seems we should declare all of our global variables
     in this way. </li>
     <li> It can be used also inside functions, after the block-
     declaration (or elsewhere). </li>
     <li> However, inside functions mode_declare is more appropriate,
     since it leaves out the default-value. </li>
    </ol>
   </li>
   <li> See "Variables" in
   ComputerAlgebra/Satisfiability/Lisp/ClauseSets/plans/Generators.hpp. </li>
  </ul>


  \todo Function application
  <ul>
   <li> What is the difference between "apply" and "xreduce" ?
    <ul>
     <li> When the arity of the function being applied isn't known, for 
     instance when the function is a "noun" or isn't defined at this point,
     "apply" continues to treat the function as an n-ary function where n is
     the size of the list argument, whereas "xreduce" treats the function as a
     binary function.
     \verbatim
(%i11) apply(nounify(union), [a,b,c,d]);
Evaluation took 0.00 seconds (0.00 elapsed) using 416 bytes.
(%o11)                                   union(a, b, c, d)
(%i12) xreduce(nounify(union), [a,b,c,d]);
Evaluation took 0.00 seconds (0.00 elapsed) using 696 bytes.
(%o12)                            union(union(union(a, b), c), d)
     \endverbatim
     </li>
    </ul>
   </li>
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
     <li> And there are error messages? </li>
     <li> And once we used compile(all), we get messages 
     "Illegal `remvalue' attempt" ?? </li>
     <li> "define_variable" can be used to get rid off some warnings, but since
     the return types are so restricted, it only applies for a few cases. </li>
     <li> Furthermore there is "translate(all)" ??? What's the difference? </li>
     <li> The output looks rather similar, though there are no error messages. </li>
     <li> For example "all_def34n : all_unsinghitting(3,4,'all_def34);"
     stops working after "compile(all)" or "translate(all)", so perhaps
     we can forget all that here? </li>
     <li> DONE (apparently with Maxima 5.15.0 it works again)
     Now it stopped working at all, and so we need to investigate the
     failures. </li>
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
     
     
  \todo Collaboration with the Maxima community
  <ul>
   <li> On the plans for the "Google Summer of code" at
   http://maxima.sourceforge.net/wiki/index.php/Design%20Notes
   there are quite some overlaps with the OKlibrary (especially
   regarding "Boolean algebra and logic"). </li>
   <li> At some point we should contact "the Maxima community",
   and discuss possibilities for collaborations:
    <ol>
     <li> The OKlibrary uses the Maxima-level only for
     "procedural specifications". </li>
     <li> So the Maxima community can use this as a map for the whole
     field, and produce some specific more efficient implementations at
     the Lisp level. </li>
     <li> Of course, also input on the use of Maxima in the OKlibrary would
     be welcome. </li>
     <li> And doxygen support for Maxima would be great. </li>
    </ol>
   </li>
   <li> A "Logic Algebra" (title of the email) module has been submitted to the
   maxima mailing list for consideration for inclusion in share/contrib and is 
   viewable at http://beshenov.ru/maxima/logic/ . Perhaps some collaboration
   could initially occur here? </li>
  </ul>


   \todo Document simplification of expressions
   <ul>
    <li> Apparently "is(a = b)" only applies very basic simplification
    rules, for example "is(4+5=9)" is true, while for "equivalence" one
    needs to use "is(equal(a,b))":
    \verbatim
is(equal(log(4)/log(2),2));
true
is(log(4)/log(2) = 2);
false
    \endverbatim
    </li>
   </ul>


   \todo New supermodule NumberTheory
   <ul>
    <li> See "fld" in
    ComputerAlgebra/Satisfiability/Lisp/MinimalUnsatisfiability/Basics.mac </li>
   </ul>

*/

