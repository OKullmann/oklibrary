// Oliver Kullmann, 3.2.2001 (Toronto)
/* Copyright 2001 - 2007, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Solvers/OKsolver/SAT2002/Filter.h
  \brief Header file for the variable-"filtering" function, used for selecting the branching variable
*/

                       /* OKsolver; 5.3.1998 */

/* Autor: Oliver Kullmann, Universitaet Frankfurt am Main, Germany */
/* ab Februar 1999: University of Toronto, Computer Science Department */

/* Angelegt: 3.2.2001 */

#ifndef GELESENFILTER_hTrEw09u
#define GELESENFILTER_hTrEw09u

#include "VarLitKlm.h"
/* #include "Klauselmenge.h" */

extern void Filter(VAR v);
extern void InitFilter();

extern bool erfuellt;
extern VZ Zweig;
extern bool Wahl;
extern bool Single;
extern bool reduziert;
extern unsigned int Schalter;
extern unsigned int DeltaN[2][2];
extern unsigned int LaP[2][2];

#ifndef BAUMRES
typedef LIT StapeleintragF;
#else
typedef struct {
  LIT l;
  KLN k;
} StapeleintragF;
#endif
typedef StapeleintragF* StapeleintragFZ;

extern StapeleintragFZ Huelle[2][2];

extern int* DeltaK[2][2];
extern unsigned int* NeuK[2][2 ];
extern unsigned int* LaAnzK[2][2];

#endif
