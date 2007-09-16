// Oliver Kullmann, 4.3.2001 (Toronto)
/* Copyright 2001 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file OKsolver/SAT2002/lokalesLernen.h
  \brief Interface for local learning
*/

                       /* OKsolver; 5.3.1998 */

/* Autor: Oliver Kullmann, Universitaet Frankfurt am Main, Germany */
/* ab Februar 1999: University of Toronto, Computer Science Department */

/* Angelegt: 4.3.2001 */


#ifndef GELESENLOKALESLERNEN
#define GELESENLOKALESLERNEN

#ifdef LOKALLERNEN

#include "VarLitKlm.h"
#include "Reduktion.h"
#include "Belegung.h"


typedef struct marke {
  Pfadinfo *Tiefe;
  NKL NK;
  struct marke *naechste;
  struct marke *vorige;
} Marken;

extern void InitlokalesLernen(void);

extern void erzeugeNK(LIT l, Stapeleintrag *spneu, Stapeleintrag *sp);

extern void erzeugeFANK(LIT l, LIT *A, LIT *E);


extern void eintragenTiefe(void);

extern void eintragenNK(void);

extern Marken *Markierung(void);

extern void Rueckgaengigmachung(Marken *Z);


extern void AufraeumenlokalesLernen(void);

extern void NullsetzenRKNK(void);

#endif

#endif
