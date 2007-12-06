// Oliver Kullmann, 22.5.2007 (Swansea)
/* Copyright 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file ProofSystems/Resolution/plans/Reductions.hpp
  \brief Plans for resolution reductions

  Here we handle reductions which have to do with adding resolvents,
  while the partial-assignment based approaches are handled in module
  FailedLiteralReduction (see plans/FailedLiteralReduction.hpp) and,
  for the general case, in module KLevelForcedAssignments (see
  plans/KLevelForcedAssignments.hpp).


  \todo Implement the reductions from the Kullmann/Luckhardt-preprints.


  \todo Implement bounded resolution.


  \todo All kinds of binary-clause reasoning are handled by module
  FailedLiteralReduction, or?

*/

