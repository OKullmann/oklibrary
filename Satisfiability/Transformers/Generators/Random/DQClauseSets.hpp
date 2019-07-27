// Oliver Kullmann, 17.7.2019 (Swansea)
/* Copyright 2019 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/* Random dependency-quantified CNF

DESIGN:

At least initially, only one additional parameter to QBRG, in three forms:
  D, +D, -D
which alters the dependencies as given by the quantifier-blocks:

 - +D adds ("to the right") dependencies uniformly.

   The number of open dependencies is
     d := na*ne - #D(quants),
   where #D(qunats) is the number of dependencies given by quantifiers.
   We must have D <= d, and the random choice is
     choose D elements from {1, ..., d},
   which are then mapped to the dependency-pairs, and added
 - -D subtracts ("to the left") dependencies uniformly.

   Here we must have D <= #D(quants), and the random choice is
     choose D elements from {1, ..., #D(quants)},
   which are mapped to the dependency-pairs, and removed.
 - D just deletes all dependencies, and chooses D dependencies uniformly
   (treating all existential variables and all universal variables equally).

   The random choice is
     choose D elements from {1, ..., na*ne},
   and we must have D <= na*ne.

 1. The handling of the dependency-sets should be unified with
    DQCNF/AutarkiesL1.cpp.
 2. Filtering of clauses (rejecting clauses with formal universal
    variables) as with QBRG.

*/

#ifndef DQCLAUSESETS_nMH1OTZkKa
#define DQCLAUSESETS_nMH1OTZkKa

#include <ProgramOptions/Environment.hpp>

#include "Distributions.hpp"
// Guaranteed to be included:
#include "QClauseSets.hpp"

namespace RandGen {

}

#endif
