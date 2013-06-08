// Oliver Kullmann, 8.6.2013 (Swansea)
/* Copyright 2013 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Satisfiability/Solvers/TawSolver/plans/general.hpp
  \brief Plans regarding updating and improving the TawSolver


  \todo Timing
  <ul>
   <li> On csltok (2.7 GHz):
   \verbatim
# Old: 588a9cf1d823c5e59090ad20938b1708e6961643

> tawSolver VanDerWaerden_2-3-12_134.cnf
V_VARS: 134, N_CLAUSES: 5172
N_UNITS: 20977611, N_BRANCHES: 1721206, N_BACK: 860593

46.44 46.11 46.01 46.25

> tawSolver VanDerWaerden_2-3-12_135.cnf
V_VARS: 135, N_CLAUSES: 5251
N_UNITS: 21978048, N_BRANCHES: 1790733, N_BACK: 895366

49.33 49.45 49.24 49.67 48.09

# New: fb0212392a795e09e3c1ae7d188ad30d0f4e57cf

> tawSolver VanDerWaerden_2-3-12_134.cnf
45.85 46.08 45.40 46.04

> tawSolver VanDerWaerden_2-3-12_135.cnf
47.71 47.96 47.75 47.22 47.58

# A bit faster: Perhaps due to saving superfluous calls to getrusage.
# It might be, that actually most of the time for these calls is not recorded
# in these times, since it belongs to "sys" (when using "time") ?!
   \endverbatim
   </li>
  </ul>

*/
