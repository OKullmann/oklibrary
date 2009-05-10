// Oliver Kullmann, 16.7.2007 (Swansea)
/* Copyright 2007, 2008, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/ExternalSources/SpecialBuilds/plans/general.hpp
  \brief Plans regarding building of concrete external sources


  \todo Install the mmv package : DONE
  <ul>
   <li> For installing %boost we need mln. Compare "Update Boost installation"
   in  Buildsystem/ExternalSources/SpecialBuilds/plans/Boost.hpp. </li>
   <li> I found "mmv_1.01b.orig.tar.gz  part01.Z  part02.Z". </li>
   <li> Apparently they only contain the mmv-program (a C program), not
   mln, mcp etc. </li>
   <li> Perhaps it's not worth the trouble --- all our uses of that package
   should be easily replaceable by little bash-applications. </li>
   <li> First attempts at installing some versions of mmv failed, and we
   have only one application, namely the "mln" application in
   Buildsystem/ExternalSources/SpecialBuilds/boost.mak, and this shall
   be done directly (via bash). </li>
   <li> At http://packages.debian.org/source/etch/mmv there is a renewed
   version? How to get a standard package out of these three files there?
   OK e-mailed the maintainer there. </li>
  </ul>


  \todo Documentation : DONE
  <ul>
   <li> Under SpecialBuilds/docus we need docus-files for every
   external source:
    <ol>
     <li> cmake : DONE (postponed until we update installation) </li>
     <li> mhash : DONE </li>
     <li> mpfr : DONE (postponed until we update installation) </li>
     <li> asciidoc : DONE </li>
     <li> %boost : DONE </li>
     <li> clisp : DONE </li>
     <li> coq : DONE </li>
     <li> doxygen : DONE </li>
     <li> gcc : DONE </li>
     <li> git : DONE </li>
     <li> gmp : DONE </li>
     <li> maxima : DONE </li>
     <li> ocaml : DONE </li>
     <li> postgresql : DONE </li>
     <li> r : DONE </li>
     <li> sage : DONE </li>
     <li> ubcsat (remark about corrected files; content of package) : DONE </li>
     <li> valgrind : DONE </li>
    </ol>
   </li>
   <li> DONE
   For each of these docus-files there is a link from
   ExternalSources/docus/general.hpp. </li>
  </ul>


  \todo Complete documentation
  <ul>
   <li> For the following external sources, docus are missing:
    <ol>
     <li> CMake </li>
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


  \todo Update MHash-building
  <ul>
   <li> This should be similar to Boost and Gmp. </li>
   <li> In the docus, we should make a remark about corrected files and
   about the content of the package. </li>
  </ul>


  \todo DONE (the new scheme is as discussed below)
  Handling of different versions
  <ul>
   <li> Perhaps it is easier to handle installation of different versions
   by just setting the variable "XXX_recommended_version_number_okl" ? </li>
   <li> This would save us the effort of creating different targets etc. </li>
   <li> Only one has to take care of the clashes with
   "XXX_supported_not_recommended_version_numbers_okl". </li>
   <li> But once we do not need the other targets anymore, this problem
   will vanish! </li>
   <li> First tried out this scheme with Valgrind. </li>
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


  \todo Combinatorics
  <ul>
   <li> http://people.scs.fsu.edu/~burkardt/cpp_src/cpp_src.html </li>
  </ul>


  \todo SAT-applications in evolutionary biology
  <ul>
   <li> Tree measures ("rSPR") at
   http://www.lsi.upc.es/~bonet/software.html </li>
  </ul>


  \todo Cryptology
  <ul>
   <li> What about Crypto++ http://www.cryptopp.com/ ? </li>
  </ul>


  \todo Other sources:
  <ul>
   <li> Build a local version of Xerces. </li>
   <li> Investigate the Poco C++ library (http://www.pocoproject.org; see
   also [C Vu, 19:2, pages 12-15]). </li>
   <li> SOCI (soci.sourceforge.net) ? </li>
   <li> gcc-xml (www.gccxml.org) ? </li>
  </ul>


*/

