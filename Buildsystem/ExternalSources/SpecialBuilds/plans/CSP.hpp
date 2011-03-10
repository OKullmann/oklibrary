// Oliver Kullmann, 17.3.2008 (Swansea)
/* Copyright 2008, 2009, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/ExternalSources/SpecialBuilds/plans/CSP.hpp
  \brief Plans regarding installation of constraint satisfaction systems


  \todo Enter these todos into the milestones.


  \todo Overview
  <ul>
   <li> http://en.wikipedia.org/wiki/Constraint_programming gives an
   overview. </li>
   <li> The MAC-algorithm at http://www.hulubei.net/tudor/csp ? </li>
   <li> Benchmarks? Competitions? </li>
  </ul>


  \todo Gecode
  <ul>
   <li> Gecode http://www.gecode.org/ </li>
   <li> Since this is a library, this is very interesting. </li>
   <li> The current version (28.4.2009) is 3.0.2, and now they also have
   a reasonable introductory text. </li>
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
minion-0.8.1/bin> BOOST_INCLUDEDIR=$OKPLATFORM/ExternalSources/Boost/1_34_1+4.1.2/include/boost-1_34_1 BOOST_LIBRARYDIR=$OKPLATFORM/ExternalSources/Boost/1_34_1+4.1.2/lib/ cmake ..
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
CC="../../../Gcc/4.1.2/bin/gcc" CXX="../../../Gcc/4.1.2/bin/g++" BOOST_INCLUDEDIR=/home/aeternus/Work/OKlibrary/OKlib/OKplatform/ExternalSources/Boost/1_34_1+4.1.2/include/boost-1_34_1/ BOOST_LIBRARYDIR=/home/aeternus/Work/OKlibrary/OKlib/OKplatform/ExternalSources/Boost/1_34_1+4.1.2/lib/ cmake ..
   \endverbatim
   </li>
   <li> We should build CMake, and put it into OKlibrary/bin. </li>
  </ul>


  \todo Mozart
  <ul>
   <li> Mozart http://www.mozart-oz.org/ </li>
   <li> This looks like a powerful system which we need to install. </li>
  </ul>


  \todo Eclipse
  <ul>
   <li> Eclipse http://eclipse-clp.org/ </li>
   <li> Apparently no longer available? The web-page seems unrelated to
   CSP. </li>
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


  \todo Local search
  <ul>
   <li> ZDC ? </li>
  </ul>

*/

