// Oliver Kullmann, 3.2.2001 (Toronto)
/* Copyright 2001 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Solvers/OKsolver/SAT2002/OKsolver_2002.c
  \brief Complete program of the old OKsolver where everything is included (for
  better inlining)

  No linking needed here. Compile with

  g++ -Wall -Wno-parentheses -Wno-return-type -std=c++11 -DNDEBUG -Ofast -funsafe-loop-optimizations -o OKsolver_2002-fast OKsolver_2002.c
*/

                       /* OKsolver; 5.3.1998 */

/* Autor: Oliver Kullmann, Universitaet Frankfurt am Main, Germany */
/* ab Februar 1999: University of Toronto, Computer Science Department */

/* Angelegt: 3.2.2001 */


#define __include__ include

#include "Parameter.c"
#include "VarLitKlm.c"
#include "BaumRes.c"
#include "Belegung.c"
#include "lokalesLernen.c"
#include "Reduktion.c"
#include "Filter.c"
#include "Abstand_nKaZ.c"
#include "Projektion_Prod.c"
#include "Speicher.c"
#include "OKsolver_2002_lnk.c"
#include "Ausgaben.c"
#include "Einlesen.c"

