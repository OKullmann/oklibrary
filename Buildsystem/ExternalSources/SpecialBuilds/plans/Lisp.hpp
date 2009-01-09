// Oliver Kullmann, 30.8.2008 (Swansea)
/* Copyright 2008, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/ExternalSources/SpecialBuilds/plans/Lisp.hpp
  \brief Plans regarding installation of Lisp compiler/interpreters othen then Ecl (the current default)


  \bug CLisp doesn't use libsigsegv (though specified)
  <ul>
   <li> According to http://www.clisp.org/impnotes/faq.html#faq-with-prefix
   the build of libsigsegv has to be adopted on 64-bit machines by adding
   "--libdir=='${exec_prefix}/lib64'" as configure-parameter:
    <ol>
     <li> For that we need a way to figure out whether the platform is 32-bit
     or 64-bit. </li>
     <li> Perhaps for 64-bit platforms we also need to distinguish between
     "bi-arch" and "mono-arch"? </li>
     <li> The above faq-link contains more information. </li>
     <li> The new libsigsegv-version is 2.6 --- perhaps this solves the problem
     already? </li>
    </ol>
   </li>
   <li> Isn't there a similar problem with libffcall ? </li>
   <li> We should try Clisp 2.46 : DONE (also with 2.47 the same problem)
    <ol>
     <li> Seems alright on cs-wsok (warnings about non-portable linking between
     static and shared libraries, but uses the local libraries). </li>
     <li> Now needs to be tested on cs-oksvr: Same problem again! </li>
    </ol>
   </li>
   <li> DONE (Ecl is now the default)
   Thus apparently we need to use another Lisp; let's see whether Ecl
   installs properly on all our systems. </li>
  </ul>


  \todo Installing Ecl : DONE
  <ul>
   <li> DONE (providing basic installation)
   Installation process:
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
     <li> DONE (provided rlwrap-installation)
     However the frontend doesn't understand any cursor key.
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
      </ol>
     </li>
    </ol>
   </li>
  </ul>


  \todo Improving the Ecl installation : DONE (moved to
  Buildsystem/ExternalSources/SpecialBuilds/plans/Ecl.hpp)
  <ul>
   <li> For tests.mak: How to get the version number from the ecl-binary?
   Ask on the Ecl mailing list. </li>
   <li> Documentation is not included in the package, so we need
   to download it. </li>
  </ul>


  \todo Installing other Lisp's
  <ul>
   <li> CMUCL http://www.cons.org/cmucl/ looks reasonable. </li>
   <li> SBCL http://www.sbcl.org/ looks alright (and maintained).
    <ol>
     <li> However, it needs another Lisp to be compiled! </li>
     <li> So let's try the other's first. </li>
    </ol>
   </li>
   <li> GCL http://savannah.gnu.org/projects/gcl seems a bit outdated, but
   we should try.
    <ol>
     <li> However, if the argument-length is really only 64, then it's
     not usable. </li>
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

