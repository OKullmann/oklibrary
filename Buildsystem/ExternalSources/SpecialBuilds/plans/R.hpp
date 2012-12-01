// Oliver Kullmann, 27.6.2007 (Swansea)
/* Copyright 2007, 2008, 2009, 2010, 2011, 2012 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/ExternalSources/SpecialBuilds/plans/R.hpp
  \brief Plans regarding installation of R


  \todo Failing tests
  <ul>
   <li> With version 2.15.2 and also previous versions we get the
   "make check" error as documented in the e-mail
   https://stat.ethz.ch/pipermail/r-devel/2012-October/065002.html
   to the R-devel list, without any answer. </li>
   <li> Per e-mail OK was informed that using
   "R_INSTALL_TAR=$(which tar)" should do the job, but that doesn't seem to
   help, at least not for version 2.15.2. </li>
   <li> Apparently Obj-C is used somewhere, so we also build this now with
   gcc. </li>
   <li> The following messages in the build-log seem suspicious:
   \verbatim
/usr/bin/ld: skipping incompatible /home/csoliver/OKplatform/ExternalSources/Installations/Gcc/4.5.4/lib/libgfortran.so when searching for -lgfortran
/usr/bin/ld: skipping incompatible /home/csoliver/OKplatform/ExternalSources/Installations/Gcc/4.5.4/lib/libgfortran.a when searching for -lgfortran
/usr/bin/ld: skipping incompatible /home/csoliver/OKplatform/ExternalSources/Installations/Gcc/4.5.4/lib/libgomp.so when searching for -lgomp
/usr/bin/ld: skipping incompatible /home/csoliver/OKplatform/ExternalSources/Installations/Gcc/4.5.4/lib/libgomp.a when searching for -lgomp
/usr/bin/ld: skipping incompatible /home/csoliver/OKplatform/ExternalSources/Installations/Gcc/4.5.4/lib/libgcc_s.so when searching for -lgcc_s
/usr/bin/ld: skipping incompatible /home/csoliver/OKplatform/ExternalSources/Installations/Gcc/4.5.4/lib/libgcc_s.so when searching for -lgcc_s
   \endverbatim
   Why that? </li>
   <li> The final error message is
   \verbatim
running code in 'reg-examples1.R' ... OK
running code in 'reg-examples2.R' ... OK
running code in 'reg-packages.R' ...make[4]: *** [reg-packages.Rout] Error 1
make[4]: Leaving directory `/home/csoliver/OKplatform/ExternalSources/builds/R/R-2.15.2/tests'
make[3]: *** [test-Reg] Error 2
   \endverbatim
   </li>
   <li> Disabling "make check" makes the build compiling, and apparently
   at least our functionality works. </li>
  </ul>


  \todo DONE (installed 2.14.0)
  Installing 2.12.1


  \todo DONE (building now with current gcc, that is, version 4.5.2, which
  has Gmp and Mpfr statically linked)
  Local installation of gfortran
  <ul>
   <li> Often gfortran is not installed. </li>
   <li> DONE (problems solved now)
   While it shouldn't be a problem to install gfortran locally.
    <ol>
     <li> DONE Just add "fortran" to gcc_enable_languages_okl. </li>
     <li> Yet we have a build-error; see "Local build of Fortran fails" in
     Buildsystem/ExternalSources/SpecialBuilds/plans/Gcc.hpp. </li>
    </ol>
   </li>
   <li> DONE (the final solution below seems alright)
   Employing the local gfortran:
    <ol>
     <li> Prefixing the configure-call with "F77=$(gfortran_call_okl)",
     we get the R-build-error
     \verbatim
checking whether mixed C/Fortran code can be run... configure: WARNING: cannot run mixed C/Fortran code
configure: error: Maybe check LDFLAGS for paths to Fortran libraries?
make: *** [R_base] Error 1
     \endverbatim
     (on csltok (Suse 11.3) and cs-wsok (Suse 10.0)). </li>
     <li> The R-installation also uses gcc, so also "CC=$(gcc_call_okl)"
     is needed; however this doesn't solve the problem. </li>
     <li> And apparently also C++ is used, so "CXX=$(gpp_call_okl)" is also
     needed; doesn't solve the problem. </li>
     <li> Using LDFLAGS="-Wl,-rpath=$(gcc_installation_dir_okl)/lib" should
     be appropriate, but doesn't solve the problem. We get
     \verbatim
checking for Fortran 77 libraries of /home/kullmann/OKplatform/ExternalSources/Installations/Gcc/4.2.4/bin/gfortran...  -L/home/kullmann/OKplatform/ExternalSources/Installations/Gcc/4.2.4/lib/gcc/x86_64-unknown-linux-gnu/4.2.4 -L/home/kullmann/OKplatform/ExternalSources/Installations/Gcc/4.2.4/lib/gcc/x86_64-unknown-linux-gnu/4.2.4/../../../../lib64 -L/lib/../lib64 -L/usr/lib/../lib64 -L/home/kullmann/OKplatform/ExternalSources/Installations/Gcc/4.2.4/lib/gcc/x86_64-unknown-linux-gnu/4.2.4/../../.. -lgfortranbegin -lgfortran -lm
     \endverbatim
     </li>
     <li> The configure-help gives as an example for using LDFLAGS
     "e.g. -L<lib dir>", so using LDFLAGS="-L $(gcc_installation_dir_okl)/lib"
     which yields
     \verbatim
checking for Fortran 77 libraries of /home/kullmann/OKplatform/ExternalSources/Installations/Gcc/4.2.4/bin/gfortran...  -L/home/kullmann/OKplatform/ExternalSources/Installations/Gcc/4.2.4/lib -L/home/kullmann/OKplatform/ExternalSources/Installations/Gcc/4.2.4/lib/gcc/x86_64-unknown-linux-gnu/4.2.4 -L/home/kullmann/OKplatform/ExternalSources/Installations/Gcc/4.2.4/lib/gcc/x86_64-unknown-linux-gnu/4.2.4/../../../../lib64 -L/lib/../lib64 -L/usr/lib/../lib64 -L/home/kullmann/OKplatform/ExternalSources/Installations/Gcc/4.2.4/lib/gcc/x86_64-unknown-linux-gnu/4.2.4/../../.. -lgfortranbegin -lgfortran -lm /home/kullmann/OKplatform/ExternalSources/Installations/Gcc/4.2.4/lib/libgfortran.a
     \endverbatim
     which looks okay, but the error stays the same. Same also when using
     "lib64" instead. </li>
     <li> There are examples using also "FC=${F77}", so well, not well
     specified, but can't hurt, so we also use it. </li>
     <li> The configure-call should be
     \verbatim
	F77=$(gfortran_call_okl) FC=$${F77} CC=$(gcc_call_okl) CXX=$(gpp_call_okl) LDFLAGS="-L $(gcc_installation_dir_okl)/lib" ./configure --prefix=$(R_install_directory_okl); $(postcondition) \
     \endverbatim
     </li>
     <li> The documentation says "However, if CC is gcc, the matching FORTRAN
     compiler (g77 for gcc 3 and gfortran for gcc 4) is used if available.".
     Does this mean F77 should not be specified? However then it doesn't
     find the Fortran-compiler; this is an error in the R-documentation (or
     in the R-build --- a *local* gcc is not recognised). </li>
     <li> The documentation speaks also about LD_LIBRARY_PATH "or your
     system's equivalent" ? However this is not mentioned in the
     configure-help-output, so it should be LDFLAGS. </li>
     <li> Ask on the R mailing list.
     System information:
     \verbatim
> version
platform       x86_64-unknown-linux-gnu     
arch           x86_64                       
os             linux-gnu                    
system         x86_64, linux-gnu            
status                                      
major          2                            
minor          11.0                         
year           2010                         
month          04                           
day            22                           
svn rev        51801                        
language       R                            
version.string R version 2.11.0 (2010-04-22)
     \endverbatim
     </li>
    </ol>
   </li>
   <li> DONE (we have one full gcc-4.1.2-installation now, and later we
   will have one full current-gcc-installation)
   Since we build gcc twice, there seem to be considerable overlaps;
   however once we move to newer gcc-versions, this will vanish: first we
   install gcc,g++ (only) with version 4.1.2, then with these gcc's we build
   (special) gmp, mpfr, and then using these gcc and these libraries we
   build the new gcc (in full, with java and fortran) and everything else.
   </li>
   <li> DONE (since we have the system-library-versions of gmp and mpfr now,
   no need that gcc builds it again)
   Better, of course, if we could just let the newer versions of gcc
   build their own gmp+mpfr (see "GCC 4.1.2 will not build on systems without
   GMP with MPFR support" in
   Buildsystem/ExternalSources/SpecialBuilds/plans/Gcc.hpp) --- then we could
   use the system-compiler to build the newest gcc (in full), while gcc in
   version 4.1.2 (with gmp) would only be there for backward-support of old
   C++-packages. </li>
  </ul>


  \bug DONE (doesn't show up on new laptop; might be language-related, since
  currently on the laptop also English is used, but so well)
  Can't output graphics
  <ul>
   <li> For example
   \verbatim
> hist(c(1,2,3,2,1))
Fehler in title(main = main, sub = sub, xlab = xlab, ylab = ylab, ...) :
  X11 Schrift -adobe-helvetica-%s-%s-*-*-%d-*-*-*-*-*-*-*, Typ 2 in Größe 14 konnte nicht geladen werden
   \endverbatim
   on csltok. </li>
   <li> Perhaps this is language-related, or related to the rather old version
   (9.2) of Suse-Linux running on csltok (it works on cs-wsok). </li>
  </ul>


  \todo Packages
  <ul>
   <li> We should supply additional packages. </li>
   <li> DONE (target R_packages)
   And also install them automatically (with documentation). </li>
   <li> Apparently with "library(Rcmdr)" we can open a (useful) graphical user
   interface, and apparently for this package "Rcmdr" is needed.
    <ol>
     <li> On cs-wsok we get the error
     \verbatim
> library(Rcmdr)
Loading required package: tcltk
Error in firstlib(which.lib.loc, package) :
  Tcl/Tk support is not available on this system
Error: package 'tcltk' could not be loaded
     \endverbatim
     though apparently everything is installed? </li>
     <li> On csltok we get messages on startup of Rcmdr that further
     packages (rgl, relimp, multcomp, lmtest, effects, car, aplpack, abind)
     should be installed; we should do this. </li>
    </ol>
   </li>
   <li> Of potential interest:
    <ol>
     <li> "gsl" http://cran.r-project.org/web/packages/gsl/index.html
      <ol>
       <li> With this we would get the exponential integral (and thus also
       the logarithmic integral). </li>
       <li> Needs the Gnu Gsl-library installed. </li>
       <li> Likely we should do this locally. </li>
       <li> But then, how to tell the R-installation about this local
       library? </li>
       <li> Using "oklib --R CMD INSTALL --help" reveals no options for
       libraries needed by the packages? </li>
       <li> Perhaps the usual configure-options are to be used, together
       with "--configure-args=ARGS" and "--configure-vars=VARS" (but unclear
       what these CMD-options mean)? </li>
      </ol>
     </li>
     <li> "SparseM" </li>
     <li> DONE "QCA" (Quine-McCluskey;
     http://cran.r-project.org/web/packages/QCA/index.html) </li>
     <li> DONE "lpsolve"
     http://cran.r-project.org/web/packages/lpSolve/index.html </li>
     <li> DONE
     "QCA3" (qualitative comparative analysis;
     http://cran.r-project.org/web/packages/QCA3/index.html) </li>
     <li> "boolean" </li>
     <li> "combinat" </li>
     <li> diagram http://r-forge.r-project.org/R/?group_id=342 </li>
     <li> "dyn" </li>
     <li> "gafit" </li>
     <li> "glpk" </li>
     <li> "graph" http://www.bioconductor.org/packages/2.5/bioc/html/graph.html
     </li>
     <li> "igraph" http://igraph.sourceforge.net/ </li>
     <li> "linprog" </li>
     <li> "pspline" </li>
     <li> "quadprog" </li>
     <li> "randaes" </li>
     <li> "Rgraphviz"
     http://www.bioconductor.org/packages/2.5/bioc/html/Rgraphviz.html </li>
     <li> "sudoku" </li>
    </ol>
   </li>
   <li> There should be also packages for machine learning. </li>
  </ul>


  \todo Improving the docus-page
  <ul>
   <li> Prerequisites:
    <ol>
     <li> What precisely is needed from the gcc-suite? </li>
     <li> What is mpfr? Is it needed still? In version 2.11.0 I can't find
     something on it anymore? </li>
    </ol>
   </li>
  </ul>


  \todo Building dvi,pdf,info-documentation
  <ul>
   <li> On csltok "make dvi" fails --- is this due to old
   tex-tools? </li>
   <li> Likely "make pdf" sits on top of dvi. </li>
   <li> What can we do with the info-documentation? </li>
   <li> How do we get documentation on the packages? </li>
  </ul>

*/

