// Oliver Kullmann, 18.3.2008 (Swansea)
/* Copyright 2008, 2009 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/ExternalSources/SpecialBuilds/plans/Graphs.hpp
  \brief Plans regarding installation of tools related to graph theory


  \todo General C++ graph libraries
  <ul>
   <li> Not so much happens with the Boost graph library, so we need to look
   out for additional or alternative libraries. </li>
   <li> Goblin http://www.math.uni-augsburg.de/~fremuth/goblin.html
    <ol>
     <li> Seems to be more or less a completed project (last update
     of webpages is from November 2007). Problematic that they only speak
     about "gcc compatibility" with versions 3.4.x --- on the other hand,
     it's seems to be rather basic C/C++ code, so perhaps there aren't
     big issues. </li>
     <li> Includes also graph layout. </li>
     <li> Focus on "network programming problems", in the context of
     optimisaton problems. Though very strong on matching problems etc. </li>
    </ol>
   </li>
   <li> Lemon http://lemon.cs.elte.hu/trac/lemon
    <ol>
     <li> Perhaps this has a wider scope. Though the list of algorithms is
     smaller than that of Goblin. It seems to aim at a broader scope, using
     more general methods, but currently only basics are provided. </li>
     <li> Most recent version from January 2009. </li>
     <li> Seems to be based on the notion of a "concept", closer to generic
     programming, while Goblin is object-oriented (even situated between
     C and C++). </li>
    </ol>
   </li>
  </ul>


  \todo Graph isomorphism
  <ul>
   <li> nauty http://cs.anu.edu.au/~bdm/nauty
    <ol>
     <li> Manual installation:
     \verbatim
builds/Graphs/Nauty> tar -xzf ../../../sources/Graphs/nauty22.tar.gz
raphs/Nauty> cd nauty22
nauty22> ./configure
nauty22> make all
nauty22> make checks
     \endverbatim
     looks all fine. </li>
     <li> Apparently it is a library, and the main application (providing
     input and output) is "dreadnaut". </li>
     <li> And there is "dreadnautB", which can handle graphs with more than
     32765 vertices. </li>
     <li> And some utilities, including the "gtools". </li>
    </ol>
   </li>
   <li> saucy http://vlsicad.eecs.umich.edu/BK/SAUCY/ </li>
  </ul>


  \todo Graph drawing
  <ul>
   <li> http://www.ogdf.net/doku.php looks promising (as a library).
    <ol>
     <li> Before we start writing any C++ code, which should investigate
     thoroughly this library. </li>
    </ol>
   </li>
   <li> http://graphviz.org/Download_source.php
    <ol>
     <li> the dot-tool is needed by doxygen for creating the graphs </li>
     <li> likely we should use the "DOT language"
     (http://graphviz.org/doc/info/lang.html) as the primary language
     for input and output of graphs (supported by Boost) ?! </li>
    </ol>
   </li>
   <li> http://www.tulip-software.org/ (should be very useful, since it
   concentrates on large graphs) </li>
   <li> dot2texi: http://www.fauskes.net/nb/introducing-dot2texi/ </li>
   <li> GraphML: http://graphml.graphdrawing.org/index.html (we need to
   look into this: an xml-based format for graphs of all sorts, hypergraphs,
   and also graphical representations). </li>
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
   or http://or.unbsj.ca/~punnen/tspsoft.html . </li>
   <li> Another overview is http://www.tsp.gatech.edu . </li>
  </ul>


  \todo Graph colouring
  <ul>
   <li> http://www.adaptivebox.net/CILib/code/gcpcodes_link.html (several
   solvers) </li>
   <li> http://www.cs.ualberta.ca/joe/Coloring (solver and generator)
    <ol>
     <li> There is "Graph Coloring Source"
     http://www.cs.ualberta.ca/~joe/Coloring/Colorsrc/index.html </li>
     <li> "Smallk" http://www.cs.ualberta.ca/~joe/Coloring/Colorsrc/smallk.html
     </li>
     <li> Trick's version of "DSATUR" http://mat.gsia.cmu.edu/COLOR/color.html
    </ol>
   </li>
   <li> graphcol http://code.google.com/p/graphcol/ </li>
   <li> There is Allen Van Gelder's SAT encoder
   ftp://ftp.cse.ucsc.edu/pub/avg/Encoder/ </li>
   <li> What about hypergraph colouring? Apparently there are no
   implementations. </li>
  </ul>


  \todo Cliques
  <ul>
   <li> "Cliquer"
    <ol>
     <li> http://users.tkk.fi/~pat/cliquer.html </li>
     <li> Manual: [Niskanen, Oestergard: Cliquer user's guide, version 1.0;
     Technical Report T48, Communications Laboratory, Helsinki University
     of Technology, 2003]. Included in our sources. </li>
     <li> Is open source, and ready to be installed. </li>
    </ol>
   </li>
  </ul>

*/

