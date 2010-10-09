// Oliver Kullmann, 7.10.2007 (Swansea)
/* Copyright 2007, 2008, 2009, 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/ExternalSources/SpecialBuilds/plans/Maxima.hpp
  \brief Plans regarding installation of Maxima


  \todo Installation of version 5.22.1
  <ul>
   <li> DONE Set problem:
    <ol>
     <li> The correct way of referring to the underlying order of a set is by
     orderlessp, not by <. </li>
     <li> This problem occurs likely at several places, but it's hard to search
     for them. </li>
     <li> So we just handle these errors as they come. </li>
    </ol>
   </li>
   <li> DONE Floating-point problem:
    <ol>
     <li> We get a test failure:
     variable_heuristics_tau([{1,2,3},{1,2,3,4}],identity) in
     ComputerAlgebra/Satisfiability/Lisp/Backtracking/tests/ConstraintSatisfaction.mac
     should yield [1,[1,2,3]]), however it yields (now) [2,[1,2,3,4]] ? </li>
     <li> It seems that this is due to a changed behaviour of "sort". </li>
     <li> No, this is not the problem, but that with 5.22.1 two equal
     tau-values now compare as "strictly less". </li>
     \verbatim
> oklib --maxima

m0 : log(3*4);
t1 : tau(m0 - [log(4),log(4),log(4)]);
  2.718281828459045
t2 : tau(m0 - [log(3),log(3),log(3),log(3)]);
  2.718281828459045
is (t1 < t2);
  false
is (t2 < t1);
  false
is (t1 = t2);
  true

m0p : log(3) + log(4);
t1 : tau(m0p - [log(4),log(4),log(4)]);
  2.718281828459045
t2 : tau(m0p - [log(3),log(3),log(3),log(3)]);
  2.718281828459045
is (t1 < t2);
  false
is (t2 < t1);
  false
is (t1 = t2);
  true

> maxima_recommended_version_number_okl=5.22.1 oklib --maxima
m0 : log(3*4);
t1 : tau(m0 - [log(4),log(4),log(4)]);
  2.718281828459045
t2 : tau(m0 - [log(3),log(3),log(3),log(3)]);
  2.718281828459045
is (t1 < t2);
  false
is (t2 < t1);
  false
is (t1 = t2);
  true

m0p : log(3) + log(4);
t1 : tau(m0p - [log(4),log(4),log(4)]);
  2.718281828459046
t2 : tau(m0p - [log(3),log(3),log(3),log(3)]);
  2.718281828459046
is (t1 < t2);
  false
is (t2 < t1);
  true
is (t1 = t2);
  false

     \endverbatim
     </li>
     <li> Actually, 5.22.1 seems to computes with more precision here:
     \verbatim
> oklib --maxima
t1 : tau([log(3),log(3),log(3)]);
  2.718281828459045
t2 : tau([log(4),log(4),log(4),log(4)]);
  2.718281828459045
is (t1 = t2);
  true
tau_hp([log(3),log(3),log(3)], 15);
  2.71828182845905b0
tau_hp([log(3),log(3),log(3)], 30);
  2.71828182845904523536028747135b0

> maxima_recommended_version_number_okl=5.22.1 oklib --maxima
t1 : tau([log(3),log(3),log(3)]);
  2.718281828459046
t2 : tau([log(4),log(4),log(4),log(4)]);
  2.718281828459046
is (t2 < t1);
  true
tau_hp([log(3),log(3),log(3)], 15);
  2.71828182845905b0
tau_hp([log(3),log(3),log(3)], 30);
  2.71828182845904523536028747135b0
     \endverbatim
     </li>
     <li> I think a reasonable solution is to compute more precise at
     Maxima-level, and so to use now for variable_heuristics_tau instead of
     sum_log_dom_size(dom) the value log_prod_dom_size(dom). </li>
    </ol>
   </li>
  </ul>


  \todo DONE (solved with Ecl 9.8.3)
  Test failures of Maxima 5.19.0 with ecl 9.8.3
  <ul>
   <li> We could ignore these errors (likely they don't concern us (yet)).
   </li>
   <li> There was also talk about a Maxima error discovered by the new
   Ecl version. So perhaps we wait for a Maxima-update. </li>
  </ul>


  \todo DONE (building succeeds now with Ecl version 9.8.2)
  Install Maxima with Ecl version 9.7.1
  <ul>
   <li> Installing Maxima using 9.7.1 fails in the same way as when using
   9.6.1; let's wait for Maxima 5.19. </li>
   <li> We get the build-error
   \verbatim
;;; Invoking external command:
;;;   /home/csoliver/SAT-Algorithmen/OKplatform/ExternalSources/Installations/Gcc/4.1.2/bin/gcc  -D_GNU_SOURCE -D_FILE_OFFSET_BITS=64 -g -O2 -fPIC -I/home/csoliver/SAT-Algorithmen/OKplatform/ExternalSources/Installations/Gmp/4.1.2/4.3.1/include -Dlinux -O "-I/home/csoliver/SAT-Algorithmen/OKplatform/ExternalSources/Installations/Ecl/9.6.1/include/" -w -c "/home/csoliver/SAT-Algorithmen/OKplatform/ExternalSources/builds/Maxima/ecl/maxima-5.18.1/src/binary-ecl/maxima-package.c" -o "/home/csoliver/SAT-Algorithmen/OKplatform/ExternalSources/builds/Maxima/ecl/maxima-5.18.1/src/binary-ecl/maxima-package.fas"
;;; Finished compiling /home/csoliver/SAT-Algorithmen/OKplatform/ExternalSources/builds/Maxima/ecl/maxima-5.18.1/src/maxima-package.lisp.
An error occurred during initialization:
Cannot find out entry point for binary file binary-ecl/maxima-package.o.
   \endverbatim
   </li>
   <li> With version 9.6.1 one has parameters
   \verbatim
--heap-size, --lisp-stack, --frame-stack, --c-stack
   \endverbatim
   How to use them? </li?
  </ul>


  \todo Install xgettext
  <ul>
   <li> To create new Maxima packages, xgettext, available at
   http://www.gnu.org/software/gettext/ , is needed </li>
  </ul>
  

  \todo Using other Lisps
  <ul>
   <li> CLisp is very slow, while Ecl has certain unprofessional aspects
   (inconsistent buildsystem, not reacting to e-mails). </li>
   <li> See "Installing other Lisp's" in
   Buildsystem/ExternalSources/SpecialBuilds/plans/Lisp.hpp. </li>
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


  \todo DONE Communicate with Maxima/Ecl to fix load behaviour with symlinks on
  path 
  <ul>
   <li> This is now fixed in Maxima 5.19.0:
   \verbatim
> mkdir Test
> ln -s `pwd`/Test TestSym
> cd TestSym
> tar zxvf ecl-9.8.3.tgz
> cd ecl-9.8.3
> mkdir install
> LDFLAGS=-Wl,-rpath=`pwd`/install/lib ./configure --prefix=`pwd`/install/
> make
> make install
> cd ..
> tar zxvf maxima-5.19.0.tar.gz
> cd maxima-5.19.0
> mkdir install
> LANG=C ./configure --prefix=`pwd`/install --with-ecl=`pwd`/../ecl-9.8.3/install/bin/ecl --enable-ecl && LANG=C make && make check && make install
> ./install/bin/maxima
> load(descriptive);
(%o1) /home/aeternus/Work/OKlibrary/OKlibTestSym/maxima-5.19.0/install/share/m\
axima/5.19.0/share/contrib/descriptive/descriptive.mac
   \endverbatim
   </li>
   <li> DONE Request by OK: is this now resolved with Maxima 5.19.0? </li>
   <li> The issue is that load-command fails (%e.g., "load(descriptive)").
   </li>
   <li> The problem occurs if one has a symbolic link on the path during the
   building of maxima and ecl. </li>
   <li> MG is communicating with the Maxima mailing list on the issue and
   hopefully this will be resolved in the next Maxima. </li>
   <li> For now, the symlink to OKplatform can simply be removed, as it is
   only a convenience. </li>
   <li> Additionally, if one wishes to use a symlink on the build path,
   the "file_search_maxima" variable may be "fixed" at runtime by using an
   old "hard-coded" version of a particular lisp function for listing the
   directories (Thanks to Robert Dodier):
   \verbatim
:lisp (setf (symbol-function 'share-subdirs-list) (symbol-function 'default-share-subdirs-list))
:lisp (set-pathnames)
   \endverbatim
   and then load functionality works fine. </li>
  </ul>


  \bug DONE (got it working with CLisp by using rmaxima, while that symbols
  must have been seen seems unavoidable)
  Inappropriate command-extension
  <ul>
   <li> Using the tab-key on csltok (32-bit, Suse 9.2), command-line extension
   happens using the existing Maxima- symbols. </li>
   <li> But including our own symbols does not happen after they have been
   included, but oklib_load_all(). </li>
   <li> On the other hand, once we used oklib_batch with a testobjects-file,
   then the included symbols are used! </li>
   <li> Is this related to "load" versus "batch"?
    <ol>
     <li> Considering for example "ohg". The extension "ohg_p" should be found.
     </li>
     <li> After oklib_load_all() this is not the case. </li>
     <li> Neither after
     oklib_load("OKlib/ComputerAlgebra/Hypergraphs/Lisp/Basics.mac"). </li>
     <li> But after
     oklib_batch("OKlib/ComputerAlgebra/Hypergraphs/Lisp/testobjects/Basics.mac")
     it works! </li>
     <li> And considering "sgn" ("sgnv2c" and "sgnvs2cs" should be found),
     still after
     oklib_load("OKlib/ComputerAlgebra/Matroids/Lisp/OrientedMatroids/SignVectors.mac")
     it is not available, though when using oklib_batch instead of oklib_load
     it works! </li>
     <li> So there appears to be something works when using the Maxima
     function "batch", but not with "load". </li>
     <li> Ah, the point is that once the symbol has been seen on the
     command line, then it's available (not before). So if we use somehow
     a symbol, from then on it's available not before. </li>
     <li> So perhaps here not much can be done? </li>
    </ol>
   </li>
   <li> The above is with Ecl.
    <ol>
     <li> With CLisp we get the behaviour, that only
     Lisp-symbols are considered, no Maxima-symbols, and no user-defined
     ones. So here something fundamental seems to go wrong (it never
     gets to incorporate Maxima- or user-defined functions). </li>
     <li> There is a command-line history, but this works only for the
     session, does not use the history from previous sessions (as we get
     it with Ecl). </li>
     <li> This is likely due to the fact that we use rmaxima with Ecl,
     but maxima with clisp. </li>
     <li> Ah, we should also use rmaxima here --- then it works!. </li>
   </li>
  </ul>


  \bug DONE
  Gnuplot not working anymore
  <ul>
   <li>
   \verbatim
(%i1) plot2d(sin(x),[x,-5,5])$
Xlib: connection to ":0.0" refused by server
Xlib: No protocol specified
gnuplot: unable to open display ':0.0'
gnuplot: X11 aborted.
   \endverbatim
   </li>
   <li> If the (currently introduced) redefinition of HOME (when calling
   Maxima) to the Maxima-installation-directory is removed, then it works
   again. </li>
   <li> Those two files apparently establishing that "gnuplot-pipe" are
   stored in the installation directory --- perhaps gnuplot can't find
   them? </li>
   <li> Setting plot_format to "gnuplot" (not "gnuplot_pipes", as it is
   the default) doesn't help (apparently with that no files are used?). </li>
   <li> And also "openmath" doesn't work anymore. </li>
   <li> Without HOME-resetting both alternative formats work (though with
   gnuplot the window doesn't stay open, but this seems to be "normal"). </li>
  </ul>

*/
