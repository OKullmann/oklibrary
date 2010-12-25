// Oliver Kullmann, 27.6.2007 (Swansea)
/* Copyright 2007, 2008, 2010 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/ExternalSources/SpecialBuilds/plans/Gcc.hpp
  \brief Plans regarding installation of gcc


  \bug Local build of Fortran fails
  <ul>
   <li> We get (for "oklib gcc") an error when building
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
   there (we could use later gcc-versions for Fortran (only)). </li>
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


  \todo Putting "configure" under our version control
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
  </ul>


  \todo GCC 4.1.2 will not build on systems without GMP with MPFR support
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


  \todo Providing gcc 4.1.2
  <ul>
   <li> Motivated by "GRASP cannot be compiled with gcc version 4.3" (see
   Buildsystem/ExternalSources/SpecialBuilds/plans/SAT.hpp), we should provide
   a permanent installation of version 4.1.2. </li>
   <li> Perhaps we should provide in general build-variables supporting
   usage of gcc (i.e., calling the compiler, and providing the instructions
   for fixing the shared libraries in the binaries. </li>
   <li> Changed configuration file sources/Gcc/configure-4.1.2.gz:
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
    </ol>
   </li>
   <li> STATUS UPDATE URGENTLY NEEDED --- MG SHOULD HAVE COMPLETED THIS!
   ALSO sources/Gcc/configure-4.1.2.gz MUST BE TAKEN INTO ACCOUNT!
   MG MUST SPECIFY HOW HIS PACKAGE WAS CREATED.
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
     the following file
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
     <li> Of course, the question is whether this patch is really a general
     improvement, and whether it creates problems with other systems? </li>
     <li> Those who are interested in this patch need to test it on other
     systems. </li>
     <li> And precise information on how to perform the improvements are
     needed (note that we are using already an altered gcc!). </li>
    </ul>
   </li>
  </ul>


  \todo Install GCC 4.2.4
  <ul>
   <li> First only as an alternative (since yet code doesn't compile with
   versions 4.2 or later). </li>
   <li> It seems best to proceed in stages, first installing 4.2.4 and
   updating our code, then installing 4.3.5, and then 4.4.4. </li>
  </ul>


  \todo Install GCC 4.3.5


  \todo Install GCC 4.4.5
  <ul>
   <li> Building 4.4.4:
    <ol>
     <li> Now Gmp and Mpfr is needed. </li>
     <li> For Gmp just use the configure-option
     "--with-gmp=$(gmp_installation_dir_okl)". </li>
     <li> However, then we have the problem of a circular dependency, since
     these libraries are built using Gcc! </li>
     <li> The main question is about the link-library-compatabilities. </li>
    </ol>
   </li>
  </ul>


  \todo Install GCC 4.5.2
  

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
