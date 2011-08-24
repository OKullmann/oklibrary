// Oliver Kullmann, 3.2.2001 (Toronto)
/* Copyright 2001 - 2007, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file OKsolver/SAT2002/Belegung.c
  \brief Implementations for assignment handling
*/

                       /* OKsolver; 5.3.1998 */

/* Autor: Oliver Kullmann, Universitaet Frankfurt am Main, Germany */
/* ab Februar 1999: University of Toronto, Computer Science Department */

/* Erzeugung: 3.2.2001 */

/* Einheit: Belegung */

#include <stdlib.h>

#include "OK.h"
#include "VarLitKlm.h"
#include "BaumRes.h"
#include "Belegung.h"

/* -------------------------------------------------------------------------------- */

Pfadinfo *Tiefe;

Pfadinfo *Pfad = NULL; 
/* "= NULL" ist zur Information fuer BaumRes, dass Pfad noch nicht angelegt */
/* wurde; */


/* -------------------------------------------------------------------------------- */

size_t BedarfBelegungV( void )
{
  return N * sizeof(Pfadinfo);
}

void *BelegungV(void *Z)
{
  Pfad = (Pfadinfo *) Z; Z = (void *) (Pfad + N);
  Tiefe = Pfad;
#ifdef BAUMRES
  {
    Pfadinfo *z; unsigned int i;
    for (i = 0, z = Pfad; i < N; i++, z++)
      z -> M = NULL;
  }
#endif
  return Z;
}

  
/* -------------------------------------------------------------------------------- */

__inline__ LIT PfadLit(void)
{
#ifndef BAUMRES
  return *Tiefe;
#else
  return Tiefe -> l;
#endif
}

__inline__ void LiteinPfad(LIT x)
{
#ifndef BAUMRES
  *Tiefe = x;
#else
  Tiefe -> l = x;
#endif
}

/* -------------------------------------------------------------------------------- */


/* Funktion zur Belegung einer Variablen */

//! processing x -> 0
__inline__ void belege(const LIT x) {
/* Mit Aktualisierung von "LaenK" und Eintrag in "Pfad" und "belegt". */
/* Vorbedingungen:  Es entsteht nicht die leere Klausel. */
  /* entferne alle x-Vorkommen aus ihren Klauseln: */
  for (LITV y = erstesVork(x); echtesVork(y, x); y = naechstesVork(y)) {
    loeseLK(y);
    LaengeM1(KlnVk(y));
  }
  /* fuer Klauseln C mit (non x) entferne alle anderen y in C aus ihren Vorkommenslisten: */
  const LIT kx = Komp(x);
  for (LITV y = erstesVork(kx); echtesVork(y, kx); y = naechstesVork(y))
    for (LITV z = naechstesVorkK(y); z != y; z = naechstesVorkK(z))
      loeseLv(z);
  LiteinPfad(x); ++Tiefe;
  const VAR v = Var(x);
  loeseV(v);
  setzenbelegt(v, true);
}


#ifdef BAUMRES

__inline__ void belege_VK(LIT x, KLN K)

/* Hier wird noch die fuer die 1-Klauseln-Erzeugung relevante Klausel */
/* in relVar eingetragen. */
/* "belege_VK" wird nicht fuer die Verzweigungsvariable verwendet. */

{
  LITV y, z;
  LIT kx;
  VAR v;

  /* entferne alle x-Vorkommen aus ihren Klauseln */

  for (y = erstesVork(x); echtesVork(y, x); y = naechstesVork(y))
    {
      loeseLK(y);
      LaengeM1(KlnVk(y));
    }

  /* fuer Klauseln C mit (non x) in C entferne alle anderen y in C aus */
  /* ihren Vorkommenslisten */

  for (y = erstesVork(kx = Komp(x)); echtesVork(y, kx); y = naechstesVork(y))
    for (z = naechstesVorkK(y); z != y; z = naechstesVorkK(z))
      loeseLv(z);

  LiteinPfad(x);
  loeseV(v = Var(x));
  setzenbelegt(v, true);

  Kln_eintragen_relV(K);
  Tiefe++;
}

#endif


/* -------------------------------------------------------------------------------- */

/* Funktion zur Umkehrung einer Belegung */


__inline__ void rebelege(LIT x)

/* Macht x -> 0 wieder rueckgaengig */
/* ("belegt" wird wieder rueckgesetzt). */
/* Ist BAUMRES gesetzt, so wird aktrelV aktualisiert. */

{
  LITV y, z;
  LIT kx;
  VAR v;

  for (y = erstesVork(x); echtesVork(y, x); y = naechstesVork(y))
    {
      bindeLK(y);
      LaengeP1(KlnVk(y));
    }

  for (y = erstesVork(kx = Komp(x)); echtesVork(y, kx); y = naechstesVork(y))
    for (z = naechstesVorkK(y); z != y; z = naechstesVorkK(z))
      bindeLv(z);

  bindeV(v = Var(x));
  setzenbelegt(v, false);

#ifdef BAUMRES
  if (enthalten(v))
    relVhinzufuegen();
#endif

}


#ifdef BAUMRES

__inline__ bool rebelege_Verz(LIT x)

/* Macht x -> 0 wieder rueckgaengig fuer eine Verzweigungsvariable. */

/* rebelege_Verz = true falls verzweigt werden muss. */

{
  LITV y, z;
  LIT kx;
  VAR v;

  for (y = erstesVork(x); echtesVork(y, x); y = naechstesVork(y))
    {
      bindeLK(y);
      LaengeP1(KlnVk(y));
    }

  for (y = erstesVork(kx = Komp(x)); echtesVork(y, kx); y = naechstesVork(y))
    for (z = naechstesVorkK(y); z != y; z = naechstesVorkK(z))
      bindeLv(z);

  bindeV(v = Var(x));
  setzenbelegt(v, false);

  return enthalten(v);

}

#endif

