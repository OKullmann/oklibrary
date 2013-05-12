// Oliver Kullmann, 16.7.2007 (Swansea)
/* Copyright 2007, 2008, 2009, 2011, 2013 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/ExternalSources/SpecialBuilds/plans/general.hpp
  \brief Plans regarding building of concrete external sources


  \todo Complete documentation
  <ul>
   <li> For the following external sources, docus are missing:
    <ol>
     <li> Coq </li>
     <li> March </li>
     <li> Minisat </li>
     <li> Mpfr </li>
     <li> Satz </li>
     <li> SP </li>
    </ol>
   </li>
   <li> Most of the current docus are not complete (or are not up-to-date).
   </li>
  </ul>


  \todo Patches
  <ul>
   <li> An overview is needed on the correct way use the diff and
   patch tools. This should then be moved to docus. </li>
   <li> One has two packages, an "original" package and an updated
   version. </li>
   <li> The task is to produce a "patch" file. This contains the difference
   between the original and updated package. This is done using the diff
   tool. </li>
   <li> This file can then be applied to the original package by another user.
   The application of the patch occurs using the patch tool. </li>
   <li> The standard method:
   <ul>
    <li> The old package is in directory A (with files, subdirectories etc).
    </li>
    <li> The updated package is in directory B. </li>
    <li> Both packages are in the same parent directory. </li>
    <li> From the parent directory run:
    \verbatim
diff -ruN A B > patchfile
    \endverbatim
    </li>
    <li> The flags "r", "u" and "N" specify that the diff should:
    <ul>
     <li> Be recursive over the given directories. </li>
     <li> Be unified. That is, 3 lines of context are printed for each
     difference. </li>
     <li> Treat non-existant files as empty files. </li>
    </ul>
    </li>
    <li> The patch file can then be applied from inside the directory of the
    package to be patched by running:
    \verbatim
patch -p1 < patchfile
    \endverbatim
    </li>
    <li> Note, the filenames to be updated are included in the patchfile.
    </li>
    <li> The "-p" argument here specifies the number of directories to
    remove from the file paths in the patchfile when looking for the files
    to patch. </li>
    <li> So using "-p1", if a file was originally in A/file1, the patch tool
    will look for this file in file1 rather than A/file1. </li>
    <li> "-p1" is important here, as we are inside the package directory
    and so must ignore the original package directory names. </li>
   </ul>
   </li>
  </ul>


  \todo Enable local/global installation for all packages
  <ul>
   <li> Likely local installation should be the default. </li>
   <li> See Gmp.mak and
   Buildsystem/ExternalSources/SpecialBuilds/plans/Gmp.hpp
   for the new scheme. </li>
   <li> List of external sources to be localised:
    <ol>
     <li> Doxygen : DONE </li>
     <li> Git : DONE </li>
     <li> Valgrind (see also "Update to version 3.4.0" in
     Buildsystem/ExternalSources/SpecialBuilds/plans/Valgrind.hpp). </li>
    </ol>
   </li>
   <li> List of external sources where also global installation should
   be possible:
    <ol>
     <li> Gcc </li>
    </ol>
   </li>
  </ul>


  \todo Organisation of links
  <ul>
   <li> There should be one main page for SAT-installation (compare
   "Installation in general" in
   Buildsystem/ExternalSources/SpecialBuilds/plans/SAT.hpp), and similar
   also for all other subjects. </li>
   <li> On these overview-pages then we place also links to software
   which is not included (since it's not open-source). </li>
   <li> Then we need also (in the doxygen-system) a general overview
   page on external sources, which contains all links to these special
   overview pages. </li>
   <li> Perhaps best we keep the current local-system overview page on
   external sources, and link it with the general doxygen overview page.
   This current page ("External Sources overview and index page")
   perhaps is then only called "External Sources index page". </li>
   <li> The new external-sources-overview page then occurs also on the
   local home page, under "Documentation". </li>
   </li>
  </ul>


  \todo Installation of C/C++ tools
  <ul>
   <li> Code analysis tool "Splint" http://www.splint.org </li>
   <li> Also "Clang" http://clang.llvm.org/ yields a C analysis tool
   (and hopefully in some time also a C++ analysis tool). </li>
  </ul>


  \todo Supporting Java
  <ul>
   <li> The following SAT and CSP libraries use Java:
    <ul>
     <li> SAT4J, see "SAT4J" in
     Buildsystem/ExternalSources/SpecialBuilds/plans/SAT.hpp. </li>
     <li> BoolVar, see "BoolVar/PB" in
     Buildsystem/ExternalSources/SpecialBuilds/plans/SAT.hpp. </li>
     <li> Sugar, see "Sugar" in
     Buildsystem/ExternalSources/SpecialBuilds/plans/CSP.hpp. </li>
     <li> Choco, see "Choco" in
     Buildsystem/ExternalSources/SpecialBuilds/plans/CSP.hpp. </li>
     <li> TAILOR, see "TAILOR" in
     Buildsystem/ExternalSources/SpecialBuilds/plans/CSP.hpp. </li>
     <li> See also "Sudoku" in
     Buildsystem/ExternalSources/SpecialBuilds/plans/CSP.hpp. </li>
    </ul>
   </li>
   <li> In the OKlibrary, we always compile from source. </li>
   <li> We should support the compilation of Java files to
   allow the inclusion of the above projects in the OKlibrary. </li>
   <li> Should we allow compilation of Java files within %OKlib?
    <ul>
     <li> As with .cpp files, .java files could be supported by
     the build system, making Java a first class language in the
     OKlibrary. </li>
     <li> One advantage to including Java, is that interfacing with
     the libraries above would be easier. </li>
     <li> However, what use is being able to interface if we can't
     then combine this with already existing C++ code? </li>
     <li> Also we already have C++; Java and the thinking
     behind it don't fit with the theme of the OKlibrary. </li>
     <li> It seems best to restrict the use of Java to compilation
     of ExternalSources. </li>
     <li> We can likely interface by compiling Java files to binary
     libraries, and linking via the Compiled Native Interface in tools
     such as gcj. See
     http://en.wikipedia.org/wiki/GNU_Compiler_for_Java#CNI_.28Compiled_Native_Interface.29
     and http://gcc.gnu.org/java/papers/cni/t1.html . </li>
    </ul>
   </li>
  </ul>


  \todo Combinatorics
  <ul>
   <li> http://people.scs.fsu.edu/~burkardt/cpp_src/cpp_src.html </li>
  </ul>


  \todo SAT-applications in evolutionary biology
  <ul>
   <li> Tree measures ("rSPR") at
   http://www.lsi.upc.es/~bonet/software.html </li>
  </ul>


  \todo Matrix libraries
  <ul>
   <li> Eigen http://eigen.tuxfamily.org/index.php?title=Main_Page looks good
   (it is also stand-alone, and headers only). </li>
   <li> GMM++ http://home.gna.org/getfem/gmm_intro.html (mainly a transformer
   between other linear algebra packages) </li>
  </ul>


  \todo Logic programming
  <ul>
   <li> http://www.mpprogramming.com/cpp/ provides Prolog-functionality for
   C++; looks interesting. </li>
  </ul>


  \todo Other sources:
  <ul>
   <li> Build a local version of Xerces. </li>
   <li> Investigate the Poco C++ library (http://www.pocoproject.org; see
   also [C Vu, 19:2, pages 12-15]). </li>
   <li> SOCI (soci.sourceforge.net) ? </li>
   <li> gcc-xml (www.gccxml.org) ? </li>
  </ul>


  \todo Suspending running processes
  <ul>
   <li> It would be useful to be able to suspend running processes
   during experimentation, and then wrap scripts in such a way that SIGTERM
   is caught and running experiments suspended. </li>
   <li> In such a way, we don't lose experiments if a machine is accidentally
   shut down. </li>
   <li> Cryopid at http://cryopid.berlios.de/ seems to offer this.
    <ol>
     <li> The web page was last updated 2005? </li>
     <li> However, the mailing list seems to have continual
     activity (see 
     https://lists.berlios.de/pipermail/cryopid-devel/2009-May/thread.html ).
     </li>
    </ol>
   </li>
  </ul>


*/

