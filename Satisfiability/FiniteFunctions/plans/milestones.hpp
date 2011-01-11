// Oliver Kullmann, 19.1.2008 (Swansea)
/* Copyright 2008, 2009, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/FiniteFunctions/plans/milestones.hpp

  \module_version Satisfiability/FiniteFunctions 0.1 (8.1.2011)


  \par Version 0.1.1

  \par
   In FiniteFunctions/plans/QuineMcCluskeySubsumptionHypergraph.hpp the
   following topics are handled:
    - Specification
    - Preparations for computing optimum representations
    - Add option to generate statistics files
    - Standardise name of prime implicates file : DONE
    - Write application tests : DONE


  \par Version 0.1.2

  \par
   In OKlib/Satisfiability/FiniteFunctions/plans/QuineMcCluskey.hpp the
   following topics are handled:
    - Understanding the costs of the current implementation


  \par Version 0.1.3

  \par
   In OKlib/Satisfiability/FiniteFunctions/plans/QuineMcCluskey.hpp the
   following topics are handled:
    - Running only through the levels (via new hash function)

  \par
   In FiniteFunctions/plans/QuineMcCluskeySubsumptionHypergraph.hpp the
   following topics are handled:
    - Improving efficiency


  \par Version 0.1.4

  \par
   In OKlib/Satisfiability/FiniteFunctions/plans/QuineMcCluskey.hpp the
   following topics are handled:
    - Various versions for different values of NUMBER_VARIABLES


  \par Version 0.1.5

  \par
   In OKlib/Satisfiability/FiniteFunctions/plans/QuineMcCluskey.hpp the
   following topics are handled:
    - Docus


  \par Version 0.1.6

  \par
   In OKlib/Satisfiability/FiniteFunctions/plans/QuineMcCluskey.hpp the
   following topics are handled:
    - Application tests


  \par Version 0.1.7

  \par
   In OKlib/Satisfiability/FiniteFunctions/plans/QuineMcCluskey.hpp the
   following topics are handled:
    - Maxima level


  \par Version 0.1.8

  \par
   In OKlib/Satisfiability/FiniteFunctions/plans/QuineMcCluskey.hpp the
   following topics are handled:
    - Precise definitions, and basic types


  \par Version 0.1.9

  \par
   In OKlib/Satisfiability/FiniteFunctions/plans/QuineMcCluskey.hpp the
   following topics are handled:
    - Input and output


  \par Version 0.1.10

  \par
   In OKlib/Satisfiability/FiniteFunctions/plans/QuineMcCluskey.hpp the
   following topics are handled:
    - Extend unit tests


  \par Version 0.1.11

  \par
   In OKlib/Satisfiability/FiniteFunctions/plans/QuineMcCluskey.hpp the
   following topics are handled:
    - Preparations for optimisation


  \par Version 0.1.12

  \par
   In OKlib/Satisfiability/FiniteFunctions/plans/QuineMcCluskey.hpp the
   following topics are handled:
    - Make QuineMcCluskey read from standard input


  \par Version 0.2

  \par
   Version 1.0 of QuineMcCluskey, fully specified, fully functional, fully
   tested.


  \par Version 0.2.1

  \par
   In OKlib/Satisfiability/FiniteFunctions/plans/general.hpp the
   following topics are handled:
    - Links and plans
    - Prime implicants and implicates

  \par
   In OKlib/Satisfiability/FiniteFunctions/plans/QuineMcCluskey.hpp the
   following topics are handled:
    - Quine/McCluskey in general


  \par Version 0.2.2

  \par
   In OKlib/Satisfiability/FiniteFunctions/plans/QuineMcCluskey.hpp the
   following topics are handled:
    - Performance analysis and code analysis


  \par Version 0.2.3

  
  \par
   In OKlib/Satisfiability/FiniteFunctions/plans/QuineMcCluskey.hpp the
   following topics are handled:
    - Fixed number of variables


  \par Version 0.2.4

  \par
   In OKlib/Satisfiability/FiniteFunctions/plans/QuineMcCluskey.hpp the
   following topics are handled:
    - pow3 should be replaced by an array computed at compile-time


  \par Version 0.2.5

  \par
   In OKlib/Satisfiability/FiniteFunctions/plans/QuineMcCluskey.hpp the
   following topics are handled:
    - Improvements of the implementation


  \par Version 0.3

  \par
   Version 2.0 of QuineMcCluskey.cpp is reached, establishing an optimised
   and highly efficient tool.


  \par Version 0.3.1

  \par
   In OKlib/Satisfiability/FiniteFunctions/plans/QuineMcCluskey.hpp the
   following topics are handled:
    - Parallelisation


  \par Version 0.4

  \par
   Tools for finding minimum CNF/DNF representations have been established,
   for the situation where the full truth table is given. And computing
   all prime clauses in this situation can be parallelised.



  -------------------------------------------------------------------------------------------------------------------------------------

  \par Version history

   - 0.0.3 : 19.1.2008; initial version (several plans).
   - 0.0.4 : 22.5.2009; established working version of QuineMcCluskey algorithm, and some further plans.
   - 0.0.5 : 1.6.2009; tidied code further and added doxygen documentation.
   - 0.0.6 : 19.8.2009; further planning, further improvement in code quality, improved documentation, further improvements regardings the build system.
   - 0.1  : 8.1.2011; first overhaul of Quine-McCluskey-implementation, computation of subsumption-hypergraph, and further plans.

*/

