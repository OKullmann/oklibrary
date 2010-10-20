// Oliver Kullmann, 22.3.2008 (Swansea)
/* Copyright 2008, 2009, 2010 Oliver Kullmann
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
   <li> Espresso
    <ol>
     <li> http://embedded.eecs.berkeley.edu/pubs/downloads/espresso/index.htm
     </li>
     <li> Espresso-ab, a fork of Espresso tidying and fixing Espresso to work
     with modern gcc versions, is available at - 
     http://code.google.com/p/eqntott/downloads/detail?name=espresso-ab-1.0.tar.gz&can=2&q=
     %- It doesn't appear to have a well established website and so offering 
     this for download within the OKlibrary could be beneficial. </li>
     <li> Espresso-ab doesn't appear to have a well known open-source license,
     although it does come with a license which allows copying,
     redistribution and derivative works. </li>
     <li> To install espresso-ab, one can download it from the site 
     (MD5SUM : 1c60ac344779dc8753f8e0785eafd8ac  espresso-ab-1.0.tar.gz) 
     and use the standard make-style compilation procedure, so
     \verbatim
cd $OKPLATFORM/ExternalSources/builds/
mkdir Espresso
cd Espresso
wget http://eqntott.googlecode.com/files/espresso-ab-1.0.tar.gz
tar zxvf espresso-ab-1.0.tar.gz
cd espresso-ab-1.0
./configure
make
./src/espresso --help
     \endverbatim
     and then one can use the following "test.pla" file
     with espresso 
     \verbatim
.i 3
.o 1
110 1
111 1
010 1
001 0
000 0
100 1
011 0
101 0
     \endverbatim
     by calling 
     \verbatim
$OKPLATFORM/ExternalSources/builds/Espresso/espresso-ab-1.0/src/espresso test.pla
.i 3
.o 1
.p 3
1-0 1
-10 1
11- 1
.e
     \endverbatim
     </li>
     <li> Note here that the PLA file is a format for describing boolean 
     functions. For simple usage one can assume that the ".i" line specifies
     the number of input variables, the ".o" line, the number of output 
     variables, and then each line specifies the entry in a truth table 
     describing the associated boolean function. If an entry is marked with
     a "-", this is simply shorthand for the expansion into multiple truth
     table entries where the "-" values take every possibily but the rest
     of the values remain unchanged. For more details, see 
     http://people.ee.duke.edu/~jab/ece151/espresso.5.html .
     </li>
     <li> Note that espresso-ab provides no documentation but 
     the original documentation applies and is available at
     http://embedded.eecs.berkeley.edu/pubs/downloads/espresso/index.htm .
     </li>
    </ol>
   </li>
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
  </ul>

*/

