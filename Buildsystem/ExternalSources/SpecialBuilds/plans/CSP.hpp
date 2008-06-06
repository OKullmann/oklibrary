// Oliver Kullmann, 17.3.2008 (Swansea)
/* Copyright 2008 Oliver Kullmann
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
   <li> Gecode http://www.gecode.org/
    <ol>
     <li> Since this is a library, this is perhaps most interesting. </li>
    </ol>
   </li>
   <li> Minion http://minion.sourceforge.net/
    <ol>
     <li> This is a single solver, apparently with a powerful input
     language. </li>
    </ol>
   </li>
   <li> Eclipse http://eclipse-clp.org/
    <ol>
     <li> Apparently no longer available? The web-page seems unrelated to
     CSP. </li>
    </ol>
   </li>
   <li> Gnu Prolog http://www.gprolog.org/ </li>
   <li> Swi Prolog http://www.swi-prolog.org/ </li>
   <li> The MAC-algorithm at http://www.hulubei.net/tudor/csp ? </li>
   <li> Benchmarks? Competitions? </li>
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

*/

