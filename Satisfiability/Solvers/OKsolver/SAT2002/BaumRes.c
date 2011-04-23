// Oliver Kullmann, 16.3.2001 (Toronto)
/* Copyright 2001 - 2007, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file OKsolver/SAT2002/BaumRes.c
  \brief Implementations of the functions for tree pruning
*/

#include <stdlib.h>
#include <string.h> /* fuer C++ (memset; 14.8.2001) */
#include <assert.h>

#include "OK.h"
#include "VarLitKlm.h"
#include "Parameter.h"
#include "Speicher.h"
#include "Belegung.h"


#ifdef BAUMRES

/* ------------------------------------------------------------------------- */


/* Variablen */

VarMenge aktrelV;
VarMenge aktrelV0;
/* Die Implementation verlangt, dass unmittelbar nach aktrelV die */
/* Variable aktrelV0 ansetzt. */


/* ------------------------------------------------------------------------- */


size_t BedarfBaumResV() {
  return 2 * GroesseVarMenge;
}

void *BaumResV(void* Z) {
  aktrelV = (VarMenge) Z; Z = (void *) (aktrelV + ANZWORTE);
  aktrelV0 = (VarMenge) Z; Z = (void *) (aktrelV0 + ANZWORTE);
  return Z;
}


void AufraeumenBaumRes() {
  if (Pfad != NULL) {
    Pfadinfo* Z = Pfad;
    for (unsigned int i = 0; i < N; ++Z, ++i) {
      assert(Z);
      free(Z -> M);
    }
    Pfad = NULL;
  }
}

/* ------------------------------------------------------------------------- */


__inline__ bool enthalten (const VAR v) {
  return ((*(VPosition(v)) & VMaske(v)) != 0UL);
}

__inline__ static void hinzufuegen (VarMenge M) {
  for (ZWort Z = aktrelV; Z != aktrelV0; ++Z, ++M)
    *Z |= *M;
}

__inline__ void hinzufuegenKl (const KLN k) {
#ifndef LITTAB
  hinzufuegen(VarK(k));
#else
  VarMaske* z = ersteVK(k);
  for (unsigned int i = UrLaenge(k); i > 0; ++z, --i) {
    assert(z -> Position);
    *(z -> Position) |= (z -> Maske);
  }
#endif
}

__inline__ void setzenKl (const KLN k) {
#ifndef LITTAB
  memcpy((void *) aktrelV, (void *)(VarK(k)), GroesseVarMenge);
#else
  memset((void *) aktrelV, 0, GroesseVarMenge);
  hinzufuegenKl(k);
#endif
}

__inline__ void relVhinzufuegen () {
  const KLN k = Tiefe -> k;
  if (k != NULL) hinzufuegenKl(k); 
  else hinzufuegen(Tiefe -> M);
}

__inline__ void relVMhinzufuegen () {
  hinzufuegen(Tiefe -> M);
}


/* die naechste Operation ist wohl die am haeufigsten benutzte */

__inline__ void Kln_eintragen_relV(const KLN k) {
  Tiefe -> k = k;
}

__inline__ void aktV_eintragen_relV() {
  if (Tiefe -> M == NULL)
    Tiefe -> M = (VarMenge) xmalloc(GroesseVarMenge);
  memcpy((void *)(Tiefe -> M), (void *) aktrelV, GroesseVarMenge);
} 

__inline__ void aktV_volleintragen_relV() {
  Tiefe -> k = NULL;
  if (Tiefe -> M == NULL)
    Tiefe -> M = (VarMenge) xmalloc(GroesseVarMenge);
  memcpy((void *)(Tiefe -> M), (void *) aktrelV, GroesseVarMenge);
}

__inline__ void aktV_speichern() {
  memcpy((void *) aktrelV0, (void *) aktrelV, GroesseVarMenge);
}

__inline__ void hinzufuegenS() {
  hinzufuegen(aktrelV0);
}

#endif

