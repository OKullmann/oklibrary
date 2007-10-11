// Oliver Kullmann, 16.7.2007 (Swansea)
/* Copyright 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/ExternalSources/SpecialBuilds/plans/general.hpp
  \brief Plans regarding building of concrete external sources


  \todo Documentation
  <ul>
   <li> Under SpecialBuilds/docus we need docus-files for every
   external source:
    <ol>
     <li> asciidoc </li>
     <li> cmake </li>
     <li> mhash (remark about corrected files; content of package) </li>
     <li> mpfr </li>
     <li> ubcsat (remark about corrected files; content of package) </li>
     <li> r : DONE </li>
     <li> %boost : DONE </li>
     <li> coq : DONE </li>
     <li> doxygen : DONE </li>
     <li> gcc : DONE </li>
     <li> git : DONE </li>
     <li> gmp : DONE </li>
     <li> ocaml : DONE </li>
     <li> postgresql : DONE </li>
     <li> sage : DONE </li>
     <li> valgrind : DONE </li>
    </ol>
   </li>
   <li> For each of these docus-files there is a link from
   ExternalSources/docus/general.hpp. </li>
  </ul>


  \todo Complete documentation
  <ul>
   <li> Most of the current docus are not complete. </li>
  </ul>


  \todo Enable local/global installation for all packages
  <ul>
   <li> Likely local installation should be the default. </li>
   <li> See Gmp.mak and
   Buildsystem/ExternalSources/SpecialBuilds/plans/Gmp.hpp
   for the new scheme. </li>
   <li> List of external sources to be localised:
    <ol>
     <li> Doxygen </li>
     <li> Git </li>
     <li> Valgrind </li>
    </ol>
   </li>
   <li> List of external sources where also global installation should
   be possible:
    <ol>
     <li> Gcc </li>
    </ol>
   </li>
  </ul>


  \todo Installing SAT solvers
  <ul>
   <li> Installing external SAT solvers should be a special service. </li>
   <li> Perhaps we have a special subdirectory of OKplatform/ExternalSources
   (for example OKplatform/ExternalSources/SATsolver) ? </li>
   <li> And perhaps also for SpecialBuilds we should create a sub-module
   SATsolvers ? (We could subdivide SpecialBuilds into the different
   sections.) </li>
   <li> These solvers should be somewhat more integrated into the OKlibrary
   than just ordinary external sources; on the other hand, these packages
   are usually in much worse shape. </li>
   <li> Of course, only open-source-solvers. </li>
   <li> For C++ solvers we need the ability to use older gcc-versions. </li>
   <li> We should have also a directory Buildsystem/ExternalSources/SATsolvers
   (solvers are not "SpecialBuilds"). </li>
   <li> List of solvers/libraries:
    <ol>
     <li> http://sat.inesc-id.pt/OpenSAT/index.php </li>
     <li> http://www.sat4j.org/ </li>
     <li> http://www-verimag.imag.fr/~cotton/jat/ </li>
     <li> http://sourceforge.net/projects/dpt </li>
     <li> Minisat: is there a "golden version"? Is it under active
     development? </li>
     <li> Chaff: same questions. </li>
     <li> March: same questions, and is there a package? </li>
     <li> Kcnfs: likely not under active development anymore. </li>
    </ol>
   </li>
  </ul>


  \todo Computer algebra
  <ul>
   <li> What about Octave? Gap? Singular? </li>
  </ul>


  \todo Graph drawing
  <ul>
   <li> http://www.ogdf.net/doku.php looks promising. </li>
   <li> graphviz (the dot-tool is needed by doxygen for creating the graphs) </li>
   <li> tulip </li>
  </ul>


  \todo Graph algorithms libraries and programs
  <ul
   <li> naughtly for graph isomorphisms </li>
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

