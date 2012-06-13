// Oliver Kullmann, 17.1.2011 (Swansea)
/* Copyright 2011, 2012 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/Reductions/Bases/plans/milestones.hpp

  \module_version Satisfiability/Reductions/Bases 0.0.9 (21.1.2011)


  \par Version 0.1

  \par
   In Satisfiability/Reductions/Bases/plans/general.hpp the following topics
   are handled:
    - RandomRUcpBases defect : DONE
    - RandomRUcpBases sorts clauses in ascending order of clause-length : DONE

  \par
   In Satisfiability/Reductions/Bases/plans/UcpBase.hpp the following topics
   are handled:
    - Testcase Intransitive2 fails : DONE
    - Computing r_1-bases for a set of prime implicates
    - Shuffling and sorting
    - Random r_1-bases
    - Random shuffling
    - Random number generation

  \par
   In Satisfiability/Reductions/Bases/RUcpGen.cpp the following topics
   are handled:
    - Update application tests


  \par Version 0.1.1

  \par
   In Satisfiability/Reductions/Bases/plans/general.hpp the following topics
   are handled:
    - Docus for RUcpGen
    - Add application tests for RandomUcpBases

  \par
   In Satisfiability/Reductions/Bases/plans/UcpBase.hpp the following topics
   are handled:
    - Improve efficiency of Bases/RUcpGen.cpp


   \par Version 0.1.2

  \par
   In Satisfiability/Reductions/Bases/plans/UcpBase.hpp the following topics
   are handled:
    - Improve efficiency of Bases/RUcpBase.cpp


  \par Version 0.1.3

  \par
   In Satisfiability/Reductions/Bases/plans/UcpBase.hpp the following topics
   are handled:
    - Further improve implementation of Bases/RUcpBase.cpp


  \par Version 0.1.4

  \par
   In Satisfiability/Reductions/Bases/plans/UcpBase.hpp the following topics
   are handled:
    - Further improve implementation of Bases/RUcpGen.cpp


  \par Version 0.2

  \par
   In Satisfiability/Reductions/Bases/plans/UcpBase.hpp the following topics
   are handled:
    - Create unit-tests for Bases/RUcpBase.cpp


  \par Version 0.2.1

  \par
   In Satisfiability/Reductions/Bases/RUcpGen.cpp the following topics
   are handled:
    - Create unit-tests for Bases/RUcpGen.cpp


  \par Version 0.3

  \par
   In Satisfiability/Reductions/Bases/plans/general.hpp the following topics
   are handled:
    - The notion of r-bases
    - r_0-base

  \par
   Further milestones are created.



  -------------------------------------------------------------------------------------------------------------------------------------

  \par Version history

   - 0.0.7 : 17.1.2011; initial version number (first implementation of r_1-base-sampling).
   - 0.0.8 : 20.1.2011; Added application tests for RUcpGen.
   - 0.0.9 : 21.1.2011; Added application tests for RUcpBase and added proof of correctness.

*/

