// Oliver Kullmann, 7.10.2007 (Swansea)
/* Copyright 2007, 2008, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/ExternalSources/SpecialBuilds/plans/Maxima.hpp
  \brief Plans regarding installation of Maxima


  \bug Maxima loading of contrib packages fails on Maxima versions >= 5.18.0.0
  <ul>
   <li> Trying to run "oklib_load_all" fails : 
   \verbatim
[11:45:36 - aeternus] ExternalSources$ oklib --maxima
m4 --prefix-builtins /home/aeternus/Work/OKlibrary/OKlib/OKplatform/OKsystem/OKlib/Buildsystem/Html/m4_shell_macro /home/aeternus/Work/OKlibrary/OKlib/OKplatform/OKsystem/OKlib/Buildsystem/MasterScript/SpecialProcessing/maxima-init.mac > /home/aeternus/Work/OKlibrary/OKlib/OKplatform/ExternalSources/Installations/Maxima/ecl/5.18.1/share/maxima/5.18.1/share/maxima-init.mac
/home/aeternus/Work/OKlibrary/OKlib/OKplatform/ExternalSources/Installations/Maxima/ecl/5.18.1/bin/rmaxima 
;;; Loading #P"/home/aeternus/Work/OKlibrary/OKlib/OKplatform/ExternalSources/Installations/Ecl/8.12.0/lib/ecl-8.12.0/defsystem.fas"
;;; Loading #P"/home/aeternus/Work/OKlibrary/OKlib/OKplatform/ExternalSources/Installations/Ecl/8.12.0/lib/ecl-8.12.0/cmp.fas"
;;; Loading #P"/home/aeternus/Work/OKlibrary/OKlib/OKplatform/ExternalSources/Installations/Ecl/8.12.0/lib/ecl-8.12.0/sysfun.lsp"
Maxima 5.18.1 http://maxima.sourceforge.net
Using Lisp ECL 8.12.0 (CVS 2008-07-12 18:54)
Distributed under the GNU Public License. See the file COPYING.
Dedicated to the memory of William Schelter.
The function bug_report() provides bug reporting information.
(%i1) oklib_load_all()$
Could not find `descriptive' using paths in file_search_maxima,file_search_lisp.
#0: oklib_plain_include(name=descriptive)(maxima-init.mac line 98)
#1: oklib_plain_include(name=/home/aeternus/Work/OKlibrary/OKlib/OKplatform/OKsystem/OKlib/ComputerAlgebra/DataStructures/Lisp/Ha...)
#2: oklib_include(name=OKlib/ComputerAlgebra/DataStructures/Lisp/HashMaps.mac)(maxima-init.mac line 94)
 -- an error.  To debug this try debugmode(true);
   \endverbatim
   </li>
   <li> Something appears to be going wrong with the file_search_maxima paths
   as there are entries such as:
   \verbatim
"/home/aeternus/Work/OKlibrary/OKlib/OKplatform/ExternalSources/Installations/Maxima/ecl/5.18.1/share/maxima/5.18.1/share/{/home/aeternus/Work/OKlibrary/OKlibrary-0.2.1.2_00104/OKplatform/ExternalSources/Installations/Maxima/ecl/5.18.1/share/maxima/5.18.1/share,/home/aeternus/Work/OKlibrary/OKlibrary-0.2.1.2_00104/OKplatform/ExternalSources/Installations/Maxima/ecl/5.18.1/share/maxima/5.18.1/share/affine,/home/aeternus/Work/OKlibrary/OKlibrary-0.2.1.2_00104/OKplatform/ExternalSources/Installations/Maxima/ecl/5.18.1/share/maxima/5.18.1/share/algebra,/home/aeternus/Work/OKlibrary/OKlibrary-0.2.1.2_00104/OKplatform/ExternalSources/Installations/Maxima/ecl/5.18.1/share/maxima/5.18.1/share/algebra/charsets,/home/aeternus/Work/OKlibrary/OKlibrary-0.2.1.2_00104/OKplatform/ExternalSources/Installations/Maxima/ecl/5.18.1/share/maxima/5.18.1/share/algebra/solver,/home/aeternus/Work/OKlibrary/OKlibrary-0.2.1.2_00104/OKplatform/ExternalSources/Installations/Maxima/ecl/5.18.1/share/maxima/5.18.1/share/calculus,/home/aeternus/Work/OKlibrary/OKlibrary-0.2.1.2_00104/OKplatform/ExternalSources/Installations/Maxima/ecl/5.18.1/share/maxima/5.18.1/share/colnew,/home/aeternus/Work/OKlibrary/OKlibrary-0.2.1.2_00104/OKplatform/ExternalSources/Installations/Maxima/ecl/5.18.1/share/maxima/5.18.1/share/colnew/lisp,/home/aeternus/Work/OKlibrary/OKlibrary-0.2.1.2_00104/OKplatform/ExternalSources/Installations/Maxima/ecl/5.18.1/share/maxima/5.18.1/share/combinatorics
   \endverbatim
   (and more) where the entry should be of the form:
   \verbatim
"/home/aeternus/Work/OKlibrary/OKlib/OKplatform/ExternalSources/Installations/Maxima/ecl/5.18.1/share/maxima/5.18.1/share/{share,affine,algebra,algebra/charsets,algebra/solver,calculus,colnew,colnew/lisp,combinatorics,
   \endverbatim
   <li>
   <li> With Maxima 5.17.1.1, this problem does not occur. </li>
   <li> An e-mail should be sent to the maxima mailing list (by MG). </li>
  </ul>

  \todo DONE (transfer to documentation)
  Building of Maxima-packages
  <ul>
   <li> Creating a package, for example, 5.17.1.1, happens as follows:
   \verbatim
> cvs -z3 -d:pserver:anonymous@maxima.cvs.sourceforge.net:/cvsroot/maxima co -P maxima

 change line with "AM_INIT_AUTOMAKE" in maxima/configure.in
 to "AM_INIT_AUTOMAKE(maxima,5.17.1.1)"

> mv maxima maxima-5.17.1.1
> cd maxima-5.17.1.1; sh bootstrap; cd ..
> tar -czf maxima-5.17.1.1.tar.gz maxima-5.17.1.1
   \endverbatim
   </li>
   <li> However, this seems not to be complete, since apparently
   then for building this package automake is needed.
    <ol>
     <li> So perhaps we need to run automake when building the package? </li>
     <li> But the bootstrap script does this already, and it seems to
     do the normal job. </li>
     <li> The recommended way of creating a package then is actually
     to call "./configure" after changing that line, and then to
     call "make dist", which creates the package. </li>
     <li> Seems to work for 5.17.1.1, but for later versions the program
     xgettext needs to be installed. </li>
     <li> Suse 10.2 doesn't include it; apparently it is part of the Gnu
     gettext package http://www.gnu.org/software/gettext/ . </li>
    </ol>
   </li>
  </ul>


  \bug Inconsistent installation of Maxima : DONE
  <ul>
   <li> The plan was that "boostrap" for 5.17.1.1 is always called for this
   special installation. </li>
   <li> However, due to the maxima-build-makefile not being updated, this
   actually doesn't happen --- but it works apparently?!? </li>
   <li> Actually, bootstrap was already called for the package we built,
   and it works, so that's the way how to build maxima-packages. </li>
  </ul>


  \todo Update to Maxima 5.17.1 : DONE (needs to be transferred to the
  docus; see also "Building of Maxima-packages" above)
  <ul>
   <li> Creating a package, which updates the current version 5.17.1 from
   CVS (calling it "5.17.1.1"; we should use this scheme from now on (it
   seems it wouldn't conflict with the "official" Maxima naming schemes)):
   \verbatim
> cvs -z3 -d:pserver:anonymous@maxima.cvs.sourceforge.net:/cvsroot/maxima co -P maxima

 change line with "AM_INIT_AUTOMAKE" in maxima/configure.in
 to "AM_INIT_AUTOMAKE(maxima,5.17.1.1)"

> mv maxima maxima-5.17.1.1
> cd maxima-5.17.1.1; sh bootstrap; cd ..
> tar -czf maxima-5.17.1.1.tar.gz maxima-5.17.1.1
   \endverbatim
   </li>
   <li> To build such a package, first "sh bootstrap" is needed. </li>
   <li> A nasty little problem is that the directory
   Installations/Maxima/ecl/5.17.1/share/maxima/5.17.1 is now called
   Installations/Maxima/ecl/5.17.1/share/maxima/5.17post. </li>
   <li> But perhaps this is just caused by the special way we created this
   replacement-package, so let's assume it's just temporary. </li>
  </ul>


  \todo Update to Maxima 5.16.x : DONE
  <ul>
   <li> DONE With 5.16.3 now there appear no errors anymore. </li>
   <li> DONE (problem with ifactor solved in gf-code)
   However, now the gf-package is very slow, such that
   "time oklib new_check" on cs-wsok goes up from
   \verbatim
real    4m1.167s
user    3m21.613s
sys     0m21.669s
   \endverbatim
   (with 5.15.0) to
   \verbatim
real    5m18.519s
user    4m31.577s
sys     0m22.377s
   \endverbatim
   which seems solely due to the gf-package. </li>
   <li>  DONE (see above)
   For comparisons: On csltok we have with ecl (and load(nset))
   \verbatim
real    7m25.216s
user    5m58.919s
sys     0m26.903s
   \endverbatim
   and with clisp (as above; also with load(nset))
   \verbatim
real    10m44.758s
user    7m35.101s
sys     0m59.227s
   \endverbatim
   (both with 5.16.3); without load(nset) clisp gets faster:
   \verbatim
real    8m38.436s
user    6m46.368s
sys     0m52.634s
   \endverbatim
   </li>
  </ul>


  \todo Enabling use of ECL : DONE
  <ul>
   <li> DONE (problems seems to have gone away)
   Interrupting a computation once with "Ctrl-C" worked, but applying
   the second time the console did not respond anymore (though the computation
   was interrupted)??
    <ol>
     <li> Just entering "do (a:1);" on the command line, and interrupting
     and restarting it a couple of times (say, five times) results in the
     system not interacting anymore (only to "Control-z", which stops it).
     </li>
    </ol>
   </li>
   <li> DONE (solved)
   Plotting discrete data does not work (as in the manual):
   \verbatim
(%i1) xx:[10, 20, 30, 40, 50]$
Evaluation took 0.0010 seconds (0.0010 elapsed)
(%i2) yy:[.6, .9, 1.1, 1.3, 1.4]$
Evaluation took 0.0010 seconds (0.0000 elapsed)
(%i3) plot2d([discrete,xx,yy])$
Maxima encountered a Lisp error:
 Unknown keyword :WAIT
   </li>
   <li> DONE (perhaps for the future one must observe this, but currently
   problems seem to have gone away)
   There is a general problem with using recursive memoised functions:
   Apparently Maxima/Lisp cannot handle them, and so we must eliminate them!
   </li>
   <li> DONE (see above)
   Loading
   ComputerAlgebra/Satisfiability/Lisp/MinimalUnsatisfiability/tests/SegFault.mac
   into (plain or oklib-initialised) ECL produces a segmentation fault (while
   clisp has no problems):
   \verbatim
ExternalSources/Installations/Maxima/ecl/5.16.3/bin> ./maxima
Maxima 5.16.3 http://maxima.sourceforge.net
Using Lisp ECL 0.9l (CVS 2008-06-19 17:09)
Distributed under the GNU Public License. See the file COPYING.
Dedicated to the memory of William Schelter.
The function bug_report() provides bug reporting information.
(%i1) load("/home/kullmann/csoliver/SAT-Algorithmen/OKplatform/OKsystem/OKlib/ComputerAlgebra/Satisfiability/Lisp/MinimalUnsatisfiability/tests/SegFault.mac");
Maxima encountered a Lisp error:

 Segmentation violation.
   \endverbatim
   </li>
   <li> DONE (see above for an extracted example)
   A segmentation fault:
   \verbatim
Maxima 5.16.3 http://maxima.sourceforge.net
Using Lisp ECL 0.9l (CVS 2008-06-19 17:09)
(%i1) oklib_load_all();
Evaluation took 5.4600 seconds (5.5430 elapsed)
(%o1) "/home/csoliver/SAT-Algorithmen/OKplatform/OKsystem/OKlib/ComputerAlgebra/include.mac"
(%i2) okltest_analyse_isorepo_defset_mvd(analyse_isorepo_defset_mvd);
Evaluation took 0.0040 seconds (0.0030 elapsed)
(%o2) true
(%i3) oklib_test_level:1;
Evaluation took 0.0000 seconds (0.0000 elapsed)
(%o3) 1
(%i4) okltest_analyse_isorepo_defset_mvd(analyse_isorepo_defset_mvd);
make: *** [run_maxima] Segmentation fault
   \endverbatim
    <ol>
     <li> The segmentation fault seems to happen in analyse_isorepo_defset_mvd
     when performing "set_hm(h,def,union(ev_hm_d(h,def,{}), ...", which
     contains a "union", and thus could have the same root as the above error?
     </li>
     <li> When splitting the computation into sub-computation, then the same
     union-error as above occurs, which can be eliminated by "load(nset)",
     but then the segmentation fault shows up nevertheless, so it seems
     to be another Ecl-bug. </li>
     <li> The above output is on cs-wsok; on csltok it looks a bit different,
     and may even work, but this only seems to hide the memory corruption: 
     \verbatim
true
Maxima encountered a Lisp error:

 Segmentation violation.
     \endverbatim
     (one sees that, different from cs-wsok, the computation is actually
     completed before the memory violation is noted). </li>
     <li> So on csltok this error is not analysable. </li>
     <li> On cs-wsok one sees that the segmentation fault happens inside
     min_variable_degree_cs, however this doesn't need to mean anything,
     since all operations on their own behave correct, and the memory
     corruption perhaps happened earlier. </li>
     <li> It seems difficult to make this segfault reproducible for the Maxima
     mailing list. </li>
     <li> DONE (one needs to input an expression like "0;")
     One strange error occurs with the file "Batch.mac" of
     the content
     \verbatim
for x in [1,2,3] do print(x); 

     \endverbatim
     where after the ";" we have one space: Both in CLisp and in ECL
     after batch("Batch.mac") the frontend hangs --- tell the Maxima
     mailing list. </li>
     <li> DONE (Maxima-error corrected)
     Then with 
     "oklib_batch("OKlib/ComputerAlgebra/Satisfiability/Lisp/Symmetries/Symmetries.mac");
     we get an error-message
     \verbatim
Maxima encountered a Lisp error:
CAR: $FOUND is not a list
Automatically continuing.
To reenable the Lisp debugger set *debugger-hook* to nil.
     \endverbatim
     which seems to indicate that a parse-error occurs at the line
     \verbatim
for GG in candidates unless found do
      if is_isomorphic_btr_fcs(FF,GG) then found : true,
     \endverbatim
     : Removing the "unless found" removes the error message??
     Tell the Maxima mailing list. </li>
     <li> DONE (see above)
     This can be reproduced by the file "Unless.mac" of content
     \verbatim
fun(found) := for x in [1,2,3] unless found do print(x);
     \endverbatim
     where "batch("Unless.mac")" results in
     \verbatim
Maxima encountered a Lisp error:
 $FOUND is not of type LIST.
Automatically continuing.
To reenable the Lisp debugger set *debugger-hook* to nil.
     \endverbatim
     The function "fun" is also not loaded; this disappears when using
     "load("Unless.mac"); </li>
     <li> Though also when loading
     OKlib/ComputerAlgebra/Satisfiability/Lisp/MinimalUnsatisfiability/testobjects/uhit_def.mac
     by oklib_load instead of oklib_batch we also get an error
     (with ecl): The frontend gets into an infinite loops and 
     doesn't react anymore. </li>
    </ol>
   </li>
   <li> DONE
   Regarding argument-length restriction in Ecl, w.r.t. general
   applications Ecl is stronger than CLisp (at least on 32-bit machines),
   while w.r.t. union it is weaker. </li>
   <li> DONE (later we should try it, but the argument-length restriction
   is handled by our system)
   We need to check cmucl --- if this is unrestricted, works and is
   not slower than CLisp, then it should become our new default. </li>
   <li> DONE
   We should make variable maxima_lisp_name_okl available in Maxima. </li>
   <li> DONE
   Upper bound on argument-lengths:
    <ol>
     <li> Providing variable "maximal_argument_length" with the
     (true) upper bound on the argument length(2^16 for Ecl, inf for CLisp).
     </li>
     <li> However for CLisp we need to differentiate between the "general"
     upper bound (actually 4095) and the "union" upper bound. </li>
    </ol>
   </li>
   <li> DONE
   okltest_set_sets_union (in
   ComputerAlgebra/Hypergraphs/Lisp/tests/SetSystems.mac) contains a testcase
   which shows a clear Maxima/Ecl bug.
    <ol>
     <li> However, this seems specific to 32-bit machines, since on cs-wsok
     it works:
     \verbatim
Maxima 5.16.3 http://maxima.sourceforge.net
Using Lisp ECL 0.9l (CVS 2008-06-19 17:09)
Distributed under the GNU Public License. See the file COPYING.
Dedicated to the memory of William Schelter.
The function bug_report() provides bug reporting information.
(%i1) apply(union,listify({{-6,-5,-4,-3,-2,-1},{-6,-5,-4,-3,-2,1},{-6,-5,-4,-3,-1,2},{-6,-5,-4,-3,1,2},{-6,-5,-4,-2,-1,3},{-6,-5,-4,-2,1,3},{-6,-5,-4,-1,2,3},{-6,-5,-4,1,2,3}, {-6,-5,-3,-2,-1,4},{-6,-5,-3,-2,1,4},{-6,-5,-3,-1,2,4},{-6,-5,-3,1,2,4},{-6,-5,-2,-1,3,4},{-6,-5,-2,1,3,4},{-6,-5,-1,2,3,4},{-6,-5,1,2,3,4},{-6,-4,-3,-2,-1,5},{-6,-4,-3,-2,1,5},{-6,-4,-3,-1,2,5},{-6,-4,-3,1,2,5},{-6,-4,-2,-1,3,5},{-6,-4,-2,1,3,5},{-6,-4,-1,2,3,5},{-6,-4,1,2,3,5},{-6,-3,-2,-1,4,5},{-6,-3,-2,1,4,5},{-6,-3,-1,2,4,5},{-6,-3,1,2,4,5},{-6,-2,-1,3,4,5},{-6,-2,1,3,4,5},{-6,-1,2,3,4,5},{-6,1,2,3,4,5},{-5,-4,-3,-2,-1,6},{-5,-4,-3,-2,1,6},{-5,-4,-3,-1,2,6},{-5,-4,-3,1,2,6},{-5,-4,-2,-1,3,6},{-5,-4,-2,1,3,6}, {-5,-4,-1,2,3,6},{-5,-4,1,2,3,6},{-5,-3,-2,-1,4,6},{-5,-3,-2,1,4,6},{-5,-3,-1,2,4,6},{-5,-3,1,2,4,6},{-5,-2,-1,3,4,6},{-5,-2,1,3,4,6},{-5,-1,2,3,4,6},{-5,1,2,3,4,6}, {-4,-3,-2,-1,5,6},{-4,-3,-2,1,5,6},{-4,-3,-1,2,5,6},{-4,-3,1,2,5,6},{-4,-2,-1,3,5,6},{-4,-3,-2,-1,5,6},{-4,-3,-2,1,5,6},{-4,-3,-1,2,5,6},{-4,-3,1,2,5,6},{-4,-2,-1,3,5,6},{-4,-2,1,3,5,6},{-4,-1,2,3,5,6},{-4,1,2,3,5,6},{-3,-2,-1,4,5,6},{-3,-2,1,4,5,6}, {-3,-1,2,4,5,6},{-3,1,2,4,5,6},{-2,-1,3,4,5,6},{-2,1,3,4,5,6},{-1,2,3,4,5,6},{1,2,3,4,5,6}}));
Evaluation took 0.0000 seconds (0.0030 elapsed)
(%o1) {-6,-5,-4,-3,-2,-1,1,2,3,4,5,6}
     \endverbatim
     </li>
     <li> While on csltok we get
     \verbatim
Function union expects a set, instead found union
 -- an error.  To debug this try debugmode(true);
     \endverbatim
     </li>
     <li> Suggestions from the Maxima mailing list:
     \verbatim
Hmm. What does trace(union) show as the arguments to union ?
If there is some expression in the arguments which is union(...) instead
of {...}, maybe trace(listify, apply) shows from where it originates.
Does load(nset) make the problem go away? (By running interpreted code
instead of compiled.)
     \endverbatim
     We get after trace(union):
     \verbatim
1" Enter "union" "[{-6,-5,-4,-3,-2,-1},{-6,-5,-4,-3,-2,1},{-6,-5,-4,-3,-1,2},{-6,-5,-4,-3,1,2},{-6,-5,-4,-2,-1,3},
 {-6,-5,-4,-2,1,3},{-6,-5,-4,-1,2,3},{-6,-5,-4,1,2,3},{-6,-5,-3,-2,-1,4},{-6,-5,-3,-2,1,4},
 {-6,-5,-3,-1,2,4},{-6,-5,-3,1,2,4},{-6,-5,-2,-1,3,4},{-6,-5,-2,1,3,4},{-6,-5,-1,2,3,4},
 {-6,-5,1,2,3,4},{-6,-4,-3,-2,-1,5},{-6,-4,-3,-2,1,5},{-6,-4,-3,-1,2,5},{-6,-4,-3,1,2,5},
 {-6,-4,-2,-1,3,5},{-6,-4,-2,1,3,5},{-6,-4,-1,2,3,5},{-6,-4,1,2,3,5},{-6,-3,-2,-1,4,5},
 {-6,-3,-2,1,4,5},{-6,-3,-1,2,4,5},{-6,-3,1,2,4,5},{-6,-2,-1,3,4,5},{-6,-2,1,3,4,5},
 {-6,-1,2,3,4,5},{-6,1,2,3,4,5},{-5,-4,-3,-2,-1,6},{-5,-4,-3,-2,1,6},{-5,-4,-3,-1,2,6},
 {-5,-4,-3,1,2,6},{-5,-4,-2,-1,3,6},{-5,-4,-2,1,3,6},{-5,-4,-1,2,3,6},{-5,-4,1,2,3,6},
 {-5,-3,-2,-1,4,6},{-5,-3,-2,1,4,6},{-5,-3,-1,2,4,6},{-5,-3,1,2,4,6},{-5,-2,-1,3,4,6},
 {-5,-2,1,3,4,6},{-5,-1,2,3,4,6},{-5,1,2,3,4,6},{-4,-3,-2,-1,5,6},{-4,-3,-2,1,5,6},
 {-4,-3,-1,2,5,6},{-4,-3,1,2,5,6},{-4,-2,-1,3,5,6},{-4,-2,1,3,5,6},{-4,-1,2,3,5,6},{-4,1,2,3,5,6},
 {-3,-2,-1,4,5,6},{-3,-2,1,4,5,6},{-3,-1,2,4,5,6},{-3,1,2,4,5,6},{-2,-1,3,4,5,6},{-2,1,3,4,5,6},
 {-1,2,3,4,5,6},?\#\<illegal\ pointer\ bfff0e28\>]""""
Function union expects a set, instead found ?the\-trace\-apply\-hack
 -- an error.  To debug this try debugmode(true);
     \endverbatim
     Then using trace(listify,apply) doesn't create interesting output, except
     of new error message
     \verbatim
 1" Enter "union" "
Maxima encountered a Lisp error:

 #<frame 268451060 3221163056> is not of type SI:INSTANCE.
     \endverbatim
     </li>
     <li> But actually "load(nset)" solves the problem! What is this doing?
     </li>
     <li> Another proposal from the Maxima mailing list:
     \verbatim
Please try
     for N step 10 thru 5000 do (print(N), apply(union, create_list({},i,1,N)));
and let us know what it prints.  How exactly did you try much bigger
unions?  It is important to know exactly what code you used.
     \endverbatim
     On cs-wsok this runs through without problems (50 seconds with makelist);
     also on csltok (71s with create_list, 122s with makelist). </li>
     <li> A proposed test case is
     \verbatim
for N: 5 thru 5000 do (lastn:N, if length(apply(union,makelist({floor(i/4),floor(i/3),2.5^i},i,1,N))) # N+floor(N/3)+1 then error(mismatch,N));
     \endverbatim
     (however N=774 is the maximal exponent possible with floats). </li>
     <li> On 32-bit machines the bounds should be
     \verbatim
gcl     64
ecl     65536
clisp   4096
cmucl   most-positive-fixnum
ANSI CL says call-arguments-limit is at least 64, so for portable code,
you can't go above 64.
     \endverbatim
     However union-argument-lengths of 2^20 on cs-wsok and on csltok definitly
     work for clisp: the bound of 4095 (corrected) though is true for
     apply(append,...). </li>
     <li> The ecl-bound of 65536 holds for union *and* append, on 32-bit as
       well as for 64-bit machines. </li>
     <li> If gcl really has such a low argument-length, then it will be
     useless for us. </li>
     <li> ecl is on the border, while cmucl is worth a try. </li>
     <li> In any case, Ecl should be able to handle the little union;
     a proposed fix is to use --disable-asmapply when building Ecl. </li>
     <li> And on csltok this fixes the bug! </li>
    </ol>
   </li>
   <li> DONE (for now our build system repairs the rmaxima-script)
   rmaxima:
    <ol>
     <li> rmaxima is broken since it just calls "maxima", i.e., it assumes
     system-wide installation.
     <li> So "maxima" in rmaxima has to be replaced by the full path. </li>
    </ol>
   </li>
   <li> DONE
   See bug "False assumption for testing hash-maps" in
   ComputerAlgebra/DataStructures/Lisp/plans/HashMaps.hpp. </li>
   <li> DONE (Maxima-bug was corrected)
   Building Maxima works now, but it seems that make_random_state
   is now broken (we get test failures for our tests)? </li>
  </ul>


  \todo Database of integer sequences
  <ul>
   <li> We have to see whether the sequences are becoming (publically)
   available. </li>
   <li> If this is not the case, then likely we shouldn't contribute to
   that database. </li>
  </ul>


  \todo Additional packages
  <ul>
   <li> Real algebraic geometry
    <ol>
     <li> The book [Algorithms in Real Algebraic Geometry; Basu, Pollack, Roy]
     is available at http://www.math.gatech.edu/~saugata/bpr-posted1.html.
     </li>
     <li> Also maxima-code for various chapters is available. </li>
    </ol>
   </li>
  </ul>


  \todo System-wide installation
  <ul>
   <li> The build-system should be extended, taking for example the
   Gmp-installation as an example, so that we can also produce/use in the
   standard way system-wide installations of Libsigsegv, CLisp and Maxima.
   </li>
  </ul>


  \todo Handling of redefined load-function
  <ul>
   <li> See "How to eliminate the annotation of lists" in
   ComputerAlgebra/plans/Maxima.hpp. </li>
   <li> And see the e-mail thread (May 2008)
   "how to stop the annotation of lists by filenames?" on the Maxima
   mailing list. </li>
   <li> We need documentation on this regarding the build-system:
    <ol>
     <li> We need to watch the Maxima distribution, whether the
     two functions involved, "load" and "add-lineinfo", ever are
     changed, and then we need to incorporate these changes. </li>
     <li> Hopefully, this is not a big thing, since these functions
     are rather small. </li>
     <li> The good point about this solution is its non-intrusiveness:
     The Maxima system doesn't know about these changes. </li>
    </ol>
   </li>
  </ul>


  \todo Install Maxima : DONE
  <ul>
   <li> Write configuration file for Maxima. DONE </li>
   <li> Write installation script for Maxima. DONE </li>
   <li> Provide links to the documentation for Maxima (including
   the maxima-book and the tutorial. DONE </li>
   <li> What about the documentation for Maxima provided with Sage? DONE
   (removed) </li>
   <li> Write docu for installation of Maxima. DONE </li>
   <li> Put information on the main-external-sources-page. DONE </li>
  </ul>


  \todo Install Maxima 5.13 on csltok : DONE (works now (with 5.14))
  <ul>
   <li> First install clisp (should we supply this?); see below. </li>
   <li>
   \verbatim
Installations> mkdir Maxima
builds> mkdir Maxima
Maxima> tar -xzf ../../sources/Maxima/maxima-5.13.0.tar.gz
Maxima> cd maxima-5.13.0/
maxima-5.13.0> LANG=C ./configure --prefix=${OKplatform}/ExternalSources/Installations/Maxima
maxima-5.13.0> LANG=C make
   \endverbatim
   yields
   \verbatim
Compiling file /home/kullmann/csoliver/SAT-Algorithmen/OKplatform/ExternalSources/builds/Maxima/maxima-5.13.0/src/clmacs.lisp ...
*** - READ from
#<INPUT BUFFERED FILE-STREAM CHARACTER
  #P"/home/kullmann/csoliver/SAT-Algorithmen/OKplatform/ExternalSources/builds/Maxima/maxima-5.13.0/src/clmacs.lisp"
  @317>: #<PACKAGE CUSTOM> has no external symbol with name "*FLOATING-POINT-RATIONAL-CONTAGION-ANSI*"
The following functions were used but not defined:
 MAXIMA::MFUNCALL
0 errors, 0 warnings
make[1]: *** [binary-clisp/maxima.mem] Error 1
make[1]: Leaving directory `/home/kullmann/csoliver/SAT-Algorithmen/OKplatform/ExternalSources/builds/Maxima/maxima-5.13.0/src'
make: *** [all-recursive] Error 1
   \endverbatim
   ??? The same error without setting prefix.
   \verbatim
maxima-5.13.0> clisp --version
WARNING: *FOREIGN-ENCODING*: reset to ASCII
GNU CLISP 2.33.2 (2004-06-02) (built on g242.suse.de [10.10.103.242])
Software: GNU-C 3.3.4 (pre 3.3.5 20040809) ANSI-C-Programm
Features:
(CLOS LOOP COMPILER CLISP ANSI-CL COMMON-LISP LISP=CL INTERPRETER SOCKETS GENERIC-STREAMS
 LOGICAL-PATHNAMES SCREEN FFI GETTEXT UNICODE BASE-CHAR=CHARACTER PC386 UNIX)
Installation directory: /usr/lib/clisp/
User language: ENGLISH
Machine: I686 (I686) csltok.swan.ac.uk [128.163.146.167]
   \endverbatim
   </li>
   <li> The installation has a sub-target "access-script":
    <ol>
     <li> This Bash-script is copied to ~/bin (overwriting existing
     files), under the name "Maxima", and it calls the maxima-program,
     passing parameters and loading all functions from the OKlibrary. </li>
     <li> All path-information is hardcoded into this script, and it is
     created from a template with "m4_SHELL"-use. </li>
     <li> Perhaps a different script is used for performing tests. </li>
    </ol>
   </li>
   <li> Move all maxima-documention-links from the Sage-documentation
   page to the Maxima documentation. </li>
  </ul>


  \todo DONE Install Maxima 5.13 on cs-wsok
  <ul>
   <li>
   \verbatim
Installations> mkdir Maxima
Installations> cd Maxima/
Maxima> tar -xzf ../../sources/Maxima/maxima-5.13.0.tar.gz
Maxima> cd maxima-5.13.0
maxima-5.13.0> ./configure --prefix=${OKplatform}/ExternalSources/Installations/Maxima
   \endverbatim
   yields, as above
   \verbatim
Compiling file /home/csoliver/SAT-Algorithmen/OKplatform/ExternalSources/Installations/Maxima/maxima-5.13.0/src/clmacs.lisp ...
*** - READ from
#<INPUT BUFFERED FILE-STREAM CHARACTER
  #P"/home/csoliver/SAT-Algorithmen/OKplatform/ExternalSources/Installations/Maxima/maxima-5.13.0/src/clmacs.lisp"
  @317>: #<PACKAGE CUSTOM> has no external symbol with name "*FLOATING-POINT-RATIONAL-CONTAGION-ANSI*"
The following functions were used but not defined:
 MAXIMA::MFUNCALL
0 errors, 0 warnings
make[1]: *** [binary-clisp/maxima.mem] Error 1
make[1]: Leaving directory `/home/csoliver/SAT-Algorithmen/OKplatform/ExternalSources/Installations/Maxima/maxima-5.13.0/src'
make: *** [all-recursive] Error 1
   \endverbatim
   again with cslisp version 2.33.
   </li>
   <li> After installation of clisp version 2.43
   \verbatim
maxima-5.13.0> ./configure --prefix=$OKPLATFORM/ExternalSources/Installations/Maxima
maxima-5.13.0> make
maxima-5.13.0> make check
maxima-5.13.0> make install
   \endverbatim
   </li>
   <li> Now under Maxima/bin one finds the executable, and under Maxima/share
   one finds the documentation. </li>
  </ul>


*/
