// Oliver Kullmann, 22.3.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/ExternalSources/SpecialBuilds/plans/BooleanFunctions.hpp
  \brief Plans regarding building packages related to boolean functions


  \todo BDD's
  <ul>
    <li> CUDD http://vlsi.colorado.edu/~fabio/CUDD/ </li>
  </ul>


  \todo Translations to SAT
  <ul>
   <li> MNI Proposition Analyzer (MPA)
   http://homepages.fh-giessen.de/~hg11260/hp_p7_de.html
   seems to be a smaller program, translating various forms of propositional
   SAT problems into CNF, and/or solving them directly (based on SAT4J). </li>
  </ul>


  \todo Boolean functions
  <ul>
    <li> There is an R-package with Quine/McCluskey etc.
     <ol>
      <li> Installation (from CRAN)
      \verbatim
> oklib --R
# Within R environment
> install.packages("QCA")
      \endverbatim
      This draws the package from external CRAN sources. The lpSolve package 
      dependency seems to be already installed for R in oklib.
      </li>
      <li> Installation (from source tarball)
      \verbatim
ExternalSources/Installations/R> oklib --R
# Within R environment (with  QCA_0.5-0.tar.gz in the current directory)
> install.packages("QCA_0.5-0.tar.gz", repos=NULL)
      \endverbatim
      Setting the "repos" parameter to null seems to force it to treat the 
      package arguments as file paths to tar.gz packages.
      </li>
     </ol>
    </li>
    <li> [Coudert, O; 1994, Two-level logic minimization: An overview;
    Integration, the VLSI Journal, vol. 17, pages 97-140] could serve
    as a start for the area of optimisation of boolean functions. </li>
    <li> [Ron Rymon, 1994, An SE-tree-based Prime Implicant Generation
    Algorithm] contains algorithms for computing prime implicants (via
    transversals). </li>
    <li> http://www.dei.isep.ipp.pt/~ACC/bfunc/ seems not to be open source.
    </li>
    <li> http://www.tcs.hut.fi/~tjunttil/circuits/index.html has translators and
    generators for boolean circuits. </li>
  </ul>


  \todo Logic synthesis
  <ul>
   <li> http://vlsicad.eecs.umich.edu/BK/Slots/slots/LogicSynthesis.html </li>
   <li> Espresso
    <ol>
     <li> http://embedded.eecs.berkeley.edu/pubs/downloads/espresso/index.htm
     </li>
    </ol>
   </li>
   <li> Using genetic programming:
    <ol>
     <li> There are quite a few papes on finding (digital) circuits via
     genetic programming, but apparently no software. </li>
    </ol>
   </li>
   <li> Grigory Yaroslavtsev and Alexander Kulikov have written some software
   to find short circuits (over the full binary base for example). </li>
  </ul>

*/

