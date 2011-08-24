// Oliver Kullmann, 3.2.2001 (Toronto)
/* Copyright 2001 - 2007, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file OKsolver/SAT2002/Belegung.h
  \brief Header file with definitions for assignment handling
*/

                       /* OKsolver; 5.3.1998 */

/* Autor: Oliver Kullmann, Universitaet Frankfurt am Main, Germany */
/* ab Februar 1999: University of Toronto, Computer Science Department */

/* Erzeugung: 3.2.2001 */


#ifndef GELESENBELEGUNG_jqoqpC5
#define GELESENBELEGUNG_jqoqpC5

#include "OK.h"
#include "VarLitKlm.h"

/* -------------------------------------------------------------------------------- */


/* Die Belegung, die von der Eingabe zur aktuellen Klauselmenge fuehrt */

#ifndef BAUMRES
typedef LIT Pfadinfo;
#else
typedef struct {
  LIT l;
  KLN k;
  VarMenge M;
} Pfadinfo;
#endif

extern Pfadinfo* Tiefe;

extern Pfadinfo* Pfad;
/* extern LIT Pfad[MAXN]; */
/* F_{aktuell} = < Pfad[i] -> 0 >_{0 <= i < Tiefe - Pfad} * F_{Eingabe} */


/* -------------------------------------------------------------------------------- */

extern LIT PfadLit();
extern void LiteinPfad(LIT x);

/* -------------------------------------------------------------------------------- */


extern void belege(LIT);
#ifdef BAUMRES
extern void belege_VK(LIT, KLN);
#endif
extern void rebelege(LIT);
#ifdef BAUMRES
extern bool rebelege_Verz(LIT);
#endif

#endif
