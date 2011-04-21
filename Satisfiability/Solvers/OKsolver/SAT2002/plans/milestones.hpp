// Oliver Kullmann, 18.8.2007 (Swansea)
/* Copyright 2007, 2008, 2009, 2010, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Solvers/OKsolver/SAT2002/plans/milestones.hpp
  
  \module_version OKsolver/SAT2002 0.9.6 (15.8.2009)


  \par Version 0.9.7

  \par
   In Solvers/OKsolver/SAT2002/plans/general.hpp the following topics
   are handled:
    - The basic idea
    - Computing the sub-problems


  \par Version 0.9.8

  \par
   In Solvers/OKsolver/SAT2002/plans/general.hpp the following topics
   are handled:
    - Computing a splitting tree


  \par Version 0.9.9

  \par
   In Solvers/OKsolver/SAT2002/plans/general.hpp the following topics
   are handled:
    - False counting of initial unit-clause eliminations
    - Improve the Dimacs-output


  \par Version 0.9.10

  \par
   In Solvers/OKsolver/SAT2002/plans/general.hpp the following topics
   are handled:
    - Output to files


  
\par Version 0.9.11

  \par
   In Solvers/OKsolver/SAT2002/plans/general.hpp the following topics
   are handled:
    - Differences


  \par Version 0.9.12

  \par
   In Solvers/OKsolver/SAT2002/plans/general.hpp the following topics
   are handled:
    - OUTPUTTREEDATAXML


  \par Version 0.9.13

  \par
   In Solvers/OKsolver/SAT2002/plans/general.hpp the following topics
   are handled:
    - Documentation problems
    - Language standards


  \par Version 0.9.14

  \par
   In Solvers/OKsolver/SAT2002/plans/general.hpp the following topics
   are handled:
    - Buildsystem


  \par Version 0.9.15

  \par
   In Solvers/OKsolver/SAT2002/plans/general.hpp the following topics
   are handled:
    - Complete the help facilities of the OKsolver


  \par Version 0.9.16

  \par
   In Solvers/OKsolver/SAT2002/plans/general.hpp the following topics
   are handled:
    - Write docus-pages


  \par Version 1.0

  \par
   Create a tag, stating that the basically unaltered original OKsolver
   has now been made fully available.


  \par Version 1.0.1

  \par
   In Solvers/OKsolver/SAT2002/plans/general.hpp the following topics
   are handled:
    - Add doxygen-documentation
    - Eliminate old history in code


  \par Version 1.0.2

  \par
   In Solvers/OKsolver/SAT2002/plans/general.hpp the following topics
   are handled:
    - Create systematic application tests
  

  \par Version 1.0.3

  \par
   In Solvers/OKsolver/SAT2002/plans/general.hpp the following topics
   are handled:
    - Add asserts throughout


  \par Version 1.0.4

  \par
   In Solvers/OKsolver/SAT2002/plans/general.hpp the following topics
   are handled:
    - Investigate unit-testing


  \par Version 1.0.5

  \par
   In Solvers/OKsolver/SAT2002/plans/general.hpp the following topics
   are handled:
    - Correct computation of basic statistics


  \par Version 1.0.6

  \par
   In Solvers/OKsolver/SAT2002/plans/general.hpp the following topics
   are handled:
    - Apply Valgrind
    - Apply code analysis tools (like Splint)


  \par Version 1.1

  \par
   The fully tested and specified original OKsolver (parallel in C and
   in Maxima).


  \par Version 1.2.1

  \par
   In Solvers/OKsolver/SAT2002/plans/general.hpp the following topics
   are handled:
    - Improve statistics


  \par Version 1.2.2

  \par
   In Solvers/OKsolver/SAT2002/plans/general.hpp the following topics
   are handled:
    - Declare variable as close to their first usage as possible


  \par Version 1.2.3

  \par
   In Solvers/OKsolver/SAT2002/plans/general.hpp the following topics
   are handled:
    - Use const-qualification


  \par Version 1.2.4

  \par
   In Solvers/OKsolver/SAT2002/plans/general.hpp the following topics
   are handled:
    - Use restrict-qualification


  \par Version 1.2.5

  \par
   In Solvers/OKsolver/SAT2002/plans/general.hpp the following topics
   are handled:
    - Elimination of compile-time options


  \par Version 1.3

  \par
   The fully cleaned-up and documented original OKsolver.


  \par Version 1.3.1

  \par
   In Solvers/OKsolver/SAT2002/plans/general.hpp the following topics
   are handled:
    - Output of clause-sets


  \par Version 1.3.2

  \par
   In Solvers/OKsolver/SAT2002/plans/general.hpp the following topics
   are handled:
    - Inefficiency on 64-bit platforms


  \par Version 1.4

  \par
   Improved usability.


  \par Version 1.5

  \par
   Make nearly everything also available at the Maxima-level, and make
   sure that both levels fully coincide.


  \par Version 1.5.1

  \par
   In Solvers/OKsolver/SAT2002/plans/general.hpp the following topics
   are handled:
    - Extend statistics


  \par Version 1.5.2

  \par
   In Solvers/OKsolver/SAT2002/plans/general.hpp the following topics
   are handled:
    - Output intermediate results


  \par Version 1.5.3

  \par
   In Solvers/OKsolver/SAT2002/plans/general.hpp the following topics
   are handled:
    - More influence on heuristics


  \par Version 1.5.4

  \par
   In Solvers/OKsolver/SAT2002/plans/general.hpp the following topics
   are handled:
    - Start planning on evaluating and optimising heuristics


  \par Version 1.6

  \par
   Extend statistics, extract forced assignments, and make evaluation and
   optimisation of heuristics possible; furthermore extend the possibilities
   to influence heuristics.


  \par Version 1.7

  \par
   In Solvers/OKsolver/SAT2002/plans/general.hpp the following topics
   are handled:
    - Apply time-measurements


  \par Version 1.8

  \par
   We have timing-date available.


  \par Version 1.8.1

  \par
   In Solvers/OKsolver/SAT2002/plans/general.hpp the following topics
   are handled:
    - Optimising the code


  \par Version 1.9

  \par
   The code has been optimised (w.r.t. speed; using also a profiler etc.)


  \par Version 1.10

  \par
   Document the extensive database-data.


  \par Version 2.0

  \par
   Now the original OKsolver is also fully reflected and fully specified.


  \par Version 2.1

  \par
   In Solvers/OKsolver/SAT2002/plans/general.hpp the following topics
   are handled:
    - Extension by cardinality constraints


  \par Version 2.2

  \par
   In Solvers/OKsolver/SAT2002/plans/general.hpp the following topics
   are handled:
    - Improved Delta(n)-distance
    - Enable finding all solutions

  \par
   Likely the above extensions should not be included in the (original)
   OKsolver-2002, but they should be moved to the generic form of
   OKsolver-2002 --- thus planning should be intensified here.



  -------------------------------------------------------------------------------------------------------------------------------------

  \par Version history

   - 0.9   : 18.8.2007; initial version number (the old OKsolver mainly unchanged since SAT2002; a bug found by Marijn Heule showed up with newer versions of gcc)
   - 0.9.1 : 20.8.2007; two bugs corrected, linking behaviour corrected, basic code maintenance performed, and basic application tests written and executed. The OKsolver in the standard version appears now to be bug-free.
   - 0.9.2 : 14.9.2007; basic tests for main variants established. Now also the main variants appear to be bug-free.
   - 0.9.3 : 22.9.2007; completed planning for counting all satisfying assignments (implementation postponed), and on the way also code and documentation maintenance.
   - 0.9.4 : 16.9.2008; various "spontaneous" workings: improved the build, extended plans, created application tests, basic code overhauls, using SIGINT instead of SIGUSR2, better documentation, corrected output of statistics.
   - 0.9.5 : 22.7.2009; extended plans, more documentation, provided combination with minisat2-preprocessor, improved timing and monitoring output.
   - 0.9.6 : 15.8.2009; improved monitoring output, improved documentation, some local code improvements, more plans on extensions.

*/

