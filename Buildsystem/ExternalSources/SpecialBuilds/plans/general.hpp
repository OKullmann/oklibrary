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


  \todo Computer algebra
  <ul>
   <li> What about Octave? Gap? Singular? </li>
  </ul>


  \todo Graph drawing
  <ul>
   <li> http://www.ogdf.net/doku.php looks promising. </li>
   <li> http://graphviz.org/Download_source.php
    <ol>
     <li> the dot-tool is needed by doxygen for creating the graphs </li>
     <li> likely we should use the "DOT language"
     (http://graphviz.org/doc/info/lang.html) as the primary language
     for input and output of graphs (supported by Boost) ?! </li>
    </ol>
   </li>
   <li> http://www.labri.fr/perso/auber/projects/tulip/ </li>
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

