// Oliver Kullmann, 20.12.2007 (Swansea)
/* Copyright 2007, 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/plans/Maxima.hpp
  \brief General plans regarding the Maxima computer algebra system


  \todo How to eliminate the annotation of lists
  <ul>
   <li> Every list created by some function coming from a file
   is internally stored with an annotation including the complete path
   for the file! </li>
   <li> Especially when saving results, this is a huge waste. </li>
   <li> Files like "all_n6" currently used which have to store sessions
   consist 30% of these filenames. Likely one can have easily
   more dramatic examples. </li>
   <li> Experiment with dll_simplest_st:
   \verbatim
T65 : dll_simplest_st_trivial2(weak_php(6,5))$
Evaluation took 118.5080 seconds (130.9962 elapsed) using 352.670 MB.
save("T65",T65);
Evaluation took 4.9283 seconds (5.2729 elapsed) using 99.510 MB.

> more T65
(in-package :maxima)
(DSKSETQ |$t65|
 '((MLIST SIMP
    (51
     "/home/kullmann/csoliver/SAT-Algorithmen/OKplatform/OKsystem/Transitional/ComputerAlgebra/Satisfiability/Lisp/Backtracking/DLL_solvers.mac"
     SRC $DLL_SIMPLEST_ST 48))
   ((%PHP SIMP) 1 1)
   ((MLIST SIMP
     (51
      "/home/kullmann/csoliver/SAT-Algorithmen/OKplatform/OKsystem/Transitional/ComputerAlgebra/Satisfiability/Lisp/Backtracking/DLL_solvers.mac"
      SRC $DLL_SIMPLEST_ST 48))
    ((%PHP SIMP) 1 2)
    ((MLIST SIMP
      (51
       "/home/kullmann/csoliver/SAT-Algorithmen/OKplatform/OKsystem/Transitional/ComputerAlgebra/Satisfiability/Lisp/Backtracking/DLL_solvers.mac"
       SRC $DLL_SIMPLEST_ST 48))

> ls -la T65
-rw-r--r--   1 kullmann users  4440379 2008-05-28 15:42 T65
> cat T65 | sed -e "/\/home/,+1 d" > T65r
> ls -la T65r
-rw-r--r--   1 kullmann users  1362063 2008-05-28 16:45 T65r
   \endverbatim
   Now entering copies of dll_simplest_st and dll_simplest_st_trivial2
   on the command line, with suffixes r:
   \verbatim
rT65 : rdll_simplest_st_trivial2(weak_php(6,5))$
Evaluation took 112.3929 seconds (129.0938 elapsed) using 352.558 MB.
is(rT65 = T65);
true
save("rT65",rT65);

> ls -la rT65
-rw-r--r--   1 kullmann users   605184 2008-05-28 16:51 rT65
   \endverbatim
   One sees that the "anonymous" version "rdll_simplest_st_trivial2",
   not coming from a file, needs only roughly 10% of the space
   of the file-version! </li>
   <li> This seems to be connected with the "lisp debug mode".
    <ol>
     <li> This debugger is useless anyway --- how to get rid off it?!? </li>
     <li> On the other hand, debugmode by default is false; so a different
     kind of debug mode is meant here. </li>
     <li> The main user of the annotation-functionality seems to be the
     error-function. </li>
    </ol>
   </li>
   <li> Perhaps it is a property of "load" ?
    <ol>
     <li> Yes, this seems to be the case. </li>
     <li> In src/mload.lisp the function "load(filename)" is defined. </li>
     <li> This function calls the parser "mread" defined in
     src/nparse.lisp. </li>
     <li> The parser mread calls the function "add-lineinfo(lis)" defined
     in src/nparse.lisp (at the end of the file). </li>
     <li> add-lineinfo adds then the filename and the linenumber. </li>
     <li> Defining add-lineinfo as identity via
     \verbatim
(defun add-lineinfo (lis) (lis))
     \endverbatim
     (thanks to the Maxima mailing list) removes the annotation, but also
     removes the file-information from the error-output (this we needed for 
     "assert"!). </li>
    </ol>
   </li>
   <li> So it seems that at file-load-time, when function definitions are
   parsed, that then the code is instrumented with the annotation-ability 
   (or not). </li>
   <li> Thus it seems that once the function is parsed and made available,
   its behaviour is fixed, and cannot be changed anymore. </li>
   <li> So it seems we need an alternative definition of "load" which
   invokes only the trivial add-lineinfo.
    <ol>
     <li> One possibility to achieve this would be to redefine add-lineinfo,
     using a boolean switch for the two possible definitions. </li>
     <li> The standard "load" then sets this switch to annotation-behaviour. 
     </li>
     <li> While the new "nload" sets this switch to no-annotation-behaviour. 
     </li>
     <li> This looks like the simplest way. </li>
    </ol>
   </li>
   <li> So we need to refine the "oklib_load" functionality:
    <ol>
     <li> The normal "load", "include" etc. does not perform the annotation. 
     </li>
      <li> While "debug" versions do perform the annotation. </li>
    </ol>
   </li>
  </ul>


  \todo File load and include
  <ul>
   <li> The issue occurs that various maxima modules such as "graphs" take a 
   considerable time to load (~0.5 seconds on a modern machine), and such a
   load occurs in various very basic modules in the library such as 
   Satisfiability/Lisp/BasicOperations.mac which is included in many files.
   </li> 
   <li> The cure is to replace all instances of "load" with a function
   "oklib_include_basic" which mimics oklib_include but without appending the
   OKSystem path (ie allowing the same single include behaviour as oklib_include
   provides for maxima modules). This seems to reduce the elapsed time for a 
   call to "oklib_load_all" by a factor of 2 (7 seconds to 3.3). Such a
   replacement was done with something like the following shell code 
   \verbatim
find . -type f | grep -v "maxima-init.mac" | xargs perl -pi -e 's/((?<![a-zA-Z0-9_\-])load ?\(/oklib_include_basic\(/g;'
   \endverbatim
   OK: What is the definition of "oklib_include_basic"?
   I don't like the name  "oklib_include_basic" so much; perhaps
   "oklib_plain_include"? And then likely we should also have "plain"
   versions of the other 3 functions.
   MG : Well the only thing needed seems to be to remove "full_name" from the 
   current functions, for example 
   \verbatim
   oklib_include(name) := block(
    if oklib_loaded_files[name] # oklib_load_round then
     (oklib_loaded_files[name] : oklib_load_round, load(name))
   );
   \endverbatim
   </li>
   <li> See "How to eliminate the annotation of lists" above! 
   So the issue becomes somewhat more complex. </li>
   <li> (DONE An errant oklib_load instead of oklib_include caused this)
   It appears that after the last submit of MG loading times nearly
   trippled? </li>
   <li> DONE (inclusions of OKlib-files happens only once, so there is
   no problem here)
   This isn't a problem usually but each new file that then includes 
   BasicOperations.mac then increases the time for oklib_load_all() to run by at
   least that ~0.5 seconds which adds up quite considerably over time. </li>
  </ul>


  \todo General design: Lists *here* are more fundamental than sets
  <ul>
   <li> See "Clause-lists instead of clause-sets" in
   ComputerAlgebra/Satisfiability/Lisp/plans/general.hpp for the details
   regarding satisfiability. </li>
   <li> It seems that instead of clause-sets we should use clause-lists
   as the fundamental objects. </li>
   <li> In the mathematical cosmos there is no creation and destruction,
   thus no order, however in the computational cosmos there is before
   and after, construction and destruction. </li>
   <li> In this way we obtain control over enumeration aspects.
    <ol>
     <li> Typically the enumeration is controlled by ordering the
     elements of certain sets. </li>
     <li> Yet this done by applying listify, thus without control. </li>
    </ol>
    Now, when enumerating all possibilities in some function:
    <ol>
     <li> Either the given orders are used. </li>
     <li> Or random_permutation is applied. </li>
     <li> Perhaps we have a global flag "oklib_randomise_order", which
     controls this behaviour. </li>
     <li> In this way we have a simple way of random sampling. </li>
    </ol>
   </li>
   <li> But clauses should stay as sets. </li>
   <li> A formal clause-list has then also a list of variables (without
   repetition). </li>
   <li> An l-hypergraph is then also given as a list of hyperedges, together
   with a list of vertices (the latter without repetition). </li>
   <li> But the hyperedges still are sets. </li>
   <li> In the same vein, l-graphs are given by lists of vertices and lists
   of edges, the former without repetition, the latter with sets as
   elements. </li>
   <li> So it seems that we should rewrite all of of the Maxima-functions. </li>
   <li> This combined with the general clean-up. </li>
   <li> The clause-sets etc. are still available, via conversions. </li>
   <li> What about combinatorial matrices? Their point is, after all,
   the order-*independence* ?!
    <ol>
     <li> So perhaps they stay as they are, using index-sets? </li>
     <li> However, there are the same issues regarding enumerations. </li>
     <li> So we should have also "l-combinatorial matrices", where
     indices are given by 2 lists. </li>
    </ol>
   </li>
   <li> Perhaps instead of using prefices like "l" (for "list") we could
   use the adjective "ordered" ?
    <ol>
     <li> Speaking of "ordered clause-sets", "ordered hypergraphs"
     etc. </li>
     <li> However then the aspect, that for examples the same clause
     can occur multiple times, is not expressed? </li>
     <li> Or does an "ordered clause-set" not have multiple occurrences
     of the same clause, while an "l-clause-set" may have? </li>
    </ol>
   </li>
  </ul>


  \todo Plan the redesign
  <ul>
   <li> Making lists fundamental, and introducing new and consistent naming
   conventions. </li>
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
   break-level 1 !) the computation can be continued.
    <ol>
     <li> Does this slow down computations? Some tests seem to indicate that
     this is not the case. </li>
     <li> So perhaps this should be our default? </li>
     <li> If something goes wrong with displaying values, then suddenly
     "continue" doesn't work anymore (but is treated as variable name)?!? </li>
     <li> And also for some other reasons continuation becomes impossible??
     (Then we don't have a "Continuable Error"). </li>
     <li> This looks like a clisp compiler weakness. Perhaps with 2.44.1 this
     is solved? </li>
     <li> Perhaps the problem is that when the execution is inside a
     sub-function then continuation is not possible? </li>
     <li> Anyway, this mechanism doesn't look really reliable yet. </li>
    </ol>
   </li>
   <li> Using "oklib_save()" in a functions stores the whole state
   of Maxima to a session file, after oklib_storage_interval
   minutes have passed, given that oklib_store is true.
    <ol>
     <li> This should be applied throughout. </li>
    </ol>
   </li>
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
     <li> And apparently "float" = "any" ?? </li>
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
   <li> Shallow copy of list arguments for functions:
    <ol>
     <li> Give examples for this behaviour. </li>
     <li> This effects especially clause-sets (which as sets are
     copied) and formal clause-sets (which as lists are only shallowly
     copied)! </li>
    </ol>
   </li>
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
     <li> Package "graphs" establishes the following DEPRECATED interface to
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
     <li> See ComputerAlgebra/DataStructures/Lisp/HashMaps.mac for interfaces.
      <ol>
       <li> The NEW interface (which removes the problem associated with
       having lists as keys in the hash map) consists of the functions
        <ul>
         <li> sm2hm for creating hash-maps (from set-maps) </li>
         <li> set_hm for seting a single value, and compose_hm_sm for
         setting the values of a whole set-map </li>
         <li> ev_hm for evaluating the hash-map at a single argument, and hm2sm
         for creating a whole set-map out of it. </li>
        </ul>
       </li>
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
   <li> Call by reference:
    <ol>
     <li> Pass referency to x by "f('x). </li>
     <li> Inside f, for assignments to x use "x :: value". </li>
     <li> And on the right-hand-side use "ev(x)". </li>
    </ol>
   </li>
   <li> Plotting of functions:
    <ol>
     <li> Options are additional parameters to the plot-function, of
     the form "[option,value]". </li>
     <li> Plotting to a terminal window of x -> x*exp(x), with explicit
     specification of the y-label:
     \verbatim
plot2d(exp(x)*x,[x,-7,1],[ylabel,"x * exp(x)"],[gnuplot_term, x11]);
     \endverbatim
     With newer gnuplot-versions the specification of the terminal
     type shouldn't be needed. </li>
     <li> Creating a ps-file via:
\verbatim
plot2d(exp(x)*x,[x,-7,1],[ylabel,"x * exp(x)"],
  [gnuplot_term,postscript],[gnuplot_out_file,"xexpx.ps"]);
\endverbatim
     With newer gnuplot-versions the terminal-type specification "ps"
     should also work. </li>
     <li> If the output-file is not specified, then a file "maxplot.ps"
     is created in the home-directory of the user. </li>
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

