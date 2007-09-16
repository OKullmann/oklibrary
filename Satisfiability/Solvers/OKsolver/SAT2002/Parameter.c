// Oliver Kullmann, 8.2.2001 (Toronto)
/* Copyright 2001 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file OKsolver/SAT2002/Parameter.c
  \brief Definitions of parameters needed for tree pruning
*/

                       /* OKsolver; 5.3.1998 */

/* Autor: Oliver Kullmann, Universitaet Frankfurt am Main, Germany */
/* ab Februar 1999: University of Toronto, Computer Science Department */

/* Erzeugung: 8.2.2001 */

/* Einheit: Parameter */

#include "OK.h"
#include "Parameter.h"

/* ------------------------------------------------------------------------- */

#ifdef BAUMRES

unsigned int ANZWORTE;
/* Anzahl benoetigter Worte zur Darstellung von Mengen von Var. */

unsigned int GroesseVarMenge;

#endif


/* Ferner wird noch die Anzahl der Ankerplaetze benoetigt: */

/* unsigned int AnzahlAnker; */

/* Und die Position von F innerhalb der Liste der Ankerelemente: */

/* unsigned int PositionAnker; */




/* ------------------------------------------------------------------------- */

void InitParameter( void )
/* benoetigt nur N */
{
/*   AnzahlAnker = 2 * N + 1; */
/*   PositionAnker = N; */

#ifdef BAUMRES
  ANZWORTE = (N - 1) / ((int) BITS) + 1;
  GroesseVarMenge = ANZWORTE * sizeof(Wort);
#endif
  return;
}



