// Oliver Kullmann, 16.7.2007 (Swansea)
/* Copyright 2007, 2008, 2009 Oliver Kullmann
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


  \todo Matrix libraries
  <ul>
   <li> Eigen http://eigen.tuxfamily.org/index.php?title=Main_Page looks good
   (it is also stand-alone, and headers only). </li>
   <li> GMM++ http://home.gna.org/getfem/gmm_intro.html (mainly a transformer
   between other linear algebra packages) </li>
  </ul>


  \todo Cryptology
  <ul>
   <li> What about Crypto++ http://www.cryptopp.com/ ? </li>
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
   <li> Cryopid at http://cryopid.berlios.de/ seems to offer this. </li>
  </ul>


*/

