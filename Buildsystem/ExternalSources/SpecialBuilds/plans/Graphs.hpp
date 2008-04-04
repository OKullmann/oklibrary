// Oliver Kullmann, 18.3.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/ExternalSources/SpecialBuilds/plans/Graphs.hpp
  \brief Plans regarding installation of tools related to graph theory


  \todo Graph isomorphism
  <ul>
   <li> naughty </li>
   <li> saucy http://vlsicad.eecs.umich.edu/BK/SAUCY/ </li>
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
   <li> dot2texi: http://www.fauskes.net/nb/introducing-dot2texi/ </li>
  </ul>


  \todo Treewidth
  <ul>
   <li> Overview on open-source implementations? </li>
   <li> For the hypergraph extensions see the homepage of Georg Gottlob. </li>
  </ul>


  \todo Travelling salesman
  <ul>
   <li> In [Gutin, Punnen; The Traveling Salesman Problem and Its
   Variations], Chapter 16, one finds a first overview, with web page
   http://www.or.deis.unibo.it/research_pages/tspsoft.html
   or http://or.unbsj.ca/~punnen/tspsoft.html. </li>
  </ul>


  \todo Graph colouring
  <ul>
   <li> http://www.cs.ualberta.ca/joe/Coloring (solver and generator) </li>
  </ul>

*/

