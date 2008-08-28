// Oliver Kullmann, 7.10.2007 (Swansea)
/* Copyright 2007, 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/ExternalSources/SpecialBuilds/plans/Maxima.hpp
  \brief Plans regarding installation of Maxima


  \bug CLisp doesn't use libsigsegv (though specified)
  <ul>
   <li> We should try Clisp 2.46
    <ol>
     <li> Seems alright on cs-wsok (warnings about non-portable linking between
     static and shared libraries, but uses the local libraries). </li>
     <li> Now needs to be tested on cs-oksvr: Same problem again! </li>
    </ol>
   </li>
   <li> Thus apparently we need to use another Lisp. </li>
  </ul>


  \todo Update to Maxima 5.16.x
  <ul>
   <li> With 5.16.3 now there appear no errors anymore. </li>
   <li> However, now the gf-package is very slow, such that
   "time oklib new_check" on cs-wsok goes up from
   \verbatim
real    4m1.167s
user    3m21.613s
sys     0m21.669s
   \endverbatim
   to
   \verbatim
real    5m18.519s
user    4m31.577s
sys     0m22.377s
   \endverbatim
   which seems solely due to the gf-package. </li>
   <li> Perhaps we should set "GF_IRREDUCIBILITY_CHECK : false".
    <ol>
     <li> With 5.15.0 the default value is "false". </li>
     <li> And actually it seems that whenever it is set to true, and
     gf-operations are performed, then it is reset to false!. This looks
     like a bug, which might be corrected in 5.16.3. </li>
    </ol>
   </li>
   <li> Perhaps also "largefield : false" should be used, but then we
   should not permanently reset the field.
    <ol>
     <li> With 5.15.0 the default value is "true". </li>
     <li> We should ask for the possibility to store and re-store actively
     the values computed by gf_set. </li>
     <li> Setting largefield to false results in 5.15.0 in the error
     "Use `fasttimes' only on CRE polynomials with same varlists" (when
     adding 0 to a polynomial). </li>
    </ol>
   </li>
  </ul>


  \todo Installing other Lisp's
  <ul>
   <li> SBCL http://www.sbcl.org/ looks alright (and maintained).
    <ol>
     <li> However, it needs another Lisp to be compiled! That excludes
     it. </li>
    </ol>
   </li>
   <li> ECL http://ecls.sourceforge.net/ again looks recent (and there were
   Maxima-related updates).
    <ol>
     <li>
     \verbatim
builds/Ecl> tar -xzf ../../sources/Ecl/ecl-0.9l.tgz
builds/Ecl> cd ecl-0.9l
Ecl/ecl-0.9l> LDFLAGS=-Wl,-rpath=/home/csoliver/SAT-Algorithmen/OKplatform/ExternalSources/Installations/Ecl/lib ./configure --prefix=/home/csoliver/SAT-Algorithmen/OKplatform/ExternalSources/Installations/Ecl --with-gmp-prefix=/home/csoliver/SAT-Algorithmen/OKplatform/ExternalSources/Installations/Gmp/4.2.3
Ecl/ecl-0.9l> make
Ecl/ecl-0.9l> make install
     \endverbatim
     seems alright (where by setting LDFLAGS we fix the path to the
     link-library needed for executing ecl. </li>
     <li> However the frontend doesn't understand any cursor key!
      <ol>
       <li> Rlwrap http://utopia.knoware.nl/~hlub/uck/rlwrap/ needs to be
       installed, and then rmaxima used. </li>
       <li>
       \verbatim
builds/Rlwrap> tar -xzf ../../sources/Maxima/rlwrap-0.30.tar.gz
builds/Rlwrap/rlwrap-0.30> ./configure --prefix=/home/csoliver/SAT-Algorithmen/OKplatform/ExternalSources/Installations/Rlwrap
/Rlwrap/rlwrap-0.30> make
/Rlwrap/rlwrap-0.30> make check
/Rlwrap/rlwrap-0.30> make install
OKplatform/bin> ln -s /home/csoliver/SAT-Algorithmen/OKplatform/ExternalSources/Installations/Rlwrap/bin/rlwrap
       \endverbatim
       </li>
       <li> However, currently rmaxima is broken since it just calls "maxima",
       i.e., it assumes system-wide installation.
        <ul>
         <li> So "maxima" in rmaxima has to be replaced by the full path. </li>
        </ul>
       </li>
      </ol>
     </li>
     <li> Building Maxima works now, but it seems that make_random_state
     is now broken (we get test failures for our tests)? </li>
    </ol>
   </li>
   <li> GCL http://savannah.gnu.org/projects/gcl seems a bit outdated, but
   we should try. </li>
   <li> CMUCL http://www.cons.org/cmucl/ looks also reasonable. </li>
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


  \todo DONE Installing libsigsegv and CLisp 2.45 locally
  <ul>
   <li> First trying to install clisp 2.45 system-wide with the existing
   build-script. </li>
   <li> The build fails immediately:
   \verbatim
cd /home/kullmann/csoliver/SAT-Algorithmen/OKplatform/ExternalSources/builds/CLisp/clisp-2.45; if [ $?!= 0 ]; then exit 1; fi; \
./configure --prefix=/home/kullmann/csoliver/SAT-Algorithmen/OKplatform/ExternalSources/Installations/CLisp/2.45 --build /home/kullmann/csoliver/SAT-Algorithmen/OKplatform/ExternalSources/builds/CLisp/clisp-2.45/oklib-build; if [ $? != 0 ]; then exit 1; fi; \
cd /home/kullmann/csoliver/SAT-Algorithmen/OKplatform/ExternalSources/builds/CLisp/clisp-2.45/oklib-build; if [ $? != 0 ]; then exit 1; fi; \
make; if [ $? != 0 ]; then exit 1; fi; \
make check; if [ $? != 0 ]; then exit 1; fi; \
make install; if [ $? != 0 ]; then exit 1; fi;
executing /home/kullmann/csoliver/SAT-Algorithmen/OKplatform/ExternalSources/builds/CLisp/clisp-2.45/src/configure --prefix=/home/kullmann/csoliver/SAT-Algorithmen/OKplatform/ExternalSources/Installations/CLisp/2.45 --build /home/kullmann/csoliver/SAT-Algorithmen/OKplatform/ExternalSources/builds/CLisp/clisp-2.45/oklib-build --cache-file=config.cache
configure: creating cache config.cache
checking for a BSD-compatible install... /usr/bin/install -c
checking whether build environment is sane... yes
checking for a thread-safe mkdir -p... /bin/mkdir -p
checking for gawk... gawk
checking whether make sets $(MAKE)... yes
configure: ** check for host type
checking build system type... Invalid configuration `/home/kullmann/csoliver/SAT-Algorithmen/OKplatform/ExternalSources/builds/CLisp/clisp-2.45/oklib-build': machine `/home/kullmann/csoliver/SAT-Algorithmen/OKplatform/ExternalSources/builds/CLisp/clisp-2.45/oklib' not recognized
configure: error: /bin/sh build-aux/config.sub /home/kullmann/csoliver/SAT-Algorithmen/OKplatform/ExternalSources/builds/CLisp/clisp-2.45/oklib-build failed
make: *** [clisp-2.45] Fehler 1
   \endverbatim
   </li>
   <li> Using "--cbc"" instead of "--build" seems to cure the problem. </li>
   <li> How to tell the clisp-installation about the local installation
   of libsigsegv ? </li>
   <li> Via "./configure --with-libsigsegv-prefix=/PATH-to_libsigsegv" </li>
   <li> Then how to tell the maxima-installation about the location of
   clisp? </li>
   <li> Apparently via "--with-clisp=<prog>" we specify the lisp-executable
   (the "maxima"-program), while by "--with-clisp-runtime=<path>" we specify
   the library file "lisp.run". </li>
  </ul>


  \todo Install CLisp : DONE
  <ul>
   <li> We should provide "oklib libsigsegv" (yet only system-wide), but
   documentation only as part of the installation of CLisp.
    <ol>
     <li> Perhaps the target "libsigsegv" is just realised as a sub-target
     in the installation-makefile for clisp. </li>
    </ol>
   </li>
   <li> Write configuration file for clisp DONE </li>
   <li> Write installation script for clisp. DONE
    <ol>
     <li> Apparently the clisp-maxima system is faulty and doesn't admit
     automatic installation. </li>
     <li> We keep the clisp.mak for the installation, and perhaps something
     changes in the future (and one can use it to install libsigsegv),
     however in the docu we give instruction on manual (system-wide)
     building. </li>
     <li> Apparently on cs-wsok one needs to use "ulimit -s 16384; make" ? </li>
     <li> Perhaps one should better use "ulimit -s 16384 -S; make", so that in
     case actually "ulimit -s" was already greater than 16384, it is not
     reduced?? Or should we just use "ulimit -s unlimited" ?? Or should
     we first find out whether "ulimit -s" yields a value which is at least
     16384 ?? The strange thing is that with "ulimit -s XXX" apparently the
     stacksize can actually only be reduced ??? </li>
     <li> And in order to compile for 64 bit on cs-wsok, apparently CC has to
     be set to "gcc -m64" ?? By using 'make CC="gcc -m64"' ?? </li>
    </ol>
   </li>
   <li> Write docu for installation of clisp. DONE </li>
   <li> Provide links to the documentation for clisp. DONE </li>
   <li> Put information on the main-external-sources-page. DONE </li>
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


  \todo Install clisp (version 2.43) on csltok: DONE (manual installation must suffice)
  <ul>
   <li> 
   \verbatim
Installations> mkdir CLisp
Installations> cd CLisp/
CLisp> tar -xjf ../../sources/Maxima/clisp-2.43.tar.bz2
CLisp> cd clisp-2.43/
clisp-2.43> ./configure
   \endverbatim
   results in
   \verbatim
  libsigsegv: no, consider installing GNU libsigsegv
./configure: libsigsegv was not detected, thus some features, such as
  generational garbage collection and
  stack overflow detection in interpreted Lisp code
cannot be provided.
Please do this:
  mkdir tools; cd tools; prefix=`pwd`/i686-pc-linux-gnu
  wget http://ftp.gnu.org/pub/gnu/libsigsegv/libsigsegv-2.5.tar.gz
  tar xfz libsigsegv-2.5.tar.gz
  cd libsigsegv-2.5
  ./configure --prefix=${prefix} && make && make check && make install
  cd ../..
  ./configure --with-libsigsegv-prefix=${prefix}
If you insist on building without libsigsegv, please pass
  --ignore-absence-of-libsigsegv
to this script:
  ./configure --ignore-absence-of-libsigsegv
   \endverbatim
   </li>
   <li> So apparently we need to install "libsigsegv" first. </li>
   <li> What about those "modules" ? It seems we don't need any optional
   module. </li>
  </ul>


  \todo Install clisp (version 2.43) on cs-wsok (system-wide) : DONE (same error as on csltok)
  <ul>
   <li> Same problem with libsigsegv. </li>
   <li> Installing libsigsegv (system-wide)
   \verbatim
Installations> tar -xzf ../sources/Maxima/libsigsegv-2.5.tar.gz
Installations> cd libsigsegv-2.5
libsigsegv-2.5> ./configure
libsigsegv-2.5> make 
libsigsegv-2.5> make check
libsigsegv-2.5> sudo make install
   \endverbatim
   </li>
   <li> Now try again to install clisp. 
   \verbatim
Installations> cd CLisp/
CLisp> tar -xjf ../../sources/Maxima/clisp-2.43.tar.bz2
CLisp>  cd clisp-2.43/
clisp-2.43> ./configure
   \endverbatim
   </li>
   yields the recommendation
   \verbatim
   cd src
   vi config.lisp
# The default stack size on your platform is insufficient
# and must be increased to at least 16384.  You must do either
# 'ulimit -s 16384' (for Bourne shell derivatives, e.g., bash and zsh)
# or 'limit stacksize 16384' (for C shell derivarives, e.g., tcsh)
   \endverbatim
   So well, looked into src/config.lisp, but didn't tell me much. The recommendation
   "ulimit -s 16384" is just for compilation??
   \verbatim
lisp-2.43/src> ulimit -s 16384
clisp-2.43/src> make
clisp-2.43/src> make check
clisp-2.43/src> make install
   \endverbatim
   </li>
   <li> Perhaps the above installation is not a 64-bit installation?
   One had to set CC to "gcc -m64" ?? </li>
  </ul>

*/
