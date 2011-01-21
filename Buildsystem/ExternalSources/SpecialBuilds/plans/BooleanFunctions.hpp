// Oliver Kullmann, 22.3.2008 (Swansea)
/* Copyright 2008, 2009, 2010, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/ExternalSources/SpecialBuilds/plans/BooleanFunctions.hpp
  \brief Plans regarding building packages related to boolean functions

  See OKlib/Satisfiability/FiniteFunctions/plans/general.hpp for
  the central module regarding boolean functions (and generalisations).


  \todo BDD's
  <ul>
    <li> CUDD http://vlsi.colorado.edu/~fabio/CUDD/ </li>
    <li> Buddy http://sourceforge.net/projects/buddy </li>
  </ul>


  \todo Aiger
  <ul>
   <li> AIGER http://fmv.jku.at/aiger contains tools for handling AIGs
   (special boolean circuits). </li>
  </ul>


  \todo ABC
  <ul>
   <li> http://www.eecs.berkeley.edu/~alanmi/abc/ </li>
   <li> Seems to be rather comprehensive. </li>
  </ul>


  \todo Translations to SAT
  <ul>
   <li> MNI Proposition Analyzer (MPA)
   http://homepages.fh-giessen.de/~hg11260/hp_p7_de.html
   seems to be a smaller program, translating various forms of propositional
   SAT problems into CNF, and/or solving them directly (based on SAT4J). </li>
   <li> See "BAT" in
   Buildsystem/ExternalSources/SpecialBuilds/plans/ModelChecking.hpp. </li>
  </ul>


  \todo Boolean functions
  <ul>
    <li> DONE (now automatically installed)
    There is an R-package with Quine/McCluskey etc.
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
    <li> http://www.dei.isep.ipp.pt/~ACC/bfunc/ seems not to be open source.
    </li>
    <li> http://www.tcs.hut.fi/~tjunttil/circuits/index.html has translators
    and
    generators for boolean circuits. </li>
  </ul>


  \todo Logic synthesis
  <ul>
   <li> http://vlsicad.eecs.umich.edu/BK/Slots/slots/LogicSynthesis.html </li>
   <li> Scherzo
    <ol>
     <li> Developed by Olivier Coudert (http://www.ocoudert.com/). </li>
     <li> Uses implicit BDD representation for primes and reduction
     techniques to avoid blow up in size of problem due to large
     numbers of prime implicates. </li>
     <li> Coudert has provided the code to MG, although it is not
     clear whether he would be willing to make it open-source. </li>
     <li> The source builds with some minor Makefile changes but
     segfaults. This seems likely to be an issue with how MG is building
     it and the difference in architectures (Scherzo seems to have been
     developed for systems other than standard x86 / x86_64). </li>
     <li> MG needs to e-mail Coudert again to check whether he would
     be willing to open-source the application. </li>
    </ol>
   </li>
   <li> BOOM
    <ol> 
     <li> Developed by Petr Fiser 
     (http://service.felk.cvut.cz/vlsi/prj/BOOM/). </li>
     <li> Non-exact boolean minimisation, but allows iterative
     minimisation. </li>
     <li> According to experimental results on the project site,
     the software is highly competitive. </li>
     <li> License requires authors reference particular papers if
     one uses the software, and one must provide details to download
     the software (MG hasn't done this). </li>
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
   <li> egntott http://code.google.com/p/eqntott/ </li>
  </ul>


  \todo Improve documentation for Espresso
  <ul>
   <li> We should improve the source-code, so that the option "--version"
   is recognised. </li>
   <li> At the ExternalSources page we need an index-page for the various
   pieces of documentation we have. </li>
   <li> We need to investigate what is in espresso-book-examples.tar.gz. </li>
  </ul>


  \bug DONE (added additional entries to the configuration specific
  to the source directories)
  False build for Espresso
  <ul>
   <li> The directory names do not follow our standards. </li>
   <li> In particular, espresso has the package name
   "espresso-ab-1.0-2.3.tar.gz" and directory names
   "espresso-ab-1.0", and yet it's executable name is
   "espresso2.3", all of which are very different. </li>
   <li> This initial decision was made so as not to change
   the version information too much from the original (keeping
   with the authors original version information (1.0)) but adding
   more reasonable version information at the same time (the
   version number as printed by the executable). </li>
   <li> To bring things inline with other packages, espresso
   should simply be repackaged to "espresso-2.3.tar.bz2" with
   "espresso-2.3" as the directory name and "espresso2.3" as
   the executable name. </li>
  </ul>

*/

