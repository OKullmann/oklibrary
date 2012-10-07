// Oliver Kullmann, 27.6.2007 (Swansea)
/* Copyright 2007, 2008, 2010, 2011, 2012 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/ExternalSources/SpecialBuilds/plans/Gcc.hpp
  \brief Plans regarding installation of gcc


  \todo DONE (not reproducible, taken as caused by some strange state of
  the computer)
  Failure building gcc412/gcc due to internal compiler error
  <ul>
   <li> These problems arise on csltok when trying to install the new package
   00140. </li>
   <li> Building gcc412 fails:
   \verbatim
/home/kullmann/OKplatform/OKplatform/ExternalSources/builds/Gcc/gcc-4.1.2/gcc/config/i386/i386.md: In function ‘bypass_p’:
/home/kullmann/OKplatform/OKplatform/ExternalSources/builds/Gcc/gcc-4.1.2/gcc/config/i386/i386.md:189:14: warning: comparison between ‘enum processor_type’ and ‘enum attr_cpu’
/home/kullmann/OKplatform/OKplatform/ExternalSources/builds/Gcc/gcc-4.1.2/gcc/config/i386/i386.md:189:14: warning: comparison between ‘enum processor_type’ and ‘enum attr_cpu’
... repeated more than 8000 times
gcc: Internal error: Killed (program cc1)
Please submit a full bug report.
See <http://bugs.opensuse.org/> for instructions.
make[3]: *** [insn-attrtab.o] Error 1
   \endverbatim
   </li>
   <li> Environment (csltok):
   \verbatim
kullmann-0:OKplatform> gcc --version
gcc (SUSE Linux) 4.5.1 20101208 [gcc-4_5-branch revision 167585]
Copyright (C) 2010 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
kullmann-0:OKplatform> uname -a
Linux csltok.swansea.ac.uk 2.6.39.3-0.5-desktop #1 SMP PREEMPT Sun Jul 31 02:04:11 BST 2011 x86_64 x86_64 x86_64 GNU/Linux
   \endverbatim
   </li>
   <li> One should install gcc version 4.5.3 on that machine. </li>
   <li> Another possibility would be to first install the local 4.5.3,
   and then using this compiler to install gcc412 --- this might actually
   be better, since it should be more likely that the newer build succeeds.
   </li>
   <li> Building gcc on csltok however also fails! We get
   \verbatim
gcc -c  -g -fkeep-inline-functions -DIN_GCC   -W -Wall -Wwrite-strings -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -Wmissing-format-attribute -pedantic -Wno-long-long -Wno-variadic-macros -Wno-overlength-strings -Wold-style-definition -Wc++-compat -fno-common  -DHAVE_CONFIG_H -I. -I. -I/home/kullmann/OKplatform/OKplatform/ExternalSources/builds/Gcc/gcc-4.5.3/gcc -I/home/kullmann/OKplatform/OKplatform/ExternalSources/builds/Gcc/gcc-4.5.3/gcc/. -I/home/kullmann/OKplatform/OKplatform/ExternalSources/builds/Gcc/gcc-4.5.3/gcc/../include -I/home/kullmann/OKplatform/OKplatform/ExternalSources/builds/Gcc/gcc-4.5.3/gcc/../libcpp/include -I/home/kullmann/OKplatform/OKplatform/ExternalSources/builds/Gcc/gcc-4.5.3_build/./gmp -I/home/kullmann/OKplatform/OKplatform/ExternalSources/builds/Gcc/gcc-4.5.3/gmp -I/home/kullmann/OKplatform/OKplatform/ExternalSources/builds/Gcc/gcc-4.5.3_build/./mpfr -I/home/kullmann/OKplatform/OKplatform/ExternalSources/builds/Gcc/gcc-4.5.3/mpfr -I/home/kullmann/OKplatform/OKplatform/ExternalSources/builds/Gcc/gcc-4.5.3/mpc/src  -I/home/kullmann/OKplatform/OKplatform/ExternalSources/builds/Gcc/gcc-4.5.3/gcc/../libdecnumber -I/home/kullmann/OKplatform/OKplatform/ExternalSources/builds/Gcc/gcc-4.5.3/gcc/../libdecnumber/bid -I../libdecnumber     insn-attrtab.c -o insn-attrtab.o
gcc: Internal error: Killed (program cc1)
   \endverbatim
   ???
   </li>
  </ul>


  \todo Gcj
  <ul>
   <li> gcj is the GNU %Compiler for Java, see http://gcc.gnu.org/java/ . </li>
   <li> It supports:
    <ul>
     <li> Compilation of .java to bytecode (.class files). </li>
     <li> Compilation of .java to linux executables and libraries. </li>
     <li> Compilation of .class to linux executables and libraries. </li>
    </ul>
   </li>
   <li> See also "Supporting Java" in
   Buildsystem/ExternalSources/SpecialBuilds/plans/general.hpp. </li>
   <li> gcj is part of the gcc package, and is built when calling "oklib gcc".
   </li>
   <li> gcj, as built by "oklib gcc", doesn't compile .java files:
    <ul>
     <li> Consider the following Java file (HelloWorld.java):
     \verbatim
class HelloWorld {
  public static void main(final String[] argv) {
    System.out.println("Hello world!");
  }
}
     \endverbatim
     </li>
     <li> Compiling "HelloWorld.java":
     \verbatim
> $OKPLATFORM/ExternalSources/Installations/Gcc/4.5.3/bin/gcj HelloWorld.java -o HelloWorld --main=HelloWorld
     \endverbatim
     yields the following error message:
     \verbatim
gcj: error trying to exec 'ecj1': execvp: No such file or directory
     \endverbatim
     </li>
     <li> The result should be to compile HelloWorld.java to a standalone executable:
     \verbatim
# Using system Gcj
> gcj HelloWorld.java -o HelloWorld --main=HelloWorld
> ./HelloWorld
Hello world!
     \endverbatim
     </li>
     <li> gcj, as of gcc-3.4, requires ecj to be downloaded before building;
     see "--with-ecj-jar" at http://gcc.gnu.org/install/configure.html and also
     in ExternalSources/builds/Gcc/gcc-4.5.3/INSTALL/configure.html:
     \verbatim
--with-ecj-jar=filename
    This option can be used to specify the location of an external jar file containing the Eclipse Java compiler. A specially modified version of this compiler is used by gcj to parse .java source files. If this option is given, the 'libjava' build will create and install an ecj1 executable which uses this jar file at runtime.

    If this option is not given, but an ecj.jar file is found in the topmost source tree at configure time, then the 'libgcj' build will create and install ecj1, and will also install the discovered ecj.jar into a suitable place in the install tree.

    If ecj1 is not installed, then the user will have to supply one on his path in order for gcj to properly parse .java source files. A suitable jar is available from ftp://sourceware.org/pub/java/.
     \endverbatim
     </li>
     <li> The GCC source package includes a script "contrib/download_ecj" to
     download ecj. This could be called before calling ./configure in the GCC
     build process, which would result in GCJ being built with support for
     .java files. </li>
     <li> ecj is the Eclipse Java compiler, and is under the Eclipse Public
     License. The difference in license is why ecj must be downloaded
     separately. </li>
     <li> The difference in licensing raises the issue of whether we want to
     compile gcj with support for .java files. </li>
    </ul>
   </li>
  </ul>


  \todo Providing gcc 4.1.2
  <ul>
   <li> Special 412-targets:
    <ol>
     <li> Programs needing gcc version 4.1.2 might also need the libraries,
     and so actually we might better keep the newly introduced 412-targets
     (which yet are flagged as "to be removed"). </li>
     <li> These targets are boost412, bzip2412, gmp412. </li>
     <li> We should also add mpfr412, and mhash412 (once updated). </li>
    </ol>
   </li>
   <li> DONE
   Motivated by "GRASP cannot be compiled with gcc version 4.3" (see
   Buildsystem/ExternalSources/SpecialBuilds/plans/SAT.hpp), we should provide
   a permanent installation of version 4.1.2. </li>
   <li> DONE
   Perhaps we should provide in general build-variables supporting
   usage of gcc (i.e., calling the compiler, and providing the instructions
   for fixing the shared libraries in the binaries. </li>
   <li> DONE (see 'Putting "configure" under our version control')
   Changed configuration file sources/Gcc/configure-4.1.2.gz:
    <ol>
     <li> On 18.6.2008 this changed file was introduced according to MG;
     ID 4494618f9a8b506adc41934941c81a7554e0c3f6. </li>
     <li> Documentation needs to be added by MG on these changes. </li>
     <li> If we compare it with the most recent gcc-4.1.2.tar.bz2 (with
     md5sum fad77c542b5f467255435ffbeed5ef51; this is the package as
     introduced by MG), then we get
     \verbatim
> diff configure gcc-4.1.2/configure
3552c3552
<        | egrep 'texinfo[^0-9]*(4\.([4-9]|[1-9][0-9])|[5-9]|[1-9][0-9])' >/dev/null 2>&1; then
---
>        | egrep 'texinfo[^0-9]*([1-3][0-9]|4\.[2-9]|[5-9])' >/dev/null 2>&1; then
     \endverbatim
     What is the meaning of these changes? </li>
     <li> From a certain version of texinfo (makeinfo etc) onwards, the precise
     format of the version number altered, and even though I had a
     new enough version of texinfo, without this patch, GCC will error during
     it's configuration step, complaining that I did not have a new enough
     version of texinfo. Applying the patch fixes this issue. See
     http://www.mail-archive.com/bug-binutils@gnu.org/msg04179.html and
     http://code.google.com/p/buildroot/source/browse/branches/john-avr32-buildroot/toolchain/binutils/2.18/100-makeinfo-version.patch?spec=svn705&r=705 .
     </li>
    </ol>
   </li>
   <li> DONE (see 'Putting "configure" under our version control')
   Gcc 4.1.2 will not link on some 64 bit systems
    <ul>
     <li> Gcc 4.1.2 assumes that 32 bit libraries are stored
     in /usr/lib /lib etc rather than /usr/lib32 /lib32 etc. </li>
     <li> This is the case for most linux systems following the
     LSB (Linux Standard Base) conventions however debian-based
     distributions use the later convention. This results
     in the linker not finding the libraries during the building
     of gcc. </li>
     <li> This has been fixed in later versions of gcc, and there
     is a patch available (MG: The patch works for me). </li>
     <li> See http://www.trevorpounds.com/blog/?tag=ubuntu for
     details and a link to the patch. </li>
     <li> An updated version of the GCC package with the patch applied is
     available at http://cs.swan.ac.uk/~csmg/gcc-4.1.2.tar.bz2 , and was
     generated by replacing gcc-4.1.2/gcc/config/i386/t-linux64 with
     the following file (called "t-linux64-new")
     \verbatim
# On x86-64 we do not need any exports for glibc for 64-bit libgcc_s,
# override the settings
# from t-slibgcc-elf-ver and t-linux
SHLIB_MAPFILES = $(srcdir)/libgcc-std.ver \
		 $(srcdir)/config/i386/libgcc-x86_64-glibc.ver


# On Debian, Ubuntu and other derivitive distributions, the 32bit libraries
# are found in /lib32 and /usr/lib32, /lib64 and /usr/lib64 are symlinks to
# /lib and /usr/lib, while other distributions install libraries into /lib64
# and /usr/lib64.  The LSB does not enforce the use of /lib64 and /usr/lib64,
# it doesn't tell anything about the 32bit libraries on those systems.  Set
# MULTILIB_OSDIRNAMES according to what is found on the target.

MULTILIB_OPTIONS = m64/m32
MULTILIB_DIRNAMES = 64 32
MULTILIB_OSDIRNAMES = ../lib64 $(if $(wildcard $(shell echo $(SYSTEM_HEADER_DIR))/../../usr/lib32),../lib32,../lib)


LIBGCC = stmp-multilib
INSTALL_LIBGCC = install-multilib

EXTRA_MULTILIB_PARTS=crtbegin.o crtend.o crtbeginS.o crtendS.o \
		     crtbeginT.o crtfastmath.o

# The pushl in CTOR initialization interferes with frame pointer elimination.
# crtend*.o cannot be compiled without -fno-asynchronous-unwind-tables,
# because then __FRAME_END__ might not be the last thing in .eh_frame
# section.
CRTSTUFF_T_CFLAGS = -fno-omit-frame-pointer -fno-asynchronous-unwind-tables
     \endverbatim
     which has the following diff with the original
     \verbatim
6a7,14
>
> # On Debian, Ubuntu and other derivitive distributions, the 32bit libraries
> # are found in /lib32 and /usr/lib32, /lib64 and /usr/lib64 are symlinks to
> # /lib and /usr/lib, while other distributions install libraries into /lib64
> # and /usr/lib64.  The LSB does not enforce the use of /lib64 and /usr/lib64,
> # it doesn't tell anything about the 32bit libraries on those systems.  Set
> # MULTILIB_OSDIRNAMES according to what is found on the target.
>
8,9c16,18
< MULTILIB_DIRNAMES = 64 32
< MULTILIB_OSDIRNAMES = ../lib64 ../lib
---
> MULTILIB_DIRNAMES = 64 32
> MULTILIB_OSDIRNAMES = ../lib64 $(if $(wildcard $(shell echo $(SYSTEM_HEADER_DIR))/../../usr/lib32),../lib32,../lib)
>
     \endverbatim
     One can see that the change allows the GCC build system to detect
     whether /lib is used for 64 bit libraries or 32 bit libraries
     </li>
     <li> Therefore, assuming that the above file (t-linux64-new - md5sum =
     0aae4a50588d97920b7f8ee96789550d ) is in the
     ExternalSources/sources/Gcc directory, and the original (pre-MG
     changes) gcc-4.1.2 tarball is called "gcc-4.1.2.tar.bz2" (md5sum
     a4a3eb15c96030906d8494959eeda23c) in the same
     directory, then we have the following procedure for generating the new
     gcc tarball:
     \verbatim
sources> mv gcc-4.1.2.tar.bz2 gcc-4.1.2.tar.bz2_ALT
sources> tar jxvf gcc-4.1.2.tar.bz2_ALT
sources> cp t-linux64-new gcc-4.1.2/gcc/config/i386/t-linux64
sources> tar jcvf gcc-4.1.2.tar.bz2 gcc-4.1.2
     \endverbatim
     Note, the md5sum will not be the same as the original as the
     creation and modification time etc for t-linux64-new will be different.
     </li>
     <li> Of course, the question is whether this patch is really a general
     improvement, and whether it creates problems with other systems? </li>
     <li> Those who are interested in this patch need to test it on other
     systems. </li>
     <li> And precise information on how to perform the improvements are
     needed (note that we are using already an altered gcc!). </li>
    </ul>
   </li>
   <li> We need information about gcc-4.1.2 in the installation page. </li>
   <li> DONE (in the FAQ)
   We need also general information on why we need gcc-4.1.2 etc. </li>
  </ul>


  \todo Install GCC 4.5.3 : DONE
  <ul>
   <li> DONE (building it, requiring zip)
   gcj
    <ol>
     <li> Regarding gcj on csoberon we get the error
     \verbatim
checking for unzip... /usr/bin/unzip
Illegal option: @
Usage: jar {ctxui}[vfm0Me] [jar-file] [manifest-file] [entry-point] [-C dir] files ...
     \endverbatim
     with configure for gcj. No zip is available on this machine, which perhaps
     makes the difference (on the other machines we have it). Looks like a
     gcc-bug; contacted the gcc mailing-list. </li>
     <li> For now we disable gcj (don't need it now). </li>
     <li> But we can simply add "zip" to the requirement. </li>
    </ol>
   </li>
   <li> DONE (we require zip)
   We need to install zip locally, and provide it to the gcc-build.
    <ol>
     <li> Building it shouldn't be a problem. </li>
     <li> Gcc doesn't make provisions for that. </li>
     <li> So apparently one has to modify the path when installing gcc? </li>
     <li> Alternatively we could put it into OKplatform/bin. </li>
     <li> However this wouldn't solve the provision-problem, since (at least
     yet) we say that putting OKplatform/bin on PATH isn't required. </li>
    </ol>
   </li>
   <li> DONE (we require now also zlib, and use this)
   Build error on cs-oksvr:
    <ol>
     <li> We get (for "oklib gcc")
     \verbatim
checking dynamic linker characteristics... configure: error: Link tests are not allowed after GCC_NO_EXECUTABLES.
make[3]: *** [configure-stage1-zlib] Error 1
     \endverbatim
     </li>
     <li> It seems that is a bug in the gcc build (fixed hopefully in the next
     version). </li>
     <li> A proposal on the gcc mailing-list is to use --with-system-zlib.
     </li>
     <li> This can be done by
     \verbatim
ExternalSources> oklib gcc gcc_user_options_okl="--with-system-zlib"
     \endverbatim
     </li>
    </ol>
   </li>
  </ul>


  \todo Loop-optimisation
  <ul>
   <li> We should also install these additional libraries "PPL" ("Parma
   Polyhedra Library") and "CLooG" for loop-optimisation. </li>
  </ul>


  \todo DONE (gcc-building provided first a corrected version, which was then
  removed)
  texi2dvi (texinfo 1.13a) fails to build gcc.texi on some systems
  <ul>
   <li> Texinfo version 1.13a has a bug due to the formatting of strings
   passed to egrep. </li>
   <li> This bug results in an error from texi2dvi during the gcc
   build process on systems using the en_GB.UTF-8 locale and
   versions of (e)grep >= 2.7. </li>
   <li> This bug was first found to affect the OKlibrary in Dec 2010, and a
   corrected version of texi2dvi was made available on 22.8.2011. </li>
   <li> DONE (correction applied in gcc-building) According to
   http://www.mail-archive.com/bug-texinfo@gnu.org/msg04519.html
   and the corresponding bug report, there is now a fix, and presumably
   this should be available in a new texinfo package soon on most systems.
   </li>
   <li> DONE (we should avoid such hacks if possible, and here it is)
   One can run "LC_ALL=C oklib gcc" and the build completes without a
   problem. </li>
   <li> DONE Request to specify which version of texi2dvi this was about; and
   also, for such issues dates need to be stated. </li>
   <li> DONE (gcc-building provides now a corrected version)
   This is not a bug in the OKlibrary, but users should be aware of the
   issue. </li>
  </ul>


  \bug DONE (no system-libraries needed anymore)
  Local Gmp/Mpfr are not used (appropriately)
  <ul>
   <li> The system-subdirectories of Gmp/Mpfr installation directories
   should not sit inside the directory with the gcc-version on the path ---
   the access to the system-version must be independent of the recommended
   gcc-version. </li>
  </ul>


  \bug DONE (the internal build of Gmp+Mpfr does static linking; dynamic
  linking would be possible using the additional information from the gcc
  mailing list)
  How to use local Gmp + Mpfr?
  <ul>
   <li> Just using "--with-gmp" and "--with-mpfr" is not enough, but some
   linking information is needed --- only which? </li>
   <li> Apparently LD_LIBRARY_PATH is to be used?
   \verbatim
checking whether the GNU Fortran compiler is working... no
configure: error: GNU Fortran is not working; the most common reason for that is that you might have linked it to shared GMP and/or MPFR libraries, and not set LD_LIBRARY_PATH accordingly. If you suspect any other reason, please report a bug in http://gcc.gnu.org/bugzilla, attaching /home/kullmann/OKplatform/ExternalSources/builds/Gcc/gcc-4.2.4_build/x86_64-unknown-linux-gnu/libgfortran/config.log
   \endverbatim
   </li>
   <li> However prefixing the configure-call by
   LD_LIBRARY_PATH="$(gmp_locsys_install_directory_okl)/lib:$(mpfr_locsys_install_directory_okl)/lib"
   has apparently no effect? </li>
   <li> In gcc-4.2.4_build/x86_64-unknown-linux-gnu/libgfortran/config.log
   we find the usual f951-problem:
   \verbatim
/home/kullmann/OKplatform/ExternalSources/builds/Gcc/gcc-4.2.4_build/./gcc/f951: error while loading shared libraries: libmpfr.so.4: cannot open shared object file: No such file or directory
   \endverbatim
   </li>
   <li> The library-file libmpfr.so.4 is in the Mpfr-lib-directory:
   \verbatim
kullmann-0:lib> pwd
/home/kullmann/OKplatform/ExternalSources/Installations/Mpfr/system/3.0.0/lib
kullmann-0:lib> ls -l
total 1256
-rw-r--r-- 1 kullmann users 896598 2010-12-26 21:56 libmpfr.a
-rwxr-xr-x 1 kullmann users   1165 2010-12-26 21:56 libmpfr.la
lrwxrwxrwx 1 kullmann users     16 2010-12-26 21:56 libmpfr.so -> libmpfr.so.4.0.0
lrwxrwxrwx 1 kullmann users     16 2010-12-26 21:56 libmpfr.so.4 -> libmpfr.so.4.0.0
-rwxr-xr-x 1 kullmann users 381897 2010-12-26 21:56 libmpfr.so.4.0.0
   \endverbatim
   So apparently LD_LIBRARY_PATH is just ignored. </li>
   <li> Tryin
   \verbatim
LDFLAGS="-L $(gmp_locsys_install_directory_okl)/lib -L $(mpfr_locsys_install_directory_okl)/lib"
   \endverbatim
   instead. But again this is just ignored. </li>
   <li> Finally trying
   \verbatim
LDFLAGS="$(gmp_locsys_link_path_okl) $(mpfr_locsys_link_path_okl)"
   \endverbatim
   And again, this is just ignored. Exactly the same f951-error as above. The
   configure script continued with some reports, and contains the line
   "LDFLAGS=''", which seems a clear indication of a error in the
   build-script. </li>
  </ul>


  \bug DONE (works now; problems were Gmp,Mpfr related)
  Local build of Fortran fails (for 4.1.2)
  <ul>
   <li> DONE (we don't build Fortran with 4.1.2)
   We get (for "oklib gcc") an error when building
   libgfortran:
   \verbatim
/home/kullmann/OKplatform/ExternalSources/builds/Gcc/gcc-4.1.2/libgfortran/mk-kinds-h.sh: Unknown type
grep '^#' < kinds.h > kinds.inc
/bin/sh: kinds.h: No such file or directory
make[3]: *** [kinds.inc] Error 1
make[3]: Leaving directory `/home/kullmann/OKplatform/ExternalSources/builds/Gcc/gcc-4.1.2_build/x86_64-unknown-linux-gnu/libgfortran'
make[2]: *** [all-target-libgfortran] Error 2
   \endverbatim
   </li>
   <li> The error message "unknown type" is from the script
   libgfortran/mk-kinds-h.sh. This is an example of a bad error-message:
    <ol>
     <li> There are two possible places for this output, without a possibility
     to distinguish them. </li>
     <li> They do not show the bad value (some size) under consideration. </li>
    </ol>
   </li>
   <li> Inserting output, it seems it comes from "case $largest_ctype",
   where apparently the value of largest_ctype is the empty string. </li>
   <li> When compiling
   \verbatim
libgfortran> more tmp17093.f90
  real (kind=16) :: x
  end
   \endverbatim
   one gets apparently an error (where there shouldn't be one):
   \verbatim
libgfortran> /home/kullmann/OKplatform/ExternalSources/builds/Gcc/gcc-4.1.2_build/./gcc/gfortran -c tmp17093.f90
gfortran: error trying to exec 'f951': execvp: No such file or directory
   \endverbatim
   and thus largest_ctype is not re-set. </li>
   <li> We get the same error on cs-wsok (Suse 10.0). </li>
   <li> An Internet-search is needed: It seems that the build has linking
   problems with Mpfr (see
   http://lists.linuxtogo.org/pipermail/openembedded-issues/2007-November/006519.html
   ). There is other noise denying (as usual) any fault on the side of gcc,
   and blaming Gmp, recommending to use
     --host=none --target=none --build=none
   to disable platform-specific optimization (which wouldn't matter much here,
   since these libraries are only used at compile-time), however this doesn't
   seem likely here to me. </li>
   <li> Let's try later gcc-versions --- perhaps the problem has been solved
   there (we could use later gcc-versions for Fortran (only)).
    <ol>
     <li> DONE (actually this was only due to not using local Gmp/Mpfr; see
     above)
     Version 4.2.4 seems to install without problems (including gfortran).
     </li>
     <li> However yet we can't use it; see "Local installation of gfortran"
     in Buildsystem/ExternalSources/SpecialBuilds/plans/R.hpp. </li>
     <li> It is likely an R-build-problem, but we need to check. </li>
    </ol>
   </li>
   <li> The newest version 3.0.0 of Mpfr doesn't seem to make a difference.
   </li>
   <li> Also without the library-settings for gmp-/mpfr we get the same
   error (on csltok; must have gone unnoticed before), so it seems it is
   not related to these additional settings. </li>
   <li> DONE (everything is now built with the system-gcc)
   Perhaps a link-failure: Now we compile gfortran with 4.1.2, while
   the system-compiler is 4.5.0 --- perhaps building gfortran links to
   something compiled with 4.5.0 ? </li>
   <li> DONE (we have now system-versions of gmp and mpfr)
   However it seems we need to build with (our) 4.1.2, since we need to
   use gmp and mpfr (which are build with (our) gcc 4.1.2.) ? </li>
   <li> We need to find out what are the precise requirements when building
   gfortran (version 4.1.2). </li>
   <li> DONE (handled now in this way)
   An alternative procedure would be to first build gcc-4.1.2 (only C and
   C++ compiler) with the system-compiler, building then gmp amd mpfr with the
   system-compiler as "system"-versions, and then building gfortran-4.1.2
   (as well as the current full gcc-suite) with the system-compiler, using
   these gmp and mpfr. </li>
   <li> Then for gmp,mpfr we would need three versions: the system-version,
   the 4.1.2-version, the current version. So well, seems appropriate. </li>
   <li> DONE
   Then actually gmp and mpfr should be build first, and then all of
   gcc can be build at once --- no need to separate the fortran-build! </li>
  </ul>


  \todo DONE (moved files into Programming/Specifics/Gcc/412)
  Putting "configure" under our version control
  <ul>
   <li> Since there are now various changes to various gcc-configuration files,
   we should put them under our version control. </li>
   <li> Starting with the files in the original gcc-4.1.2-package. </li>
   <li> It seems that the md5sum-value of the gcc-package is for the original
   package, then the new general configure-file was introduced, and then
   the special configuration-file as discussed below. (It seems these were
   the only changes.) </li>
   <li> We handle it then as with for example Ubcsat: the package is the
   original, and during the build the new files are placed. </li>
   <li> The appropriate place for the gcc-replacement files seems
   Programming/SystemSpecifics/Gcc. </li>
   <li> At this time Programming/SystemSpecifics/plans/milestones.hpp
   should be updated. </li>
   <li> The Externalsources/sources/gcc-4.1.2.tar.bz2 is now the original
   from the Gcc site with md5sum a4a3eb15c96030906d8494959eeda23c. </li>
  </ul>


  \todo DONE (solved now --- with build 4.1.2 only for C,C++)
  GCC 4.1.2 will not build on systems without GMP with MPFR support
  <ul>
   <li> DONE (we built gmp and mpfr locally, and compile Fortran then; needs
   to be checked though)
   GCC 4.1.2 fails to build on some machines (notably the Swansea
   University Computer Science Linux lab machines), with the following error:
   \verbatim
checking whether the C compiler (gcc  ) is a cross-compiler... no
checking whether we are using GNU C... yes
checking whether gcc accepts -g... yes
checking for gnatbind... gnatbind
checking whether compiler driver understands Ada... no
checking how to compare bootstrapped objects... cmp --ignore-initial=16 $$f1 $$f2
checking for correct version of gmp.h... yes
checking for MPFR... no
configure: error: GMP with MPFR support is required to build fortran
make: *** [gcc] Error 1
make: Leaving directory `/tmp/OKlib/OKplatform/ExternalSources'
   \endverbatim
   </li>
   <li> DONE (at the moment we just have everything built with "our" gcc
   version 4.1.2; later we will have this and the builds with our newest
   version)
   Ideally we could just build GMP and MPFR as part of the OKlibrary
   build process, however, these libraries would then build using a newer
   version of GCC and we run the risk of there being incompatibilities
   with the version we are building when we start to compile anything in
   the library which might somehow link to standard libraries. </li>
   <li> The GCC website (http://gcc.gnu.org/install/prerequisites.html)
   suggests it should be possible to simply drop GMP and MPFR source
   directories in the GCC source directory and build as normal :
   \verbatim
GNU Multiple Precision Library (GMP) version 4.3.2 (or later)
    Necessary to build GCC. If you do not have it installed in your library search path, you will have to configure with the --with-gmp configure option. See also --with-gmp-lib and --with-gmp-include. Alternatively, if a GMP source distribution is found in a subdirectory of your GCC sources named gmp, it will be built together with GCC.
MPFR Library version 2.4.2 (or later)
    Necessary to build GCC. It can be downloaded from http://www.mpfr.org/. The --with-mpfr configure option should be used if your MPFR Library is not installed in your default library search path. See also --with-mpfr-lib and --with-mpfr-include. Alternatively, if a MPFR source distribution is found in a subdirectory of your GCC sources named mpfr, it will be built together with GCC.
   \endverbatim
   </li>
   <li> However, building GCC with GMP and MPFR source directories "in-tree"
   (as such a method is called) with GCC 4.1.2 doesn't work yielding the same
   error. </li>
   <li> Either additional configure options are needed, or GCC 4.1.2 doesn't
   support this. Perhaps such functionality has only been introduced in later
   GCC versions?
   </li>
  </ul>


  \todo DONE (we go directly to 4.5.2, to minimise problems with additional
  libraries)
  Install GCC 4.2.4
  <ul>
   <li> First only as an alternative (since yet code doesn't compile with
   versions 4.2 or later). </li>
   <li> Installation seems to work, however not for gfortran with local
   Gmp+Mpfr (see above). </li>
  </ul>


  \todo DONE (we go directly to 4.5.2, to minimise problems with additional libraries)
  Install GCC 4.3.5


  \todo DONE (we go directly to 4.5.2, to minimise problems with additional libraries)
  Install GCC 4.4.5
  <ul>
   <li> DONE (we use 4.4.5, and we build local Gmp+Mpfr using system-gcc)
   Building 4.4.4:
    <ol>
     <li> Now Gmp and Mpfr is needed. </li>
     <li> For Gmp just use the configure-option
     "--with-gmp=$(gmp_installation_dir_okl)". </li>
     <li> However, then we have the problem of a circular dependency, since
     these libraries are built using Gcc! </li>
     <li> The main question is about the link-library-compatabilities. </li>
    </ol>
   </li>
   <li> Building error (on csltok):
    <ol>
     <li> We get
     \verbatim
ExternalSources> oklib gcc gcc_enable_languages_okl="c,c++,fortran" gcc_recommended_version_number_okl=4.4.5

rm gcc.pod gfortran.pod
make[4]: Leaving directory `/home/kullmann/OKplatform/ExternalSources/builds/Gcc/gcc-4.4.5_build/gcc'
mkdir -p -- x86_64-unknown-linux-gnu/libgcc
Checking multilib configuration for libgcc...
Configuring stage 2 in x86_64-unknown-linux-gnu/libgcc
configure: creating cache ./config.cache
checking for --enable-version-specific-runtime-libs... no
checking for a BSD-compatible install... /usr/bin/install -c
checking for gawk... gawk
checking build system type... x86_64-unknown-linux-gnu
checking host system type... x86_64-unknown-linux-gnu
checking for x86_64-unknown-linux-gnu-ar... ar
checking for x86_64-unknown-linux-gnu-lipo... lipo
checking for x86_64-unknown-linux-gnu-nm... /home/kullmann/OKplatform/ExternalSources/builds/Gcc/gcc-4.4.5_build/./gcc/nm
checking for x86_64-unknown-linux-gnu-ranlib... ranlib
checking for x86_64-unknown-linux-gnu-strip... strip
checking whether ln -s works... yes
checking for x86_64-unknown-linux-gnu-gcc... /home/kullmann/OKplatform/ExternalSources/builds/Gcc/gcc-4.4.5_build/./gcc/xgcc -B/home/kullmann/OKplatform/ExternalSources/builds/Gcc/gcc-4.4.5_build/./gcc/ -B/home/kullmann/OKplatform/ExternalSources/Installations/Gcc/4.4.5/x86_64-unknown-linux-gnu/bin/ -B/home/kullmann/OKplatform/ExternalSources/Installations/Gcc/4.4.5/x86_64-unknown-linux-gnu/lib/ -isystem /home/kullmann/OKplatform/ExternalSources/Installations/Gcc/4.4.5/x86_64-unknown-linux-gnu/include -isystem /home/kullmann/OKplatform/ExternalSources/Installations/Gcc/4.4.5/x86_64-unknown-linux-gnu/sys-include
checking for suffix of object files... configure: error: in `/home/kullmann/OKplatform/ExternalSources/builds/Gcc/gcc-4.4.5_build/x86_64-unknown-linux-gnu/libgcc':
configure: error: cannot compute suffix of object files: cannot compile
See `config.log' for more details.
make[3]: *** [configure-stage2-target-libgcc] Error 1
     \endverbatim
     </li>
     <li> So here already with libgcc (not with libgfortran) we get an error??
     </li>
     <li> In gcc-4.4.5_build/x86_64-unknown-linux-gnu/libgcc/config.log we find
     \verbatim
/home/kullmann/OKplatform/ExternalSources/builds/Gcc/gcc-4.4.5_build/./gcc/cc1: error while loading shared libraries: libmpfr.so.4: cannot open shared object file: No such file or directory
     \endverbatim
     so, as usual, the linking-specifications are ignored. Later one finds
     (again) "LDFLAGS=''". </li>
    </ol>
   </li>
   <li> The installation-documentation doesn't say anything on this. </li>
   <li> It seems that now also libraries "PPL" ("Parma Polyhedra Library")
   and "CLooG" are needed? Not needed, both are only for loop-optimisation.
   </li>
   <li> Since we need the system-versions of Gmp and Mpfr apparently only
   for Gcc, we should try to put source-directories "gmp" and "mpfr" into
   the extracted gcc-source. </li>
  </ul>


  \todo Installation in general
  <ul>
   <li> Documentation:
    <ol>
     <li> Linking to the gcc-documentation (main info-page) should be checked
     for completeness. </li>
     <li> We could offer also to show the man-page (just open it into a
     browser); however then it should be said that likely the
     html-documentation is more complete. </li>
     <li> We should also move the info-pages to the doc-directory. How to view
     them? The least is just to open them into a browser. </li>
    </ol>
   </li>
   <li> DONE (not used anymore)
   If variable "gcc_version_okl" is set, then it should have one of the
   allowed values (while otherwise we get an error). </li>
   <li> We must understand, how gcc interacts with 32- and 64-bit
   environments, and how to take control of this. </li>
   <li> There should be make-variables, which allow control over
   some settings for the build of gcc. </li>
   <li> texti2pdf is a script, which can be put into ~/bin for example,
   and is needed for building the gcc-documentation --- how to provide it?
   Such small utilities could be put under version control (OKlibrary) ? </li>
   <li> Installation of R requires a Fortran compiler. Therefore the
    system-installation of Gcc should allow for enabling of Fortran
    language support. </li>
   <li> DONE (yes, GMP and MPFR are installed locally)
   Enabling Fortran language support in Gcc requires that the MPFR
    and GMP libraries are installed. Should we install these also locally?
    <ol>
     <li> GMP is also of interest to us regarding big-number-types, so we need
     full control about it, and perhaps a local installation is warranted
     (ignoring the system installation). UPDATE NEEDED </li>
    </ol>
   </li>
   <li> DONE (placed in Buildsystem/Configuration)
   Shouldn't file external_sources_versions.mak be placed in subdirectory
   Buildsystem/ExternalSources ? </li>
   <li> DONE (it seems reasonable
   to remove the build-directory from the prerequisite-list and to build it
   "manually")
   If the filestamp does already exist, then we want nothing to happen ---
   however yet the build-directory will be rebuilt if not existent, since
   it is a prerequisite of the rule for the "tag-paths". So it seems
   necessary to remove the build-directory from the prerequisite-list,
   however then it seems impossible to create the build-directory, if actually
   gcc *is* to be build, via the target-mechanism. </li>
   <li> DONE (now just "oklib gcc", and potentially setting
  gcc_recommended_version_number_okl)
   Instead of, %e.g., "make gcc-4.1.2", wouldn't it be more consistent with
   building Boost to also have "make gcc gcc-version=4.1.2" ? </li>
  </ul>


*/
