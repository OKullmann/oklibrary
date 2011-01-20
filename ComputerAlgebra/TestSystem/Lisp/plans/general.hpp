// Oliver Kullmann, 26.1.2008 (Swansea)
/* Copyright 2008, 2009, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ComputerAlgebra/TestSystem/Lisp/plans/general.hpp
  \brief General plans for the Maxima test system


  \bug Error not detected
  <ul>
   <li> With ComputerAlgebra/Cryptology/Lisp/testobjects/Conversions.mac
   and with Maxima 5.19.0 (Ecl) we get the error (this is just an example,
   the functions referred to do no longer exist):
   \verbatim
(%i49) okltest_binl2dnf_l(binl2dnf_l)
Maxima encountered a Lisp error:

 FRAME-STACK overflow at size 2304. Stack can probably be resized.

Automatically continuing.
To reenable the Lisp debugger set *debugger-hook* to nil.
Evaluation took 0.1560 seconds (0.1660 elapsed)
   \endverbatim
   however this error is not detected by the buildsystem, but the system
   continues. </li>
   <li> On the other hand, with CLisp we get
   \verbatim

*** - Program stack overflow. RESET

[../src/eval.d:573] reset() found no driver frame (sp=0xff8c9400-0xff8c2d80)
Exiting on signal 6
make: *** [run_maxima] Aborted
   \endverbatim
   and so here the buildsystem detects the error. </li>
  </ul>


  \todo How to calibrate floating-point efficiency
  <ul>
   <li> Examples, contrasting Maxima version 5.21.1 with 5.22.1:
   \verbatim

testf1(n) := float(apply("+",create_list(log(i)*(-1)^i,i,1,n)));
testf1c(n):=block([fpprec:30],bfloat(apply("+",create_list(log(i)*(-1)^i,i,1,n))));
testf1f(n) := apply("+",create_list(float(log(i))*(-1)^i,i,1,n));

21:

(%i24) testf1(10000);
Evaluation took 79.0520 seconds (125.7410 elapsed)
(%o24) 4.830986538632788
(%i25) testf1c(10000);
Evaluation took 75.6320 seconds (141.3070 elapsed)
(%o25) 4.8309865386327771337329138576b0
(%i31) testf1f(10000);
Evaluation took 0.3380 seconds (0.4770 elapsed)
(%o31) 4.830986538632788

(%i26) testf1(20000);
Evaluation took 301.3270 seconds (546.0920 elapsed)
(%o26) 5.177547628912792
(%i27) testf1c(20000);
Evaluation took 326.1730 seconds (542.8570 elapsed)
(%o27) 5.17754762891278624677437887665b0
(%i32) testf1f(20000);
Evaluation took 0.6810 seconds (1.0060 elapsed)
(%o32) 5.177547628912792

(%i28) testf1(40000);
Evaluation took 1244.4280 seconds (2258.0720 elapsed)
(%o28) 5.524114969192852
(%i29) testf1c(40000);
Evaluation took 1256.1750 seconds (2077.7460 elapsed)
(%o29) 5.52411496919276345877464646714b0
(%i33) testf1f(40000);
Evaluation took 1.3840 seconds (2.1210 elapsed)
(%o33) 5.524114969192852

22:

(%i19) testf1(10000);
Evaluation took 68.2900 seconds (123.3390 elapsed)
(%o19) 4.83098653863282
(%i20) testf1c(10000);
Evaluation took 76.8890 seconds (123.0450 elapsed)
(%o20) 4.8309865386327771337329138576b0
(%i26) testf1f(10000);
Evaluation took 0.3280 seconds (0.6130 elapsed)
(%o26) 4.83098653863282

(%i21) testf1(20000);
Evaluation took 289.1520 seconds (468.5160 elapsed)
(%o21) 5.17754762891287
(%i22) testf1c(20000);
Evaluation took 286.7340 seconds (559.2420 elapsed)
(%o22) 5.17754762891278624677437887665b0
(%i27) testf1f(20000);
Evaluation took 0.7100 seconds (1.2810 elapsed)
(%o27) 5.17754762891287

(%i23) testf1(40000);
Evaluation took 1175.0270 seconds (2232.6280 elapsed)
(%o23) 5.524114969192786
(%i24) testf1c(40000);
Evaluation took 1206.8040 seconds (1911.2210 elapsed)
(%o24) 5.52411496919276345877464646714b0
(%i28) testf1f(40000);
Evaluation took 1.3790 seconds (2.1250 elapsed)
(%o28) 5.524114969192786

   \endverbatim
   There are differences, but no clear picture. </li>
   <li> For comparison, a C++ implementation:
   \verbatim
// File SumLog.cpp
#include <iostream>
#include <sstream>
#include <cmath>
#include <iomanip>
int main(const int argc, const char* const argv[]) {
  if (argc != 2) return 1;
  std::stringstream s;
  s << argv[1];
  unsigned int n;
  s >> n;
  if (not s) return 2;
  double sum = 0;
  for (struct {unsigned int i; int e;} l = {0,-1}; l.i < n; ++l.i, l.e*=-1)
    sum += std::log(l.i+1) * l.e;
  std::cout << "n=" << n << ": " << std::setprecision(16) << sum << "\n";
}

> g++ -O3 SumLog.cpp -o SumLog
> ./SumLog 10000
n=10000: 4.830986538632788
> ./SumLog 20000
n=20000: 5.177547628912792
> ./SumLog 40000
n=40000: 5.524114969192852
   \endverbatim
   </li>
   <li> The question is what are good values for oklib_float_comparison_factor
   and oklib_float_comparison_exponent ? </li>
  </ul>


  \todo Create milestones.


  \todo DONE Improving assert
  <ul>
   <li> DONE (now caught by errcatch)
   There are some errors which our test-system does not notice.
    <ol>
     <li> One finds them by searching for "error" in the OKlibBuilding
     log. </li>
     <li> Apparently these are Lisp errors. </li>
     <li> Perhaps one could catch "MACSYMA-QUIT" ?! </li>
    </ol>
   </li>
  </ul>


  \todo Outline of the test system
  <ul>
   <li> DONE (we roll our own)
   Ask on the Maxima mailing list, whether they have a system in use.
    <ol>
     <li> Apparently, they only have a system where they put expressions and
     expected values into files. That's insufficient. </li>
    </ol>
   </li>
   <li> Compare with the C++ test system; see TestSystem/plans/TestSystem.hpp.
    <ol>
     <li> Similar to the C++ test-system, we have generic test functions,
     which take as argument the function to be tested. </li>
     <li> Likely only functions are to be tested. </li>
     <li> So we could just use, as for the C++ test-system, sub-directories
     "tests" and "testobjects", containing the generic test functions and the
     test instantiations (i.e., expressions evaluating the test function on
     the function to be tested), respectively. </li>
    </ol>
   </li>
   <li> DONE (at least at this time, we just rely on Maxima evaluating all
   expressions in the file, and do not have our own mechanism for running
   tests)
   Execution of the tests:
    <ol>
     <li> Like with the C++ system, in the testobjects-files
     one finds instructions for loading the "testobjects" into a global list
     (provided via dynamic binding when running the tests).
      <ol>
       <li> These "testobjects" perhaps are just the respective function calls,
       unevaluated, while executing the tests means evaluating these terms.
       </li>
       <li> So we need one function "install_testokl(t)", which stores the term
       t, unevaluated, on a global list "testobjects_testokl". </li>
      </ol>
     </li>
     <li> However, just writing the expressions into the testobject-file is
     easier, and seems to do the job as well ?!? </li>
     <li> The files in the "tests"-directories get loaded with
     oklib_load_all(), but not the testobjects-files. </li>
     <li> But the mechanics of running tests (how to find out about errors,
     how to get more precise information, etc.) is not clear yet. See
     the next point about "Assert". </li>
    </ol>
   </li>
   <li> Asserts:
    <ol>
     <li> DONE (when using the buildsystem, then the file with the error is
     printed, while when calling tests directly, then the caller knows about
     the filenames, so the apparent inability of Maxima to output better
     filenames is not so harmful here)
     In ComputerAlgebra/TestSystem/Lisp/Asserts.mac we have implemented
     first assert-functions, which seem to work (and so the discussions below
     perhaps are outdated).
      <ol>
       <li> One problem is that for file-names only the basename is printed.
       </li>
       <li> In the context of the test-system this seems less harmful, since
       the backtrace writes also the oklib_load-function-calls. </li>
       <li> However it would be nice to tell the system to print filenames
       using (some part of) the path. </li>
      </ol>
     </li>
     <li> DONE (only for floating-point comparisons we use a special assert,
     but otherwise we do not write special asserts --- too much trouble)
     As with the C++ system, we have some special "asserts" for conditions,
     which also provide error-messages. </li>
     <li> DONE (in the case of an error there is no return-value, since
     we just abort the test-function)
     Every test-function returns just true in case of success,
     while otherwise false is returned --- though the return value
     likely is not much of use, but the real output is the side
     effect that an error-message is printed, using "error" (this
     should cause abortion). </li>
     <li> DONE (apparently it doesn't work when called by ourselfes)
     The "backtrace()"-call is useful here: In case of
     an error not just the error message is printed, but also the trace of
     function calls. </li>
     <li> DONE (yet the simple system suffices)
     Perhaps we create a macro for this error-output (similar
     to the C++-macro). </li>
     <li> DONE (this could be considered later)
     Is it possible to provide information about the file etc. where
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
       <li> Seems difficult; perhaps the error-output is stored in a file?
       </li>
       <li> This would then be the error-output and the testobject-term. </li>
       <li> It seems not possible to redirect the output of the
       backtrace-function? </li>
       <li> Perhaps we use "load" first to load the testobject-files (without
       output), and then via "batch" the testobjects are processed, without
       (much) output in case of no error, while we have all the above
       error-information in case of an error. </li>
       <li> We issue then an error inside the batch-file, and so the make-process
       notices the error and halts. </li>
       <li> If an error occurred, perhaps with "trace(all)" everything is
       traced, and the error term is re-evaluated? "trace" seems to be more
       informative than "backtrace". </li>
      </ul>
     </li>
     <li> DONE (yes, at least for now it's sufficient).
     On the other hand, just using the error-function seems to provide
     enough information?!? </li>
     <li> DONE (it's called "oklib_test_level")
     There is a global variable "test_level" for the test-level. </li>
     <li> DONE (yet no need for that)
     And perhaps also "log_level". </li>
     <li> DONE (for now that mmust suffice)
     Since we don't have namespaces, we need naming-conventions.
     Perhaps "okltest_" as generic prefix. </li>
     <li> DONE Each test-function has one argument, the function to be tested.
     </li>
    </ol>
   </li>
   <li> DONE (actually, it is now "oklib new_check"; later we should, perhaps
   under "full" and "extensive", also run the tests in the full
   oklib-environment, i.e., calling oklib_load_all() first; and then also
   calling the tests twice (within the same environment))
   "oklib check" is also responsible for the maxima-tests, via a sub-goal
   (so that also only the maxima-tests can be involved).
    <ol>
     <li> After loading all testobject-files, a function "run_testokl" is
     called which evaluates the terms in "testobjects_testokl". </li>
     <li> A complication arises for functions to be tested which require
     special contexts. Best to avoid this. However if needed, then the
     testobject should just also contain this context. </li>
     <li> "Contexts" seem just to refer to "facts" etc. It could be that
     a special environment is needed, with special variables and functions
     defined; but again this should be provided by the testobject. </li>
     <li> Preparing the environment:
      <ol>
       <li> Since we run the test with a fresh Maxima, we don't need to use
       "kill(all)" at the beginning. </li>
       <li> However we should have the possibility to run the tests several
       times, to see whether there are harmful side-effects. </li>
       <li> So all "main" functions of the OKlibrary shouldn't change the
       global environment (these "main" functions include all test-functions;
       perhaps "main" here means "testable"). </li>
      </ol>
     </li>
     <li> As usual all testobjects for the calling directory level are
     executed. </li>
     <li> Likely we should not provide a mechanism for running only tests when
     needed (too complicated).
      <ol>
       <li> Just run always all respective tests (and "basic"
       tests really should run quickly). </li>
       <li> But, as usual, only those belonging to the current directory
       level. </li>
      </ol>
     </li>
     <li> DONE (basically it's done like that, but without any special
     include-hierarchies; yet for every level we need a generic makefile)
     Perhaps actually simplest is that "oklib check" gathers all
     testobject-files (by "find"), writes "oklib_load"-instructions
     accordingly into a file "okltest", and then just runs Maxima, using the
     option "--batch=okltest".
      <ol>
       <li> More precisely, the manual set-up would just create an
       include-hierarchy, parallel to the include-hierarchy for normal
       Maxima-code. </li>
       <li> And then running the tests at a specific level happens via
       batch-processing the include-file at the wished level. </li>
       <li> Should we use "oklib_load" or "oklib_include" ? Perhaps
       oklib_load, since repeated inclusion is unlikely, and if, then
       there should be a reason. </li>
       <li> Now the build-system just simulates creation of these
       include-files (this is more intelligent than the above simple
       method, lumping all includes into one file). </li>
       <li> The main target is "check-maxima", with subtargets
       "prepare_tests_maxima" and "run_tests_maxima". </li>
       <li> prepare_tests_maxima recursively creates the include-files,
       in system_directories/tests. </li>
       <li> run_tests_maxima runs the appropriate include-file. </li>
      </ol>
     </li>
    </ol>
   </li>
   <li> DONE (error messages are output to a file, and in this way we detect
   the presence of an error)
   First rough "script" for running the tests
    <ol>
     <li>
     \verbatim
OKplatform> (for F in $(find OKsystem/OKlib/ComputerAlgebra -path "*/testobjects/*.mac"); do oklib --maxima --batch=${F} --very-quiet; if [[ $? != 0 ]]; then exit 1; fi; done)
     \endverbatim
     would work except that maxima returns 0 apparently in any case???
     It seems not possible to get a reaction on the error??? </li>
     <li> A possibility would be to set an environment-variable via "system".
     But apparently using for example system("export OKLIBMAXIMA=1") is not
     transported to the outer world, and thus is unusable. </li>
     <li> Or we use a different configuration file, as discussed above; but
     still the problem how to get informed about the error. </li>
     <li> Is it really necessary to write to a file via "system" ??? </li>
     <li> It seems that we should add the capability to the assert-function
     that in case of error and oklib_automatic_test=true (by default it's
     false) something is written to a file. </li>
     <li> For batch-running testobject-files, before the run the file is
     deleted, and if it has been created then the buildsystem issues an
     error. </li>
     <li> This assumes that every testobject-file is run on its own ---
     seems alright. </li>
     <li> The output of the batch-runs is all redirected into one log-file.
     One needs to look at it from time to time. </li>
     <li> Perhaps automatically one should parse the output for "error" etc.
     </li>
     <li> According to the e-mail reply by Mike Hansen to my request at the
     Maxima-mailing list (27.2.2008) we could use the Sage interface. </li>
    </ol>
   </li>
  </ul>


  \todo Testing the demos
  <ul>
   <li> Also the demos-files need to be run, via oklib_load, to see whether
   they still function correct. </li>
   <li> Since they contain asserts, this is also contains tests. </li>
   <li> A problem is that some demos run longer. </li>
   <li> So demos need to be qualified as "basic tests", "full tests",
   or "extensive tests". The problem how to do this.
    <ol>
     <li> The first line of a demos-file shall be
     \code
if oklib_test_demos then
 if oklib_test_demos_level < 2 then return()$
     \endcode
     (where "2" here is the test-level of this file ("extensive" in
     this case)).
     </li>
     <li> Normally, oklib_test_demos=false. </li>
     <li> For running the demos in test-mode, oklib_test_demos=true,
     and then oklib_test_demos_level decides whether to run the demos-file
     or not.
     </li>
    </ol>
   </li>
   <li> And should these "tests" be included in the normal maxima-test? </li>
   <li> First we create a special target "maxima_check_demos", and then
   we'll decide. </li>
   <li> This target, as usual, loads all demos-files (.mac-files in
   demos-directories) below the given directory. </li>
   <li> So again a special maxima-init-file is created by the process,
   which contains all respective load-instructions. </li>
   <li> Again the question is whether we do this recursively. </li>
   <li> And again this appears to be superior. </li>
  </ul>


  \todo Improving the test system
  <ul>
   <li> It would be better to check whether all testobject-expressions actually
   evaluate to true (since tests might be broken, and for example simply
   nothing might be computed):
    <ol>
     <li> I remember that there is a Maxima function, which like "batch"
     executes all expressions in a file, and checks whether each evaluates
     to true? </li>
     <li> Apparently there is an undocumented feature of batch, namely it
     can be called as "batch(filename, 'test)", in which case it expects
     the file to be organised as a list of pairs of expressions
     \verbatim
expression;
expected_value$
     \endverbatim
     however this would be clumsy (since our expected value is always just
     "true"), and furthermore also some error-log is written to some file,
     which likely is (too) hard to control. </li>
    </ol>
   </li>
   <li> In oklib_test_level>=1 we must also additionally run the test
   with oklib_test_level-- and with oklib_monitor=true and
   oklib_monitor_level=0,1 (at least). </li>
   <li> return-expressions in functions to be tested:
    <ol>
     <li> If the function to be tested doesn't use a block, but uses a
     return-expression, then this just terminates the assert-evaluation
     itself. </li>
     <li> How can this be avoided? </li>
    </ol>
   </li>
   <li> DONE (now handled in 
   Buildsystem/MasterScript/SpecialProcessing/plans/general.hpp)
    We have the problem that the created file maxima-init.mac clashes
    with other such files created when running "oklib --maxima":
    <ol>
     <li> DONE (this is now handled by the userdir-directory)
     Is it possible to use for the test-runs a different initialisation
     file? Ask on the Maxima mailing list. </li>
     <li> See "Improve locality" in
     Buildsystem/MasterScript/SpecialProcessing/plans/general.hpp. </li>
    </ol>
   </li>
  </ul>


  \todo Handling floating and bigfloating numbers
  <ul>
   <li> It seems that assert_float_equal and assert_bfloat_equal work fine,
   however especially for assert_bfloat_equal this needs to be properly
   tested. </li>
  </ul>

*/

