// Oliver Kullmann, 17.3.2008 (Swansea)
/* Copyright 2008, 2009, 2011, 2012 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/ExternalSources/SpecialBuilds/plans/CSP.hpp
  \brief Plans regarding installation of constraint satisfaction systems


  \todo Overview
  <ul>
   <li> http://en.wikipedia.org/wiki/Constraint_programming gives an
   overview. </li>
   <li> The MAC-algorithm at http://www.hulubei.net/tudor/csp ? </li>
   <li> Competitions:
    <ul>
     <li> There was a CSP competition, http://www.cril.univ-artois.fr/CPAI09/.
     It appears to have stopped in 2009. </li>
     <li> For finite domain problems, there is the MiniZinc challenge,
     http://www.g12.csse.unimelb.edu.au/minizinc/challenge2011/challenge.html.
     </li>
    </ul>
   </li>
   <li> Benchmarks:
    <ul>
     <li> Available at the competition sites
     http://www.cril.univ-artois.fr/CPAI08/ and
     http://www.cril.univ-artois.fr/CPAI09/.
    </ul>
   </li>
  </ul>


  \todo Constraint solvers
  <ul>
   <li> We currently have no CSP solvers available in the OKlibrary. </li>
   <li> We should at least install the following:
    <ul>
     <li> Mistral, at http://homepages.laas.fr/ehebrard/Software.html .
     </li>
     <li> Sugar, at http://bach.istc.kobe-u.ac.jp/sugar/ . </li>
     <li> Minion, at http://minion.sourceforge.net/ . </li>
     <li> Choco, at http://www.emn.fr/z-info/choco-solver/ . </li>
     <li> All of these solvers are discussed individually in
     todos of the same name below. </li>
    </ul>
   </li>
  </ul>


  \todo Constraint modelling languages
  <ul>
   <li> We need an overview of constraint modelling languages. </li>
   <li> Which solvers read which languages? </li>
   <li> Are there translations between them? </li>
   <li> See:
    <ul>
     <li> XCSP 2.1
      <ul>
       <li> See http://www.cril.univ-artois.fr/CPAI08/XCSP2_1.pdf . </li>
      </ul>
     </li>
     <li> Essence
      <ul>
       <li> See http://www.cs.york.ac.uk/aig/constraints/AutoModel/ . </li>
      </ul>
     </li>
     <li> Zinc
      <ul>
       <li> Subsets of the language, MiniZinc and FlatZinc exist. </li>
       <li> See http://www.g12.csse.unimelb.edu.au/ for the specification.
       </li>
       <li> Used as the input format for the MiniZinc Challenge; see
       http://www.g12.csse.unimelb.edu.au/minizinc/challenge2011/challenge.html .
       </li>
       <li> Gecode provides a CSP solver which takes FlatZinc files
       as input. </li>
      </ul>
     </li>
    </ul>
   </li>
   <li> %Tools:
    <ul>
     <li> TAILOR
      <ul>
       <li> Translates from Essence' or XCSP 2.1 to Essence', FlatZinc,
       Minion and Gecode. </li>
       <li> See http://www.cs.st-andrews.ac.uk/~andrea/tailor/. </li>
       <li> %Support discontinued in September 2010. </li>
      </ul>
     </li>
    </ul>
   </li>
  </ul>


  \todo Sugar
  <ul>
   <li> http://bach.istc.kobe-u.ac.jp/sugar/ </li>
   <li> See ExternalSources/sources/CSP/Sugar. </li>
   <li> The older version of this translation-system is "CSP2SAT"; see
   http://bach.istc.kobe-u.ac.jp/csp2sat/. They recommend to use "Sugar",
   however Sugar is written in Java. </li>
  </ul>


  \todo Gecode
  <ul>
   <li> Gecode http://www.gecode.org/ </li>
   <li> Since this is a library, this is very interesting. </li>
   <li> The current version (28.7.2011) is 3.6.0, and now they also have
   a reasonable introductory text. </li>
   <li> Gecode provides an executable which reads "FlatZinc" files
   and outputs the solution. </li>
  </ul>


  \todo Minion
  <ul>
   <li> Minion http://minion.sourceforge.net/ </li>
   <li> This is a single solver, apparently with a powerful input
   language. </li>
   <li> Installation:
   \verbatim
builds/CSP> tar -xzf ../../sources/CSP/Minion/minion-0.8.1-src.tar.gz
builds/CSP> cd minion-0.8.1
minion-0.8.1> mkdir bin
minion-0.8.1> cd bin
minion-0.8.1/bin> BOOST_INCLUDEDIR=$(boost_source_library_okl)/boost BOOST_LIBRARYDIR=$(boost_link_library_okl) cmake ..
CMake Error: Boost not found -- do you have the iostreams component?
   \endverbatim
   So we need CMake and Boost --- how to tell Minion about the location
   of Boost sources and link-libraries? The above should work, but
   doesn't. </li>
   <li> The above doesn't work for several reasons : 
   <ul>
    <li> The cmake files present in minion seem to use "HINTS" rather than
    "PATHS" for "FIND_PATH" and "FIND_LIBRARY" which is only available in 
    versions of cmake >= 2.6.1. </li>
    <li> One must make sure the "iostreams" library in Boost is built which
    requires the "bzip2" headers to be installed for it to be built during
    the Boost installation. </li>
    <li> As the Boost library files include the gcc compiler version in 
    their names, one must ensure that the gcc version being used by cmake
    is the local version by setting CC and CXX environment variables. Either
    that or ensure there are symlinks such as "libboost_iostreams.so" etc,
    as the "FindBoost.cmake" cmake module detects either. </li>
   </ul>
   Ensuring all of the above and using the following command seems to generate
   the minion Makefile : 
   \verbatim
CC="$(gcc_call_okl)" CXX="$(gcc_call_okl)" BOOST_INCLUDEDIR=$(boost_source_library_okl)/boost/ BOOST_LIBRARYDIR=$(boost_link_library_okl) cmake ..
   \endverbatim
   </li>
   <li> Building minion 0.12 with current (outdated) cmake:
   \verbatim
builds/CSP> tar -xzf ../../sources/CSP/Minion/minion-0.12-src.tar.gz
builds/CSP> cd minion-0.12
minion-0.12> mkdir bin
minion-0.12> cd bin
minion-0.12/bin> CC="$(gcc_call_okl)" CXX="$(gcc_call_okl)" cmake -DBOOST_ROOT:PATH=$(boost_installation_dir_okl) -DBZIP2_LIBRARIES:PATH=$(bzip2_link_library_okl)/libbz2.a
   \endverbatim
   Note that $(gcc_call_okl) are make variables.
   </li>
   <li> We should update CMake. </li>
  </ul>


  \todo Mistral
  <ul>
   <li> Website at http://homepages.laas.fr/ehebrard/Software.html . </li>
   <li> Seems to be a library. </li>
   <li> Open-source? </li>
  </ul>


  \todo Choco
  <ul>
   <li> Website at http://www.emn.fr/z-info/choco-solver/ . </li>
   <li> An open-source Java library. </li>
  </ul>


  \todo Mozart
  <ul>
   <li> Mozart http://www.mozart-oz.org/ </li>
   <li> This looks like a powerful system which we need to install. </li>
  </ul>


  \todo Eclipse
  <ul>
   <li> Eclipse http://eclipseclp.org/index.html </li>
   <li> Looks like another powerful system. </li>
  </ul>


  \todo N Queens
  <ul>
   <li> http://www.jsomers.com/nqueen_demo/nqueens.html with jmsnqueens.zip
    <ol>
     <li> We should upgrade the code to C99, using appropriately
     32,64 or 128 bits (and getting rid off the "WIN32"-etc-macros). </li>
     <li> We should also add a compile-time check for the required
     "2's complement architecture". </li>
     <li> Add conditionally compiled code to (somehow) compute the number of
     "nodes". </li>
     <li> Apparently no special compiler-options are recommended. </li>
    </ol>
   </li>
   <li> reines.tgz </li>
  </ul>


  \todo Sudoku (only "big Sudoku", of arbitrary dimensions)
  <ul>
   <li> http://sudoku.sourceforge.net/ (Java) </li>
   <li> http://pythonsudoku.sourceforge.net/ (Python) </li>
   <li> http://sudoku-sensei.sourceforge.net/ (C) </li>
   <li> http://robert.rsa3.com/sudoku.html (C++) </li>
  </ul>


  \todo TAILOR
  <ul>
   <li> http://www.cs.st-andrews.ac.uk/~andrea/tailor/ </li>
   <li> A tool for translating general CSP modelling languages
   to specific CSP solver input formats. </li>
   <li> TAILOR is written in Java. </li>
   <li> It can translate Essence and XCSP to Minion input.
   XCSP is the language used for the CSP08 and CSP09 competitions.
   XCSP is an XML format. </li>
   <li> We should install this and consider using one of these general
   formats, rather than writing many different translations. </li>
  </ul>


  \todo Local search
  <ul>
   <li> ZDC ? </li>
  </ul>

*/

